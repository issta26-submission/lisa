// Test suite for cmsFloat64Number compute_z(cmsCIECAM02* pMod)
// This test is designed to compile with a C/C++11 toolchain and without GTest.
// It links against the cmscam02.c (LittleCMS) implementation.
// The tests exercise the behavior of compute_z which calculates:
//     z = 1.48 + sqrt(n)
// where n is a member of cmsCIECAM02.
// Notes:
// - We include the internal header to get the exact struct layout for cmsCIECAM02.
// - We test a variety of n values including zero, positive, and negative (NaN case).
// - All tests are non-terminating assertions and print results to stdout.
// - Static helpers and a small test harness are implemented in plain C++.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cmath>


// Import necessary internal definitions for types used by cmscam02.c

// Declare the focal function from the C source.
// The function uses C linkage; ensure correct linkage for the test harness.
extern "C" cmsFloat64Number compute_z(cmsCIECAM02* pMod);

// Small floating-point comparison helper
static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Test 1: z when n = 0 -> z = 1.48
// Rationale: verifies the lower boundary behavior and correct addition.
static bool test_compute_z_zero_n() {
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(mod)); // Zero out to ensure deterministic state
    mod.n = 0.0;                       // Core dependent value
    cmsFloat64Number res = compute_z(&mod);

    double expected = 1.48;            // 1.48 + sqrt(0) = 1.48
    if (!almost_equal((double)res, expected)) {
        std::printf("FAILED test_compute_z_zero_n: expected %.10f, got %.10f\n",
                    expected, (double)res);
        return false;
    }
    std::printf("PASSED test_compute_z_zero_n\n");
    return true;
}

// Test 2: z for small positive n
// Rationale: ensures correct sqrt calculation for small values.
static bool test_compute_z_small_positive_n() {
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(mod));
    mod.n = 1.0; // sqrt(1) = 1
    cmsFloat64Number res = compute_z(&mod);

    double expected = 1.48 + 1.0; // 2.48
    if (!almost_equal((double)res, expected)) {
        std::printf("FAILED test_compute_z_small_positive_n: expected %.10f, got %.10f\n",
                    expected, (double)res);
        return false;
    }
    std::printf("PASSED test_compute_z_small_positive_n\n");
    return true;
}

// Test 3: z for n = 4 (sqrt = 2)
static bool test_compute_z_n_four() {
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(mod));
    mod.n = 4.0;
    cmsFloat64Number res = compute_z(&mod);

    double expected = 1.48 + 2.0; // 3.48
    if (!almost_equal((double)res, expected)) {
        std::printf("FAILED test_compute_z_n_four: expected %.10f, got %.10f\n",
                    expected, (double)res);
        return false;
    }
    std::printf("PASSED test_compute_z_n_four\n");
    return true;
}

// Test 4: z for larger n = 9 (sqrt = 3)
static bool test_compute_z_n_nine() {
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(mod));
    mod.n = 9.0;
    cmsFloat64Number res = compute_z(&mod);

    double expected = 1.48 + 3.0; // 4.48
    if (!almost_equal((double)res, expected)) {
        std::printf("FAILED test_compute_z_nine: expected %.10f, got %.10f\n",
                    expected, (double)res);
        return false;
    }
    std::printf("PASSED test_compute_z_nine\n");
    return true;
}

// Test 5: n negative -> expect NaN (domain error for sqrt of negative)
// Rationale: pow with a negative base and exponent 0.5 should yield NaN.
static bool test_compute_z_negative_n_yields_nan() {
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(mod));
    mod.n = -1.0;
    cmsFloat64Number res = compute_z(&mod);

    if (!std::isnan((double)res)) {
        std::printf("FAILED test_compute_z_negative_n_yields_nan: expected NaN, got %.10f\n",
                    (double)res);
        return false;
    }
    std::printf("PASSED test_compute_z_negative_n_yields_nan\n");
    return true;
}

// Main test harness
int main() {
    // Candidate Keywords observed from the focal method:
    // - pMod, n, pow, cmsFloat64Number, cmsCIECAM02
    // These tests validate the core dependency n and the calculation using sqrt.
    int total = 0;
    int passed = 0;

    total++; if (test_compute_z_zero_n()) passed++; else {
        std::printf("Test 1 failed: test_compute_z_zero_n\n"); }

    total++; if (test_compute_z_small_positive_n()) passed++; else {
        std::printf("Test 2 failed: test_compute_z_small_positive_n\n"); }

    total++; if (test_compute_z_n_four()) passed++; else {
        std::printf("Test 3 failed: test_compute_z_n_four\n"); }

    total++; if (test_compute_z_n_nine()) passed++; else {
        std::printf("Test 4 failed: test_compute_z_nine\n"); }

    total++; if (test_compute_z_negative_n_yields_nan()) passed++; else {
        std::printf("Test 5 failed: test_compute_z_negative_n_yields_nan\n"); }

    std::printf("Tests completed: %d/%d passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}