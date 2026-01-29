// Test suite for CombineGammaFloat in a C++11 environment without GTest.
// This harness builds small tabulated gamma curves, applies CombineGammaFloat,
// and validates the resulting behavior against analytically predicted outcomes.
// The tests do not terminate the program on failure to maximize coverage.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <lcms2.h>
#include <cmath>


// Include LittleCMS header to access required APIs.
// Ensure the project links against liblcms2 (if available in the environment).

// Candidate Keywords derived from Step 1:
// - cmsToneCurve, cmsEvalToneCurveFloat, cmsBuildTabulatedToneCurve16
// - CombineGammaFloat, gamma composition (g2 ∘ g1)
// - identity curves, power-law gamma curves (via tabulated curves)
// - DbgThread / cmsContext concept (we use cmsCreateContext for a valid context)
// - cmsFreeToneCurve, memory management for curves
// - evaluation of resulting curve with cmsEvalToneCurveFloat

// Simple tolerance-based assertion helper
static void expect_near(double a, double b, double tol, const char* msg) {
    if (std::fabs(a - b) > tol) {
        std::fprintf(stderr, "ASSERT FAILED: %s | actual=%f, expected=%f, tol=%e\n", msg, a, b, tol);
        // Do not exit immediately to maximize test coverage
    } else {
        std::printf("PASS: %s | actual=%f, expected=%f, tol=%e\n", msg, a, b, tol);
    }
}

// Helper to create a gamma curve from a power p: g(x) = x^p, mapped to 16-bit table
static cmsToneCurve* CreateGammaCurveFromPower(cmsContext Context, double p) {
    cmsUInt16Number Tab[256];
    for (int i = 0; i < 256; ++i) {
        double x = (double)i / 255.0;
        double y = std::pow(x, p);
        if (y < 0.0) y = 0.0;
        if (y > 1.0) y = 1.0;
        Tab[i] = (cmsUInt16Number)std::floor(y * 65535.0 + 0.5);
    }
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(Context, 256, Tab);
    return curve;
}

// Test 1: Identity g1 and Identity g2 → combined should behave like identity
// combinedTab[i] ≈ floor((i/255)^1 * 65535 + 0.5) / 65535
static void test_CombineGammaFloat_identity_identity() {
    std::printf("Running test_CombineGammaFloat_identity_identity...\n");

    cmsContext Context = cmsCreateContext(nullptr, nullptr);
    if (!Context) {
        std::fprintf(stderr, "Failed to create CMS context.\n");
        return;
    }

    // Identity-like tab: f -> f (with 8-bit input mapped to 16-bit output)
    cmsUInt16Number Tab[256];
    for (int i = 0; i < 256; ++i) {
        double f = (double)i / 255.0;
        Tab[i] = (cmsUInt16Number)std::floor(f * 65535.0 + 0.5);
    }

    cmsToneCurve* g1 = cmsBuildTabulatedToneCurve16(Context, 256, Tab);
    cmsToneCurve* g2 = cmsBuildTabulatedToneCurve16(Context, 256, Tab);

    cmsToneCurve* Comb = CombineGammaFloat(g1, g2);
    if (!Comb) {
        std::fprintf(stderr, "CombineGammaFloat returned NULL in identity-identity test.\n");
        cmsFreeContext(Context);
        return;
    }

    // Validate the combined curve at several sample points
    const double tol = 1e-4;
    for (int i = 0; i <= 255; ++i) {
        double x = (double)i / 255.0;
        cmsFloat32Number y = cmsEvalToneCurveFloat(Comb, (cmsFloat32Number)x);
        double expected = x; // identity expectation
        expect_near((double)y, expected, tol,
                    "Identity-Identity: combined curve at i/255 matches i/255");
    }

    cmsFreeToneCurve(g1);
    cmsFreeToneCurve(g2);
    cmsFreeToneCurve(Comb);
    cmsFreeContext(Context);

    std::printf("Completed test_CombineGammaFloat_identity_identity.\n\n");
}

