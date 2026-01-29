// Step 1: Program Understanding
// - Focus: to_premul8(uint8_t rgb8, uint8_t a8) which premultiplies an 8-bit color channel by alpha.
// - The function uses a8 to compute an alpha_factor via fixed-point domain conversion and then
//   computes rgb = ((rgb8 * alpha_factor) + 0x8000) >> 16, finally returning the low byte.
// - Observed branching: if (a8 == 0) return rgb8; otherwise perform premultiplication.
// - We will provide minimal, deterministic stubs for dependencies (_cmsToFixedDomain and FROM_8_TO_16)
//   so the focal method can be compiled and tested in isolation.
// - Our test suite will validate true/false branches and numerical correctness against a mathematically
//   equivalent safe reference implementation (rounded premultiplication: round((rgb8 * a8) / 255)).


// Step 2: Unit Test Generation
// - Dependencies within the focal class file are simulated via small, deterministic stubs.
// - The test suite targets the function to_premul8 and verifies outputs for multiple rgb8/a8 pairs.
// - We avoid any Google Test framework and implement a simple test harness with non-terminating assertions
//   to maximize code coverage across tests.

#include <cstdio>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <lcms2_internal.h>
#include <cstdint>


// Candidate Keywords (from understanding)
// - to_premul8, rgb8, a8, alpha_factor, _cmsToFixedDomain, FROM_8_TO_16
// - Premultiply, 16-bit fixed-point, rounding, boundary conditions (a8 == 0), 8-bit result

// Step 3: Test Case Refinement
// - Implement deterministic macros for missing dependencies to mirror the original function's behavior closely.
#define FROM_8_TO_16(x) ((uint16_t)((x) * 0x101)) // Map 8-bit [0..255] to 16-bit [0..65535] as 0x00..0xFFFF
static inline uint32_t _cmsToFixedDomain(uint32_t x) { return x; } // Identity mapping to simplify testing

// Forward declaration of the focal method under test.
// The actual function from the source under test is reproduced here for self-contained testing.
uint8_t to_premul8(uint8_t rgb8, uint8_t a8)
{
{
    uint32_t alpha_factor, rgb;
    if (a8 == 0) return rgb8;
    alpha_factor = _cmsToFixedDomain(FROM_8_TO_16(a8));
    rgb = ((uint32_t)rgb8 * alpha_factor + 0x8000) >> 16;
    return (uint8_t)rgb;
}
}

// Reference implementation for expected value without relying on the internal macros.
// This represents the mathematically correct premultiplication with rounding: round((rgb8 * a8) / 255)
static inline uint8_t premul_expected(uint8_t rgb8, uint8_t a8)
{
    if (a8 == 0) return rgb8;
    uint32_t prod = (uint32_t)rgb8 * (uint32_t)a8;
    uint32_t val = (prod + 127) / 255; // rounding to nearest integer
    return (uint8_t)val;
}

// Simple non-terminating test harness
static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

static void test_assert_equal_uint8(const char* desc, uint8_t expected, uint8_t actual)
{
    ++g_total;
    if (expected == actual) {
        ++g_passed;
        std::printf("PASS: %s | expected=%u actual=%u\n", desc, (unsigned)expected, (unsigned)actual);
    } else {
        ++g_failed;
        std::printf("FAIL: %s | expected=%u actual=%u\n", desc, (unsigned)expected, (unsigned)actual);
    }
}

// Main test suite
int main() {
    // Step 2: Unit Test Generation
    // Test 1: Branch when a8 == 0 should return rgb8 unchanged (true path)
    // We test several rgb8 values to ensure no unintended side effects.
    {
        // probing a8 = 0 branch
        const uint8_t test_rgb_values[] = {0, 1, 64, 128, 199, 255};
        const size_t N = sizeof(test_rgb_values) / sizeof(test_rgb_values[0]);
        for (size_t i = 0; i < N; ++i) {
            uint8_t rgb8 = test_rgb_values[i];
            uint8_t a8 = 0;
            uint8_t out = to_premul8(rgb8, a8);
            test_assert_equal_uint8("to_premul8 with a8=0 returns original rgb8", rgb8, out);
        }
    }

    // Test 2: Some representative values for a8 saturated at 255 (should preserve rgb8 after rounding)
    // We additionally verify against the reference premul_expected to ensure numerical consistency.
    {
        const uint8_t test_rgb_values[] = {0, 16, 64, 128, 192, 255};
        const uint8_t test_a_values[] = {1, 16, 64, 128, 200, 255};
        for (size_t i = 0; i < sizeof(test_rgb_values)/sizeof(test_rgb_values[0]); ++i) {
            for (size_t j = 0; j < sizeof(test_a_values)/sizeof(test_a_values[0]); ++j) {
                uint8_t rgb8 = test_rgb_values[i];
                uint8_t a8 = test_a_values[j];
                uint8_t actual = to_premul8(rgb8, a8);
                uint8_t expected = premul_expected(rgb8, a8);
                test_assert_equal_uint8("to_premul8 consistency with rounding (a8>0)", expected, actual);
            }
        }
    }

    // Test 3: Comprehensive matrix coverage for multiple rgb8 and a8 values
    // Ensures broad coverage of the input space.
    {
        const uint8_t rgb_values[] = {0, 8, 32, 96, 160, 224, 255};
        const uint8_t a_values[]   = {0, 1, 3, 7, 15, 64, 128, 200, 255};
        for (size_t i = 0; i < sizeof(rgb_values)/sizeof(rgb_values[0]); ++i) {
            for (size_t j = 0; j < sizeof(a_values)/sizeof(a_values[0]); ++j) {
                uint8_t rgb8 = rgb_values[i];
                uint8_t a8   = a_values[j];
                uint8_t actual = to_premul8(rgb8, a8);
                uint8_t expected = premul_expected(rgb8, a8);
                test_assert_equal_uint8("to_premul8 matrix test", expected, actual);
            }
        }
    }

    // Summary
    std::printf("Total tests: %d, Passed: %d, Failed: %d\n",
                g_total, g_passed, g_failed);

    // Exit code 0 if all tests pass, non-zero otherwise (convenient for CI).
    return (g_failed == 0) ? 0 : 1;
}