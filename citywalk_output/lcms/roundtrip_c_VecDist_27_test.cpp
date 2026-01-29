/*
Unit test suite for VecDist function found in roundtrip.c

Guidance implemented from the provided instructions:
- Uses only C++11 standard library (no GTest).
- Calls the VecDist function via C linkage to avoid name mangling.
- Exercises multiple input scenarios to ensure robust coverage (edge cases, typical cases, and max values).
- Uses non-terminating, custom checks that print results and continue execution.
- Explanatory comments accompany each test case.

Candidate Keywords identified from the focal method:
VecDist, cmsUInt8Number, bin[3], bout[3], rdist, gdist, bdist, fabs, sqrt, double
*/

#include <math.h>
#include <iostream>
#include <string>
#include <lcms2.h>
#include <cmath>



// Ensure C linkage for the function under test
extern "C" double VecDist(cmsUInt8Number bin[3], cmsUInt8Number bout[3]);

// Global counters for test tracking (non-terminating checks)
static int g_tests = 0;
static int g_failures = 0;

// Helper: approximate double comparison
static void check_double_equal(double expected, double actual, const std::string& testName, double tol = 1e-9) {
    ++g_tests;
    if (std::fabs(expected - actual) <= tol) {
        std::cout << "[PASSED] " << testName
                  << " | expected: " << expected
                  << ", actual: " << actual << "\n";
    } else {
        ++g_failures;
        std::cout << "[FAILED] " << testName
                  << " | expected: " << std::setprecision(15) << expected
                  << ", actual: " << std::setprecision(15) << actual << "\n";
    }
}

// Helper: exact boolean check
static void check_true(bool cond, const std::string& testName) {
    ++g_tests;
    if (cond) {
        std::cout << "[PASSED] " << testName << "\n";
    } else {
        ++g_failures;
        std::cout << "[FAILED] " << testName << "\n";
    }
}

int main(int argc, char* argv[]) {
    // Note: This test suite targets VecDist which computes Euclidean distance
    // between two 3-channel unsigned 8-bit vectors.

    // Test 1: Identical vectors -> distance should be 0
    // Rationale: baseline case; no difference in components.
    cmsUInt8Number bin1[3] = {0, 0, 0};
    cmsUInt8Number bout1[3] = {0, 0, 0};
    double d1 = VecDist(bin1, bout1);
    check_double_equal(0.0, d1, "VecDist: identical zero vectors returns 0");

    // Test 2: Single channel difference
    // Rationale: distance should reflect the absolute difference on one channel.
    cmsUInt8Number bin2[3] = {0, 0, 0};
    cmsUInt8Number bout2[3] = {3, 0, 0};
    double d2 = VecDist(bin2, bout2);
    check_double_equal(3.0, d2, "VecDist: difference on first channel only (3,0,0) => 3");

    // Test 3: Differences across all channels
    // Rationale: classic Pythagorean distance in 3D space.
    cmsUInt8Number bin3[3] = {0, 0, 0};
    cmsUInt8Number bout3[3] = {3, 4, 12}; // 3-4-12 triangle components
    double d3 = VecDist(bin3, bout3);
    check_double_equal(13.0, d3, "VecDist: (0,0,0) vs (3,4,12) => 13");

    // Test 4: Max-valued channels (255,255,255)
    // Rationale: verify behavior at upper bounds of unsigned 8-bit range.
    cmsUInt8Number bin4[3] = {0, 0, 0};
    cmsUInt8Number bout4[3] = {255, 255, 255};
    double d4 = VecDist(bin4, bout4);
    double expected4 = 255.0 * std::sqrt(3.0); // sqrt(3 * 255^2)
    check_double_equal(expected4, d4, "VecDist: max-valued channels (0,0,0) vs (255,255,255)");

    // Test 5: Mixed values (non-trivial non-uniform differences)
    // Rationale: verify correctness with arbitrary values where all channels differ.
    cmsUInt8Number bin5[3] = {100, 150, 200};
    cmsUInt8Number bout5[3] = {50, 100, 150};
    double d5 = VecDist(bin5, bout5);
    double diff = 50.0; // each channel difference
    double expected5 = std::sqrt(diff*diff + diff*diff + diff*diff); // sqrt(3*50^2)
    check_double_equal(expected5, d5, "VecDist: mixed values (100,150,200) vs (50,100,150)");

    // Test 6: Identical non-zero vectors
    // Rationale: distance should still be 0 when vectors are identical, regardless of values.
    cmsUInt8Number bin6[3] = {10, 20, 30};
    cmsUInt8Number bout6[3] = {10, 20, 30};
    double d6 = VecDist(bin6, bout6);
    check_double_equal(0.0, d6, "VecDist: identical non-zero vectors (10,20,30) => 0");

    // Summary
    std::cout << "Tests run: " << g_tests << ", Failures: " << g_failures << "\n";

    return (g_failures == 0) ? 0 : 1;
}