#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Forward declaration of the focal function under test.
// It has C linkage in the cmsio1.c file, so we declare it extern "C" here.
extern "C" cmsPipeline* BuildGrayOutputPipeline(cmsHPROFILE hProfile);

/*
 * Lightweight test suite for BuildGrayOutputPipeline in cmsio1.c
 * Strategy:
 * - Create real in-memory ICC profiles using LittleCMS APIs.
 * - Ensure GrayTRCTag exists by building a simple tone curve and assigning it to the profile.
 * - Test two code paths by using Lab-data PCS vs non-Lab PCS (SRGB profile).
 * - Test the NULL-return path by removing the GrayTRCTag and ensuring BuildGrayOutputPipeline returns NULL.
 * - Use a simple, non-terminating assertion style: print failures but continue with other tests.
 *
 * Note: This test relies on the presence of the LittleCMS library (lcms2). It targets C++11 compilers.
 */

// Helper: create a simple 256-point gamma-like tone curve (identity-like)
static cmsToneCurve* CreateIdentityGrayTRC(cmsContext ContextID)
{
    const cmsUInt32Number N = 256;
    cmsUInt16Number table[N];
    for (cmsUInt32Number i = 0; i < N; ++i) {
        // map 0..65535 linearly for a simple monotonic curve
        table[i] = (cmsUInt16Number)((i * 65535) / (N - 1));
    }
    // cmsBuildToneCurve(ContextID, nPoints, table)
    return cmsBuildToneCurve(ContextID, N, table);
}

// Helper: Run a single test and return whether it passed
static bool RunTest(const std::string& name, bool condition)
{
    if (condition) {
        std::cout << "[PASS] " << name << "\n";
        return true;
    } else {
        std::cout << "[FAIL] " << name << "\n";
        return false;
    }
}

// Test 1: Lab PCS should yield a non-NULL pipeline
static bool Test_BuildGrayOutputPipeline_LabPCS()
{
    // Create a Lab profile
    cmsHPROFILE labProfile = cmsCreateLab4Profile(NULL);
    if (labProfile == NULL) {
        std::cerr << "Could not create Lab profile for test.\n";
        return false;
    }

    // Build a simple GrayTRC and assign to the profile
    cmsContext ctx = cmsGetProfileContextID(labProfile);
    cmsToneCurve* GrayTRC = CreateIdentityGrayTRC(ctx);
    if (GrayTRC == NULL) {
        std::cerr << "Failed to create GrayTRC for Lab profile.\n";
        cmsCloseProfile(labProfile);
        return false;
    }

    if (!cmsSetTag(labProfile, cmsSigGrayTRCTag, (void*)GrayTRC)) {
        std::cerr << "Failed to set GrayTRCTag on Lab profile.\n";
        cmsFreeToneCurve(GrayTRC);
        cmsCloseProfile(labProfile);
        return false;
    }
    // Note: BuildGrayOutputPipeline should not take ownership of GrayTRC pointer here;
    // The library will free the RevGrayTRC internally and keep the provided tag data.

    // Call the focal method
    cmsPipeline* lut = BuildGrayOutputPipeline(labProfile);

    // Cleanup: Remove the tag and profile; pipeline may still use the tag data internally
    cmsDeleteTag(labProfile, cmsSigGrayTRCTag);
    cmsCloseProfile(labProfile);
    if (lut) cmsPipelineFree(lut);

    return lut != NULL;
}

// Test 2: Non-Lab PCS (e.g., SRGB/XYZ-based) should yield a non-NULL pipeline
static bool Test_BuildGrayOutputPipeline_SRGBPCS()
{
    // Create an sRGB profile
    cmsHPROFILE srgbProfile = cmsCreateSRGBProfile();
    if (srgbProfile == NULL) {
        std::cerr << "Could not create SRGB profile for test.\n";
        return false;
    }

    // Build a simple GrayTRC and assign to the profile
    cmsContext ctx = cmsGetProfileContextID(srgbProfile);
    cmsToneCurve* GrayTRC = CreateIdentityGrayTRC(ctx);
    if (GrayTRC == NULL) {
        std::cerr << "Failed to create GrayTRC for SRGB profile.\n";
        cmsCloseProfile(srgbProfile);
        return false;
    }

    if (!cmsSetTag(srgbProfile, cmsSigGrayTRCTag, (void*)GrayTRC)) {
        std::cerr << "Failed to set GrayTRCTag on SRGB profile.\n";
        cmsFreeToneCurve(GrayTRC);
        cmsCloseProfile(srgbProfile);
        return false;
    }

    // Call the focal method
    cmsPipeline* lut = BuildGrayOutputPipeline(srgbProfile);

    // Cleanup
    cmsDeleteTag(srgbProfile, cmsSigGrayTRCTag);
    cmsCloseProfile(srgbProfile);
    if (lut) cmsPipelineFree(lut);

    return lut != NULL;
}

// Test 3: If GrayTRCTag is missing, function should return NULL
static bool Test_BuildGrayOutputPipeline_MissingGrayTRCTag()
{
    // Create a Lab profile
    cmsHPROFILE labProfile = cmsCreateLab4Profile(NULL);
    if (labProfile == NULL) {
        std::cerr << "Could not create Lab profile for missing-tag test.\n";
        return false;
    }

    // Build and set a GrayTRC, then immediately remove the tag
    cmsContext ctx = cmsGetProfileContextID(labProfile);
    cmsToneCurve* GrayTRC = CreateIdentityGrayTRC(ctx);
    if (GrayTRC == NULL) {
        std::cerr << "Failed to create GrayTRC for missing-tag test.\n";
        cmsCloseProfile(labProfile);
        return false;
    }

    if (!cmsSetTag(labProfile, cmsSigGrayTRCTag, (void*)GrayTRC)) {
        std::cerr << "Failed to set GrayTRCTag for missing-tag test.\n";
        cmsFreeToneCurve(GrayTRC);
        cmsCloseProfile(labProfile);
        return false;
    }

    // Now remove the tag to simulate missing GrayTRCTag
    cmsDeleteTag(labProfile, cmsSigGrayTRCTag);

    // Call the focal method
    cmsPipeline* lut = BuildGrayOutputPipeline(labProfile);

    // Cleanup
    cmsCloseProfile(labProfile);
    if (lut) cmsPipelineFree(lut);

    // Expect NULL when GrayTRCTag is missing
    return lut == NULL;
}

int main()
{
    std::cout << "Starting tests for BuildGrayOutputPipeline (cmsio1.c) ...\n";

    int passed = 0;
    int total = 0;

    // Test 1
    total++;
    if (RunTest("BuildGrayOutputPipeline with Lab PCS", Test_BuildGrayOutputPipeline_LabPCS()))
        ++passed;

    // Test 2
    total++;
    if (RunTest("BuildGrayOutputPipeline with SRGB (non-Lab) PCS", Test_BuildGrayOutputPipeline_SRGBPCS()))
        ++passed;

    // Test 3
    total++;
    if (RunTest("BuildGrayOutputPipeline returns NULL when GrayTRCTag is missing", Test_BuildGrayOutputPipeline_MissingGrayTRCTag()))
        ++passed;

    std::cout << "Tests completed: " << passed << " / " << total << " passed.\n";

    // Return non-zero if some tests failed
    return (passed == total) ? 0 : 1;
}