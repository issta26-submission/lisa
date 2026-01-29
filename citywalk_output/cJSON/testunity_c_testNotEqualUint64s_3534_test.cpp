/*
  Test Suite: testNotEqualUint64s variations
  Context:
  - Focus: Validate behavior when 64-bit integers are not equal, mirroring the focal method testNotEqualUint64s.
  - Environment: Unity framework (UNITY_SUPPORT_64 governs 64-bit path). When 64-bit support is unavailable, tests are ignored.
  - Core dependencies reflected as Candidate Keywords:
    - UNITY_SUPPORT_64, UNITY_UINT64, TEST_ASSERT_EQUAL_UINT64
    - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
    - v0, v1 values as 64-bit constants
  - Design: Provide multiple not-equal scenarios to exercise failure paths and one not-equal variant to verify different 64-bit literals are treated distinctly.
  - Note: This file is self-contained and uses Unity’s main runner. No GTest/GMock.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// If 64-bit support is unavailable, mark tests to be ignored.
#ifndef UNITY_SUPPORT_64
#define TESTS_MAY_BE_64BIT 0
#else
#define TESTS_MAY_BE_64BIT 1
#endif

// Empty setup/teardown as per Unity conventions
void setUp(void) {}
void tearDown(void) {}

/*
  Test 1: Not Equal - baseline 64-bit values
  Purpose: Ensure that two different 64-bit values cause TEST_ASSERT_EQUAL_UINT64 to fail,
           triggering Unity's abort path as expected.
*/
void testNotEqualUint64s_Variant01(void)
{
#if !TESTS_MAY_BE_64BIT
    // If 64-bit is not available, skip this test
    TEST_IGNORE();
#else
    UNITY_UINT64 v0 = 9000000000ULL;
    UNITY_UINT64 v1 = 9100000000ULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT64(v0, v1);
    VERIFY_FAILS_END
#endif
}

/*
  Test 2: Not Equal - large 64-bit values with different literals
  Purpose: Another not-equal scenario to increase coverage over distinct literal representations.
*/
void testNotEqualUint64s_Variant02(void)
{
#if !TESTS_MAY_BE_64BIT
    TEST_IGNORE();
#else
    UNITY_UINT64 v0 = 1234567890123ULL;
    UNITY_UINT64 v1 = 1234567890124ULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT64(v0, v1);
    VERIFY_FAILS_END
#endif
}

/*
  Test 3: Not Equal - equal values to exercise failure path for equality assertion
  Purpose: Ensure that when values are equal, the same assertion still aborts, validating the
           correctness of the failure path when an equality check is violated (edge case).
*/
void testNotEqualUint64s_Variant03(void)
{
#if !TESTS_MAY_BE_64BIT
    TEST_IGNORE();
#else
    UNITY_UINT64 v0 = 0x1FFFFFFFFFFFFFULL; // large 64-bit literal
    UNITY_UINT64 v1 = 0x1FFFFFFFFFFFFFULL; // intentionally equal

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT64(v0, v1);
    VERIFY_FAILS_END
#endif
}

/* Main runner for the test suite. Executes all variants above. */
int main(void)
{
    UNITY_BEGIN("testNotEqualUint64s_variants_suite.c");

    RUN_TEST(testNotEqualUint64s_Variant01);
    RUN_TEST(testNotEqualUint64s_Variant02);
    RUN_TEST(testNotEqualUint64s_Variant03);

    return UNITY_END();
}