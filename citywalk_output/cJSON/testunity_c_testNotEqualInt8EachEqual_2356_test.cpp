// Test suite extension for the focal method: testNotEqualInt8EachEqual
// This file adds additional test cases to exercise the behavior of
// TEST_ASSERT_EACH_EQUAL_INT8 with various inputs, aiming to improve coverage
// of the not-equal path and its edge cases.
// 
// Notes and rationale:
// - We rely on the Unity testing framework macros already present in the
//   project (e.g., EXPECT_ABORT_BEGIN, VERIFY_FAILS_END, TEST_ASSERT_EACH_EQUAL_INT8).
// - The tests exercise true (pass) and false (abort) branches of the predicate
//   by using different array contents and lengths.
// - We intentionally use non-terminating assertions (no EXPECT_ABORT_BEGIN) for
//   cases that should pass, and wrap failure cases with EXPECT_ABORT_BEGIN/VERIFY_FAILS_END
//   to validate that aborts occur as expected.
// - The tests are written in C++11-compatible style but rely on the C Unity API for
//   compatibility with the existing test suite in testunity.c.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations (Unity requires test functions to be visible to the test runner)
void testNotEqualInt8EachEqual_Ext_AllMatch_Pass(void);
void testNotEqualInt8EachEqual_Ext_FirstMismatch_Fail(void);
void testNotEqualInt8EachEqual_Ext_NullActual(void);
void testNotEqualInt8EachEqual_Ext_LengthOne_Pass(void);
void testNotEqualInt8EachEqual_Ext_LengthTwo_Fail(void);

#ifdef __cplusplus
}
#endif

// Basic setUp/tearDown required by Unity (can stay empty for these tests)
void setUp(void) {}
void tearDown(void) {}

// Test 1: All elements match the expected value; should pass (no abort)
void testNotEqualInt8EachEqual_Ext_AllMatch_Pass(void)
{
    // Candidate Keywords: UNITY_INT8, TEST_ASSERT_EACH_EQUAL_INT8, 1, 4
    // Purpose: Validate the positive path where every element equals the target value.
    UNITY_INT8 p0[] = {1, 1, 1, 1};
    // Expect no abort; this should pass through TEST_ASSERT_EACH_EQUAL_INT8.
    TEST_ASSERT_EACH_EQUAL_INT8(1, p0, 4);
}

// Test 2: First element matches but a later element does not; should abort
void testNotEqualInt8EachEqual_Ext_FirstMismatch_Fail(void)
{
    // Candidate Keywords: UNITY_INT8, TEST_ASSERT_EACH_EQUAL_INT8, mismatch, 4
    // Purpose: Ensure that a single mismatch in the array triggers an abort.
    UNITY_INT8 p0[] = {1, 2, 1, 1};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT8(1, p0, 4);
    VERIFY_FAILS_END
}

// Test 3: Null actual pointer should trigger abort
void testNotEqualInt8EachEqual_Ext_NullActual(void)
{
    // Candidate Keywords: NULL, TEST_ASSERT_EACH_EQUAL_INT8, abort
    // Purpose: Verify that passing a NULL pointer for the actual array causes abort.
    UNITY_INT8* p0 = NULL;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT8(1, p0, 4);
    VERIFY_FAILS_END
}

// Test 4: Length of 1 with a matching first element should pass
void testNotEqualInt8EachEqual_Ext_LengthOne_Pass(void)
{
    // Candidate Keywords: length=1, p0[0]==target
    // Purpose: Ensure that length=1 cases behave correctly (only first element checked).
    UNITY_INT8 p0[] = {7, 8, 9, 10};
    TEST_ASSERT_EACH_EQUAL_INT8(7, p0, 1);
}

// Test 5: Length of 2 with a mismatch in the checked portion should abort
void testNotEqualInt8EachEqual_Ext_LengthTwo_Fail(void)
{
    // Candidate Keywords: length=2, mismatch within first 2 elements
    // Purpose: Validate abort when a mismatch occurs within the specified length window.
    UNITY_INT8 p0[] = {7, 8, 7, 7};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT8(7, p0, 2);
    VERIFY_FAILS_END
}

// Simple test runner
int main(void)
{
    UNITY_BEGIN();

    // Run added tests to extend coverage for testNotEqualInt8EachEqual
    RUN_TEST(testNotEqualInt8EachEqual_Ext_AllMatch_Pass);
    RUN_TEST(testNotEqualInt8EachEqual_Ext_FirstMismatch_Fail);
    RUN_TEST(testNotEqualInt8EachEqual_Ext_NullActual);
    RUN_TEST(testNotEqualInt8EachEqual_Ext_LengthOne_Pass);
    RUN_TEST(testNotEqualInt8EachEqual_Ext_LengthTwo_Fail);

    return UNITY_END();
}