// Test suite for the focal method: testNotEqualString3
// Context:
// - The focal method is a Unity-based test (C) that asserts a string equality failure.
// - The testNotEqualString3 function is located in testunity.c and uses Unity macros
//   EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to validate that TEST_ASSERT_EQUAL_STRING("", "bar")
//   properly triggers a failure (since "" != "bar").
// - This C++11 test file (test_suite.cpp) leverages Unity's C interface to execute the focal
//   test and also provides an independent external reproduction of the same abort-path
//   to increase coverage across multiple test entry points without modifying testunity.c.
//
// Notes:
// - The test suite is designed to compile with a C++11 compiler, but it remains compatible
//   with the Unity C framework. No GTest is used.
// - Explanatory comments are provided for each unit test to clarify intent and coverage.

#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


extern "C" {
}

// Forward declaration of the focal test (as defined in testunity.c)
extern "C" void testNotEqualString3(void);

// -----------------------------------------------------------------------------
// Auxiliary test: reproduce the focal logic externally (without touching testunity.c).
// This re-uses the same Unity abort-path to validate not-equal strings (-
// important for ensuring coverage of the abort path and the failure messaging).
// -----------------------------------------------------------------------------
static void testNotEqualString3_exercised(void)
{
    // This test mirrors the focal scenario: comparing "" with "bar" should fail.
    // We expect an abort (TEST_ASSERT_EQUAL_STRING should fail), and the test
    // harness confirms the abort via Unity's EXPECT_ABORT_BEGIN / VERIFY_FAILS_END.
    // This test is intentionally isolated to maximize code coverage of the
    // not-equal string path without altering testunity.c's original test.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING("", "bar");
    VERIFY_FAILS_END
}

// -----------------------------------------------------------------------------
// Main entrypoint: initialize Unity, run focal test and the auxiliary reproduction.
// This ensures the focal method's behavior is exercised in isolation as well as
// under an external reproduction path for better coverage in a C++ test harness.
// -----------------------------------------------------------------------------
int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Run the focal test as defined in testunity.c
    // This validates that the testNotEqualString3 path aborts as expected when the strings differ.
    RUN_TEST(testNotEqualString3);

    // Run our external reproduction to increase coverage of the same abort-path logic.
    RUN_TEST(testNotEqualString3_exercised);

    // Finalize and return the test results
    return UNITY_END();
}