// High-quality C++11 unit tests for BuildGrayInputMatrixPipeline using the public CMS API.
// No GTest, a lightweight test harness is implemented with non-terminating assertions.
// The tests rely on the LittleCMS (lcms2) library being available (link with -llcms2).

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include LittleCMS headers (C API). Guarded for C++ compilation.

// Extern declaration for the focal method under test.
// It is assumed to be exported by the library.
extern "C" cmsPipeline* BuildGrayInputMatrixPipeline(cmsHPROFILE hProfile);

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "TEST FAILURE: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr) != nullptr, msg)
#define EXPECT_NULL(ptr, msg) EXPECT_TRUE((ptr) == nullptr, msg)


// Test 1: Ensure the function returns NULL when GrayTRCTag is not present
static void Test_NullGrayTRC()
{
    // Create a Lab profile (PCS == Lab) but deliberately do not set GrayTRCTag
    cmsHPROFILE hLab = cmsCreateLab4Profile(NULL);
    if (hLab == NULL) {
        std::cerr << "Failed to create Lab profile for Test_NullGrayTRC" << std::endl;
        ++g_failures;
        return;
    }

    cmsPipeline* lut = BuildGrayInputMatrixPipeline(hLab);

    EXPECT_NULL(lut, "BuildGrayInputMatrixPipeline should return NULL when GrayTRCTag is missing");

    // Cleanup
    if (lut) cmsPipelineFree(lut);
    cmsCloseProfile(hLab);
}


// Test 2: Lab branch path: GrayTRC present; PCS == LabData
// Expected: BuildGrayInputMatrixPipeline should return a non-NULL pipeline
static void Test_LabBranch()
{
    // Create Lab profile and attach GrayTRC and an EmptyTab (as per the function's Lab path)
    cmsHPROFILE hLab = cmsCreateLab4Profile(NULL);
    if (hLab == NULL) {
        std::cerr << "Failed to create Lab profile for Test_LabBranch" << std::endl;
        ++g_failures;
        return;
    }

    // Obtain context from the profile (as used by the CMS tone curve builder)
    cmsContext ContextID = cmsGetProfileContextID(hLab);

    // Prepare two tone curves: GrayTRC for LabCurves[0], and an EmptyTab for LabCurves[1,2]
    cmsUInt16Number Zero[2] = { 0x8080, 0x8080 };
    cmsToneCurve* GrayTRC = cmsBuildTabulatedToneCurve16(ContextID, 2, Zero);
    if (GrayTRC == NULL) {
        std::cerr << "Failed to create GrayTRC for Test_LabBranch" << std::endl;
        ++g_failures;
        cmsCloseProfile(hLab);
        return;
    }

    cmsToneCurve* EmptyTab = cmsBuildTabulatedToneCurve16(ContextID, 2, Zero);
    if (EmptyTab == NULL) {
        std::cerr << "Failed to create EmptyTab for Test_LabBranch" << std::endl;
        cmsFreeToneCurve(GrayTRC);
        ++g_failures;
        cmsCloseProfile(hLab);
        return;
    }

    // Write the GrayTRCTag into the profile
    if (!cmsWriteTag(hLab, cmsSigGrayTRCTag, (void*)GrayTRC)) {
        std::cerr << "Failed to write GrayTRCTag for Test_LabBranch" << std::endl;
        cmsFreeToneCurve(GrayTRC);
        cmsFreeToneCurve(EmptyTab);
        ++g_failures;
        cmsCloseProfile(hLab);
        return;
    }

    // Note: We intentionally do not free GrayTRC here; the profile owns it now.
    // The EmptyTab is a temporary curve used by the Lab path and will be freed by the library
    // inside BuildGrayInputMatrixPipeline if needed, but we avoid freeing it here to prevent
    // double-free in case the function takes ownership unexpectedly.

    // Build the input matrix pipeline
    cmsPipeline* Lut = BuildGrayInputMatrixPipeline(hLab);
    EXPECT_NOT_NULL(Lut, "BuildGrayInputMatrixPipeline should return a non-NULL pipeline when GrayTRCTag is present and PCS is Lab");

    // Cleanup: free the pipeline and the profile
    if (Lut) cmsPipelineFree(Lut);

    // Free the temporary GrayTRC and EmptyTab. If the profile still owns GrayTRC, cmsCloseProfile
    // will free it as well; attempting to free it again would be wrong.
    cmsFreeToneCurve(EmptyTab);

    cmsCloseProfile(hLab);
}


// Test 3: Non-Lab path (e.g., sRGB): GrayTRC present; PCS != LabData
// Expected: BuildGrayInputMatrixPipeline should return a non-NULL pipeline
static void Test_NonLabBranch()
{
    // Create a standard sRGB profile (PCS is not Lab)
    cmsHPROFILE hRGB = cmsCreate_sRGBProfile();
    if (hRGB == NULL) {
        std::cerr << "Failed to create sRGB profile for Test_NonLabBranch" << std::endl;
        ++g_failures;
        return;
    }

    // Obtain context from the profile
    cmsContext ContextID = cmsGetProfileContextID(hRGB);
    cmsUInt16Number Zero[2] = { 0x8080, 0x8080 };

    // Create GrayTRC and attach to the profile
    cmsToneCurve* GrayTRC = cmsBuildTabulatedToneCurve16(ContextID, 2, Zero);
    if (GrayTRC == NULL) {
        std::cerr << "Failed to create GrayTRC for Test_NonLabBranch" << std::endl;
        ++g_failures;
        cmsCloseProfile(hRGB);
        return;
    }

    if (!cmsWriteTag(hRGB, cmsSigGrayTRCTag, (void*)GrayTRC)) {
        std::cerr << "Failed to write GrayTRCTag for Test_NonLabBranch" << std::endl;
        cmsFreeToneCurve(GrayTRC);
        ++g_failures;
        cmsCloseProfile(hRGB);
        return;
    }

    // Build the input matrix pipeline
    cmsPipeline* Lut = BuildGrayInputMatrixPipeline(hRGB);
    EXPECT_NOT_NULL(Lut, "BuildGrayInputMatrixPipeline should return a non-NULL pipeline for non-Lab PCS when GrayTRCTag is present");

    // Cleanup
    if (Lut) cmsPipelineFree(Lut);

    // Free GrayTRC; profile will still own the tag data; this is safe since we are freeing independent
    cmsFreeToneCurve(GrayTRC);

    cmsCloseProfile(hRGB);
}


// Main test runner
int main()
{
    std::cout << "Running BuildGrayInputMatrixPipeline unit tests (no GTest)..." << std::endl;

    Test_NullGrayTRC();
    Test_LabBranch();
    Test_NonLabBranch();

    if (g_failures != 0) {
        std::cout << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}