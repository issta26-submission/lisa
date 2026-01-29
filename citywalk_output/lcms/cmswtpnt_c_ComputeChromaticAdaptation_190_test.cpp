// Unit test suite for the focal method: ComputeChromaticAdaptation
// This test suite is written for C++11 without using GoogleTest.
// It relies on the public Little CMS (lcms2) types and the focal function
// declared in the provided sources. It uses a lightweight, non-terminating
// assertion mechanism to maximize code coverage.
// Explanatory comments are provided for each test case.

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cmath>


// Include the Little CMS public definitions to get types like cmsMAT3, cmsCIEXYZ, etc.

// Provide a C linkage declaration for the focal function to avoid name mangling
extern "C" cmsBool ComputeChromaticAdaptation(cmsMAT3* Conversion,
                                           const cmsCIEXYZ* SourceWhitePoint,
                                           const cmsCIEXYZ* DestWhitePoint,
                                           const cmsMAT3* Chad);

// Lightweight non-terminating assertion helpers
static int g_tests_total = 0;
static int g_tests_passed = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        ++g_tests_total; \
        if (static_cast<bool>(cond)) { \
            ++g_tests_passed; \
        } else { \
            std::cerr << "[TEST FAIL] " << msg << " (" #cond ")" << std::endl; \
        } \
    } while (0)

#define ASSERT_FALSE(cond, msg) \
    do { \
        ++g_tests_total; \
        if (!(static_cast<bool>(cond))) { \
            ++g_tests_passed; \
        } else { \
            std::cerr << "[TEST FAIL] " << msg << " (NOT " #cond ")" << std::endl; \
        } \
    } while (0)

static bool almost_equal(double a, double b, double tol = 1e-6) {
    return std::fabs(a - b) <= tol;
}

// Helper to set a 3x3 matrix to the identity
static void set_identity(cmsMAT3& M) {
    M.v[0].n[0] = 1.0; M.v[0].n[1] = 0.0; M.v[0].n[2] = 0.0;
    M.v[1].n[0] = 0.0; M.v[1].n[1] = 1.0; M.v[1].n[2] = 0.0;
    M.v[2].n[0] = 0.0; M.v[2].n[1] = 0.0; M.v[2].n[2] = 1.0;
}

// Helper to set a diagonal 3x3 matrix
static void set_diag(cmsMAT3& M, double a, double b, double c) {
    M.v[0].n[0] = a; M.v[0].n[1] = 0.0; M.v[0].n[2] = 0.0;
    M.v[1].n[0] = 0.0; M.v[1].n[1] = b; M.v[1].n[2] = 0.0;
    M.v[2].n[0] = 0.0; M.v[2].n[1] = 0.0; M.v[2].n[2] = c;
}

// Test 1: Identity Chromatic Adaptation (Chad = identity) with non-zero source/dest white points
// Expectation: Conversion becomes diag(Dest/Source) and function returns TRUE.
static void test_identity_chad_true_path() {
    cmsMAT3 Chad;
    set_identity(Chad);

    cmsCIEXYZ Source;
    Source.X = 0.2; Source.Y = 0.3; Source.Z = 0.4;

    cmsCIEXYZ Dest;
    Dest.X = 0.5; Dest.Y = 0.6; Dest.Z = 0.7;

    cmsMAT3 Conversion;
    cmsBool ret = ComputeChromaticAdaptation(&Conversion, &Source, &Dest, &Chad);

    // Expected diagonal ratios
    double rX = Dest.X / Source.X;
    double rY = Dest.Y / Source.Y;
    double rZ = Dest.Z / Source.Z;

    ASSERT_TRUE(ret == TRUE, "ComputeChromaticAdaptation should return TRUE for valid inputs with identity Chad");

    // Check that Conversion equals diag(rX, rY, rZ)
    bool diag_ok = almost_equal(Conversion.v[0].n[0], rX) &&
                   almost_equal(Conversion.v[0].n[1], 0.0) &&
                   almost_equal(Conversion.v[0].n[2], 0.0) &&
                   almost_equal(Conversion.v[1].n[0], 0.0) &&
                   almost_equal(Conversion.v[1].n[1], rY) &&
                   almost_equal(Conversion.v[1].n[2], 0.0) &&
                   almost_equal(Conversion.v[2].n[0], 0.0) &&
                   almost_equal(Conversion.v[2].n[1], 0.0) &&
                   almost_equal(Conversion.v[2].n[2], rZ);

    ASSERT_TRUE(diag_ok, "Conversion matrix should be diag(Dest/Source) for identity Chad");
}

