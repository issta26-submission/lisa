// Extended test suite additions for testPrintNumbersUnsigned32
// Context: This code augments the existing Unity-based testunity.c
// The added tests are focused on enhancing coverage for testPrintNumbersUnsigned32
// by validating additional unsigned 32-bit edge cases when the output spy is enabled.
// Comment-based explanations accompany each test to clarify intent and coverage goals.


// NOTE: These tests are designed to be compiled together with the existing
// Unity test harness. They rely on the same output spying mechanism that the
// focal test uses (startPutcharSpy, endPutcharSpy, getBufferPutcharSpy, etc.),
// as well as the macro TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS for assertion
// against the expected printed strings.


// Extended coverage: verify additional boundary values when output spy is enabled.
// This increases the number of values tested beyond the original five, including
// intermediate and near-maximum values to improve branch coverage of the
// printing logic for unsigned 32-bit numbers.
void testPrintNumbersUnsigned32_ExtendedValues(void)
{
#ifndef USING_OUTPUT_SPY
    // If the output spy is not enabled, the original test is designed to be ignored.
    // This ensures the test suite remains compilable in environments without
    // the output spy, while still providing coverage when spy is active.
    TEST_IGNORE();
#else
    // Validate printing of a small non-zero value.
    // Ensures basic decimal formatting path is exercised.
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("2", 2);

    // Validate printing of a value very close to the 32-bit max minus 1.
    // Helps ensure no off-by-one or truncation issues near the upper bound.
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("4294967294", (UNITY_UINT32)0xFFFFFFFE);

    // Validate printing of the maximum 32-bit unsigned value.
    // Confirms full width (10 digits) formatting and correct handling of edge case.
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("4294967295", (UNITY_UINT32)0xFFFFFFFF);
#endif
}


// Extended coverage: ultra-high boundary sanity check for zero and max, placed
// as an additional test to complement existing coverage. This test reinforces
// the intended behavior when crossing boundary values in the print routine.
void testPrintNumbersUnsigned32_UltraHighBoundary(void)
{
#ifndef USING_OUTPUT_SPY
    TEST_IGNORE();
#else
    // Re-assert that zero prints correctly (redundant check but useful for
    // ensuring consistent behavior across test variants).
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("0", 0);

    // Re-assert that the maximum 32-bit unsigned value prints correctly.
    TEST_ASSERT_EQUAL_PRINT_UNSIGNED_NUMBERS("4294967295", (UNITY_UINT32)0xFFFFFFFF);
#endif
}