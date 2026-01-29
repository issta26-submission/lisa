// Test suite for the focal method testDoublesNotEqualNegative1
// This suite is designed to be compiled with Unity (C), adapted for a C++11 project if needed.
// It focuses on exercising the focal method in testunity.c and provides additional
// paths to improve coverage of the related predicate branches.
// Explanatory comments are included with each unit test to outline the purpose and coverage.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Forward declare the focal test function defined in testunity.c.
// It is implemented using Unity macros (EXPECT_ABORT_BEGIN, TEST_ASSERT_EQUAL_DOUBLE, etc.)
// The function itself contains a branch controlled by UNITY_EXCLUDE_DOUBLE.
void testDoublesNotEqualNegative1(void);
#ifdef __cplusplus
}
#endif

// Candidate Keywords (Step 1): 
// - testDoublesNotEqualNegative1: focal test under scrutiny
// - UNITY_EXCLUDE_DOUBLE: conditional compilation flag controlling double support
// - TEST_IGNORE: skip path when doubles are excluded
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: verify expected abort behavior on assertion failure
// - TEST_ASSERT_EQUAL_DOUBLE: double-precision equality assertion
// - Failing vs. passing paths: coverage of equal vs. not-equal scenarios (via direct tests and focal test)
 
// Test 1: Integration path - run the focal test function as-is.
// Purpose: Ensure the focal method's behavior is exercised in the environment where the
//          double-assert would fail (or be ignored if doubles are excluded).
// Coverage: branches inside the focal function (double-supported vs double-excluded) are exercised
//           depending on compile-time macro UNITY_EXCLUDE_DOUBLE.
static void test_DoublesNotEqualNegative1_ViaFocalFunction(void)
{
    // Call the focal test function defined in testunity.c.
    // If UNITY_EXCLUDE_DOUBLE is defined, the focal path will invoke TEST_IGNORE().
    // Otherwise, it will attempt TEST_ASSERT_EQUAL_DOUBLE with two different values
    // and verify that an abort occurs via EXPECT_ABORT_BEGIN/VERIFY_FAILS_END.
    testDoublesNotEqualNegative1();
}

// Test 2: Direct assertion path to verify abort-on-not-equal behavior in isolation.
// Purpose: Independently exercise a failing double comparison, independent of the focal test's gating.
// Coverage: tests the not-equal path of TEST_ASSERT_EQUAL_DOUBLE when doubles are available.
static void test_DoublesNotEqualNegative1_DirectAbortPath(void)
{
#ifndef UNITY_EXCLUDE_DOUBLE
    EXPECT_ABORT_BEGIN
    // Deliberately compare two different doubles to trigger an abort.
    TEST_ASSERT_EQUAL_DOUBLE(-9273.9649, -9273.0049);
    VERIFY_FAILS_END
#else
    // If doubles are excluded, mark as ignored to reflect the alternate branch.
    TEST_IGNORE_MESSAGE("Doubles support excluded in this build; skipping direct abort path.");
#endif
}

// Test 3: Direct double equality pass path (coverage for the 'true' branch of the predicate).
// Purpose: Provide a non-aborting scenario to ensure the test harness covers the non-failing path.
// Coverage: ensures that a correct equal-double assertion passes without aborting.
static void test_DoublesNotEqualNegative1_DoubleEqualityPasses(void)
{
#ifndef UNITY_EXCLUDE_DOUBLE
    // This assertion should pass (the numbers are equal), contributing coverage
    // to the non-aborting path of double comparisons.
    TEST_ASSERT_EQUAL_DOUBLE(-9273.9649, -9273.9649);
#else
    TEST_IGNORE_MESSAGE("Doubles support excluded in this build; skipping direct equality pass test.");
#endif
}

int main(void)
{
    UNITY_BEGIN("test_doubles_not_equal_negative1_suite.cpp");

    // Register tests. Each test should be runnable in isolation and cover both
    // the focal method path and supportive direct paths for coverage.
    RUN_TEST(test_DoublesNotEqualNegative1_ViaFocalFunction);
    RUN_TEST(test_DoublesNotEqualNegative1_DirectAbortPath);
    RUN_TEST(test_DoublesNotEqualNegative1_DoubleEqualityPasses);

    return UNITY_END();
}