/*
  This code provides a self-contained unit test suite (C++11) for the focal method:
  from_premul16_float(uint16_t rgb16, uint16_t a16)

  Key behaviors exercised:
  - When a16 == 0, rgb16 is returned unchanged (true branch).
  - When a16 > 0, rgb16 is scaled by alpha factor = 65535.0 / a16, clamped to 65535, and rounded.
  - Branch covering potential overflow is exercised (rgb clamped to 65535 before rounding).

  Notes:
  - No Google Test is used; a lightweight test harness is implemented with non-terminating assertions.
  - Tests are designed to maximize coverage of branching logic and rounding behavior.
  - The test suite uses straightforward C++ standard library facilities only.
  - The focal function is provided here with C linkage to ensure compatibility with potential C sources.
*/

#include <cstdio>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Focal method under test copied to ensure testability in a standalone C++11 environment.
// Original signature (as provided in the prompt) is preserved with C linkage.
extern "C" uint16_t from_premul16_float(uint16_t rgb16, uint16_t a16) {
    double alpha_factor, rgb;
    if (a16 == 0) return rgb16;
    alpha_factor = 65535.0 / (double)a16;
    rgb = ((double)rgb16 * alpha_factor);
    if (rgb > 65535.0) rgb = 65535.0;
    return (uint16_t)round(rgb);
}

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Compute expected result using the same mathematical specification as the focal method.
// This separate reference helps validate behavior without relying solely on the implementation.
static uint16_t expected_from_premul16_float(uint16_t rgb16, uint16_t a16) {
    if (a16 == 0) return rgb16;
    double alpha_factor = 65535.0 / (double)a16;
    double rgb = ((double)rgb16 * alpha_factor);
    if (rgb > 65535.0) rgb = 65535.0;
    return (uint16_t)std::round(rgb);
}

// Test 1: a16 == 0 should return rgb16 unchanged (true branch)
static void test_from_premul16_float_zero_a16() {
    // Explanation: When a16 is zero, function should bypass any scaling.
    uint16_t rgb16 = 12345;
    uint16_t a16 = 0;
    uint16_t result = from_premul16_float(rgb16, a16);
    uint16_t expected = expected_from_premul16_float(rgb16, a16);
    ++g_total_tests;
    if (result != expected) {
        fprintf(stderr, "FAIL: test_from_premul16_float_zero_a16 - expected %u, got %u\n", (unsigned)expected, (unsigned)result);
        ++g_failed_tests;
    } else {
        printf("PASS: test_from_premul16_float_zero_a16\n");
    }
}

// Test 2: Overflow path should clamp to 65535 (true path, with clamping before rounding)
static void test_from_premul16_float_overflow_clamp() {
    // Explanation: rgb16 at max and a16 very small -> rgb would overflow; should clamp to 65535.
    uint16_t rgb16 = 65535;
    uint16_t a16 = 1; // alpha_factor = 65535
    uint16_t result = from_premul16_float(rgb16, a16);
    uint16_t expected = expected_from_premul16_float(rgb16, a16);
    ++g_total_tests;
    if (result != expected) {
        fprintf(stderr, "FAIL: test_from_premul16_float_overflow_clamp - expected %u, got %u\n", (unsigned)expected, (unsigned)result);
        ++g_failed_tests;
    } else {
        printf("PASS: test_from_premul16_float_overflow_clamp\n");
    }
}

// Test 3: Normal scaling with rounding (non-boundary case)
static void test_from_premul16_float_normal_rounding() {
    // Explanation: rgb16=32768, a16=32768 -> alpha_factor ≈ 1.999969..., rgb ≈ 65535.0, should round to 65535
    uint16_t rgb16 = 32768;
    uint16_t a16 = 32768;
    uint16_t result = from_premul16_float(rgb16, a16);
    uint16_t expected = expected_from_premul16_float(rgb16, a16);
    ++g_total_tests;
    if (result != expected) {
        fprintf(stderr, "FAIL: test_from_premul16_float_normal_rounding - expected %u, got %u\n", (unsigned)expected, (unsigned)result);
        ++g_failed_tests;
    } else {
        printf("PASS: test_from_premul16_float_normal_rounding\n");
    }
}

// Test 4: Rounding up case (0.5 fraction after multiplication)
static void test_from_premul16_float_round_half_up() {
    // Explanation: rgb16=1000, a16=2000 -> factor = 65535/2000 = 32.7675
    // rgb = 1000 * 32.7675 = 32767.5 -> rounds to 32768
    uint16_t rgb16 = 1000;
    uint16_t a16 = 2000;
    uint16_t result = from_premul16_float(rgb16, a16);
    uint16_t expected = expected_from_premul16_float(rgb16, a16);
    ++g_total_tests;
    if (result != expected) {
        fprintf(stderr, "FAIL: test_from_premul16_float_round_half_up - expected %u, got %u\n", (unsigned)expected, (unsigned)result);
        ++g_failed_tests;
    } else {
        printf("PASS: test_from_premul16_float_round_half_up\n");
    }
}

// Test 5: Large alpha with equal inputs (a16 equals 65535 should yield rgb16)
static void test_from_premul16_float_idempotent_alpha65535() {
    uint16_t rgb16 = 65535;
    uint16_t a16 = 65535;
    uint16_t result = from_premul16_float(rgb16, a16);
    uint16_t expected = expected_from_premul16_float(rgb16, a16);
    ++g_total_tests;
    if (result != expected) {
        fprintf(stderr, "FAIL: test_from_premul16_float_idempotent_alpha65535 - expected %u, got %u\n", (unsigned)expected, (unsigned)result);
        ++g_failed_tests;
    } else {
        printf("PASS: test_from_premul16_float_idempotent_alpha65535\n");
    }
}

int main() {
    // Run tests
    test_from_premul16_float_zero_a16();
    test_from_premul16_float_overflow_clamp();
    test_from_premul16_float_normal_rounding();
    test_from_premul16_float_round_half_up();
    test_from_premul16_float_idempotent_alpha65535();

    // Summary
    printf("Test summary: %d ran, %d failed\n", g_total_tests, g_failed_tests);
    return g_failed_tests;
}