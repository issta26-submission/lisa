// Test suite for the focal method testIntsWithinDeltaAndCustomMessage located in testunity.c
// This file is designed to be compiled with a C++11 compiler against the Unity C framework.
// The goal is to exercise the behavior of TEST_ASSERT_INT_WITHIN_MESSAGE and its interaction
// with the custom message parameter, covering both passing and failing scenarios.
// Note: This test suite uses the non-terminating assertion style (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END)
// for failing cases to maximize code coverage without terminating the entire test run.

#include <unity.h>
#include <cstdio>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Candidate Keywords extracted from the focal method (Step 1):
// - TEST_ASSERT_INT_WITHIN (core assertion macro)
// - TEST_ASSERT_INT_WITHIN_MESSAGE (custom message variant)
// - "Custom Message." (string passed as the custom message)
// - Delta-based numeric comparison (absolute difference <= delta)
// - Positive/negative integer values (various ranges)
// - Resulting pass/fail behavior of within-delta checks

// Prototypes for existing focal test (declared as extern to avoid redefining Unity setup/teardown logic)
extern void testIntsWithinDeltaAndCustomMessage(void);

// Test 1: Verify that a sequence of within-delta assertions passes when a custom message is provided.
// This mirrors the focal method's successful path and ensures the custom message is accepted by the macro.
void test_IntsWithinDeltaAndCustomMessage_Passes(void)
{
    // All of these should pass because each actual value is within the specified delta of the expected value.
    TEST_ASSERT_INT_WITHIN_MESSAGE(1, 5000, 5001, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, 5000, 4996, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, 5000, 5005, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(500, 50, -440, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(2, -1, -1, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, -1, 1, "Custom Message.");
}

// Test 2: Ensure that a failure is reported when the within-delta assertion with a custom message fails
// (first assertion in the focal sequence intentionally fails).
void test_IntsWithinDeltaAndCustomMessage_FailsFirstWithCustomMessage(void)
{
    // Expect the assertion to fail, but do not abort the test run.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT_WITHIN_MESSAGE(1, 5000, 5002, "Custom Message.");
    VERIFY_FAILS_END
}

// Test 3: Ensure that a failure is reported when the within-delta assertion with a custom message fails
// (a later assertion in the focal sequence is forced to fail).
void test_IntsWithinDeltaAndCustomMessage_FailsLastWithCustomMessage(void)
{
    // Force a failure on a later within-delta check to exercise failure reporting with custom message.
    EXPECT_ABORT_BEGIN
    // Choose values that exceed the delta: absolute difference must be > 5
    TEST_ASSERT_INT_WITHIN_MESSAGE(5, -1, 10, "Custom Message.");
    VERIFY_FAILS_END
}

// Optional: Additional test to verify that invoking the focal test directly (as a unit) succeeds.
// This is not strictly required but can help ensure the original test remains portable in isolation.
void test_RunOriginalFocalTestDirectly(void)
{
    // Invoking the focal test function itself. If it passes, this test passes.
    // If the focal test ever fails, this test will fail as well.
    testIntsWithinDeltaAndCustomMessage();
}

// Unity boilerplate main to run the test suite.
// Domain knowledge note: We avoid GTest/GMock and rely on Unity's RUN_TEST to execute each test function.
int main(void)
{
    UNITY_BEGIN();

    // Register tests
    RUN_TEST(test_IntsWithinDeltaAndCustomMessage_Passes);
    RUN_TEST(test_IntsWithinDeltaAndCustomMessage_FailsFirstWithCustomMessage);
    RUN_TEST(test_IntsWithinDeltaAndCustomMessage_FailsLastWithCustomMessage);
    RUN_TEST(test_RunOriginalFocalTestDirectly);

    return UNITY_END();
}