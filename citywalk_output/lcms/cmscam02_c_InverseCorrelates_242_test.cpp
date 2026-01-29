// Lightweight unit tests for the focal method InverseCorrelates (CAM02COLOR InverseCorrelates(CAM02COLOR clr, cmsCIECAM02* pMod))
// This test suite uses a simple in-file test harness (no GoogleTest/GTest). It aims for non-terminating assertions
// and broad coverage of branches inside InverseCorrelates. Compile with a C/C++ project that provides the lcms2
// headers and implementation (as used in the provided focal method).
//
// Example compile command (adjust include/lib paths as needed):
// g++ -std=c++11 -I/path/to/lcms/include -L/path/to/lcms/lib test_inverse_correlates.cpp -llcms2 -o test_inverse_correlates

#include <cstring>
#include <limits>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include the library headers that define CAM02COLOR and cmsCIECAM02.
// The focal method relies on these types, so we must use the real definitions.

// Ensure linkage to the focal function
extern "C" CAM02COLOR InverseCorrelates(CAM02COLOR clr, cmsCIECAM02* pMod);

// Simple non-terminating assertion macros for the lightweight test harness
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_NEAR(val, exp, tol, msg) do { \
    if (std::fabs((val) - (exp)) > (tol)) { \
        std::cerr << "[FAIL] " << msg << " Expected: " << std::setprecision(12) << (exp) \
                  << ", Got: " << (val) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_FINITE(val, msg) do { \
    if (!(std::isfinite(val))) { \
        std::cerr << "[FAIL] " << msg << " Value is not finite (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

// Helper: robustly check if two doubles are both finite and near
static bool near(double a, double b, double tol = 1e-6) {
    if(!std::isfinite(a) || !std::isfinite(b)) return false;
    return std::fabs(a - b) <= tol;
}

// Test 1: t <= 0 branch (C == 0 yields t == 0). Verifies clr.a and clr.b are set to 0.0.
void test_inverse_correlates_branch_t_zero() {
    // Prepare input color with C = 0 to force t <= 0
    CAM02COLOR clr;
    std::memset(&clr, 0, sizeof(clr));
    clr.C = 0.0;    // triggers t <= 0
    clr.J = 50.0;   // nonzero to exercise A computation
    clr.h = 30.0;   // arbitrary

    // Prepare model with plausible parameters
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(mod));
    mod.n = 1.0;
    mod.Nc = 0.95;
    mod.Ncb = 0.8;
    mod.adoptedWhite.A = 1.0;
    mod.c = 0.69;
    mod.z = 1.0;
    mod.Nbb = 0.88;

    // Call the focal method
    CAM02COLOR result = InverseCorrelates(clr, &mod);

    // Expect A/B branch to have been skipped (a and b set to 0)
    EXPECT_NEAR(result.a, 0.0, 1e-9, "When C==0 (t<=0), a should be 0");
    EXPECT_NEAR(result.b, 0.0, 1e-9, "When C==0 (t<=0), b should be 0");

    // Also ensure RGBpa fields have been computed (not NaN/Inf)
    EXPECT_FINITE(result.RGBpa[0], "RGBpa[0] should be finite");
    EXPECT_FINITE(result.RGBpa[1], "RGBpa[1] should be finite");
    EXPECT_FINITE(result.RGBpa[2], "RGBpa[2] should be finite");
}

// Test 2: t > 0 with hr in first branch (fabs(sin(hr)) >= fabs(cos(hr))). Expect a ~ 0 due to cos(hr)/sin(hr) near 0 at hr ~ 90 degrees.
void test_inverse_correlates_branch_sin_dominant() {
    // Prepare input color with C > 0 to ensure t > 0
    CAM02COLOR clr;
    std::memset(&clr, 0, sizeof(clr));
    clr.C = 1.0;
    clr.J = 50.0;
    clr.h = 90.0;   // hr = h * d2r -> pi/2
    // clr.A will be computed inside function; we don't set it here.

    // Prepare model with plausible parameters
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(mod));
    mod.n = 1.0;
    mod.Nc = 0.95;
    mod.Ncb = 0.8;
    mod.adoptedWhite.A = 1.0;
    mod.c = 0.69;
    mod.z = 1.0;
    mod.Nbb = 0.88;

    CAM02COLOR result = InverseCorrelates(clr, &mod);

    // In the sin-dominant branch with hr ~ 90 degrees, cos(hr)/sin(hr) ~ 0, so a ~ 0
    EXPECT_NEAR(result.a, 0.0, 1e-6, "In sin-dominant branch, a should be approximately 0 due to cos/sin ratio");
    // b should be finite and non-negative (depends on internal constants); ensure it's finite and not NaN
    EXPECT_FINITE(result.b, "In sin-dominant branch, b should be finite");
    // R,G,B components should be finite
    EXPECT_FINITE(result.RGBpa[0], "RGBpa[0] should be finite");
    EXPECT_FINITE(result.RGBpa[1], "RGBpa[1] should be finite");
    EXPECT_FINITE(result.RGBpa[2], "RGBpa[2] should be finite");
}

// Test 3: t > 0 with hr in else branch (fabs(sin(hr)) < fabs(cos(hr))). Expect b ~ 0 (sin(hr)/cos(hr) ~ 0) and a finite.
void test_inverse_correlates_branch_cos_dominant() {
    // Prepare input color with C > 0 to ensure t > 0
    CAM02COLOR clr;
    std::memset(&clr, 0, sizeof(clr));
    clr.C = 0.5;
    clr.J = 50.0;
    clr.h = 0.0;     // hr = 0 -> sin(hr)=0, cos(hr)=1 -> else branch
    // clr.A will be computed inside function

    // Prepare model with plausible parameters
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(mod));
    mod.n = 1.0;
    mod.Nc = 0.95;
    mod.Ncb = 0.8;
    mod.adoptedWhite.A = 1.0;
    mod.c = 0.69;
    mod.z = 1.0;
    mod.Nbb = 0.88;

    CAM02COLOR result = InverseCorrelates(clr, &mod);

    // In the cos-dominant branch, sin(hr)/cos(hr) = 0, so b ~ 0
    EXPECT_NEAR(result.b, 0.0, 1e-6, "In cos-dominant branch, b should be approximately 0 due to sin/cos ratio");
    // a should be finite
    EXPECT_FINITE(result.a, "In cos-dominant branch, a should be finite");
    // RGBpa components should be finite
    EXPECT_FINITE(result.RGBpa[0], "RGBpa[0] should be finite");
    EXPECT_FINITE(result.RGBpa[1], "RGBpa[1] should be finite");
    EXPECT_FINITE(result.RGBpa[2], "RGBpa[2] should be finite");
}

// Main test runner
int main() {
    std::cout << "Running tests for InverseCorrelates (no GTest)." << std::endl;

    test_inverse_correlates_branch_t_zero();
    test_inverse_correlates_branch_sin_dominant();
    test_inverse_correlates_branch_cos_dominant();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}