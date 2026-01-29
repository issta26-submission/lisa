/*
  Lightweight C++11 test suite for cre2_new (wrapper to RE2) without GoogleTest.
  - Step 2: Generate tests targeting two code branches:
    1) opt == NULL
    2) opt != NULL
  - Step 3: Refine with additional edge cases to improve coverage.
  - The tests rely solely on the public CRE2/C API exposed by cre2.h.
  - Tests are non-terminating and report results to stdout and a final summary.
*/

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


extern "C" {
}

// Global failure counter
static int g_failures = 0;

// Simple, non-terminating assertion helper
static void assert_true(bool cond, const std::string &msg) {
    if (cond) {
        std::cout << "PASS: " << msg << std::endl;
    } else {
        std::cerr << "FAIL: " << msg << std::endl;
        ++g_failures;
    }
}

// Helper to safely delete a CRE2 regexp pointer casted to CRE2 type
static void safe_delete_regexp(const cre2_regexp_t *re) {
    if (re) {
        // cre2_delete expects non-const; cast away const for deletion
        cre2_delete(const_cast<cre2_regexp_t*>(re));
    }
}

// Test 1: cre2_new with NULL opt and a valid pattern should return non-null and be usable
static void test_cre2_new_null_opt_valid_pattern() {
    // Arrange
    const char *pattern = "abc";
    int pattern_len = static_cast<int>(std::strlen(pattern));

    // Act
    void* obj = cre2_new(pattern, pattern_len, NULL);

    // Assert
    const cre2_regexp_t* re = reinterpret_cast<const cre2_regexp_t*>(obj);
    assert_true(obj != NULL, "cre2_new should return non-null when opt is NULL and pattern is valid");

    if (re) {
        int err = cre2_error_code(re);
        assert_true(err == 0, "cre2_error_code should be 0 for valid pattern with NULL opt");
        // Optional: could add a light-weight sanity check by performing a match, but
        // to keep dependencies minimal, we rely on error_code here.
    }
    // Cleanup
    if (re) cre2_delete(const_cast<cre2_regexp_t*>(re));
}

// Test 2: cre2_new with non-NULL opt should return non-null and be usable
static void test_cre2_new_nonnull_opt_valid_pattern() {
    // Arrange
    cre2_options_t *opt = cre2_opt_new();
    const char *pattern = "abc";
    int pattern_len = static_cast<int>(std::strlen(pattern));

    // Act
    void* obj = cre2_new(pattern, pattern_len, opt);

    // Assert
    const cre2_regexp_t* re = reinterpret_cast<const cre2_regexp_t*>(obj);
    assert_true(obj != NULL, "cre2_new should return non-null when opt is non-NULL and pattern is valid");

    if (re) {
        int err = cre2_error_code(re);
        assert_true(err == 0, "cre2_error_code should be 0 for valid pattern with non-NULL opt");
    }

    // Cleanup
    if (re) cre2_delete(const_cast<cre2_regexp_t*>(re));
    if (opt) cre2_opt_delete(opt);
}

// Test 3: cre2_new with NULL opt and an invalid pattern should still return non-null but error_code != 0
static void test_cre2_new_null_opt_invalid_pattern() {
    // Arrange: intentionally invalid pattern (unmatched parenthesis)
    const char *pattern = "(";
    int pattern_len = static_cast<int>(std::strlen(pattern));

    // Act
    void* obj = cre2_new(pattern, pattern_len, NULL);

    // Assert
    const cre2_regexp_t* re = reinterpret_cast<const cre2_regexp_t*>(obj);
    assert_true(obj != NULL, "cre2_new should return non-null even for invalid pattern with NULL opt");

    if (re) {
        int err = cre2_error_code(re);
        assert_true(err != 0, "cre2_error_code should be non-zero for invalid pattern with NULL opt");
    }

    // Cleanup
    if (re) cre2_delete(const_cast<cre2_regexp_t*>(re));
}

// Test 4: cre2_new with non-NULL opt and an invalid pattern should still return non-null but error_code != 0
static void test_cre2_new_nonnull_opt_invalid_pattern() {
    // Arrange
    cre2_options_t *opt = cre2_opt_new();
    const char *pattern = "(";
    int pattern_len = static_cast<int>(std::strlen(pattern));

    // Act
    void* obj = cre2_new(pattern, pattern_len, opt);

    // Assert
    const cre2_regexp_t* re = reinterpret_cast<const cre2_regexp_t*>(obj);
    assert_true(obj != NULL, "cre2_new should return non-null even for invalid pattern with non-NULL opt");

    if (re) {
        int err = cre2_error_code(re);
        assert_true(err != 0, "cre2_error_code should be non-zero for invalid pattern with non-NULL opt");
    }

    // Cleanup
    if (re) cre2_delete(const_cast<cre2_regexp_t*>(re));
    if (opt) cre2_opt_delete(opt);
}

// Test 5: cre2_new with empty pattern (edge case) should be handled gracefully
static void test_cre2_new_empty_pattern() {
    // Arrange
    const char *pattern = "";
    int pattern_len = 0;

    // Act
    void* obj = cre2_new(pattern, pattern_len, NULL);

    // Assert
    const cre2_regexp_t* re = reinterpret_cast<const cre2_regexp_t*>(obj);
    assert_true(obj != NULL, "cre2_new should handle empty pattern and return non-null");

    if (re) {
        int err = cre2_error_code(re);
        // Empty pattern is generally valid; expect 0
        assert_true(err == 0, "cre2_error_code should be 0 for empty pattern");
    }

    // Cleanup
    if (re) cre2_delete(const_cast<cre2_regexp_t*>(re));
}

int main() {
    std::cout << "Running cre2_new unit tests (without GTest)" << std::endl;

    test_cre2_new_null_opt_valid_pattern();
    test_cre2_new_nonnull_opt_valid_pattern();
    test_cre2_new_null_opt_invalid_pattern();
    test_cre2_new_nonnull_opt_invalid_pattern();
    test_cre2_new_empty_pattern();

    if (g_failures > 0) {
        std::cout << "Summary: " << g_failures << " test(s) failed." << std::endl;
        return 1;
    } else {
        std::cout << "Summary: All tests passed." << std::endl;
        return 0;
    }
}