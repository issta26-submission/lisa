// File: test_NotEqualString5_suite.cpp
// Purpose: Provide a focused, self-contained test suite (in C++11 compatible code)
//          that exercises the behavior demonstrated by the focal testNotEqualString5
//          from testunity.c, using the Unity C framework macros.
// Notes:
// - This suite uses extern "C" linkage for the focal test components to ensure
//   compatibility when linked with the C-based Unity test runner.
// - The tests deliberately replicate the Not-Equal-Strings scenario to validate
//   the abort/fail path (via EXPECT_ABORT_BEGIN / VERIFY_FAILS_END).
// - The suite demonstrates coverage of the core dependent components (Candidate
//   Keywords) used by testNotEqualString5: str1, str2, TEST_ASSERT_EQUAL_STRING,
//   EXPECT_ABORT_BEGIN, VERIFY_FAILS_END.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Prototypes for focal C tests (from testunity.c would exist in the real project)
void testNotEqualString5(void);
#ifdef __cplusplus
}
#endif

// Candidate Keywords extracted from the focal method under test
// - str1, str2: two test strings with deliberate byte-level differences
// - TEST_ASSERT_EQUAL_STRING: comparison primitive under test
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: control flow to capture aborts as test outcomes

// Wrapper #1: Replicates the exact Not-Equal-Strings scenario used by testNotEqualString5
// This test ensures that two distinct 4-byte strings, differing in a non-terminating
// byte (0x03 vs 0x04), cause an expected assertion failure.
void testNotEqualString5_Wrapper(void)
{
    // Candidate Keywords: str1, str2, TEST_ASSERT_EQUAL_STRING, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
    const char str1[] = { 0x41, 0x42, 0x03, 0x00 }; // "AB" + 0x03 + null terminator
    const char str2[] = { 0x41, 0x42, 0x04, 0x00 }; // "AB" + 0x04 + null terminator

    // Begin test that should abort due to string inequality
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING(str1, str2);
    VERIFY_FAILS_END
}

// Wrapper #2: Additional variant to broaden coverage of the not-equal string scenario
// This uses a different differing byte (0x03 vs 0x03) to validate that change in a
// different position is still treated as a mismatch (and aborts as expected).
void testNotEqualString5_Variant_Wrapper(void)
{
    // Two strings that differ in the second byte after 'A' 'B' (0x42 vs 0x43)
    const char str1[] = { 0x41, 0x42, 0x03, 0x00 }; // "AB" + 0x03 + null terminator
    const char str2[] = { 0x41, 0x43, 0x03, 0x00 }; // "AC" + 0x03 + null terminator

    // Begin test that should abort due to string inequality
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING(str1, str2);
    VERIFY_FAILS_END
}

// Main runner for standalone execution (not strictly required if integrated into an existing
// Unity-based test runner, but provided here to ensure this file can be executed independently.
// This respects the standard Unity main flow and invokes both wrappers to maximize coverage.
int main(void)
{
    UNITY_BEGIN();

    // Execute the focal Not-Equal-String test wrappers to verify abort behavior.
    // Each test is documented to explain the purpose and the Candidate Keywords it exercises.
    RUN_TEST(testNotEqualString5_Wrapper);      // Verifies abort when two strings differ at a non-terminating byte
    RUN_TEST(testNotEqualString5_Variant_Wrapper); // Verifies abort with an alternate single-byte difference

    return UNITY_END();
}