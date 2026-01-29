// Test suite for RootOfLeastSquaresFitQuadraticCurve in cmssamp.c
// This test harness is designed to be compiled with C++11 without GoogleTest.
// It calls the focal function directly and verifies multiple important branches.
// Note: The function signature is assumed to return cmsFloat64Number (likely double).
// We declare it here with a C linkage to match the symbol in cmssamp.c.

#include <limits>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cmath>


// Forward declaration of the focal method under test.
// We assume cmsFloat64Number is a typedef for double in the actual implementation.
// If cmsFloat64Number is different in the including project, adjust the return type accordingly.
// The parameters use plain double arrays to simplify interop for the test harness.
extern "C" double RootOfLeastSquaresFitQuadraticCurve(int n, double x[], double y[]);

// -------------------------------------------------------------------------------------
// Candidate Keywords (Step 1) - Representation of core components used by the focal method
// - n, x[], y[]: input data size and coordinates
// - Sums: sum_x, sum_x2, sum_x3, sum_x4, sum_y, sum_yx, sum_yx2
// - Linear algebra: cmsMAT3 m; cmsVEC3 v, res; function _cmsMAT3solve
// - Coefficients: a, b, c (quadratic y = a*x^2 + b*x + c, normalized from solution)
// - Branch predicates: n < 4, fabs(a) < 1.0e-10, d <= 0, clipping to [0, 50]
// - Utility: cmsmax, cmsmin, sqrt, fabs
// - Practical branches: a ~ 0 leading to linear solution; d > 0 leading to positive root
// -------------------------------------------------------------------------------------

// Helper for approximate comparison
static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::abs(a - b) <= tol;
}

// Simple tester macro
#define TEST_ASSERT(cond, msg)                                  \
    do {                                                         \
        if (cond) {                                              \
            std::cout << "[PASS] " << msg << std::endl;         \
        } else {                                                 \
            std::cout << "[FAIL] " << msg << std::endl;         \
        }                                                        \
    } while (0)

// Run a suite of tests covering true/false branches of predicates
static void RunRootOfLeastSquaresTests() {
    std::cout << "Running RootOfLeastSquaresFitQuadraticCurve tests..." << std::endl;

    // Test 1: n < 4 should early return 0 (branch: if (n < 4) return 0;)
    {
        double x[] = {0.0, 1.0, 2.0};       // n = 3
        double y[] = {1.0, 3.0, 5.0};       // y = 2x + 1 (linear)
        double res = RootOfLeastSquaresFitQuadraticCurve(3, x, y);
        TEST_ASSERT(almost_equal(res, 0.0), "n < 4 returns 0");
    }

    // Test 2: Linear case where a ~ 0 but b != 0
    // Data: y = 2x + 3 -> LS fit should yield (a ~ 0, b ~ 2, c ~ 3) -> root -c/b clipped to [0,50] -> 0
    {
        double x[] = {0.0, 1.0, 2.0, 3.0};
        double y[] = {3.0, 5.0, 7.0, 9.0}; // 2x + 3
        double res = RootOfLeastSquaresFitQuadraticCurve(4, x, y);
        TEST_ASSERT(almost_equal(res, 0.0), "Linear LS fit returns 0 (clamped) for small a");
    }

    // Test 3: Exact quadratic with a != 0, discriminant positive -> expect a positive root
    // Data from y = x^2 - 0.5 (a=1, b=0, c=-0.5). Root of a x^2 + b x + c = 0 => x = sqrt(2)/2 ~ 0.70710678
    {
        double x[] = {0.0, 1.0, 2.0, 3.0};       // four points
        double y[] = {-0.5, 0.5, 3.5, 8.5};      // y = x^2 - 0.5 exactly
        double res = RootOfLeastSquaresFitQuadraticCurve(4, x, y);
        double expected = std::sqrt(2.0) / 2.0;   // ~0.70710678
        TEST_ASSERT(almost_equal(res, expected, 1e-6), "Quadratic LS with a>0 yields positive root ~0.7071");
    }

    // Test 4: Quadratic with discriminant negative (d <= 0) -> should return 0
    // Data from y = x^2 + 1 (a=1, b=0, c=1) -> d = -4 < 0
    {
        double x[] = {0.0, 1.0, 2.0, 3.0};
        double y[] = {1.0, 2.0, 5.0, 10.0}; // y = x^2 + 1
        double res = RootOfLeastSquaresFitQuadraticCurve(4, x, y);
        TEST_ASSERT(almost_equal(res, 0.0), "Quadratic LS with d<=0 returns 0");
    }

    // Test 5: Constant data (a ~ 0, b ~ 0) -> should return 0 (branch: if fabs(a) < 1e-10 && fabs(b) < 1e-10)
    {
        double x[] = {0.0, 1.0, 2.0, 3.0};
        double y[] = {7.0, 7.0, 7.0, 7.0}; // Constant
        double res = RootOfLeastSquaresFitQuadraticCurve(4, x, y);
        TEST_ASSERT(almost_equal(res, 0.0), "Constant data yields 0 (a and b ~ 0)");
    }

    // Optional additional check: ensure non-negative clamps are in effect by inspecting a near-upper-bound scenario.
    // Construct a scenario that could push rt high; for safety we just ensure the result stays within [0,50].
    {
        double x[] = { -10.0, 0.0, 10.0, 20.0 };
        // A quadratic that tends to produce a positive root; choose y to approximate a x^2 with large c
        double y[] = {  1000.0,  100.0,  1100.0,  5000.0 }; // not exact LS relation, but ensures execution path
        double res = RootOfLeastSquaresFitQuadraticCurve(4, x, y);
        // Just verify it returns a finite value in [0,50], not NaN or Inf
        bool finite = std::isfinite(res);
        bool inrange = (res >= 0.0 - 1e-9) && (res <= 50.0 + 1e-9);
        TEST_ASSERT(finite && inrange, "Result finite and clamped to [0,50] when data is noisy");
    }
}

// Entry point
int main() {
    RunRootOfLeastSquaresTests();
    return 0;
}