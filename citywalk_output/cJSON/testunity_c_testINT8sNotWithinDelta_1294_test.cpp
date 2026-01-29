/************************************************************
 * Test Suite: Focused validation of the focal method
 *         testINT8sNotWithinDelta (as found in testunity.c)
 *
 * Notes:
 * - This suite uses the Unity C unit-test framework (no GTest).
 * - The focal method uses Unity's EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
 *   to assert that a not-within-delta condition correctly triggers a failure.
 * - The tests below replicate and extend the intent of the focal method
 *   by exercising not-within-delta and within-delta scenarios for INT8.
 * - All tests are written in C++11-compatible code but rely on Unity's C API.
 *
 * Step 1 (Candidate Keywords): 
 * - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
 * - TEST_ASSERT_INT8_WITHIN, TEST_ASSERT_INT8_WITHIN_MESSAGE
 * - INT8 type, delta concept (within vs not within)
 * - Aborting test harness behavior (non-terminating assertions)
 ************************************************************/

/* Candidate Keywords to test mapping:
   - Core macros: EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
   - Assertion: TEST_ASSERT_INT8_WITHIN, TEST_ASSERT_INT8_WITHIN_MESSAGE
   - Data type: INT8 (signed 8-bit)
   - Behavioral branches: within delta (pass) vs not within delta (abort)
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


/* Forward declarations for setup/teardown if needed by Unity's runtime.
   (Not strictly required for these focused tests, but kept for compatibility.) */
void setUp(void);
void tearDown(void);

/* 1) Test: Not within delta should abort (replicates focal behavior)
   This mirrors the focal method's intent: with delta=2, expected=-3, actual=0
   the difference is 3, which is outside the allowed delta, so the test should abort. */
void test_INT8sNotWithinDelta_FocalEquivalent_ShouldAbort(void)
{
    // Expects the assertion to fail and abort the test flow
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_WITHIN(2, -3, 0);
    VERIFY_FAILS_END
}

/* 2) Test: Not within delta with a custom message should abort
   Verifies the variant that passes a custom message string. */
void test_INT8sNotWithinDelta_CustomMessage_ShouldAbort(void)
{
    // Using the message variant to ensure message handling doesn't suppress abort
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_INT8_WITHIN_MESSAGE(2, -3, 0, "Custom message: INT8 within delta failed.");
    VERIFY_FAILS_END
}

/* 3) Test: Within delta should pass (covering the true-branch)
   Ensure that a value exactly at the delta boundary passes. */
void test_INT8sWithinDelta_Pass_ShouldSucceed(void)
{
    // Difference is exactly delta (| -3 - (-1) | = 2)
    TEST_ASSERT_INT8_WITHIN(2, -3, -1);
}

/* 4) Test: Boundary condition explicitly within delta should pass
   A near-boundary case to increase coverage of edge behavior. */
void test_INT8sWithinDelta_BoundaryWithin_ShouldSucceed(void)
{
    // Same boundary scenario with a different actual value
    TEST_ASSERT_INT8_WITHIN(2, 0, -2); // difference 2
}

/* 5) Optional: Within delta with negative values, still should pass
   Verifies sign-agnostic delta behavior. */
void test_INT8sWithinDelta_NegativeValues_ShouldSucceed(void)
{
    TEST_ASSERT_INT8_WITHIN(2, -100, -98); // difference 2
}

/* Unity test runner entry points */
int main(void)
{
    // Unity test framework startup
    UnityBegin("test_suite_int8_not_within_delta.cpp");

    // Register tests
    RUN_TEST(test_INT8sNotWithinDelta_FocalEquivalent_ShouldAbort);
    RUN_TEST(test_INT8sNotWithinDelta_CustomMessage_ShouldAbort);
    RUN_TEST(test_INT8sWithinDelta_Pass_ShouldSucceed);
    RUN_TEST(test_INT8sWithinDelta_BoundaryWithin_ShouldSucceed);
    RUN_TEST(test_INT8sWithinDelta_NegativeValues_ShouldSucceed);

    // Shutdown Unity
    return UnityEnd();
}