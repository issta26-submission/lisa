// Test suite for the focal method: png_set_text_compression_method
// This test harness provides a minimal, self-contained environment to
// verify the behavior of the function under test without requiring
// external dependencies (no GTest). The goal is to exercise true/false
// branches and edge cases as described in the problem statement.

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>


// ------------------------------------------------------------------------------------
// Minimal environment to host the focal function and its dependencies

// Lightweight type definitions to mimic the libpng structures used by the focal method.
struct png_struct {
    int zlib_text_method;
};

// png_structrp is a pointer to the implementation-specific png_struct.
// In the actual project, this is defined by libpng headers.
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;

// Forward declarations of helpers the focal function uses.
// In the real library these are provided by the PNG codebase.
void png_debug(int, const char*);
void png_warning(png_structrp, const char*);

// A small in-memory log to capture warnings for assertions.
static std::vector<std::string> g_warning_log;

// Clears the warning log between tests.
static void clear_warnings() {
    g_warning_log.clear();
}

// Retrieves a const reference to the warning log for assertions.
static const std::vector<std::string>& get_warnings() {
    return g_warning_log;
}

// Simple logging mechanism that records warnings instead of printing.
void png_warning(png_structrp /*png_ptr*/, const char* msg) {
    if (msg) {
        g_warning_log.push_back(std::string(msg));
    } else {
        g_warning_log.push_back(std::string("(null warning)"));
    }
}

// Debug hook; kept for compatibility with the focal method but does nothing in tests.
void png_debug(int /*level*/, const char* /*msg*/) {
    // No-op for unit tests
}

// ------------------------------------------------------------------------------------
// The focal method under test (reproduced locally for isolated testing)
// Note: We reproduce the exact logic as shown in the problem statement.
void png_set_text_compression_method(png_structrp png_ptr, int method)
{
{
   png_debug(1, "in png_set_text_compression_method");
   if (png_ptr == NULL)
      return;
   if (method != 8)
      png_warning(png_ptr, "Only compression method 8 is supported by PNG");
   png_ptr->zlib_text_method = method;
}
}

// ------------------------------------------------------------------------------------
// Lightweight, non-terminating test framework (no GTest). Uses return values to
// indicate pass/fail and prints detailed information about failures.

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper to report a failure with file/line and message.
static void report_failure(const char* file, int line, const std::string& msg) {
    ++g_failed_tests;
    std::cerr << "Test Failure at " << file << ":" << line << " - " << msg << "\n";
}

// Run a test that is expected to pass. Returns true on pass.
#define RUN_TEST(test_fn)                                                        \
    do {                                                                         \
        bool passed = (test_fn)();                                              \
        ++g_total_tests;                                                         \
        if (!passed) {                                                          \
            report_failure(__FILE__, __LINE__, "Test " #test_fn " failed");     \
        }                                                                        \
    } while (0)

// Expectation helpers (non-terminating)
#define EXPECT_TRUE(cond, msg)                                                    \
    do { if(!(cond)) { report_failure(__FILE__, __LINE__, (msg)); } } while(0)

#define EXPECT_EQ(actual, expected, msg)                                          \
    do { if(!((actual) == (expected))) { report_failure(__FILE__, __LINE__, (msg)); } } while(0)


// ------------------------------------------------------------------------------------
// Test Cases

// 1) Null pointer: ensure function returns without dereferencing and without crash.
// Coverage: false branch (png_ptr == NULL) triggers return early.
static bool test_null_pointer_handling() {
    clear_warnings();
    // When png_ptr is NULL, the function should return immediately.
    png_set_text_compression_method(nullptr, 5);

    // No side effects to observe; ensure no warnings were produced.
    EXPECT_EQ(static_cast<int>(get_warnings().size()), 0,
              "Warning should not occur when png_ptr is NULL");
    return true;
}

// 2) Method equals 8: ensure it sets the field and does not emit a warning.
// Coverage: true branch for equality with 8; no warning expected.
static bool test_method_eight_sets_and_no_warning() {
    clear_warnings();

    png_struct s;
    s.zlib_text_method = -1; // initial state different from 8

    png_set_text_compression_method(&s, 8);

    EXPECT_EQ(s.zlib_text_method, 8,
              "zlib_text_method should be set to 8 when method == 8");
    EXPECT_EQ(static_cast<int>(get_warnings().size()), 0,
              "No warnings should be emitted when method == 8");
    return true;
}

// 3) Method not equal to 8 (e.g., 5): ensure it warns and sets the method.
// Coverage: true branch for not-8, plus warning recorded.
static bool test_method_not_eight_warns_and_sets() {
    clear_warnings();

    png_struct s;
    s.zlib_text_method = 0;

    png_set_text_compression_method(&s, 5);

    EXPECT_EQ(s.zlib_text_method, 5,
              "zlib_text_method should be updated to 5 when method == 5");
    const auto& logs = get_warnings();
    EXPECT_EQ(static_cast<int>(logs.size()), 1,
              "Exactly one warning should be emitted for non-8 method");
    if (!logs.empty()) {
        EXPECT_TRUE(logs[0] == std::string("Only compression method 8 is supported by PNG"),
                    "Warning message content should match expected text");
    }
    return true;
}

// 4) Repeated calls: 8 after a non-8 call should not emit a warning and update value.
// Coverage: ensure warning state does not persist erroneously across calls.
static bool test_subsequent_call_after_non_eight() {
    clear_warnings();

    png_struct s;
    s.zlib_text_method = 0;

    // First call with non-8 to generate a warning and set method.
    png_set_text_compression_method(&s, 5);
    // Then call with 8; should not warn.
    png_set_text_compression_method(&s, 8);

    EXPECT_EQ(s.zlib_text_method, 8,
              "zlib_text_method should be updated to 8 on second call");
    const auto& logs = get_warnings();
    // There should be exactly one warning from the first call
    EXPECT_EQ(static_cast<int>(logs.size()), 1,
              "There should remain a single prior warning after second call");
    return true;
}

// ------------------------------------------------------------------------------------
// Main: run all tests and report summary

int main() {
    std::cout << "Starting unit tests for png_set_text_compression_method...\n";

    RUN_TEST(test_null_pointer_handling);
    RUN_TEST(test_method_eight_sets_and_no_warning);
    RUN_TEST(test_method_not_eight_warns_and_sets);
    RUN_TEST(test_subsequent_call_after_non_eight);

    std::cout << "Tests run: " << g_total_tests << "\n";
    std::cout << "Tests failed: " << g_failed_tests << "\n";

    if (g_failed_tests == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED. See details above.\n";
        return 1;
    }
}