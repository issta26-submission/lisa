// Unit test suite for the focal method: _cmsReadFloatInputTag
// Target: C++11 environment, no Google Test framework.
// Approach: Create minimal, focused tests using the actual LittleCMS public API.
// Note: This test relies on the presence of LittleCMS (lcms2) in the build environment.
//       The tests exercise the function with Lab and XYZ profile spaces to traverse
//       the true branches of the conditional logic inside _cmsReadFloatInputTag.
//       It uses a lightweight, custom test harness (no GTest) and prints results to stdout.

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstdlib>


// Include the public LittleCMS header

extern "C" {
    // Declare the focal function. It is defined in cmsio1.c (not in a header).
    // We declare it with C linkage to avoid name mangling in C++.
    cmsPipeline* _cmsReadFloatInputTag(cmsHPROFILE hProfile, cmsTagSignature tagFloat);
}

// Lightweight assertion helpers
#define ASSERT_TRUE(cond, msg) \
    do { if(!(cond)) { std::cerr << "ASSERT_TRUE FAILED: " << (msg) \
        << " (Condition: " #cond ") at " __FILE__ ":" << __LINE__ << std::endl; return false; } } while(0)

#define ASSERT_NULL(ptr, msg) \
    do { if((ptr) != NULL) { std::cerr << "ASSERT_NULL FAILED: " << (msg) \
        << " (Pointer is non-NULL) at " __FILE__ ":" << __LINE__ << std::endl; return false; } } while(0)

#define ASSERT_NOT_NULL(ptr, msg) \
    do { if((ptr) == NULL) { std::cerr << "ASSERT_NOT_NULL FAILED: " << (msg) \
        << " (Pointer is NULL) at " __FILE__ ":" << __LINE__ << std::endl; return false; } } while(0)

static bool test_ReadFloatInputTag_LabProfile_AToB0_ReturnsNonNull()
{
    // Test: Lab profile space should trigger the "spc == cmsSigLabData" branch
    // and, given a valid AToB0Tag, return a non-NULL pipeline after normalization
    // Note: cmsCreateLab4Profile creates a Lab device profile with PCS likely LabData.

    // Create a Lab profile (Lab device space)
    cmsHPROFILE hLab = cmsCreateLab4Profile(nullptr);
    ASSERT_NOT_NULL(hLab, "Failed to create Lab profile (hLab is NULL).");

    // Call the focal function with a well-known tag that exists in ICC profiles
    // In Lab profiles, the AToB0Tag (cmsSigAToB0Tag) typically contains a transform pipeline.
    cmsTagSignature labTag = cmsSigAToB0Tag;
    cmsPipeline* Lut = _cmsReadFloatInputTag(hLab, labTag);

    // We expect a non-NULL pipeline to indicate success
    ASSERT_NOT_NULL(Lut, "Expected non-NULL Lut for Lab profile with AToB0Tag.");

    // Cleanup
    if (Lut) cmsPipelineFree(Lut);
    cmsCloseProfile(hLab);

    return true;
}

static bool test_ReadFloatInputTag_XYZProfile_AToB0_ReturnsNonNull()
{
    // Test: XYZ profile space should trigger the "spc == cmsSigXYZData" branch
    // and, with a valid AToB0Tag, return a non-NULL pipeline after normalization
    cmsHPROFILE hXYZ = cmsCreateXYZProfile();
    ASSERT_NOT_NULL(hXYZ, "Failed to create XYZ profile (hXYZ is NULL).");

    cmsTagSignature xyzTag = cmsSigAToB0Tag;
    cmsPipeline* Lut = _cmsReadFloatInputTag(hXYZ, xyzTag);

    // Expect non-NULL Lut for a valid XYZ profile and tag
    ASSERT_NOT_NULL(Lut, "Expected non-NULL Lut for XYZ profile with AToB0Tag.");

    if (Lut) cmsPipelineFree(Lut);
    cmsCloseProfile(hXYZ);

    return true;
}

static bool test_ReadFloatInputTag_InvalidTag_ReturnsNULL()
{
    // Test: Passing an invalid tag should result in NULL Lut (failed ReadTag)
    cmsHPROFILE hLab = cmsCreateLab4Profile(nullptr);
    ASSERT_NOT_NULL(hLab, "Failed to create Lab profile (for invalid tag test).");

    // Use an obviously invalid tag value to simulate cmsReadTag failure
    cmsTagSignature invalidTag = (cmsTagSignature)0xFFFFFFFF; // unlikely to be valid
    cmsPipeline* Lut = _cmsReadFloatInputTag(hLab, invalidTag);

    // Expect NULL due to unreadable/non-existent tag
    ASSERT_NULL(Lut, "Expected NULL Lut for invalid tag signature.");

    if (Lut) cmsPipelineFree(Lut);
    cmsCloseProfile(hLab);

    return true;
}

// Entry point for the test suite
int main()
{
    bool all_passed = true;

    std::cout << "Running unit tests for _cmsReadFloatInputTag (lcms2)..." << std::endl;

    if (!test_ReadFloatInputTag_LabProfile_AToB0_ReturnsNonNull()) {
        std::cerr << "Test 1 failed." << std::endl;
        all_passed = false;
    } else {
        std::cout << "Test 1 passed." << std::endl;
    }

    if (!test_ReadFloatInputTag_XYZProfile_AToB0_ReturnsNonNull()) {
        std::cerr << "Test 2 failed." << std::endl;
        all_passed = false;
    } else {
        std::cout << "Test 2 passed." << std::endl;
    }

    if (!test_ReadFloatInputTag_InvalidTag_ReturnsNULL()) {
        std::cerr << "Test 3 failed." << std::endl;
        all_passed = false;
    } else {
        std::cout << "Test 3 passed." << std::endl;
    }

    if (all_passed) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return EXIT_SUCCESS;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return EXIT_FAILURE;
    }
}