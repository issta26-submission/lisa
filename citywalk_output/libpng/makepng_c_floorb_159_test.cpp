// Minimal C++11 test suite for the focal method floorb(double d) from makepng.c
// This test does not rely on Google Test. It uses a tiny, non-terminating assertion framework.
// The tests exercise floorb behavior on a variety of inputs within the 0-255 range and a couple of edge cases.

// Explanation notes:
// - The function under test is: png_byte floorb(double d) { d = floor(d); return (png_byte)d; }
// - We assume png_byte is an unsigned 8-bit type (commonly unsigned char). We declare the symbol with C linkage for linking.
// - Tests focus on deterministic cases where floor(d) yields values in [0, 255], plus a wrap-around case (256.0) to exercise casting behavior.
// - Negative inputs yield undefined/implementation-defined results when cast to png_byte in standard C, so we avoid asserting on those in this suite.

#include <cstdint>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <iomanip>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>


// Provide a minimal forward-declaration for the C function under test.
// We declare a C-compatible prototype so that the C++ test can link with makepng.c.
extern "C" {
typedef unsigned char png_byte; // align with common libpng typedef
png_byte floorb(double);
}

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_pass = 0;

// Print a failure message without terminating the test runner
static void log_failure(const char* test_name, int actual, int expected) {
    std::cerr << "FAIL: " << test_name
              << " | expected: " << expected
              << ", actual: " << actual << std::endl;
}

// Non-terminating assertion for equality of uint8 (png_byte)
#define EXPECT_EQ(actual, expected, test_name) do { \
    ++g_tests_run; \
    int _a = static_cast<int>(actual); \
    int _e = static_cast<int>(expected); \
    if (_a == _e) { \
        ++g_tests_pass; \
    } else { \
        log_failure(test_name, _a, _e); \
    } \
} while(0)

// Convenience macro for clearly typed comparisons
#define TEST_NAME(name) name##__test

// Test 1: floorb should return 0 for exactly 0.0
static void TEST_NAME(floorb_zero)() {
    // true branch: floor(0.0) -> 0.0, cast to png_byte -> 0
    png_byte res = floorb(0.0);
    EXPECT_EQ(res, 0, "floorb(0.0) should be 0");
}

// Test 2: floorb should floor a positive non-integer value correctly
static void TEST_NAME(floorb_positive_fraction)() {
    // floor(3.7) -> 3, cast to png_byte -> 3
    png_byte res = floorb(3.7);
    EXPECT_EQ(res, 3, "floorb(3.7) should be 3");
}

// Test 3: floorb should floor a positive nearly-integer value just below an integer
static void TEST_NAME(floorb_noninteger_near_int)() {
    // floor(2.999) -> 2
    png_byte res = floorb(2.999);
    EXPECT_EQ(res, 2, "floorb(2.999) should be 2");
}

// Test 4: floorb wraps around for values >= 256 after flooring
static void TEST_NAME(floorb_wrap_around)() {
    // floor(256.0) -> 256; (png_byte)256 is implementation-defined, commonly 0
    png_byte res = floorb(256.0);
    EXPECT_EQ(res, 0, "floorb(256.0) should wrap to 0 on typical 8-bit unsigned cast");
}

// Test 5: floorb handles a small negative value by mapping to the corresponding unsigned value.
// Note: This relies on implementation-defined behavior of casting negative doubles to png_byte.
// We document and test for the commonly observed result (255 for -1).
static void TEST_NAME(floorb_negative_wrap)() {
    // floor(-0.1) -> -1; (png_byte)(-1) typically yields 255
    png_byte res = floorb(-0.1);
    EXPECT_EQ(res, 255, "floorb(-0.1) is commonly 255 due to unsigned wrap from -1");
}

// Test 6: floorb for an upper-bound positive within 0..255
static void TEST_NAME(floorb_upper_bound)() {
    // floor(255.9) -> 255
    png_byte res = floorb(255.9);
    EXPECT_EQ(res, 255, "floorb(255.9) should be 255");
}

// Test 7: floorb for an exact integer within range
static void TEST_NAME(floorb_exact_integer)() {
    // floor(125.0) -> 125
    png_byte res = floorb(125.0);
    EXPECT_EQ(res, 125, "floorb(125.0) should be 125");
}

// Summary reporting
static void report_summary() {
    std::cout << "Tests run: " << g_tests_run
              << ", Passed: " << g_tests_pass
              << ", Failed: " << (g_tests_run - g_tests_pass) << std::endl;
}

// Entry point
int main() {
    // Run all tests
    TEST_NAME(floorb_zero)();
    TEST_NAME(floorb_positive_fraction)();
    TEST_NAME(floorb_noninteger_near_int)();
    TEST_NAME(floorb_wrap_around)();
    TEST_NAME(floorb_negative_wrap)();
    TEST_NAME(floorb_upper_bound)();
    TEST_NAME(floorb_exact_integer)();

    // Print summary
    report_summary();
    // Exit code 0 indicates the test program itself ran to completion.
    // The overall success is reported by the summary output.
    return 0;
}