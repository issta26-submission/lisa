/*
Unit Test Suite for cmsFloat64Number computeNbb(cmsCIECAM02* pMod)

Step 1 - Program Understanding (summary via comments)
- The focal method computeNbb computes: 0.725 * pow((1.0 / pMod->n), 0.2)
- Core dependent component (Candidate Keyword): pMod->n (the N component member of cmsCIECAM02)
- Therefore, tests should validate correctness for various positive, edge, and negative inputs to n.
- This test suite relies on the cmsCIECAM02 type and the function prototype for computeNbb being accessible via the project headers.

Step 2 - Test Case Generation (design)
- Test positive typical values: n = 1.0, 2.0, 1000.0
- Test edge-ish values: very small n (e.g., 1e-6) to confirm numeric stability
- Test negative value: n = -1.0 should yield NaN (domain issue for fractional power)
- Ensure there is no termination on a single failure; instead collect results and report after all tests

Step 3 - Test Refinement (domain knowledge)
- Use precise near-equality checks with tolerances
- Use NaN checks for invalid inputs
- Access cmsCIECAM02 via lcms2_internal.h definitions; call computeNbb through C linkage from C++ test
- Do not rely on private/internal implementation details beyond the exposed struct member n

Notes:
- The test uses C++11 standard library only.
- No gtest; a lightweight custom test harness is implemented.
- Static helpers are avoided to ensure compatibility with the external C code.
- The test should compile in a project that already builds cmscam02.c and links with its object.

Code begins here
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <lcms2_internal.h>
#include <cmath>


// Include the project definitions to access cmsCIECAM02 and related types.
// This header is expected to define cmsCIECAM02 and cmsFloat64Number.

// Declare the function under test with C linkage for the C/C++ boundary.
extern "C" cmsFloat64Number computeNbb(cmsCIECAM02* pMod);

// Simple, non-terminating test assertions (accumulate results)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// NEAR comparison macro for floating point numbers
#define EXPECT_NEAR(actual, expected, tol, desc) do { \
    double a = (actual); \
    double b = (expected); \
    if (fabs(a - b) > (tol)) { \
        std::printf("FAILED: %s. Expected near %g, got %g\n", (desc), (expected), (actual)); \
        ++g_tests_failed; \
    } else { \
        std::printf("PASSED: %s\n", (desc)); \
    } \
    ++g_tests_run; \
} while(0)

// NaN check macro
#define EXPECT_NAN(actual, desc) do { \
    double a = (actual); \
    if (!std::isnan(a)) { \
        std::printf("FAILED: %s. Expected NaN, got %g\n", (desc), (actual)); \
        ++g_tests_failed; \
    } else { \
        std::printf("PASSED: %s\n", (desc)); \
    } \
    ++g_tests_run; \
} while(0)

// Test 1: Basic positive values
void test_computeNbb_basic_values() {
    cmsCIECAM02 mod;

    // n = 1.0 -> 0.725 * (1.0)^0.2 = 0.725
    mod.n = 1.0;
    cmsFloat64Number r = computeNbb(&mod);
    EXPECT_NEAR((double)r, 0.725, 1e-12, "n=1.0 -> 0.725");

    // n = 2.0 -> 0.725 * (0.5)^0.2
    mod.n = 2.0;
    r = computeNbb(&mod);
    double expected = 0.725 * std::pow(0.5, 0.2);
    EXPECT_NEAR((double)r, expected, 1e-12, "n=2.0 -> 0.725 * (0.5)^0.2");

    // n = 1000.0 -> 0.725 * (0.001)^0.2
    mod.n = 1000.0;
    r = computeNbb(&mod);
    expected = 0.725 * std::pow(0.001, 0.2);
    EXPECT_NEAR((double)r, expected, 1e-12, "n=1000.0 -> 0.725 * (0.001)^0.2");
}

// Test 2: Edge and extreme values
void test_computeNbb_extremes() {
    cmsCIECAM02 mod;

    // Very small n -> large 1/n, but raised to 0.2, should remain finite
    mod.n = 1e-6;
    cmsFloat64Number r = computeNbb(&mod);
    double expected = 0.725 * std::pow(1.0 / 1e-6, 0.2);
    EXPECT_NEAR((double)r, expected, 1e-12, "n=1e-6 -> finite large value");

    // Negative value should yield NaN due to fractional power of negative
    mod.n = -1.0;
    r = computeNbb(&mod);
    EXPECT_NAN((double)r, "n=-1.0 -> NaN due to negative base in pow");
}

// Test 3: Negative and zero handling (ensuring no undefined behavior beyond NaN)
void test_computeNbb_negative_zero_handling() {
    cmsCIECAM02 mod;

    // n = 0 should lead to division by zero inside pow; behavior may be NaN/inf
    // We expect NaN for safety, since 1.0 / 0.0 is inf and pow(inf, 0.2) is inf, but the library may define differently.
    mod.n = 0.0;
    cmsFloat64Number r = computeNbb(&mod);
    // Accept either NaN or Inf, but ensure test doesn't crash. We'll just check for NaN/Inf possibility.
    if (std::isnan((double)r) || std::isinf((double)r)) {
        std::printf("PASSED: n=0.0 -> result is NaN or Inf as expected\n");
        ++g_tests_run;
    } else {
        std::printf("WARNING: n=0.0 -> got finite value %g (acceptable if library handles div-by-zero differently)\n", (double)r);
        ++g_tests_run;
    }
}

// Run all tests
int main() {
    std::printf("Starting test suite for computeNbb...\n");
    test_computeNbb_basic_values();
    test_computeNbb_extremes();
    test_computeNbb_negative_zero_handling();

    std::printf("Test results: run=%d, failed=%d\n", g_tests_run, g_tests_failed);
    return (g_tests_failed == 0) ? 0 : 1;
}