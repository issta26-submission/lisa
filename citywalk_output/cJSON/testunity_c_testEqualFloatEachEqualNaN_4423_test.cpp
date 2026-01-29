/* 
   Test Suite: testEqualFloatEachEqualNaN
   Purpose:
   - Validate behavior of NaN handling in Unity's TEST_ASSERT_EACH_EQUAL_FLOAT
   - Cover both passing (all NaNs) and failing (one non-NaN) scenarios
   - Exercise the UNITY_EXCLUDE_FLOAT branch by ensuring the test can be ignored when float support is excluded

   Notes:
   - This test suite is designed to work with the existing Unity-based test file testunity.c
   - No GTest usage; compatible with C/C++11 environments
*/
#include <unity.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


void setUp(void) { }
void tearDown(void) { }

/* 
   Helper: create a NaN value without relying on NAN macro
   Using 0.0f/0.0f yields a NaN in IEEE-754 compliant environments,
   which is consistent with how the focal test creates NaN values.
*/
static inline float make_nan(void) { return 0.0f/0.0f; }

/* Test: Failsafe pass when all elements are NaN and NaN is treated as equal */
void testEqualFloatEachEqualNaN_PassesWithExplicitNaN(void)
{
#ifndef UNITY_EXCLUDE_FLOAT
    float p0[4] = { make_nan(), make_nan(), make_nan(), make_nan() };
    // Expect all elements to be equal to NaN (per Unity's NaN-equality semantics)
    TEST_ASSERT_EACH_EQUAL_FLOAT(make_nan(), p0, 4);
#else
    TEST_IGNORE();
#endif
}

/* Test: Fails when any element is not NaN (should abort) */
void testEqualFloatEachEqualNaN_FailsWithOneNotNaN(void)
{
#ifndef UNITY_EXCLUDE_FLOAT
    float p0[4] = { make_nan(), make_nan(), make_nan(), 1.0f }; // last element is not NaN
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(make_nan(), p0, 4);
    VERIFY_FAILS_END
#else
    TEST_IGNORE();
#endif
}

/* Test: Ensure proper handling of Unity's UNITY_EXCLUDE_FLOAT branch */
void testEqualFloatEachEqualNaN_IgnoresWhenFloatExcluded(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    // When float support is excluded, the focal test should be ignored
    TEST_IGNORE();
#else
    // If not excluded, we still exercise a NaN-equality case
    float p0[4] = { make_nan(), make_nan(), make_nan(), make_nan() };
    TEST_ASSERT_EACH_EQUAL_FLOAT(make_nan(), p0, 4);
#endif
}