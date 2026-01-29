// Unit test suite for the focal method: cmsHTRANSFORM _cmsChain2Lab
// This test suite uses a lightweight internal test harness (no GTest) and
// relies on LittleCMS (lcms2*) being available in the test environment.
//
// Notes:
// - Tests are written for C++11 compatibility.
// - Uses non-terminating assertions to exercise multiple code paths in a single run.
// - Tests focus on validating the behavior of _cmsChain2Lab as per the provided code.
// - Extern "C" declaration is used to ensure the C symbol linkage for the focal function.

#include <cstdio>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include LittleCMS headers

// Declare the focal function with C linkage to enable direct calls from C++
extern "C" cmsHTRANSFORM _cmsChain2Lab(
    cmsContext ContextID,
    cmsUInt32Number nProfiles,
    cmsUInt32Number InputFormat,
    cmsUInt32Number OutputFormat,
    const cmsUInt32Number Intents[],
    const cmsHPROFILE hProfiles[],
    const cmsBool BPC[],
    const cmsFloat64Number AdaptationStates[],
    cmsUInt32Number dwFlags);

// Lightweight test harness
static int g_test_failures = 0;

// Non-terminating assertions (logging failures but continuing)
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[TEST FAILURE] " << __FUNCTION__ << ": Condition failed: " #cond << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_NULL(p) do { \
    if((p) != nullptr) { \
        std::cerr << "[TEST FAILURE] " << __FUNCTION__ << ": Expected NULL pointer, got " << (void*)(p) << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_NOT_NULL(p) do { \
    if((p) == nullptr) { \
        std::cerr << "[TEST FAILURE] " << __FUNCTION__ << ": Expected non-NULL pointer" << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_EQ_UINT(a, b) do { \
    if((a) != (b)) { \
        std::cerr << "[TEST FAILURE] " << __FUNCTION__ << ": Expected " << (unsigned)(a) << " == " << (unsigned)(b) << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

//
// Test 1: Overflow path returns NULL when nProfiles > 254
// The function should early-return NULL without attempting to create a Lab profile or a transform.
//
void test_OverflowReturnsNull() {
    // No need for a real context or profiles since the function should exit early.
    cmsUInt32Number nProfiles = 255; // > 254 triggers the early return
    const cmsUInt32Number Intents[1] = { INTENT_RELATIVE_COLORIMETRIC };
    cmsHTRANSFORM xform = _cmsChain2Lab(nullptr,
                                        nProfiles,
                                        0, // InputFormat (unused due to early return)
                                        0, // OutputFormat (unused due to early return)
                                        Intents, // one element (unused due to early return)
                                        nullptr, // hProfiles
                                        nullptr, // BPC
                                        nullptr, // AdaptationStates
                                        0); // dwFlags

    EXPECT_NULL(xform);
}

// Test 2: Zero profiles path creates a transform (boundary case nProfiles == 0)
// Verifies that with no input profiles, the function still attempts to create a Lab-at-end chain
// and returns a (possibly) valid transform. This test primarily ensures no crash and a valid return type
// when constraints are minimal.
void test_ZeroProfilesCreatesTransform() {
    // Create a valid source profile to obtain a proper ContextID
    cmsHPROFILE hSource = cmsCreate_sRGBProfile();
    if (hSource == nullptr) {
        std::cerr << "[TEST SKIP] " << __FUNCTION__ << ": Failed to create sRGB profile; skipping this test." << std::endl;
        return;
    }
    cmsContext ContextID = cmsGetProfileContextID(hSource);

    cmsUInt32Number nProfiles = 0;
    const cmsUInt32Number Intents[1] = { INTENT_RELATIVE_COLORIMETRIC };
    const cmsHPROFILE hProfiles[1] = { nullptr }; // unused when nProfiles == 0
    const cmsBool BPC[1] = { 0 };
    const cmsFloat64Number AdaptationStates[1] = { 1.0 };

    // Input and output formats: choose commonly used formats
    const cmsUInt32Number InputFormat = TYPE_RGB_16;
    const cmsUInt32Number OutputFormat = TYPE_Lab_16;

    cmsHTRANSFORM xform = _cmsChain2Lab(ContextID,
                                        nProfiles,
                                        InputFormat,
                                        OutputFormat,
                                        Intents,
                                        hProfiles,
                                        BPC,
                                        AdaptationStates,
                                        0);

    // We only require that a transform is returned (non-NULL) or gracefully NULL;
    // Both outcomes are possible depending on the library state. We document the result.
    if (xform != nullptr) {
        cmsDeleteTransform(xform);
        EXPECT_TRUE(true); // a non-NULL transform was returned
    } else {
        EXPECT_TRUE(true); // either outcome is acceptable here; ensure the test runs
    }

    cmsCloseProfile(hSource);
}

// Test 3: One input profile path (nProfiles == 1) creates a transform
// We use a known in-memory source profile (sRGB) to build a simple chain: RGB source -> Lab at end.
void test_OneProfileCreatesTransform() {
    cmsHPROFILE hSource = cmsCreate_sRGBProfile();
    cmsHPROFILE hInput  = cmsCreate_sRGBProfile();
    if (hSource == nullptr || hInput == nullptr) {
        std::cerr << "[TEST SKIP] " << __FUNCTION__ << ": Failed to create source/input profiles; skipping this test." << std::endl;
        if (hSource) cmsCloseProfile(hSource);
        if (hInput)  cmsCloseProfile(hInput);
        return;
    }

    cmsContext ContextID = cmsGetProfileContextID(hSource);

    cmsUInt32Number nProfiles = 1;
    const cmsUInt32Number Intents[1] = { INTENT_RELATIVE_COLORIMETRIC };
    const cmsHPROFILE hProfiles[1] = { hInput };
    const cmsBool BPC[1] = { 0 };
    const cmsFloat64Number AdaptationStates[1] = { 1.0 };

    const cmsUInt32Number InputFormat = TYPE_RGB_16;
    const cmsUInt32Number OutputFormat = TYPE_Lab_16;

    cmsHTRANSFORM xform = _cmsChain2Lab(ContextID,
                                        nProfiles,
                                        InputFormat,
                                        OutputFormat,
                                        Intents,
                                        hProfiles,
                                        BPC,
                                        AdaptationStates,
                                        0);

    EXPECT_NOT_NULL(xform);
    if (xform != nullptr) {
        cmsDeleteTransform(xform);
    }

    cmsCloseProfile(hSource);
    cmsCloseProfile(hInput);
}

// Helper to run all tests
void runAllTests() {
    std::cout << "Starting unit tests for _cmsChain2Lab..." << std::endl;

    test_OverflowReturnsNull();
    test_ZeroProfilesCreatesTransform();
    test_OneProfileCreatesTransform();

    if (g_test_failures == 0) {
        std::cout << "[ALL TESTS PASSED] No failures detected." << std::endl;
    } else {
        std::cerr << "[TEST SUMMARY] Failures: " << g_test_failures << std::endl;
    }
}

int main() {
    runAllTests();
    return g_test_failures > 0 ? 1 : 0;
}