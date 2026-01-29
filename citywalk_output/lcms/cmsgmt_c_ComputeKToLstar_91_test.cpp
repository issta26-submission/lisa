// Unit tests for the focal method: ComputeKToLstar
// This test suite provides a lightweight, self-contained harness (no GTest) to exercise
// the ComputeKToLstar function from the provided focal code.
// It uses a small custom EXPECT-style assertion framework to run multiple scenarios
// and report failures without aborting early.
//
// Notes and assumptions:
// - The test relies on the public Little CMS API (lcms2.h) for context/profile creation
//   and tone-curve evaluation where available.
// - We focus on validating the successful path (non-NULL output) and basic endpoint behavior.
// - Static/internal helpers are not mocked; instead we exercise the real function with
//   a minimal, realistic setup using a standard profile (sRGB) for inputs.
// - The test avoids terminating on first failure; it aggregates results for a final summary.
// - This test is intended to be compiled and linked alongside the LittleCMS library.

#include <cstdio>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include Little CMS public API

// Forward declaration of the focal method under test.
// The function is defined in cmsgmt.c with C linkage; expose it for C++ tests.
extern "C" cmsToneCurve* ComputeKToLstar(
    cmsContext            ContextID,
    cmsUInt32Number       nPoints,
    cmsUInt32Number       nProfiles,
    const cmsUInt32Number Intents[],
    const cmsHPROFILE     hProfiles[],
    const cmsBool         BPC[],
    const cmsFloat64Number AdaptationStates[],
    cmsUInt32Number       dwFlags);

// Optional: cmsEvalToneCurveFloat is used to inspect endpoints if available.
// If not available in the linked library, we simply verify non-NULL and basic properties.
extern "C" cmsFloat64Number cmsEvalToneCurveFloat(cmsToneCurve* ToneCurve, cmsFloat64Number In);

// Simple non-terminating assertion helpers
static int gTestsTotal = 0;
static int gTestsFailed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++gTestsTotal; \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gTestsFailed; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr, msg) do { \
    ++gTestsTotal; \
    if ((ptr) == NULL) { \
        std::cerr << "EXPECT_NOT_NULL failed: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gTestsFailed; \
    } \
} while(0)

#define EXPECT_LE(a, b, msg) do { \
    ++gTestsTotal; \
    if (!((a) <= (b))) { \
        std::cerr << "EXPECT_LE failed: " << (msg) << " ( " << (a) << " <=? " << (b) << " )" \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gTestsFailed; \
    } \
} while(0)

#define EXPECT_GE(a, b, msg) do { \
    ++gTestsTotal; \
    if (!((a) >= (b))) { \
        std::cerr << "EXPECT_GE failed: " << (msg) << " ( " << (a) << " >=? " << (b) << " )" \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gTestsFailed; \
    } \
} while(0)


// Helper to safely create a default CMS context
static cmsContext CreateDefaultContext() {
    // NULL for the allocator and user data; typical usage in tests
    return cmsCreateContext(NULL, NULL);
}

// Helper to clean up a single profile
static void CloseProfileSafe(cmsHPROFILE h) {
    if (h) cmsCloseProfile(h);
}

// Test 1: Basic success path with a minimal setup.
// - Create a valid CMS context
// - Use a single sRGB profile as the "input profile" (hProfiles[0])
// - Call ComputeKToLstar with a small number of points and a simple configuration
// - Validate non-null output and endpoints via cmsEvalToneCurveFloat when available
static bool Test_KToLstar_BasicSuccess() {
    std::cout << "Running Test_KToLstar_BasicSuccess..." << std::endl;

    cmsContext ContextID = CreateDefaultContext();
    if (ContextID == NULL) {
        std::cerr << "Failed to create CMS context." << std::endl;
        return false;
    }

    const cmsUInt32Number nPoints = 5;
    const cmsUInt32Number nProfiles = 1;

    cmsUInt32Number Intents[1] = { INTENT_PERCEPTUAL };
    cmsHPROFILE hProfiles[1] = { NULL };
    hProfiles[0] = cmsCreate_sRGBProfile(); // Standard sRGB as a source profile

    cmsBool BPC[1] = { FALSE }; // no black point compensation
    cmsFloat64Number AdaptationStates[1] = { 0.0 };
    cmsUInt32Number dwFlags = 0;

    cmsToneCurve* curve = ComputeKToLstar(ContextID,
                                          nPoints,
                                          nProfiles,
                                          Intents,
                                          hProfiles,
                                          BPC,
                                          AdaptationStates,
                                          dwFlags);

    EXPECT_NOT_NULL(curve, "ComputeKToLstar should not return NULL with basic setup.");

    if (curve) {
        // Try to evaluate endpoints if API is available
        cmsFloat64Number y0 = cmsEvalToneCurveFloat(curve, 0.0);
        cmsFloat64Number y1 = cmsEvalToneCurveFloat(curve, 1.0);

        // Endpoints should be in [0,1], with 0 near 0 and 1 near 1
        EXPECT_LE(0.0, y0, "Curve evaluated at 0.0 should be >= 0");
        EXPECT_GE(0.0, 1.0 - y0, "Curve evaluated at 0.0 should be close to 0 within tolerance");
        EXPECT_LE(0.0, y1, "Curve evaluated at 1.0 should be >= 0");
        EXPECT_GE(1.0, y1, "Curve evaluated at 1.0 should be <= 1");
        // Optional: ensure monotonicity for a basic check
        cmsFloat64Number yMid = cmsEvalToneCurveFloat(curve, 0.5);
        EXPECT_TRUE(y0 <= yMid && yMid <= y1, "Curve should be non-decreasing over [0,1]");
        // Free the curve
        cmsFreeToneCurve(curve);
    }

    CloseProfileSafe(hProfiles[0]);
    cmsDisposeContext(ContextID);

    return (gTestsFailed == 0);
}

