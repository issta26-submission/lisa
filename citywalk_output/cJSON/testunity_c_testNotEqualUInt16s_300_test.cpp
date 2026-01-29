/************************************************************
 * Unit Test Suite for: testNotEqualUInt16s
 * Context: Unity-based C unit tests integrated in a C++11 project.
 * Objective: Extend coverage for the focal method
 *            testNotEqualUInt16s (ensuring not-equal_uint16 case aborts).
 * Notes:
 *  - We provide multiple variations to exercise different
 *    ways to access the uint16 values (direct, via pointers, via array).
 *  - All tests that expect a failure use the EXPECT_ABORT_BEGIN /
 *    VERIFY_FAILS_END pattern to maximize code coverage without
 *    terminating the test runner prematurely.
 *  - This suite is designed to be compiled with Unity (no GTest).
 ************************************************************/

// Candidate Keywords extracted from the focal method
// - UNITY_UINT16, TEST_ASSERT_EQUAL_UINT16
// - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
// - v0, v1, pointers, array access
// - TEST_ASSERT_EQUAL_UINT16(v0, v1) as the assertion under test

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

// Lightweight setup/teardown to satisfy Unity's expectations
void setUp(void) {}
void tearDown(void) {}

// Test 1: Basic not-equal scenario using direct variables
// Purpose: Validate that TEST_ASSERT_EQUAL_UINT16 fails when two distinct
//          16-bit values are compared, using the exact pattern from the focal method.
void testNotEqualUInt16s_BasicDifferentValues(void)
{
    UNITY_UINT16 v0 = 0xFFFF; // 65535
    UNITY_UINT16 v1 = 0xFFFE; // 65534
    // Expect the assertion to fail and abort the test
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16(v0, v1);
    VERIFY_FAILS_END
}

// Test 2: Not-equal scenario using explicit border values
// Purpose: Ensure not-equality at a different pair of boundary values is detected.
void testNotEqualUInt16s_BorderValues(void)
{
    UNITY_UINT16 v0 = 0x0000; // 0
    UNITY_UINT16 v1 = 0x0001; // 1
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16(v0, v1);
    VERIFY_FAILS_END
}

// Test 3: Not-equal scenario via pointers
// Purpose: Confirm that dereferencing pointers yields the same not-equal failure.
void testNotEqualUInt16s_ViaPointers(void)
{
    UNITY_UINT16 a = 0xFFFF;
    UNITY_UINT16 b = 0xFFFE;
    UNITY_UINT16 *pa = &a;
    UNITY_UINT16 *pb = &b;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16(*pa, *pb);
    VERIFY_FAILS_END
}

// Test 4: Not-equal scenario via an array
// Purpose: Validate the same condition when values are accessed through an array element.
void testNotEqualUInt16s_ViaArray(void)
{
    UNITY_UINT16 vals[2] = { 0xFFFF, 0xFFFE };
    UNITY_UINT16 *pv0 = &vals[0];
    UNITY_UINT16 *pv1 = &vals[1];
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT16(*pv0, *pv1);
    VERIFY_FAILS_END
}

// Main runner for the above tests
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testNotEqualUInt16s_BasicDifferentValues);
    RUN_TEST(testNotEqualUInt16s_BorderValues);
    RUN_TEST(testNotEqualUInt16s_ViaPointers);
    RUN_TEST(testNotEqualUInt16s_ViaArray);

    return UNITY_END();
}