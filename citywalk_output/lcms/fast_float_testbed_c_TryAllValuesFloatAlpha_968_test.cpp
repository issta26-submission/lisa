// Lightweight C++11 test suite for TryAllValuesFloatAlpha
// This test targets the focal method described and avoids GTest.
// It relies on the LittleCMS API. Ensure to link with -llcms2 when compiling.
// The tests are designed to be non-terminating (no exceptions) and print results to stdout.

#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <lcms2.h>


// Include LittleCMS headers. Adjust include path if necessary.

// The focal method is defined in fast_float_testbed.c with C linkage.
// Declare it here for C++ to call it directly.
extern "C" void TryAllValuesFloatAlpha(cmsHPROFILE hlcmsProfileIn,
                                       cmsHPROFILE hlcmsProfileOut,
                                       cmsInt32Number Intent,
                                       cmsBool copyAlpha);

// Simple test harness (no GTest). Provides basic assertions via macros.
#define TEST_ASSERT(cond, msg)                                  \
    do {                                                          \
        if (!(cond)) {                                            \
            std::cerr << "ASSERTION FAILED: " << (msg) << "\n"; \
            return false;                                         \
        }                                                         \
    } while (0)

static bool test_TryAllValuesFloatAlpha_NoCopyAlpha() {
    // Test the focal method with copyAlpha = false (0) to exercise the non-copy path.
    // Candidate Keywords: Raw context, Plugin context, Transform creation, RGB float pixels, comparison via ValidFloat.
    std::cout << "[Test] TryAllValuesFloatAlpha with copyAlpha = FALSE" << std::endl;

    // Create input and output profiles (sRGB). These are small, standard profiles suitable for tests.
    cmsHPROFILE inProfile  = cmsCreate_sRGBProfile();
    cmsHPROFILE outProfile = cmsCreate_sRGBProfile();

    TEST_ASSERT(inProfile != NULL, "Failed to create input sRGB profile");
    TEST_ASSERT(outProfile != NULL, "Failed to create output sRGB profile");

    // Call the focal method. Use perceptual intent (CMS_INTENT_PERCEPTUAL) and copyAlpha = 0.
    // We don't perform post-call assertions on internal state; success is defined as no crash.
    TryAllValuesFloatAlpha(inProfile, outProfile, CMS_INTENT_PERCEPTUAL, 0);

    // Do not free profiles here. The focal method closes the profiles inside.
    // This matches the function's expected lifecycle and avoids double-closing.

    std::cout << "[Test] Completed: TryAllValuesFloatAlpha_NoCopyAlpha" << std::endl;
    return true;
}

static bool test_TryAllValuesFloatAlpha_WithCopyAlpha() {
    // Test the focal method with copyAlpha = true (non-zero) to exercise the alpha-copy path.
    std::cout << "[Test] TryAllValuesFloatAlpha with copyAlpha = TRUE" << std::endl;

    cmsHPROFILE inProfile  = cmsCreate_sRGBProfile();
    cmsHPROFILE outProfile = cmsCreate_sRGBProfile();

    TEST_ASSERT(inProfile != NULL, "Failed to create input sRGB profile");
    TEST_ASSERT(outProfile != NULL, "Failed to create output sRGB profile");

    // Call the focal method with copyAlpha enabled.
    TryAllValuesFloatAlpha(inProfile, outProfile, CMS_INTENT_RELATIVE_COLORIMETRIC, 1);

    // Profiles are closed inside TryAllValuesFloatAlpha.
    std::cout << "[Test] Completed: TryAllValuesFloatAlpha_WithCopyAlpha" << std::endl;
    return true;
}

int main() {
    int testsTotal = 0;
    int testsPassed = 0;

    // Run Test 1
    ++testsTotal;
    if (test_TryAllValuesFloatAlpha_NoCopyAlpha()) {
        ++testsPassed;
        std::cout << "[OK]  Test 1 passed" << std::endl;
    } else {
        std::cout << "[FAIL] Test 1 failed" << std::endl;
    }

    // Run Test 2
    ++testsTotal;
    if (test_TryAllValuesFloatAlpha_WithCopyAlpha()) {
        ++testsPassed;
        std::cout << "[OK]  Test 2 passed" << std::endl;
    } else {
        std::cout << "[FAIL] Test 2 failed" << std::endl;
    }

    std::cout << "Summary: " << testsPassed << " / " << testsTotal << " tests passed." << std::endl;
    return (testsTotal == testsPassed) ? 0 : 1;
}