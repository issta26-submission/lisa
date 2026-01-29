// Candidate Keywords (Step 1):
// - UNITY_EXCLUDE_DOUBLE, d_zero, f_zero
// - TEST_ASSERT_DOUBLE_IS_NEG_INF
// - testDoubleIsNegInf1 (focal method under test) and its dependencies
// - std::isinf (IEEE754 checks) and float/double handling
// - Unity testing macros: RUN_TEST, UnityBegin, UnityEnd

// This file provides a small supplementary test suite in C++11 that exercises
// the focal method testDoubleIsNegInf1 and supplements it with additional
// coverage for the double/IEEE-754 based negative infinity behavior that the
// focal test relies on. It avoids GTest and uses the Unity framework as per
// the project setup. The tests are designed to be executable even when the
// UNITY_EXCLUDE_DOUBLE or UNITY_EXCLUDE_FLOAT macros influence behavior in the
// focal code.

// Note: The focal function testDoubleIsNegInf1 is defined in testunity.c. We
// declare it here with C linkage so that Unity's RUN_TEST can invoke it
// alongside additional tests that we provide to cover related dependencies.

#include <cmath>
#include <unity.h>
#include <cstdio>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
// Forward declaration of the focal test function defined in testunity.c
void testDoubleIsNegInf1(void);
#ifdef __cplusplus
}
#endif


// Step 2: Supplemental tests to exercise the double-based negative infinity behavior
// while keeping compatibility with the existing Unity-based test infrastructure.

// Test 1: Verify that -3.0 / 0.0 yields a negative infinity using a local double
// representation. This mirrors the behavior under the Unity test while using
// standard math checks to maximize coverage of the numeric edge-case handling.
#ifndef UNITY_EXCLUDE_DOUBLE
// Local replication of the dependency (d_zero) for independent verification.
static const double local_d_zero = 0.0;

void testDoubleNegInf_FromZero(void)
{
    // Compute a value that should be -infinity under IEEE-754 semantics
    double val = -3.0 / local_d_zero;
    // Validate that the result is negative infinity without terminating the test
    bool isNegInf = std::isinf(val) && (val < 0.0);
    TEST_ASSERT_TRUE(isNegInf);
}
#endif

// Test 2: Verify that a similar negative infinity result can be produced when using
// an explicit float zero, exercising UNITY_EXCLUDE_FLOAT related behavior.
// This broadens coverage to the path that handles float constants versus double.
#ifndef UNITY_EXCLUDE_FLOAT
static const float local_f_zero = 0.0f;

void testDoubleNegInf_FromFloatZero(void)
{
    // Compute a negative infinity using a float zero constant
    float val = -3.0f / local_f_zero;
    // Cast to double for a robust isinf check
    bool isNegInf = std::isinf(static_cast<double>(val)) && (val < 0.0f);
    TEST_ASSERT_TRUE(isNegInf);
}
#endif

// Step 3: Test harness (for C++11 project) that runs the focal test plus supplemental tests.
// The main function uses Unity's API to execute tests in a single executable.

int main(void)
{
    // Begin the Unity test run
    UnityBegin("test_double_neg_inf_suite.cpp");

    // Execute the focal test function (as provided by testunity.c)
    RUN_TEST(testDoubleIsNegInf1);

    // Supplemental tests to exercise the double/inf behavior and its dependencies
#ifndef UNITY_EXCLUDE_DOUBLE
    // Supplemental test that validates -3.0 / 0.0 -> -INF using double
    RUN_TEST(testDoubleNegInf_FromZero);
#endif

#ifndef UNITY_EXCLUDE_FLOAT
    // Supplemental test that validates -3.0f / 0.0f -> -INF using float
    RUN_TEST(testDoubleNegInf_FromFloatZero);
#endif

    // End and report test results
    return UnityEnd();
}