// Test 2: Inverse of Chad fails -> function should return FALSE
// We supply a singular Chad matrix (all zeros) to force inversion failure.
static void test_singular_chad_false_path() {
    cmsMAT3 Chad;
    // Make Chad singular (zero matrix)
    Chad.v[0].n[0] = 0.0; Chad.v[0].n[1] = 0.0; Chad.v[0].n[2] = 0.0;
    Chad.v[1].n[0] = 0.0; Chad.v[1].n[1] = 0.0; Chad.v[1].n[2] = 0.0;
    Chad.v[2].n[0] = 0.0; Chad.v[2].n[1] = 0.0; Chad.v[2].n[2] = 0.0;

    cmsCIEXYZ Source;
    Source.X = 0.2; Source.Y = 0.3; Source.Z = 0.4;

    cmsCIEXYZ Dest;
    Dest.X = 0.5; Dest.Y = 0.6; Dest.Z = 0.7;

    cmsMAT3 Conversion;
    cmsBool ret = ComputeChromaticAdaptation(&Conversion, &Source, &Dest, &Chad);

    ASSERT_FALSE(ret == TRUE, "ComputeChromaticAdaptation should return FALSE when Chad is singular (inversion fails)");
}

// Test 3: Source white point has a zero component triggering false branch
// With identity Chad and Source.X = 0, the first determinant check should fail.
static void test_false_path_due_to_zero_source_component() {
    cmsMAT3 Chad;
    set_identity(Chad);

    cmsCIEXYZ Source;
    Source.X = 0.0;  // Zero component should trigger early FALSE
    Source.Y = 0.3;
    Source.Z = 0.4;

    cmsCIEXYZ Dest;
    Dest.X = 0.5;
    Dest.Y = 0.6;
    Dest.Z = 0.7;

    cmsMAT3 Conversion;
    cmsBool ret = ComputeChromaticAdaptation(&Conversion, &Source, &Dest, &Chad);

    ASSERT_FALSE(ret == TRUE, "ComputeChromaticAdaptation should return FALSE when Source white point has a zero X component");
}

// Test 4: Non-identity diagonal Chad should still yield the same diagonal ratios
// For a diagonal Chad with non-unit values, the resulting Conversion should equal diag(Dest/Source)
static void test_diag_chad_invariance() {
    cmsMAT3 Chad;
    set_diag(Chad, 2.0, 3.0, 4.0); // Diagonal Chad

    cmsCIEXYZ Source;
    Source.X = 0.2; Source.Y = 0.3; Source.Z = 0.4;

    cmsCIEXYZ Dest;
    Dest.X = 0.5; Dest.Y = 0.6; Dest.Z = 0.7;

    cmsMAT3 Conversion;
    cmsBool ret = ComputeChromaticAdaptation(&Conversion, &Source, &Dest, &Chad);

    // Expected diag ratios are Dest/Source again
    double rX = Dest.X / Source.X;
    double rY = Dest.Y / Source.Y;
    double rZ = Dest.Z / Source.Z;

    ASSERT_TRUE(ret == TRUE, "ComputeChromaticAdaptation should return TRUE for valid inputs with diagonal Chad");

    bool diag_ok = almost_equal(Conversion.v[0].n[0], rX) &&
                   almost_equal(Conversion.v[0].n[1], 0.0) &&
                   almost_equal(Conversion.v[0].n[2], 0.0) &&
                   almost_equal(Conversion.v[1].n[0], 0.0) &&
                   almost_equal(Conversion.v[1].n[1], rY) &&
                   almost_equal(Conversion.v[1].n[2], 0.0) &&
                   almost_equal(Conversion.v[2].n[0], 0.0) &&
                   almost_equal(Conversion.v[2].n[1], 0.0) &&
                   almost_equal(Conversion.v[2].n[2], rZ);

    ASSERT_TRUE(diag_ok, "Conversion should equal diag(Dest/Source) even with diagonal Chad, due to commutativity in this case");
}

int main() {
    // Step 1: Candidate keywords extracted from the focal method
    // Keywords: ComputeChromaticAdaptation, Chad (chromatic adaptation matrix),
    // SourceWhitePoint, DestWhitePoint, ConeSourceXYZ, ConeDestXYZ, ConeSourceRGB,
    // ConeDestRGB, Chad_Inv, Conversion, MATRIX_DET_TOLERANCE, _cmsMAT3inverse,
    // _cmsMAT3eval, _cmsVEC3init, _cmsMAT3per, _cmsAdaptationMatrix, etc.
    //
    // The tests below exercise the true/false branches of the ComputeChromaticAdaptation method
    // and validate the expected behavior for different Chad matrices and white points.

    // Run tests
    test_identity_chad_true_path();
    test_singular_chad_false_path();
    test_false_path_due_to_zero_source_component();
    test_diag_chad_invariance();

    // Summary
    std::cout << "Unit test run complete. Passed " << g_tests_passed
              << " of " << g_tests_total << " tests." << std::endl;

    return (g_tests_total == g_tests_passed) ? 0 : 1;
}