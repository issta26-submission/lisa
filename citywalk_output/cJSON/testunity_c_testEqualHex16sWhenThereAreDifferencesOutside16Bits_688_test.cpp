// Expanded test for testEqualHex16sWhenThereAreDifferencesOutside16Bits
// This test now covers multiple scenarios where the lower 16 bits are equal
// while higher bits differ, ensuring that equality is determined based on 16-bit values only.
void testEqualHex16sWhenThereAreDifferencesOutside16Bits(void)
{
{
    // Baseline checks from original test
    TEST_ASSERT_EQUAL_HEX16(0x54321,0x64321);            // lower 16 bits equal (0x4321)
    TEST_ASSERT_EQUAL_HEX16(0xFFFF4321,0x00004321);      // lower 16 bits equal (0x4321) despite differing high parts

    // Additional scenarios to ensure differences outside 16 bits are ignored
    TEST_ASSERT_EQUAL_HEX16(0x00004567, 0x12344567);     // same lower 16 bits (0x4567) with different high 16 bits
    TEST_ASSERT_EQUAL_HEX16(0xFFFF4567, 0x00004567);     // high parts differ, still equal on 16-bit comparison
    TEST_ASSERT_EQUAL_HEX16(0xFFFF0000, 0x00000000);     // lower 16 bits are 0 in both cases
    TEST_ASSERT_EQUAL_HEX16(0xA1B20021, 0xB1B20021);     // different high 16 bits, same lower 16 bits (0x0021)
}
 }