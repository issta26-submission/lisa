// Unit test suite for the focal method: channels_of_type(int color_type)
// This test harness is written in C++11 and does not rely on GoogleTest.
// It assumes the target library (makepng.c) is compiled and linked with this test.
// The test uses a minimal assertion mechanism that logs failures but continues execution.

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>


// Declaration of the focal function (assumed to be provided in makepng.c).
extern "C" int channels_of_type(int color_type);

// Global failure counter to allow non-terminating assertions.
static int g_failures = 0;

// Lightweight assertion helper: records a failure and prints a message without terminating tests.
static void assert_eq(int expected, int actual, const char* description) {
    if (expected != actual) {
        std::cerr << "Assertion failed: " << description
                  << " (expected " << expected << ", got " << actual << ")" << std::endl;
        ++g_failures;
    }
}

/*
Test Case 1: Palette branch
- If the color_type has the PALETTE mask set, channels_of_type should return 1.
- This tests the true-branch of the first if-statement and also ensures that
  palette bit takes precedence over other color bits.
*/
static void test_palette_branch() {
    int v;

    v = channels_of_type(1); // 0b0001: PALETTE set
    assert_eq(1, v, "Palette bit set should return 1");

    v = channels_of_type(3); // 0b0011: PALETTE + COLOR set
    assert_eq(1, v, "Palette bit set (with COLOR) should still return 1");

    v = channels_of_type(5); // 0b0101: PALETTE + ALPHA set
    assert_eq(1, v, "Palette bit set (with ALPHA) should still return 1");

    v = channels_of_type(9); // 0b1001: PALETTE set with higher bits
    assert_eq(1, v, "Palette bit set (higher bits) should still return 1");
}

/*
Test Case 2: Color without alpha
- Non-palette path: color_type has COLOR bit set and no ALPHA bit.
- Expected channels = 3 (RGB with no alpha).
*/
static void test_color_without_alpha() {
    int v = channels_of_type(2); // 0b0010: COLOR set, no PALETTE, no ALPHA
    assert_eq(3, v, "RGB without alpha should yield 3 channels");
}

/*
Test Case 3: Alpha without color (gray with alpha)
- Non-palette path: no COLOR bit, ALPHA bit set.
- Expected channels = 1, then +1 due to ALPHA = 2.
*/
static void test_alpha_without_color() {
    int v = channels_of_type(4); // 0b0100: ALPHA set, no COLOR
    assert_eq(2, v, "GRAY with ALPHA should yield 2 channels");
}

/*
Test Case 4: Color and alpha
- Non-palette path: COLOR bit and ALPHA bit both set.
- Expected channels = 3, then +1 due to ALPHA = 4.
*/
static void test_color_and_alpha() {
    int v = channels_of_type(6); // 0b0110: COLOR + ALPHA
    assert_eq(4, v, "RGB with ALPHA should yield 4 channels");
}

/*
Test Case 5: No relevant bits
- Non-palette path with none of COLOR or ALPHA.
- Expected channels = 1 (gray with no alpha).
*/
static void test_none_bits() {
    int v = channels_of_type(0); // 0b0000: no COLOR, no ALPHA, no PALETTE
    assert_eq(1, v, "No COLOR/ALPHA/PALETTE bits should yield 1 channel");
}

// Entry point for running all tests.
int main() {
    // Run all test cases.
    test_palette_branch();
    test_color_without_alpha();
    test_alpha_without_color();
    test_color_and_alpha();
    test_none_bits();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}