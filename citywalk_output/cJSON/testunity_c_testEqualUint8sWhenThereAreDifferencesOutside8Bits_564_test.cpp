// Additional test cases for the focal method testEqualUint8sWhenThereAreDifferencesOutside8Bits
// These tests focus on ensuring that 8-bit comparisons ignore higher-than-8-bit differences.
// Place these tests in testunity.c (or appropriate Unity test source) alongside the focal test.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Variant 1: Basic literals with high bits differing but low byte equal
// Validates that Unity's 8-bit comparison uses only the low 8 bits for equality.
void testEqualUint8sWhenThereAreDifferencesOutside8Bits_VariantBasic(void)
{
    // High bits differ, but low 8 bits are equal (0x21)
    TEST_ASSERT_EQUAL_UINT8(0x321, 0x421);

    // Another pair with same low byte but different high bits
    TEST_ASSERT_EQUAL_UINT8(0xFF21, 0x0021);
}

// Variant 2: Use 16-bit and 32-bit variables to ensure masking occurs correctly
void testEqualUint8sWhenThereAreDifferencesOutside8Bits_VariantWithSizedVariables(void)
{
    // 16-bit values differing in high bits, same low 8 bits
    uint16_t a = 0x0321;
    uint16_t b = 0x0421;
    TEST_ASSERT_EQUAL_UINT8(a, b); // low byte 0x21 == 0x21

    // 32-bit values differing in high bits, same low 8 bits
    uint32_t c = 0x00000321;
    uint32_t d = 0x00000421;
    TEST_ASSERT_EQUAL_UINT8(c, d); // low byte 0x21 == 0x21
}

// Variant 3: Casting and pointer/array usage to exercise diverse code paths
void testEqualUint8sWhenThereAreDifferencesOutside8Bits_VariantWithCastingAndPointers(void)
{
    // Explicit casts to uint8_t
    TEST_ASSERT_EQUAL_UINT8((uint8_t)0x321, (uint8_t)0x421);

    // Array elements with same low byte
    uint16_t arr1[] = {0x0123, 0x0321};
    uint16_t arr2[] = {0x0123, 0x0421};
    TEST_ASSERT_EQUAL_UINT8(arr1[1], arr2[1]); // both low bytes 0x21

    // Direct variable comparison with cast via 8-bit extraction
    uint16_t x = 0x0321;
    uint16_t y = 0x0421;
    TEST_ASSERT_EQUAL_UINT8((uint8_t)x, (uint8_t)y);
}