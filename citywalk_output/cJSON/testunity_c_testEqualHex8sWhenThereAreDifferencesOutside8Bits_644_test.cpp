/* Additional test cases for the focal method:
   testEqualHex8sWhenThereAreDifferencesOutside8Bits
   Located in testunity.c
   These tests verify that TEST_ASSERT_EQUAL_HEX8 correctly compares 8-bit
   values by ignoring differences in the higher bits, while still allowing
   negative/errored paths to be detected via Unity's abort wrappers.

   Rationale:
   - Validate the intended behavior where only the least significant 8 bits are
     used for comparison.
   - Cover both passing scenarios (differences only outside 8 bits) and
     failing scenarios (differences inside the 8-bit window).
   - Include variations that exercise different integral types that feed into the
     macro (constants, variables, signed/unsigned casts) to improve coverage.
*/

void testEqualHex8sWhenThereAreDifferencesOutside8Bits_VariantPass1(void)
{
    // Upper bits differ, lower 8 bits are equal -> should pass
    // 0x321 & 0xFF = 0x21, 0x421 & 0xFF = 0x21
    TEST_ASSERT_EQUAL_HEX8(0x321, 0x421);
}

void testEqualHex8sWhenThereAreDifferencesOutside8Bits_VariantPass2(void)
{
    // Another pair where lower 8 bits are equal but upper bits differ
    // 0xFF21 & 0xFF = 0x21, 0x0021 & 0xFF = 0x21
    TEST_ASSERT_EQUAL_HEX8(0xFF21, 0x0021);
}

void testEqualHex8sWhenThereAreDifferencesOutside8Bits_VariantPass3_WithDifferentTypes(void)
{
    // Use different integral types to feed into the macro
    int a = 0x321;
    int b = 0x421; // lower 8 bits equal (0x21)
    TEST_ASSERT_EQUAL_HEX8(a, b);

    unsigned int c = 0xFF21;
    unsigned int d = 0x0021; // lower 8 bits equal (0x21)
    TEST_ASSERT_EQUAL_HEX8(c, d);
}

void testEqualHex8sWhenThereAreDifferencesOutside8Bits_VariantFailLowerBits(void)
{
    // Lower 8 bits differ -> should fail, regardless of upper bits
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX8(0x321, 0x322); // lower bits 0x21 vs 0x22
    VERIFY_FAILS_END
}