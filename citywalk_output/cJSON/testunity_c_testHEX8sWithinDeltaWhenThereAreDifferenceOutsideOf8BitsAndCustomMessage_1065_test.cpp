/***********************
 * Unit Test Suite
 * Focal Method: testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage
 * Location: testunity.c
 *
 * This file provides a compact, focused C++11 compatible test harness
 * (Unity-based) to validate the focal HEX8 within-delta behavior and its
 * custom-message variant. It intentionally exercises both failure and success
 * paths to maximize coverage of the core dependent components (HEX8, delta
 * checking, and custom messages).
 *
 * Notes:
 * - The focal function is defined in testunity.c (C linkage). We declare it
 *   with extern "C" to ensure proper linkage when compiled with a C++ compiler.
 * - This test suite includes wrappers that:
 *     a) verify the focal function fails as expected (within delta requirement not met),
 *     b) verify a passing case using the HEX8 within-delta macro with a custom message,
 *     c) verify a direct within-delta path without a custom message.
 * - This file uses Unity's non-terminating assertion patterns (e.g., EXPECT_ABORT_BEGIN)
 *   to ensure code paths are exercised without terminating the entire test program.
 *
 * Candidate Keywords (Step 1): HEX8, delta, within, 8-bit, 0x123, 0xF23, Custom Message,
 * testHEX8sWithinDeltaWithinDeltaAndCustomMessage, testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage
 ***********************/

/* Candidate Keywords (documentation for test design)
   - HEX8: 8-bit hexadecimal comparison
   - Delta: allowed difference threshold (5 in focal examples)
   - WithinDelta: predicate that checks |expected - actual| <= delta
   - CustomMessage: test variant that prints a user-supplied message on failure
   - DifferenceOutside8Bits: test case where high/low parts differ beyond 8 bits
   - testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage: focal method under test
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal function prototype (C linkage)
void testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage(void);
#ifdef __cplusplus
}
#endif

/* 
   Additional test wrappers defined with C linkage so they can be registered
   and executed by Unity's runner in a mixed C/C++ project.
*/

// 1) Wrapper: focal function should fail (difference outside 8-bit delta)
extern "C" void testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage_IsFailure(void)
{
    // Expect the focal test to abort due to assertion failure
    EXPECT_ABORT_BEGIN
    testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage();
    VERIFY_FAILS_END
}

// 2) Direct success path: within-delta with a custom message (should pass)
extern "C" void testHEX8sWithinDeltaWithinDeltaAndCustomMessage_Pass(void)
{
    // This path ensures the within-delta macro with a custom message succeeds.
    // 0x111 -> 0x11, 0x112 -> 0x12; delta = 5; |0x11 - 0x12| = 1 <= 5
    TEST_ASSERT_HEX8_WITHIN_MESSAGE(5, 0x111, 0x112, "Custom Message.");
}

// 3) Direct success path (without custom message) to broaden coverage
extern "C" void testHEX8sWithinDeltaEdgeCaseWithinDeltaNoMessage(void)
{
    // Passes: 0x111 (0x11) vs 0x112 (0x12) within delta 5
    TEST_ASSERT_HEX8_WITHIN(5, 0x111, 0x112);
}

// 4) Direct failure path (without custom message) to exercise failure mode
extern "C" void testHEX8sWithinDeltaEdgeCaseNotWithinDeltaNoMessage(void)
{
    // Fails: 0x111 (0x11) vs 0x117 (0x17) delta 5 -> |0x11 - 0x17| = 0x6 = 6 > 5
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX8_WITHIN(5, 0x111, 0x117);
    VERIFY_FAILS_END
}

// Main runner for this standalone test suite (optional if you integrate with an
// existing Unity test runner. If a single Unity runner is used, you can remove
// this main and rely on the central runner to invoke RUN_TEST for each symbol above.)
#ifdef __cplusplus
extern "C" {
#endif
int main(void)
{
    // Begin Unity test session
    UnityBegin("test_hex8_within_delta_suite_cpp");

    // Register tests (names must match the symbols above)
    RUN_TEST(testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage_IsFailure);
    RUN_TEST(testHEX8sWithinDeltaWithinDeltaAndCustomMessage_Pass);
    RUN_TEST(testHEX8sWithinDeltaEdgeCaseWithinDeltaNoMessage);
    RUN_TEST(testHEX8sWithinDeltaEdgeCaseNotWithinDeltaNoMessage);

    // End Unity test session
    return UnityEnd();
}
#ifdef __cplusplus
}
#endif

/* 
   Note to the tester (Step 2/Step 3 alignment):
   - The focal test (testHEX8sWithinDeltaWhenThereAreDifferenceOutsideOf8BitsAndCustomMessage)
     intentionally exercises the failure path when a difference outside the 8-bit delta is detected.
   - The additional tests provide coverage for both passing and failing paths of HEX8 within-delta
     checks, including the variant with a custom message.
   - If you integrate this with an existing Unity harness (as in testunity.c), ensure there is no
     conflicting main() entry point. The provided main() is optional and can be omitted to rely
     on the project-wide Unity runner.
   - All tests are written with C linkage for maximum compatibility with the project's Unity test runner.
*/