// Test suite for focal method: testUINT16sNotWithinDelta (located in testunity.c)
// This suite uses the Unity framework to run the focal test(s) as part of a larger test suite.
// Step 1: Candidate Keywords extracted from the focal method and its context
// - UINT16, 65535, 0, delta = 2
// - TEST_ASSERT_UINT16_WITHIN, within-delta check, wrap-around behavior for unsigned 16-bit
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END (the focal test expects the assertion to fail)
// - Custom message variant: TEST_ASSERT_UINT16_WITHIN_MESSAGE
// - Domain: static/test harness macros, Unity integration, non-GTest C unit testing
// Step 2 & 3: We instantiate a small C++11 test harness that runs the existing focal test(s)
// via Unity's RUN_TEST macro. This preserves the original test semantics (including aborts)
// while enabling execution from a C++11 driver, as requested.
// The tests under focus:
//   - testUINT16sNotWithinDelta
//   - testUINT16sNotWithinDeltaAndCustomMessage
//   - testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirst
//   - testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirstAndCustomMessage
//   - testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassBigFirst
//   - testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassBigFirstAndCustomMessage

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations of focal tests (C code in testunity.c)
void testUINT16sNotWithinDelta(void);
void testUINT16sNotWithinDeltaAndCustomMessage(void);
void testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirst(void);
void testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirstAndCustomMessage(void);
void testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassBigFirst(void);
void testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassBigFirstAndCustomMessage(void);

#ifdef __cplusplus
}
#endif

int main(void)
{
    // Minimal Unity test runner harness in C++11 context
    // Step 2: Execute the focal test(s) that exercise the "NotWithinDelta" scenarios
    UnityBegin("test_uint16_not_within_delta_suite.cpp");

    // Each RUN_TEST executes the corresponding focal test. Comments describe intent.

    // Test: NotWithinDelta when delta is 2, expected 65535, actual 0 (unsigned wrap behavior)
    // This should abort, confirming the not-within-delta condition is detected.
    RUN_TEST(testUINT16sNotWithinDelta);

    // Test: NotWithinDelta with custom message variant
    // Verifies same abort behavior while providing a failure message.
    RUN_TEST(testUINT16sNotWithinDeltaAndCustomMessage);

    // Tests: NotWithinDelta with signed-int-would-pass scenarios (small first)
    // Ensures the unsigned delta check still fails when a signed comparison would be true.
    RUN_TEST(testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirst);
    RUN_TEST(testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirstAndCustomMessage);

    // Tests: NotWithinDelta with signed-int-would-pass scenarios (big first)
    // Same as above but with the values ordered to exercise wrap behavior.
    RUN_TEST(testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassBigFirst);
    RUN_TEST(testUINT16sNotWithinDeltaEvenThoughASignedIntWouldPassBigFirstAndCustomMessage);

    return UnityEnd();
}