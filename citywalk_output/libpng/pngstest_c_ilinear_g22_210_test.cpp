// Test suite for ilinear_g22(int fixed_srgb) from pngstest.c
// This test is implemented in C++11 without a third-party testing framework.
// It relies on the external C symbol ilinear_g22 provided by the project under test.
// The test suite focuses on basic correctness properties inferred from the focal method:
//
// - The function computes 65535 * g22_to_d[fixed_srgb], then passes the result to u16d (likely a
//   rounding/clamping converter to 0..65535). We cannot access g22_to_d or u16d directly from tests,
//   therefore we validate observable properties.
//
// Candidate Keywords from the focal method (for reference/traceability):
// - ilinear_g22
// - g22_to_d
// - u16d
// - 65535
// - fixed_srgb
//
// Test goals (based on domain knowledge and constraints):
// - Boundary checks: ilinear_g22(0) should map to 0, ilinear_g22(255) should map close to 65535.
// - Monotonicity: ilinear_g22 should be non-decreasing with increasing fixed_srgb.
// - Range checks: all results should lie within [0, 65535].
// - A sample mid-point should lie strictly between the boundary values (where applicable).
//
// Notes:
// - We use a lightweight, non-terminating assertion approach: tests accumulate failures and report at the end.
// - We do not rely on private/internal details beyond the external function contract.
// - To run, compile this file together with the project's C source that defines ilinear_g22
//   (e.g., using a linker against pngstest.o or the appropriate library).

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <ctype.h>


// Declaration of the focal function (from pngstest.c) with C linkage.
extern "C" int ilinear_g22(int fixed_srgb);

// Helper to print test status
static void print_status(const char* test_name, bool passed) {
    if (passed) {
        std::printf("[PASS] %s\n", test_name);
    } else {
        std::printf("[FAIL] %s\n", test_name);
    }
}

static bool test_boundaries() {
    // Boundary tests: expect 0 at 0 and 65535 at 255 (common mapping for 8-bit sRGB gamma->linear)
    const int v0 = ilinear_g22(0);
    const int v255 = ilinear_g22(255);

    bool pass = true;
    if (v0 != 0) {
        std::printf("  Boundary check: ilinear_g22(0) = %d (expected 0)\n", v0);
        pass = false;
    }
    if (v255 != 65535) {
        // Depending on the exact implementation of u16d, there's a strong expectation that 255 maps to max.
        // If the implementation slightly differs due to rounding, this assertion might fail.
        // We still report the discrepancy clearly.
        std::printf("  Boundary check: ilinear_g22(255) = %d (expected 65535)\n", v255);
        pass = false;
    }
    return pass;
}

static bool test_monotonicity() {
    // Ensure non-decreasing values as fixed_srgb increases
    int prev = ilinear_g22(0);
    for (int i = 1; i <= 255; ++i) {
        int cur = ilinear_g22(i);
        if (cur < prev) {
            std::printf("  Monotonicity violation at i=%d: %d -> %d\n", i-1, prev, cur);
            return false;
        }
        prev = cur;
    }
    return true;
}

static bool test_value_ranges() {
    // All results should lie within [0, 65535]
    for (int i = 0; i <= 255; ++i) {
        int v = ilinear_g22(i);
        if (v < 0 || v > 65535) {
            std::printf("  Range violation at i=%d: %d\n", i, v);
            return false;
        }
    }
    return true;
}

static bool test_midpoint_increasing() {
    // Check that a mid-range input yields a value between their neighbors, e.g., 127, 128, 129
    int v127 = ilinear_g22(127);
    int v128 = ilinear_g22(128);
    int v129 = ilinear_g22(129);
    // Ensure strict-ish monotonic behavior around the midpoint (not guaranteed to be strictly increasing due to rounding)
    if (!(v127 <= v128 && v128 <= v129)) {
        std::printf("  Midpoint test failed: v127=%d, v128=%d, v129=%d\n", v127, v128, v129);
        return false;
    }
    return true;
}

int main() {
    // Simple test harness that runs all tests and reports summary.
    int total = 0;
    int failures = 0;

    auto run = [&](const char* name, bool (*test)()) {
        total++;
        bool ok = test();
        print_status(name, ok);
        if (!ok) ++failures;
    };

    run("iliner_g22 boundary values (0 and 255)", test_boundaries);
    run("iliner_g22 monotonicity across 0..255", test_monotonicity);
    run("iliner_g22 value range within [0, 65535]", test_value_ranges);
    run("iliner_g22 mid-point monotonicity (127-129)", test_midpoint_increasing);

    if (failures == 0) {
        std::printf("All tests PASSED (%d assertions).\n", total);
        return EXIT_SUCCESS;
    } else {
        std::printf("Tests completed with %d failure(s) out of %d tests.\n", failures, total);
        return EXIT_FAILURE;
    }
}