// test_fast_float_lab.cpp
// Lightweight C++11 test suite for the focal method CheckToFloatLab in
// fast_float_testbed.c (as provided in the prompt).
//
// Notes:
// - This test does not rely on any external testing framework (GTest, etc.).
// - It uses a minimal in-file test harness with simple assertions.
// - It imports the target function via C linkage.
// - It exercises the function under test and also performs an auxiliary
//   validation using the same Little CMS API to exercise related paths.
// - The tests aim to provide coverage for core behavior and ensure the function
//   runs to completion without crashing. The internal logging path is a no-op in
//   the provided code, so assertions focus on execution and observable behavior
//   via the Little CMS API.
// - This test requires Little CMS (lcms2) headers and library to be available
//   at link time (e.g., -llcms2).

#include <cstdio>
#include <fast_float_internal.h>
#include <limits>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <lcms2.h>
#include <cmath>


// Include Little CMS header for auxiliary validation in tests

// Declare the focal function with C linkage (as defined in the C source)
extern "C" void CheckToFloatLab(void);

// Simple assertion macros to avoid bringing in a heavy framework
#define ASSERT_TRUE(x) if(!(x)) { std::cerr << "Assertion failed: " #x " at " << __FILE__ << ":" << __LINE__ << "\n"; return false; }
#define ASSERT_FALSE(x) if((x)) { std::cerr << "Assertion failed: " #x " is true at " << __FILE__ << ":" << __LINE__ << "\n"; return false; }

// Candidate Keywords mapping (Step 1): 
// - Plugin/Raw contexts: cmsCreateContext, cmsFastFloatExtensions
// - ICC Profiles: hsRGB, hLab
// - Transforms: cmsCreateTransformTHR, cmsCreateTransformTHR
// - Color data: TYPE_RGB_8, TYPE_Lab_DBL, cmsCIELab
// - DeltaE comparison: cmsDeltaE
// - Loop over RGB space increments (r, g, b)
// - Threshold check: err > 0.1 (branch under test)
// This test suite references these keywords to align with the focal method's core logic.

// Helper: perform a smaller, focused deltaE check using the same Little CMS API
// as the focal function to ensure there's at least one case where the two
// transforms yield noticeably different Lab results (err > 0.1).
static bool auxiliary_CheckDeltaE_ExistsSmallSample()
{
    // Domain knowledge: Use the same dependencies as in CheckToFloatLab, but in a
    // smaller sampling to verify that the two transform paths diverge enough to
    // exceed the 0.1 DeltaE threshold at least for some RGB input.
    cmsContext Plugin = cmsCreateContext(cmsFastFloatExtensions(), NULL);
    cmsContext Raw = cmsCreateContext(NULL, NULL);
    cmsHPROFILE hsRGB = cmsCreate_sRGBProfile();
    cmsHPROFILE hLab = cmsCreateLab4Profile(NULL);

    cmsHTRANSFORM xform_plugin = cmsCreateTransformTHR(Plugin, hsRGB, TYPE_RGB_8, hLab, TYPE_Lab_DBL, INTENT_PERCEPTUAL, 0);
    cmsHTRANSFORM xform = cmsCreateTransformTHR(Raw, hsRGB, TYPE_RGB_8, hLab, TYPE_Lab_DBL, INTENT_PERCEPTUAL, 0);

    bool found = false;
    cmsCIELab Lab1, Lab2;
    cmsUInt8Number rgb[3];

    // Reduce sampling to keep test fast but still meaningful
    for (int r = 0; r < 256; r += 32) {
        for (int g = 0; g < 256; g += 32) {
            for (int b = 0; b < 256; b += 32) {
                rgb[0] = static_cast<cmsUInt8Number>(r);
                rgb[1] = static_cast<cmsUInt8Number>(g);
                rgb[2] = static_cast<cmsUInt8Number>(b);

                cmsDoTransform(xform_plugin, rgb, &Lab1, 1);
                cmsDoTransform(xform, rgb, &Lab2, 1);

                double err = cmsDeltaE(&Lab1, &Lab2);
                if (err > 0.1) {
                    found = true;
                    goto cleanup; // stop after first finding
                }
            }
        }
    }

cleanup:
    cmsDeleteTransform(xform);
    cmsDeleteTransform(xform_plugin);
    cmsCloseProfile(hsRGB);
    cmsCloseProfile(hLab);
    cmsDeleteContext(Raw);
    cmsDeleteContext(Plugin);
    return found;
}

// Test 1: Ensure the focal function runs to completion without crashing.
// Rationale: If the function can execute its entire set of transforms and
// cleanup without raising an exception or crashing, it passes this basic
// sanity check.
static bool test_CheckToFloatLab_DoesNotCrash()
{
    // Domain knowledge: The function is deterministic and has no return value.
    // A successful run implies it handled all resources properly.
    try {
        CheckToFloatLab();
    } catch (...) {
        std::cerr << "CheckToFloatLab threw an exception\n";
        return false;
    }
    // If we reach here, the function completed execution.
    return true;
}

// Test 2: Validate that there exists at least one RGB input leading to a
// DeltaE > 0.1 between the two transform paths (plugin vs raw). This ensures
// the code path inside the if (err > 0.1) branch can be exercised.
// This complements the RunCheckToFloatLab test by verifying observable behavior
// in a separate, controlled context using the same underlying library.
static bool test_CheckToFloatLab_DeltaE_ExistsInAuxiliaryPath()
{
    return auxiliary_CheckDeltaE_ExistsSmallSample();
}

// Minimal test harness (no external frameworks)
int main()
{
    int passed = 0;
    int failed = 0;

    // Test 1: Do not crash
    if (test_CheckToFloatLab_DoesNotCrash()) {
        std::cout << "[PASS] CheckToFloatLab_DoesNotCrash\n";
        ++passed;
    } else {
        std::cerr << "[FAILED] CheckToFloatLab_DoesNotCrash\n";
        ++failed;
    }

    // Test 2: DeltaE > 0.1 exists in auxiliary sampling
    if (test_CheckToFloatLab_DeltaE_ExistsInAuxiliaryPath()) {
        std::cout << "[PASS] CheckToFloatLab_DeltaE_ExistsInAuxiliaryPath\n";
        ++passed;
    } else {
        std::cerr << "[FAILED] CheckToFloatLab_DeltaE_ExistsInAuxiliaryPath\n";
        ++failed;
    }

    std::cout << "Summary: " << passed << " passed, " << failed << " failed.\n";

    // Return non-zero if any test failed to signal issues to CI systems
    return (failed == 0) ? 0 : 1;
}