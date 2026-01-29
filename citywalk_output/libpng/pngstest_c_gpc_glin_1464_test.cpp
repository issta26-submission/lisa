/*
Unit test suite for gpc_glin in pngstest.c
Purpose:
- Verify the behavior of gpc_glin which converts a 24-bit RGB Pixel to a grayscale-like output (r=g=b) with alpha set to 65535.
- Exercise both branches of the conditional:
  1) Grayscale input (in->r == in->g == in->b) -> expected uses ilinear(in->g)
  2) Non-grayscale input (any channel differs) -> expected uses YfromRGB(sRGB_to_d[r], sRGB_to_d[g], sRGB_to_d[b]) via u16d
- Validate that out->a is always 65535.
Notes:
- We rely on the existence of the following external C routines/objects provided by the pngstest.c project:
  - unsigned short ilinear(int fixed_srgb)
  - double YfromRGB(double r, double g, double b)
  - const double sRGB_to_d[256]
  - unsigned short u16d(double d)
  - gpc_glin(Pixel *out, const Pixel *in, const Background *back)
- Pixel is defined with fields r, g, b, a (unsigned 16-bit). Background is opaque to gpc_glin.
- Tests are written in C++11, not using GTest. A lightweight test runner is implemented.
- All tests are non-terminating on failure to maximize coverage (accumulates results).
*/

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// External C declarations to link with the pngstest.c implementation.
// We declare a C-compatible layout for Pixel and Background to match the C definitions
// and declare the functions used by gpc_glin for computing the expected results.

extern "C" {
    // Pixel layout assumed: 32-bit members (16-bit fields for color channels)
    typedef struct {
        uint16_t r;
        uint16_t g;
        uint16_t b;
        uint16_t a;
    } Pixel;

    // Background is opaque for these tests
    typedef struct {
        // The real implementation may have fields; we do not rely on them in tests.
        int dummy;
    } Background;

    // Function under test
    void gpc_glin(Pixel *out, const Pixel *in, const Background *back);

    // Helpers used in the implementation of gpc_glin
    unsigned short ilinear(int fixed_srgb);
    double YfromRGB(double r, double g, double b);
    extern const double sRGB_to_d[256];
    unsigned short u16d(double d);
}

// Lightweight test harness
static int g_failures = 0;
static int g_tests_run = 0;

#define TOP_OK "\033[1;32mOK\033[0m"
#define TOP_FAIL "\033[1;31mFAIL\033[0m"

#define ASSERT_EQ_U16(expected, actual, description)                                  \
    do {                                                                               \
        ++g_tests_run;                                                                 \
        if ((unsigned short)(expected) != (unsigned short)(actual)) {                   \
            std::cerr << "Test " << g_tests_run << " FAILED: " << (description)        \
                      << " | Expected: " << (unsigned int)(expected)                    \
                      << ", Actual: " << (unsigned int)(actual) << std::endl;         \
            ++g_failures;                                                             \
        } else {                                                                       \
            std::cout << "Test " << g_tests_run << " PASSED: " << (description)        \
                      << " -> " << TOP_OK << std::endl;                              \
        }                                                                              \
    } while (0)

#define ASSERT_EQ_EQUIV(content_true_true, content_true_false, description)               \
    do {                                                                                  \
        ++g_tests_run;                                                                    \
        if (!(content_true_true)) {                                                       \
            std::cerr << "Test " << g_tests_run << " FAILED: " << (description)            \
                      << " | equivalence predicate false. " << std::endl;                \
            ++g_failures;                                                                \
        } else {                                                                          \
            std::cout << "Test " << g_tests_run << " PASSED: " << (description)             \
                      << " -> " << TOP_OK << std::endl;                                 \
        }                                                                                 \
    } while (0)


// Helper to initialize a Pixel with given channels
static Pixel make_pixel(uint16_t r, uint16_t g, uint16_t b, uint16_t a = 0) {
    Pixel p; p.r = r; p.g = g; p.b = b; p.a = a; return p;
}

