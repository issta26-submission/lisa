// Lightweight C++11 unit tests for cre2_set_compile (no GTest)
// This test suite exercises the focal method by exercising a CRE2 set with:
// - an empty set
// - a set with a valid pattern
// - a set with an invalid pattern to trigger a false/0 result from Compile
// Notes:
// - We avoid terminating assertions; tests log pass/fail and continue.
// - We rely only on the provided CRE2 wrappers (cre2.h) and RE2 internals as needed.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <cassert>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>



// Simple non-terminating assertion helper that logs results
static void log_result(const std::string &test_name, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cerr << "[FAIL] " << test_name << "\n";
    }
}

// Helper: safe string length for compile-time literals
static size_t clen(const char* s) { return s ? std::strlen(s) : 0; }

// Test 1: Empty CRE2 set should compile successfully (true branch)
static bool test_cre2_set_compile_empty_set() {
    // Create an empty CRE2 set. Pass a conservative anchor value (0) to avoid
    // depending on a specific enum value in the test environment.
    cre2_set *set = cre2_set_new(nullptr, 0);
    if (set == nullptr) {
        return false; // cannot proceed without a valid set
    }

    // Call the focal method through the C wrapper
    int result = cre2_set_compile(set);

    // Clean up
    cre2_set_delete(set);

    // Expected: Compile() on an empty set should succeed (true -> non-zero)
    return (result != 0);
}

// Test 2: Empty + a valid simple pattern should compile successfully (true branch)
static bool test_cre2_set_compile_with_valid_pattern() {
    cre2_set *set = cre2_set_new(nullptr, 0);
    if (set == nullptr) {
        return false;
    }

    // Add a simple valid pattern that RE2 supports
    const char *pattern = "abc";
    char error[256];
    cre2_set_add(set, pattern, std::strlen(pattern), error, sizeof(error));

    int result = cre2_set_compile(set);

    cre2_set_delete(set);

    // Expect Compile() to succeed with a valid pattern
    return (result != 0);
}

// Test 3: Set with an invalid pattern should fail Compile() (false branch)
static bool test_cre2_set_compile_with_invalid_pattern() {
    cre2_set *set = cre2_set_new(nullptr, 0);
    if (set == nullptr) {
        return false;
    }

    // Add an obviously invalid pattern to trigger Compile failure
    const char *invalid_pattern = "("; // unbalanced, invalid regex
    char error[256];
    cre2_set_add(set, invalid_pattern, std::strlen(invalid_pattern), error, sizeof(error));

    int result = cre2_set_compile(set);

    cre2_set_delete(set);

    // Expect Compile() to fail (0)
    return (result == 0);
}

int main() {
    int tests_run = 0;
    int tests_passed = 0;

    // Test 1
    tests_run++;
    bool t1 = test_cre2_set_compile_empty_set();
    if (t1) tests_passed++;
    log_result("cre2_set_compile - empty set should succeed", t1);

    // Test 2
    tests_run++;
    bool t2 = test_cre2_set_compile_with_valid_pattern();
    if (t2) tests_passed++;
    log_result("cre2_set_compile - valid pattern should compile", t2);

    // Test 3
    tests_run++;
    bool t3 = test_cre2_set_compile_with_invalid_pattern();
    if (t3) tests_passed++;
    log_result("cre2_set_compile - invalid pattern should fail compile", t3);

    // Summary
    std::cout << "Test results: " << tests_passed << " / " << tests_run << " passed." << std::endl;

    // Return non-zero if any test failed
    return (tests_passed == tests_run) ? 0 : 1;
}