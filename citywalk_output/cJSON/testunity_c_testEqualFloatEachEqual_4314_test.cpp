/*
 * Test Suite: Additional coverage for testEqualFloatEachEqual
 * Context: Unity-based unit tests (no GTest). Targets the focal method
 *          testEqualFloatEachEqual from testunity.c.
 *
 * Notes:
 * - This suite adds multiple cases to exercise passing and failing scenarios
 *   for TEST_ASSERT_EACH_EQUAL_FLOAT(..., array, size).
 * - It also includes a test path that mirrors the Unity Exclude-Float branch
 *   to ensure both branches are considered during coverage.
 * - The tests use non-terminating EXPECT-style patterns where appropriate to
 *   increase executed code paths without terminating the entire test run.
 *
 * Candidate Keywords (derived from the focal method and dependencies):
 * - TEST_ASSERT_EACH_EQUAL_FLOAT
 * - 1.0f, -0.123f, 25.4f, -0.2f (float literals)
 * - p0, p1, p2, p3 (float arrays)
 * - UNITY_EXCLUDE_FLOAT (branch coverage)
 * - TEST_IGNORE (branch when float support is excluded)
 * - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END (to cover failure paths)
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Prototypes for the tests in this file
void testEqualFloatEachEqual_PassAllFour(void);
void testEqualFloatEachEqual_PassSingleElement(void);
void testEqualFloatEachEqual_FailLastElement(void);
void testEqualFloatEachEqual_FailThirdElement(void);
void testEqualFloatEachEqual_ExcludeFloatBranch(void);

/* Test: Pass when all four elements are 1.0f (same as original focal case, size=4) */
void testEqualFloatEachEqual_PassAllFour(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    // If float is excluded at compile-time, skip this test path
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // All values equal to the expected 1.0f across 4 elements
    TEST_ASSERT_EACH_EQUAL_FLOAT(1.0f, p0, 4);
#endif
    // This test ensures the "all-equal" path across full array length is exercised.
}

/* Test: Pass when size=1 (single-element array equals the expected value) */
void testEqualFloatEachEqual_PassSingleElement(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f};
    TEST_ASSERT_EACH_EQUAL_FLOAT(1.0f, p0, 1);
#endif
    // Verifies minimal non-trivial path (single-element equality).
}

/* Test: Fail when the last element differs (should trigger failure path) */
void testEqualFloatEachEqual_FailLastElement(void)
{
    float p0[] = {1.0f, 1.0f, 1.0f, 2.0f}; // last element differs from 1.0f
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(1.0f, p0, 4);
    VERIFY_FAILS_END
    // Ensures the failure path detects mismatch at the final element.
}

/* Test: Fail when a middle/third element differs (different index) */
void testEqualFloatEachEqual_FailThirdElement(void)
{
    float p0[] = {1.0f, 1.0f, 0.999999f, 1.0f}; // third element differs slightly
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_FLOAT(1.0f, p0, 4);
    VERIFY_FAILS_END
    // Ensures the failure path detects mismatch in the middle of the array.
}

/* Test: Branch coverage for UNITY_EXCLUDE_FLOAT path (similar to focal behavior) */
void testEqualFloatEachEqual_ExcludeFloatBranch(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    // When floats are excluded, the test body should be skipped (TEST_IGNORE()).
    TEST_IGNORE();
#else
    // If float is available, perform a simple pass to exercise code path.
    float p0[] = {1.0f, 1.0f};
    TEST_ASSERT_EACH_EQUAL_FLOAT(1.0f, p0, 2);
#endif
}

/* Minimal test runner for this file. This allows compiling as a standalone unit
 * test binary that targets only the tests defined above. In a larger suite,
 * you would integrate RUN_TEST calls into the project's existing test harness.
 */
int main(void)
{
    UnityBegin("test_equal_float_each_equal_suite.c");

    RUN_TEST(testEqualFloatEachEqual_PassAllFour);
    RUN_TEST(testEqualFloatEachEqual_PassSingleElement);
    RUN_TEST(testEqualFloatEachEqual_FailLastElement);
    RUN_TEST(testEqualFloatEachEqual_FailThirdElement);
    RUN_TEST(testEqualFloatEachEqual_ExcludeFloatBranch);

    return UnityEnd();
}