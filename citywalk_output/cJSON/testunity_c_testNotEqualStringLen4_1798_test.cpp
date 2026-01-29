// Test suite for the focal method testNotEqualStringLen4
// This suite augments coverage around TEST_ASSERT_EQUAL_STRING_LEN by
// exercising true/false branches of the predicate and null handling.
// Follows Unity C test conventions and avoids GTest.

// Notes:
// - Candidate Keywords: TEST_ASSERT_EQUAL_STRING_LEN, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
// - Core dependencies (from <FOCAL_CLASS_DEP>): unity.h, string.h
// - This file can be compiled and linked with Unity as a separate translation unit.
// - Tests are invoked from main() using RUN_TEST, as per <DOMAIN_KNOWLEDGE> guidance.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Standard Unity setup/teardown hooks (no per-test state needed here)
static void setUp(void) { /* no setup required for these tests */ }
static void tearDown(void) { /* no teardown required for these tests */ }

// 1) Reproduce the focal case: strings differ within the specified length, expecting abort
void testNotEqualStringLen4_Additional1(void)
{
    // Core predicate: TEST_ASSERT_EQUAL_STRING_LEN(expected, actual, len)
    // Expect an abort since "ba\r\x16" != "ba\r\n" within first 4 bytes
    // This mirrors the original focal test in testunity.c
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("ba\r\x16", "ba\r\n", 4);
    VERIFY_FAILS_END
}

// 2) Positive path: equal strings within the specified length should not abort
void testNotEqualStringLen4_PassWhenEqualWithin4(void)
{
    // Both strings are equal for the first 4 bytes; no abort should occur
    TEST_ASSERT_EQUAL_STRING_LEN("ba\r\n", "ba\r\n", 4);
}

// 3) Null handling: expected string is NULL should abort
void testNotEqualStringLen4_ExpectedNull_ShouldFail(void)
{
    // When expected string is NULL, the assertion should fail (abort)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN(NULL, "bar", 4);
    VERIFY_FAILS_END
}

// 4) Null handling: actual string is NULL should abort
void testNotEqualStringLen4_ActualNull_ShouldFail(void)
{
    // When actual string is NULL, the assertion should fail (abort)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("bar", NULL, 4);
    VERIFY_FAILS_END
}

// 5) Equality within length boundary: ensure that equality within len causes no abort
void testNotEqualStringLen4_EqualityWithin4Chars(void)
{
    // Equality within the specified 4 characters should not abort
    TEST_ASSERT_EQUAL_STRING_LEN("ba\r\n", "ba\r\n", 4);
}

// Main runner: explicitly invoke the tests to maximize portability with non-GTest setups
int main(void)
{
    UnityBegin("test_NotEqualStringLen4_suite.cpp");

    RUN_TEST(testNotEqualStringLen4_Additional1);
    RUN_TEST(testNotEqualStringLen4_PassWhenEqualWithin4);
    RUN_TEST(testNotEqualStringLen4_ExpectedNull_ShouldFail);
    RUN_TEST(testNotEqualStringLen4_ActualNull_ShouldFail);
    RUN_TEST(testNotEqualStringLen4_EqualityWithin4Chars);

    return UnityEnd();
}