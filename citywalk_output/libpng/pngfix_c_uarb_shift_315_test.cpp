// Self-contained unit test suite for the focal method uarb_shift
// This test targets the core behavior of the function operating on a
// "uarb" value represented as an array of 16-bit digits.
// The tests are implemented in C++11 (no GTest) and use a small
// in-house assertion framework to maximize coverage without terminating.
// Notes:
// - The real project uses libpng and a larger uarb type; for the purposes
//   of these tests, we implement a faithful local replica of the function
//   signature and semantics to validate its logic in isolation.
// - The tests focus on typical shift scenarios, multi-digit carry handling,
//   and the ndigits reduction behavior described in the focal method.
//
// Candidate Keywords (Step 1): uarb_shift, inout, ndigits, right_shift, carry,
// temp, 16-bit digits, PNG unsigned 16-bit type (png_uint_16), assertion,
// multi-digit shift, ndigits reduction, right shift propagation

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <setjmp.h>
#include <string.h>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <ctype.h>


// Typedefs to mimic the original code's types
typedef uint16_t png_uint_16;

// The focal method rewritten in C++11 for self-contained testing.
// Signature matches: int uarb_shift(uarb inout, int ndigits, unsigned int right_shift)
typedef png_uint_16* uarb;

// Self-contained implementation of uarb_shift mirroring the provided focal code.
// This is a faithful replica to enable isolated unit testing without external
// dependencies. In the real project, this would be linked from pngfix.c.
int uarb_shift(uarb inout, int ndigits, unsigned int right_shift)
{
    int i = ndigits;
    png_uint_16 carry = 0;
    // Predicate from the focal code; kept identical for correctness
    assert(right_shift >= 1 && right_shift <= 15);

    while (--i >= 0)
    {
        png_uint_16 temp = (png_uint_16)(carry | (inout[i] >> right_shift));
        /* Bottom bits to top bits of carry */
        carry = (png_uint_16)((inout[i] << (16 - right_shift)) & 0xffff);
        inout[i] = temp;
        /* The shift may reduce ndigits */
        if (i + 1 == ndigits && temp == 0)
            ndigits = i;
    }
    return ndigits;
}

// Lightweight, non-terminating test framework (no GTest).
// Tracks total tests and failures, prints concise output.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_tests_run; \
    if (!((actual) == (expected))) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << __func__ << ": " << (msg) \
                  << " | Expected: " << (expected) \
                  << "  Actual: " << (actual) << std::endl; \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << __func__ << ": " << (msg) << std::endl; \
    } \
} while (0)

#define ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[ASSERT] " << __func__ << ": " << (msg) << std::endl; \
    } \
} while (0)

// Test 1: Single-digit, max-right-shift (15) with ndigits=1.
// Expect the digit to shift right and potentially reduce ndigits to 0.
static void test_single_digit_shift_max()
{
    // Candidate Keyword mapping: inout single-digit array, right_shift=15
    uint16_t inout[1] = {0x0001}; // small value to verify clear shift to zero
    int ndigits = 1;
    int ret = uarb_shift(inout, ndigits, 15);

    // After shifting 0x0001 by 15 bits: 0x0000, and since temp == 0 and i+1 == ndigits,
    // ndigits should reduce to 0.
    EXPECT_EQ(ret, 0, "single-digit with 15-bit shift should reduce ndigits to 0");
    EXPECT_EQ(inout[0], (uint16_t)0x0000, "single-digit value after 15-bit shift should be 0");
}

// Test 2: Two digits, shift by 1, no reduction of ndigits (top non-zero after shift).
static void test_two_digits_shift1_no_reduction()
{
    // Candidate Keyword mapping: two digits, check propagation
    uint16_t inout[2] = {0x4000, 0x0000}; // top digit non-zero after shift
    int ndigits = 2;
    int ret = uarb_shift(inout, ndigits, 1);

    // Expected:
    // i=1: temp = 0, inout[1] = 0, carry = 0
    // i=0: temp = 0 | (0x4000 >> 1) = 0x2000
    // inout[0] = 0x2000
    // temp != 0 so ndigits remains 2
    EXPECT_EQ(ret, 2, "two-digit shift by 1 should not reduce ndigits when top is non-zero");
    EXPECT_EQ(inout[0], (uint16_t)0x2000, "top digit after shift should be 0x2000");
    EXPECT_EQ(inout[1], (uint16_t)0x0000, "bottom digit after shift should remain 0");
}

