// Test suite for the focal method: InverseChromaticAdaptation
// This test targets the CAM02COLOR InverseChromaticAdaptation(CAM02COLOR clr, cmsCIECAM02* pMod)
// The tests are written in C++11, without GoogleTest, and rely on the public library types
// provided by Little CMS (headers included). The tests are designed to be self-contained and
// executable from main() as requested.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include the library headers that define CAM02COLOR, cmsCIECAM02, and related types.
// Path assumptions: existing in the build environment.
// If your environment uses different include paths, adjust accordingly.

extern "C" {
    // Ensure the linker can find the symbol if it isn't prototyped in the included headers.
    CAM02COLOR InverseChromaticAdaptation(CAM02COLOR clr, cmsCIECAM02* pMod);
}

// Simple floating-point comparison helper
static bool approxEq(double a, double b, double tol = 1e-6) {
    return std::fabs(a - b) <= tol;
}

// Test 1: When D = 0.0, the denominator simplifies to 1.0, so output equals input clr.RGBc
// This validates the basic non-attenuating chromatic adaptation path.
static bool test_InverseChromaticAdaptation_DZero_basic() {
    // Setup input color
    CAM02COLOR clr;
    clr.RGBc[0] = 10.0;
    clr.RGBc[1] = 20.0;
    clr.RGBc[2] = 30.0;
    clr.RGB[0] = clr.RGB[1] = clr.RGB[2] = 0.0; // ensure output starts clean

    // Setup modulation/processing context
    cmsCIECAM02 mod;
    // Fill adoptedWhite with simple values
    mod.adoptedWhite.XYZ[0] = 1.0;
    mod.adoptedWhite.XYZ[1] = 2.0; // Y component used in denominator
    mod.adoptedWhite.XYZ[2] = 3.0;

    // Ensure RGB scaling factors exist for each channel
    mod.adoptedWhite.RGB[0] = 1.0;
    mod.adoptedWhite.RGB[1] = 1.0;
    mod.adoptedWhite.RGB[2] = 1.0;

    mod.D = 0.0;

    CAM02COLOR out = InverseChromaticAdaptation(clr, &mod);

    bool ok = true;
    ok &= approxEq(out.RGB[0], clr.RGBc[0]);
    ok &= approxEq(out.RGB[1], clr.RGBc[1]);
    ok &= approxEq(out.RGB[2], clr.RGBc[2]);
    // Additional check: RGBc should be unchanged (function uses it as input, not modified)
    ok &= approxEq(out.RGBc[0], clr.RGBc[0]);
    ok &= approxEq(out.RGBc[1], clr.RGBc[1]);
    ok &= approxEq(out.RGBc[2], clr.RGBc[2]);

    if (!ok) {
        std::cerr << "Test D=0.0 basic: FAILED. Expected RGB = RGBc (10,20,30). Got ("
                  << out.RGB[0] << ", " << out.RGB[1] << ", " << out.RGB[2] << ").\n";
    }
    return ok;
}

// Test 2: D = 0.5 with uniform adoptedWhite.RGB[i] = 1.0 and XYZ[1] = 2.0
// Expect each channel to be clr.RGBc[i] / (0.5 * XYZ[1] / 1.0 + 0.5) = clr.RGBc[i] / 1.5
static bool test_InverseChromaticAdaptation_DHalf_uniformRGB() {
    CAM02COLOR clr;
    clr.RGBc[0] = 6.0;
    clr.RGBc[1] = 12.0;
    clr.RGBc[2] = 18.0;
    clr.RGB[0] = clr.RGB[1] = clr.RGB[2] = 0.0;

    cmsCIECAM02 mod;
    mod.adoptedWhite.XYZ[0] = 0.0;
    mod.adoptedWhite.XYZ[1] = 2.0; // Y value
    mod.adoptedWhite.XYZ[2] = 0.0;
    mod.adoptedWhite.RGB[0] = 1.0;
    mod.adoptedWhite.RGB[1] = 1.0;
    mod.adoptedWhite.RGB[2] = 1.0;
    mod.D = 0.5;

    CAM02COLOR out = InverseChromaticAdaptation(clr, &mod);

    bool ok = true;
    ok &= approxEq(out.RGB[0], 6.0 / 1.5);
    ok &= approxEq(out.RGB[1], 12.0 / 1.5);
    ok &= approxEq(out.RGB[2], 18.0 / 1.5);

    if (!ok) {
        std::cerr << "Test D=0.5 uniform RGB: FAILED. Expected (4,8,12). Got ("
                  << out.RGB[0] << ", " << out.RGB[1] << ", " << out.RGB[2] << ").\n";
    }

    // Also ensure RGBc remains unchanged
    if (!approxEq(out.RGBc[0], clr.RGBc[0]) || !approxEq(out.RGBc[1], clr.RGBc[1]) || !approxEq(out.RGBc[2], clr.RGBc[2])) {
        std::cerr << "Test D=0.5 uniform RGB: RGBc changed unexpectedly.\n";
        ok = false;
    }

    return ok;
}

