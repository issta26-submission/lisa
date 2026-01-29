/*************************************************************
 * Test Suite for cmsBool CloseEnough(cmsFloat64Number a, cmsFloat64Number b)
 * 
 * Step 1 – Program Understanding (embedded in comments):
 * - Core logic: returns true if fabs(b - a) < (1.0 / 65535.0)
 * - Threshold: 1.0 / 65535.0
 * - Branches to cover: true when diff < threshold; false when diff >= threshold
 * 
 * Step 2 – Unit Test Generation (focused on CloseEnough):
 * - Tests include: exact zeroes, just-below-threshold, exactly-threshold, above-threshold,
 *   and several near-boundary scenarios with positive and negative numbers.
 * 
 * Step 3 – Test Case Refinement:
 * - Use non-terminating assertions (do not abort on failure)
 * - Use C++11, only standard library and provided interfaces
 * - Call the C function via C linkage in a safe way
 * - Expose test results via console output for easy inspection
 *************************************************************/

/* 
   Note: This test suite is designed to link with the provided C focal function
   declared as:
       extern "C" cmsBool CloseEnough(cmsFloat64Number a, cmsFloat64Number b);
   To maximize compatibility in a C++11 environment, we declare compatible aliases
   for the types here and call the function with C linkage.
*/

#include <iostream>
#include <cmath>
#include <testcms2.h>


// Candidate type aliases to align with likely project definitions.
// We purposely use simple equivalents to ensure correct linkage with the C function.
using cmsBool = int;
using cmsFloat64Number = double;

// Declaration of the focal function with C linkage to avoid name mangling issues.
extern "C" cmsBool CloseEnough(cmsFloat64Number a, cmsFloat64Number b);

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Helper to log test outcomes without stopping execution
static void expect_bool(bool actual, bool expected, const char* desc) {
    ++g_tests_run;
    if (actual == expected) {
        std::cout << "[PASS] " << desc << std::endl;
    } else {
        ++g_tests_failed;
        std::cout << "[FAIL] " << desc
                  << " (expected " << (expected ? "true" : "false")
                  << ", got " << (actual ? "true" : "false") << ")" << std::endl;
    }
}

/*
   Test Case 1: Exact same inputs should be True (diff = 0)
   - a = 0.0, b = 0.0
*/
static void test_CloseEnough_case_exact_zeroes() {
    bool res = CloseEnough(0.0, 0.0) != 0;
    expect_bool(res, true, "CloseEnough(0.0, 0.0) should be true (diff = 0)");
}

/*
   Test Case 2: Just below the threshold should be True
   - a = 0.0, b = delta where delta = (1/65535) - tiny
*/
static void test_CloseEnough_case_just_below_threshold() {
    const double eps = 1.0 / 65535.0;
    const double tiny = 1e-12;
    const double delta = eps - tiny;
    bool res = CloseEnough(0.0, delta) != 0;
    expect_bool(res, true, "CloseEnough(0.0, eps - tiny) should be true");
}

/*
   Test Case 3: Exactly at the threshold should be False
   - a = 0.0, b = delta where delta = 1/65535
*/
static void test_CloseEnough_case_exact_threshold_false() {
    const double eps = 1.0 / 65535.0;
    bool res = CloseEnough(0.0, eps) != 0;
    expect_bool(res, false, "CloseEnough(0.0, eps) should be false (diff equals threshold)");
}

/*
   Test Case 4: Above threshold should be False
   - a = 0.0, b = 2 * eps
*/
static void test_CloseEnough_case_above_threshold_false() {
    const double eps = 1.0 / 65535.0;
    bool res = CloseEnough(0.0, 2.0 * eps) != 0;
    expect_bool(res, false, "CloseEnough(0.0, 2*eps) should be false (diff > threshold)");
}

/*
   Test Case 5: Negative numbers near each other
   - a = -1.0, b = -1.0 + delta, where delta < eps
*/
static void test_CloseEnough_case_negative_nearby_true() {
    const double eps = 1.0 / 65535.0;
    const double delta = eps - 1e-12;
    bool res = CloseEnough(-1.0, -1.0 + delta) != 0;
    expect_bool(res, true, "CloseEnough(-1.0, -1.0 + delta) should be true");
}

/*
   Test Case 6: Boundary near positive side with small offset
   - a = 1.0, b = 1.0 - eps + tiny (diff < eps)
*/
static void test_CloseEnough_case_boundary_positive_true() {
    const double eps = 1.0 / 65535.0;
    const double tiny = 1e-12;
    bool res = CloseEnough(1.0, 1.0 - eps + tiny) != 0;
    expect_bool(res, true, "CloseEnough(1.0, 1.0 - eps + tiny) should be true");
}

/*
   Test Case 7: Cross threshold on negative difference
   - a = 1.0, b = 1.0 - eps - tiny (diff > eps)
*/
static void test_CloseEnough_case_cross_threshold_false() {
    const double eps = 1.0 / 65535.0;
    const double tiny = 1e-12;
    bool res = CloseEnough(1.0, 1.0 - eps - tiny) != 0;
    expect_bool(res, false, "CloseEnough(1.0, 1.0 - eps - tiny) should be false");
}

int main() {
    // Run all test cases
    test_CloseEnough_case_exact_zeroes();
    test_CloseEnough_case_just_below_threshold();
    test_CloseEnough_case_exact_threshold_false();
    test_CloseEnough_case_above_threshold_false();
    test_CloseEnough_case_negative_nearby_true();
    test_CloseEnough_case_boundary_positive_true();
    test_CloseEnough_case_cross_threshold_false();

    // Summary
    std::cout << "\nTest Summary: "
              << g_tests_run << " run, "
              << (g_tests_run - g_tests_failed) << " passed, "
              << g_tests_failed << " failed."
              << std::endl;

    // Non-terminating tests: return non-zero if any failure occurred
    return (g_tests_failed == 0) ? 0 : 1;
}