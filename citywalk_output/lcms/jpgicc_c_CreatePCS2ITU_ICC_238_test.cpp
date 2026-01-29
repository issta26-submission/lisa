// Test suite for CreatePCS2ITU_ICC() in jpgicc.c
// - Uses only standard C++11 and the provided Little CMS (lcms2) API
// - No Google Test; a small, non-terminating assertion harness is used
// - Explanatory comments accompany each unit test

#include <iccjpeg.h>
#include <jpeglib.h>
#include <iostream>
#include <cstdlib>
#include <lcms2.h>
#include <utils.h>


// Include Little CMS header for access to cmsHPROFILE, cmsSigBToA0Tag, etc.

// Ensure we can reference the focal function from C
extern "C" cmsHPROFILE CreatePCS2ITU_ICC(void);

// Simple non-terminating assertion macro for test continuity
static int gFailCount = 0;
#define T_EXPECT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "TEST_FAIL: " << (msg) << std::endl; \
        ++gFailCount; \
    } else { \
        std::cout << "TEST_PASS: " << (msg) << std::endl; \
    } \
} while (0)

// Helper: retrieve the BToA0 tag (as a pipeline) from a profile
static cmsPipeline* GetBToA0FromProfile(cmsHPROFILE hProfile) {
    if (hProfile == NULL) return NULL;
    void* tag = cmsReadTag(hProfile, cmsSigBToA0Tag);
    return (cmsPipeline*)tag;
}

/*
Step 1 (Conceptual): Candidate Keywords
- BToA0Tag, ColorMap, GRID_POINTS, PCS2ITU, ITU2Lab
- BToA0Tag holds the color pipeline for conversion Lab->ITU or vice versa
- The test focuses on the presence and basic integrity of the created profile
*/

// Test 1: Normal path - ensure CreatePCS2ITU_ICC returns a valid profile with expected properties
// This exercises: successful pipeline creation, tagging, and profile configuration
static void Test_CreatePCS2ITU_ICC_Normally_Returns_Profile_With_Expected_Fields() {
    cmsHPROFILE hProfile = CreatePCS2ITU_ICC();
    T_EXPECT(hProfile != NULL, "CreatePCS2ITU_ICC() should return non-null profile on success");

    if (hProfile != NULL) {
        // Validate core profile metadata set by the function
        int colorSpace = cmsGetColorSpace(hProfile);
        int pcs = cmsGetPCS(hProfile);
        int deviceClass = cmsGetDeviceClass(hProfile);

        T_EXPECT(colorSpace == cmsSigLabData, "Profile ColorSpace should be LabData");
        T_EXPECT(pcs == cmsSigLabData, "Profile PCS should be LabData");
        T_EXPECT(deviceClass == cmsSigColorSpaceClass, "Profile DeviceClass should be ColorSpaceClass");

        // Validate the existence of the BToA0 tag which holds the CLUT/ColorMap
        cmsPipeline* BToA0 = GetBToA0FromProfile(hProfile);
        T_EXPECT(BToA0 != NULL, "BToA0 tag should exist and be a non-null pipeline");

        // If we reach here, clean up resources
        cmsCloseProfile(hProfile);
    }
}

// Test 2: Ensure the BToA0 tag is actually a pipeline handle (sanity check on cast)
static void Test_CreatePCS2ITU_ICC_BToA0Tag_IsPipeline() {
    cmsHPROFILE hProfile = CreatePCS2ITU_ICC();
    T_EXPECT(hProfile != NULL, "CreatePCS2ITU_ICC() should return non-null profile for tag-test");

    if (hProfile != NULL) {
        cmsPipeline* BToA0 = GetBToA0FromProfile(hProfile);
        T_EXPECT(BToA0 != NULL, "BToA0Tag should be retrievable and non-null as a pipeline");

        // Optional: ensure that the pipeline is at least one stage (basic sanity check)
        // Since internal structure is opaque, we rely on non-null pointer as basic sanity
        cmsCloseProfile(hProfile);
    }
}

// Test 3: Repeated calls should yield valid profiles without crashing (basic stability)
static void Test_CreatePCS2ITU_ICC_Stability_On_Repeated_Calls() {
    const int iterations = 3;
    for (int i = 0; i < iterations; ++i) {
        cmsHPROFILE hProfile = CreatePCS2ITU_ICC();
        T_EXPECT(hProfile != NULL, "Repeated call should return non-null profile");

        if (hProfile != NULL) {
            int colorSpace = cmsGetColorSpace(hProfile);
            int pcs = cmsGetPCS(hProfile);
            int dev = cmsGetDeviceClass(hProfile);

            T_EXPECT(colorSpace == cmsSigLabData, "Repeated call: ColorSpace LabData");
            T_EXPECT(pcs == cmsSigLabData, "Repeated call: PCS LabData");
            T_EXPECT(dev == cmsSigColorSpaceClass, "Repeated call: DeviceClass ColorSpaceClass");

            // BToA0 tag presence check
            cmsPipeline* BToA0 = GetBToA0FromProfile(hProfile);
            T_EXPECT(BToA0 != NULL, "Repeated call: BToA0 tag exists");

            cmsCloseProfile(hProfile);
        }
    }
}

// Domain knowledge notes embedded as comments for maintainers
// - This test suite focuses on the observable effects of CreatePCS2ITU_ICC (non-private behavior)
// - We avoid touching static/file-scope helpers and rely on public CMS API
// - No GTest; using non-terminating assertions to maximize coverage

int main() {
    std::cout << "Running tests for CreatePCS2ITU_ICC() in jpgicc.c" << std::endl;

    Test_CreatePCS2ITU_ICC_Normally_Returns_Profile_With_Expected_Fields();
    Test_CreatePCS2ITU_ICC_BToA0Tag_IsPipeline();
    Test_CreatePCS2ITU_ICC_Stability_On_Repeated_Calls();

    if (gFailCount > 0) {
        std::cerr << "Total test failures: " << gFailCount << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "All tests passed." << std::endl;
        return EXIT_SUCCESS;
    }
}