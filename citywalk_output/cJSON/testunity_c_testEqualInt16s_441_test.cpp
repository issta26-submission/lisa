/*
 * Test Suite: Enhanced coverage for testEqualInt16s behavior
 * Focus: Validate various forms of equality checks for 16-bit signed integers
 * Context: Based on the focal method testEqualInt16s from testunity.c
 * Approach: Provide additional non-GTest Unity-based tests to exercise
 *           multiple true-branch paths and to safely capture false-branch paths
 *           using EXPECT_ABORT_BEGIN / VERIFY_FAILS_END patterns.
 * Notes:
 * - This file is designed to be compiled with a C/C++11 toolchain
 *   using Unity (no GTest).
 * - The tests use standard Unity macros and stdint types to ensure portability
 *   across platforms.
 * - Static members and private/internal functions from the original focal unit
 *   are not accessed here; we rely on public Unity test constructs.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Optional: provide empty setUp/tearDown for Unity compatibility
void setUp(void)   { }
void tearDown(void){ }

// Test 1: Verify testEqualInt16s behavior with positive identical values
// This mirrors the positive-path coverage of the focal test but uses explicit
// int16_t types and explicit pointers to exercise address-based cases.
void testEqualInt16s_WithSamePositiveValues(void)
{
    int16_t v0 = 0x7876;
    int16_t v1 = 0x7876;
    int16_t *p0 = &v0;
    int16_t *p1 = &v1;

    // Direct value equality checks in various forms
    TEST_ASSERT_EQUAL_INT16(0x7876, 0x7876);
    TEST_ASSERT_EQUAL_INT16(v0, v1);
    TEST_ASSERT_EQUAL_INT16(0x7876, v1);
    TEST_ASSERT_EQUAL_INT16(v0, 0x7876);

    // Pointer-based equality checks
    TEST_ASSERT_EQUAL_INT16(*p0, v1);
    TEST_ASSERT_EQUAL_INT16(*p0, *p1);
    TEST_ASSERT_EQUAL_INT16(*p0, 0x7876);
}

// Test 2: Verify testEqualInt16s behavior with negative identical values
// Ensures that negative 16-bit values are handled correctly by the equality macro.
void testEqualInt16s_WithSameNegativeValues(void)
{
    int16_t v0 = -0x7876; // -30838 in decimal
    int16_t v1 = -0x7876;
    int16_t *p0 = &v0;
    int16_t *p1 = &v1;

    // Direct and indirect equality checks
    TEST_ASSERT_EQUAL_INT16(-0x7876, -0x7876);
    TEST_ASSERT_EQUAL_INT16(v0, v1);
    TEST_ASSERT_EQUAL_INT16(-0x7876, v1);
    TEST_ASSERT_EQUAL_INT16(v0, -0x7876);

    TEST_ASSERT_EQUAL_INT16(*p0, v1);
    TEST_ASSERT_EQUAL_INT16(*p0, *p1);
    TEST_ASSERT_EQUAL_INT16(*p0, -0x7876);
}

// Test 3: Negative path - failure when values differ
// This ensures the test harness can capture a failure path using Unity's
// non-terminating assertion capture (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END)
void testNotEqualInt16s_ShouldFail_WithDirectValues(void)
{
    int16_t v0 = 0x7876;
    int16_t v1 = 0x7877; // different value
    // Intentionally trigger a failure to verify the harness handles it
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT16(0x7876, 0x7877);
    VERIFY_FAILS_END
}

// Test 4: Negative path - failure when values differ via pointers
// Use pointers to show the failure path also surfaces when using dereferenced values.
void testNotEqualInt16s_ShouldFail_WithPointers(void)
{
    int16_t v0 = 0x7876;
    int16_t v1 = 0x7877; // different value
    int16_t *p0 = &v0;
    int16_t *p1 = &v1;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT16(*p0, *p1); // dereferenced values differ
    VERIFY_FAILS_END
}

// Test 5: Zero equality case to cover edge values
// Ensures that zero comparisons behave identically across all forms used
// in the focal test (direct, through variables and through pointers).
void testEqualInt16s_WithZero(void)
{
    int16_t v0 = 0;
    int16_t v1 = 0;
    int16_t *p0 = &v0;
    int16_t *p1 = &v1;

    TEST_ASSERT_EQUAL_INT16(0, 0);
    TEST_ASSERT_EQUAL_INT16(v0, v1);
    TEST_ASSERT_EQUAL_INT16(0, v1);
    TEST_ASSERT_EQUAL_INT16(v0, 0);

    TEST_ASSERT_EQUAL_INT16(*p0, v1);
    TEST_ASSERT_EQUAL_INT16(*p0, *p1);
    TEST_ASSERT_EQUAL_INT16(*p0, 0);
}

// Runner for the test suite
// This main can be compiled and linked to Unity's runtime to execute the tests.
// It is suitable for being compiled as part of a C/C++11 project without GTest.
int main(void)
{
    UnityBegin("test_equal_int16s_suite.c");

    RUN_TEST(testEqualInt16s_WithSamePositiveValues);
    RUN_TEST(testEqualInt16s_WithSameNegativeValues);
    RUN_TEST(testNotEqualInt16s_ShouldFail_WithDirectValues);
    RUN_TEST(testNotEqualInt16s_ShouldFail_WithPointers);
    RUN_TEST(testEqualInt16s_WithZero);

    return UnityEnd();
}