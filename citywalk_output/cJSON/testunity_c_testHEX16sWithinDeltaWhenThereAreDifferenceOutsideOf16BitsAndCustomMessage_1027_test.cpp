// Additional test cases for the focal method testHEX16sWithinDeltaWhenThereAreDifferenceOutsideOf16BitsAndCustomMessage
// These tests expand coverage around HEX16 within-delta behavior with and without custom messages.

void testHEX16sWithinDeltaWhenDifferenceWithin16BitsAndCustomMessage(void)
{
    // Positive case: lower 16-bit difference is within delta; verify it passes with a custom message.
    // 0x4321 vs 0x4324 differ by 3 in the low 16 bits; delta = 3
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(3, 0x4321, 0x4324, "Within delta small difference.");
}

void testHEX16sWithinDeltaNotWithinDeltaEvenWithCustomMessage(void)
{
    // Negative case: after masking to 16 bits, difference exceeds delta; expect failure with a custom message.
    // 0x5432A (lower 16-bit = 0x432A) vs 0x44321 (lower 16-bit = 0x4321) -> diff = 9 > delta(5)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX16_WITHIN_MESSAGE(5, 0x5432A, 0x44321, "Custom Message for failure.");
    VERIFY_FAILS_END
}