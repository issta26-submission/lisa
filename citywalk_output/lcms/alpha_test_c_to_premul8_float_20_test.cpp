/*
Step 1: Program Understanding (Summary in comments)
- Target focal function: to_premul8_float(uint8_t rgb8, uint8_t a8)
- Behavior:
  - If a8 == 0, return rgb8 unchanged.
  - Otherwise, compute alpha_factor = a8 / 255.0
    rgb = rgb8 * alpha_factor
    return uint8_t(round(rgb))
- Core dependent components (Candidate Keywords): rgb8, a8, alpha_factor, rgb, round
- This test suite is designed to validate the focal function behavior from C linkage in a C++11 test file.
- We exercise both the true/false branches and boundary conditions (a8 == 0, a8 == 255, various rgb8 values).

Step 2/3: Unit Test Generation and Refinement
- Implemented as a lightweight, non-terminating test harness (no GTest) in C++11.
- Tests:
  1) a8 == 0 returns rgb8 (covers early return).
  2) Non-zero a8 values produce rgb8 * (a8/255.0) rounded according to standard rounding rules.
  3) Boundary values such as a8 == 255 produce rgb8 unchanged (consistency check).
  4) A representative mix of values to ensure coverage across the input domain.
- All tests are designed to continue on failure and report detailed outputs, suitable for integration with a larger non-GTest C++11 project.

Code (C++11 test suite for to_premul8_float)
*/
#include <cstdio>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <cmath>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Declaration of the focal C function with C linkage.
// It is assumed to be defined in the project (e.g., alpha_test.c) and linked during build.
extern "C" uint8_t to_premul8_float(uint8_t rgb8, uint8_t a8);

/*
 Lightweight test harness:
 - Runs a series of test cases for to_premul8_float.
 - Uses non-terminating assertions: reports failures but continues running.
 - Outputs per-test results and a final summary.
*/

// Global counters for test results
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Helper to report test results
static void report_result(const char* test_name, bool passed) {
    ++g_tests_run;
    if (passed) {
        printf("PASSED: %s\n", test_name);
    } else {
        ++g_tests_failed;
        fprintf(stderr, "FAILED: %s\n", test_name);
    }
}

// Test 1: When a8 == 0, the function should return rgb8 unchanged (early exit path)
static void test_to_premul8_float_a8_zero_returns_input(void) {
    const uint8_t test_values[] = {0, 1, 50, 100, 199, 255};
    const size_t n = sizeof(test_values)/sizeof(test_values[0]);

    for (size_t i = 0; i < n; ++i) {
        uint8_t rgb8 = test_values[i];
        uint8_t a8 = 0;
        uint8_t out = to_premul8_float(rgb8, a8);
        char test_name[64];
        snprintf(test_name, sizeof(test_name), "a8_zero_returns_input_rgb8_%u", (unsigned)rgb8);
        report_result(test_name, out == rgb8);
    }
}

// Test 2: Non-zero a8 values should produce rgb8 * (a8/255.0) rounded (matching the focal logic)
static void test_to_premul8_float_nonzero_matches_formula(void) {
    // A representative set of cases to cover typical and edge values
    struct Case { uint8_t rgb8; uint8_t a8; };
    const Case cases[] = {
        {0, 1},       // 0 * (1/255) -> 0
        {255, 1},     // 255 * (1/255) -> ~1, rounds to 1
        {255, 128},   // 255 * 128/255 -> 128
        {123, 77},    // 123 * 77/255 -> ~37.23 -> 37
        {128, 128},   // 128 * 128/255 -> ~64.0 -> 64
        {10, 1},      // 10 * 1/255 -> ~0.039 -> 0
        {10, 255},    // 10 * 255/255 -> 10
        {0, 255},     // 0 * 1.0 -> 0
        {199, 5},     // 199 * 5/255 -> ~3.90 -> 4
        {50, 200}      // 50 * 200/255 -> ~39.22 -> 39
    };
    const size_t n = sizeof(cases)/sizeof(cases[0]);

    for (size_t i = 0; i < n; ++i) {
        uint8_t rgb8 = cases[i].rgb8;
        uint8_t a8   = cases[i].a8;

        // Expected based on the same formula as the focal implementation:
        // expected = round((rgb8) * (a8 / 255.0))
        double alpha_factor = (double)a8 / 255.0;
        double rgb = ((double)rgb8) * alpha_factor;
        uint8_t expected = (uint8_t) std::round(rgb);
        uint8_t actual = to_premul8_float(rgb8, a8);

        char test_name[96];
        snprintf(test_name, sizeof(test_name),
                 "nonzero_matches_formula_rgb8_%u_a8_%u_expected_%u_actual_%u",
                 (unsigned)rgb8, (unsigned)a8, (unsigned)expected, (unsigned)actual);
        report_result(test_name, actual == expected);
    }
}

// Test 3: Boundary consistency: a8 == 255 should yield rgb8 (full alpha)
static void test_to_premul8_float_boundary_alpha255(void) {
    const uint8_t test_values[] = {0, 1, 50, 100, 199, 255};
    const size_t n = sizeof(test_values)/sizeof(test_values[0]);

    for (size_t i = 0; i < n; ++i) {
        uint8_t rgb8 = test_values[i];
        uint8_t a8 = 255;
        uint8_t out = to_premul8_float(rgb8, a8);
        char test_name[64];
        snprintf(test_name, sizeof(test_name), "alpha255_consistency_rgb8_%u", (unsigned)rgb8);
        // With a8 == 255, rgb should remain unchanged after rounding if the math yields rgb8*1.0
        // i.e., the function should produce rgb8
        report_result(test_name, out == rgb8);
    }
}

// Runner to execute all tests
static void run_all_tests(void) {
    printf("Starting tests for to_premul8_float...\n");
    test_to_premul8_float_a8_zero_returns_input();
    test_to_premul8_float_nonzero_matches_formula();
    test_to_premul8_float_boundary_alpha255();
    printf("Finished tests. Ran %d tests with %d failures.\n",
           g_tests_run, g_tests_failed);
}

// Main entry: execute tests
int main() {
    // Run all tests and return non-zero if any test failed
    run_all_tests();
    return g_tests_failed;
}