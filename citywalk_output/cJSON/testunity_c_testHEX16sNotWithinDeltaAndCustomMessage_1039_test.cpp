/*
 * Test Suite: HEX16 within-not-within delta scenarios (focusing on testHEX16sNotWithinDeltaAndCustomMessage)
 *
 * This file provides a small, self-contained set of Unity-based tests in C++11
 * to exercise the HEX16 within-delta predicate, specifically scenarios
 * associated with the focal Canonical test in testunity.c:
 *   testHEX16sNotWithinDeltaAndCustomMessage
 *
 * Strategy:
 * - Leverage Unity's TEST_ASSERT_HEX16_WITHIN_MESSAGE(delta, expected, actual, "Custom Message.")
 * - Validate both failing (not within delta) and passing (within delta) paths.
 * - Use explicit explanatory comments for each test.
 *
 * Notes:
 * - This test suite is designed to be compilable under C++11.
 * - It avoids GTest; it relies on the Unity framework (unity.h) and its macros.
 * - The tests are written to be executable in a typical Unity test runner environment.
 * - We keep static/class-private concerns out of scope as the focal method is a C-style test macro
 *   usage; the tests exercise the macro usage paths rather than private state.
 */

/* Include Unity headers with C linkage for compatibility in C++. */
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

#ifdef __cplusplus
}
#endif

/* Candidate Keywords (for traceability of the focal method):
   - testHEX16sNotWithinDeltaAndCustomMessage
   - HEX16 within-delta predicate
   - TEST_ASSERT_HEX16_WITHIN_MESSAGE
   - Custom Message. (the message string passed to the assertion)
   - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END semantics (aborts handled by Unity macro)
*/

/*setUp/tearDown are optional in this suite; provide empty implementations.*/
void setUp(void) {}
void tearDown(void) {}

/* 
 * Test 1: Not Within Delta scenario using a custom message
 * This mirrors the focal scenario by asserting two 16-bit values (0xFFFF and 0x0000)
 * with a delta of 2. The difference is 0xFFFF, which is far outside the delta,
 * so the assertion should fail (i.e., test should register a failure).
 * This exercises the "not within delta" branch of the macro with a custom message.
 */
void testHEX16sNotWithinDeltaAndCustomMessage_VariantA(void)
{
    // Expected: Not within delta; should fail the hex16 within-Delta check with a message
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(2, 0xFFFFu, 0x0000u, "Custom Message.");
}

/* 
 * Test 2: Not Within Delta scenario with the operands swapped
 * This variant ensures the predicate also handles the symmetric case correctly:
 * delta = 2, expected = 0, actual = 0xFFFF.
 * This again should fail, validating the non-commutative check path.
 */
void testHEX16sNotWithinDeltaAndCustomMessage_VariantB(void)
{
    // Expected: Not within delta; should fail the hex16 within-Delta check with a message
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(2, 0x0000u, 0xFFFFu, "Custom Message.");
}

/* 
 * Test 3: Within Delta scenario with a custom message
 * Verifies the passing path where the absolute difference is within the delta.
 * This confirms that the custom message form does not interfere with a successful check.
 */
void testHEX16sWithinDeltaAndCustomMessage_ShouldPass(void)
{
    // delta = 2, actual difference = 2 (within delta)
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(2, 0x1234u, 0x1236u, "Custom Message.");
}

/* Main runner: register all tests and execute them. */
int main(void)
{
    UNITY_BEGIN("HEX16 within-delta vs not-within-delta tests for testHEX16sNotWithinDeltaAndCustomMessage focal scenarios");
    
    RUN_TEST(testHEX16sNotWithinDeltaAndCustomMessage_VariantA);
    RUN_TEST(testHEX16sNotWithinDeltaAndCustomMessage_VariantB);
    RUN_TEST(testHEX16sWithinDeltaAndCustomMessage_ShouldPass);
    
    return UNITY_END();
}