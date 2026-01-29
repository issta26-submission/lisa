/*
 * Additional Unity-based test suite for HEX16 within delta scenarios.
 * This file provides extra coverage for the focal case:
 *   testHEX16sWithinDeltaWhenThereAreDifferenceOutsideOf16Bits
 * by introducing new test variants that exercise boundary, wrap-around,
 * and non-wrap-around behavior of the TEST_ASSERT_HEX16_WITHIN macro.
 *
 * Note:
 * - This file is intended to be compiled and linked with Unity (unity.c / unity.h)
 *   in addition to the existing testunity.c in the project.
 * - To avoid colliding with the existing focal test in testunity.c, this file defines
 *   distinct test names and a separate main() test runner.
 * - The tests deliberately avoid terminating the test run; non-terminating assertions
 *   (via RUN_TEST and Unity's framework) are used where applicable.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Optional: keep setup/teardown minimal for isolated tests
void setUp(void) { }
void tearDown(void) { }

/*
 * Test 1: Lower 16-bit boundary exact difference
 * Verifies that when lower 16 bits differ by exactly 5, the within-delta check passes
 * even if the full 32-bit values differ in their upper bits.
 */
void testHEX16sWithinDeltaExactBoundaryLowerBits(void)
{
    // Lower 16-bits: 0x12340 -> 0x2340
    // Lower 16-bits: 0x12345 -> 0x2345
    // Difference = 5, which is within delta = 5
    TEST_ASSERT_HEX16_WITHIN(5, 0x12340, 0x12345);
}

// Test 2: Wrap-around boundary pass (upper bits differ, lower bits differ by a small amount)
void testHEX16sWithinDeltaWrapAroundPass(void)
{
    // 0x8000 -> int16_t cast = -32768
    // 0x8005 -> int16_t cast = -32763
    // Difference in signed 16-bit domain is 5, within delta = 5
    TEST_ASSERT_HEX16_WITHIN(5, 0x8000, 0x8005);
}

// Test 3: Wrap-around boundary fail when delta is too small
void testHEX16sWithinDeltaWrapAroundFail(void)
{
    // Same values as Test 2, but delta is 4, so difference (5) exceeds delta
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_WITHIN(4, 0x8000, 0x8005);
    VERIFY_FAILS_END
}

// Test 4: Zero delta with identical lower bits should pass
void testHEX16sWithinDeltaZeroDeltaSameLowerBitsPass(void)
{
    // Both values have identical lower 16-bit value
    TEST_ASSERT_HEX16_WITHIN(0, 0xFFFF, 0xFFFF);
}

// Test 5: Zero delta with different lower bits should fail
void testHEX16sWithinDeltaZeroDeltaDifferentLowerBitsFail(void)
{
    // Different lower 16-bit values, delta = 0 -> must fail
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_WITHIN(0, 0xFFFF, 0xFFFE);
    VERIFY_FAILS_END
}

/*
 * Main runner for the additional tests.
 * This allows compiling this file standalone or alongside the existing test runner.
 */
int main(void)
{
    UnityBegin("testHEX16WithinDelta_Additional.c");

    // Register tests
    RUN_TEST(testHEX16sWithinDeltaExactBoundaryLowerBits);
    RUN_TEST(testHEX16sWithinDeltaWrapAroundPass);
    RUN_TEST(testHEX16sWithinDeltaWrapAroundFail);
    RUN_TEST(testHEX16sWithinDeltaZeroDeltaSameLowerBitsPass);
    RUN_TEST(testHEX16sWithinDeltaZeroDeltaDifferentLowerBitsFail);

    return UnityEnd();
}