// Test 3: D = 0.5 with varied adoptedWhite.RGB across channels to exercise per-channel denominators
static bool test_InverseChromaticAdaptation_DHalf_variedRGBDenominators() {
    CAM02COLOR clr;
    clr.RGBc[0] = 14.0;
    clr.RGBc[1] = 15.0;
    clr.RGBc[2] = 16.0;
    clr.RGB[0] = clr.RGB[1] = clr.RGB[2] = 0.0;

    cmsCIECAM02 mod;
    mod.adoptedWhite.XYZ[0] = 0.0;
    mod.adoptedWhite.XYZ[1] = 3.0; // Y = 3
    mod.adoptedWhite.XYZ[2] = 0.0;

    // Vary per-channel RGB scaling
    mod.adoptedWhite.RGB[0] = 0.5; // i=0 denom = (3*0.5/0.5) + 0.5 = 3.0 + 0.5 = 3.5
    mod.adoptedWhite.RGB[1] = 2.0; // i=1 denom = (3*0.5/2.0) + 0.5 = 0.75 + 0.5 = 1.25
    mod.adoptedWhite.RGB[2] = 1.0; // i=2 denom = (3*0.5/1.0) + 0.5 = 1.5 + 0.5 = 2.0

    mod.D = 0.5;

    CAM02COLOR out = InverseChromaticAdaptation(clr, &mod);

    bool ok = true;
    ok &= approxEq(out.RGB[0], 14.0 / 3.5); // 4.0
    ok &= approxEq(out.RGB[1], 15.0 / 1.25); // 12.0
    ok &= approxEq(out.RGB[2], 16.0 / 2.0); // 8.0

    if (!ok) {
        std::cerr << "Test D=0.5 varied RGB denominators: FAILED. Got ("
                  << out.RGB[0] << ", " << out.RGB[1] << ", " << out.RGB[2] << ").\n";
    }

    // Ensure RGBc unchanged
    if (!approxEq(out.RGBc[0], clr.RGBc[0]) || !approxEq(out.RGBc[1], clr.RGBc[1]) || !approxEq(out.RGBc[2], clr.RGBc[2])) {
        std::cerr << "Test D=0.5 varied RGB denominators: RGBc changed unexpectedly.\n";
        ok = false;
    }

    return ok;
}

// Test 4: Sanity check that inputs are preserved when possible (RGBc remains unchanged, only RGB derived from RGBc)
static bool test_InverseChromaticAdaptation_RGBcPreserved() {
    CAM02COLOR clr;
    clr.RGBc[0] = 2.0;
    clr.RGBc[1] = 3.0;
    clr.RGBc[2] = 4.0;
    clr.RGB[0] = clr.RGB[1] = clr.RGB[2] = 0.0;

    cmsCIECAM02 mod;
    mod.adoptedWhite.XYZ[1] = 1.0;
    mod.adoptedWhite.RGB[0] = 2.0;
    mod.adoptedWhite.RGB[1] = 2.0;
    mod.adoptedWhite.RGB[2] = 2.0;
    mod.D = 0.25;

    // Arbitrary XYZ to avoid division by zero
    mod.adoptedWhite.XYZ[0] = 0.1;
    mod.adoptedWhite.XYZ[2] = 0.3;

    CAM02COLOR out = InverseChromaticAdaptation(clr, &mod);

    // RGBc should be unchanged
    if (!approxEq(out.RGBc[0], clr.RGBc[0]) || !approxEq(out.RGBc[1], clr.RGBc[1]) || !approxEq(out.RGBc[2], clr.RGBc[2])) {
        std::cerr << "Test RGBcPreserved: RGBc was modified unexpectedly to ("
                  << out.RGBc[0] << ", " << out.RGBc[1] << ", " << out.RGBc[2] << ").\n";
        return false;
    }

    return true;
}

int main() {
    int failures = 0;

    std::cout << "Starting InverseChromaticAdaptation test suite...\n";

    if (!test_InverseChromaticAdaptation_DZero_basic()) {
        ++failures;
    } else {
        std::cout << "PASS: test_InverseChromaticAdaptation_DZero_basic\n";
    }

    if (!test_InverseChromaticAdaptation_DHalf_uniformRGB()) {
        ++failures;
    } else {
        std::cout << "PASS: test_InverseChromaticAdaptation_DHalf_uniformRGB\n";
    }

    if (!test_InverseChromaticAdaptation_DHalf_variedRGBDenominators()) {
        ++failures;
    } else {
        std::cout << "PASS: test_InverseChromaticAdaptation_DHalf_variedRGBDenominators\n";
    }

    if (!test_InverseChromaticAdaptation_RGBcPreserved()) {
        ++failures;
    } else {
        std::cout << "PASS: test_InverseChromaticAdaptation_RGBcPreserved\n";
    }

    if (failures == 0) {
        std::cout << "All InverseChromaticAdaptation tests PASSED.\n";
        return 0;
    } else {
        std::cout << "InverseChromaticAdaptation tests FAILED with " << failures << " failure(s).\n";
        return 1;
    }
}