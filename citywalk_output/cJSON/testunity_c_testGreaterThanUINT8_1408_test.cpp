/*
  Unit test suite for the focal method: testGreaterThanUINT8

  Step 2: This suite is designed to extend coverage specifically for
  testGreaterThanUINT8 as defined in testunity.c. It exercises multiple
  scenarios to ensure both true and false predicate branches are covered
  (using Unity's provided macros). It uses non-terminating assertions where
  possible and demonstrates how to handle expected failures with
  EXPECT_ABORT_BEGIN/VERIFY_FAILS_END wrappers.

  Candidate Keywords guiding test design:
  - UNITY_UINT8, v0, v1, p0, p1
  - TEST_ASSERT_GREATER_THAN_UINT8
  - Pointer dereferencing: *p0, *p1
  - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END for non-terminating negative tests
  - Basic edge cases: 0, 255, equal values
  - Coverage of both direct and indirect (via pointers) comparisons
  -Main test harness using RUN_TEST to execute tests

  Notes:
  - This file is intended to be compiled alongside Unity's runtime (unity.c/.h)
    and testunity.c, or adjusted to your build setup.
  - It is written in C++11 compatible style but uses the Unity C API.
  - The tests assume the Unity macros TEST_ASSERT_GREATER_THAN_UINT8, etc.,
    behave as defined in the focal project. Some tests intentionally expect
    failures and are wrapped with EXPECT_ABORT_BEGIN/VERIFY_FAILS_END.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
}
#endif

// Forward declarations for Unity-based tests
void testGreaterThanUINT8_Extended_PassCases(void);
void testGreaterThanUINT8_Extended_PassWithOtherValues(void);
void testGreaterThanUINT8_Extended_FailOnReverse(void);
void testGreaterThanUINT8_Extended_EqualCaseFail(void);

int main(void) {
    UNITY_BEGIN();

    // Run extended coverage tests for testGreaterThanUINT8
    RUN_TEST(testGreaterThanUINT8_Extended_PassCases);
    RUN_TEST(testGreaterThanUINT8_Extended_PassWithOtherValues);
    RUN_TEST(testGreaterThanUINT8_Extended_FailOnReverse);
    RUN_TEST(testGreaterThanUINT8_Extended_EqualCaseFail);

    return UNITY_END();
}

/*
  Test: testGreaterThanUINT8_Extended_PassCases
  Purpose: Validate that the focal macro succeeds when v0 < v1
           using direct values and via pointers.
  Coverage:
  - Direct comparison v0 vs v1
  - Pointer indirection: *p0 vs v1
  - Direct comparison with v0 vs *p1
  - Pointer indirection with both pointers: *p0 vs *p1
*/
void testGreaterThanUINT8_Extended_PassCases(void)
{
    // Pass scenario using direct values
    UNITY_UINT8 v0 = 0;
    UNITY_UINT8 v1 = 255;
    UNITY_UINT8 *p0 = &v0;
    UNITY_UINT8 *p1 = &v1;

    // All four combinations should pass under the macro semantics
    TEST_ASSERT_GREATER_THAN_UINT8(v0, v1);
    TEST_ASSERT_GREATER_THAN_UINT8(*p0, v1);
    TEST_ASSERT_GREATER_THAN_UINT8(v0, *p1);
    TEST_ASSERT_GREATER_THAN_UINT8(*p0, *p1);

    // Additional pass scenario with different values where v0 < v1
    v0 = 5;
    v1 = 10;
    p0 = &v0;
    p1 = &v1;

    TEST_ASSERT_GREATER_THAN_UINT8(v0, v1);
    TEST_ASSERT_GREATER_THAN_UINT8(*p0, v1);
    TEST_ASSERT_GREATER_THAN_UINT8(v0, *p1);
    TEST_ASSERT_GREATER_THAN_UINT8(*p0, *p1);
}

/*
  Test: testGreaterThanUINT8_Extended_PassWithOtherValues
  Purpose: Extra coverage for a variety of non-boundary but valid cases
  where v0 < v1 still holds, including mixed-type pointer usage.
*/
void testGreaterThanUINT8_Extended_PassWithOtherValues(void)
{
    UNITY_UINT8 v0 = 1;
    UNITY_UINT8 v1 = 2;
    UNITY_UINT8 *p0 = &v0;
    UNITY_UINT8 *p1 = &v1;

    TEST_ASSERT_GREATER_THAN_UINT8(v0, v1);
    TEST_ASSERT_GREATER_THAN_UINT8(*p0, v1);
    TEST_ASSERT_GREATER_THAN_UINT8(v0, *p1);
    TEST_ASSERT_GREATER_THAN_UINT8(*p0, *p1);
}

/*
  Test: testGreaterThanUINT8_Extended_FailOnReverse
  Purpose: Ensure that the macro correctly reports a failure when the
           expected relation is not satisfied (v0 > v1 is true for v0=255, v1=0
           if the macro semantics are that the second argument should be greater).
  This test is wrapped with EXPECT_ABORT_BEGIN to capture the expected abort
  without terminating the entire test suite.
*/
void testGreaterThanUINT8_Extended_FailOnReverse(void)
{
    UNITY_UINT8 v0 = 255;
    UNITY_UINT8 v1 = 0;
    UNITY_UINT8 *p0 = &v0;
    UNITY_UINT8 *p1 = &v1;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_UINT8(v0, v1);
    VERIFY_FAILS_END
}

/*
  Test: testGreaterThanUINT8_Extended_EqualCaseFail
  Purpose: Validate that an equal pair (v0 == v1) does not satisfy the
  greater-than predicate, ensuring the false branch is covered.
*/
void testGreaterThanUINT8_Extended_EqualCaseFail(void)
{
    UNITY_UINT8 v0 = 100;
    UNITY_UINT8 v1 = 100;
    UNITY_UINT8 *p0 = &v0;
    UNITY_UINT8 *p1 = &v1;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_UINT8(v0, v1);
    VERIFY_FAILS_END
}