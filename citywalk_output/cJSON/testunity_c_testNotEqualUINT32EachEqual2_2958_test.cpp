/*
  Comprehensive test suite for the focal method testNotEqualUINT32EachEqual2
  - Focus: UNITY_UINT32 and TEST_ASSERT_EACH_EQUAL_UINT32 behavior when not all
    elements in an array are equal to the target value (987 in this case).
  - Rationale: Exercise various mismatch positions to ensure the macro aborts as soon
    as a non-matching element is encountered, and also cover a positive path where all
    elements match.
  - This file is meant to be compiled with a C++11 toolchain, using the Unity C unit test framework.
  - No GTest or GoogleMock usage; Unity macros are exercised directly.
  - The test suite complements the existing testNotEqualUINT32EachEqual2() in testunity.c by
    adding additional coverage scenarios.
  - Static and class dependencies are not directly invoked here; tests rely on Unity's
    assertion and abort scaffolding exposed via Unity macros.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// If there are any needed declarations from the focal project, declare here.
// For this isolated suite, Unity's macros and UNITY_UINT32 type suffice.

/*
  Test 1: Basic mismatch at the first element
  - p0 = {1, 987, 987, 987}
  - Expect TEST_ASSERT_EACH_EQUAL_UINT32 to fail (abort) since first element is not 987.
*/
void testNotEqualUINT32EachEqual2_BasicMismatchFirst(void)
{
    UNITY_UINT32 p0[] = {1, 987, 987, 987};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT32(987, p0, 4);
    VERIFY_FAILS_END
}

/*
  Test 2: Mismatch at the second element
  - p0 = {987, 1, 987, 987}
  - Expect abort because not all elements equal 987 (second element differs).
*/
void testNotEqualUINT32EachEqual2_MismatchSecond(void)
{
    UNITY_UINT32 p0[] = {987, 1, 987, 987};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT32(987, p0, 4);
    VERIFY_FAILS_END
}

/*
  Test 3: Mismatch at the third element
  - p0 = {987, 987, 1, 987}
  - Expect abort because not all elements equal 987 (third element differs).
*/
void testNotEqualUINT32EachEqual2_MismatchThird(void)
{
    UNITY_UINT32 p0[] = {987, 987, 1, 987};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT32(987, p0, 4);
    VERIFY_FAILS_END
}

/*
  Test 4: Mismatch at the fourth element
  - p0 = {987, 987, 987, 1}
  - Expect abort because not all elements equal 987 (fourth element differs).
*/
void testNotEqualUINT32EachEqual2_MismatchFourth(void)
{
    UNITY_UINT32 p0[] = {987, 987, 987, 1};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT32(987, p0, 4);
    VERIFY_FAILS_END
}

/*
  Test 5: All elements equal (positive path)
  - p0 = {987, 987, 987, 987}
  - Expect the assertion to pass (no abort) since all elements match 987.
*/
void testNotEqualUINT32EachEqual2_AllEqual(void)
{
    UNITY_UINT32 p0[] = {987, 987, 987, 987};
    // This should pass without abort since every element matches the expected value.
    TEST_ASSERT_EACH_EQUAL_UINT32(987, p0, 4);
}

/*
  Minimal test harness: run the above tests using Unity's RUN_TEST mechanism.
  Note: The original testunity.c likely contains its own test runner; this file provides
  an additional, self-contained runner to exercise the focal macros more thoroughly.
*/
int main(void)
{
    UnityBegin("test_not_equal_uint32_each_equal2_additional_tests.cpp");

    RUN_TEST(testNotEqualUINT32EachEqual2_BasicMismatchFirst);
    RUN_TEST(testNotEqualUINT32EachEqual2_MismatchSecond);
    RUN_TEST(testNotEqualUINT32EachEqual2_MismatchThird);
    RUN_TEST(testNotEqualUINT32EachEqual2_MismatchFourth);
    RUN_TEST(testNotEqualUINT32EachEqual2_AllEqual);

    return UnityEnd();
}