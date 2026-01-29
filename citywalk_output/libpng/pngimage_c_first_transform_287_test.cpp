// C++11 compatible unit tests for the focal method: first_transform(int transforms)
// The method returns the lowest set bit of the input (transforms & -transforms).
// This test suite is designed to be compiled alongside the provided C source (pngimage.c)
// and linked with it. It uses a lightweight, non-terminating assertion mechanism.

#include <cstdint>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>


// Declare the focal C function with C linkage so the C++ test code can call it.
extern "C" int first_transform(int transforms);

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Expectation macro: records pass/fail and prints diagnostic on failure.
#define EXPECT_EQ_DESC(description, expected, actual)                                         \
    do {                                                                                        \
        ++g_total_tests;                                                                        \
        if ((expected) == (actual)) {                                                           \
            ++g_passed_tests;                                                                   \
        } else {                                                                                \
            std::cerr << "TEST FAILED: " << (description) << " | expected: "               \
                      << (expected) << ", actual: " << (actual) << std::endl;               \
        }                                                                                       \
    } while (0)


// Test 1: Zero input should yield 0 (no bits set)
static void test_zero_input() {
    // Domain knowledge: lowest set bit of 0 is 0 (no bits set)
    EXPECT_EQ_DESC("first_transform(0) should be 0", 0, first_transform(0));
}

// Test 2: Powers of two should return themselves as the lowest set bit
// Coverage: true branch for powers of two; ensures identity for single-bit numbers
static void test_powers_of_two() {
    EXPECT_EQ_DESC("first_transform(1) should be 1", 1, first_transform(1));
    EXPECT_EQ_DESC("first_transform(2) should be 2", 2, first_transform(2));
    EXPECT_EQ_DESC("first_transform(4) should be 4", 4, first_transform(4));
    EXPECT_EQ_DESC("first_transform(8) should be 8", 8, first_transform(8));
}

// Test 3: Non-power-of-two values should return the least significant bit set
// Coverage: multiple common cases to ensure correct computation
static void test_non_powers_of_two() {
    EXPECT_EQ_DESC("first_transform(3) should be 1", 1, first_transform(3));      // 11 -> 01
    EXPECT_EQ_DESC("first_transform(5) should be 1", 1, first_transform(5));      // 101 -> 001
    EXPECT_EQ_DESC("first_transform(6) should be 2", 2, first_transform(6));      // 110 -> 010
    EXPECT_EQ_DESC("first_transform(12) should be 4", 4, first_transform(12));    // 1100 -> 0100
    EXPECT_EQ_DESC("first_transform(0x7FFFFFF0) should be 0x10", 0x10, first_transform(0x7FFFFFF0)); // ends with 00010000
}

// Test 4: Negative inputs (two's-complement behavior assumptions)
// Note: The function uses transforms & -transforms; for negative values, this still yields
// the least significant bit of the two's-complement representation (within reasonable UB-safe range).
static void test_negative_inputs() {
    // -1 -> all ones; lowest set bit is 1
    EXPECT_EQ_DESC("first_transform(-1) should be 1", 1, first_transform(-1));
    // -2 -> 0x...FE; lowest set bit is 2
    EXPECT_EQ_DESC("first_transform(-2) should be 2", 2, first_transform(-2));
    // -3 -> 0x...FD; lowest set bit is 1
    EXPECT_EQ_DESC("first_transform(-3) should be 1", 1, first_transform(-3));
    // -4 -> 0x...FC; lowest set bit is 4
    EXPECT_EQ_DESC("first_transform(-4) should be 4", 4, first_transform(-4));
    // Note: Avoid INT_MIN (-2147483648) due to potential undefined behavior in negation.
}

// Run all tests and report a summary
static void run_all_tests() {
    test_zero_input();
    test_powers_of_two();
    test_non_powers_of_two();
    test_negative_inputs();
    
    std::cerr << "Test summary: " << g_passed_tests << " / " << g_total_tests
              << " tests passed." << std::endl;
}

// Main entry point for the test suite
int main(void) {
    run_all_tests();
    if (g_total_tests == g_passed_tests) {
        // All tests passed
        return 0;
    } else {
        // Some tests failed; signal failure to test harness
        return 1;
    }
}