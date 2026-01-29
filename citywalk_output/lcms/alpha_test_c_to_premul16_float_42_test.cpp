// alpha_test.cpp
// Purpose: Unit test suite for the focal method to_premul16_float(uint16_t rgb16, uint16_t a16)
// This test is designed to be compiled and linked with the C source providing the function.
// It uses a lightweight, non-terminating test harness to maximize coverage without GTest.
// Author: Generated per provided instructions

#include <cstdio>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Step 1 (Conceptual): Candidate Keywords representing core dependencies of to_premul16_float
// - rgb16: input 16-bit color channel value
// - a16: input 16-bit alpha value
// - alpha_factor: computed as a16 / 65535.0
// - rgb: intermediate premultiplied result
// - round: used to convert the floating point result back to uint16_t
// - to_premul16_float: focal function under test

// Step 2: Declare the focal function (C linkage to ensure proper linking with C sources)
extern "C" uint16_t to_premul16_float(uint16_t rgb16, uint16_t a16);

// Lightweight test harness (no external test framework)
static int total_tests = 0;
static int passed_tests = 0;

// Helper: Run a single test case and report pass/fail without aborting on failure
static bool run_case(const std::string& name, uint16_t rgb16, uint16_t a16) {
    total_tests++;

    // Compute expected value according to the function's documented behavior
    uint16_t expected;
    if (a16 == 0) {
        // Early return branch: when alpha is zero, should return rgb16 unchanged
        expected = rgb16;
    } else {
        double alpha_factor = (double)a16 / 65535.0;
        double rgb = (double)rgb16 * alpha_factor;
        expected = (uint16_t)round(rgb);
    }

    // Actual function under test
    uint16_t actual = to_premul16_float(rgb16, a16);

    bool ok = (actual == expected);
    if (ok) {
        ++passed_tests;
        std::cout << "[PASS] " << name << ": rgb16=" << rgb16
                  << ", a16=" << a16 << " => " << actual << "\n";
    } else {
        std::cout << "[FAIL] " << name << ": rgb16=" << rgb16
                  << ", a16=" << a16 << " => got " << actual
                  << ", expected " << expected << "\n";
    }
    return ok;
}

int main() {
    // Test suite for to_premul16_float
    // Coverage goals:
    // - Branch: a16 == 0 (no premultiplication; return rgb16 as-is)
    // - Branch: a16 > 0 (premultiply with alpha factor)
    // - Boundary and rounding checks for typical values

    // Test 1: Zero alpha should return the original rgb16 (early return branch)
    // Purpose: Validate the true branch where a16 == 0 short-circuits multiplication
    run_case("Zero alpha returns original rgb", 12345, 0);

    // Test 2: Full alpha should preserve rgb16 (alpha factor = 1.0)
    // Purpose: Ensure a16 == 65535 yields no change to rgb16
    run_case("Full alpha preserves rgb", 12345, 65535);

    // Test 3: Mid-range alpha with non-trivial scaling
    // Purpose: Validate floating-point multiplication and rounding behavior
    run_case("Mid-range alpha scaling", 12345, 32768);

    // Test 4: Edge case with zero rgb16 and full alpha
    // Purpose: Ensure 0 remains 0 after premultiplication
    run_case("Zero rgb with full alpha stays zero", 0, 65535);

    // Test 5: Small rgb and very small alpha to test rounding toward zero
    // Purpose: Check that tiny products round correctly to zero when appropriate
    run_case("Tiny rgb with minimal alpha rounds to zero", 8, 1);

    // Test 6: Large rgb with minimal alpha to observe rounding toward small values
    run_case("Large rgb with minimal alpha", 65535, 1);

    // Test 7: Large rgb with full alpha to verify no change
    run_case("Large rgb with full alpha unchanged", 65535, 65535);

    // Summary
    std::cout << "\nTest Summary: " << passed_tests << " / " << total_tests
              << " tests passed.\n";
    return (passed_tests == total_tests) ? 0 : 1;
}