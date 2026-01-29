// Unit test suite for cmsHTRANSFORM CreateRoundtripXForm in cmssamp.c
// This test suite is written for C++11, without GoogleTest, and uses only
// the standard library and the provided CMS (lcms2) APIs.
// The tests exercise the focal function by creating real CMS profiles and
// validating basic transform behavior, ensuring resource cleanup and
// coverage of different Intent parameter values.
//
// How to run (example):
// g++ -std=c++11 -I<include_path> -L<lib_path> -llcms2 -o cms_roundtrip_xform_test cms_roundtrip_xform_test.cpp
// ./cms_roundtrip_xform_test
//
// Notes:
// - The focal function is provided as C linkage in the original file.
// - We declare it here with extern "C" to avoid name mangling when linking from C++.
// - We do not rely on any test framework; instead we implement a tiny harness that
//   reports successes/failures and continues to exercise other tests.

#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include the LittleCMS header. Adjust include path if needed.

// Declare the focal function with C linkage to avoid name mangling when linking.
extern "C" cmsHTRANSFORM CreateRoundtripXForm(cmsHPROFILE hProfile, cmsUInt32Number nIntent);

// Simple non-terminating assertion helpers
static int g_total = 0;
static int g_passed = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++g_total; \
    if (cond) { \
        ++g_passed; \
    } else { \
        std::cerr << "Assertion failed: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while (0)

// Helper: Transform a Lab input through the provided transform and verify L is in [0,100]
static bool transform_lab_and_validate_L(cmsHTRANSFORM xform) {
    if (xform == nullptr) return false;

    cmsCIELab inLab, outLab;
    inLab.L = 50.0;
    inLab.a = 0.0;
    inLab.b = 0.0;

    cmsUInt32Number n = 1;
    if (cmsDoTransform(xform, &inLab, &outLab, n) != 1) {
        return false;
    }

    // Sanity check: Out L should be within a reasonable Lab range [0,100]
    if (!(outLab.L >= 0.0 && outLab.L <= 100.0)) {
        return false;
    }

    // Additional rough check for finite numeric values
    if (!std::isfinite(outLab.L) || !std::isfinite(outLab.a) || !std::isfinite(outLab.b)) {
        return false;
    }

    return true;
}

// Test 1: Basic functionality with sRGB profile and INTENT_RELATIVE_COLORIMETRIC
// Expectation: CreateRoundtripXForm returns a non-null transform and this transform can process a Lab value.
static void test_CreateRoundtripXForm_basic_valid_input() {
    std::cout << "Running test_CreateRoundtripXForm_basic_valid_input..." << std::endl;

    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    TEST_ASSERT(hProfile != nullptr, "Failed to create sRGB profile");

    cmsUInt32Number intent = INTENT_RELATIVE_COLORIMETRIC;
    cmsHTRANSFORM xform = CreateRoundtripXForm(hProfile, intent);

    // Validate: transform should be created
    TEST_ASSERT(xform != nullptr, "CreateRoundtripXForm returned NULL transform for sRGB profile");

    // Validate that the transform can be used to transform a Lab value
    bool ok = transform_lab_and_validate_L(xform);
    TEST_ASSERT(ok, "Transform via CreateRoundtripXForm did not produce valid Lab output");

    if (xform) cmsDeleteTransform(xform);
    if (hProfile) cmsCloseProfile(hProfile);

    std::cout << "Finished test_CreateRoundtripXForm_basic_valid_input." << std::endl;
}

// Test 2: Different nIntent values should yield independent transforms and still be usable
// We'll create two transforms with different intents and verify both can process Lab data.
static void test_CreateRoundtripXForm_different_intents_independent_transforms() {
    std::cout << "Running test_CreateRoundtripXForm_different_intents_independent_transforms..." << std::endl;

    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    TEST_ASSERT(hProfile != nullptr, "Failed to create sRGB profile for Test 2");

    // First transform with INTENT_RELATIVE_COLORIMETRIC
    cmsUInt32Number intent1 = INTENT_RELATIVE_COLORIMETRIC;
    cmsHTRANSFORM t1 = CreateRoundtripXForm(hProfile, intent1);
    TEST_ASSERT(t1 != nullptr, "First CreateRoundtripXForm failed for Test 2");

    // Second transform with a different intent (e.g., INTENT_PERCEPTUAL)
    cmsUInt32Number intent2 = INTENT_PERCEPTUAL;
    cmsHTRANSFORM t2 = CreateRoundtripXForm(hProfile, intent2);
    TEST_ASSERT(t2 != nullptr, "Second CreateRoundtripXForm failed for Test 2");

    // Validate both transforms can process Lab values
    bool ok1 = transform_lab_and_validate_L(t1);
    bool ok2 = transform_lab_and_validate_L(t2);

    TEST_ASSERT(ok1, "First transform in Test 2 failed to process Lab");
    TEST_ASSERT(ok2, "Second transform in Test 2 failed to process Lab");

    // Cleanup
    if (t1) cmsDeleteTransform(t1);
    if (t2) cmsDeleteTransform(t2);
    if (hProfile) cmsCloseProfile(hProfile);

    std::cout << "Finished test_CreateRoundtripXForm_different_intents_independent_transforms." << std::endl;
}

// Test 3: Repeated creation/cleanup to ensure no resource leaks or crashes
// Create multiple transforms in sequence and ensure cleanup does not crash or degrade.
static void test_CreateRoundtripXForm_repeated_creation_cleanup() {
    std::cout << "Running test_CreateRoundtripXForm_repeated_creation_cleanup..." << std::endl;

    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    TEST_ASSERT(hProfile != nullptr, "Failed to create sRGB profile for Test 3");

    const int repeats = 5;
    std::vector<cmsHTRANSFORM> transforms;
    for (int i = 0; i < repeats; ++i) {
        cmsHTRANSFORM t = CreateRoundtripXForm(hProfile, (i % 2) ? INTENT_PERCEPTUAL : INTENT_RELATIVE_COLORIMETRIC);
        TEST_ASSERT(t != nullptr, "CreateRoundtripXForm returned NULL during repeated creation");
        transforms.push_back(t);
        // Try a tiny transform usage to ensure it's usable
        if (t) {
            bool ok = transform_lab_and_validate_L(t);
            TEST_ASSERT(ok, "Repeated transform failed to process Lab in Test 3");
        }
    }

    // Cleanup
    for (auto t : transforms) {
        if (t) cmsDeleteTransform(t);
    }
    if (hProfile) cmsCloseProfile(hProfile);

    std::cout << "Finished test_CreateRoundtripXForm_repeated_creation_cleanup." << std::endl;
}

int main() {
    // Candidate Keywords explored for understanding:
    // cmsHTRANSFORM, CreateRoundtripXForm, hProfile, INTENT_RELATIVE_COLORIMETRIC,
    // INTENT_PERCEPTUAL, cmsCreate_sRGBProfile, cmsDeleteTransform, cmsDoTransform

    test_CreateRoundtripXForm_basic_valid_input();
    test_CreateRoundtripXForm_different_intents_independent_transforms();
    test_CreateRoundtripXForm_repeated_creation_cleanup();

    std::cout << "\nTest Summary: " << g_passed << " / " << g_total << " tests passed." << std::endl;
    if (g_passed == g_total) {
        std::cout << "ALL TESTS PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << (g_total - g_passed) << " tests failed." << std::endl;
        return 1;
    }
}