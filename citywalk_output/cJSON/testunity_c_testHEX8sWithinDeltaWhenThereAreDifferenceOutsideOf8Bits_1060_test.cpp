/***************************************
 * Test Suite for: testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits
 * Location: testunity.c (as per the focal method description)
 *
 * This supplementary test file exercises the core behavior of the
 * HEX8 within-delta assertion used in the focal test. It complements
 * the existing test in testunity.c by adding additional scenarios that
 * verify both passing and failing paths, including custom messages.
 *
 * The tests are written for the Unity framework (C). They are self-contained
 * and do not rely on private members; they exercise true/false branches of
 * the HEX8 within-delta predicate using non-terminating and aborting tests
 * as appropriate.
 *
 * Notes:
 * - The tests use the same macros as in the provided focal test to ensure
 *   consistent behavior with the original test harness.
 * - We declare the necessary spy functions used by EXPECT_ABORT_BEGIN macros.
 ***************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Forward declarations for the spy utilities used by EXPECT_ABORT macros.
// These are typically provided by the Unity test harness (as seen in testunity.c).
void startPutcharSpy(void);
void endPutcharSpy(void);
char* getBufferPutcharSpy(void);

// Prototypes for the test suite helpers (optional, kept local for clarity).
// No static dependencies are introduced here; tests rely solely on Unity macros.


// Test: HEX8 within delta passes when high bits differ but lower 8 bits are same.
void testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits_Passes(void)
{
    // The lower 8 bits of 0x123 and 0xF23 are both 0x23, so
    // the difference within 8 bits is 0, which is <= delta (5).
    // This verifies that high-order bits are ignored for 8-bit comparisons.
    TEST_ASSERT_HEX8_WITHIN(5, 0x123, 0xF23);
}

// Test: HEX8 within delta passes with a custom message when the values match within 8 bits.
void testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits_PassesWithMessage(void)
{
    // Same scenario as above but uses the _WITHIN_MESSAGE variant to
    // ensure custom messages do not affect the pass condition.
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(5, 0x123, 0xF23, "HEX8 within delta should pass with custom message");
}

// Test: HEX8 within delta fails when 8-bit values exceed the delta (without considering high bits).
void testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits_FailsWithoutMessage(void)
{
    // Here the lower 8 bits differ: 0x23 vs 0x3A (difference 0x17 = 23 > 5)
    // This should fail, despite high-order bits differing, because the
    // 8-bit comparison exceeds the delta threshold.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN(5, 0x123, 0x13A);
    VERIFY_FAILS_END
}

// Test: HEX8 within delta fails with a custom message when the delta condition is not met.
void testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits_FailsWithMessage(void)
{
    // Same as above but with a custom message to ensure message variants do not affect failure.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(5, 0x123, 0x13A, "HEX8 within delta should fail with custom message");
    VERIFY_FAILS_END
}


// Main entry point to register and run the tests defined above.
int main(void)
{
    // Initialize Unity test framework.
    UnityBegin("test_focal_hex8_within_suite.c");

    // Register tests.
    RUN_TEST(testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits_Passes);
    RUN_TEST(testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits_PassesWithMessage);
    RUN_TEST(testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits_FailsWithoutMessage);
    RUN_TEST(testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8Bits_FailsWithMessage);

    // Finalize and return test results.
    return UnityEnd();
}