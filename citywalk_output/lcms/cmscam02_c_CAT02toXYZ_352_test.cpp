#include <lcms2_internal.h>
#include <iostream>
#include <cmath>


// Minimal domain-specific test harness for the focal function CAT02toXYZ.
// Assumes the library defines CAM02COLOR with fields:
//   cmsFloat64Number XYZ[3];
//   cmsFloat64Number RGB[3];
// and provides: CAM02COLOR CAT02toXYZ(CAM02COLOR clr);
typedef double cmsFloat64Number;

typedef struct {
    cmsFloat64Number XYZ[3];
    cmsFloat64Number RGB[3];
} CAM02COLOR;

extern "C" CAM02COLOR CAT02toXYZ(CAM02COLOR clr);

// Utility: compare two doubles with tolerance
static bool almost_equal(double a, double b, double tol = 1e-6) {
    return std::fabs(a - b) <= tol;
}

// Utility: compare two 3-element vectors
static bool vec3_almost_equal(const cmsFloat64Number a[3], const cmsFloat64Number b[3], double tol = 1e-6) {
    return almost_equal(a[0], b[0], tol) &&
           almost_equal(a[1], b[1], tol) &&
           almost_equal(a[2], b[2], tol);
}

// Core test helper: run a single test case for given input RGB and expected XYZ
static bool test_case_identity(const char* name,
                               const cmsFloat64Number inR, const cmsFloat64Number inG, const cmsFloat64Number inB,
                               const cmsFloat64Number expX, const cmsFloat64Number expY, const cmsFloat64Number expZ) {
    CAM02COLOR input;
    input.RGB[0] = inR; input.RGB[1] = inG; input.RGB[2] = inB;
    input.XYZ[0] = input.XYZ[1] = input.XYZ[2] = 0.0;

    CAM02COLOR output = CAT02toXYZ(input);

    cmsFloat64Number expectedXYZ[3] = { expX, expY, expZ };
    bool ok = vec3_almost_equal(output.XYZ, expectedXYZ);

    // Additional check: the function should not modify RGB values (only XYZ is updated)
    if (!(almost_equal(output.RGB[0], inR) && almost_equal(output.RGB[1], inG) && almost_equal(output.RGB[2], inB))) {
        ok = false;
    }

    if (ok) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        std::cerr << "[FAIL] " << name
                  << " -> XYZ(" << output.XYZ[0] << ", " << output.XYZ[1] << ", " << output.XYZ[2] << ") "
                  << "expected(" << expX << ", " << expY << ", " << expZ << "), "
                  << "RGB(" << output.RGB[0] << ", " << output.RGB[1] << ", " << output.RGB[2] << ") vs input RGB("
                  << inR << ", " << inG << ", " << inB << ")\n";
    }

    return ok;
}

// Run all tests, reporting results to stdout/stderr
static void run_all_tests() {
    bool all_pass = true;

    // Test 1: Zero RGB should yield zero XYZ
    // This ensures a basic linear transformation baseline is correct and no artifacts appear.
    all_pass &= test_case_identity("CAT02toXYZ_zero_input",
                                   0.0, 0.0, 0.0,
                                   0.0, 0.0, 0.0);

    // Test 2: Red channel only
    // Verifies the first column of the transformation matrix contribution.
    all_pass &= test_case_identity("CAT02toXYZ_red_only",
                                   1.0, 0.0, 0.0,
                                   1.096124, 0.454369, -0.009628);

    // Test 3: Green channel only
    // Verifies the second column of the transformation matrix contribution.
    all_pass &= test_case_identity("CAT02toXYZ_green_only",
                                   0.0, 1.0, 0.0,
                                   -0.278869, 0.473533, -0.005698);

    // Test 4: Blue channel only
    // Verifies the third column of the transformation matrix contribution.
    all_pass &= test_case_identity("CAT02toXYZ_blue_only",
                                   0.0, 0.0, 1.0,
                                   0.182745, 0.072098, 1.015326);

    // Test 5: Equal RGB mix should map to equal XYZ
    // This checks the linear combination consistency when all channels are equal.
    all_pass &= test_case_identity("CAT02toXYZ_equal_mix",
                                   0.5, 0.5, 0.5,
                                   0.5, 0.5, 0.5);

    if (all_pass) {
        std::cout << "All tests passed.\n";
    } else {
        std::cerr << "Some tests FAILED.\n";
    }
}

int main() {
    // Execute the test suite
    run_all_tests();
    // Return non-zero if any test failed
    return 0;
}