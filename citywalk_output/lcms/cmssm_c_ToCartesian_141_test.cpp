// Unit test suite for the ToCartesian function in cmssm.c
// This test file provides a self-contained C++11 test harness that
// links against the C implementation of ToCartesian.
// It uses non-terminating, lightweight assertions to maximize coverage.

#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>
#include <cmath>
#include <sstream>


// Provide fallback M_PI if not defined by the platform headers
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

// -----------------------------------------------------------------------------
// Lightweight type definitions that mirror the expected CMS types used by
// ToCartesian. These definitions are intentionally minimal to allow linking
// with the existing C implementation in cmssm.c.
// -----------------------------------------------------------------------------

typedef double cmsFloat64Number;  // 64-bit floating point
typedef int cmsBool;
typedef unsigned int cmsUInt32Number;

#define FALSE 0
#define TRUE 1

// Indexes into the v->n array
#define VX 0
#define VY 1
#define VZ 2

typedef struct {
    cmsFloat64Number n[3];
} cmsVEC3;

typedef struct {
    cmsFloat64Number alpha;  // angle in degrees
    cmsFloat64Number theta;  // angle in degrees
    cmsFloat64Number r;      // radius
} cmsSpherical;

// Forward declaration of the focal function (C linkage)
extern "C" void ToCartesian(cmsVEC3* v, const cmsSpherical* sp);

// -----------------------------------------------------------------------------
// Lightweight test harness (non-terminating assertions). Accumulate failures
// and report at end of test run.
// -----------------------------------------------------------------------------

struct TestContext {
    int failures;

    TestContext() : failures(0) {}

    void fail(const std::string& msg) {
        ++failures;
        std::cerr << "Failure: " << msg << std::endl;
    }

    template <typename T, typename U>
    void expect_approx_eq(const T& a, const U& b, double tol, const std::string& desc) {
        double da = static_cast<double>(a);
        double db = static_cast<double>(b);
        if (std::fabs(da - db) > tol) {
            std::ostringstream oss;
            oss << desc << " | expected ~" << std::setprecision(12) << db
                << " but got " << da << " (tol " << tol << ")";
            fail(oss.str());
        }
    }

    template <typename T, typename U>
    void expect_eq(const T& a, const U& b, const std::string& desc) {
        if (!(a == b)) {
            std::ostringstream oss;
            oss << desc << " | expected " << a << " == " << b;
            fail(oss.str());
        }
    }

    // Helpers to verify a 3D vector against expected components
    void expect_vec3_approx_eq(const cmsVEC3& actual, const double expected[3], double tol, const std::string& desc) {
        expect_approx_eq(actual.n[VX], expected[0], tol, desc + " (VX)");
        expect_approx_eq(actual.n[VY], expected[1], tol, desc + " (VY)");
        expect_approx_eq(actual.n[VZ], expected[2], tol, desc + " (VZ)");
    }
};

// -----------------------------------------------------------------------------
// Test cases
// Each test is implemented as a separate function with explanatory comments.
// -----------------------------------------------------------------------------

// Test 1: Basic axis-aligned cases with theta/alpha at 0 or 90 degrees.
// Verifies known simple mappings:
// - Case A: alpha=0, theta=0, r=1  -> v = (L=1, a=0, b=0)
// - Case B: alpha=90, theta=0, r=1 -> v = (L=1, a=0, b=0)
// - Case C: alpha=0, theta=90, r=1 -> v = (L=0, a=0, b=1)
void test_ToCartesian_axis_aligned(TestContext& ctx) {
    const double tol = 1e-6;

    // Common helper lambda to run a single case
    auto run_case = [&](int case_id, double alpha, double theta, double r, const double expected[3]) {
        cmsSpherical sp;
        sp.alpha = alpha;
        sp.theta = theta;
        sp.r = r;

        cmsVEC3 v;
        ToCartesian(&v, &sp);

        ctx.expect_vec3_approx_eq(v, expected, tol,
            "case " + std::to_string(case_id) + " (alpha=" + std::to_string(alpha)
            + ", theta=" + std::to_string(theta) + ", r=" + std::to_string(r) + ")");
    };

    // Case A
    double expA[3] = {1.0, 0.0, 0.0};
    run_case(1, 0.0, 0.0, 1.0, expA);

    // Case B
    double expB[3] = {1.0, 0.0, 0.0};
    run_case(2, 90.0, 0.0, 1.0, expB);

    // Case C
    double expC[3] = {0.0, 0.0, 1.0};
    run_case(3, 0.0, 90.0, 1.0, expC);
}

// Test 2: A general case with non-trivial angles to validate correct trig handling.
// Parameters: alpha=30 deg, theta=60 deg, r=2
// Expected:
// sin_alpha = sin(pi/6) = 0.5
// cos_alpha = cos(pi/6) = 0.8660254038
// sin_theta = sin(pi/3) = 0.8660254038
// cos_theta = cos(pi/3) = 0.5
// a = r*sin_theta*sin_alpha = 2*0.8660254038*0.5 = 0.8660254038
// b = r*sin_theta*cos_alpha = 2*0.8660254038*0.8660254038 = 1.5
// L = r*cos_theta = 2*0.5 = 1.0
void test_ToCartesian_general_case(TestContext& ctx) {
    const double tol = 1e-6;

    cmsSpherical sp;
    sp.alpha = 30.0;
    sp.theta = 60.0;
    sp.r = 2.0;

    cmsVEC3 v;
    ToCartesian(&v, &sp);

    double expected[3] = {1.0, 0.8660254037844386, 1.5};

    ctx.expect_vec3_approx_eq(v, expected, tol,
        "general case (alpha=30°, theta=60°, r=2)");
}

// Test 3: Negative angles to ensure proper trigonometric handling with sign.
// Parameters: alpha=-45 deg, theta=-60 deg, r=1
// Expected:
// sin_alpha = sin(-pi/4) = -0.70710678
// cos_alpha = cos(-pi/4) = 0.70710678
// sin_theta = sin(-pi/3) = -0.86602540
// cos_theta = cos(-pi/3) = 0.5
// a = r*sin_theta*sin_alpha = (-0.866...)*(-0.7071) = 0.6123724357
// b = r*sin_theta*cos_alpha = (-0.866...)*(0.7071) = -0.6123724357
// L = r*cos_theta = 0.5
void test_ToCartesian_negative_angles(TestContext& ctx) {
    const double tol = 1e-6;

    cmsSpherical sp;
    sp.alpha = -45.0;
    sp.theta = -60.0;
    sp.r = 1.0;

    cmsVEC3 v;
    ToCartesian(&v, &sp);

    double expected[3] = {0.5, 0.612372435694, -0.612372435694};

    ctx.expect_vec3_approx_eq(v, expected, tol,
        "negative angles (alpha=-45°, theta=-60°, r=1)");
}

// Run all tests
void run_all_tests() {
    TestContext ctx;

    // Run individual test cases with explanatory comments
    // Test 1: Axis-aligned edge cases
    test_ToCartesian_axis_aligned(ctx);

    // Test 2: General non-trivial angles
    test_ToCartesian_general_case(ctx);

    // Test 3: Negative angle handling
    test_ToCartesian_negative_angles(ctx);

    // Summary
    if (ctx.failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cout << ctx.failures << " test(s) failed." << std::endl;
    }
}

// Entry point
int main() {
    // Execute the test suite. All tests are non-terminating and will print
    // their failures if any, but will attempt to cover as much code as possible.
    run_all_tests();
    return 0;
}