/*
  Unit Test Suite for focal method: testNotEqualStringEachEqual3

  Context:
  - The focal method exercises Unity's TEST_ASSERT_EACH_EQUAL_STRING macro.
  - The test is designed to abort when an element of the input array does not match the expected string.
  - The original focal test uses an input array ending with NULL to trigger a failure.

  Test design rationale (Candidate Keywords):
  - TEST_ASSERT_EACH_EQUAL_STRING
  - "foo" as the expected string
  - testStrings array (containing "foo" and NULL in various positions)
  - 4 as the number of elements to compare
  - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to capture abort behavior

  Approach:
  - Provide multiple non-passing variants to ensure the abort path is exercised.
  - Each test uses EXPECT_ABORT_BEGIN/VERIFY_FAILS_END to verify that the macro aborts as expected.
  - All tests are self-contained C code compatible with C++11 compilation.
  - A small Unity-based test runner is included via RUN_TEST in main().

  Note:
  - This suite does not rely on private members or private APIs.
  - It uses only the Unity framework macros and standard C/C++ constructs.
*/

#include <stddef.h>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Variant 1: Last element is NULL, matching "foo" for first three entries should abort due to NULL
void testNotEqualStringEachEqual3_Variant1(void)
{
    // Candidate Scenario: { "foo", "foo", "foo", NULL } should abort because NULL is not equal to "foo"
    const char *testStrings[] = { "foo", "foo", "foo", NULL };
    // Expect the comparison to abort (failure path)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

// Variant 2: Fourth element differs ("bar" instead of "foo"); should abort
void testNotEqualStringEachEqual3_Variant2(void)
{
    // Candidate Scenario: { "foo", "foo", "bar", "foo" }
    const char *testStrings[] = { "foo", "foo", "bar", "foo" };
    // Expect the comparison to abort due to mismatch at index 2
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

// Variant 3: Third element differs ("bar" instead of "foo"); should abort
void testNotEqualStringEachEqual3_Variant3(void)
{
    // Candidate Scenario: { "foo", "foo", "bar", "foo" } (demonstrates another mismatch location)
    const char *testStrings[] = { "foo", "foo", "bar", "foo" };
    // Expect the comparison to abort due to mismatch at index 2
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

// Optional: A combined main runner to execute the three variants in a single test run.
// If a separate test runner is used in the build, this main can be omitted.
int main(void)
{
    UnityBegin("test_NotEqualStringEachEqual3_suite");

    RUN_TEST(testNotEqualStringEachEqual3_Variant1);
    RUN_TEST(testNotEqualStringEachEqual3_Variant2);
    RUN_TEST(testNotEqualStringEachEqual3_Variant3);

    return UnityEnd();
}