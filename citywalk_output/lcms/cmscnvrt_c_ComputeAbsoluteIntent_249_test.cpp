// cmscnvrt_test.cpp
// Lightweight C++11 test suite for the focal method:
//   cmsBool ComputeAbsoluteIntent(...)
// The tests are designed to be executable without using GoogleTest.
// They rely on the CMS LCMS2 types and the ComputeAbsoluteIntent function
// being available via linked libraries (e.g., -llcms2) in the build environment.
//
// Key ideas implemented here:
// - Step 1 (Program Understanding) translated into test goals with Candidate Keywords
//   (AdaptationState, WhitePointIn, WhitePointOut, ChromaticAdaptationMatrixIn, ChromaticAdaptationMatrixOut, m, Inverse, CHAD, Temp, Identity, Scale)
// - Step 2 (Unit Test Generation) creates tests that exercise true/false branches
//   and both fully adapted and non-fully adapted paths.
// - Step 3 (Test Case Refinement) uses a small in-process harness to execute tests
//   while continuing after failures (non-terminating assertions). It also covers
//   static-like access by using the public API and struct members directly where safe.
// - No GTest: All tests are plain C++ and accumulate results; the main prints a summary.
// Domain knowledge: tests use CMS types (cmsBool, cmsFloat64Number, cmsCIEXYZ, cmsMAT3)
// and rely on the public function ComputeAbsoluteIntent signature.
//
// Note: This code assumes the CMS LCMS2 headers are available (e.g., include <lcms2.h>)
// and that the library is linked with the test binary. Adapt include paths as needed.

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Include CMS types and prototypes. Path may vary; adjust if necessary.

// Ensure linkage with C code
extern "C" {
    // Declaration of the focal method from cmscnvrt.c
    cmsBool ComputeAbsoluteIntent(cmsFloat64Number AdaptationState,
                                  const cmsCIEXYZ* WhitePointIn,
                                  const cmsMAT3* ChromaticAdaptationMatrixIn,
                                  const cmsCIEXYZ* WhitePointOut,
                                  const cmsMAT3* ChromaticAdaptationMatrixOut,
                                  cmsMAT3* m);
}

// Simple in-process test harness (non-terminating assertions)
static int g_passed = 0;
static int g_failed = 0;

static void logFailure(const std::string& testName, const std::string& detail) {
    std::cerr << "[FAIL] " << testName << ": " << detail << std::endl;
    ++g_failed;
}

static void logSuccess(const std::string& testName) {
    std::cout << "[PASS] " << testName << std::endl;
    ++g_passed;
}

// Utility helpers to set up a identity 3x3 matrix for CMS MAT3 layout.
// Typical layout in LCMS: cmsMAT3 contains 3 vectors v[0], v[1], v[2] representing
// the matrix columns. We initialize a standard mathematical identity:
// [1 0 0; 0 1 0; 0 0 1]
static void setIdentityCMSMAT3(cmsMAT3* m) {
    // Initialize as identity (column-major layout in many CMS internal representations)
    // Column 0
    m->v[0].X = 1.0;  m->v[0].Y = 0.0; m->v[0].Z = 0.0;
    // Column 1
    m->v[1].X = 0.0;  m->v[1].Y = 1.0; m->v[1].Z = 0.0;
    // Column 2
    m->v[2].X = 0.0;  m->v[2].Y = 0.0; m->v[2].Z = 1.0;
}

// Helper to alias an "empty" cmsCIEXYZ quickly
static void setXYZ(cmsCIEXYZ* p, cmsFloat64Number X, cmsFloat64Number Y, cmsFloat64Number Z) {
    p->X = X;
    p->Y = Y;
    p->Z = Z;
}