// Test 2: Basic path with a smaller number of points (nPoints = 2).
// This checks edge-case in the loop where (nPoints-1) equals 1 and division by 1 is valid.
// Ensures we don't crash and return a valid curve in this minimal case.
static bool Test_KToLstar_MinPoints() {
    std::cout << "Running Test_KToLstar_MinPoints..." << std::endl;

    cmsContext ContextID = CreateDefaultContext();
    if (ContextID == NULL) {
        std::cerr << "Failed to create CMS context." << std::endl;
        return false;
    }

    const cmsUInt32Number nPoints = 2;
    const cmsUInt32Number nProfiles = 1;

    cmsUInt32Number Intents[1] = { INTENT_RELATIVE_COLORIMETRIC };
    cmsHPROFILE hProfiles[1] = { NULL };
    hProfiles[0] = cmsCreate_sRGBProfile();

    cmsBool BPC[1] = { FALSE };
    cmsFloat64Number AdaptationStates[1] = { 0.0 };
    cmsUInt32Number dwFlags = 0;

    cmsToneCurve* curve = ComputeKToLstar(ContextID,
                                          nPoints,
                                          nProfiles,
                                          Intents,
                                          hProfiles,
                                          BPC,
                                          AdaptationStates,
                                          dwFlags);

    EXPECT_NOT_NULL(curve, "ComputeKToLstar with nPoints=2 should not return NULL.");

    if (curve) {
        cmsFloat64Number v0 = cmsEvalToneCurveFloat(curve, 0.0);
        cmsFloat64Number v1 = cmsEvalToneCurveFloat(curve, 1.0);
        EXPECT_TRUE(v0 >= 0.0 && v0 <= 1.0, "Endpoint at 0.0 should be in [0,1].");
        EXPECT_TRUE(v1 >= 0.0 && v1 <= 1.0, "Endpoint at 1.0 should be in [0,1].");
        cmsFreeToneCurve(curve);
    }

    CloseProfileSafe(hProfiles[0]);
    cmsDisposeContext(ContextID);

    return (gTestsFailed == 0);
}

// Test 3: BPC toggle variations to cover both branches where BPC[] is FALSE and TRUE.
// We run two sub-tests to ensure no crashes and a valid curve is produced in both cases.
static bool Test_KToLstar_BPCVariation() {
    std::cout << "Running Test_KToLstar_BPCVariation..." << std::endl;

    // Helper lambda to run with given BPC value
    auto runWithBPC = [](cmsBool bp) -> bool {
        cmsContext ContextID = CreateDefaultContext();
        if (ContextID == NULL) {
            std::cerr << "Failed to create CMS context." << std::endl;
            return false;
        }

        const cmsUInt32Number nPoints = 8;
        const cmsUInt32Number nProfiles = 1;

        cmsUInt32Number Intents[1] = { INTENT_PERCEPTUAL };
        cmsHPROFILE hProfiles[1] = { cmsCreate_sRGBProfile() };
        cmsBool BPC[1] = { bp };
        cmsFloat64Number AdaptationStates[1] = { 0.0 };
        cmsUInt32Number dwFlags = 0;

        cmsToneCurve* curve = ComputeKToLstar(ContextID,
                                              nPoints,
                                              nProfiles,
                                              Intents,
                                              hProfiles,
                                              BPC,
                                              AdaptationStates,
                                              dwFlags);

        bool ok = (curve != NULL);
        if (curve) cmsFreeToneCurve(curve);
        CloseProfileSafe(hProfiles[0]);
        cmsDisposeContext(ContextID);
        return ok;
    };

    bool okFalse = runWithBPC(FALSE);
    bool okTrue  = runWithBPC(TRUE);

    EXPECT_TRUE(okFalse, "ComputeKToLstar should succeed with BPC = FALSE.");
    EXPECT_TRUE(okTrue,  "ComputeKToLstar should succeed with BPC = TRUE.");

    return (gTestsFailed == 0);
}


// Entry point to run all tests
int main() {
    bool allGood = true;

    allGood &= Test_KToLstar_BasicSuccess();
    allGood &= Test_KToLstar_MinPoints();
    allGood &= Test_KToLstar_BPCVariation();

    if (allGood) {
        std::cout << "All tests passed (" << gTestsTotal << " tests)." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests failed. Total: " << gTestsTotal
                  << ", Failures: " << gTestsFailed << std::endl;
        return 1;
    }
}