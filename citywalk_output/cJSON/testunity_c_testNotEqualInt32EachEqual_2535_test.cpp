// New supplementary test cases for the focal method TEST_ASSERT_EACH_EQUAL_INT32
// Location: testNotEqualInt32EachEqual_additional.c
// This file adds additional scenarios to exercise the negative (abort) path of
// TEST_ASSERT_EACH_EQUAL_INT32, ensuring robust coverage of the dependent macro.
//
// The tests follow the Unity framework conventions and are designed to compile
// under C11 without GTest/GMock dependency.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// No global static state required for these tests; provide empty setups.
void setUp(void) {}
void tearDown(void) {}

// 1) First element mismatches the expected value
// Rationale: Ensure the abort path is triggered immediately on the first non-matching element.
void testNotEqualInt32EachEqual_FirstElementMismatch(void)
{
    UNITY_INT32 p0[] = {127, 126, 127, 127}; // element 1 differs from 127
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT32(127, p0, 4);
    VERIFY_FAILS_END
}

// 2) Second element mismatches the expected value
// Rationale: Validate that mismatch in a middle position also triggers abort correctly.
void testNotEqualInt32EachEqual_SecondElementMismatch(void)
{
    UNITY_INT32 p0[] = {127, 13, 127, 127}; // element 1 differs from 127 (second position)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT32(127, p0, 4);
    VERIFY_FAILS_END
}

// 3) Last element mismatches the expected value
// Rationale: Confirm that a late mismatch still aborts and reports failure as expected.
void testNotEqualInt32EachEqual_LastElementMismatch(void)
{
    UNITY_INT32 p0[] = {127, 127, 127, 0x7F}; // last element differs from 127
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT32(127, p0, 4);
    VERIFY_FAILS_END
}

// 4) Actual array is NULL (null pointer path)
// Rationale: Ensure the abort path is taken when the actual array pointer is NULL.
void testNotEqualInt32EachEqual_NullActual(void)
{
    UNITY_INT32* p0 = NULL;
    // Expect abort because expected value 127 cannot be compared against a NULL array.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT32(127, p0, 4);
    VERIFY_FAILS_END
}

// 5) Length zero should provoke abort (consistent with similar tests for other types in the file)
// Rationale: Verify that a zero-length array triggers the abort path in this specific test suite pattern.
void testNotEqualInt32EachEqual_LengthZero(void)
{
    UNITY_INT32 p0[1] = {1}; // actual data exists, but length is forced to 0 in the call
    // As per existing pattern in NotEqualEachEqualLengthZero tests, expect abort
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT32(0, p0, 0);
    VERIFY_FAILS_END
}

// 6) Large 32-bit values and negatives to ensure type handling and sign/overflow considerations
// Rationale: Exercise the macro with a variety of 32-bit boundaries to ensure no undefined behavior.
void testNotEqualInt32EachEqual_WithLargeAndNegativeValues(void)
{
    UNITY_INT32 p0[] = {INT32_MAX, INT32_MIN, 0, INT32_MAX}; // mismatches exist, not all equal to INT32_MAX
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_INT32(INT32_MAX, p0, 4);
    VERIFY_FAILS_END
}

// Test runner to execute the added tests.
// Depending on the project build, you may integrate these RUN_TEST calls into an
// existing test runner. Here we provide a standalone main for modular compilation.

int main(void)
{
    UnityBegin("testNotEqualInt32EachEqual_additional.c");

    RUN_TEST(testNotEqualInt32EachEqual_FirstElementMismatch);
    RUN_TEST(testNotEqualInt32EachEqual_SecondElementMismatch);
    RUN_TEST(testNotEqualInt32EachEqual_LastElementMismatch);
    RUN_TEST(testNotEqualInt32EachEqual_NullActual);
    RUN_TEST(testNotEqualInt32EachEqual_LengthZero);
    RUN_TEST(testNotEqualInt32EachEqual_WithLargeAndNegativeValues);

    return UnityEnd();
}