/*
  Supplemental C++11 test suite for the focal Unity test: testEqualStringEachEqual
  This file adds extended coverage around the focal test by exercising additional
  success and failure scenarios for the macro TEST_ASSERT_EACH_EQUAL_STRING.

  Rationale aligned with instructions:
  - Step 1: Candidate Keywords (embedded as comments for traceability)
    Candidate Keywords: TEST_ASSERT_EACH_EQUAL_STRING, Unity, testStrings*, "foo", "boo", "", NULL,
    EXPECT_ABORT_BEGIN, VERIFY_FAILS_END, RUN_TEST, UNITY_BEGIN, UNITY_END

  - Step 2: Unit Test Generation
    Adds extra pass/fail tests mirroring the behavior of testEqualStringEachEqual by
    using additional input arrays and lengths to exercise both true and false branches.

  - Step 3: Test Case Refinement
    Includes cases with multiple lengths, empty strings, and a NULL within the array to
    exercise robustness and error handling of the macro in non-terminating tests.

  Note: This test file is intended to be compiled with the Unity framework (C) in a C++
  project (C linkage for Unity API via extern "C"). It does not depend on GTest.
*/

extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

}

/*
  Test suite: Extra coverage for testEqualStringEachEqual behavior.

  Each test mirrors the semantics of TEST_ASSERT_EACH_EQUAL_STRING(expected, array, length)
  and asserts either pass or fail without terminating the entire test executable prematurely
  (using Unity's EXPECT_ABORT_BEGIN / VERIFY_FAILS_END where appropriate).
*/

// 1) Extra pass: simple repetition of "foo" across 4 elements (extension of original case)
void testExtraPass1(void)
{
    // This mirrors the original first case: four "foo" elements equal to "foo"
    const char *arr[] = { "foo", "foo", "foo", "foo" };
    TEST_ASSERT_EACH_EQUAL_STRING("foo", arr, 4);
}

// 2) Extra pass: longer array where all elements are identical and match the expected string
void testExtraPass2(void)
{
    // Six elements, all "bar" -> should pass
    const char *arr[] = { "bar", "bar", "bar", "bar", "bar", "bar" };
    TEST_ASSERT_EACH_EQUAL_STRING("bar", arr, 6);
}

// 3) Extra pass: array of empty strings
void testExtraPass3(void)
{
    // Four empty strings -> should pass for empty string equality
    const char *arr[] = { "", "", "", "" };
    TEST_ASSERT_EACH_EQUAL_STRING("", arr, 4);
}

// 4) Extra fail: one element differs (should abort)
void testExtraFail1(void)
{
    // Last element differs: expect fail
    const char *arr[] = { "foo", "foo", "foo", "moo" };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", arr, 4);
    VERIFY_FAILS_END
}

// 5) Extra fail: NULL inside the array should trigger a failure (safely handled by Unity)
void testExtraFail2(void)
{
    // NULL in the array should cause a failure when comparing against "foo"
    const char *arr[] = { "foo", "foo", NULL, "foo" };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", arr, 4);
    VERIFY_FAILS_END
}

// 6) Extra fail: multiple mismatches across the array
void testExtraFail3(void)
{
    // Array contains two different values; at least one mismatch -> fail
    const char *arr[] = { "foo", "bar", "foo", "foo" };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", arr, 4);
    VERIFY_FAILS_END
}

// Optional: a test that ensures a fully matching array of a different length passes
void testExtraPass4(void)
{
    const char *arr[] = { "alpha", "alpha", "alpha" };
    TEST_ASSERT_EACH_EQUAL_STRING("alpha", arr, 3);
}

// Test runner entry point
int main(void)
{
    UNITY_BEGIN("test_equal_string_each_equal_extended");

    // Run extended test cases
    RUN_TEST(testExtraPass1);
    RUN_TEST(testExtraPass2);
    RUN_TEST(testExtraPass3);
    RUN_TEST(testExtraFail1);
    RUN_TEST(testExtraFail2);
    RUN_TEST(testExtraFail3);
    // Optional additional pass
    RUN_TEST(testExtraPass4);

    return UNITY_END();
}