// DistanceFunctionTests.cpp
// A lightweight C++11 test harness for the C function:
//     double distance(const cmsUInt16Number* a, const cmsUInt16Number* b);
// The function is assumed to be defined in testcms2.c and linked accordingly.
//
// Notes:
// - We avoid any GTest usage as requested.
// - We expose the C function with C linkage to ensure proper symbol resolution.
// - Tests cover typical, edge, and random scenarios for a 3D point distance.
// - Each test case is documented with comments explaining the intent and scenario.

#include <cstdio>
#include <cmath>
#include <testcms2.h>
#include <cstdlib>


// Domain/types aligned with the focal signature.
// The real project defines cmsUInt16Number in its headers; here we alias to unsigned short.
typedef unsigned short cmsUInt16Number;

// Ensure we link against the C implementation of distance.
// The distance function is defined in testcms2.c (as shown in the prompt).
extern "C" double distance(const cmsUInt16Number* a, const cmsUInt16Number* b);

// Simple test harness state.
static int g_total = 0;
static int g_passed = 0;

// Helper to run a single distance test case.
// a0..a2: components of vector a
// b0..b2: components of vector b
// expected: expected distance value
// name: descriptive test name
// tol: permissible tolerance for floating point comparison
static bool run_case(int a0, int a1, int a2,
                     int b0, int b1, int b2,
                     double expected, const char* name,
                     double tol = 1e-9)
{
    cmsUInt16Number a[3] = {
        static_cast<cmsUInt16Number>(a0),
        static_cast<cmsUInt16Number>(a1),
        static_cast<cmsUInt16Number>(a2)
    };
    cmsUInt16Number b[3] = {
        static_cast<cmsUInt16Number>(b0),
        static_cast<cmsUInt16Number>(b1),
        static_cast<cmsUInt16Number>(b2)
    };

    double got = distance(a, b);
    double diff = std::fabs(got - expected);

    g_total++;
    bool ok = diff <= tol;
    if (ok) {
        g_passed++;
        std::printf("PASS: %s\n", name);
    } else {
        std::printf("FAIL: %s\n", name);
        std::printf("      inputs A(%d, %d, %d) B(%d, %d, %d)\n",
                    a0, a1, a2, b0, b1, b2);
        std::printf("      expected %.12f, got %.12f, tol %.1e\n",
                    expected, got, tol);
    }
    return ok;
}

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    // Step 2: Comprehensive test suite for distance
    // Each test is crafted to exercise valid inputs and typical edge cases.

    // Test 1: Zero distance between identical points.
    // Rationale: The distance from a point to itself should be 0.
    // Expected: 0.0
    run_case(0, 0, 0, 0, 0, 0, 0.0, "Zero distance between identical points");

    // Test 2: Classic 3-4-0 Pythagorean scenario in 3D.
    // Rationale: Distance between (3,0,0) and (0,4,0) should be 5.
    // Expected: 5.0
    run_case(3, 0, 0, 0, 4, 0, 5.0, "Pythagorean distance in XY plane (3,0,0) vs (0,4,0)");

    // Test 3: Diagonal across all three axes.
    // Rationale: Distance between (1,2,3) and (4,6,8) equals sqrt(3^2 + 4^2 + 5^2) = sqrt(50)
    // Expected: sqrt(50)
    run_case(1, 2, 3, 4, 6, 8, std::sqrt(50.0), "Diagonal distance across three axes (1,2,3) vs (4,6,8)",
             1e-12);

    // Test 4: Edge case with maximum 16-bit unsigned values.
    // Rationale: Distances involving 65535 can stress numeric limits; use a large vector.
    // Expected: sqrt( (65535)^2 + (65535)^2 + (65535)^2 ) = sqrt(3 * 65535^2)
    {
        double expected4 = std::sqrt(3.0 * static_cast<double>(65535) * static_cast<double>(65535));
        run_case(0, 0, 0, 65535, 65535, 65535, expected4,
                 "Distance from (0,0,0) to (65535,65535,65535)");
    }

    // Test 5: Arbitrary values to ensure general correctness.
    // Rationale: Check a non-trivial distance where differences are (100, -44, 55)
    // Expected: sqrt(100^2 + (-44)^2 + 55^2) = sqrt(10000 + 1936 + 3025) = sqrt(14961)
    {
        double expected5 = std::sqrt(14961.0);
        run_case(123, 45, 67, 23, 89, 12, expected5,
                 "Arbitrary case: (123,45,67) vs (23,89,12)");
    }

    // Optional: You can add more cases if needed to increase coverage.

    // Summary
    std::printf("Total tests: %d, Passed: %d, Failed: %d\n",
                g_total, g_passed, g_total - g_passed);

    // Return non-zero if any test failed to signal a test-suite failure to CI systems.
    return (g_passed == g_total) ? 0 : 1;
}