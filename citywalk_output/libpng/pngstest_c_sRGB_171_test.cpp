// Lightweight C++11 unit test suite for the focal method sRGB(double linear)
// This test uses a minimal in-file test harness (no external test framework like GTest).
// It relies on the existence of the original C library symbols:
//   extern "C" unsigned char sRGB(double linear);
//   extern "C" double sRGB_from_linear(double linear);
//   extern "C" unsigned char u8d(double d);
// The tests compare against a standard gamma-corrected reference implementation.

#include <cstdint>
#include <algorithm>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <ctype.h>


// Declarations of the focal functions from the C project (to be linked during compilation)
extern "C" unsigned char sRGB(double linear);
extern "C" double sRGB_from_linear(double linear);
extern "C" unsigned char u8d(double d);

// Helper: standard SRGB inverse gamma transformation (reference implementation)
static double srgb_from_linear_std(double l)
{
    // Clamp input to [0,1] to mirror expected domain handling
    if (l <= 0.0) return 0.0;
    if (l >= 1.0) return 1.0;

    // Piecewise gamma correction (linear to sRGB in [0,1])
    if (l <= 0.0031308)
        return 12.92 * l;
    else
        return 1.055 * std::pow(l, 1.0/2.4) - 0.055;
}

// Helper: expected sRGB output for a given linear input, using the standard gamma model
static unsigned char expected_srgb_double(double linear)
{
    // Ensure domain [0,1]
    if (linear < 0.0) linear = 0.0;
    if (linear > 1.0) linear = 1.0;

    double s = srgb_from_linear_std(linear);
    double v = 255.0 * s;

    // Clamp result and round to nearest integer [0,255]
    if (v < 0.0) v = 0.0;
    if (v > 255.0) v = 255.0;
    int iv = static_cast<int>(v + 0.5);
    if (iv < 0) iv = 0;
    if (iv > 255) iv = 255;
    return static_cast<unsigned char>(iv);
}

// Simple test framework
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define LOG_MSG(msg) do { std::cerr << msg << std::endl; } while(0)
#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        LOG_MSG("Expectation failed: " << (msg)); \
    } \
} while(0)

static bool test_srgb_zero_input_matches_reference()
{
    // Test: sRGB(0.0) should map to 0
    unsigned char actual = sRGB(0.0);
    unsigned char expected = expected_srgb_double(0.0);
    if (actual != expected) {
        LOG_MSG("sRGB(0.0) = " << static_cast<int>(actual) << ", expected " << static_cast<int>(expected));
        return false;
    }
    return true;
}

// Test: sRGB(1.0) should map to 255
static bool test_srgb_one_input_matches_reference()
{
    unsigned char actual = sRGB(1.0);
    unsigned char expected = expected_srgb_double(1.0);
    if (actual != expected) {
        LOG_MSG("sRGB(1.0) = " << static_cast<int>(actual) << ", expected " << static_cast<int>(expected));
        return false;
    }
    return true;
}

// Test: monotonicity of sRGB with respect to linear input
static bool test_srgb_monotonic_increasing()
{
    const double samples[] = {0.0, 0.05, 0.15, 0.3, 0.6, 0.85, 1.0};
    const size_t N = sizeof(samples) / sizeof(samples[0]);
    unsigned char prev = sRGB(samples[0]);
    for (size_t i = 1; i < N; ++i) {
        unsigned char cur = sRGB(samples[i]);
        if (cur < prev) {
            LOG_MSG("sRGB monotonicity violated: sRGB(" << samples[i-1] << ")=" << int(prev)
                    << " > sRGB(" << samples[i] << ")=" << int(cur));
            return false;
        }
        prev = cur;
    }
    return true;
}

// Test: sRGB output aligns with standard gamma model for several representative values
static bool test_srgb_against_reference_values()
{
    struct TestCase { double input; unsigned char expected; };
    TestCase cases[] = {
        {0.125, expected_srgb_double(0.125)},
        {0.25,  expected_srgb_double(0.25)},
        {0.375, expected_srgb_double(0.375)},
        {0.75,  expected_srgb_double(0.75)},
        {0.9,   expected_srgb_double(0.9)},
    };
    const size_t N = sizeof(cases) / sizeof(cases[0]);
    bool all_ok = true;
    for (size_t i = 0; i < N; ++i) {
        unsigned char actual = sRGB(cases[i].input);
        unsigned char expected = cases[i].expected;
        if (actual != expected) {
            LOG_MSG("sRGB(" << cases[i].input << ") = " << int(actual)
                    << ", expected (std gamma) " << int(expected));
            all_ok = false;
        }
    }
    return all_ok;
}

int main(void)
{
    // Run tests
    std::cout << "Starting sRGB unit tests (no GTest, C++11)..." << std::endl;

    // Ensure we are testing with the actual focal method linked in
    // (Extern declarations above assume correct linking with libpng's sRGB implementations.)

    // Test 1: zero input
    test_srgb_zero_input_matches_reference();
    // Test 2: one input
    test_srgb_one_input_matches_reference();
    // Test 3: monotonic behavior
    if (!test_srgb_monotonic_increasing()) {
        ++g_tests_failed;
        LOG_MSG("Monotonicity test failed.");
    } else {
        // optional log success
        LOG_MSG("Monotonicity test passed.");
    }
    // Test 4: specific values against standard gamma reference
    if (!test_srgb_against_reference_values()) {
        ++g_tests_failed;
        LOG_MSG("Reference-value tests failed.");
    } else {
        LOG_MSG("Reference-value tests passed.");
    }

    // Summary
    std::cout << "sRGB tests run: " << g_tests_run << ", failures: " << g_tests_failed << std::endl;
    if (g_tests_failed == 0) {
        std::cout << "All sRGB unit tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Some sRGB unit tests FAILED." << std::endl;
        return 1;
    }
}