/*
  Unit test suite for the focal method main (as represented by the test-set.c logic)
  using C++11. This test suite does not rely on GTest; it uses a small
  self-contained test harness with non-terminating assertions to maximize
  coverage of the CRE2-based regex set creation, population, compilation and
  matching workflow.

  Key focus areas mapped from the focal code (Candidate Keywords):
  - cre2_options_t, cre2_opt_new, cre2_opt_delete
  - cre2_set, cre2_set_new, CRE2_UNANCHORED
  - cre2_set_add_simple, cre2_set_add
  - CRE2_UNANCHORED behavior and error handling on invalid patterns
  - cre2_set_compile
  - cre2_set_match and the expected match results across sample texts
  - Error buffer handling for invalid patterns

  Notes:
  - The CRE2 library is a C API; this test harness calls into it from C++11.
  - Tests are designed to be executable without terminating the process on
    first failure (non-terminating assertions) to maximize coverage.
  - This file does not depend on GTest; it contains a minimal inline test runner.
*/

#include <stdlib.h>
#include <cre2.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <string>
#include <cre2-test.h>
#include <vector>
#include <iostream>


// Include CRE2 C API (ensure the build system links against cre2)
extern "C" {
}

// Simple non-terminating assertion helpers
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

static void log_result(const TestResult& r) {
    if (r.passed) {
        std::cout << "[PASS] " << r.name;
    } else {
        std::cout << "[FAIL] " << r.name;
    }
    if (!r.message.empty()) {
        std::cout << " - " << r.message;
    }
    std::cout << std::endl;
}

// Domain knowledge constants mirrored from the focal test-set.c
static const int ERROR_LEN = 100;

// Test 1: Basic happy-path behavior mirroring the original main's test-set
static TestResult test_cre2_basic_match_scenario() {
    TestResult result;
    result.name = "CRE2 Basic Add/Compile/Match Path";

    const char *pattern0 = "hello";
    const char *pattern1 = "world";
    const char *pattern2 = "po.*ar";
    const char *pattern3 = "(invalid";

    char *text;
    int index;

    cre2_options_t *opt = cre2_opt_new();
    if (opt == NULL) {
        result.passed = false;
        result.message = "Failed to allocate CRE2 options (cre2_opt_new returned NULL).";
        return result;
    }

    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (set == NULL) {
        result.passed = false;
        result.message = "Error creating the cre2_set (cre2_set_new returned NULL).";
        cre2_opt_delete(opt);
        return result;
    }

    // Add patterns
    index = cre2_set_add_simple(set, pattern0);
    if (index != 0) {
        result.passed = false;
        result.message = "Failed adding pattern0 to set. cre2_set_add_simple returned " + std::to_string(index);
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    index = cre2_set_add_simple(set, pattern1);
    if (index != 1) {
        result.passed = false;
        result.message = "Failed adding pattern1 to set. cre2_set_add_simple returned " + std::to_string(index);
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    index = cre2_set_add_simple(set, pattern2);
    if (index != 2) {
        result.passed = false;
        result.message = "Failed adding pattern2 to set. cre2_set_add_simple returned " + std::to_string(index);
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    // Try to add invalid pattern
    char error[ERROR_LEN];
    index = cre2_set_add(set, pattern3, strlen(pattern3), error, ERROR_LEN);
    if (index != -1) {
        result.passed = false;
        result.message = "Error: successfully added an invalid pattern3 to set (expected -1).";
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    } else {
        // Optional: ensure error buffer has some message
        // We won't treat missing message as fatal; it's expected that an invalid
        // pattern yields an error string.
        // Print the error to aid debugging if desired.
        // std::cerr << "Invalid pattern error: " << error << std::endl;
    }

    // Compile regex set
    if (!cre2_set_compile(set)) {
        result.passed = false;
        result.message = "Failed to compile regex set.";
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    int match[3];
    int count;

    // Test first match
    text = (char*)"hello world!";
    count = cre2_set_match(set, text, strlen(text), match, 3);
    if (count != 2 || match[0] != 0 || match[1] != 1) {
        result.passed = false;
        result.message = "Failed to match first text: " + std::string(text);
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    // Test second match
    text = (char*)"blabla hello polar bear!";
    count = cre2_set_match(set, text, strlen(text), match, 3);
    if (count != 2 || match[0] != 0 || match[1] != 2) {
        result.passed = false;
        result.message = "Failed to match second text: " + std::string(text);
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    // Test third text (no match)
    text = (char*)"this should not match anything";
    count = cre2_set_match(set, text, strlen(text), match, 3);
    if (count != 0) {
        result.passed = false;
        result.message = "Failed to assert zero matches for text: " + std::string(text);
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    // Cleanup
    cre2_opt_delete(opt);
    cre2_set_delete(set);

    result.passed = true;
    return result;
}

// Test 2: Invalid pattern handling does not crash and allows cleanup
static TestResult test_cre2_invalid_pattern_handling_only() {
    TestResult result;
    result.name = "CRE2 Invalid Pattern Handling and Cleanup";

    const char *pattern0 = "hello";
    const char *pattern1 = "world";
    const char *pattern2 = "po.*ar";
    const char *pattern3 = "(invalid";

    char error[ERROR_LEN];
    int index;

    cre2_options_t *opt = cre2_opt_new();
    if (opt == NULL) {
        result.passed = false;
        result.message = "Failed to allocate CRE2 options (cre2_opt_new returned NULL).";
        return result;
    }

    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (set == NULL) {
        result.passed = false;
        result.message = "Error creating the cre2_set (cre2_set_new returned NULL).";
        cre2_opt_delete(opt);
        return result;
    }

    index = cre2_set_add_simple(set, pattern0);
    if (index != 0) {
        result.passed = false;
        result.message = "Failed adding pattern0 to set. cre2_set_add_simple returned " + std::to_string(index);
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    index = cre2_set_add_simple(set, pattern1);
    if (index != 1) {
        result.passed = false;
        result.message = "Failed adding pattern1 to set. cre2_set_add_simple returned " + std::to_string(index);
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    index = cre2_set_add_simple(set, pattern2);
    if (index != 2) {
        result.passed = false;
        result.message = "Failed adding pattern2 to set. cre2_set_add_simple returned " + std::to_string(index);
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    index = cre2_set_add(set, pattern3, strlen(pattern3), error, ERROR_LEN);
    if (index != -1) {
        result.passed = false;
        result.message = "Error: successfully added an invalid pattern3 to set in invalid-pattern test.";
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    if (!cre2_set_compile(set)) {
        result.passed = false;
        result.message = "Failed to compile regex set in invalid-pattern test.";
        cre2_opt_delete(opt);
        cre2_set_delete(set);
        return result;
    }

    // No matching tests required here; the goal is to ensure no crash and proper cleanup
    cre2_opt_delete(opt);
    cre2_set_delete(set);

    result.passed = true;
    return result;
}

// Entry point for the test suite
int main() {
    std::vector<TestResult> results;

    results.push_back(test_cre2_basic_match_scenario());
    results.push_back(test_cre2_invalid_pattern_handling_only());

    int failures = 0;
    for (const auto& r : results) {
        log_result(r);
        if (!r.passed) ++failures;
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}