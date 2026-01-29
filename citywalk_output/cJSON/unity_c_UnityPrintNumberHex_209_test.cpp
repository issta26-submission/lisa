// Test suite for UnityPrintNumberHex in unity.c using a custom C++ test harness (no GTest)

// This test captures output via the Unity framework's UnityOutputChar mechanism.
// We implement UnityOutputChar in C++ to accumulate printed characters into a string
// so we can assert the exact hex representation produced by UnityPrintNumberHex.

#include <stddef.h>
#include <cstring>
#include <unity.h>
#include <iostream>
#include <string>


// Ensure C linkage for the Unity C code and types
extern "C" {
}

// Forward declaration of the focal function under test from unity.c
extern "C" void UnityPrintNumberHex(const UNITY_UINT number, const char nibbles_to_print);

// Buffer to capture printed characters via UnityOutputChar
static std::string g_output;

// Provide Unity's output hook. Unity's code uses UNITY_OUTPUT_CHAR to print a character.
// By implementing UnityOutputChar here, we intercept all hex digit output.
extern "C" void UnityOutputChar(char c)
{
    g_output.push_back(static_cast<char>(c));
}

// Helpers for test assertions (non-terminating, align with domain knowledge)
static int g_test_count = 0;
static int g_failures = 0;

#define EXPECT_EQ_STR(expected, actual) \
    do { \
        g_test_count++; \
        if ((expected) != (actual)) { \
            std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ \
                      << " | Expected \"" << (expected) \
                      << "\" but got \"" << (actual << std::string()) << "\"\n"; \
            g_failures++; \
        } \
    } while(0)

#define EXPECT_EQ(expected, actual) \
    do { \
        g_test_count++; \
        if (!((expected) == (actual))) { \
            std::cout << "FAIL: " << __FILE__ << ":" << __LINE__ \
                      << " | Expected " << (expected) << " but got " << (actual) << "\n"; \
            g_failures++; \
        } \
    } while(0)

#define RESET_OUTPUT() do { g_output.clear(); } while(0)


// Test 1: Basic 8-digit hex representation for a full 32-bit number
static void test_UnityPrintNumberHex_basicEightDigits()
{
    RESET_OUTPUT();
    // 0x1234ABCD -> "1234ABCD"
    UnityPrintNumberHex(0x1234ABCDu, 8);
    EXPECT_EQ_STR("1234ABCD", g_output);
}

// Test 2: All-ones should print as "FFFFFFFF"
static void test_UnityPrintNumberHex_allOnes()
{
    RESET_OUTPUT();
    UnityPrintNumberHex(0xFFFFFFFFu, 8);
    EXPECT_EQ_STR("FFFFFFFF", g_output);
}

// Test 3: Leading zeros should be preserved when nibble count includes high-order zeros
static void test_UnityPrintNumberHex_leadingZeros()
{
    RESET_OUTPUT();
    // 0x0000000F with 4 nibbles -> "000F"
    UnityPrintNumberHex(0x0000000Fu, 4);
    EXPECT_EQ_STR("000F", g_output);
}

// Test 4: Nibbles exceed the maximum, ensuring clamping to 2*sizeof(number) (8 for 32-bit)
static void test_UnityPrintNumberHex_nibblesClampMax()
{
    RESET_OUTPUT();
    // Number 0x00001234 with nibble count 100 should clamp to 8 -> "00001234"
    UnityPrintNumberHex(0x00001234u, 100);
    EXPECT_EQ_STR("00001234", g_output);
}

// Test 5: Zero nibble prints nothing (empty output)
static void test_UnityPrintNumberHex_zeroNibble()
{
    RESET_OUTPUT();
    UnityPrintNumberHex(0xDEADBEEFu, 0);
    EXPECT_EQ_STR("", g_output);
}

// Test 6: Negative nibble value (cast to char results in -1). Should clamp and print 8 digits.
static void test_UnityPrintNumberHex_negativeNibbleValue()
{
    RESET_OUTPUT();
    // -1 as char typically becomes 0xFF; clamp to 8 digits
    UnityPrintNumberHex(0x12ABCD01u, (char)-1);
    EXPECT_EQ_STR("12ABCD01", g_output);
}

// Minimal test runner (no GTest). Call tests from main and report summary.
int main(void)
{
    // Run tests
    test_UnityPrintNumberHex_basicEightDigits();
    test_UnityPrintNumberHex_allOnes();
    test_UnityPrintNumberHex_leadingZeros();
    test_UnityPrintNumberHex_nibblesClampMax();
    test_UnityPrintNumberHex_zeroNibble();
    test_UnityPrintNumberHex_negativeNibbleValue();

    // Summary
    std::cout << "Tested UnityPrintNumberHex: total=" << g_test_count
              << " failures=" << g_failures << std::endl;

    return (g_failures == 0) ? 0 : 1;
}

/*
Notes:
- This test suite uses a custom UnityOutputChar hook to capture what UnityPrintNumberHex prints.
- The tests cover:
  - Normal operation with 8 digits (leading to full 32-bit hex output).
  - All-ones output (F's).
  - Leading zeros preservation when nibble count is small or large.
  - Clamping behavior when nibble count exceeds 2*sizeof(number) (8 for 32-bit UNITY_UINT).
  - Zero nibble case (no output).
  - Negative nibble input (through signed char) leading to clamped maximum digits.
- The tests are written in C++11 but target a C function under test; linkage is handled with extern "C".
- This test does not use GTest or any other heavy framework, adhering to the provided constraints.
*/