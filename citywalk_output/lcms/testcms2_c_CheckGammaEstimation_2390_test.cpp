// Test suite for the focal method: CheckGammaEstimation
// This test is implemented in C++11 without any testing framework (no gtest/gmock).
// It relies on the public interfaces exposed by testcms2.h that are used by the
// Little CMS test suite (e.g., Build_sRGBGamma, cmsEstimateGamma, CheckGammaEstimation).

#include <cstdio>
#include <cmath>
#include <testcms2.h>


// Include the project-specific test header which exposes the focal method's dependencies.
// It provides Build_sRGBGamma(), cmsEstimateGamma(), CheckGammaEstimation(), and
// cmsFreeToneCurve() among others.

// Simple non-terminating assertion helpers
static bool assert_true(bool expr, const char* msg) {
    if (!expr) {
        if (msg) std::fprintf(stderr, "Assertion failed: %s\n", msg);
        return false;
    }
    return true;
}

// Test 1: Positive case
// Purpose: Ensure CheckGammaEstimation returns 1 when the provided gamma g exactly
// matches the gamma estimated by cmsEstimateGamma for a standard sRGB gamma curve.
// Rationale: If cmsEstimateGamma is deterministic, feeding the same estimate back
// into CheckGammaEstimation should pass within the tolerance (0.001).
static int Test_Positive_GammaEstimation() {
    // Build a standard sRGB gamma tone curve
    cmsToneCurve* c = Build_sRGBGamma();
    if (c == nullptr) {
        std::fprintf(stderr, "Test_Positive_GammaEstimation: Failed to build sRGB gamma curve.\n");
        return 0;
    }

    // Compute an estimated gamma for the curve
    cmsFloat64Number est = cmsEstimateGamma(c, 0.001);

    // Invoke the focal method with the computed estimate
    cmsInt32Number ret = CheckGammaEstimation(c, est);

    // Clean up
    cmsFreeToneCurve(c);

    // We expect ret == 1
    if (!assert_true(ret == 1, "CheckGammaEstimation should return 1 for matching gamma")) {
        return 0;
    }

    return 1;
}

// Test 2: Negative case
// Purpose: Validate that CheckGammaEstimation returns 0 when given a gamma value
// that is deliberately incorrect (off by more than 0.001).
static int Test_Negative_GammaEstimation() {
    // Build the same sRGB gamma tone curve
    cmsToneCurve* c = Build_sRGBGamma();
    if (c == nullptr) {
        std::fprintf(stderr, "Test_Negative_GammaEstimation: Failed to build sRGB gamma curve.\n");
        return 0;
    }

    // Compute the true estimated gamma
    cmsFloat64Number est = cmsEstimateGamma(c, 0.001);

    // Deliberately offset the gamma value to trigger the false branch
    cmsFloat64Number wrong_g = est + 0.01; // larger than 0.001 tolerance

    // Invoke the focal method with the wrong gamma
    cmsInt32Number ret = CheckGammaEstimation(c, wrong_g);

    // Clean up
    cmsFreeToneCurve(c);

    // We expect ret == 0
    if (!assert_true(ret == 0, "CheckGammaEstimation should return 0 for non-matching gamma")) {
        return 0;
    }

    return 1;
}

int main() {
    int total = 0;
    int passed = 0;

    std::printf("Starting CheckGammaEstimation tests...\n");

    // Run tests
    total++; if (Test_Positive_GammaEstimation()) passed++;
    total++; if (Test_Negative_GammaEstimation()) passed++;

    std::printf("Tests completed: %d/%d passed.\n", passed, total);

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}