// Test 1: Full adaptation (AdaptationState == 1.0) uses the standard V4 behavior.
// Expectation: m becomes a diagonal scaling with X_in/X_out, Y_in/Y_out, Z_in/Z_out on
// the corresponding diagonal entries.
static void test_FullAdaptation_DiagonalScaling() {
    const std::string testName = "FullAdaptation_DiagonalScaling";

    cmsCIEXYZ WhitePointIn, WhitePointOut;
    setXYZ(&WhitePointIn, 1.0, 1.0, 1.0);
    setXYZ(&WhitePointOut, 2.0, 4.0, 8.0); // not all equal to stress per-axis scaling

    cmsMAT3 ChromaticAdaptationMatrixIn, ChromaticAdaptationMatrixOut;
    setIdentityCMSMAT3(&ChromaticAdaptationMatrixIn);
    setIdentityCMSMAT3(&ChromaticAdaptationMatrixOut);

    cmsMAT3 m;
    cmsBool ok = ComputeAbsoluteIntent(1.0,
                                       &WhitePointIn, &ChromaticAdaptationMatrixIn,
                                       &WhitePointOut, &ChromaticAdaptationMatrixOut,
                                       &m);
    if (!ok) {
        logFailure(testName, "ComputeAbsoluteIntent returned FALSE on full adaptation path.");
        return;
    }

    // Expect m.v[0].X == X_in/X_out, m.v[1].Y == Y_in/Y_out, m.v[2].Z == Z_in/Z_out
    cmsFloat64Number expX = WhitePointIn.X / WhitePointOut.X; // 0.5
    cmsFloat64Number expY = WhitePointIn.Y / WhitePointOut.Y; // 0.25
    cmsFloat64Number expZ = WhitePointIn.Z / WhitePointOut.Z; // 0.125

    const cmsFloat64Number tol = 1e-6;

    bool pass = true;
    if (std::fabs(m.v[0].X - expX) > tol) {
        logFailure(testName, "m.v[0].X does not match expected X_in/X_out.");
        pass = false;
    }
    if (std::fabs(m.v[1].Y - expY) > tol) {
        logFailure(testName, "m.v[1].Y does not match expected Y_in/Y_out.");
        pass = false;
    }
    if (std::fabs(m.v[2].Z - expZ) > tol) {
        logFailure(testName, "m.v[2].Z does not match expected Z_in/Z_out.");
        pass = false;
    }

    if (pass) logSuccess(testName);
}

// Test 2: Non-adapted path (AdaptationState == 0.0) with identity chromatic matrices.
// Expectation: m equals Scale * ChromaticAdaptationMatrixOut, which is identity * Scale
// i.e., diagonal scaling equal to In/Out ratios.
static void test_IncompleteAdaptation_ZeroState() {
    const std::string testName = "IncompleteAdaptation_ZeroState";

    cmsCIEXYZ WhitePointIn, WhitePointOut;
    setXYZ(&WhitePointIn, 1.0, 2.0, 3.0);
    setXYZ(&WhitePointOut, 2.0, 4.0, 6.0);

    cmsMAT3 ChromaticAdaptationMatrixIn, ChromaticAdaptationMatrixOut;
    setIdentityCMSMAT3(&ChromaticAdaptationMatrixIn);
    setIdentityCMSMAT3(&ChromaticAdaptationMatrixOut);

    cmsMAT3 m;
    cmsBool ok = ComputeAbsoluteIntent(0.0,
                                       &WhitePointIn, &ChromaticAdaptationMatrixIn,
                                       &WhitePointOut, &ChromaticAdaptationMatrixOut,
                                       &m);
    if (!ok) {
        logFailure(testName, "ComputeAbsoluteIntent returned FALSE on incomplete adaptation path.");
        return;
    }

    // Expect diag scaling X_in/X_out, Y_in/Y_out, Z_in/Z_out
    cmsFloat64Number expX = WhitePointIn.X / WhitePointOut.X;
    cmsFloat64Number expY = WhitePointIn.Y / WhitePointOut.Y;
    cmsFloat64Number expZ = WhitePointIn.Z / WhitePointOut.Z;
    const cmsFloat64Number tol = 1e-6;

    bool pass = true;
    if (std::fabs(m.v[0].X - expX) > tol) {
        logFailure(testName, "m.v[0].X != expected after incomplete adaptation (X).");
        pass = false;
    }
    if (std::fabs(m.v[1].Y - expY) > tol) {
        logFailure(testName, "m.v[1].Y != expected after incomplete adaptation (Y).");
        pass = false;
    }
    if (std::fabs(m.v[2].Z - expZ) > tol) {
        logFailure(testName, "m.v[2].Z != expected after incomplete adaptation (Z).");
        pass = false;
    }

    if (pass) logSuccess(testName);
}

// Test 3: Non-identity Adaptation (0.0 < AdaptationState < 1.0) with identity mats.
// The exact numeric result depends on CHAD handling; we primarily verify the
// function completes successfully (TRUE) and does not crash.
static void test_IntermediateAdaptation_Path() {
    const std::string testName = "IntermediateAdaptation_Path";

    cmsCIEXYZ WhitePointIn, WhitePointOut;
    setXYZ(&WhitePointIn, 1.0, 1.0, 1.0);
    setXYZ(&WhitePointOut, 1.5, 1.5, 1.5);

    cmsMAT3 ChromaticAdaptationMatrixIn, ChromaticAdaptationMatrixOut;
    setIdentityCMSMAT3(&ChromaticAdaptationMatrixIn);
    setIdentityCMSMAT3(&ChromaticAdaptationMatrixOut);

    cmsMAT3 m;
    cmsBool ok = ComputeAbsoluteIntent(0.5,
                                       &WhitePointIn, &ChromaticAdaptationMatrixIn,
                                       &WhitePointOut, &ChromaticAdaptationMatrixOut,
                                       &m);
    if (!ok) {
        logFailure(testName, "ComputeAbsoluteIntent returned FALSE on intermediate adaptation path.");
        return;
    }

    // We cannot reliably predict CHAD-based results here without deeper knowledge,
    // but we can assert that the function completed and wrote something to m.
    // A minimal sanity check: m should be a valid cmsMAT3 (not all zeros in a pathological case).
    bool isNotAllZero = false;
    if (std::fabs(m.v[0].X) > 0.0 || std::fabs(m.v[0].Y) > 0.0 || std::fabs(m.v[0].Z) > 0.0 ||
        std::fabs(m.v[1].X) > 0.0 || std::fabs(m.v[1].Y) > 0.0 || std::fabs(m.v[1].Z) > 0.0 ||
        std::fabs(m.v[2].X) > 0.0 || std::fabs(m.v[2].Y) > 0.0 || std::fabs(m.v[2].Z) > 0.0) {
        isNotAllZero = true;
    }

    if (isNotAllZero) {
        logSuccess(testName);
    } else {
        logFailure(testName, "Computed matrix appears to be all zeros; unexpected but possible in degenerate case.");
    }
}

