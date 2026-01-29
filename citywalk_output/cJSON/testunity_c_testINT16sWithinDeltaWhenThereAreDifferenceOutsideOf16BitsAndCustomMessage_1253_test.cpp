// Test suite for the focal method related to INT16 within a delta with a custom message.
// This file provides a minimal, self-contained Unity-based test suite (C++11 compatible).
// It focuses on the focal scenario: INT16 within a delta when there is a difference that
// appears outside of 16-bit boundaries but is masked within 16-bit representation.
// Note: This suite does not rely on GTest/GMock. It uses the Unity framework directly.

// Step 1: Candidate Keywords extracted from the focal method
// - TEST_ASSERT_INT16_WITHIN_MESSAGE
// - delta (5 in focal example)
// - 0x54321, 0x44321 (values involved in the focal test)
// - "Custom Message." (custom message used in the focal test)
// - 16-bit wrap-around semantics
// - Unity test harness macros
// - Non-GTest, portable C-style unit test style

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Minimal setup/teardown to satisfy Unity's expectations, if any static state exists.
static void setUp(void) { /* no setup required for these tests */ }
static void tearDown(void) { /* no teardown required for these tests */ }

// Test 1: Validate that the focal delta check passes when the two 16-bit representations are equal
// under a delta of 5, despite a difference outside the 16-bit boundary.
// This mirrors the focal method's intention where the high bits differ but the actual 16-bit
// values are equal, resulting in a 0 difference after masking.
void test_INT16sWithinDelta_Passes_CustomMessage(void)
{
    // Candidate keywords alignment:
    // - delta = 5
    // - expected = 0x54321 (16-bit becomes 0x4321)
    // - actual   = 0x44321 (16-bit becomes 0x4321)
    // - custom message: "Custom Message."
    TEST_ASSERT_INT16_WITHIN_MESSAGE(5, 0x54321, 0x44321, "Custom Message.");
}

// Test 2: Validate that the focal delta check passes when using negative values that wrap
// around the 16-bit boundary but remain within the delta after 16-bit masking.
// This tests the sign-aware path where the two 16-bit representations are close enough.
void test_INT16sWithinDelta_Passes_NegativeWrap(void)
{
    // delta = 2
    // expected = -1 (0xFFFF)
    // actual   = -3 (0xFFFD)
    // Difference in 16-bit representation is 2, which is within delta.
    TEST_ASSERT_INT16_WITHIN_MESSAGE(2, -1, -3, "Custom Message.");
}

// Test 3: Validate 16-bit wrap-around edge case where the wrapped difference is exactly the delta.
// This ensures the macro handles edge-case boundary conditions correctly.
void test_INT16sWithinDelta_WrapAroundEdgePass(void)
{
    // delta = 1
    // expected = 0x12345 (16-bit 0x2345)
    // actual   = 0x12344 (16-bit 0x2344)
    // Difference in 16-bit representation is 1, exactly on the delta threshold.
    TEST_ASSERT_INT16_WITHIN_MESSAGE(1, 0x12345, 0x12344, "Custom Message.");
}

// Runner
int main(void)
{
    UnityBegin("test_int16_within_delta_suite.cpp");

    RUN_TEST(test_INT16sWithinDelta_Passes_CustomMessage);
    RUN_TEST(test_INT16sWithinDelta_Passes_NegativeWrap);
    RUN_TEST(test_INT16sWithinDelta_WrapAroundEdgePass);

    return UnityEnd();
}