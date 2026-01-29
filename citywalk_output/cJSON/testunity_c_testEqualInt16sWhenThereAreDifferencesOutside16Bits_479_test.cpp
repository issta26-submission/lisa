// test_equal_int16s_outside16bits.cpp
// Purpose: Extend testing coverage for the focal scenario
// "testEqualInt16sWhenThereAreDifferencesOutside16Bits"
// by exercising various pairs whose lower 16 bits are equal
// despite differences in the upper 16 bits.
// This file uses Unity (C unit testing framework) and is compatible
// with C++11 compilers when compiled as C/C++ mixed project.
// Note: This is a separate test runner that can be linked with Unity's core.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Ensure Unity's C linkage when compiling in C++
extern "C" {
}

// Candidate Keywords guiding test design:
// - TEST_ASSERT_EQUAL_INT16
// - lower 16 bits equality
// - differences outside 16 bits
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END for negative tests

// Test: Equal 16-bit values when upper 16 bits differ (Variant A)
static void testEqualInt16sOutside16Bits_VariantA(void)
{
    // 0x12345678 and 0x98765678 differ in upper bits, lower 16 bits are 0x5678
    // This should pass because TEST_ASSERT_EQUAL_INT16 compares only the low 16 bits
    TEST_ASSERT_EQUAL_INT16(0x12345678, 0x98765678);
    // Explanation: lower 16 bits (0x5678) are identical; test passes.
}

// Test: Equal 16-bit values when upper 16 bits differ (Variant B)
static void testEqualInt16sOutside16Bits_VariantB(void)
{
    // Provided example from focal method: 0xFFFF4321 vs 0x00004321
    // Lower 16 bits are both 0x4321; should pass.
    TEST_ASSERT_EQUAL_INT16(0xFFFF4321, 0x00004321);
}

// Test: Additional variation with different upper bits but same lower 16 bits (Variant C)
static void testEqualInt16sOutside16Bits_VariantC(void)
{
    // Upper bits differ, lower 16 bits equal to 0xABCD
    TEST_ASSERT_EQUAL_INT16(0x0000ABCD, 0xFFFFABCD);
}

// Test: Additional variation with distinctive upper bits (Variant D)
static void testEqualInt16sOutside16Bits_VariantD(void)
{
    // Lower 16 bits = 0x1234
    TEST_ASSERT_EQUAL_INT16(0x7FFF1234, 0xFFFF1234);
}

// Test: Basic identical values (path coverage for simple case) (Variant E)
static void testEqualInt16sOutside16Bits_VariantE(void)
{
    // Identical values test to ensure no accidental upper-bit interference
    TEST_ASSERT_EQUAL_INT16(0x12345678, 0x12345678);
}

// Test: Negative case - failure when lower 16 bits differ (should abort test)
static void testNotEqualInt16sOutside16Bits_ShouldFail(void)
{
    // Intentionally make lower 16 bits differ to verify the assertion fails
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT16(0x12345678, 0x12345677);
    VERIFY_FAILS_END
}

// Main runner to execute the added tests.
// This allows compiling this file as a standalone test or linking with a broader suite.
int main(void)
{
    UnityBegin("test_equal_int16s_outside16bits.cpp");

    RUN_TEST(testEqualInt16sOutside16Bits_VariantA);
    RUN_TEST(testEqualInt16sOutside16Bits_VariantB);
    RUN_TEST(testEqualInt16sOutside16Bits_VariantC);
    RUN_TEST(testEqualInt16sOutside16Bits_VariantD);
    RUN_TEST(testEqualInt16sOutside16Bits_VariantE);
    RUN_TEST(testNotEqualInt16sOutside16Bits_ShouldFail);

    return UnityEnd();
}