// High-quality C++11 unit test suite for cre2_error_string function
// NOTE: This test suite does not rely on Google Test. It uses a small
// internal assertion mechanism and runs from main().
// It exercises the focal function cre2_error_string(const cre2_regexp_t*).

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Simple internal test harness
static int g_failures = 0;
#define TEST_FAIL(msg) do { std::cerr << "FAIL: " << msg << std::endl; ++g_failures; } while(0)
#define TEST_PASS(msg) do { std::cout << "PASS: " << msg << std::endl; } while(0)

// Ensure we safely copy error strings since the returned pointer may be invalid after delete
static std::string copy_error_string(const char* p) {
    if (p == nullptr) return std::string();
    return std::string(p);
}

/*
  Test 1: Invalid pattern should produce a non-empty error string.
  Rationale:
  - cre2_new should create a regexp object even for some invalid patterns in this C wrapper.
  - cre2_error_string(re) should reflect the error via RE2's error() string.
  - We copy the string content before deleting the object to avoid dangling pointers.
*/
static void test_invalid_pattern_error_string() {
    // Setup
    cre2_options_t *opt = cre2_opt_new();
    if (opt == nullptr) {
        TEST_FAIL("cre2_opt_new() returned nullptr for invalid pattern test");
        return;
    }

    const char* bad_pattern = "("; // invalid due to unbalanced parenthesis
    int pattern_len = static_cast<int>(std::strlen(bad_pattern));
    cre2_regexp_t *re = cre2_new(bad_pattern, pattern_len, opt);

    // Act: retrieve error string (if available)
    std::string error_str;

    if (re != nullptr) {
        const char* err_cstr = cre2_error_string(re);
        error_str = copy_error_string(err_cstr);
        cre2_delete(re);
    } else {
        // If cre2_new returns nullptr for an invalid pattern, this is a separate API behavior.
        // We consider this a failure for the test since we cannot verify the error string here.
        TEST_FAIL("cre2_new() returned nullptr for invalid pattern; cannot verify error string");
        cre2_opt_delete(opt);
        return;
    }

    // Cleanup
    cre2_opt_delete(opt);

    // Assert: error string should be non-empty
    if (error_str.empty()) {
        TEST_FAIL("Expected non-empty error string for invalid pattern, got empty string");
    } else {
        TEST_PASS("Invalid pattern produced non-empty error string");
        std::cout << "  Error: \"" << error_str << "\"\n";
    }
}

/*
  Test 2: Valid pattern should produce an empty error string.
  Rationale:
  - For a valid CRE2 pattern, error() should be empty, so cre2_error_string should return "".
  - Verify by comparing contents to an empty string.
*/
static void test_valid_pattern_error_string() {
    // Setup
    cre2_options_t *opt = cre2_opt_new();
    if (opt == nullptr) {
        TEST_FAIL("cre2_opt_new() returned nullptr for valid pattern test");
        return;
    }

    const char* good_pattern = "abc"; // valid simple pattern
    int pattern_len = static_cast<int>(std::strlen(good_pattern));
    cre2_regexp_t *re = cre2_new(good_pattern, pattern_len, opt);

    // Act
    std::string error_str;

    if (re != nullptr) {
        const char* err_cstr = cre2_error_string(re);
        error_str = copy_error_string(err_cstr);
        cre2_delete(re);
    } else {
        TEST_FAIL("cre2_new() returned nullptr for valid pattern; cannot verify error string");
        cre2_opt_delete(opt);
        return;
    }

    // Cleanup
    cre2_opt_delete(opt);

    // Assert: error string should be empty
    if (!error_str.empty()) {
        TEST_FAIL("Expected empty error string for valid pattern, got: \"" + error_str + "\"");
    } else {
        TEST_PASS("Valid pattern produced empty error string");
    }
}

/*
  Test 3: Consistency check - multiple calls to cre2_error_string produce content-equal results.
  Rationale:
  - The content of the error string should be stable across multiple invocations on the same re object.
  - We copy the content to std::string to compare values independent of the underlying c_str lifetime.
*/
static void test_error_string_consistency() {
    // Setup
    cre2_options_t *opt = cre2_opt_new();
    if (opt == nullptr) {
        TEST_FAIL("cre2_opt_new() returned nullptr for consistency test");
        return;
    }

    const char* pattern = "x("; // intentionally invalid to ensure a non-empty error
    int len = static_cast<int>(std::strlen(pattern));
    cre2_regexp_t *re = cre2_new(pattern, len, opt);
    if (re == nullptr) {
        // If creation failed, consider this a test setup failure
        TEST_FAIL("cre2_new() returned nullptr for invalid pattern in consistency test");
        cre2_opt_delete(opt);
        return;
    }

    // Act: two consecutive reads
    const char* s1 = cre2_error_string(re);
    const char* s2 = cre2_error_string(re);

    std::string err1 = copy_error_string(s1);
    std::string err2 = copy_error_string(s2);

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Assert: both strings should be equal in content
    if (err1 != err2) {
        TEST_FAIL("Inconsistent error_string between calls on the same re: \"" + err1 + "\" vs \"" + err2 + "\"");
    } else {
        TEST_PASS("Consistent error_string across multiple calls");
        if (!err1.empty()) {
            std::cout << "  Consistent error: \"" << err1 << "\"\n";
        } else {
            std::cout << "  Consistent empty error string as expected for invalid pattern.\n";
        }
    }
}

int main() {
    std::cout << "Starting cre2_error_string unit tests (C++11, no GTest)\n";

    test_invalid_pattern_error_string();
    test_valid_pattern_error_string();
    test_error_string_consistency();

    if (g_failures > 0) {
        std::cerr << "Summary: FAILED " << g_failures << " test(s).\n";
        return 1;
    } else {
        std::cout << "Summary: ALL tests PASSED.\n";
        return 0;
    }
}