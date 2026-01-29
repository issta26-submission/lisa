// High-level: This test suite targets the focal function
// cmsHPROFILE Create_GrayLab(void) found in testcms2.c.
// We use plain C++11 (no Google Test) and rely on the LittleCMS API
// being available in the build environment (lcms2.h and library).
// The tests focus on the observable behavior of the function:
// - successful path returns a non-NULL profile with Lab as PCS
// - subsequent calls return distinct profile handles
// Notes:
// - We exercise true branches (non-NULL Curve) via normal library behavior.
// - We do not attempt to force the Curve == NULL path here, as control over
//   internal cmsBuildGamma is not guaranteed without library hook points.
// - Static/global members are not exercised here since the focal function uses
//   library-provided runtime state rather than static class members.

#include <iostream>
#include <lcms2.h>
#include <cassert>
#include <testcms2.h>


// Include LittleCMS public API

// Declare the focal function from the C source file (testcms2.c)
extern "C" cmsHPROFILE Create_GrayLab(void);

// Candidate Keywords matched to Step 1:
// DbgThread, cmsBuildGamma, Curve, cmsCreateGrayProfileTHR, cmsFreeToneCurve,
// cmsSetPCS, cmsD50_xyY, cmsSigLabData, cmsHPROFILE

// Test suite namespace to avoid global symbol pollution
namespace TestCMS_CreateGrayLab {

    // Test 1: Validate that the normal execution path returns a non-null profile
    // and that the Profile Connection Space (PCS) is set to Lab data (cmsSigLabData).
    // This covers the true branch of the if (Curve == NULL) condition.
    bool Test_CreateGrayLab_ReturnsNonNullAndLabPCS() {
        // Act
        cmsHPROFILE hProfile = Create_GrayLab();

        // Assert
        if (hProfile == NULL) {
            std::cerr << "Test_CreateGrayLab_ReturnsNonNullAndLabPCS: FAILED - Create_GrayLab returned NULL.\n";
            return false;
        }

        // Query PCS and verify it's Lab
        cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);
        if (pcs != cmsSigLabData) {
            std::cerr << "Test_CreateGrayLab_ReturnsNonNullAndLabPCS: FAILED - PCS != LabData (got: " << pcs << ").\n";
            cmsCloseProfile(hProfile);
            return false;
        }

        // Cleanup
        cmsCloseProfile(hProfile);
        return true;
    }

    // Test 2: Ensure multiple invocations produce distinct profile handles.
    // This helps verify there is no static singleton within the function and
    // that resources are independently allocated per call.
    bool Test_CreateGrayLab_TwoCallsProduceDistinctProfiles() {
        // Act
        cmsHPROFILE hProfile1 = Create_GrayLab();
        cmsHPROFILE hProfile2 = Create_GrayLab();

        // Assert both calls succeeded
        if (hProfile1 == NULL || hProfile2 == NULL) {
            if (hProfile1) cmsCloseProfile(hProfile1);
            if (hProfile2) cmsCloseProfile(hProfile2);
            std::cerr << "Test_CreateGrayLab_TwoCallsProduceDistinctProfiles: FAILED - One of the profiles is NULL.\n";
            return false;
        }

        // Ensure they are distinct handles
        if (hProfile1 == hProfile2) {
            cmsCloseProfile(hProfile1);
            cmsCloseProfile(hProfile2);
            std::cerr << "Test_CreateGrayLab_TwoCallsProduceDistinctProfiles: FAILED - Both calls returned the same handle.\n";
            return false;
        }

        // Cleanup
        cmsCloseProfile(hProfile1);
        cmsCloseProfile(hProfile2);
        return true;
    }

} // namespace TestCMS_CreateGrayLab

int main(int argc, char* argv[]) {
    // Run tests and report results
    using namespace TestCMS_CreateGrayLab;

    int failures = 0;

    std::cout << "Running Create_GrayLab unit tests...\n";

    // Test 1: true-path coverage for non-NULL Gamma Curve
    if (Test_CreateGrayLab_ReturnsNonNullAndLabPCS()) {
        std::cout << "Test 1 passed: Create_GrayLab returns non-null profile with Lab PCS.\n";
    } else {
        ++failures;
        std::cout << "Test 1 failed.\n";
    }

    // Test 2: multiple invocations yield distinct profiles
    if (Test_CreateGrayLab_TwoCallsProduceDistinctProfiles()) {
        std::cout << "Test 2 passed: Consecutive calls produce distinct profiles.\n";
    } else {
        ++failures;
        std::cout << "Test 2 failed.\n";
    }

    if (failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED. Failures: " << failures << "\n";
        return 1;
    }
}