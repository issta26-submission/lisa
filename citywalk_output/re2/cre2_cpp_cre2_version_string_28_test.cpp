/*
Unit Test Suite for cre2_version_string (void)

Purpose:
- Validate that cre2_version_string correctly returns the version interface string
  defined by cre2_VERSION_INTERFACE_STRING.

Guidance followed (mapped to the provided steps):
- Step 1 (Program Understanding):
  Core dependent components identified:
  - cre2_version_string: function under test
  - cre2_VERSION_INTERFACE_STRING: expected version string macro/constant
  - cre2.h: header providing the cre2 API (and likely the macro)
  - The test targets content equality with cre2_VERSION_INTERFACE_STRING and ensures a non-null, non-empty C-string is returned.

- Step 2 (Unit Test Generation):
  Tests are crafted to cover:
  - Non-null return value
  - Exact string content match with cre2_VERSION_INTERFACE_STRING
  - Non-empty string (length > 0)

- Step 3 (Test Case Refinement):
  - Tests are implemented using plain C++11 standard library (no GTest/GMock)
  - Static configuration concerns are respected (no private access)
  - Non-terminating assertions: test continues after failures; a global counter tracks failures
  - All tests are executable via a single main()
  - Clear explanatory comments accompany each test case

Notes:
- This test file assumes it is compiled/link-ed with the same build context as the focal code (cre2.cpp / cre2.h and associated build system). It includes cre2.h to access cre2_version_string and cre2_VERSION_INTERFACE_STRING.
- If cre2_VERSION_INTERFACE_STRING is a macro that expands to a string literal, string comparison via strcmp() remains valid.

Code (C++11, no GTest, self-contained test runner)
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


// Include the focal library header to access cre2_version_string and the interface string macro

// Global failure counter to allow non-terminating tests
static int g_failures = 0;

// Simple, non-terminating assertion helper for C-strings
static void expect_string_eq(const char* actual, const char* expected, const std::string& test_name) {
    if (actual == nullptr) {
        std::cerr << "FAIL: " << test_name << " - actual is null, expected: \"" << (expected ? expected : "NULL") << "\"\n";
        ++g_failures;
        return;
    }
    if (std::strcmp(actual, expected) != 0) {
        std::cerr << "FAIL: " << test_name << " - strings differ. actual: \"" << actual
                  << "\", expected: \"" << expected << "\"\n";
        ++g_failures;
    } else {
        std::cout << "PASS: " << test_name << "\n";
    }
}

// Simple non-null assertion helper
static void expect_not_null(const char* ptr, const std::string& test_name) {
    if (ptr == nullptr) {
        std::cerr << "FAIL: " << test_name << " - actual is null\n";
        ++g_failures;
    } else {
        std::cout << "PASS: " << test_name << "\n";
    }
}

// Simple non-empty assertion helper
static void expect_not_empty(const char* ptr, const std::string& test_name) {
    if (ptr == nullptr || ptr[0] == '\0') {
        std::cerr << "FAIL: " << test_name << " - actual is empty or null\n";
        ++g_failures;
    } else {
        std::cout << "PASS: " << test_name << "\n";
    }
}

/*
Candidate Keywords (Step 1 / Step 2 mapping):
- cre2_version_string: focal method under test
- cre2_VERSION_INTERFACE_STRING: expected version string
- cre2.h: header for cre2 interfaces
- Non-null / Non-empty / Content equality checks
- C-string semantics and comparison (strcmp)
- No GTest; plain C++11 test harness
*/

// Test 1: Ensure the function returns a non-null pointer
static void test_version_string_returns_non_null() {
    const char* v = cre2_version_string();
    expect_not_null(v, "cre2_version_string returns non-null");
}

// Test 2: Ensure the returned string content exactly matches cre2_VERSION_INTERFACE_STRING
static void test_version_string_matches_interface_string() {
    const char* v = cre2_version_string();
    // cre2_VERSION_INTERFACE_STRING is expected to be the canonical interface string
    // Compare string contents to avoid pointer identity issues
    expect_string_eq(v, cre2_VERSION_INTERFACE_STRING, "cre2_version_string matches interface string");
}

// Test 3: Ensure the returned string is not empty
static void test_version_string_not_empty() {
    const char* v = cre2_version_string();
    expect_not_empty(v, "cre2_version_string not empty");
}

// Optional: Test 4 (defensive): Ensure the first character of the string is not the null terminator
static void test_version_string_first_char_valid() {
    const char* v = cre2_version_string();
    if (v != nullptr) {
        if (v[0] == '\0') {
            std::cerr << "FAIL: cre2_version_string first character is null terminator, string appears empty\n";
            ++g_failures;
        } else {
            std::cout << "PASS: cre2_version_string first character valid\n";
        }
    } else {
        // If null, previous test should have caught it; still report here for completeness
        std::cerr << "FAIL: cre2_version_string is null in first-char test\n";
        ++g_failures;
    }
}

int main() {
    // Run all tests
    std::cout << "Running tests for cre2_version_string...\n";

    test_version_string_returns_non_null();
    test_version_string_matches_interface_string();
    test_version_string_not_empty();
    test_version_string_first_char_valid();

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED (" << 4 << " checks performed).\n";
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED.\n";
        return 1;
    }
}