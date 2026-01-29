/*
  C++11 test suite for the focal method:
  uarb_set(uarb result, png_alloc_size_t val)

  - The tests exercise the real C function from pngfix.c by linking against libpng.
  - We declare the function with C linkage and use compatible types from libpng.
  - The tests cover:
     * val == 0 (no digits written)
     * val fits in a single 16-bit digit
     * val spans exactly two 16-bit digits
     * val spans four 16-bit digits (multi-digit case)

  Note: This test intentionally uses non-terminating assertions (continue execution)
  to maximize coverage, and reports results to stdout.
*/

#include <setjmp.h>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>


// Type alias matching the expected C prototype used in pngfix.c
using uarb = png_uint_16*;

// Declaration of the focal function with C linkage
extern "C" int uarb_set(uarb result, size_t val);

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple assertion helper that reports and continues
#define ASSERT_EQ(expected, actual, msg) do {                   \
    ++g_total_tests;                                              \
    if ((expected) != (actual)) {                                 \
        std::cerr << "TEST_FAIL: " << (msg) << " | Expected: "        \
                  << (expected) << " | Actual: " << (actual) << "\n"; \
        ++g_failed_tests;                                         \
    }                                                             \
} while (0)

#define ASSERT_TRUE(cond, msg) do {                                 \
    ++g_total_tests;                                                 \
    if (!(cond)) {                                                   \
        std::cerr << "TEST_FAIL: " << (msg) << " | Condition is false.\n"; \
        ++g_failed_tests;                                            \
    }                                                                \
} while (0)

// Utility to print a short array for debugging (not used in assertions)
static void print_uint16_array(const png_uint_16* arr, size_t n) {
    std::cout << "[";
    for (size_t i = 0; i < n; ++i) {
        if (i) std::cout << ", ";
        std::cout << std::hex << "0x" << arr[i] << std::dec;
    }
    std::cout << "]\n";
}

// Test 1: val == 0 should write 0 digits and leave the array unchanged
// Expected: returns 0 and result[] remains as before the call.
void test_zero_value() {
    // Prepare input
    png_uint_16 buf[4] = { 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD };
    png_uint_16 orig[4];
    std::memcpy(orig, buf, sizeof(buf));

    // Call the focal function
    int ndigits = uarb_set(buf, 0);

    // Assertions
    ASSERT_EQ(0, ndigits, "val == 0 should produce 0 digits");
    // Array should remain unchanged
    for (int i = 0; i < 4; ++i) {
        ASSERT_EQ(orig[i], buf[i], "buf should be unchanged when val == 0");
    }
}

// Test 2: val fits into a single 16-bit digit
// Expected: ndigits == 1, buf[0] == 1, rest unchanged
void test_single_digit() {
    png_uint_16 buf[4] = { 0x1234, 0xABCD, 0x0FFF, 0x0000 };
    png_uint_16 orig[4];
    std::memcpy(orig, buf, sizeof(buf));

    int ndigits = uarb_set(buf, 1); // val == 1

    ASSERT_EQ(1, ndigits, "val == 1 should produce 1 digit");
    ASSERT_EQ(1, buf[0], "buf[0] should be 1 after uarb_set");
    // Other digits should remain unchanged
    for (int i = 1; i < 4; ++i) {
        ASSERT_EQ(orig[i], buf[i], "buf[i] should remain unchanged for i>=1");
    }
}

// Test 3: val spans exactly two 16-bit digits
// val = 0x00010001 => digits [0x0001, 0x0001]
void test_two_digits() {
    png_uint_16 buf[4] = { 0xFFFF, 0xEEEE, 0xDDDD, 0xCCCC };
    png_uint_16 orig[4];
    std::memcpy(orig, buf, sizeof(buf));

    int ndigits = uarb_set(buf, 0x00010001); // val = 65537

    ASSERT_EQ(2, ndigits, "val = 0x00010001 should produce 2 digits");
    ASSERT_EQ(0x0001, buf[0], "buf[0] should be 0x0001");
    ASSERT_EQ(0x0001, buf[1], "buf[1] should be 0x0001");
    // Remaining digits should remain unchanged
    for (int i = 2; i < 4; ++i) {
        ASSERT_EQ(orig[i], buf[i], "buf[i] should remain unchanged for i>=2");
    }
}

// Test 4: val spans four 16-bit digits (multi-digit case)
 // val = 0x123456789ABCDEF0 => digits [0xDEF0, 0x9ABC, 0x5678, 0x1234]
void test_four_digits() {
    png_uint_16 buf[4] = { 0x0A0A, 0x0B0B, 0x0C0C, 0x0D0D };
    png_uint_16 orig[4];
    std::memcpy(orig, buf, sizeof(buf));

    int ndigits = uarb_set(buf, 0x123456789ABCDEF0ULL); // val with high 64-bit

    ASSERT_EQ(4, ndigits, "val spanning four 16-bit digits should produce 4 digits");
    ASSERT_EQ(0xDEF0, buf[0], "buf[0] should be 0xDEF0");
    ASSERT_EQ(0x9ABC, buf[1], "buf[1] should be 0x9ABC");
    ASSERT_EQ(0x5678, buf[2], "buf[2] should be 0x5678");
    ASSERT_EQ(0x1234, buf[3], "buf[3] should be 0x1234");
    // No further digits exist; ensure initial content of array is overwritten as expected
    // Validate no unintended changes beyond 4 elements (array size is 4)
}

// Driver main: run all tests
int main(int argc, char** argv) {
    // Run tests
    test_zero_value();
    test_single_digit();
    test_two_digits();
    test_four_digits();

    // Summary
    std::cout << "Tests run: " << g_total_tests << "\n";
    if (g_failed_tests == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failed_tests << " TEST(S) FAILED\n";
        return 1;
    }
}