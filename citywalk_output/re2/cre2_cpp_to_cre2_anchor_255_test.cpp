// Test suite for the focal method to_cre2_anchor(cre2_anchor_t anchor)
// Context: The function maps CRE2 anchor enum values to RE2 anchors.
// This test validates the true/false branches of the mapping and default behavior.
// The test is designed to compile with C++11, without GoogleTest, and relies on
// the project's cre2.h and RE2 headers being available during linking.

#include <cre2.h>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include project dependencies and RE2 types.
// The focal method uses RE2::Anchor and cre2_anchor_t / CRE2_* constants.

// Declaration of the focal function under test. If the function is not declared
// in cre2.h, we declare it here to enable linkage with the project implementation.
extern RE2::Anchor to_cre2_anchor(cre2_anchor_t anchor);

// Global counter to collect test failures without aborting on first failure.
// This enables non-terminating assertions to maximize code execution paths.
static int g_test_failures = 0;

// Helper: Execute a single test case and report results.
// - input: a CRE2 anchor enum value to feed into the focal method.
// - expected: the RE2::Anchor value we expect as the mapping result.
// - test_name: human-friendly description for the test case.
static void run_single_case(cre2_anchor_t input, RE2::Anchor expected, const std::string& test_name)
{
    RE2::Anchor result = to_cre2_anchor(input);
    int result_int = static_cast<int>(result);
    int expected_int = static_cast<int>(expected);

    if (result_int != expected_int) {
        std::cerr << "[FAIL] " << test_name
                  << " | Input CRE2 anchor = " << static_cast<int>(input)
                  << " => Result RE2 anchor = " << result_int
                  << ", expected = " << expected_int << std::endl;
        ++g_test_failures;
    } else {
        std::cout << "[PASS] " << test_name
                  << " | Input CRE2 anchor = " << static_cast<int>(input)
                  << " maps to RE2 anchor = " << result_int << std::endl;
    }
}

// Entry point: run all test cases covering true/false branches and default behavior.
int main()
{
    // The mapping logic (from the focal method) is:
    // - CRE2_ANCHOR_START  -> RE2::ANCHOR_START
    // - CRE2_ANCHOR_BOTH   -> RE2::ANCHOR_BOTH
    // - CRE2_UNANCHORED    -> RE2::UNANCHORED
    // - Any other value    -> default UNANCHORED (due to initialization)

    // Test 1: CRE2_ANCHOR_START should map to RE2::ANCHOR_START
    run_single_case(CRE2_ANCHOR_START, RE2::ANCHOR_START, "to_cre2_anchor maps CRE2_ANCHOR_START -> RE2::ANCHOR_START");

    // Test 2: CRE2_ANCHOR_BOTH should map to RE2::ANCHOR_BOTH
    run_single_case(CRE2_ANCHOR_BOTH, RE2::ANCHOR_BOTH, "to_cre2_anchor maps CRE2_ANCHOR_BOTH -> RE2::ANCHOR_BOTH");

    // Test 3: CRE2_UNANCHORED should map to RE2::UNANCHORED
    run_single_case(CRE2_UNANCHORED, RE2::UNANCHORED, "to_cre2_anchor maps CRE2_UNANCHORED -> RE2::UNANCHORED");

    // Test 4: An unknown value should default to UNANCHORED.
    // Cast an out-of-range integer to cre2_anchor_t to simulate an invalid enum value.
    run_single_case(static_cast<cre2_anchor_t>(999), RE2::UNANCHORED,
                    "to_cre2_anchor with invalid input -> default UNANCHORED (fallback)");

    // Summary
    if (g_test_failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) FAILED.\n";
        return 1;
    }
}