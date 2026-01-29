// Minimal C++11 unit test harness for Clamp_L_doubleV2 (no GTest; uses plain assertions with output)

// We describe and test the focal function Clamp_L_doubleV2 which clamps a double L
// to the range [0, L_max], where L_max = ((0xFFFF * 100.0) / 0xFF00).

#include <lcms2_internal.h>
#include <cstdio>
#include <cmath>


// Declaration of the focal function from the C source file(cmspcs.c).
// cmsFloat64Number is a typedef for double in the original project; using double here is correct for testing.
extern "C" double Clamp_L_doubleV2(double L);

// Helper: compute the L_max value exactly as in the focal implementation.
// This mirrors: const cmsFloat64Number L_max = (cmsFloat64Number) (0xFFFF * 100.0) / 0xFF00;
static double compute_L_max() {
    return (static_cast<double>(0xFFFF) * 100.0) / static_cast<double>(0xFF00);
}

// Tolerance used for floating point comparisons.
static const double TOL = 1e-12;

// Test 1: Negative input should clamp to 0
// This covers the "if (L < 0) L = 0;" branch.
static bool test_negative_is_clamped_to_zero() {
    double L = -1.0;
    double res = Clamp_L_doubleV2(L);
    double expected = 0.0;
    if (std::fabs(res - expected) <= TOL) {
        printf("PASS: test_negative_is_clamped_to_zero\n");
        return true;
    } else {
        printf("FAIL: test_negative_is_clamped_to_zero - expected %.12f, got %.12f\n", expected, res);
        return false;
    }
}

// Test 2: In-range value should be returned unchanged
// This covers the "if (L > L_max) L = L_max;" branch not taken.
static bool test_within_bounds_returns_same_value() {
    const double L = 42.5; // a representative value well within [0, L_max]
    const double res = Clamp_L_doubleV2(L);
    const double expected = L;
    if (std::fabs(res - expected) <= TOL) {
        printf("PASS: test_within_bounds_returns_same_value\n");
        return true;
    } else {
        printf("FAIL: test_within_bounds_returns_same_value - expected %.12f, got %.12f\n", expected, res);
        return false;
    }
}

// Test 3: Value exactly at L_max should be returned unchanged
static bool test_at_max_returns_L_max() {
    const double L_max = compute_L_max();
    const double res = Clamp_L_doubleV2(L_max);
    if (std::fabs(res - L_max) <= TOL) {
        printf("PASS: test_at_max_returns_L_max\n");
        return true;
    } else {
        printf("FAIL: test_at_max_returns_L_max - expected %.12f, got %.12f\n", L_max, res);
        return false;
    }
}

// Test 4: Value above L_max should be clamped to L_max
static bool test_above_max_clamped_to_L_max() {
    const double L_max = compute_L_max();
    const double L = L_max * 2.0;
    const double res = Clamp_L_doubleV2(L);
    if (std::fabs(res - L_max) <= TOL) {
        printf("PASS: test_above_max_clamped_to_L_max\n");
        return true;
    } else {
        printf("FAIL: test_above_max_clamped_to_L_max - expected %.12f, got %.12f\n", L_max, res);
        return false;
    }
}

// Test 5: Value just below L_max should be returned unchanged
static bool test_just_below_L_max_returns_same() {
    const double L_max = compute_L_max();
    const double L = L_max - 0.001; // clearly below max
    const double res = Clamp_L_doubleV2(L);
    if (std::fabs(res - L) <= TOL) {
        printf("PASS: test_just_below_L_max_returns_same\n");
        return true;
    } else {
        printf("FAIL: test_just_below_L_max_returns_same - expected %.12f, got %.12f\n", L, res);
        return false;
    }
}

// Main runner
int main() {
    bool all_passed = true;

    all_passed &= test_negative_is_clamped_to_zero();
    all_passed &= test_within_bounds_returns_same_value();
    all_passed &= test_at_max_returns_L_max();
    all_passed &= test_above_max_clamped_to_L_max();
    all_passed &= test_just_below_L_max_returns_same();

    if (all_passed) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED\n");
        return 1;
    }
}