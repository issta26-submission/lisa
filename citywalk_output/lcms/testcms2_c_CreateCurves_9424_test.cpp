/*
Candidate Keywords for the focal method (CreateCurves) and its dependencies:
- Gamma, Transfer, h
- cmsBuildGamma, cmsFreeToneCurve
- cmsCreateLinearizationDeviceLink
- cmsSigRgbData
- cmsToneCurve
- cmsHPROFILE
- cmsDoTransform, cmsCreateTransform, cmsDeleteTransform
- INTENT constants (e.g., INTENT_PERCEPTUAL)
- TYPE_RGB_8 (data format)
- DbgThread (testing context)
This test suite targets the CreateCurves function in testcms2.c and validates basic contract:
- non-null profile returned
- profile color space signature corresponds to cmsSigRgbData
- device-link transform preserves black (0,0,0) and white (255,255,255) for 8-bit data
The tests are written in C++11 style without Google Test and rely only on the public LittleCMS API.
*/

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <iomanip>
#include <lcms2.h>


// Include LittleCMS public headers. Adjust include path if necessary.
extern "C" {
}

// Forward declarations of testable function and necessary helpers
extern "C" cmsHPROFILE CreateCurves(void);

// Simple test harness for non-terminating tests
static void log_test(const std::string& testName, bool pass, const std::string& detail = "") {
    if (pass) {
        std::cout << "[PASS] " << testName;
        if (!detail.empty()) std::cout << " - " << detail;
        std::cout << std::endl;
    } else {
        std::cout << "[FAIL] " << testName;
        if (!detail.empty()) std::cout << " - " << detail;
        std::cout << std::endl;
    }
}

// Test 1: CreateCurves should return a non-null profile and RGB data color space
bool test_CreateCurves_ReturnsNonNullAndRgbData() {
    // Call the focal method under test
    cmsHPROFILE h = CreateCurves();

    // Check non-null handle
    if (h == nullptr) {
        log_test("CreateCurves_ReturnsNonNullAndRgbData", false, "CreateCurves returned NULL profile");
        return false;
    }

    // Check that the profile color space is RGB data
    cmsColorSpaceSignature cs = cmsGetColorSpaceSignature(h);
    bool isRgbData = (cs == cmsSigRgbData);

    cmsCloseProfile(h);

    if (!isRgbData) {
        log_test("CreateCurves_ReturnsNonNullAndRgbData", false, "Profile color space signature is not cmsSigRgbData");
        return false;
    }

    log_test("CreateCurves_ReturnsNonNullAndRgbData", true, "Profile is non-null and RGB Data as expected");
    return true;
}

// Test 2: Transform using the created device-link should preserve extremes (black and white)
// - For 8-bit RGB, black (0,0,0) should map to (0,0,0) and white (255,255,255) should map to (255,255,255)
bool test_CreateCurves_TransformBlackWhitePreserved() {
    cmsHPROFILE h = CreateCurves();
    if (h == nullptr) {
        log_test("CreateCurves_TransformBlackWhitePreserved", false, "CreateCurves returned NULL profile");
        return false;
    }

    // Create a transform: InProfile -> OutProfile with 8-bit RGB channels
    cmsHTRANSFORM xform = cmsCreateTransform(h, TYPE_RGB_8, h, TYPE_RGB_8, INTENT_PERCEPTUAL);
    if (xform == nullptr) {
        cmsCloseProfile(h);
        log_test("CreateCurves_TransformBlackWhitePreserved", false, "cmsCreateTransform failed to create transform");
        return false;
    }

    // Test black
    unsigned char inBlack[3] = {0, 0, 0};
    unsigned char outBlack[3] = {0, 0, 0};
    cmsDoTransform(xform, inBlack, outBlack, 1);
    bool blackOK = (outBlack[0] == 0 && outBlack[1] == 0 && outBlack[2] == 0);

    // Test white
    unsigned char inWhite[3] = {255, 255, 255};
    unsigned char outWhite[3] = {0, 0, 0};
    cmsDoTransform(xform, inWhite, outWhite, 1);
    bool whiteOK = (outWhite[0] == 255 && outWhite[1] == 255 && outWhite[2] == 255);

    // Cleanup
    cmsDeleteTransform(xform);
    cmsCloseProfile(h);

    if (blackOK && whiteOK) {
        log_test("CreateCurves_TransformBlackWhitePreserved", true,
                 "Black maps to black and White maps to white, as expected for a gamma-based device link");
        return true;
    } else {
        std::string detail = std::string("Black maps: ") +
            (blackOK ? "OK" : "Mismatch") + std::string("; White maps: ") +
            (whiteOK ? "OK" : "Mismatch");
        log_test("CreateCurves_TransformBlackWhitePreserved", false, detail);
        return false;
    }
}

int main() {
    // Explain test suite goals (comments only; runtime prints show results)
    // Step 1: Program Understanding
    // Analyzed focal method: creates a gamma curve (1.1), uses it for all three RGB channels,
    // builds a device link profile with cmsCreateLinearizationDeviceLink, frees the Gamma tone curve
    // and returns the profile.

    // Step 2: Unit Test Generation
    // Tests implemented:
    // - Test that CreateCurves returns a non-null profile and that its color space is cmsSigRgbData.
    // - Test that a transform from the device-link profile preserves black and white (robust 8-bit check).

    // Execute tests
    bool all_pass = true;

    all_pass &= test_CreateCurves_ReturnsNonNullAndRgbData();
    all_pass &= test_CreateCurves_TransformBlackWhitePreserved();

    if (all_pass) {
        std::cout << "\nAll tests passed successfully." << std::endl;
        return 0;
    } else {
        std::cout << "\nSome tests failed. Review logs above." << std::endl;
        return 1;
    }
}