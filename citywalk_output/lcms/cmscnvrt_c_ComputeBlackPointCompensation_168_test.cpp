#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>



// Tiny, non-terminating assertion helper
#define ASSERT_NEAR(a, b, tol, msg) do { \
    if (std::fabs((a) - (b)) > (tol)) { \
        std::cerr << "ASSERTION FAILED: " << (msg) \
                  << " | Got: " << (a) << " vs Expected: " << (b) \
                  << " (tol " << (tol) << ")" << std::endl; \
        return false; \
    } \
} while(0)

static bool near(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Test 1: Basic correctness of ComputeBlackPointCompensation
// This test builds a simple scenario where the input and output black points
// are arbitrarily chosen but ensure non-degenerate denominators (tx, ty, tz).
// It then verifies:
// - The resulting matrix m is diagonal with components ax, ay, az on the diagonal
// - The resulting offset off contains the expected bx, by, bz
// - The linear mapping m*bpin + off reproduces the original bpout for the given inputs
// The test derives all expected values directly from the same mathematical formula
// used in the focal method, but recomputed in test code to validate the implementation.
static bool test_basic_case() {
    const cmsCIEXYZ* D50 = cmsD50_XYZ(); // D50 reference point
    if (!D50) {
        std::cerr << "Test Basic: cmsD50_XYZ() returned null" << std::endl;
        return false;
    }

    // Choose in-point offset from D50 to guarantee tx != 0, ty != 0, tz != 0
    cmsCIEXYZ InPoint  = { D50->X + 0.10, D50->Y + 0.10, D50->Z + 0.10 };
    // Choose a distinct OutPoint to compute expected params
    cmsCIEXYZ OutPoint = { 0.80, 0.90, 0.70 };

    cmsMAT3 M;
    cmsVEC3 Off;

    // Call the focal function under test
    ComputeBlackPointCompensation(&InPoint, &OutPoint, &M, &Off);

    // Recompute expected values using the same formulas as the function
    // ax = (bpout.x - D50.x) / (bpin.x - D50.x)
    // bx = -D50.x * (bpout.x - bpin.x) / (bpin.x - D50.x)
    double tx = InPoint.X - D50->X;
    double ty = InPoint.Y - D50->Y;
    double tz = InPoint.Z - D50->Z;

    double ax = (OutPoint.X - D50->X) / tx;
    double ay = (OutPoint.Y - D50->Y) / ty;
    double az = (OutPoint.Z - D50->Z) / tz;

    double bx = - D50->X * (OutPoint.X - InPoint.X) / tx;
    double by = - D50->Y * (OutPoint.Y - InPoint.Y) / ty;
    double bz = - D50->Z * (OutPoint.Z - InPoint.Z) / tz;

    const double tol = 1e-9;

    // Validate matrix m contents (diagonal with (ax, ay, az))
    if (!near(M.v[0].X, ax, tol) || !near(M.v[0].Y, 0.0, tol) || !near(M.v[0].Z, 0.0, tol)) {
        std::cerr << "Test Basic: M.v[0] not matching expected ax-dominant vector." << std::endl;
        return false;
    }
    if (!near(M.v[1].X, 0.0, tol) || !near(M.v[1].Y, ay, tol) || !near(M.v[1].Z, 0.0, tol)) {
        std::cerr << "Test Basic: M.v[1] not matching expected ay-dominant vector." << std::endl;
        return false;
    }
    if (!near(M.v[2].X, 0.0, tol) || !near(M.v[2].Y, 0.0, tol) || !near(M.v[2].Z, az, tol)) {
        std::cerr << "Test Basic: M.v[2] not matching expected az-dominant vector." << std::endl;
        return false;
    }

    // Validate offset off
    if (!near(Off.X, bx, tol) || !near(Off.Y, by, tol) || !near(Off.Z, bz, tol)) {
        std::cerr << "Test Basic: Off components do not match expected bx/by/bz." << std::endl;
        return false;
    }

    // Validate forward mapping: bpout == m*bpin + off
    double Ox = M.v[0].X * InPoint.X + Off.X;
    double Oy = M.v[1].Y * InPoint.Y + Off.Y;
    double Oz = M.v[2].Z * InPoint.Z + Off.Z;

    if (!near(Ox, OutPoint.X, tol) || !near(Oy, OutPoint.Y, tol) || !near(Oz, OutPoint.Z, tol)) {
        std::cerr << "Test Basic: Forward mapping deerived value does not match OutPoint." << std::endl;
        return false;
    }

    std::cout << "Test Basic Case: PASSED" << std::endl;
    return true;
}

// Test 2: Another scenario with different In/Out values to exercise non-trivial ax/ay/az signs
// This ensures the implementation handles negative slopes and varied offsets correctly.
static bool test_second_case() {
    const cmsCIEXYZ* D50 = cmsD50_XYZ();
    if (!D50) {
        std::cerr << "Test Second: cmsD50_XYZ() returned null" << std::endl;
        return false;
    }

    // InPoint chosen to be distinct from D50 in all dimensions
    cmsCIEXYZ InPoint  = { D50->X - 0.20, D50->Y - 0.30, D50->Z + 0.25 };
    cmsCIEXYZ OutPoint = { 0.35, 0.50, 0.60 };

    cmsMAT3 M;
    cmsVEC3 Off;

    ComputeBlackPointCompensation(&InPoint, &OutPoint, &M, &Off);

    // Recompute expected parameters
    double tx = InPoint.X - D50->X;
    double ty = InPoint.Y - D50->Y;
    double tz = InPoint.Z - D50->Z;

    double ax = (OutPoint.X - D50->X) / tx;
    double ay = (OutPoint.Y - D50->Y) / ty;
    double az = (OutPoint.Z - D50->Z) / tz;

    double bx = - D50->X * (OutPoint.X - InPoint.X) / tx;
    double by = - D50->Y * (OutPoint.Y - InPoint.Y) / ty;
    double bz = - D50->Z * (OutPoint.Z - InPoint.Z) / tz;

    const double tol = 1e-9;

    // Matrix checks
    if (!near(M.v[0].X, ax, tol) || !near(M.v[0].Y, 0.0, tol) || !near(M.v[0].Z, 0.0, tol)) {
        std::cerr << "Test Second: M.v[0] did not match expected axis X component." << std::endl;
        return false;
    }
    if (!near(M.v[1].X, 0.0, tol) || !near(M.v[1].Y, ay, tol) || !near(M.v[1].Z, 0.0, tol)) {
        std::cerr << "Test Second: M.v[1] did not match expected axis Y component." << std::endl;
        return false;
    }
    if (!near(M.v[2].X, 0.0, tol) || !near(M.v[2].Y, 0.0, tol) || !near(M.v[2].Z, az, tol)) {
        std::cerr << "Test Second: M.v[2] did not match expected axis Z component." << std::endl;
        return false;
    }

    // Off checks
    if (!near(Off.X, bx, tol) || !near(Off.Y, by, tol) || !near(Off.Z, bz, tol)) {
        std::cerr << "Test Second: Off components do not match expected bx/by/bz." << std::endl;
        return false;
    }

    // Forward mapping check
    double Ox = M.v[0].X * InPoint.X + Off.X;
    double Oy = M.v[1].Y * InPoint.Y + Off.Y;
    double Oz = M.v[2].Z * InPoint.Z + Off.Z;

    if (!near(Ox, OutPoint.X, tol) || !near(Oy, OutPoint.Y, tol) || !near(Oz, OutPoint.Z, tol)) {
        std::cerr << "Test Second: Forward mapping check failed." << std::endl;
        return false;
    }

    std::cout << "Test Second Case: PASSED" << std::endl;
    return true;
}

int main() {
    // Run tests from main function (no GTest; pure C++11 standard test harness)
    bool all_passed = true;

    // Run Test 1
    if (!test_basic_case()) {
        std::cerr << "Overall result: Test Basic Case FAILED" << std::endl;
        all_passed = false;
    }

    // Run Test 2
    if (!test_second_case()) {
        std::cerr << "Overall result: Test Second Case FAILED" << std::endl;
        all_passed = false;
    }

    if (all_passed) {
        std::cout << "All tests PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests FAILED" << std::endl;
        return 1;
    }
}