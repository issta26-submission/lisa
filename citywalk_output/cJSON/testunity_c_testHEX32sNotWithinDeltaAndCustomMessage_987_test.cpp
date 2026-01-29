/*
  Candidate Keywords (derived from the focal method testHEX32sNotWithinDeltaAndCustomMessage):
  - HEX32, WITHIN, WITHIN_MESSAGE
  - DELTA (first parameter), expected/actual values
  - Custom Message string
  - Boundary values: 2147483647u, 2147483650u, 0x7FFFFFFF, 0xFFFFFFFF
  - Not-within scenarios (delta small or zero)
  - Unity macros: EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
  - Non-terminating test style: using EXPECT_ABORT_BEGIN/VERIFY_FAILS_END to allow multiple checks
  - Namespace: global C code in testUnity framework
*/

void testHEX32sNotWithinDeltaAndCustomMessage(void)
{
{
    // Original failing case to validate NotWithinDelta with a custom message.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message.");
    VERIFY_FAILS_END
}
{
    // Additional failing case with delta = 2 to ensure the custom message path prints
    // and that the non-within condition still aborts.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(2, 2147483647u, 2147483650u, "Custom Message.");
    VERIFY_FAILS_END
}
{
    // Failing case across a boundary: comparing near max to 0xFFFFFFFF with a small delta.
    // This ensures large unsigned differences are treated correctly.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(4, 0x7FFFFFFF, 0xFFFFFFFFu, "Custom Message.");
    VERIFY_FAILS_END
}
{
    // Failing case with zero delta to force exact-match failure.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_HEX32_WITHIN_MESSAGE(0, 0x12345678u, 0x12345679u, "Custom Message.");
    VERIFY_FAILS_END
}
}