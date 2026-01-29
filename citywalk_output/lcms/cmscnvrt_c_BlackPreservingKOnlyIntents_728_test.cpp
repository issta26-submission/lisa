// TEST SUITE for BlackPreservingKOnlyIntents in cmscnvrt.c
// - No GTest/GMock usage (per instructions).
// - Uses lightweight, non-terminating EXPECT_* style assertions.
// - Assumes LittleCMS-like environment is available (lcms2.h, etc.).
// - Focuses on two basic, deterministic test scenarios and parameter validation.
// - Comments explain each test and reasoning for chosen inputs.

#include <cstdio>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include LittleCMS headers. Adjust include paths as necessary in your build system.

// Prototype of focal function (external linkage). The actual function is defined in cmscnvrt.c.
// We declare it here so tests can call it from this translation unit.
extern "C" cmsPipeline* BlackPreservingKOnlyIntents(cmsContext     ContextID,
                                                 cmsUInt32Number nProfiles,
                                                 cmsUInt32Number TheIntents[],
                                                 cmsHPROFILE     hProfiles[],
                                                 cmsBool         BPC[],
                                                 cmsFloat64Number AdaptationStates[],
                                                 cmsUInt32Number dwFlags);

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        fprintf(stderr, "TEST FAILURE: %s\n", msg); \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr, msg) do { \
    ++g_tests_run; \
    if ((ptr) == NULL) { \
        fprintf(stderr, "TEST FAILURE: %s (pointer is NULL)\n", msg); \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_NULL(ptr, msg) do { \
    ++g_tests_run; \
    if ((ptr) != NULL) { \
        fprintf(stderr, "TEST FAILURE: %s (pointer is NOT NULL)\n", msg); \
        ++g_tests_failed; \
    } \
} while(0)

// Helper: create a CMYK profile for testing
static cmsHPROFILE CreateCMYKProfile()
{
    // cmsCreateCMYKProfile() returns a new, in-memory CMYK profile
    return cmsCreateCMYKProfile();
}

// Helper: clean up profiles safely
static void CloseProfiles(cmsHPROFILE* profs, cmsUInt32Number n)
{
    if (profs == nullptr) return;
    for (cmsUInt32Number i = 0; i < n; ++i) {
        if (profs[i] != NULL) cmsCloseProfile(profs[i]);
        profs[i] = NULL;
    }
}

// Test 1: Parameter validation for nProfiles bounds
// - Ensure function returns NULL when nProfiles is 0 or > 255.
static void test_nProfilesValidation_returnsNull()
{
    printf("Running test_nProfilesValidation_returnsNull...\n");

    // Use a couple of CMYK profiles; they won't be reached due to early return.
    cmsHPROFILE hProfiles[2] = { NULL, NULL };
    hProfiles[0] = CreateCMYKProfile();
    hProfiles[1] = CreateCMYKProfile();

    cmsUInt32Number TheIntents[1] = { 0 };
    cmsBool BPC[1] = { 0 };
    cmsFloat64Number AdaptationStates[1] = { 1.0 };

    // Case 1: nProfiles = 0 -> should return NULL
    cmsPipeline* p = BlackPreservingKOnlyIntents(NULL, 0, TheIntents, hProfiles, BPC, AdaptationStates, 0);
    EXPECT_NULL(p, "nProfiles = 0 should return NULL");

    // Case 2: nProfiles = 256 -> should return NULL
    p = BlackPreservingKOnlyIntents(NULL, 256, TheIntents, hProfiles, BPC, AdaptationStates, 0);
    EXPECT_NULL(p, "nProfiles = 256 should return NULL");

    // Cleanup
    CloseProfiles(hProfiles, 2);
}

// Test 2: Basic valid CMYK path returns a non-null pipeline
// - Use a simple, valid CMYK-only setup to reach the main path.
// - Verifies that a pipeline is produced (not NULL) for a minimal, valid input.
static void test_BasicCMYKPath_returnsNonNull()
{
    printf("Running test_BasicCMYKPath_returnsNonNull...\n");

    cmsHPROFILE hProfiles[1] = { NULL };
    hProfiles[0] = CreateCMYKProfile();

    // The Intents array uses a typical ICC-compatible intent value.
    // Canonical values for intents in LittleCMS: 0=INTENT_PERCEPTUAL, etc.
    cmsUInt32Number TheIntents[1] = { 0 }; // Perceptual
    cmsBool BPC[1] = { 0 };                 // Black Point Compensation off
    cmsFloat64Number AdaptationStates[1] = { 1.0 };

    cmsPipeline* p = BlackPreservingKOnlyIntents(NULL, 1, TheIntents, hProfiles, BPC, AdaptationStates, 0);

    EXPECT_NOT_NULL(p, "BlackPreservingKOnlyIntents should return a non-NULL pipeline for a valid CMYK single-profile input");

    // Cleanup: free pipeline and profile
    if (p) cmsPipelineFree(p);
    cmsCloseProfile(hProfiles[0]);
}

// Test 3: Basic multi-profile CMYK path
// - Use two CMYK profiles to exercise multi-profile handling.
// - Verifies that a pipeline is produced (non-NULL) when multiple CMYK profiles are provided.
static void test_MultiCMYKPath_returnsNonNull()
{
    printf("Running test_MultiCMYKPath_returnsNonNull...\n");

    cmsHPROFILE hProfiles[2] = { NULL, NULL };
    hProfiles[0] = CreateCMYKProfile();
    hProfiles[1] = CreateCMYKProfile();

    cmsUInt32Number TheIntents[2] = { 0, 0 }; // Perceptual for both profiles
    cmsBool BPC[2] = { 0, 0 };
    cmsFloat64Number AdaptationStates[2] = { 1.0, 1.0 };

    cmsPipeline* p = BlackPreservingKOnlyIntents(NULL, 2, TheIntents, hProfiles, BPC, AdaptationStates, 0);

    EXPECT_NOT_NULL(p, "BlackPreservingKOnlyIntents should return a non-NULL pipeline for multiple CMYK profiles");

    if (p) cmsPipelineFree(p);
    cmsCloseProfile(hProfiles[0]);
    cmsCloseProfile(hProfiles[1]);
}

// Main: run tests
int main()
{
    fprintf(stdout, "Starting BlackPreservingKOnlyIntents test suite (C++11, without GTest)...\n");

    test_nProfilesValidation_returnsNull();
    test_BasicCMYKPath_returnsNonNull();
    test_MultiCMYKPath_returnsNonNull();

    fprintf(stdout, "Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
    if (g_tests_failed) {
        fprintf(stderr, "Some tests FAILED.\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "All tests PASSED (as far as deterministic checks could verify).\n");
    return EXIT_SUCCESS;
}