// Test 3: Three digits, shift by 1 with complete reduction (all zeros after shift).
static void test_three_digits_shift1_full_reduction()
{
    uint16_t inout[3] = {0x0001, 0x0000, 0x0000};
    int ndigits = 3;
    int ret = uarb_shift(inout, ndigits, 1);

    // Expect all zeros and ndigits reduced to 0
    EXPECT_EQ(ret, 0, "three-digit zero-out after shift should reduce ndigits to 0");
    EXPECT_EQ(inout[0], (uint16_t)0x0000, "inout[0] should be 0 after shift");
    EXPECT_EQ(inout[1], (uint16_t)0x0000, "inout[1] should be 0 after shift");
    EXPECT_EQ(inout[2], (uint16_t)0x0000, "inout[2] should be 0 after shift");
}

// Test 4: Two digits, shift by 1 with top digit becoming zero but earlier non-zero.
// This checks propagation where top becomes zero only after final iteration.
static void test_two_digits_shift1_with_nonzero_top()
{
    uint16_t inout[2] = {0x8000, 0x0000};
    int ndigits = 2;
    int ret = uarb_shift(inout, ndigits, 1);

    // i=1: temp=0 -> inout[1]=0
    // i=0: temp=(0|0x8000>>1)=0x4000 -> inout[0]=0x4000
    // temp != 0, so ndigits remains 2
    EXPECT_EQ(ret, 2, "two-digit shift should keep ndigits when top isn't reduced to 0");
    EXPECT_EQ(inout[0], (uint16_t)0x4000, "top digit after shift should be 0x4000");
    EXPECT_EQ(inout[1], (uint16_t)0x0000, "bottom digit after shift should be 0");
}

// Test 5: Two digits, shift by 1 with non-trivial carry-like behavior in multi-byte setup.
static void test_two_digits_shift1_carry_like()
{
    // Slightly more interesting: top has a LSB that will become the carry into next step
    uint16_t inout[2] = {0x8001, 0x0000};
    int ndigits = 2;
    int ret = uarb_shift(inout, ndigits, 1);

    // i=1: temp=0; inout[1]=0
    // i=0: temp = (0 | 0x8001>>1) = 0x4000
    // inout[0] becomes 0x4000
    // ndigits remains 2
    EXPECT_EQ(ret, 2, "two-digit shift should keep ndigits for non-zero top after shift");
    EXPECT_EQ(inout[0], (uint16_t)0x4000, "top digit after shift should be 0x4000");
    EXPECT_EQ(inout[1], (uint16_t)0x0000, "bottom digit after shift should be 0");
}

// Test 6: Three digits, shift by 4, ensure partial reductions and correct digit values.
static void test_three_digits_shift4_no_full_reduction()
{
    // Candidate Keywords: shift amount 4 with non-trivial lower nibble
    uint16_t inout[3] = {0x00F0, 0x0000, 0x0000};
    int ndigits = 3;
    int ret = uarb_shift(inout, ndigits, 4);

    // i=2: temp=0
    // i=1: temp=0
    // i=0: temp = 0x00F0 >> 4 = 0x000F
    // inout becomes {0x000F, 0x0000, 0x0000}
    // Since temp != 0 at i=0, ndigits remains 3
    EXPECT_EQ(ret, 3, "three-digit shift by 4 should keep ndigits as 3");
    EXPECT_EQ(inout[0], (uint16_t)0x000F, "top digit after shift should be 0x000F");
    EXPECT_EQ(inout[1], (uint16_t)0x0000, "middle digit after shift should remain 0");
    EXPECT_EQ(inout[2], (uint16_t)0x0000, "bottom digit after shift should remain 0");
}

// Entry point to run all tests
int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    // Run all test cases
    test_single_digit_shift_max();
    test_two_digits_shift1_no_reduction();
    test_three_digits_shift1_full_reduction();
    test_two_digits_shift1_with_nonzero_top();
    test_two_digits_shift1_carry_like();
    test_three_digits_shift4_no_full_reduction();

    // Summary
    std::cout << "Unit Test Summary: "
              << g_tests_run << " tests run, "
              << g_tests_failed << " failures." << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}