// Test suite for BlackPreservingKPlaneIntents in cmscnvrt.c
// This test suite is designed for C++11 without a testing framework like GoogleTest.
// It uses the LittleCMS public API where available and provides lightweight
// non-terminating assertion helpers to maximize code execution coverage.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>


// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

#define TEST_LOG(msg) std::cout << "[TEST] " << msg << std::endl
#define ASSERT(cond, msg) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "[FAILED] " << msg << " (condition: " #cond ")" << std::endl; \
    } \
} while(0)

#define ASSERT_NULL(ptr, msg) ASSERT((ptr) == nullptr, msg)
#define ASSERT_NOT_NULL(ptr, msg) ASSERT((ptr) != nullptr, msg)

// Helper: safely release resources if created
static void safeCloseProfile(cmsHPROFILE p) {
    if (p != nullptr) cmsCloseProfile(p);
}

// Helper: safely free pipeline if created
static void safeFreePipeline(cmsPipeline* p) {
    if (p != nullptr) cmsPipelineFree(p);
}

// Test 1: Sanity check - nProfiles outside [1,255] must return NULL
// This verifies the guard clause at the beginning of BlackPreservingKPlaneIntents.
static bool test_InvalidNProfiles()
{
    TEST_LOG("Test 1: Invalid nProfiles (<1) should return NULL");
    cmsContext ContextID = cmsCreateContext(nullptr, nullptr);
    if (!ContextID) {
        // If context creation fails, skip the test gracefully
        TEST_LOG("Context creation failed; skipping Test 1");
        cmsDeleteContext(ContextID);
        return true;
    }

    // The arrays themselves won't be accessed due to the early return,
    // so we can pass minimal types.
    cmsUInt32Number TheIntents[1] = { 0 };
    cmsHPROFILE hProfiles[1] = { nullptr };
    cmsBool BPC[1] = { 0 };
    cmsFloat64Number AdaptationStates[1] = { 0.0 };

    cmsPipeline* Result = BlackPreservingKPlaneIntents(ContextID,
                                                      0,                 // nProfiles = 0 (invalid)
                                                      TheIntents,
                                                      hProfiles,
                                                      BPC,
                                                      AdaptationStates,
                                                      0);

    ASSERT_NULL(Result, "Expected NULL when nProfiles < 1");
    // Cleanup
    cmsDeleteContext(ContextID);
    return g_failed == 0;
}

// Test 2: Positive path with multiple valid CMYK profiles
// This exercise exercises the main code path when the input is valid CMYK profiles.
// It checks that a non-NULL pipeline is returned.
static bool test_ThreeCMYKProfilesReturnsPipeline()
{
    TEST_LOG("Test 2: Three valid CMYK profiles should yield a non-NULL pipeline");
    cmsContext ContextID = cmsCreateContext(nullptr, nullptr);
    if (!ContextID) {
        TEST_LOG("Context creation failed; skipping Test 2");
        return true;
    }

    // Create three CMYK profiles (basic baseline profiles)
    cmsHPROFILE h1 = cmsCreateCMYKProfile();
    cmsHPROFILE h2 = cmsCreateCMYKProfile();
    cmsHPROFILE h3 = cmsCreateCMYKProfile();

    bool createdAll = (h1 != nullptr) && (h2 != nullptr) && (h3 != nullptr);

    cmsUInt32Number TheIntents[3] = { 0, 0, 0 };
    cmsHPROFILE hProfiles[3] = { h1, h2, h3 };
    cmsBool BPC[3] = { 0, 0, 0 };
    cmsFloat64Number AdaptationStates[3] = { 0.0, 0.0, 0.0 };

    cmsPipeline* Result = NULL;
    if (createdAll) {
        Result = BlackPreservingKPlaneIntents(ContextID,
                                              3,
                                              TheIntents,
                                              hProfiles,
                                              BPC,
                                              AdaptationStates,
                                              0);
        ASSERT_NOT_NULL(Result, "BlackPreservingKPlaneIntents should return a non-NULL pipeline for valid CMYK profiles");
        safeFreePipeline(Result);
    } else {
        TEST_LOG("Failed to create all CMYK profiles; skipping the core test");
        ASSERT(false, " CMYK profile creation failed");
    }

    // Cleanup
    safeCloseProfile(h1);
    safeCloseProfile(h2);
    safeCloseProfile(h3);
    cmsDeleteContext(ContextID);
    return g_failed == 0;
}

// Test 3: Boundary condition - nProfiles = 256 (exceeds maximum allowed by function)
// This should return NULL immediately due to the sanity check.
static bool test_256ProfilesReturnsNULL()
{
    TEST_LOG("Test 3: nProfiles = 256 should return NULL (out of allowed range 1..255)");
    cmsContext ContextID = cmsCreateContext(nullptr, nullptr);
    if (!ContextID) {
        TEST_LOG("Context creation failed; skipping Test 3");
        return true;
    }

    const cmsUInt32Number N = 256;
    cmsUInt32Number TheIntents[256] = {0};
    cmsHPROFILE hProfiles[256];
    // Provide NULL profiles; however, the function should bail out before dereferencing.
    std::memset(hProfiles, 0, sizeof(hProfiles));
    cmsBool BPC[256] = {0};
    cmsFloat64Number AdaptationStates[256];
    std::memset(AdaptationStates, 0, sizeof(AdaptationStates));

    cmsPipeline* Result = BlackPreservingKPlaneIntents(ContextID,
                                                      N,
                                                      TheIntents,
                                                      hProfiles,
                                                      BPC,
                                                      AdaptationStates,
                                                      0);

    ASSERT_NULL(Result, "Expected NULL when nProfiles > 255");
    cmsDeleteContext(ContextID);
    return g_failed == 0;
}

// Main: Run all tests and report final status
int main()
{
    TEST_LOG("Starting BlackPreservingKPlaneIntents unit test suite (C++11, no GTest)");
    // Reset counters
    g_total = 0;
    g_failed = 0;

    bool t1 = test_InvalidNProfiles();
    bool t2 = test_ThreeCMYKProfilesReturnsPipeline();
    bool t3 = test_256ProfilesReturnsNULL();

    // Summary
    std::cout << "========================================" << std::endl;
    std::cout << "Total tests: " << g_total << ", Failures: " << g_failed << std::endl;
    if (g_failed == 0) {
        TEST_LOG("All tests passed successfully.");
        return 0;
    } else {
        TEST_LOG("Some tests failed. See details above.");
        return 1;
    }
}