/*
 * Test Suite: Focused verification of the focal method testDoublesWithinDelta
 * Context: Reproduces and exercises the behavior of the focal Unity test snippet
 *          located in testunity.c, adapted for a C++11 non-GTest environment.
 *
 * Step 1 (Program Understanding):
 * - The focal method asserts four double comparisons within specified deltas.
 * - If UNITY_EXCLUDE_DOUBLE is defined, the test is ignored (TEST_IGNORE-like).
 * - Core dependencies are the double-within checks and the macro UNITY_EXCLUDE_DOUBLE.
 *
 * Step 2 (Unit Test Generation):
 * - Provide a small, self-contained test harness in C++11 that emulates the
 *   behavior of the focal test, including an "ignore" path when UNITY_EXCLUDE_DOUBLE
 *   is defined.
 * - Implement a light-weight within-delta assertion helper to avoid external frameworks.
 *
 * Step 3 (Test Case Refinement):
 * - Cover the original four true-branch assertions (within delta) when executed.
 * - Provide an additional boundary-not-within test to exercise false-branch handling
 *   (without aborting the program, since we aren't using GTest/Unity).
 * - Ensure static/compile-time toggle (UNITY_EXCLUDE_DOUBLE) drives the ignore path.
 *
 * Notes:
 * - This suite is designed to be compiled with C++11 (or higher) and without GTest.
 * - The focal behavior is reproduced; the exact Unity macros are not required for
 *   compilation in this standalone harness.
 * - Explanatory comments accompany each unit test function.
 */

// Candidate Keywords (representing core components):
// - UNITY_EXCLUDE_DOUBLE
// - TEST_ASSERT_DOUBLE_WITHIN
// - Double delta comparisons
// - Floating-point literals in test data
// - Fallback: ignore path when UNITY_EXCLUDE_DOUBLE is defined

#include <cmath>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Global test counters for a lightweight harness
static int gTotalTests = 0;
static int gTotalFailures = 0;

// Flag set by a failing assertion within the focal test
static bool gFailureFlag = false;

// Utility: reset failure flag before each isolated test
static void resetFailureFlag() {
    gFailureFlag = false;
}

// Utility: record a failing assertion with a descriptive message
static void failAssertion(const char* msg) {
    gFailureFlag = true;
    // Emit a clear failure message without aborting the test runner
    std::cerr << "ASSERTION FAILED: " << (msg ? msg : "unknown reason") << std::endl;
}

// Helper: emulate Unity's TEST_ASSERT_DOUBLE_WITHIN(delta, expected, actual)
static void assertDoubleWithin(double delta, double expected, double actual, const std::string& label) {
    if (std::fabs(expected - actual) > delta) {
        failAssertion("Double within delta assertion failed: " + label);
    }
}

// Focal method under test (reproduced in this harness):
// Mirrors the original Unity-based testDoublesWithinDelta logic
void testDoublesWithinDelta(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // Simulate Unity's TEST_IGNORE mechanism when doubles are excluded
    // Mark as ignored (no failure, no abort)
    std::cout << "testDoublesWithinDelta: IGNORED (UNITY_EXCLUDE_DOUBLE defined)\n";
#else
    // Four double-within assertions as per the focal method
    // 1) delta 0.00003, expected 187245.03485, actual 187245.03488
    assertDoubleWithin(0.00003, 187245.03485, 187245.03488, "delta=0.00003, 187245.03485 vs 187245.03488");

    // 2) delta 1.0, expected 187245.0, actual 187246.0
    assertDoubleWithin(1.0, 187245.0, 187246.0, "delta=1.0, 187245.0 vs 187246.0");

    // 3) delta 0.05, expected 9273.2549, actual 9273.2049
    assertDoubleWithin(0.05, 9273.2549, 9273.2049, "delta=0.05, 9273.2549 vs 9273.2049");

    // 4) delta 0.007, expected -726.93725, actual -726.94424
    assertDoubleWithin(0.007, -726.93725, -726.94424, "delta=0.007, -726.93725 vs -726.94424");
#endif
}

// Additional boundary-not-within test (to increase coverage by exercising the false-branch)
static void testDoublesWithinDelta_BoundaryNotWithin(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // If the test is ignored, do not treat as a failure
    std::cout << "testDoublesWithinDelta_BoundaryNotWithin: IGNORED (UNITY_EXCLUDE_DOUBLE defined)\n";
#else
    // Intentionally choose a delta small enough to make this fail
    resetFailureFlag();
    assertDoubleWithin(0.00001, 100.0, 100.00002, "boundary test: delta=1e-5, 100.0 vs 100.00002");
    // We expect a failure for this specific case; reflect in the harness output
    if (gFailureFlag) {
        std::cout << "testDoublesWithinDelta_BoundaryNotWithin: PASSED (failure detected as expected)\n";
    } else {
        std::cout << "testDoublesWithinDelta_BoundaryNotWithin: FAILED (expected failure was not detected)\n";
        gTotalFailures++;
    }
#endif
}

// Wrapper test to execute the focal test as part of the suite
static void run_testDoublesWithinDelta_Normal(void)
{
    resetFailureFlag();
    gTotalTests++;

    testDoublesWithinDelta();

    if (!gFailureFlag) {
        std::cout << "[PASS] testDoublesWithinDelta_Normal: all assertions within delta passed\n";
    } else {
        gTotalFailures++;
        std::cout << "[FAIL] testDoublesWithinDelta_Normal: one or more assertions failed\n";
    }
}

// Wrapper test to exercise boundary-not-within case
static void run_testDoublesWithinDelta_BoundaryNotWithin(void)
{
    // This test checks the false branch by deliberately causing a failure
    gTotalTests++;

    testDoublesWithinDelta_BoundaryNotWithin();

    // The inner function prints its own PASS/FAIL outcome. We only adjust totals here if needed.
    // For simplicity, rely on internal messaging; no additional global flag required here.
}

// Entry point: execute all tests in this suite
int main(void)
{
    // Run normal focal-test scenario
    run_testDoublesWithinDelta_Normal();

    // Run boundary-not-within scenario to improve coverage
    run_testDoublesWithinDelta_BoundaryNotWithin();

    // Summary
    std::cout << "\nTest Summary:\n";
    std::cout << "  Total tests run: " << gTotalTests << "\n";
    std::cout << "  Total failures:  " << gTotalFailures << "\n";

    // Return non-zero if there were failures
    return (gTotalFailures > 0) ? 1 : 0;
}