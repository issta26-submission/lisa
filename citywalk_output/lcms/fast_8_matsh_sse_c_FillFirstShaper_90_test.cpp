// Unit test suite for FillFirstShaper using a lightweight C++11 test harness.
// This test assumes the presence of LittleCMS-like APIs and the FillFirstShaper
// function compiled in the same project (as provided in fast_8_matsh_sse.c).
// The tests rely on public API to construct tone curves and evaluate them.
//
// Build notes (assumed):
// - Link with LittleCMS (lcms2) headers and library.
// - The FillFirstShaper function has C linkage.
// - Include path for lcms2.h should be configured in the build system.
//
// Tests focus on:
// 1) Verifying FillFirstShaper fills 256 entries by evaluating the curve at i/255.0.
// 2) Verifying behavior with a non-linear gamma-like curve (gamma = 2.2).
// 3) Ensuring results stay in the expected domain [0, 1].
// 4) Non-terminating style: accumulate failures and report at the end.

#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <string>
#include <cpuid.h>
#include <lcms2.h>
#include <cmath>


// Provide C linkage for C APIs.
extern "C" {
}

// Declaration of the focal method under test (C linkage).
extern "C" void FillFirstShaper(cmsFloat32Number* Table, cmsToneCurve* Curve);

// Helper: simple, non-terminating assertion utility for test logs.
static bool almostEqual(float a, float b, float tol) {
    return std::fabs(static_cast<double>(a) - static_cast<double>(b)) <= static_cast<double>(tol);
}

// Helper: run a single test and print status.
static bool runTest(const std::string& name, const std::function<bool()>& testBody) {
    bool ok = testBody();
    if (ok) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        std::cout << "[FAIL] " << name << std::endl;
    }
    return ok;
}

/*
 Test 1: Linear (gamma = 1.0) tone curve

 Rationale:
 - Build a 256-entry tone curve where table[i] = i/255.0 (linear ramp).
 - FillFirstShaper should evaluate the curve at R = i/255.0 and store results in Table[i].
 - For a linear curve, cmsEvalToneCurveFloat(Curve, R) should yield R (within numeric tolerance),
   so after FillFirstShaper, Table[i] should equal ramp[i].
 Expected: All 256 values are equal to ramp[i] within a small tolerance.
*/
static bool testFillFirstShaper_linearCurve() {
    constexpr int N = 256;
    cmsFloat32Number ramp[N];
    for (int i = 0; i < N; ++i) {
        ramp[i] = (cmsFloat32Number)(i) / 255.0f;
    }

    cmsToneCurve* curve = cmsBuildToneCurveFloat(N, ramp);
    if (curve == nullptr) {
        std::cerr << "Error: Failed to build linear tone curve for test." << std::endl;
        return false;
    }

    cmsFloat32Number Table[N];
    FillFirstShaper(Table, curve);

    bool passed = true;
    const double tol = 1e-5;
    for (int i = 0; i < N; ++i) {
        // The expected value is the ramp value at i, since the curve is linear.
        cmsFloat32Number expected = ramp[i];
        cmsFloat32Number actual   = Table[i];
        if (!almostEqual(static_cast<float>(actual), static_cast<float>(expected), static_cast<float>(tol))) {
            std::cerr << "Mismatch at index " << i
                      << ": expected " << static_cast<float>(expected)
                      << ", got " << static_cast<float>(actual) << std::endl;
            passed = false;
            // Do not abort; collect all failures for better coverage insight.
        }
    }

    cmsFreeToneCurve(curve);
    return passed;
}

/*
 Test 2: Non-linear gamma-like curve (gamma = 2.2)

 Rationale:
 - Build a 256-entry tone curve with values ramp2[i] = (i/255.0) ^ 2.2.
 - This simulates a typical gamma-corrected curve.
 - FillFirstShaper should evaluate the curve at R = i/255.0; for a correctly built 256-point curve,
   cmsEvalToneCurveFloat(Curve, R) should return ramp2[i], so Table[i] should equal ramp2[i].
 Expected: All 256 values match ramp2[i] within tolerance.
*/
static bool testFillFirstShaper_gammaCurve() {
    constexpr int N = 256;
    cmsFloat32Number ramp[N];
    const double gamma = 2.2;
    for (int i = 0; i < N; ++i) {
        double x = static_cast<double>(i) / 255.0;
        ramp[i] = (cmsFloat32Number)std::pow(x, gamma);
    }

    cmsToneCurve* curve = cmsBuildToneCurveFloat(N, ramp);
    if (curve == nullptr) {
        std::cerr << "Error: Failed to build gamma-tone curve for test." << std::endl;
        return false;
    }

    cmsFloat32Number Table[N];
    FillFirstShaper(Table, curve);

    bool passed = true;
    const double tol = 1e-4;
    for (int i = 0; i < N; ++i) {
        cmsFloat32Number expected = ramp[i];
        cmsFloat32Number actual   = Table[i];
        if (!almostEqual(static_cast<float>(actual), static_cast<float>(expected), static_cast<float>(tol))) {
            std::cerr << "Gamma mismatch at index " << i
                      << ": expected " << static_cast<float>(expected)
                      << ", got " << static_cast<float>(actual) << std::endl;
            passed = false;
        }
    }

    cmsFreeToneCurve(curve);
    return passed;
}

/*
 Test 3: Domain boundary and basic validity

 Rationale:
 - Ensure all Table[i] values lie within [0, 1] after FillFirstShaper, which is a reasonable
   expectation for tone-curve evaluations on inputs in [0,1].
 - This test complements the two precise-output tests by checking bounds.
*/
static bool testFillFirstShaper_domainBounds() {
    constexpr int N = 256;
    cmsFloat32Number ramp[N];
    // Use a simple linear ramp to create a valid curve.
    for (int i = 0; i < N; ++i) ramp[i] = (cmsFloat32Number)(i) / 255.0f;

    cmsToneCurve* curve = cmsBuildToneCurveFloat(N, ramp);
    if (curve == nullptr) {
        std::cerr << "Error: Failed to build tone curve for domain-bounds test." << std::endl;
        return false;
    }

    cmsFloat32Number Table[N];
    FillFirstShaper(Table, curve);

    bool withinBounds = true;
    for (int i = 0; i < N; ++i) {
        float v = static_cast<float>(Table[i]);
        if (!(v >= 0.0f - 1e-6f && v <= 1.0f + 1e-6f)) {
            std::cerr << "Value out of bounds at index " << i << ": " << v << std::endl;
            withinBounds = false;
        }
    }

    cmsFreeToneCurve(curve);
    return withinBounds;
}

// Main function: run all tests and print summary.
// As requested, test methods are invoked from main (no GTest or Google Mock).
int main() {
    int total = 0;
    int failed = 0;

    auto t1 = runTest("FillFirstShaper_linearCurve (linear ramp)", []() { return testFillFirstShaper_linearCurve(); });
    total += 1; if (!t1) ++failed;

    auto t2 = runTest("FillFirstShaper_gammaCurve (gamma = 2.2)", []() { return testFillFirstShaper_gammaCurve(); });
    total += 1; if (!t2) ++failed;

    auto t3 = runTest("FillFirstShaper_domainBounds (value in [0,1])", []() { return testFillFirstShaper_domainBounds(); });
    total += 1; if (!t3) ++failed;

    std::cout << "Test summary: " << (total - failed) << " passed, " << failed << " failed, out of " << total << " tests." << std::endl;

    return failed ? 1 : 0;
}