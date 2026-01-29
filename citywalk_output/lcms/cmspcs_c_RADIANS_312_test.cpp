/*
Unit test suite for the focal function RADIANS located in cmspcs.c.

Step 1 - Program Understanding Highlights (translated into test goals)
- Core operation: converts degrees to radians via (deg * M_PI) / 180.
- Public type: cmsFloat64Number (alias for double in test environment).
- Dependency: M_PI macro (pi constant from <math.h>/<cmath>).

Candidate Keywords representing core dependencies to verify:
- RADIANS, cmsFloat64Number, M_PI, deg, pi-related constants, double precision behavior.

Step 2 - Generated Test Suite Overview
- Tests cover representative inputs including 0, 45, 90, 180, -90, and 360 degrees.
- Validates against expected radian results using a tolerant comparison to account for floating-point arithmetic.
- Provides a non-terminating assertion mechanism (EXPECT-like) to maximize code coverage.

Step 3 - Test Case Refinement Notes
- Uses C++11 standard only (no GTest).
- Tests compiled alongside the focal C source cmspcs.c; declares the function with extern "C" linkage for safe C/C++ interop.
- Static helpers are avoided in the tested function; test harness handles static-like concerns via test scope.
- Provides explanatory comments for each unit test case.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <limits>


#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

// Type alias to mirror the project's cmsFloat64Number
typedef double cmsFloat64Number;

// Declaration of the focal function with C linkage for compatibility when linked with cmspcs.c
extern "C" cmsFloat64Number RADIANS(cmsFloat64Number deg);

// Global counter for test failures to allow non-terminating test execution
static int gFailures = 0;

// Lightweight non-terminating EXPECT-like assertion for floating-point comparison
static void ExpectNear(cmsFloat64Number actual, cmsFloat64Number expected, cmsFloat64Number tol, const char* label) {
    if (std::fabs(actual - expected) > tol) {
        std::cerr << "FAIL: " << label
                  << " | expected ~ " << expected
                  << ", got " << actual << "\n";
        ++gFailures;
    } else {
        std::cout << "PASS: " << label << "\n";
    }
}

// Test case 1: 0 degrees should yield 0 radians
static void test_radians_zero() {
    cmsFloat64Number deg = 0.0;
    cmsFloat64Number res = RADIANS(deg);
    ExpectNear(res, 0.0, 1e-12, "RADIANS(0 deg) -> 0 radians");
}

// Test case 2: 45 degrees should yield pi/4 radians
static void test_radians_45_deg() {
    cmsFloat64Number deg = 45.0;
    cmsFloat64Number res = RADIANS(deg);
    cmsFloat64Number expected = M_PI / 4.0;
    ExpectNear(res, expected, 1e-12, "RADIANS(45 deg) -> pi/4 radians");
}

// Test case 3: 90 degrees should yield pi/2 radians
static void test_radians_90_deg() {
    cmsFloat64Number deg = 90.0;
    cmsFloat64Number res = RADIANS(deg);
    cmsFloat64Number expected = M_PI / 2.0;
    ExpectNear(res, expected, 1e-12, "RADIANS(90 deg) -> pi/2 radians");
}

// Test case 4: 180 degrees should yield pi radians
static void test_radians_180_deg() {
    cmsFloat64Number deg = 180.0;
    cmsFloat64Number res = RADIANS(deg);
    cmsFloat64Number expected = M_PI;
    ExpectNear(res, expected, 1e-12, "RADIANS(180 deg) -> pi radians");
}

// Test case 5: -90 degrees should yield -pi/2 radians
static void test_radians_negative_90_deg() {
    cmsFloat64Number deg = -90.0;
    cmsFloat64Number res = RADIANS(deg);
    cmsFloat64Number expected = -M_PI / 2.0;
    ExpectNear(res, expected, 1e-12, "RADIANS(-90 deg) -> -pi/2 radians");
}

// Test case 6: 360 degrees should yield 2*pi radians
static void test_radians_360_deg() {
    cmsFloat64Number deg = 360.0;
    cmsFloat64Number res = RADIANS(deg);
    cmsFloat64Number expected = 2.0 * M_PI;
    ExpectNear(res, expected, 1e-12, "RADIANS(360 deg) -> 2*pi radians");
}

// Entry point to execute all tests
int main() {
    std::cout << "Starting RADIANS() unit tests...\n";

    test_radians_zero();
    test_radians_45_deg();
    test_radians_90_deg();
    test_radians_180_deg();
    test_radians_negative_90_deg();
    test_radians_360_deg();

    std::cout << "RADIANS() tests completed. Failures: " << gFailures << "\n";

    // Non-terminating assertions: return non-zero if any test failed
    return gFailures ? 1 : 0;
}