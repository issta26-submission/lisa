// Lightweight C++11 unit test suite for the focal function:
// cmsBool _cmsBuildRGB2XYZtransferMatrix(cmsMAT3* r, const cmsCIExyY* WhitePt, const cmsCIExyYTRIPLE* Primrs)
// This test avoids GTest and uses non-terminating assertions (logs failures but continues).
// The tests focus on basic correctness paths and input-domain coverage as described.

#include <limits>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include LittleCMS core types

// Declaration of the focal function (assumed to have external linkage in cmswtpnt.c)
extern "C" cmsBool _cmsBuildRGB2XYZtransferMatrix(cmsMAT3* r,
                                               const cmsCIExyY* WhitePt,
                                               const cmsCIExyYTRIPLE* Primrs);

// Simple non-terminating assertion macro
static int g_failures = 0;
#define TASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[TEST FAILED] " << msg \
                      << " (in " << __FUNCTION__ << ")\n"; \
            ++g_failures; \
        } else { \
            std::cout << "[TEST PASSED] " << msg << "\n"; \
        } \
    } while (0)

// Helper: check matrix elements for finiteness
static bool IsFiniteMatrix(const cmsMAT3* M) {
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            double val = M->v[r].n[c];
            if (!std::isfinite(val)) return false;
        }
    }
    return true;
}

// Helper: set sRGB-like primaries
static void SetSRGBPrimaries(cmsCIExyYTRIPLE* Primrs) {
    Primrs->Red.x   = 0.64; Primrs->Red.y   = 0.33;
    Primrs->Green.x = 0.30; Primrs->Green.y = 0.60;
    Primrs->Blue.x  = 0.15; Primrs->Blue.y  = 0.06;
}

// Test 1: Degenerate (non-invertible) primaries should cause the function to fail.
static void test_degPrimaries_Invertibility() {
    cmsCIExyY WhitePt;
    cmsCIExyYTRIPLE Primrs;
    cmsMAT3 M;

    // Degenerate primaries: all zero x,y values -> non-invertible Primaries matrix
    Primrs.Red.x = 0.0; Primrs.Red.y = 0.0;
    Primrs.Green.x = 0.0; Primrs.Green.y = 0.0;
    Primrs.Blue.x = 0.0; Primrs.Blue.y = 0.0;

    // Typical D65 white point
    WhitePt.x = 0.3127;
    WhitePt.y = 0.3290;

    // Call the focal function
    cmsBool ok = _cmsBuildRGB2XYZtransferMatrix(&M, &WhitePt, &Primrs);

    // Expect failure (FALSE)
    TASSERT(ok == FALSE, "Degenerate primaries should fail (return FALSE).");
}

// Test 2: Valid primaries (sRGB-like) with standard D65 WhitePoint; expect finite results.
static void test_validPrimaries_ShouldSucceed() {
    cmsCIExyY WhitePt;
    cmsCIExyYTRIPLE Primrs;
    cmsMAT3 M;

    SetSRGBPrimaries(&Primrs);

    // D65 white point
    WhitePt.x = 0.3127;
    WhitePt.y = 0.3290;

    cmsBool ok = _cmsBuildRGB2XYZtransferMatrix(&M, &WhitePt, &Primrs);

    TASSERT(ok == TRUE, "Valid primaries should succeed (return TRUE).");
    TASSERT(IsFiniteMatrix(&M), "Resulting 3x3 matrix should contain finite values.");
}

// Test 3: Variation in WhitePoint should produce different matrices (sanity check).
static void test_whitePointInfluence() {
    cmsCIExyY WhitePtA;
    cmsCIExyY WhitePtB;
    cmsCIExyYTRIPLE Primrs;
    cmsMAT3 M1, M2;

    SetSRGBPrimaries(&Primrs);

    // WhitePoint A: D65
    WhitePtA.x = 0.3127; WhitePtA.y = 0.3290;
    // WhitePoint B: a slightly different white (not physically exact, but valid for test)
    WhitePtB.x = 0.3457; WhitePtB.y = 0.3582;

    cmsBool okA = _cmsBuildRGB2XYZtransferMatrix(&M1, &WhitePtA, &Primrs);
    cmsBool okB = _cmsBuildRGB2XYZtransferMatrix(&M2, &WhitePtB, &Primrs);

    TASSERT(okA == TRUE, "Matrix A should succeed.");
    TASSERT(okB == TRUE, "Matrix B should succeed.");
    TASSERT(IsFiniteMatrix(&M1) && IsFiniteMatrix(&M2), "Both matrices must be finite.");

    // Ensure matrices differ (since WhitePt changes)
    bool differ = false;
    for (int r = 0; r < 3 && !differ; ++r) {
        for (int c = 0; c < 3; ++c) {
            if (std::abs(M1.v[r].n[c] - M2.v[r].n[c]) > 1e-9) {
                differ = true;
                break;
            }
        }
    }
    TASSERT(differ, "Matrices for different WhitePoints should differ.");
}

// Main: run all tests and report summary
int main() {
    std::cout << "Starting cmsBuildRGB2XYZtransferMatrix unit tests (no GTest)..." << std::endl;

    test_degPrimaries_Invertibility();
    test_validPrimaries_ShouldSucceed();
    test_whitePointInfluence();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}