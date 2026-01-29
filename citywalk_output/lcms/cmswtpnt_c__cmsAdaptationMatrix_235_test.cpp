// Lightweight unit tests for the focal method cmsAdaptToIlluminant (and indirectly _cmsAdaptationMatrix)
 // This test suite is designed for a C++11 environment without GoogleTest.
 // It uses a small, non-terminating test harness that reports failures and continues execution.
 // The tests rely on the LittleCMS-like API (lcms2.h / lcms2_internal.h) and the public function:
 //   cmsBool cmsAdaptToIlluminant(cmsCIEXYZ* Result, const cmsCIEXYZ* SourceWhitePt, const cmsCIEXYZ* Illuminant, const cmsCIEXYZ* Value)

#include <cstring>
#include <cstdio>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


extern "C" {
}

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

static bool almostEqual(double a, double b, double eps = 1e-4) {
    return std::fabs(a - b) <= eps;
}

// Test 1: Identity adaptation when SourceWhitePt == Illuminant
// If there is no chromatic adaptation required, the result of transforming any Value
// should be identical to Value.
bool test_identity_case() {
    // Define a representative white point (D65-like values) and identical illuminant
    cmsCIEXYZ Source;
    Source.X = 0.3127;
    Source.Y = 0.3290;
    Source.Z = 0.3583;

    cmsCIEXYZ Illuminant = Source; // Same as SourceWhitePt

    // Arbitrary test color value
    cmsCIEXYZ Value;
    Value.X = 0.10;
    Value.Y = 0.20;
    Value.Z = 0.30;

    cmsCIEXYZ Result;
    g_total_tests++;

    cmsBool ok = cmsAdaptToIlluminant(&Result, &Source, &Illuminant, &Value);
    if (ok != TRUE) {
        std::fprintf(stderr, "test_identity_case: cmsAdaptToIlluminant() reported FALSE (expected TRUE).\n");
        g_failed_tests++;
        return false;
    }

    // Expect Result == Value (within tolerance)
    bool eqX = almostEqual((double)Result.X, (double)Value.X);
    bool eqY = almostEqual((double)Result.Y, (double)Value.Y);
    bool eqZ = almostEqual((double)Result.Z, (double)Value.Z);

    if (!(eqX && eqY && eqZ)) {
        std::fprintf(stderr,
            "test_identity_case: mismatch. Value=(%.6f,%.6f,%.6f) -> Result=(%.6f,%.6f,%.6f)\n",
            Value.X, Value.Y, Value.Z, Result.X, Result.Y, Result.Z);
        g_failed_tests++;
        return false;
    }

    return true;
}

// Test 2: Non-identity adaptation when SourceWhitePt != Illuminant
// Choose distinct Source and Illuminant to ensure adaptation path is exercised and yields a different result
bool test_non_identity_case() {
    CmsBool ok;
    // Distinct white points
    cmsCIEXYZ Source;
    Source.X = 0.3127;
    Source.Y = 0.3290;
    Source.Z = 0.3583;

    cmsCIEXYZ Illuminant;
    Illuminant.X = 0.4000;
    Illuminant.Y = 0.3500;
    Illuminant.Z = 0.4500;

    // A representative color value
    cmsCIEXYZ Value;
    Value.X = 0.150;
    Value.Y = 0.250;
    Value.Z = 0.350;

    cmsCIEXYZ Result;
    g_total_tests++;

    ok = cmsAdaptToIlluminant(&Result, &Source, &Illuminant, &Value);
    if (ok != TRUE) {
        std::fprintf(stderr, "test_non_identity_case: cmsAdaptToIlluminant() reported FALSE (expected TRUE).\n");
        g_failed_tests++;
        return false;
    }

    // Expect Result to differ from Value for non-identical illuminants
    bool diffX = !almostEqual((double)Result.X, (double)Value.X);
    bool diffY = !almostEqual((double)Result.Y, (double)Value.Y);
    bool diffZ = !almostEqual((double)Result.Z, (double)Value.Z);

    if (!(diffX || diffY || diffZ)) {
        std::fprintf(stderr,
            "test_non_identity_case: adaptation did not change the value as expected. Value=(%.6f,%.6f,%.6f) -> Result=(%.6f,%.6f,%.6f)\n",
            Value.X, Value.Y, Value.Z, Result.X, Result.Y, Result.Z);
        g_failed_tests++;
        return false;
    }

    return true;
}

// Optional: Test 3 (additional coverage) - ensure that multiple calls with the same inputs produce consistent results
// This helps exercise internal caching or deterministic behavior of the adaptation pipeline.
bool test_consistency_case() {
    // Use a fixed pair of white points
    cmsCIEXYZ Source;
    Source.X = 0.3127;
    Source.Y = 0.3290;
    Source.Z = 0.3583;

    cmsCIEXYZ Illuminant;
    Illuminant.X = 0.3127;
    Illuminant.Y = 0.3290;
    Illuminant.Z = 0.3583;

    cmsCIEXYZ Value;
    Value.X = 0.05;
    Value.Y = 0.15;
    Value.Z = 0.25;

    cmsCIEXYZ Result1, Result2;
    g_total_tests++;

    cmsBool ok1 = cmsAdaptToIlluminant(&Result1, &Source, &Illuminant, &Value);
    cmsBool ok2 = cmsAdaptToIlluminant(&Result2, &Source, &Illuminant, &Value);

    if (ok1 != TRUE || ok2 != TRUE) {
        std::fprintf(stderr, "test_consistency_case: cmsAdaptToIlluminant() failed on one or both calls.\n");
        g_failed_tests++;
        return false;
    }

    // Results should be identical across calls (deterministic)
    bool sameX = almostEqual((double)Result1.X, (double)Result2.X);
    bool sameY = almostEqual((double)Result1.Y, (double)Result2.Y);
    bool sameZ = almostEqual((double)Result1.Z, (double)Result2.Z);

    if (!(sameX && sameY && sameZ)) {
        std::fprintf(stderr, "test_consistency_case: results differ between identical inputs.\n");
        g_failed_tests++;
        return false;
    }

    return true;
}

// Helper to print summary
void print_summary() {
    std::fprintf(stderr, "\nTest summary: %d total, %d failed.\n",
                 g_total_tests, g_failed_tests);
}

// Entry point
int main() {
    // Run tests
    bool t1 = test_identity_case();
    bool t2 = test_non_identity_case();
    bool t3 = test_consistency_case();

    print_summary();

    if (t1 && t2 && t3 && g_failed_tests == 0)
        std::printf("All tests passed.\n");
    else
        std::printf("Some tests failed. See details above.\n");

    // Return non-zero if any test failed
    return (g_failed_tests > 0) ? 1 : 0;
}