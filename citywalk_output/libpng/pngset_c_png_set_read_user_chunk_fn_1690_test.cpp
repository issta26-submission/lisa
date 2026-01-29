// Test suite for png_set_read_user_chunk_fn
// This test targets the focal method inside the provided codebase.
// It uses internal header access (pngpriv.h) to inspect private fields
// and validates both branches of the conditional logic.

#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstdio>


// Ensure C linkage for libpng internal headers
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_fail_count = 0;

static void fail_msg(const char* msg) {
    std::cerr << "Test failure: " << msg << std::endl;
    ++g_fail_count;
}

#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { fail_msg(msg); } } while(0)

#define EXPECT_EQUAL(a, b, msg) \
    do { if(!((a) == (b))) { \
        char buf[256]; snprintf(buf, sizeof(buf), "Expected %s, got %p vs %p", msg, (void*)(a), (void*)(b)); \
        fail_msg(buf); \
    } } while(0)

static int g_callback_calls = 0;
static png_structrp g_callback_ptr = NULL;
static png_voidp g_callback_data = NULL;

// Callback to capture invocation from the png_set_read_user_chunk_fn mechanism
static void test_read_user_chunk_fn(png_structrp ptr, png_unknown_chunkp chunk, png_voidp data) {
    (void)chunk; // unused in test
    g_callback_calls++;
    g_callback_ptr  = ptr;
    g_callback_data = data;
}

// Utility to reset global callback state before tests
static void reset_callback_state() {
    g_callback_calls = 0;
    g_callback_ptr = NULL;
    g_callback_data = NULL;
}

// Test 1: Verify that png_set_read_user_chunk_fn correctly sets both
// read_user_chunk_fn and user_chunk_ptr when png_ptr is valid.
static void test_sets_fn_and_user_ptr_when_ptr_valid() {
    reset_callback_state();

    // Allocate a png_struct instance (as per libpng internal types)
    png_structrp png_ptr = new png_struct;

    // Set with a non-null user chunk function and a test pointer
    png_voidp test_user_ptr = (png_voidp)0xDEADBEEF;
    png_set_read_user_chunk_fn(png_ptr, test_user_ptr, test_read_user_chunk_fn);

    // Validate that the internal fields were updated
    // read_user_chunk_fn should point to our test function
    EXPECT_TRUE(png_ptr->read_user_chunk_fn == test_read_user_chunk_fn,
                "read_user_chunk_fn should be set to test_read_user_chunk_fn");

    // user_chunk_ptr should store the provided user pointer
    EXPECT_EQUAL(png_ptr->user_chunk_ptr, test_user_ptr,
                 "user_chunk_ptr should be set to the provided pointer");

    // Now invoke the stored callback through the function pointer to ensure
    // it matches the expected signature and is callable
    if (png_ptr->read_user_chunk_fn != NULL) {
        // Call with null chunk and data to ensure callback routing works
        png_ptr->read_user_chunk_fn(png_ptr, NULL, NULL);
        // The callback should have been invoked exactly once with the same png_ptr
        EXPECT_TRUE(g_callback_calls == 1,
                    "Callback should be invoked exactly once after direct call");
        EXPECT_TRUE(g_callback_ptr == png_ptr,
                    "Callback should receive the original png_ptr");
        EXPECT_TRUE(g_callback_data == NULL,
                    "Callback data should be NULL as passed");
    } else {
        fail_msg("read_user_chunk_fn unexpectedly NULL after set");
    }

    delete[] (char*)png_ptr; // clean up; casting to char* due to opaque type
}

// Test 2: Verify that png_set_read_user_chunk_fn correctly handles a NULL
// read_user_chunk_fn while still updating user_chunk_ptr.
static void test_sets_only_user_ptr_when_fn_null() {
    // Allocate a new png_struct instance
    png_structrp png_ptr = new png_struct;

    // Update both fields with NULL function and a non-null user_ptr
    png_voidp test_user_ptr = (png_voidp)0xBADC0DE;
    png_set_read_user_chunk_fn(png_ptr, test_user_ptr, NULL);

    // The function should set user_chunk_ptr regardless of read_user_chunk_fn being NULL
    EXPECT_EQUAL(png_ptr->user_chunk_ptr, test_user_ptr,
                 "user_chunk_ptr should be updated even when read_user_chunk_fn is NULL");

    // read_user_chunk_fn should be NULL as provided
    EXPECT_TRUE(png_ptr->read_user_chunk_fn == NULL,
                "read_user_chunk_fn should be NULL when not provided");

    // Cleanup
    delete[] (char*)png_ptr;
}

// Test 3: Null png_ptr should be handled gracefully (no crash)
static void test_null_png_ptr_is_noop() {
    // Call with NULL pointer; should not crash or modify any known state
    // The test ends here; if a crash occurred, the process would terminate prior to this point.
    png_set_read_user_chunk_fn(NULL, (png_voidp)0x1234, test_read_user_chunk_fn);
    // If we reach here, the NULL handling did not crash.
    SUCCEED:
    ;
}

// Helper to indicate a test that always passes (used for readability)
static void SucceedHelper() { }

// Simple pass-through success macro
#define SUCCEED do { } while(0)

// Entry point to execute all tests
int main() {
    std::cout << "Starting PNG set_read_user_chunk_fn unit tests (C++11, no GTest)..." << std::endl;

    test_sets_fn_and_user_ptr_when_ptr_valid();
    test_sets_only_user_ptr_when_fn_null();
    test_null_png_ptr_is_noop();

    if (g_fail_count == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_fail_count << " tests FAILED" << std::endl;
        return 1;
    }
}