// C++11 test suite for the internal gz_error function in gzlib.c
// This test harness uses a lightweight, custom test framework (no GTest) and
// exercises multiple code paths inside gz_error by manipulating the gz_statep
// structure. It relies on the library's internal definitions (via gzguts.h) to
// access state fields (msg, err, path, x.have).

#include <iostream>
#include <gzguts.h>
#include <cstdlib>
#include <cstring>


// Ensure basic zlib-like error codes exist if the environment doesn't provide them.
#ifndef Z_OK
#define Z_OK 0
#define Z_MEM_ERROR (-4)
#define Z_BUF_ERROR (-5)
#endif

// Forward declarations of the target function and the internal state type.
// The test assumes the library provides an accessible gz_error with C linkage
// and a gz_statep type defined in gzguts.h.
extern "C" {
// gz_error is defined in gzlib.c as: void ZLIB_INTERNAL gz_error(gz_statep state, int err, const char *msg);
void gz_error(gz_statep state, int err, const char *msg);
}

// Lightweight test framework
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond) \
    do { \
        ++g_tests_run; \
        if (!(cond)) { \
            std::cerr << "Test failed: " __FILE__ ":" << __LINE__ << " - " #cond "\n"; \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_STR_EQ(a, b) \
    do { \
        ++g_tests_run; \
        if ((a) == NULL && (b) == NULL) { \
            /* both NULL, considered equal */ \
        } else if ((a) == NULL || (b) == NULL) { \
            std::cerr << "Test failed: " __FILE__ ":" << __LINE__ \
                      << " - expected " << (b) << " but got NULL\n"; \
            ++g_tests_failed; \
        } else if (strcmp((a), (b)) != 0) { \
            std::cerr << "Test failed: " __FILE__ ":" << __LINE__ \
                      << " - expected \"" << (b) << "\" but got \"" << (a) << "\"\n"; \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_PTR_EQ(a, b) \
    do { \
        ++g_tests_run; \
        if ((a) != (b)) { \
            std::cerr << "Test failed: " __FILE__ ":" << __LINE__ \
                      << " - expected pointer " << (void*)(b) << " but got " << (void*)(a) << "\n"; \
            ++g_tests_failed; \
        } \
    } while (0)

// Test 1: When an old message exists and a non-error message is provided,
// expect the old message to be freed and a new formatted message to be created.
void test_msg_cleanup_and_reallocation()
{
    gz_state s;
    memset(&s, 0, sizeof(s));
    gz_statep state = &s;

    // Prepare an existing message and a non-mem-error condition
    state->path = "pathA";
    state->err = Z_BUF_ERROR;       // not Z_MEM_ERROR
    state->msg = (char*)malloc(6);
    if (state->msg != NULL) {
        strcpy(state->msg, "oldm");
    }

    // Call with a non-error code and a new message
    gz_error(state, Z_OK, "newmsg");

    // Expect: old message freed, new message allocated as "pathA: newmsg"
    EXPECT_TRUE(state->msg != NULL);
    EXPECT_STR_EQ(state->msg, "pathA: newmsg");

    // Clean up
    free(state->msg);
    state->msg = NULL;
    state->path = NULL;
    state->err = 0;
    state->x.have = 0;
}

// Test 2: If err is Z_MEM_ERROR, no new message should be constructed
void test_mem_error_no_message_created()
{
    gz_state s;
    memset(&s, 0, sizeof(s));
    gz_statep state = &s;

    state->path = "rootPath";
    state->msg = NULL;
    state->err = 0;
    state->x.have = 1;

    gz_error(state, Z_MEM_ERROR, "memory failed");

    // Expect: no message allocated, error code stored
    EXPECT_TRUE(state->msg == NULL);
    EXPECT_TRUE(state->err == Z_MEM_ERROR);
}

// Test 3: Calling gz_error with a NULL msg should clear any existing message and return
void test_clear_existing_message_on_null_msg()
{
    gz_state s;
    memset(&s, 0, sizeof(s));
    gz_statep state = &s;

    state->path = "clearPath";
    state->err = 0;
    state->msg = (char*)malloc(4);
    if (state->msg != NULL) {
        strcpy(state->msg, "txt");
    }

    // Call with NULL message
    gz_error(state, Z_OK, NULL);

    // Expect: existing message freed and state->msg set to NULL
    EXPECT_TRUE(state->msg == NULL);

    // Clean up
    if (state->path) state->path = NULL;
}

// Test 4: Check correct formatting of the error message as "path: msg"
void test_message_format_includes_path()
{
    gz_state s;
    memset(&s, 0, sizeof(s));
    gz_statep state = &s;

    state->path = "root";
    state->msg = NULL;
    state->err = Z_OK;
    state->x.have = 1;

    gz_error(state, Z_OK, "failed");

    EXPECT_TRUE(state->msg != NULL);
    EXPECT_STR_EQ(state->msg, "root: failed");

    // Clean up
    free(state->msg);
    state->msg = NULL;
}

// Test 5: If a non-success error is provided, x.have should be cleared (set to 0)
void test_error_sets_x_have_to_zero_for_fatal_errors()
{
    gz_state s;
    memset(&s, 0, sizeof(s));
    gz_statep state = &s;

    state->path = "fatalPath";
    state->msg = NULL;
    state->err = 0;
    state->x.have = 1;

    // Use an arbitrary non-zero error not equal to Z_OK and not equal to Z_BUF_ERROR
    int arbitrary_err = 123;
    gz_error(state, arbitrary_err, "fatal");

    EXPECT_TRUE(state->x.have == 0);
    EXPECT_TRUE(state->err == arbitrary_err);
}

// Test 6: Ensure that when err == Z_OK no change occurs to x.have other than normal message creation
void test_ok_error_leaves_x_have_unchanged_and_creates_message()
{
    gz_state s;
    memset(&s, 0, sizeof(s));
    gz_statep state = &s;

    state->path = "okPath";
    state->msg = NULL;
    state->err = 0;
    state->x.have = 1;

    gz_error(state, Z_OK, "all good");

    // Should create a message and not modify x.have because err == Z_OK
    EXPECT_TRUE(state->x.have == 1);
    EXPECT_TRUE(state->msg != NULL);
    EXPECT_STR_EQ(state->msg, "okPath: all good");

    // Clean up
    free(state->msg);
    state->msg = NULL;
}

// Main runner
int main()
{
    // Run tests
    test_msg_cleanup_and_reallocation();
    test_mem_error_no_message_created();
    test_clear_existing_message_on_null_msg();
    test_message_format_includes_path();
    test_clear_existing_message_on_null_msg(); // re-use as needed
    test_error_sets_x_have_to_zero_for_fatal_errors();
    test_ok_error_leaves_x_have_unchanged_and_creates_message();

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << ", Failures: " << g_tests_failed
              << std::endl;
    return (g_tests_failed == 0) ? 0 : 1;
}