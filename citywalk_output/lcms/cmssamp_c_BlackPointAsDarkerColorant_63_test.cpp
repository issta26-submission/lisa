// Test suite for the focal method: BlackPointAsDarkerColorant
// This file provides a small standalone test runner (no GTest, as requested).
// It targets the function declared in the LCMS library: cmsBool BlackPointAsDarkerColorant(...)

#include <limits>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include LCMS headers to declare the function under test and necessary types.
// The LCMS headers already export C linkage when compiled as C++, so this is safe.

// Domain knowledge hints:
// - This test suite focuses on basic control flow coverage for the focal method.
// - We cover at least one false-path (unsupported Intent) and one true-path (typical Intent).
// - We keep tests simple and rely on the real LCMS implementation to exercise deeper branches.
// - We use non-terminating style-like checks via returning boolean success/failure and printing results.
// - We avoid mocking; we rely on actual LCMS profile objects (e.g., sRGB).

/*
Candidate Keywords (representing core components the focal method interacts with):
- cmsIsIntentSupported
- cmsFormatterForColorspaceOfProfile
- cmsGetColorSpace
- _cmsEndPointsBySpace
- cmsCreateLab2ProfileTHR
- cmsCreateTransformTHR
- cmsCloseProfile
- cmsDoTransform
- cmsLab2XYZ
- cmsGetProfileContextID
- cmsGetDeviceClass
- cmsDetectBlackPoint
- INTENT constants (INTENT_PERCEPTUAL, INTENT_RELATIVE_COLORIMETRIC, INTENT_SATURATION)
- cmsCIEXYZ, cmsCIELab structures
- cmsUInt32Number, cmsHTRANSFORM, cmsHPROFILE
- Lab clipping logic (Lab.L adjustments)
- dwFlags parameter (usage avoided in tests, but present)
*/

// Simple test harness utilities

#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { std::cerr << "Assertion failed: " << (msg) << "  [" << __FILE__ << ":" << __LINE__ << "]\n"; return false; } } while(false)
#define ASSERT_FINITE(val) do { if(!std::isfinite(val)) { std::cerr << "Assertion failed: non-finite value encountered  [" << __FILE__ << ":" << __LINE__ << "]\n"; return false; } } while(false)


// Test 1: Unsupported Intent should cause the function to bail out early (FALSE is returned) and
//         BlackPoint should be set to 0,0,0.
bool test_BlackPointAsDarkerColorant_UnsupportedIntent()
{
    // Create a standard sRGB profile to exercise the path.
    cmsHPROFILE hInput = cmsCreate_sRGBProfile();
    if (hInput == NULL) {
        std::cerr << "Test skipped: failed to create sRGB profile.\n";
        return false;
    }

    // Use an obviously invalid/unsupported Intent value to trigger the early exit.
    cmsUInt32Number unsupportedIntent = 999; // outside the known intents
    cmsCIEXYZ BlackPoint = { -1.0, -1.0, -1.0 }; // pre-fill with junk to ensure write-back is observable

    cmsBool ok = BlackPointAsDarkerColorant(hInput, unsupportedIntent, &BlackPoint, 0);

    cmsCloseProfile(hInput);

    // Expectation: function returns FALSE and writes 0.0 to BlackPoint components.
    ASSERT_TRUE(ok == FALSE, "BlackPointAsDarkerColorant should return FALSE for unsupported intent");

    // Validate BlackPoint is reset to zeros (as per contract in code).
    ASSERT_TRUE(BlackPoint.X == 0.0 && BlackPoint.Y == 0.0 && BlackPoint.Z == 0.0,
                "BlackPoint should be reset to 0.0 for unsupported intent");

    return true;
}


// Test 2: Valid (supported) Intent should return TRUE and output a finite BlackPoint.
// This exercises the main success path where the function computes a black point using
// an available colorant and Lab-based conversion.
bool test_BlackPointAsDarkerColorant_ValidIntent_ReturnsTrue()
{
    // Create a standard sRGB profile to exercise the typical path.
    cmsHPROFILE hInput = cmsCreate_sRGBProfile();
    if (hInput == NULL) {
        std::cerr << "Test skipped: failed to create sRGB profile.\n";
        return false;
    }

    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    cmsCIEXYZ BlackPoint = { 0.0, 0.0, 0.0 };

    cmsBool ok = BlackPointAsDarkerColorant(hInput, intent, &BlackPoint, 0);

    cmsCloseProfile(hInput);

    // Expectation: function returns TRUE and BlackPoint is a valid XYZ point (finite numbers)
    ASSERT_TRUE(ok == TRUE, "BlackPointAsDarkerColorant should return TRUE for a valid perceptual intent");

    // If a black point could not be computed, values may be all zeros; we still expect finite numbers when computed.
    // Validate finiteness to ensure we didn't get NaN/Inf from color conversions.
    ASSERT_FINITE(BlackPoint.X);
    ASSERT_FINITE(BlackPoint.Y);
    ASSERT_FINITE(BlackPoint.Z);

    return true;
}


// Minimal main to run tests (no GTest or any framework; plain C++11 main)
int main()
{
    int failures = 0;

    std::cout << "Running test: BlackPointAsDarkerColorant_UnsupportedIntent\n";
    if (!test_BlackPointAsDarkerColorant_UnsupportedIntent()) {
        ++failures;
        std::cout << "FAILED: BlackPointAsDarkerColorant_UnsupportedIntent\n";
    } else {
        std::cout << "PASSED: BlackPointAsDarkerColorant_UnsupportedIntent\n";
    }

    std::cout << "Running test: BlackPointAsDarkerColorant_ValidIntent_ReturnsTrue\n";
    if (!test_BlackPointAsDarkerColorant_ValidIntent_ReturnsTrue()) {
        ++failures;
        std::cout << "FAILED: BlackPointAsDarkerColorant_ValidIntent_ReturnsTrue\n";
    } else {
        std::cout << "PASSED: BlackPointAsDarkerColorant_ValidIntent_ReturnsTrue\n";
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << failures << " TEST(S) FAILED\n";
        return 1;
    }
}