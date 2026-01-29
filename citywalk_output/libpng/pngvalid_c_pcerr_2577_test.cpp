// Unit tests for the focal method pcerr in pngvalid.c
// Language: C++11
// Notes:
// - We assume the real project provides a C function:
//     double pcerr(const png_modifier *pm, int in_depth, int out_depth);
// - For testing in C++, we provide a minimal compatible struct layout to match
//   the fields accessed by pcerr. The actual binary will be linked at build time.
// - This test suite focuses on true/false branches of the condition inside pcerr.
// - No GTest is used; simple non-terminating assertions are implemented to
//   maximize test coverage without aborting on failure.

#include <signal.h>
#include <string.h>
#include <iomanip>
#include <float.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>


// Provide a compatible forward declaration for the focal C function.
// The real project should provide the proper declaration via its headers.
extern "C" double pcerr(const struct png_modifier* pm, int in_depth, int out_depth);

// Minimal compatible definition of png_modifier to access required fields.
// The actual libpng/pngvalid may define this differently; we rely on the
// fields used by pcerr as shown in the focal method.
struct png_modifier {
    int assume_16_bit_calculations;      // when non-zero, force 16-bit behavior
    int calculations_use_input_precision; // if non-zero, use in_depth for 16-bit check
    int maxpc16;                          // max percentage for 16-bit path (as integer)
    int maxpc8;                           // max percentage for 8-bit path (as integer)
};

// Simple near-equality assertion for doubles
static inline bool nearly_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Global test state to enable non-terminating assertions (continue on failures)
static int g_failures = 0;

// Lightweight assertion macro that reports failures but does not terminate tests
#define ASSERT_NEAR(expected, actual, tol, msg)                                  \
    do {                                                                           \
        if (!nearly_equal((expected), (actual), (tol))) {                          \
            std::cerr << std::setw(60) << std::left                   \
                      << "TEST FAIL: " << (msg) << " | "                    \
                      << "Expected: " << (expected)                        \
                      << "  Actual: " << (actual) << std::endl;            \
            ++g_failures;                                                        \
        } else {                                                                 \
            std::cout << "TEST PASS: " << (msg) << std::endl;                   \
        }                                                                        \
    } while (0)

// Helper to run a single test and print a banner
static void run_test_banner(const std::string &name) {
    std::cout << "\n=== Running " << name << " ===" << std::endl;
}

// Test 1: assume_16_bit_calculations is true -> always use maxpc16
static void test_case_assume16_always_uses_maxpc16() {
    run_test_banner("test_case_assume16_always_uses_maxpc16");
    png_modifier pm = {};
    pm.assume_16_bit_calculations = 1;     // Force 16-bit path
    pm.calculations_use_input_precision = 0;
    pm.maxpc16 = 200;                        // 200 * 0.01 = 2.0
    pm.maxpc8 = 10;                          // won't be used

    double result = pcerr(&pm, 8, 8);        // depths do not matter due to flag
    double expected = 2.0;
    ASSERT_NEAR(expected, result, 1e-9, "Case1: 16-bit path forced by assume_16_bit_calculations");
}

// Test 2: calculations_use_input_precision = true, in_depth == 16
// Expect: use maxpc16 * 0.01
static void test_case_use_input_precision_in_depth_16() {
    run_test_banner("test_case_use_input_precision_in_depth_16");
    png_modifier pm = {};
    pm.assume_16_bit_calculations = 0;
    pm.calculations_use_input_precision = 1; // Use in_depth for 16-bit check
    pm.maxpc16 = 150;                          // 150 * 0.01 = 1.5
    pm.maxpc8 = 5;

    double result = pcerr(&pm, 16, 24);        // in_depth is 16
    double expected = 1.5;
    ASSERT_NEAR(expected, result, 1e-9, "Case2: in_depth == 16 uses maxpc16");
}

// Test 3: calculations_use_input_precision = true, in_depth != 16
// Expect: use maxpc8 * 0.01
static void test_case_use_input_precision_in_depth_not_16() {
    run_test_banner("test_case_use_input_precision_in_depth_not_16");
    png_modifier pm = {};
    pm.assume_16_bit_calculations = 0;
    pm.calculations_use_input_precision = 1; // Use in_depth for 16-bit check
    pm.maxpc16 = 180;                          // would be 1.8 if used
    pm.maxpc8 = 42;                            // 0.42 if used

    double result = pcerr(&pm, 12, 16);         // in_depth != 16, but out_depth is 16; here in_depth=12
    // Since in_depth != 16, the condition is false; should take maxpc8
    double expected = 0.42;
    ASSERT_NEAR(expected, result, 1e-9, "Case3: in_depth != 16 uses maxpc8");
}

// Test 4: calculations_use_input_precision = false, out_depth == 16
// Expect: use maxpc16 * 0.01
static void test_case_out_depth_16_uses_maxpc16_when_calcs_use_input_false() {
    run_test_banner("test_case_out_depth_16_uses_maxpc16_when_calcs_use_input_false");
    png_modifier pm = {};
    pm.assume_16_bit_calculations = 0;
    pm.calculations_use_input_precision = 0; // Use out_depth for 16-bit check
    pm.maxpc16 = 123;                          // 1.23
    pm.maxpc8 = 9;                             // not used

    double result = pcerr(&pm, 8, 16);          // out_depth == 16
    double expected = 1.23;
    ASSERT_NEAR(expected, result, 1e-9, "Case4: out_depth == 16 uses maxpc16");
}

// Test 5: calculations_use_input_precision = false, out_depth != 16
// Expect: use maxpc8 * 0.01
static void test_case_out_depth_not_16_uses_maxpc8_when_calcs_use_input_false() {
    run_test_banner("test_case_out_depth_not_16_uses_maxpc8_when_calcs_use_input_false");
    png_modifier pm = {};
    pm.assume_16_bit_calculations = 0;
    pm.calculations_use_input_precision = 0; // Use out_depth for 16-bit check
    pm.maxpc16 = 77;                             // 0.77 if used
    pm.maxpc8 = 33;                              // 0.33

    double result = pcerr(&pm, 8, 8);             // out_depth is 8 (not 16)
    double expected = 0.33;
    ASSERT_NEAR(expected, result, 1e-9, "Case5: out_depth != 16 uses maxpc8");
}

int main() {
    // Step 3 (Test Case Refinement): Execute all test cases
    test_case_assume16_always_uses_maxpc16();
    test_case_use_input_precision_in_depth_16();
    test_case_use_input_precision_in_depth_not_16();
    test_case_out_depth_16_uses_maxpc16_when_calcs_use_input_false();
    test_case_out_depth_not_16_uses_maxpc8_when_calcs_use_input_false();

    // Summary
    if (g_failures == 0) {
        std::cout << "\nALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "\nTESTS FAILED: " << g_failures << " failure(s)" << std::endl;
        return 1;
    }
}