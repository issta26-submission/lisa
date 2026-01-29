// Test suite for the focal method: uarb_mult_digit
// Target: validate behavior of the one-digit multiply-and-accumulate routine.
//
// Assumptions for test harness (to be linked with the project under test):
// - The function uarb_mult_digit is defined with C linkage in the library/source.
// - Type alias: uarb is a pointer to 16-bit digits (little-endian, least-significant digit first).
// - We declare the function here with compatible types for linking with the actual implementation.
// - We do not rely on any GTest; a small, non-terminating assertion framework is implemented below.
// - The test code is written in C++11 but interacts with C-style data/types.

#include <cstdint>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Domain-specific note: This test focuses on the logical behavior of the algorithm
// under several scenarios, including zero multiplier, zero digits, simple
// multiplication without carry, and carry extending to an extra digit.

extern "C" {
    // Declaration of the focal function (signature inferred from focal method).
    // The actual project provides these types; we mirror them here for linking.
    // typedef uint16_t png_uint_16;
    // typedef uint16_t* uarb;
    int uarb_mult_digit(uint16_t *acc, int a_digits, uint16_t *num, int n_digits, uint16_t val);
}

// Lightweight non-terminating assertion helpers (to maximize coverage without stopping tests)
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { fprintf(stderr, "EXPECT_TRUE FAILED: %s\n", msg); ++g_failures; } } while(0)

#define EXPECT_EQ(expected, actual, msg) \
    do { if((expected) != (actual)) { fprintf(stderr, "EXPECT_EQ FAILED: %s (expected %lld, got %lld)\n", msg, (long long)(expected), (long long)(actual)); ++g_failures; } } while(0)

#define EXPECT_MEMEQ(expected_ptr, actual_ptr, nbytes, msg) \
    do { if(0 != memcmp((expected_ptr), (actual_ptr), (nbytes))) { fprintf(stderr, "EXPECT_MEMEQ FAILED: %s\n", msg); ++g_failures; } } while(0)

// Test 1: val == 0 => no multiplication, acc should be unchanged and function should return a_digits
void test_val_zero_no_change() {
    // Setup: a_digits = 2, n_digits = 2
    uint16_t acc[4] = { 0x1234, 0x5678, 0x0000, 0x0000 };
    uint16_t num[2] = { 0x9ABC, 0xDEF0 };
    int a_digits = 2;
    int n_digits = 2;
    uint16_t val = 0; // zero multiplier

    uint16_t acc_before[4];
    std::memcpy(acc_before, acc, sizeof(acc));

    int ret = uarb_mult_digit(acc, a_digits, num, n_digits, val);

    // Expectation: return a_digits, acc remains unchanged
    EXPECT_EQ(a_digits, ret, "return value should be a_digits when val == 0");
    EXPECT_MEMEQ(acc_before, acc, 4 * sizeof(uint16_t), "acc should be unchanged when val == 0");
}

// Test 2: n_digits == 0 => no operation, acc unchanged, returns a_digits
void test_n_digits_zero_no_change() {
    // Setup: a_digits = 2, n_digits = 0
    uint16_t acc[4] = { 0x1111, 0x2222, 0x0000, 0x0000 };
    uint16_t num[2] = { 0x3333, 0x4444 };
    int a_digits = 2;
    int n_digits = 0;
    uint16_t val = 0x1234; // non-zero
    uint16_t acc_before[4];
    std::memcpy(acc_before, acc, sizeof(acc));

    int ret = uarb_mult_digit(acc, a_digits, num, n_digits, val);

    // Expectation: return a_digits, acc unchanged
    EXPECT_EQ(a_digits, ret, "return value should be a_digits when n_digits == 0");
    EXPECT_MEMEQ(acc_before, acc, 4 * sizeof(uint16_t), "acc should be unchanged when n_digits == 0");
}

// Test 3: Simple multiplication with no carry beyond existing digits
void test_simple_multiply_no_carry() {
    // acc = [0, 0], num = [1, 2], val = 3
    // Expected: acc becomes [3, 6], return a_digits (2)
    uint16_t acc[4] = { 0x0000, 0x0000, 0, 0 };
    uint16_t num[2] = { 0x0001, 0x0002 };
    int a_digits = 2;
    int n_digits = 2;
    uint16_t val = 3;

    int ret = uarb_mult_digit(acc, a_digits, num, n_digits, val);

    EXPECT_EQ(2, ret, "return value should be a_digits when operation fits within digits");
    EXPECT_EQ(0x0003, acc[0], "acc[0] should be 3 (1*3)");
    EXPECT_EQ(0x0006, acc[1], "acc[1] should be 6 (2*3)");
}

// Test 4: Carry extends to an extra digit beyond a_digits
void test_carry_extends_to_extra_digit() {
    // Setup designed to force an extra digit:
    // a_digits = 1, acc[0] = 0xFFFF, acc[1] unused (0)
    // num = [1, 0], val = 1
    // After first digit: acc[0] becomes 0, carry = 1
    // Second digit: acc[1] becomes 1, no further carry
    // Should return 2 (out_digits > a_digits)
    uint16_t acc[4] = { 0xFFFF, 0x0000, 0, 0 };
    uint16_t num[2] = { 0x0001, 0x0000 };
    int a_digits = 1;
    int n_digits = 2;
    uint16_t val = 0x0001;

    int ret = uarb_mult_digit(acc, a_digits, num, n_digits, val);

    EXPECT_EQ(2, ret, "should return out_digits when a carry extends beyond a_digits");
    EXPECT_EQ(0x0000, acc[0], "acc[0] should be 0 after first digit (0xFFFF + 1 = 0, carry 1)");
    EXPECT_EQ(0x0001, acc[1], "acc[1] should be 1 due to carry into new digit");
}

// Runner to execute all tests
void run_all_tests() {
    fprintf(stderr, "Running tests for uarb_mult_digit...\n");
    test_val_zero_no_change();
    test_n_digits_zero_no_change();
    test_simple_multiply_no_carry();
    test_carry_extends_to_extra_digit();
}

// Entry point (non-GTest style)
int main(void) {
    run_all_tests();
    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TESTS FAILED: %d failure(s)\n", g_failures);
        return 1;
    }
}