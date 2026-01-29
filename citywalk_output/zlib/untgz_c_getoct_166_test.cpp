// Unit test suite for the C function getoct in untgz.c
// This test suite is tailored for C++11 environments, without using a testing framework.
// It links against the C function via C linkage and provides a simple test harness
// with non-terminating assertions (tests continue after failures).

#include <cstring>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>


// Declare the C function under test with C linkage.
extern "C" int getoct(char *p, int width);

// Simple non-terminating assertion mechanism
static int g_total = 0;
static int g_passed = 0;

#define ASSERT_EQ(expected, actual, name) do { \
    ++g_total; \
    if ((expected) == (actual)) { \
        ++g_passed; \
        std::cout << "[PASS] " << name << " | Expected: " << (expected) \
                  << " | Actual: " << (actual) << "\n"; \
    } else { \
        std::cout << "[FAIL] " << name << " | Expected: " << (expected) \
                  << " | Actual: " << (actual) << "\n"; \
    } \
} while(0)

// Helper to build a modifiable buffer from a string literal
static int call_getoct_from_literal(const char* input, int width) {
    char buf[256];
    std::strncpy(buf, input, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    return getoct(buf, width);
}

// Individual tests

// 1) Basic octal parsing without spaces: "123" -> 1*64 + 2*8 + 3 = 83
static void test_basic_octal_no_spaces() {
    const char* input = "123";
    int width = 3;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(83, result, "test_basic_octal_no_spaces");
}

// 2) Skipping spaces between digits: "1 2 3" with width 5 -> 83
static void test_skip_spaces_in_between() {
    const char* input = "1 2 3";
    int width = 5;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(83, result, "test_skip_spaces_in_between");
}

// 3) Invalid character outside '0'..'7' should return -1: "8" with width 1
static void test_invalid_char_returns_minus_one() {
    const char* input = "8";
    int width = 1;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(-1, result, "test_invalid_char_returns_minus_one");
}

// 4) Null terminator breaks the loop early: "12" with width 3 -> 1*8 + 2 = 10
static void test_null_terminator_stops_early() {
    const char* input = "12";
    int width = 3;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(10, result, "test_null_terminator_stops_early");
}

// 5) Width limiting: "1234" with width 2 -> processes only '1','2' => 10
static void test_width_limit_reduces_result() {
    const char* input = "1234";
    int width = 2;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(10, result, "test_width_limit_reduces_result");
}

// 6) Empty input should return 0
static void test_empty_input_returns_zero() {
    const char* input = "";
    int width = 5;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(0, result, "test_empty_input_returns_zero");
}

// 7) Single digit: "7" with width 1 -> 7
static void test_single_digit() {
    const char* input = "7";
    int width = 1;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(7, result, "test_single_digit");
}

// 8) Leading space should be skipped: " 7" with width 2 -> 7
static void test_leading_space_skipped() {
    const char* input = " 7";
    int width = 2;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(7, result, "test_leading_space_skipped");
}

// 9) Zero digit after a valid digit: "70" with width 2 -> 7*8 + 0 = 56
static void test_zero_digit_in_sequence() {
    const char* input = "70";
    int width = 2;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(56, result, "test_zero_digit_in_sequence");
}

// 10) Non-digit after a valid digit causes -1: "1x3" with width 3
static void test_non_digit_after_digit_yields_minus_one() {
    const char* input = "1x3";
    int width = 3;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(-1, result, "test_non_digit_after_digit_yields_minus_one");
}

// 11) Width equals number of digits with valid digits: "765" -> 501
static void test_three_digits_all_valid() {
    const char* input = "765";
    int width = 3;
    int result = call_getoct_from_literal(input, width);
    ASSERT_EQ(501, result, "test_three_digits_all_valid");
}

// 12) Stop on null in the middle with more width: "1" with width 5 -> 1
static void test_stop_on_null_middle_with_extra_width() {
    const char* input = "1";
    int width = 5;
    int result = call_getoct_from_literal(input, width);
    // After '1', next is '\0' and loop breaks; result remains 1
    ASSERT_EQ(1, result, "test_stop_on_null_middle_with_extra_width");
}

// Main: run all tests
int main() {
    std::cout << "Starting getoct.cpp unit tests...\n";

    test_basic_octal_no_spaces();
    test_skip_spaces_in_between();
    test_invalid_char_returns_minus_one();
    test_null_terminator_stops_early();
    test_width_limit_reduces_result();
    test_empty_input_returns_zero();
    test_single_digit();
    test_leading_space_skipped();
    test_zero_digit_in_sequence();
    test_non_digit_after_digit_yields_minus_one();
    test_three_digits_all_valid();
    test_stop_on_null_middle_with_extra_width();

    std::cout << "\nSummary: " << g_passed << " / " << g_total << " tests passed.\n";
    // Return non-zero if any test failed
    return (g_total == g_passed) ? 0 : 1;
}