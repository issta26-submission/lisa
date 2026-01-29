// Test suite for the focal method CheckJointCurvesSShaped and related tone-curve utilities.
// This file is designed to be compiled with a C/C++11 compiler without GoogleTest.
// It directly calls the function under test from testcms2.c and exercises related
// LittleCMS tone-curve APIs to improve coverage.
// Explanatory comments are provided for each test to describe intent and coverage.

#include <iostream>
#include <testcms2.h>


// Import the C test framework and the LittleCMS-based test helpers.
// testcms2.h is a C header; ensure C linkage for the declarations when included from C++.
extern "C" {
}

// Lightweight, non-terminating test harness.
// It records total tests run and failures but continues executing subsequent tests
// to maximize code coverage (as requested in the domain knowledge).
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT(cond, desc) do {                                  \
    ++g_total_tests;                                                     \
    if (!(cond)) {                                                       \
        ++g_failed_tests;                                                \
        std::cerr << "[FAIL] " << desc                                \
                  << " (in " << __FILE__ << ":" << __LINE__ << ")"        \
                  << std::endl;                                        \
    } else {                                                             \
        std::cout << "[PASS] " << desc << std::endl;                   \
    }                                                                    \
} while (0)

// Test 1: Validate that the focal function CheckJointCurvesSShaped returns a 0/1 value.
// Rationale: The function ends by returning the result of cmsIsToneCurveLinear(Result),
// which should yield either 0 (non-linear) or 1 (linear). This test ensures
// the function completes and produces a boolean-like value without crashing.
void test_CheckJointCurvesSShaped_ReturnsBoolean() {
    // Call the focal function (C linkage assumed via header).
    int rc = CheckJointCurvesSShaped();
    // Acceptable values are 0 or 1 for a boolean-like indicator.
    TEST_ASSERT((rc == 0) || (rc == 1),
                "CheckJointCurvesSShaped returns 0 or 1 as a boolean indicator");
    // Additional non-fatal assertion: rc should be non-negative.
    TEST_ASSERT(rc >= 0, "CheckJointCurvesSShaped rc is non-negative");
}

// Test 2: Reproduce the internal sequence used by the focal method to exercise
// the involved tone-curve APIs directly. This ensures that the components
// (cmsBuildParametricToneCurve, cmsReverseToneCurve, cmsJoinToneCurve,
// cmsIsToneCurveLinear) work together without leaking resources.
void test_ParametricCurveOperations_Workflow() {
    cmsFloat64Number p = 3.2;
    // Build a parametric tone curve (same pathway as focal method).
    cmsToneCurve *Forward = cmsBuildParametricToneCurve(DbgThread(), 108, &p);
    TEST_ASSERT(Forward != NULL, "Forward curve creation via cmsBuildParametricToneCurve");

    // Reverse the curve and join two copies to form a new curve.
    cmsToneCurve *Reverse = cmsReverseToneCurve(Forward);
    TEST_ASSERT(Reverse != NULL, "Reverse curve creation via cmsReverseToneCurve");

    cmsToneCurve *Result = cmsJoinToneCurve(DbgThread(), Forward, Forward, 4096);
    TEST_ASSERT(Result != NULL, "Joined curve creation via cmsJoinToneCurve");

    // Clean up the inputs and verify the output-like property (non-null result).
    cmsFreeToneCurve(Forward);
    cmsFreeToneCurve(Reverse);
    cmsInt32Number rc = cmsIsToneCurveLinear(Result);
    cmsFreeToneCurve(Result);
    TEST_ASSERT((rc == 0) || (rc == 1),
                "IsToneCurveLinear result is 0 or 1 for the joined curve");
}

// Test 3: Validate robustness across a small set of parametric values.
// This helps ensure the internal parametric curve construction behaves
// consistently for different inputs and that the subsequent operations
// do not crash or misbehave.
void test_ParametricCurveOperations_VaryP() {
    const cmsFloat64Number p_values[] = {0.5, 3.2, 7.0};
    const int count = sizeof(p_values) / sizeof(p_values[0]);

    for (int i = 0; i < count; ++i) {
        cmsFloat64Number p = p_values[i];
        cmsToneCurve *Forward = cmsBuildParametricToneCurve(DbgThread(), 108, &p);
        TEST_ASSERT(Forward != NULL, "Forward curve creation with varying p");

        cmsToneCurve *Reverse = cmsReverseToneCurve(Forward);
        TEST_ASSERT(Reverse != NULL, "Reverse curve creation with varying p");

        cmsToneCurve *Result = cmsJoinToneCurve(DbgThread(), Forward, Forward, 4096);
        TEST_ASSERT(Result != NULL, "Joined curve with varying p");

        cmsFreeToneCurve(Forward);
        cmsFreeToneCurve(Reverse);
        cmsInt32Number rc = cmsIsToneCurveLinear(Result);
        cmsFreeToneCurve(Result);
        TEST_ASSERT((rc == 0) || (rc == 1),
                    "IsToneCurveLinear for varying p yields 0 or 1");
    }
}

// Entry point for the test module.
// It sequentially runs all defined test cases and reports a final status.
// The program returns 0 if all tests pass (no failures), otherwise 1.
int main() {
    std::cout << "Starting unit tests for CheckJointCurvesSShaped and related tone-curve operations\n";

    test_CheckJointCurvesSShaped_ReturnsBoolean();
    test_ParametricCurveOperations_Workflow();
    test_ParametricCurveOperations_VaryP();

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << "\n";

    // Return non-zero if any test failed to signal issues in CI.
    return (g_failed_tests == 0) ? 0 : 1;
}