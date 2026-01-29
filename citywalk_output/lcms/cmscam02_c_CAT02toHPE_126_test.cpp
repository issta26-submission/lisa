// Test suite for CAM02COLOR CAT02toHPE in cmscam02.c without using GTest.
// The tests are written in C++11 and rely on the project headers to define
// CAM02COLOR, cmsFloat64Number, and the external function CAT02toHPE.
//
// Purpose: validate the 3x3 linear transformation implemented in CAT02toHPE
// by verifying the RGBp output for three basis input colors (red, green, blue).

#include <lcms2_internal.h>
#include <iostream>
#include <cstdio>
#include <cmath>


// Bring in C headers inside extern "C" for correct linkage when included in C++
extern "C" {
}

// Declare the function to ensure C linkage in C++ test
extern "C" CAM02COLOR CAT02toHPE(CAM02COLOR clr);

// Simple non-terminating test assertion and result reporting
static int g_failures = 0;

static bool approxEqual(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

static bool arraysApproxEqual(const double a[3], const double b[3], double tol = 1e-9) {
    for (int i = 0; i < 3; ++i) {
        if (!approxEqual(a[i], b[i], tol)) return false;
    }
    return true;
}

// Test 1: Red input [1, 0, 0] should yield [M0, M3, M6]
static void test_CAT02toHPE_red() {
    // Compute M just like in the focal function
    cmsFloat64Number M[9];
    M[0] = ((0.38971 * 1.096124) + (0.68898 * 0.454369) + (-0.07868 * -0.009628));
    M[1] = ((0.38971 * -0.278869) + (0.68898 * 0.473533) + (-0.07868 * -0.005698));
    M[2] = ((0.38971 * 0.182745) + (0.68898 * 0.072098) + (-0.07868 * 1.015326));
    M[3] = ((-0.22981 * 1.096124) + (1.18340 * 0.454369) + (0.04641 * -0.009628));
    M[4] = ((-0.22981 * -0.278869) + (1.18340 * 0.473533) + (0.04641 * -0.005698));
    M[5] = ((-0.22981 * 0.182745) + (1.18340 * 0.072098) + (0.04641 * 1.015326));
    M[6] = (-0.009628);
    M[7] = (-0.005698);
    M[8] = (1.015326);

    CAM02COLOR clr;
    clr.RGBc[0] = 1.0;
    clr.RGBc[1] = 0.0;
    clr.RGBc[2] = 0.0;

    CAM02COLOR result = CAT02toHPE(clr);

    double expected[3] = { (double)M[0], (double)M[3], (double)M[6] };

    double actual[3] = { result.RGBp[0], result.RGBp[1], result.RGBp[2] };

    bool ok = arraysApproxEqual(actual, expected);
    if (!ok) {
        std::cout << "FAIL: test_CAT02toHPE_red - expected {"
                  << expected[0] << ", " << expected[1] << ", " << expected[2]
                  << "} but got {" << actual[0] << ", " << actual[1] << ", "
                  << actual[2] << "}.\n";
        ++g_failures;
    } else {
        std::cout << "PASS: test_CAT02toHPE_red\n";
    }
}

// Test 2: Green input [0, 1, 0] should yield [M1, M4, M7]
static void test_CAT02toHPE_green() {
    // Reuse the same M computation as in the focal method
    cmsFloat64Number M[9];
    M[0] = ((0.38971 * 1.096124) + (0.68898 * 0.454369) + (-0.07868 * -0.009628));
    M[1] = ((0.38971 * -0.278869) + (0.68898 * 0.473533) + (-0.07868 * -0.005698));
    M[2] = ((0.38971 * 0.182745) + (0.68898 * 0.072098) + (-0.07868 * 1.015326));
    M[3] = ((-0.22981 * 1.096124) + (1.18340 * 0.454369) + (0.04641 * -0.009628));
    M[4] = ((-0.22981 * -0.278869) + (1.18340 * 0.473533) + (0.04641 * -0.005698));
    M[5] = ((-0.22981 * 0.182745) + (1.18340 * 0.072098) + (0.04641 * 1.015326));
    M[6] = (-0.009628);
    M[7] = (-0.005698);
    M[8] = (1.015326);

    CAM02COLOR clr;
    clr.RGBc[0] = 0.0;
    clr.RGBc[1] = 1.0;
    clr.RGBc[2] = 0.0;

    CAM02COLOR result = CAT02toHPE(clr);

    double expected[3] = { (double)M[1], (double)M[4], (double)M[7] };

    double actual[3] = { result.RGBp[0], result.RGBp[1], result.RGBp[2] };

    bool ok = arraysApproxEqual(actual, expected);
    if (!ok) {
        std::cout << "FAIL: test_CAT02toHPE_green - expected {"
                  << expected[0] << ", " << expected[1] << ", " << expected[2]
                  << "} but got {" << actual[0] << ", " << actual[1] << ", "
                  << actual[2] << "}.\n";
        ++g_failures;
    } else {
        std::cout << "PASS: test_CAT02toHPE_green\n";
    }
}

// Test 3: Blue input [0, 0, 1] should yield [M2, M5, M8]
static void test_CAT02toHPE_blue() {
    // Reuse M calculation
    cmsFloat64Number M[9];
    M[0] = ((0.38971 * 1.096124) + (0.68898 * 0.454369) + (-0.07868 * -0.009628));
    M[1] = ((0.38971 * -0.278869) + (0.68898 * 0.473533) + (-0.07868 * -0.005698));
    M[2] = ((0.38971 * 0.182745) + (0.68898 * 0.072098) + (-0.07868 * 1.015326));
    M[3] = ((-0.22981 * 1.096124) + (1.18340 * 0.454369) + (0.04641 * -0.009628));
    M[4] = ((-0.22981 * -0.278869) + (1.18340 * 0.473533) + (0.04641 * -0.005698));
    M[5] = ((-0.22981 * 0.182745) + (1.18340 * 0.072098) + (0.04641 * 1.015326));
    M[6] = (-0.009628);
    M[7] = (-0.005698);
    M[8] = (1.015326);

    CAM02COLOR clr;
    clr.RGBc[0] = 0.0;
    clr.RGBc[1] = 0.0;
    clr.RGBc[2] = 1.0;

    CAM02COLOR result = CAT02toHPE(clr);

    double expected[3] = { (double)M[2], (double)M[5], (double)M[8] };

    double actual[3] = { result.RGBp[0], result.RGBp[1], result.RGBp[2] };

    bool ok = arraysApproxEqual(actual, expected);
    if (!ok) {
        std::cout << "FAIL: test_CAT02toHPE_blue - expected {"
                  << expected[0] << ", " << expected[1] << ", " << expected[2]
                  << "} but got {" << actual[0] << ", " << actual[1] << ", "
                  << actual[2] << "}.\n";
        ++g_failures;
    } else {
        std::cout << "PASS: test_CAT02toHPE_blue\n";
    }
}

int main() {
    // Run tests in sequence
    test_CAT02toHPE_red();
    test_CAT02toHPE_green();
    test_CAT02toHPE_blue();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED.\n";
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED.\n";
        return 1;
    }
}