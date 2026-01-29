// Test suite for ExtractGray2Y in cmsps2.c using a lightweight, non-GTest approach.
// This test suite aims to cover the true and false branches of the condition
// inside ExtractGray2Y (Out != NULL && xform != NULL) by simulating scenarios
// where the internal transform is successfully created and where it is not.
//
// Note: This test relies on the LittleCMS public API and does not mock any
// private/static members. It uses a simple in-source harness to run tests
// from main() and reports failures via a small, non-terminating assertion macro.

#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


static int g_failures = 0;

// Lightweight assertion macros (non-terminating)
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        fprintf(stderr, "TEST FAILED: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ_U16(a,b) do { \
    if((cmsUInt16Number)(a) != (cmsUInt16Number)(b)) { \
        fprintf(stderr, "TEST FAILED: %s == %s (%u != %u) at %s:%d\n", #a, #b, (unsigned)(a), (unsigned)(b), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

// Helper: saturate-like rounding for expected value
static cmsUInt16Number SaturateWordFromDouble(double v) {
    // Emulate _cmsQuickSaturateWord behavior: clamp to [0, 65535] with rounding
    if (v <= 0.0) return 0;
    if (v >= 65535.0) return 65535;
    int iv = static_cast<int>(v + 0.5); // rounding
    if (iv < 0) iv = 0;
    if (iv > 65535) iv = 65535;
    return static_cast<cmsUInt16Number>(iv);
}

// Test A: True branch - Out and xform are both valid; verify values match a separate transform
static void test_ExtractGray2Y_trueBranch() {
    // Create a default context and a simple grayscale profile to drive the transform
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    cmsHPROFILE hProfile = cmsCreateGrayProfile(nullptr);
    const cmsUInt32Number Intent = INTENT_PERCEPTUAL;

    // Call the focal function
    cmsToneCurve* Out = ExtractGray2Y(ctx, hProfile, Intent);
    EXPECT_TRUE(Out != nullptr);

    if (Out != nullptr) {
        // Independently reproduce the transform to compute expected values for a few samples
        cmsHPROFILE hXYZ = cmsCreateXYZProfile();
        cmsHTRANSFORM xform = cmsCreateTransformTHR(ctx, hProfile, TYPE_GRAY_8, hXYZ, TYPE_XYZ_DBL, Intent, cmsFLAGS_NOOPTIMIZE);

        if (xform != NULL) {
            const int samples[] = {0, 32, 128, 255};
            const int nSamples = sizeof(samples)/sizeof(samples[0]);
            for (int k = 0; k < nSamples; ++k) {
                int i = samples[k];
                cmsUInt8Number Gray = static_cast<cmsUInt8Number>(i);
                cmsCIEXYZ XYZ;
                cmsDoTransform(xform, &Gray, &XYZ, 1);

                // Expected value derived from XYZ.Y scaled to 0..65535 with rounding
                double scaledY = XYZ.Y * 65535.0;
                cmsUInt16Number expected = SaturateWordFromDouble(scaledY);

                // Ensure actual table value matches expected
                EXPECT_EQ_U16(Out->Table16[i], expected);
            }
            cmsDeleteTransform(xform);
        } else {
            // If transform creation fails in this environment, report but do not crash
            fprintf(stderr, "Warning: xform NULL in true-branch test; skipping per-sample comparison.\n");
        }

        cmsCloseProfile(hXYZ);
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    cmsFreeToneCurve(Out);
    // Context cleanup is omitted; program exit will reclaim resources
}

// Test B: False branch - xform cannot be created (invalid profile), ensure function returns a valid Out
static void test_ExtractGray2Y_falseBranch_xformNull() {
    // Use an intentionally invalid profile pointer to force xform creation to fail
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    cmsHPROFILE invalidProfile = (cmsHPROFILE)0xDEADBEEF;
    const cmsUInt32Number Intent = INTENT_PERCEPTUAL;

    // Call the focal function; xform creation should fail and the loop should be skipped
    cmsToneCurve* Out = ExtractGray2Y(ctx, invalidProfile, Intent);
    EXPECT_TRUE(Out != nullptr);

    if (Out != nullptr) {
        // Since xform was NULL, the output table should remain at its default zero-initialized state
        bool allZero = true;
        for (int i = 0; i < 256; ++i) {
            if (Out->Table16[i] != 0) {
                allZero = false;
                break;
            }
        }
        EXPECT_TRUE(allZero);
        cmsFreeToneCurve(Out);
    }

    // Cleanup
    // No profile to close for the invalid profile
}

// Entry point for running the tests
int main() {
    // Run tests
    test_ExtractGray2Y_trueBranch();
    test_ExtractGray2Y_falseBranch_xformNull();

    if (g_failures == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Total failures: %d\n", g_failures);
        return g_failures;
    }
}