// Test 2: g1 = identity, g2 = power gamma p (e.g., p = 2.2)
// Expect combined curve to approximate g2 since g1 is identity.
static void test_CombineGammaFloat_identity_with_power() {
    std::printf("Running test_CombineGammaFloat_identity_with_power...\n");

    const double p = 2.2;

    cmsContext Context = cmsCreateContext(nullptr, nullptr);
    if (!Context) {
        std::fprintf(stderr, "Failed to create CMS context.\n");
        return;
    }

    // Identity g1
    cmsUInt16Number TabID[256];
    for (int i = 0; i < 256; ++i) {
        double f = (double)i / 255.0;
        TabID[i] = (cmsUInt16Number)std::floor(f * 65535.0 + 0.5);
    }
    cmsToneCurve* g1 = cmsBuildTabulatedToneCurve16(Context, 256, TabID);

    // g2: gamma p
    cmsToneCurve* g2 = CreateGammaCurveFromPower(Context, p);

    cmsToneCurve* Comb = CombineGammaFloat(g1, g2);
    if (!Comb) {
        std::fprintf(stderr, "CombineGammaFloat returned NULL in identity+power test.\n");
        cmsFreeToneCurve(g1);
        cmsFreeToneCurve(g2);
        cmsFreeContext(Context);
        return;
    }

    // Validate for a few representative points
    const int testPoints[] = {0, 64, 128, 200, 255};
    const int nPts = sizeof(testPoints) / sizeof(testPoints[0]);
    const double tol = 1e-3;
    for (int k = 0; k < nPts; ++k) {
        int i = testPoints[k];
        double x = (double)i / 255.0;
        cmsFloat32Number y = cmsEvalToneCurveFloat(Comb, (cmsFloat32Number)x);
        double expected = std::pow(x, p); // since g1 is identity, combined ≈ g2(x)
        expect_near((double)y, expected, tol, "Identity + Power gamma: y ≈ x^p");
    }

    cmsFreeToneCurve(g1);
    cmsFreeToneCurve(g2);
    cmsFreeToneCurve(Comb);
    cmsFreeContext(Context);

    std::printf("Completed test_CombineGammaFloat_identity_with_power.\n\n");
}

// Test 3: General power composition: g1 with p1, g2 with p2
// Expect combined curve to approximate x^(p1*p2)
static void test_CombineGammaFloat_power_composition() {
    std::printf("Running test_CombineGammaFloat_power_composition...\n");

    const double p1 = 0.8;
    const double p2 = 2.0;
    const double product = p1 * p2; // 1.6

    cmsContext Context = cmsCreateContext(nullptr, nullptr);
    if (!Context) {
        std::fprintf(stderr, "Failed to create CMS context.\n");
        return;
    }

    // g1: gamma p1
    cmsToneCurve* g1 = CreateGammaCurveFromPower(Context, p1);

    // g2: gamma p2
    cmsToneCurve* g2 = CreateGammaCurveFromPower(Context, p2);

    cmsToneCurve* Comb = CombineGammaFloat(g1, g2);
    if (!Comb) {
        std::fprintf(stderr, "CombineGammaFloat returned NULL in power composition test.\n");
        cmsFreeToneCurve(g1);
        cmsFreeToneCurve(g2);
        cmsFreeContext(Context);
        return;
    }

    // Validate at several representative points
    const int testPoints[] = {0, 32, 64, 128, 192, 255};
    const int nPts = sizeof(testPoints) / sizeof(testPoints[0]);
    const double tol = 1e-3;
    for (int k = 0; k < nPts; ++k) {
        int i = testPoints[k];
        double x = (double)i / 255.0;
        cmsFloat32Number y = cmsEvalToneCurveFloat(Comb, (cmsFloat32Number)x);
        double expected = std::pow(x, product);
        expect_near((double)y, expected, tol, "Power composition: y ≈ x^(p1*p2)");
    }

    cmsFreeToneCurve(g1);
    cmsFreeToneCurve(g2);
    cmsFreeToneCurve(Comb);
    cmsFreeContext(Context);

    std::printf("Completed test_CombineGammaFloat_power_composition.\n\n");
}

int main() {
    // Run tests in sequence
    test_CombineGammaFloat_identity_identity();
    test_CombineGammaFloat_identity_with_power();
    test_CombineGammaFloat_power_composition();

    std::cout << "All tests executed. Review PASS messages above for validation." << std::endl;
    return 0;
}