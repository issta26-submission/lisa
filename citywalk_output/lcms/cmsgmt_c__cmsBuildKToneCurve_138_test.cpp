// Lightweight C++11 unit tests for the focal method:
// cmsToneCurve* _cmsBuildKToneCurve(...)
//
// Notes:
// - This test suite relies on LittleCMS (lcms2) being available.
// - It uses a minimal, self-contained test harness (no GoogleTest or GMock).
// - Tests call the internal _cmsBuildKToneCurve function directly by declaring its prototype.
// - Tests aim to cover positive (success) and a negative (failure) path.
// - Tests must be compiled and linked against liblcms2 (e.g., g++ test.cpp -llcms2).
//
// Important: Adjust include/link paths according to your environment.

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>



// Declaration of the focal internal function under test.
// It is not part of the public API, so we declare it here for testing purposes.
// The function signature mirrors the one found in the focal method.
extern "C" cmsToneCurve* _cmsBuildKToneCurve(
    cmsContext        ContextID,
    cmsUInt32Number   nPoints,
    cmsUInt32Number   nProfiles,
    const cmsUInt32Number Intents[],
    const cmsHPROFILE hProfiles[],
    const cmsBool     BPC[],
    const cmsFloat64Number AdaptationStates[],
    cmsUInt32Number   dwFlags);

// Simple non-throwing assertion helpers for test continuity.
#define TEST_ASSERT_TRUE(expr, msg)  do { if (!(expr)) { \
    std::cerr << "[FAIL] " << msg << " (" << #expr << ")" << std::endl; \
} else { std::cout << "[PASS] " << msg << std::endl; } } while (0)

#define TEST_ASSERT_FALSE(expr, msg) TEST_ASSERT_TRUE(!(expr), msg)
#define TEST_ASSERT_NOT_NULL(ptr, msg) TEST_ASSERT_TRUE((ptr) != nullptr, msg)
#define TEST_ASSERT_NULL(ptr, msg) TEST_ASSERT_TRUE((ptr) == nullptr, msg)

static void cleanup_profiles(const cmsHPROFILE hProfiles[], cmsUInt32Number n)
{
    for (cmsUInt32Number i = 0; i < n; ++i) {
        if (hProfiles[i] != nullptr) cmsCloseProfile(hProfiles[i]);
    }
}

// Test 1: Success path - two CMYK profiles with valid device class and color space.
// Expect a non-null K-tone curve that is monotonic.
bool test_buildKToneCurve_success()
{
    // Create two CMYK profiles: input (first) and output (last) profiles.
    cmsHPROFILE inProfile  = cmsCreateCMYKProfile();
    cmsHPROFILE outProfile = cmsCreateCMYKProfile();

    if (inProfile == nullptr || outProfile == nullptr) {
        std::cerr << "[ERROR] Failed to create CMYK profiles for success test." << std::endl;
        cleanup_profiles((const cmsHPROFILE[]){inProfile, outProfile}, 2);
        return false;
    }

    const cmsHPROFILE hProfiles[2] = { inProfile, outProfile };

    cmsUInt32Number Intents[2] = { INTENT_PERCEPTUAL, INTENT_PERCEPTUAL };
    cmsBool BPC[2]        = { FALSE, FALSE };
    cmsFloat64Number AdaptationStates[2] = { 0.0, 0.0 };

    // Call the focal function with a valid context (nullptr is acceptable here).
    cmsToneCurve* KTone = _cmsBuildKToneCurve(nullptr,
                                             256,
                                             2,
                                             Intents,
                                             hProfiles,
                                             BPC,
                                             AdaptationStates,
                                             0);

    // Cleanup
    cleanup_profiles(hProfiles, 2);

    // Assertions
    TEST_ASSERT_NOT_NULL(KTone, "KTone curve should be created (success path).");
    if (KTone != nullptr) {
        int monotonic = cmsIsToneCurveMonotonic(KTone);
        TEST_ASSERT_TRUE(monotonic != 0, "KTone curve should be monotonic.");
        cmsFreeToneCurve(KTone);
    }

    return (KTone != nullptr);
}

// Test 2: Failure path - last profile color space is not CMYK (e.g., RGB).
// Expect NULL to be returned due to color space predicate failing.
bool test_buildKToneCurve_colorSpaceMismatch()
{
    // First profile must be CMYK, last profile will be RGB to trigger color-space check.
    cmsHPROFILE inProfile  = cmsCreateCMYKProfile();
    cmsHPROFILE rgbProfile = cmsCreateSRGBProfile(); // RGB color space

    if (inProfile == nullptr || rgbProfile == nullptr) {
        std::cerr << "[ERROR] Failed to create profiles for color-space-mismatch test." << std::endl;
        cleanup_profiles((const cmsHPROFILE[]){inProfile, rgbProfile}, 2);
        return false;
    }

    const cmsHPROFILE hProfiles[2] = { inProfile, rgbProfile };

    cmsUInt32Number Intents[2] = { INTENT_PERCEPTUAL, INTENT_PERCEPTUAL };
    cmsBool BPC[2]        = { FALSE, FALSE };
    cmsFloat64Number AdaptationStates[2] = { 0.0, 0.0 };

    cmsToneCurve* KTone = _cmsBuildKToneCurve(nullptr,
                                             256,
                                             2,
                                             Intents,
                                             hProfiles,
                                             BPC,
                                             AdaptationStates,
                                             0);

    // Cleanup
    cleanup_profiles(hProfiles, 2);

    // Assert NULL is returned due to color space mismatch
    TEST_ASSERT_NULL(KTone, "KTone curve should be NULL when last profile is not CMYK (color-space check).");
    return (KTone == nullptr);
}

int main()
{
    std::cout << "Starting cmsBuildKToneCurve unit tests (C++11, no GTest)..." << std::endl;

    int passed = 0;
    int total  = 0;

    // Test 1: Success path
    ++total;
    bool t1 = test_buildKToneCurve_success();
    if (t1) ++passed;

    // Test 2: Color-space mismatch path
    ++total;
    bool t2 = test_buildKToneCurve_colorSpaceMismatch();
    if (t2) ++passed;

    std::cout << "Tests passed: " << passed << " / " << total << std::endl;

    return (passed == total) ? 0 : 1;
}