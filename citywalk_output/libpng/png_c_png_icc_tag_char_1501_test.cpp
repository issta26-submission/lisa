// Unit test suite for the focal method: png_icc_tag_char
// This test suite is designed to be compiled without Google Test (GTest) and uses
// a lightweight, non-terminating assertion style to maximize code coverage.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>


// Declaration of the focal C function with C linkage for proper linking.
// We assume the underlying implementation is provided (e.g., in png.c) and
// linked together with this test file.
extern "C" char png_icc_tag_char(uint32_t byte);

// Global counters for test results
static int g_tests_run = 0;
static int g_failures = 0;

// Lightweight assertion helper: checks expected vs actual without aborting the test run.
// Prints per-test results and keeps execution flowing for full coverage.
static void check_char(char actual, char expected, const char* test_name) {
    ++g_tests_run;
    if (actual != expected) {
        std::cerr << "[FAIL] " << test_name
                  << " | Expected: '" << expected
                  << "' (ASCII " << int(expected) << "), "
                  << "Got: '" << actual
                  << "' (ASCII " << int(actual) << ")" << std::endl;
        ++g_failures;
    } else {
        std::cout << "[PASS] " << test_name << std::endl;
    }
}

// Test 1: Printable ASCII lower bound
// Domain: 0x20 (space). Should return the same printable character ' '.
static void test_png_icc_tag_char_printable_space() {
    // GIVEN a byte whose lower printable ASCII value is 0x20
    uint32_t input = 0x20;
    // WHEN passed to png_icc_tag_char
    char result = png_icc_tag_char(input);
    // THEN it should return a space character
    check_char(result, ' ', "test_png_icc_tag_char_printable_space (0x20)");
}

// Test 2: Printable ASCII upper bound
// Domain: 0x7E ('~'). Should return the same printable character '~'.
static void test_png_icc_tag_char_printable_tilde() {
    uint32_t input = 0x7E;
    char result = png_icc_tag_char(input);
    check_char(result, '~', "test_png_icc_tag_char_printable_tilde (0x7E)");
}

// Test 3: Printable ASCII mid-range
// Domain: 0x41 ('A'). Should return 'A'.
static void test_png_icc_tag_char_printable_A() {
    uint32_t input = 0x41;
    char result = png_icc_tag_char(input);
    check_char(result, 'A', "test_png_icc_tag_char_printable_A (0x41)");
}

// Test 4: Below printable range
// Domain: 0x1F (31). Should return '?' as per 0x1F & 0xFF -> 31, which is below 32.
static void test_png_icc_tag_char_below_min() {
    uint32_t input = 0x1F;
    char result = png_icc_tag_char(input);
    check_char(result, '?', "test_png_icc_tag_char_below_min (0x1F)");
}

// Test 5: Above printable range
// Domain: 0x7F (127). Should return '?' since 127 is outside 32..126.
static void test_png_icc_tag_char_above_max() {
    uint32_t input = 0x7F;
    char result = png_icc_tag_char(input);
    check_char(result, '?', "test_png_icc_tag_char_above_max (0x7F)");
}

// Test 6: Large input with wrap (mask to 0xFF -> 0x78 -> 'x')
// Domain: 0x12345678 masks to 0x78 (120) which is printable 'x'
static void test_png_icc_tag_char_wraps_to_printable() {
    uint32_t input = 0x12345678;
    char result = png_icc_tag_char(input);
    check_char(result, 'x', "test_png_icc_tag_char_wraps_to_printable (0x12345678 -> 0x78)");
}

// Test 7: Zero input
// Domain: 0x00 -> below printable range, should return '?'
static void test_png_icc_tag_char_zero_input() {
    uint32_t input = 0x00000000;
    char result = png_icc_tag_char(input);
    check_char(result, '?', "test_png_icc_tag_char_zero_input (0x0)");
}

// Test 8: Maximum unsigned 32-bit input
// Domain: 0xFFFFFFFF masks to 0xFF -> 255, which is non-printable, should return '?'
static void test_png_icc_tag_char_max_uint32() {
    uint32_t input = 0xFFFFFFFF;
    char result = png_icc_tag_char(input);
    check_char(result, '?', "test_png_icc_tag_char_max_uint32 (0xFFFFFFFF)");
}

// Test 9: Printable ASCII mid-range with 0x3F ('?')
// Domain: 0x3F is printable (it's '?'). Should return '?'.
static void test_png_icc_tag_char_question_mark_input() {
    uint32_t input = 0x3F;
    char result = png_icc_tag_char(input);
    check_char(result, '?', "test_png_icc_tag_char_question_mark_input (0x3F)");
}

// Test 10: Printable ASCII digit
// Domain: 0x30 ('0'), should return '0'
static void test_png_icc_tag_char_printable_digit_0() {
    uint32_t input = 0x30;
    char result = png_icc_tag_char(input);
    check_char(result, '0', "test_png_icc_tag_char_printable_digit_0 (0x30)");
}

// Runner that executes all tests and prints a final summary
static void run_all_tests() {
    test_png_icc_tag_char_printable_space();
    test_png_icc_tag_char_printable_tilde();
    test_png_icc_tag_char_printable_A();
    test_png_icc_tag_char_below_min();
    test_png_icc_tag_char_above_max();
    test_png_icc_tag_char_wraps_to_printable();
    test_png_icc_tag_char_zero_input();
    test_png_icc_tag_char_max_uint32();
    test_png_icc_tag_char_question_mark_input();
    test_png_icc_tag_char_printable_digit_0();

    // Summary
    std::cout << "Total tests run: " << g_tests_run
              << ", Failures: " << g_failures << std::endl;
}

// Entry point: run tests from main to avoid requiring a test framework.
// This satisfies the requirement to call test methods from main when GTest is not used.
int main() {
    run_all_tests();
    // Return non-zero if any test failed to aid automated build systems.
    return (g_failures == 0) ? 0 : 1;
}