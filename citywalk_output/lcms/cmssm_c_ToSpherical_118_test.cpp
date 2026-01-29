// Unit tests for ToSpherical in cmssm.c
// This test suite is designed to be compiled with C++11 without using GTest.
// It relies on the external function declaration for ToSpherical and CMS types
// defined in the CMS header (lcms2_internal.h). The tests exercise both
// the zero-vector branch and several non-zero vectors to validate r, alpha
// and theta calculations.

#include <lcms2_internal.h>
#include <cstdio>
#include <cmath>
#include <cstdlib>


// Dependency header that defines cmsSpherical, cmsVEC3, VX/VY/VZ, etc.
// Adjust include path as necessary for your build environment.

extern "C" void ToSpherical(cmsSpherical* sp, const cmsVEC3* v);

static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Simple non-terminating assertion helper for doubles
static void assert_double_eq(const char* testName, const char* label,
                             double got, double expected, double tol = 1e-9) {
    if (!almost_equal(got, expected, tol)) {
        printf("Test %s FAILED: %s = %.12f, expected %.12f (tol %.1e)\n",
               testName, label, got, expected, tol);
    } else {
        printf("Test %s PASSED: %s = %.12f (within tol %.1e)\n",
               testName, label, got, tol);
    }
}

// Test 1: ToSpherical should handle zero vector (r == 0) branch correctly.
// Expect r == 0 and alpha == 0, theta == 0.
static bool test_ToSpherical_zero_vector() {
    const char* testName = "ZeroVector";
    cmsVEC3 v;
    // L = 0, a = 0, b = 0
    v.n[0] = 0.0; // VX
    v.n[1] = 0.0; // VY
    v.n[2] = 0.0; // VZ

    cmsSpherical sp;
    ToSpherical(&sp, &v);

    bool pass = true;
    if (sp.r != 0.0) {
        printf("Test %s FAILED: r = %f, expected 0.0\n", testName, sp.r);
        pass = false;
    }
    if (sp.alpha != 0.0) {
        printf("Test %s FAILED: alpha = %f, expected 0.0\n", testName, sp.alpha);
        pass = false;
    }
    if (sp.theta != 0.0) {
        printf("Test %s FAILED: theta = %f, expected 0.0\n", testName, sp.theta);
        pass = false;
    }
    if (pass) {
        printf("Test %s PASSED: zero vector handled correctly.\n", testName);
    }

    // Use the helper to log details (even when passing) for uniform output
    assert_double_eq(testName, "r", sp.r, 0.0);
    assert_double_eq(testName, "alpha", sp.alpha, 0.0);
    assert_double_eq(testName, "theta", sp.theta, 0.0);

    return pass;
}

// Test 2: Non-zero vector with L=3, a=4, b=0
// Expect r = 5, alpha = atan2(4, 0) = pi/2, theta = atan2( sqrt(4^2+0^2)=4, 3 )
static bool test_ToSpherical_case_positive_quadrant() {
    const char* testName = "Case_Positive_Quadrant";
    cmsVEC3 v;
    v.n[0] = 3.0; // L
    v.n[1] = 4.0; // a
    v.n[2] = 0.0; // b

    cmsSpherical sp;
    ToSpherical(&sp, &v);

    double expected_r = std::sqrt(3.0*3.0 + 4.0*4.0 + 0.0);
    double expected_alpha = std::atan2(4.0, 0.0);
    double expected_theta = std::atan2(std::sqrt(4.0*4.0 + 0.0*0.0), 3.0);

    assert_double_eq(testName, "r", sp.r, expected_r);
    assert_double_eq(testName, "alpha", sp.alpha, expected_alpha);
    assert_double_eq(testName, "theta", sp.theta, expected_theta);

    bool pass = almost_equal(sp.r, expected_r) &&
                almost_equal(sp.alpha, expected_alpha) &&
                almost_equal(sp.theta, expected_theta);
    if (pass) {
        printf("Test %s PASSED: non-zero vector in first quadrant.\n", testName);
    }
    return pass;
}

// Test 3: Non-zero vector in negative quadrant L=1, a=-1, b=-1
// Expect r = sqrt(3), alpha = atan2(-1, -1) ~ -2.356... , theta = atan2(sqrt(2), 1)
static bool test_ToSpherical_negative_quadrant() {
    const char* testName = "Case_Negative_Quadrant";
    cmsVEC3 v;
    v.n[0] = 1.0;  // L
    v.n[1] = -1.0; // a
    v.n[2] = -1.0; // b

    cmsSpherical sp;
    ToSpherical(&sp, &v);

    double expected_r = std::sqrt(3.0);
    double expected_alpha = std::atan2(-1.0, -1.0);
    double expected_theta = std::atan2(std::sqrt(2.0), 1.0);

    assert_double_eq(testName, "r", sp.r, expected_r);
    assert_double_eq(testName, "alpha", sp.alpha, expected_alpha);
    assert_double_eq(testName, "theta", sp.theta, expected_theta);

    bool pass = almost_equal(sp.r, expected_r) &&
                almost_equal(sp.alpha, expected_alpha) &&
                almost_equal(sp.theta, expected_theta);
    if (pass) {
        printf("Test %s PASSED: negative quadrant values computed correctly.\n", testName);
    }
    return pass;
}

// Test 4: Non-zero vector L=0, a=2, b=2
// Expect r = sqrt(8), alpha = atan2(2,2) = pi/4, theta = atan2(sqrt(8), 0) = pi/2
static bool test_ToSpherical_case_vertex_on_bisector() {
    const char* testName = "Case_Bisector_L0A2B2";
    cmsVEC3 v;
    v.n[0] = 0.0; // L
    v.n[1] = 2.0; // a
    v.n[2] = 2.0; // b

    cmsSpherical sp;
    ToSpherical(&sp, &v);

    double expected_r = std::sqrt(8.0);
    double expected_alpha = std::atan2(2.0, 2.0);
    double expected_theta = std::atan2(std::sqrt(8.0), 0.0);

    // Note: atan2(y, 0) with y>0 returns pi/2
    // We perform exact checks
    assert_double_eq(testName, "r", sp.r, expected_r);
    assert_double_eq(testName, "alpha", sp.alpha, expected_alpha);
    assert_double_eq(testName, "theta", sp.theta, expected_theta);

    bool pass = almost_equal(sp.r, expected_r) &&
                almost_equal(sp.alpha, expected_alpha) &&
                almost_equal(sp.theta, expected_theta);
    if (pass) {
        printf("Test %s PASSED: L=0, a=b case calculated correctly.\n", testName);
    }
    return pass;
}

int main() {
    // Run all tests and summarize
    int total = 0;
    int passed = 0;

    if (test_ToSpherical_zero_vector()) passed++;
    total++;

    if (test_ToSpherical_case_positive_quadrant()) passed++;
    total++;

    if (test_ToSpherical_negative_quadrant()) passed++;
    total++;

    if (test_ToSpherical_case_vertex_on_bisector()) passed++;
    total++;

    printf("Test suite completed: %d/%d tests passed.\n", passed, total);

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}