/*
Unit test suite for the focal method:
png_chunk_benign_error(png_const_structrp png_ptr, png_const_charp error_message)

Key dependencies (candidate keywords derived from the focal class/docs):
- PNG_FLAG_BENIGN_ERRORS_WARN
- png_ptr->flags
- png_chunk_warning
- png_chunk_error
- PNG_ERROR_TEXT_SUPPORTED (affects PNG_UNUSED usage)
- PNG_UNUSED macro
- png_const_structrp, png_const_charp (stdio-like types used by the function)
This test suite is self-contained (no GTest). It uses simple, non-terminating assertions
to maximize code coverage and execution paths.
*/

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Candidate Keywords (for documentation/reference):
// - PNG_FLAG_BENIGN_ERRORS_WARN
// - png_ptr->flags
// - png_chunk_warning
// - png_chunk_error
// - PNG_ERROR_TEXT_SUPPORTED
// - PNG_UNUSED
// - png_const_structrp, png_const_charp

// Minimal, self-contained type definitions to simulate a tiny subset of libpng
typedef struct png_struct {
    unsigned int flags;
} png_struct;

typedef const png_struct* png_const_structrp;
typedef png_struct* png_structrp;

typedef const char* png_const_charp;
typedef char* png_charp;
typedef unsigned long png_alloc_size_t;

// Macro to simulate libpng's unused-parameter handling when PNG_ERROR_TEXT_SUPPORTED is not provided
#ifndef PNG_ERROR_TEXT_SUPPORTED
#define PNG_UNUSED(x) (void)(x)
#else
#define PNG_UNUSED(x)
#endif

// Flags (subset)
#define PNG_FLAG_BENIGN_ERRORS_WARN 0x01

// Global test state to validate that side-effecting calls happened as expected
static int g_chunk_warning_calls = 0;
static int g_chunk_error_calls = 0;
static std::string g_last_warning_message;
static std::string g_last_error_message;

// Stub implementations of the dependency functions used by the focal method.
// In real code, these would be provided by the library; here we instrument them.

static void png_chunk_warning(png_const_structrp png_ptr, png_const_charp warning_message)
{
    (void)png_ptr; // not used in this test harness
    ++g_chunk_warning_calls;
    g_last_warning_message = warning_message ? warning_message : "";
}

static void png_chunk_error(png_const_structrp png_ptr, png_const_charp error_message)
{
    (void)png_ptr;
    ++g_chunk_error_calls;
    g_last_error_message = error_message ? error_message : "";
}

// The focal method under test, reproduced here for a self-contained unit test.
// It follows the exact structure described in the prompt.
void png_chunk_benign_error(png_const_structrp png_ptr,
    png_const_charp error_message)
{
{
   if ((png_ptr->flags & PNG_FLAG_BENIGN_ERRORS_WARN) != 0)
      png_chunk_warning(png_ptr, error_message);
   else
      png_chunk_error(png_ptr, error_message);
#  ifndef PNG_ERROR_TEXT_SUPPORTED
      PNG_UNUSED(error_message)
#  endif
}
}

// Utility to reset test state between tests
static void reset_test_state()
{
    g_chunk_warning_calls = 0;
    g_chunk_error_calls = 0;
    g_last_warning_message.clear();
    g_last_error_message.clear();
}

// Lightweight test assertion helpers (non-terminating)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << (msg) << " (Condition: " #cond ")\n"; \
    } else { \
        /* Optional: verbose success can be enabled if needed */ \
    } \
} while (false)

static void test_benign_error_warn_true()
{
    // Test that when PNG_FLAG_BENIGN_ERRORS_WARN is set, png_chunk_warning is called
    // and png_chunk_error is not called. Also verify the message passed through.
    reset_test_state();

    png_struct p;
    p.flags = PNG_FLAG_BENIGN_ERRORS_WARN;

    const char* msg = "benign_warning_case";
    png_chunk_benign_error(reinterpret_cast<png_const_structrp>(&p), msg);

    // true branch should be taken: warning called
    EXPECT_TRUE(g_chunk_warning_calls == 1, "png_chunk_warning should be called once when flag is set");
    EXPECT_TRUE(g_chunk_error_calls == 0, "png_chunk_error should NOT be called when flag is set");
    EXPECT_TRUE(g_last_warning_message == std::string(msg), "warning_message should be passed through to png_chunk_warning");
    EXPECT_TRUE(g_last_error_message.empty(), "error_message should be unused (empty) when warning path is taken");
}

static void test_benign_error_warn_false()
{
    // Test that when PNG_FLAG_BENIGN_ERRORS_WARN is not set, png_chunk_error is called
    reset_test_state();

    png_struct p;
    p.flags = 0; // clear the flag

    const char* msg = "benign_error_case";
    png_chunk_benign_error(reinterpret_cast<png_const_structrp>(&p), msg);

    // false branch should be taken: error called
    EXPECT_TRUE(g_chunk_warning_calls == 0, "png_chunk_warning should NOT be called when flag is not set");
    EXPECT_TRUE(g_chunk_error_calls == 1, "png_chunk_error should be called once when flag is not set");
    EXPECT_TRUE(g_last_error_message == std::string(msg), "error_message should be passed through to png_chunk_error");
    EXPECT_TRUE(g_last_warning_message.empty(), "warning_message should be unused (empty) when error path is taken");
}

// A brief helper to summarize test results
static void summarize_results()
{
    std::cout << "\nTest Summary: " << g_tests_run << " tests run, "
              << g_tests_failed << " failures.\n";
    if (g_tests_failed == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << g_tests_failed << " tests failed.\n";
    }
}

int main()
{
    std::cout << "Starting unit tests for png_chunk_benign_error (self-contained harness)\n";

    test_benign_error_warn_true();
    test_benign_error_warn_false();

    summarize_results();

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}