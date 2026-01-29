// test_suite.cpp
// Purpose:
// - Provide a self-contained C++11 based test suite that exercises the focal Unity-based
//   test function patterns found in testunity.c, specifically around testUINT32sNotWithinDeltaAndCustomMessage.
// - Uses Unity's non-GTest style macros to avoid terminating the test on expected failures.
// - Does not duplicate tests from testunity.c; instead, it directly exercises the delta-not-within
//   scenarios via inline tests that mirror the focal behavior.

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// The focal test function is defined in testunity.c with C linkage.
// We do not rely on that function directly to avoid potential symbol collisions or
// reliance on internal static state. Instead, we replicate the intended negative-test
// scenarios using the same Unity macros to ensure coverage of the same code paths.
// Expose the focal function just for potential direct invocation if needed.
extern "C" void testUINT32sNotWithinDeltaAndCustomMessage(void);

// Scenario 1: Not Within Delta with Custom Message (mirroring focal path)
void test_UINT32NotWithinDeltaAndCustomMessage_ViaMacroDirect(void)
{
    // This test expects the assertion to fail and abort, which Unity handles via EXPECT_ABORT_BEGIN
    // and VERIFY_FAILS_END. The test itself should pass because the abort is the expected outcome.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message.");
    VERIFY_FAILS_END
}

// Scenario 2: Not Within Delta when an unsigned would be considered within delta if interpreted as signed
// Small-first order: actual =  -1 (as signed) but treated as unsigned 4294967295
void test_UINT32NotWithinDelta_EvenThoughASignedIntWouldPass_SmallFirst(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN(5, 1, -1);
    VERIFY_FAILS_END
}

// Scenario 3: Not Within Delta with Custom Message and signed interpretation (SmallFirst)
void test_UINT32NotWithinDelta_EvenThoughASignedIntWouldPass_SmallFirst_WithCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(5, 1, -1, "Custom Message.");
    VERIFY_FAILS_END
}

// Scenario 4: Not Within Delta when an unsigned would be considered within delta if interpreted as signed
// Big-first order: actual = 1, expected = -1
void test_UINT32NotWithinDelta_EvenThoughASignedIntWouldPass_BigFirst(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN(5, -1, 1);
    VERIFY_FAILS_END
}

// Scenario 5: Not Within Delta with Custom Message and signed interpretation (BigFirst)
void test_UINT32NotWithinDelta_EvenThoughASignedIntWouldPass_BigFirst_WithCustomMessage(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(5, -1, 1, "Custom Message.");
    VERIFY_FAILS_END
}

// Optional: Directly call the focal test function to ensure it executes in isolation.
// This is not strictly necessary since the preceding tests mirror the same behavior with
// the same macros, but it helps guarantee the focal path remains exercisable in the suite.
void test_RunFocalNotWithinDeltaAndCustomMessageDirectCall(void)
{
    // If the focal function is self-contained and uses Unity's macros correctly, this should abort
    // as expected and be counted as a pass for this test harness.
    testUINT32sNotWithinDeltaAndCustomMessage();
}

// Main runner for the test suite.
// Note: We deliberately invoke only the above test scenarios to avoid duplicating tests that already exist
// in testunity.c (which would cause linker conflicts if redefined here).
int main(void)
{
    // Initialize Unity test framework
    UnityBegin("test_suite.cpp");

    // Run the focused scenarios that mirror the focal test behavior
    RUN_TEST(test_UINT32NotWithinDeltaAndCustomMessage_ViaMacroDirect);
    RUN_TEST(test_UINT32NotWithinDelta_EvenThoughASignedIntWouldPass_SmallFirst);
    RUN_TEST(test_UINT32NotWithinDelta_EvenThoughASignedIntWouldPass_SmallFirst_WithCustomMessage);
    RUN_TEST(test_UINT32NotWithinDelta_EvenThoughASignedIntWouldPass_BigFirst);
    RUN_TEST(test_UINT32NotWithinDelta_EvenThoughASignedIntWouldPass_BigFirst_WithCustomMessage);
    // Optional direct focal-like invocation
    // RUN_TEST(test_RunFocalNotWithinDeltaAndCustomMessageDirectCall);

    // Conclude testing
    return UnityEnd();
}