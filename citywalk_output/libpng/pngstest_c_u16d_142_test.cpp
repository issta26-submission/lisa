// Test suite for the focal method u16d(double) from pngstest.c
// This test is written in C++11 (no GTest) and relies on the C interface provided by the project.
// The tests exercise the relationship: u16d(d) == (png_uint_16)closestinteger(d)
// Candidate Keywords covered: u16d, closestinteger, png_uint_16, casting, double, boundary values, range [0..65535]

#include <vector>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <png.h>
#include <ctype.h>


// Import the C functions and types from the project under test.
// We assume libpng's header provides png_uint_16 and that u16d/closestinteger have C linkage.
extern "C" {
png_uint_16 u16d(double d);
double closestinteger(double x);
}

// Simple non-terminating assertion-like helper suitable for a lightweight test harness.
// It reports mismatch but does not abort the test execution immediately.
static int g_failures = 0;
static void check_equal(const char* test_name, png_uint_16 expected, png_uint_16 actual) {
    if (expected != actual) {
        std::cerr << "[FAIL] " << test_name
                  << " | expected: " << static_cast<unsigned>(expected)
                  << ", actual: " << static_cast<unsigned>(actual) << "\n";
        ++g_failures;
    } else {
        std::cout << "[PASS] " << test_name << "\n";
    }
}

// Step 2 & 3: Generate and refine test cases for u16d
// The tests evaluate that u16d(d) equals (png_uint_16)closestinteger(d) for a variety of
// non-negative input values where the intermediate rounding result is representable
// within the 0..65535 range. Negative inputs would lead to undefined behavior when casting
// to an unsigned type in C, so they are intentionally avoided in this test suite.
static void test_u16d_matches_closestinteger_for_various_inputs() {
    // True/false branch coverage is not applicable to a simple arithmetic wrapper here,
    // but we still cover a diverse set of inputs including edge-like values around
    // typical rounding thresholds (e.g., .5 cases) and large values near the 16-bit limit.
    // Domain-specific notes:
    // - Inputs are chosen to ensure closestinteger(d) is within [0, 65535].
    // - We compare u16d(d) against the direct cast of closestinteger(d) to png_uint_16.
    struct TestCase {
        double input;
        const char* label;
    };
    const TestCase tests[] = {
        { 0.0,           "d == 0.0" },
        { 0.3,           "d < 0.5 -> rounds to 0" },
        { 0.5,           "d == 0.5 -> rounds to 1" },
        { 1.2,           "small positive value" },
        { 1.5,           "half boundary -> rounds up" },
        { 2.5,           "half boundary -> rounds up" },
        { 10.0,          "tens value" },
        { 12345.6,       "mid-range value" },
        { 32767.2,       "near mid 32767" },
        { 32767.6,       "near mid 32768 after rounding" },
        { 32768.0,       "exact 32768" },
        { 65534.2,       "near 65535" },
        { 65534.6,       "near 65535 with rounding up" },
        { 65535.0,       "maximum representable after rounding" }
    };
    const int n = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < n; ++i) {
        double d = tests[i].input;
        const char* name = tests[i].label;

        // Expected value computed from closestinteger(d) then cast to 16-bit unsigned.
        double rounded = closestinteger(d);
        png_uint_16 expected = (png_uint_16)rounded;

        // Actual value produced by the focal method under test.
        png_uint_16 actual = u16d(d);

        // Report result with a clear label.
        char test_name[256];
        std::snprintf(test_name, sizeof(test_name),
                      "u16d(%g) == (png_uint_16)closestinteger(%s)",
                      d, name);

        check_equal(test_name, expected, actual);
    }
}

// Entrypoint for the test suite.
// We run the focused test above and then report a final summary.
// Per the domain knowledge, if GTest isn't allowed, we invoke test methods from main.
int main() {
    std::cout << "Starting unit tests for u16d(double d)...\n";

    test_u16d_matches_closestinteger_for_various_inputs();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0; // success
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1; // failure
    }
}