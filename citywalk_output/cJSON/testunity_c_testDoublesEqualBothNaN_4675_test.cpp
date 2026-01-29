/*
  Test Suite: NaN handling for double equality in Unity
  Purpose:
  - Validate the focal method testDoublesEqualBothNaN in testunity.c by exercising
    the NaN-equality path and providing alternative NaN creation methods in a C++
    test harness (compatible with C Unity tests).

  Step alignment:
  - Step 1: Analyze focal method behavior (testDoublesEqualBothNaN) which relies on
    Unity's TEST_ASSERT_EQUAL_DOUBLE to compare two NaN Double values produced via
    0.0 / d_zero (or similar). The macro path changes based on UNITY_EXCLUDE_DOUBLE.
  - Step 2: Build a test suite that covers the focal path via the wrapper call to the focal
    function, as well as direct NaN equality checks to ensure correct behavior when NaN
    is produced in different ways (0/0, std::numeric_limits<double>::quiet_NaN()).
  - Step 3: Refine tests for coverage and maintainability; include explanatory comments per test.

  Notes:
  - This file is written for a C++11 toolchain, but relies on Unity (C) semantics.
  - It declares the focal function with C linkage where needed and invokes Unity's RUN_TEST.
  - The tests are designed to avoid terminating the entire test suite on failure;
    Unity handles per-test failures/captures accordingly.
*/

#include <cmath>
#include <limits>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal function under test (defined in testunity.c)
void testDoublesEqualBothNaN(void);
#ifdef __cplusplus
}
#endif

// Wrapper to invoke the focal test from this test suite
#ifdef __cplusplus
extern "C" {
void test_DoublesEqual_BothNaN_FromFocal(void);
}
#else
void test_DoublesEqual_BothNaN_FromFocal(void);
#endif

// Implementation: Wrapper that delegates to the focal test
void test_DoublesEqual_BothNaN_FromFocal(void)
{
    // Call the focal test as-is. Depending on UNITY_EXCLUDE_DOUBLE, this may
    // run the assertion or be ignored by Unity.
    testDoublesEqualBothNaN();
}

// Additional direct NaN-equality test using two NaN values created via 0.0/0.0
// This helps ensure that Unity's TEST_ASSERT_EQUAL_DOUBLE treats NaN==NaN
// when NaNs are produced at runtime in a straightforward manner.
void test_DoublesEqual_BothNaN_DirectNaN_UsingSelfEquality(void)
{
    // Create two NaN values by dividing zero by zero
    double a = 0.0 / 0.0;
    double b = 0.0 / 0.0;

    // Expose NaN-equality behavior: if Unity handles NaN equality specially,
    // this will pass. If not, this test will fail, signaling a potential policy issue.
    TEST_ASSERT_EQUAL_DOUBLE(a, b);
}

// Additional direct NaN-equality test using C++ standard library NaN provider
// This verifies NaN equality across a second NaN-producing method.
void test_DoublesEqual_BothNaN_DirectNaN_UsingCppNan(void)
{
    // Obtain NaN via C++ standard library limits
    double a = std::numeric_limits<double>::quiet_NaN();
    double b = std::numeric_limits<double>::quiet_NaN();

    // Validate that two independently created NaNs compare equal under Unity's assertion
    TEST_ASSERT_EQUAL_DOUBLE(a, b);
}

// Unity test runner
int main(void)
{
    UNITY_BEGIN();

    // Run focal test wrapper (will exercise the code path including UNITY_EXCLUDE_DOUBLE)
    RUN_TEST(test_DoublesEqual_BothNaN_FromFocal);

    // Run direct NaN equality tests to ensure coverage of NaN equality logic
    RUN_TEST(test_DoublesEqual_BothNaN_DirectNaN_UsingSelfEquality);
    RUN_TEST(test_DoublesEqual_BothNaN_DirectNaN_UsingCppNan);

    return UNITY_END();
}