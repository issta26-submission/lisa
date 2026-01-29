// crlf_suite.cpp
// A small Unity-based test suite focused on validating string equality
// including carriage return and line feed sequences. This suite also
// exercises the focal testEqualStringsWithCarriageReturnsAndLineFeeds
// function located in testunity.c (via its external test entry).

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of the focal test function from testunity.c
// This function contains tests asserting that strings with CRLF are
// treated as literal content and compared correctly.
void testEqualStringsWithCarriageReturnsAndLineFeeds(void);

#ifdef __cplusplus
}
#endif

// Additional independent tests to increase coverage around CRLF handling.
// These tests exercise various ways of constructing strings containing
// CRLF and verify that Unity's TEST_ASSERT_EQUAL_STRING handles them
// as ordinary characters sequences (not as special tokens).

// Verifies that two identical literals containing CRLF are considered equal.
void testCRLFEquality_DirectLiteralVsLiteral(void)
{
    // true positive: both sides contain the same CRLF-containing literal
    TEST_ASSERT_EQUAL_STRING("foo\r\nbar", "foo\r\nbar");
}

// Verifies that two dynamically created strings containing CRLF are equal.
void testCRLFEquality_DynamicVsDynamic(void)
{
    // two separate pointers but with identical CRLF-containing content
    const char *a = "foo\r\nbar";
    const char *b = "foo\r\nbar";
    TEST_ASSERT_EQUAL_STRING(a, b);
}

// Verifies that a dynamic string containing CRLF is equal to a literal CRLF string.
void testCRLFEquality_DynamicVsLiteral(void)
{
    const char *a = "foo\r\nbar";
    TEST_ASSERT_EQUAL_STRING("foo\r\nbar", a);
}

// Verifies that empty strings containing CRLF-like content (empty) are equal.
void testCRLFEquality_EmptyStrings(void)
{
    TEST_ASSERT_EQUAL_STRING("", "");
}

// Optional: a negative scenario (commented out to avoid halting the test suite).
// If enabled, this would verify that a mismatch in CRLF-containing strings fails as expected.
// Note: Running a failing test will mark the suite as failed according to Unity's harness.
// void testCRLFEquality_NotEqual_ShouldFail(void)
// {
//     TEST_ASSERT_EQUAL_STRING("foo\r\nbar", "foo\r\nbaz");
// }

int main(void)
{
    // Initialize the Unity test framework
    UnityBegin("crlf_suite.cpp");

    // Execute the focal test from testunity.c to ensure its CRLF behavior remains intact.
    RUN_TEST(testEqualStringsWithCarriageReturnsAndLineFeeds);

    // Run additional CRLF-related test cases to enhance coverage.
    RUN_TEST(testCRLFEquality_DirectLiteralVsLiteral);
    RUN_TEST(testCRLFEquality_DynamicVsDynamic);
    RUN_TEST(testCRLFEquality_DynamicVsLiteral);
    RUN_TEST(testCRLFEquality_EmptyStrings);

    // Finalize and return the test results.
    return UnityEnd();
}