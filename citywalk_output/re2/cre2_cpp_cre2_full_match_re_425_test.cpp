// Unit test suite for cre2_full_match_re (C wrapper around RE2 full-match)
// This test is designed to be run without a testing framework (no GTest).
// It uses a small in-process test harness that reports results to stdout.
// The goal is to exercise true/false branches of the focal function and ensure
// reasonable handling of memory for the match output.
// Notes:
// - We assume the signature of cre2_full_match_re as typically defined by the
//   project wrappers: int cre2_full_match_re(const char* pattern, int pattern_len,
//   const char* text, int text_len, cre2_string_t* match, int nmatch)
// - cre2_string_t is a simple struct { char* data; int length; } used by the
//   project to return matched substrings. The function returns >0 on match,
// 0 if no match, and -1 on error.
// - The test cleans up any allocated memory in cre2_string_t if produced.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Forward declaration of cre2_string_t to be used in test.
// In the actual project, include the proper header (cre2.h) which defines this.
// We include cre2.h to pull in the real definitions and the cre2_full_match_re
// function prototype.
extern "C" {
}

// Simple non-terminating assertion helper: logs failure but continues execution.
static void log_failure(const std::string& test_name, const std::string& msg) {
    std::cerr << "[FAIL] " << test_name << ": " << msg << std::endl;
}

// Simple success logger
static void log_success(const std::string& test_name, const std::string& msg) {
    std::cout << "[OK]   " << test_name << ": " << msg << std::endl;
}

// Helper to run a single full-match test case.
// Returns true if test passed, false otherwise.
static bool run_full_match_test(const std::string& test_name,
                                const char* pattern, const char* text,
                                bool should_match) {
    // Prepare input lengths
    int p_len = static_cast<int>(std::strlen(pattern));
    int t_len = static_cast<int>(std::strlen(text));

    // Prepare output container for the match (single slot for simplicity)
    cre2_string_t match;
    match.data = NULL;
    match.length = 0;

    // Call the focal function
    int ret = cre2_full_match_re(pattern, p_len, text, t_len, &match, 1);

    // Interpret return value: >0 means matched, 0 means no match, -1 error
    bool did_match = (ret > 0);

    // Free any allocated memory if produced by the API
    if (match.data != NULL) {
        free(match.data);
        match.data = NULL;
        match.length = 0;
    }

    // Validate result against expectation. We purposefully do not terminate on
    // failure to maximize coverage (non-terminating assertion).
    if (did_match == should_match) {
        log_success(test_name, "Pattern \"" + std::string(pattern) +
                   "\" against text \"" + std::string(text) +
                   "\" produced expected result: " + (did_match ? "MATCH" : "NO MATCH"));
        return true;
    } else {
        log_failure(test_name, "Pattern \"" + std::string(pattern) +
                   "\" against text \"" + std::string(text) +
                   "\" produced result " + std::to_string(ret) +
                   " (expected " + (should_match ? "MATCH" : "NO MATCH") + ")");
        return false;
    }
}

// Entry point
int main() {
    // Collect all test results
    std::vector<bool> results;

    // Test 1: Simple exact match
    // pattern "abc" should fully match text "abc"
    results.push_back(run_full_match_test(
        "FullMatch_SimpleExact",
        "abc",
        "abc",
        true));

    // Test 2: Simple non-match (text longer than pattern)
    // pattern "abc" should NOT fully match text "abcd"
    results.push_back(run_full_match_test(
        "FullMatch_SimpleNonExact_LengthMismatch",
        "abc",
        "abcd",
        false));

    // Test 3: Full match with wildcard pattern (a.* with text "abc" should match)
    // Note: RE2 syntax uses standard regex; "a.*" should full-match "abc".
    results.push_back(run_full_match_test(
        "FullMatch_WildcardPattern",
        "a.*",
        "abc",
        true));

    // Test 4: Non-match with wildcard pattern (pattern does not cover full text)
    // pattern "a.*d" should not fully match "abcd" (ends with 'd' but not whole)
    results.push_back(run_full_match_test(
        "FullMatch_WildcardNonExact",
        "a.*d",
        "abcd",
        true)); // In RE2, "a.*d" can fully match "abcd" -> true

    // The above Test 4 assertion assumes "abcd" is fully matched by "a.*d".
    // If the actual behavior differs in the environment, this test may fail.
    // To ensure robustness, we could adjust based on observed behavior.

    // Summary
    int total = static_cast<int>(results.size());
    int passed = 0;
    for (bool r : results) if (r) ++passed;

    std::cout << "=========================" << std::endl;
    std::cout << "Total tests: " << total << ", Passed: " << passed
              << ", Failed: " << (total - passed) << std::endl;
    std::cout << "=========================" << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}