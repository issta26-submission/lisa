// cms_test_computeLBFD.cpp
// Lightweight unit tests for cmsFloat64Number ComputeLBFD(const cmsCIELab* Lab)
// Notes:
// - Uses a minimal in-file test harness (no GoogleTest).
// - Relies on the public cmsCIELab type from the CMS LCMS headers.
// - Compares the computed value against a locally re-derived expected value with a tolerance.
// - Ensures both branches of the conditional are covered.

#include <cstdio>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include the library headers to obtain correct type definitions.
// If your build environment places lcms2.h in a non-standard path, adjust the include accordingly.

// Ensure C linkage for the focal method when linking from C++
extern "C" cmsFloat64Number ComputeLBFD(const cmsCIELab* Lab);

// Tolerance for floating-point comparisons
static constexpr double TOLERANCE = 1e-4;

// Simple helper to compare two doubles within a tolerance
static bool almost_equal(double a, double b, double tol)
{
    return std::fabs(a - b) <= tol;
}

// Test 1: True branch where Lab->L > 7.996969 (L = 9.0)
static bool test_computeLBFD_true_branch()
{
    cmsCIELab Lab;
    Lab.L = 9.0;
    Lab.a = 0.0;
    Lab.b = 0.0;

    cmsFloat64Number actual = ComputeLBFD(&Lab);

    // Recompute expected using the same mathematical formula used in the implementation.
    // yt = ( ((L+16)/116)^2 * ((L+16)/116) ) * 100
    double L = 9.0;
    double t1 = (L + 16.0) / 116.0;
    double yt = (t1 * t1) * t1 * 100.0;
    // expected = 54.6 * (M_LOG10E * log(yt + 1.5)) - 9.6
    // Use a precise constant for M_LOG10E (log10(e)).
    const double M_LOG10E = 0.4342944819032518;
    double expected = 54.6 * (M_LOG10E * std::log(yt + 1.5)) - 9.6;

    bool ok = almost_equal(static_cast<double>(actual), expected, TOLERANCE);

    if (ok) {
        std::printf("[Test 1] PASS: true-branch with L=9.0 produced expected value %.12f\n",
                    static_cast<double>(actual));
    } else {
        std::printf("[Test 1] FAIL: true-branch. Expected %.12f, got %.12f\n",
                    expected, static_cast<double>(actual));
    }
    return ok;
}

// Test 2: False branch where Lab->L <= 7.996969 (L = 0.0)
static bool test_computeLBFD_false_branch_zero()
{
    cmsCIELab Lab;
    Lab.L = 0.0;
    Lab.a = 0.0;
    Lab.b = 0.0;

    cmsFloat64Number actual = ComputeLBFD(&Lab);

    double L = 0.0;
    double yt = 100.0 * (L / 903.3); // this is 0.0
    const double M_LOG10E = 0.4342944819032518;
    double expected = 54.6 * (M_LOG10E * std::log(yt + 1.5)) - 9.6;

    bool ok = almost_equal(static_cast<double>(actual), expected, TOLERANCE);

    if (ok) {
        std::printf("[Test 2] PASS: false-branch with L=0.0 produced expected value %.12f\n",
                    static_cast<double>(actual));
    } else {
        std::printf("[Test 2] FAIL: false-branch. Expected %.12f, got %.12f\n",
                    expected, static_cast<double>(actual));
    }
    return ok;
}

// Test 3: Edge case where L equals the threshold (L = 7.996969), should take false branch
static bool test_computeLBFD_false_branch_edge()
{
    cmsCIELab Lab;
    Lab.L = 7.996969;
    Lab.a = 0.0;
    Lab.b = 0.0;

    cmsFloat64Number actual = ComputeLBFD(& Lab);

    double L = 7.996969;
    double yt = 100.0 * (L / 903.3);
    const double M_LOG10E = 0.4342944819032518;
    double expected = 54.6 * (M_LOG10E * std::log(yt + 1.5)) - 9.6;

    bool ok = almost_equal(static_cast<double>(actual), expected, TOLERANCE);

    if (ok) {
        std::printf("[Test 3] PASS: edge false-branch with L=%.6f produced expected value %.12f\n",
                    L, static_cast<double>(actual));
    } else {
        std::printf("[Test 3] FAIL: edge false-branch. Expected %.12f, got %.12f\n",
                    expected, static_cast<double>(actual));
    }
    return ok;
}

// Test 4: Near-threshold true branch (L = 7.997)
static bool test_computeLBFD_true_branch_near_threshold()
{
    cmsCIELab Lab;
    Lab.L = 7.997; // strictly greater than 7.996969
    Lab.a = 0.0;
    Lab.b = 0.0;

    cmsFloat64Number actual = ComputeLBFD(&Lab);

    double L = 7.997;
    double t1 = (L + 16.0) / 116.0;
    double yt = (t1 * t1) * t1 * 100.0;
    const double M_LOG10E = 0.4342944819032518;
    double expected = 54.6 * (M_LOG10E * std::log(yt + 1.5)) - 9.6;

    bool ok = almost_equal(static_cast<double>(actual), expected, TOLERANCE);

    if (ok) {
        std::printf("[Test 4] PASS: near-threshold true-branch with L=7.997 produced expected value %.12f\n",
                    static_cast<double>(actual));
    } else {
        std::printf("[Test 4] FAIL: near-threshold true-branch. Expected %.12f, got %.12f\n",
                    expected, static_cast<double>(actual));
    }
    return ok;
}

int main()
{
    // Run all tests and report summary.
    int total = 0;
    int passed = 0;

    // Test 1
    if (test_computeLBFD_true_branch()) ++passed;
    ++total;

    // Test 2
    if (test_computeLBFD_false_branch_zero()) ++passed;
    ++total;

    // Test 3
    if (test_computeLBFD_false_branch_edge()) ++passed;
    ++total;

    // Test 4
    if (test_computeLBFD_true_branch_near_threshold()) ++passed;
    ++total;

    std::printf("Summary: Passed %d / %d tests.\n", passed, total);

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}