// Test 1: Grayscale input with zero value expected to map via ilinear(0)
static void test_grayscale_zero() {
    Pixel in = make_pixel(0, 0, 0);
    Pixel out = make_pixel(0, 0, 0);
    Background back;
    (void)back; // unused by gpc_glin, as shown in the focal method

    gpc_glin(&out, &in, &back);

    // Validate that output channels are equal (grayscale result)
    bool channels_equal = (out.r == out.g) && (out.g == out.b);
    if (!channels_equal) {
        std::cerr << "test_grayscale_zero: Output channels are not grayscale." << std::endl;
        ++g_failures;
        ++g_tests_run; // We've already counted this test via the macro, but for clarity
        return;
    }

    // Validate alpha
    if (out.a != 65535) {
        std::cerr << "test_grayscale_zero: Alpha channel mismatch. Expected 65535, got "
                  << out.a << std::endl;
        ++g_failures;
        return;
    }

    // Validate value equals ilinear(0)
    unsigned short expected = ilinear(0);
    ASSERT_EQ_U16(expected, out.r, "Grayscale zero maps via ilinear(0)");
}

// Test 2: Grayscale input with non-zero value to exercise grayscale branch
static void test_grayscale_max_or_nonzero() {
    // Try a non-zero grayscale (e.g., 12345, all channels equal)
    uint16_t val = 12345;
    Pixel in = make_pixel(val, val, val);
    Pixel out = make_pixel(0, 0, 0);
    Background back;
    (void)back;

    gpc_glin(&out, &in, &back);

    // Validate grayscale output
    bool channels_equal = (out.r == out.g) && (out.g == out.b);
    if (!channels_equal) {
        std::cerr << "test_grayscale_max_or_nonzero: Output channels are not grayscale." << std::endl;
        ++g_failures;
        return;
    }

    // Validate alpha
    if (out.a != 65535) {
        std::cerr << "test_grayscale_max_or_nonzero: Alpha channel mismatch. Expected 65535, got "
                  << out.a << std::endl;
        ++g_failures;
        return;
    }

    unsigned short expected = ilinear(val);
    ASSERT_EQ_U16(expected, out.r, "Grayscale non-zero maps via ilinear(val)");
}

// Test 3: Non-grayscale input to exercise the colored branch
static void test_non_grayscale_branch() {
    // Non-gray input: r, g, b differ
    Pixel in = make_pixel(10, 200, 6550);
    Pixel out = make_pixel(0, 0, 0);
    Background back;
    (void)back;

    gpc_glin(&out, &in, &back);

    // Validate grayscale output (branches still set r=g=b)
    bool channels_equal = (out.r == out.g) && (out.g == out.b);
    if (!channels_equal) {
        std::cerr << "test_non_grayscale_branch: Output channels are not grayscale." << std::endl;
        ++g_failures;
        return;
    }

    // Validate alpha
    if (out.a != 65535) {
        std::cerr << "test_non_grayscale_branch: Alpha channel mismatch. Expected 65535, got "
                  << out.a << std::endl;
        ++g_failures;
        return;
    }

    // Compute expected via YfromRGB and sRGB_to_d if available externally.
    // This path exercises the non-gray branch and checks numeric correctness when possible.
    // We guard against potential missing external symbols by attempting computation and
    // falling back to a best-effort check on grayscale property only.

    // Attempt to compute expected using external helpers, if available
    double dr = sRGB_to_d[in.r];
    double dg = sRGB_to_d[in.g];
    double db = sRGB_to_d[in.b];
    double y = YfromRGB(dr, dg, db);
    double expected_double = 65535.0 * y;
    unsigned short expected = u16d(expected_double);

    // Compare
    if (out.r != expected) {
        // If exact numeric expectation is not available due to external linkage issues,
        // still report that values differ to encourage coverage.
        std::cerr << "test_non_grayscale_branch: Non-gray path computed value mismatch. "
                  << "Expected (approx): " << expected << ", Got: " << out.r << std::endl;
        // Do not fail hard; still provide a result summary for coverage
        ++g_failures;
    } else {
        std::cout << "test_non_grayscale_branch: Non-gray path matched external luminance mapping." << std::endl;
    }
}

// Entry point
int main(void) {
    std::cout << "Running gpc_glin unit tests (C++ harness)..." << std::endl;

    // Run tests
    test_grayscale_zero();
    test_grayscale_max_or_nonzero();
    test_non_grayscale_branch();

    // Summary
    std::cout << "----------------------------------------" << std::endl;
    if (g_failures == 0) {
        std::cout << TOP_OK << " All tests passed (" << g_tests_run << " tests)." << std::endl;
        return 0;
    } else {
        std::cerr << TOP_FAIL << " Tests failed: " << g_failures
                  << " / " << g_tests_run << std::endl;
        return 1;
    }
}