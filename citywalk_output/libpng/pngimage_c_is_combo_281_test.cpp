// Test suite for the focal method is_combo(int transforms) extracted from pngimage.c
// This test suite is written in C++11 style without using Google Test.
// It uses a lightweight, non-terminating assertion approach to maximize code coverage
// and continues execution after a failure to exercise more code paths.
//
// Context and Candidate Keywords (from Step 1):
// - Candidate Keywords: transforms, bitwise AND, multi-bit vs single-bit, zero input, negative input
// - Functionality: is_combo computes transforms & (transforms - 1), which is non-zero
//   when more than one bit is set in the integer 'transforms' (i.e., not a power of two and not zero).
// - Purpose for tests: verify zero-result for 0 and single-bit inputs, and non-zero for inputs with
//   multiple set bits (including edge cases like negative values).
//
// Notes:
// - The function is declared with C linkage to allow linking with the C source (pngimage.c).
// - We avoid using private/internal details or private state beyond what is exported by the focal function.

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>


// The focal function is_combo is defined in a C source file (pngimage.c).
// Declare it here with C linkage so the C++ test code can link properly.
extern "C" int is_combo(int transforms);

// Lightweight test harness (non-terminating assertions)
static int tests_run = 0;
static int tests_failed = 0;

// Report test result without aborting on failure
static void report_result(const char* test_name, bool passed) {
    ++tests_run;
    if (passed) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        ++tests_failed;
        std::cerr << "[FAIL] " << test_name << std::endl;
    }
}

// Test 1: Zero input should yield zero (transforms = 0)
 // Candidate keyword mapping: transforms = 0, edge case, zero-set-bits
static void test_is_combo_zero() {
    const char* test_name = "test_is_combo_zero (input 0) expects 0";
    int input = 0;
    int expected = 0;
    int actual = is_combo(input);
    report_result(test_name, actual == expected);
}

// Test 2: Single-bit input should yield zero
 // Candidate keyword mapping: single-bit, power of two, boundary case
static void test_is_combo_single_bit() {
    const char* test_name = "test_is_combo_single_bit (input 1) expects 0";
    int input = 1; // 0b1
    int expected = 0; // 1 & 0 = 0
    int actual = is_combo(input);
    report_result(test_name, actual == expected);
}

// Test 3: Small multi-bit input (e.g., 3 -> 0b11) should yield non-zero
 // Candidate keyword mapping: multiple-set-bits, small integers
static void test_is_combo_small_multi_bits() {
    const char* test_name = "test_is_combo_small_multi_bits (input 3) expects non-zero";
    int input = 3; // 0b11
    int expected = input & (input - 1); // 3 & 2 = 2
    int actual = is_combo(input);
    report_result(test_name, actual == expected);
}

// Test 4: Larger multi-bit input (e.g., 5 -> 0b101) should yield non-zero
 // Candidate keyword mapping: multiple-set-bits, non-contiguous bits
static void test_is_combo_large_multi_bits() {
    const char* test_name = "test_is_combo_large_multi_bits (input 5) expects non-zero";
    int input = 5; // 0b0101
    int expected = input & (input - 1); // 5 & 4 = 4
    int actual = is_combo(input);
    report_result(test_name, actual == expected);
}

// Test 5: Very large two-bit input (e.g., 0x40000001) should yield non-zero
 // Candidate keyword mapping: high-bit, two-set-bits
static void test_is_combo_high_bit_two_set() {
    const char* test_name = "test_is_combo_high_bit_two_set (input 0x40000001) expects non-zero";
    int input = 0x40000001; // two bits set: bit 30 and bit 0
    int expected = input & (input - 1); // removes lowest set bit
    int actual = is_combo(input);
    report_result(test_name, actual == expected);
}

// Test 6: Negative input should yield non-zero (undefined behavior depends on representation,
// but for two's complement it should be non-zero for -1, etc.)
 // Candidate keyword mapping: negative input handling, two's complement
static void test_is_combo_negative_input() {
    const char* test_name = "test_is_combo_negative_input (input -1) expects non-zero";
    int input = -1;               // All bits set
    int expected = (-1) & (-2);    // -2 (0x...FE) in two's complement
    int actual = is_combo(input);
    report_result(test_name, actual == expected);
}

// Test 7: Composite runner to ensure multiple bits with one zero between (e.g., 0x40000003)
 // Candidate keyword mapping: mixed bits, cross-check with formula
static void test_is_combo_mixed_bits() {
    const char* test_name = "test_is_combo_mixed_bits (input 0x40000003) expects non-zero";
    int input = 0x40000003; // two bits: bit 30 and bits 0-1
    int expected = input & (input - 1); // removes lowest set bit
    int actual = is_combo(input);
    report_result(test_name, actual == expected);
}

// Runner to execute all tests
static void run_all_tests() {
    test_is_combo_zero();
    test_is_combo_single_bit();
    test_is_combo_small_multi_bits();
    test_is_combo_large_multi_bits();
    test_is_combo_high_bit_two_set();
    test_is_combo_negative_input();
    test_is_combo_mixed_bits();
}

// main function: if a test harness is required, this is invoked to execute tests
int main(void) {
    // Explain purpose briefly
    std::cout << "Starting is_combo unit tests (no GTest; non-terminating assertions):" << std::endl;
    run_all_tests();
    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << std::endl;
    return (tests_failed > 0) ? 1 : 0;
}