// Test 4: Failure path when the inversion of ChromaticAdaptationMatrixIn fails.
// We simulate this by providing a non-invertible matrix (all zeros).
static void test_InversionFailure_FalseReturn() {
    const std::string testName = "InversionFailure_FalseReturn";

    cmsCIEXYZ WhitePointIn, WhitePointOut;
    setXYZ(&WhitePointIn, 0.0, 0.0, 0.0);
    setXYZ(&WhitePointOut, 1.0, 1.0, 1.0);

    cmsMAT3 ChromaticAdaptationMatrixIn, ChromaticAdaptationMatrixOut;
    // Non-invertible matrix: all zeros
    ChromaticAdaptationMatrixIn.v[0].X = 0.0; ChromaticAdaptationMatrixIn.v[0].Y = 0.0; ChromaticAdaptationMatrixIn.v[0].Z = 0.0;
    ChromaticAdaptationMatrixIn.v[1].X = 0.0; ChromaticAdaptationMatrixIn.v[1].Y = 0.0; ChromaticAdaptationMatrixIn.v[1].Z = 0.0;
    ChromaticAdaptationMatrixIn.v[2].X = 0.0; ChromaticAdaptationMatrixIn.v[2].Y = 0.0; ChromaticAdaptationMatrixIn.v[2].Z = 0.0;

    setIdentityCMSMAT3(&ChromaticAdaptationMatrixOut);

    cmsMAT3 m;
    cmsBool ok = ComputeAbsoluteIntent(0.0,
                                       &WhitePointIn, &ChromaticAdaptationMatrixIn,
                                       &WhitePointOut, &ChromaticAdaptationMatrixOut,
                                       &m);
    if (ok) {
        logFailure(testName, "ComputeAbsoluteIntent unexpectedly returned TRUE despite non-invertible In-Matrix.");
        return;
    } else {
        logSuccess(testName);
    }
}

// Entry point
int main() {
    // Step 2: Execute a comprehensive suite covering key branches
    // Step 3: Ensure tests are non-terminating on failure; accumulate results

    // Run tests
    test_FullAdaptation_DiagonalScaling();
    test_IncompleteAdaptation_ZeroState();
    test_IntermediateAdaptation_Path();
    test_InversionFailure_FalseReturn();

    // Summary
    std::cout << "\nTest Summary: " << g_passed << " passed, " << g_failed << " failed.\n";

    // Return code: 0 if all passed, 1 if any failed (diagnostic friendly)
    return (g_failed == 0) ? 0 : 1;
}

/*
Step-by-step notes:
- Candidate Keywords extracted from the focal method and dependencies:
  - AdaptationState, WhitePointIn, WhitePointOut, ChromaticAdaptationMatrixIn, ChromaticAdaptationMatrixOut
  - m (output cmsMAT3), m.v[0], m.v[1], m.v[2] (vector components)
  - Scale, m1, m2, m3, m4 (intermediate matrices)
  - Inverse, identity checks (_cmsMAT3inverse, _cmsMAT3isIdentity, _cmsMAT3identity)
  - CHAD2Temp, Temp2CHAD (CHAD handling)
  - path branches: AdaptationState == 1.0 (full adaptation) vs else (non-full)
  - Failure modes: non-invertible matrices, negative Temp, etc.

Notes for future extension:
- Additional tests can be added to exercise the Temp-based CHAD path by providing varied non-identity ChromaticAdaptationMatrixIn/Out values and a nontrivial AdaptationState (0.2, 0.8, etc.). The current suite validates both the full adaptation and non-adapted paths and the failure path via a non-invertible matrix.
- If the environment exposes additional public helpers for CMS matrix initialization, those can be used to make tests more robust and readable.
- Static dependencies: This test uses the public CMS API types and the focal function; it avoids mocking private/internal components and focuses on observable behavior via the public interface.