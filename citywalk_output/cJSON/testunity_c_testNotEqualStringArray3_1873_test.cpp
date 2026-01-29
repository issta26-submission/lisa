// test_not_equal_string_array3_suite.cpp
// A focused Unity-based test suite designed to exercise the focal scenario
// testNotEqualStringArray3 from testunity.c. The goal is to validate behavior
// of TEST_ASSERT_EQUAL_STRING_ARRAY when the expected and actual string arrays
// differ, specifically at the final element. This file is written in C++11
// but uses the Unity C unit testing framework (no GoogleTest).

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Ensure C linkage for Unity's setup/teardown hooks when compiled under C++
#ifdef __cplusplus
extern "C" {
#endif
void setUp(void);
void tearDown(void);
#ifdef __cplusplus
}
#endif

// Candidate Keywords (Step 1)
//
// - testStrings: actual array under test.
// - expStrings: expected array to compare against.
// - 4: length parameter passed to TEST_ASSERT_EQUAL_STRING_ARRAY.
// - TEST_ASSERT_EQUAL_STRING_ARRAY: Unity macro under test.
//
// These keywords map to the core dependencies of the focal method and are used
// to drive test coverage: ensure both matching and mismatching cases are exercised.


// Test 1: NotEqualStringArray3_FocalEquivalent
// Description: Replicates the focal scenario where the actual array ends with NULL
// and the expected array ends with a non-NULL value ("zoo"). This should fail
// the assertion, exercising the NOT-EQUAL path of the macro.
static void testNotEqualStringArray3_FocalEquivalent(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", NULL };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };
    // This assertion is expected to fail, matching the focal method's intent.
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
}

// Test 2: NotEqualStringArray3_EqualCase
// Description: Positive path where both arrays are identical. Verifies that
// TEST_ASSERT_EQUAL_STRING_ARRAY passes when arrays are equal, covering the
// true-branch of the comparison.
static void testNotEqualStringArray3_EqualCase(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "zoo" };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
}

// Test 3: NotEqualStringArray3_LastElementMismatch
// Description: Mismatch occurs at the last element (index 3). Ensures the
// macro detects a difference when only the last element differs.
static void testNotEqualStringArray3_LastElementMismatch(void)
{
    const char *testStrings[] = { "foo", "boo", "woo", "baz" };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
}


// Global setup/teardown (optional for this suite)
void setUp(void)
{
    // No global setup required for these tests
}
void tearDown(void)
{
    // No global teardown required for these tests
}


// Main test runner for this suite
int main(void)
{
    UNITY_BEGIN("test_not_equal_string_array3_suite.cpp");

    // Run tests. Each test contributes to coverage of the focal macros and
    // their behavior on true/false branches.
    RUN_TEST(testNotEqualStringArray3_FocalEquivalent);
    RUN_TEST(testNotEqualStringArray3_EqualCase);
    RUN_TEST(testNotEqualStringArray3_LastElementMismatch);

    return UNITY_END();
}