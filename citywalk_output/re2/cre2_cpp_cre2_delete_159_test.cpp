// Test suite for cre2_delete in cre2.cpp
// Notes:
// - Uses a lightweight, non-terminating test harness (no Google Test).
// - Exercises both branches conceptually: valid motif deletion and null-pointer deletion.
// - Focuses on correctness and safety of cre2_delete behavior.
// - Includes explanatory comments for each test case.

#include <cre2.h>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the CRE2/C API header used by the project.
// The focal method under test is cre2_delete in cre2.cpp.

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;

#define TEST_ASSERT(cond, msg) do {                          \
    ++g_total_tests;                                           \
    if (cond) {                                                \
        ++g_passed_tests;                                      \
    } else {                                                   \
        std::cerr << "TEST FAILURE: " << (msg) << std::endl;  \
    }                                                          \
} while (0)

// Test 1: cre2_delete should be safe to call with a nullptr.
// The implementation is a simple delete TO_RE2(re); pattern.
// Deleting a null pointer in C++ is well-defined; this test ensures no crash or exception.
void test_cre2_delete_nullptr() {
    // No setup required; directly call with nullptr.
    // If cre2_delete throws or crashes, the test harness will catch (non-terminating).
    try {
        cre2_delete(nullptr);
        TEST_ASSERT(true, "cre2_delete(nullptr) should be safe and not crash");
    } catch (...) {
        TEST_ASSERT(false, "cre2_delete(nullptr) threw an exception");
    }
}

// Test 2: cre2_delete on a valid CRE2 regexp should be safe.
// Create a simple valid pattern, obtain cre2_regexp_t*, then delete it using cre2_delete.
// This exercises the typical workflow and ensures no crash during deletion.
void test_cre2_delete_valid_re() {
    cre2_options_t *opt = cre2_opt_new();
    bool test_ok = true;

    if (!opt) {
        TEST_ASSERT(false, "cre2_opt_new() returned NULL (cannot proceed with test_cre2_delete_valid_re)");
        return;
    }

    cre2_opt_set_log_errors(opt, 0);

    cre2_regexp_t *rex = cre2_new("a", 1, opt); // simple valid pattern
    if (rex == NULL) {
        TEST_ASSERT(false, "cre2_new() failed to create a valid regexp for test_cre2_delete_valid_re");
        test_ok = false;
    } else {
        // Perform deletion of the created regexp
        try {
            cre2_delete(rex);
            TEST_ASSERT(true, "cre2_delete on a valid regexp should not crash");
        } catch (...) {
            TEST_ASSERT(false, "cre2_delete threw an exception on a valid regexp");
            test_ok = false;
        }
    }

    // Cleanup options (only if created)
    cre2_opt_delete(opt);

    if (test_ok) {
        // If we reached here, test passed
        // Note: The test result is already recorded above via TEST_ASSERT.
        ;
    }
}

// Test 3: cre2_new should return NULL for an invalid pattern and cre2_delete should not be invoked.
// This validates that invalid input does not lead to a crash path when cre2_delete would be invoked (not invoked here).
void test_cre2_new_invalid_pattern_returns_null() {
    cre2_options_t *opt = cre2_opt_new();
    bool test_ok = true;

    if (!opt) {
        TEST_ASSERT(false, "cre2_opt_new() returned NULL in test_cre2_new_invalid_pattern_returns_null");
        return;
    }

    cre2_opt_set_log_errors(opt, 0);

    // Intentionally invalid regex pattern: unbalanced "("
    const char *invalid_pat = "(";
    cre2_regexp_t *rex = cre2_new(invalid_pat, 1, opt);
    if (rex != NULL) {
        // If the library unexpectedly returns a valid rex, we must cleanup and fail
        cre2_delete(rex);
        TEST_ASSERT(false, "cre2_new() unexpectedly succeeded for an invalid pattern");
        test_ok = false;
    } else {
        // Expected: NULL on invalid pattern
        TEST_ASSERT(true, "cre2_new() correctly returned NULL for an invalid pattern");
    }

    cre2_opt_delete(opt);

    if (!test_ok) {
        // nothing else to do; test result captured by TEST_ASSERT
    }
}

// Simple runner to execute all tests and report summary
int main() {
    std::cout << "Starting test suite for cre2_delete (cre2.cpp) ..." << std::endl;

    test_cre2_delete_nullptr();
    test_cre2_delete_valid_re();
    test_cre2_new_invalid_pattern_returns_null();

    std::cout << "Test results: " << g_passed_tests << " / " << g_total_tests << " tests passed." << std::endl;

    // Return 0 if all tests passed; non-zero otherwise.
    return (g_passed_tests == g_total_tests) ? 0 : 1;
}