// Test suite for the focal method CheckOneRGB_f from testcms2.c
// This test uses LittleCMS to create a simple identity RGB transform and
// verifies both true and false branches of the predicate logic inside CheckOneRGB_f.
// The tests are written in C++11 and do not rely on any testing framework (GTest not used).
// The main function manually invokes all tests and prints a summary.

// Note: This test assumes the LittleCMS library is available in the environment.
// Compile example (typical): 
// g++ -std=c++11 -o test_suite test_suite.cpp -llcms2

#include <cstdio>
#include <vector>
#include <iostream>
#include <testcms2.h>
#include <lcms2.h>
#include <cmath>


// LittleCMS headers

// Prototype for the focal method CheckOneRGB_f from testcms2.c
// We declare it with C linkage since the implementation is in C.
extern "C" cmsInt32Number CheckOneRGB_f(cmsHTRANSFORM xform,
                                     cmsInt32Number R, cmsInt32Number G, cmsInt32Number B,
                                     cmsFloat64Number X, cmsFloat64Number Y, cmsFloat64Number Z,
                                     cmsFloat64Number err);

// Helper: Create a simple identity transform (sRGB -> sRGB) for testing.
// Returns a transform handle that should be deleted by cmsDeleteTransform after use.
static cmsHTRANSFORM CreateIdentityTransform() {
    cmsHPROFILE In  = cmsCreate_sRGBProfile();
    cmsHPROFILE Out = cmsCreate_sRGBProfile();
    if (!In || !Out) {
        if (In) cmsCloseProfile(In);
        if (Out) cmsCloseProfile(Out);
        return nullptr;
    }

    // Use 8-bit per channel RGB input/output
    cmsHTRANSFORM xform = cmsCreateTransform(In, TYPE_RGB_8, Out, TYPE_RGB_8, INTENT_PERCEPTUAL, 0);

    // Release sources
    cmsCloseProfile(In);
    cmsCloseProfile(Out);

    return xform;
}

// Test 1: Identity transform should return true when X,Y,Z exactly match the normalized input RGB.
// This covers the "true" branch of all three IsGoodVal checks.
static bool test_CheckOneRGB_f_identity_true() {
    // Create an identity transform
    cmsHTRANSFORM xform = CreateIdentityTransform();
    if (xform == nullptr) {
        std::cerr << "Failed to create identity transform for Test 1\n";
        return false;
    }

    // Input RGB: R=128, G=0, B=255
    const cmsInt32Number R = 128;
    const cmsInt32Number G = 0;
    const cmsInt32Number B = 255;

    // Expected outputs are normalized to [0,1]
    const cmsFloat64Number X = (cmsFloat64Number)R / 255.0;
    const cmsFloat64Number Y = (cmsFloat64Number)G / 255.0;
    const cmsFloat64Number Z = (cmsFloat64Number)B / 255.0;

    const cmsFloat64Number err = 0.01; // generous tolerance to ensure true evaluation

    cmsInt32Number res = CheckOneRGB_f(xform, R, G, B, X, Y, Z, err);

    cmsDeleteTransform(xform);

    return res != 0;
}

// Test 2: False path on the first predicate (X value). Ensure short-circuiting works when the first check fails.
// Use the same identity transform but perturb X slightly beyond the allowed error.
static bool test_CheckOneRGB_f_identity_false_X() {
    cmsHTRANSFORM xform = CreateIdentityTransform();
    if (xform == nullptr) {
        std::cerr << "Failed to create identity transform for Test 2\n";
        return false;
    }

    // Input RGB: R=128, G=0, B=255
    const cmsInt32Number R = 128;
    const cmsInt32Number G = 0;
    const cmsInt32Number B = 255;

    // Exact expected outputs
    const cmsFloat64Number X_exact = (cmsFloat64Number)R / 255.0;
    const cmsFloat64Number Y = (cmsFloat64Number)G / 255.0;
    const cmsFloat64Number Z = (cmsFloat64Number)B / 255.0;

    // Introduce a deliberate large error in X to force the first predicate to fail
    const cmsFloat64Number err = 0.0005;
    const cmsFloat64Number X_bad = X_exact + 0.05; // large deviation

    cmsInt32Number res = CheckOneRGB_f(xform, R, G, B, X_bad, Y, Z, err);

    cmsDeleteTransform(xform);

    return res == 0;
}

// Test 3: False path on the second predicate (Y value) while first predicate passes.
// We adjust X to be within tolerance, but set Y far from Out[1].
static bool test_CheckOneRGB_f_identity_false_Y() {
    cmsHTRANSFORM xform = CreateIdentityTransform();
    if (xform == nullptr) {
        std::cerr << "Failed to create identity transform for Test 3\n";
        return false;
    }

    // Input RGB: R=128, G=0, B=255
    const cmsInt32Number R = 128;
    const cmsInt32Number G = 0;
    const cmsInt32Number B = 255;

    const cmsFloat64Number X = ((cmsFloat64Number)R) / 255.0;
    const cmsFloat64Number Y_correct = ((cmsFloat64Number)G) / 255.0;
    const cmsFloat64Number Z = ((cmsFloat64Number)B) / 255.0;

    // Make X correct within tolerance
    // Introduce a small adjustment to X to ensure first predicate passes
    const cmsFloat64Number X = X_correct + 0.0004;

    // Deliberate large error in Y to trigger second predicate failure
    const cmsFloat64Number err = 0.001;  // still small
    const cmsFloat64Number Y_bad = Y_correct + 0.05; // large deviation

    cmsInt32Number res = CheckOneRGB_f(xform, R, G, B, X, Y_bad, Z, err);

    cmsDeleteTransform(xform);

    return res == 0;
}

// Test 4: False path on the third predicate (Z value) while first two predicates pass.
// We adjust X and Y to be within tolerance, but set Z far from Out[2].
static bool test_CheckOneRGB_f_identity_false_Z() {
    cmsHTRANSFORM xform = CreateIdentityTransform();
    if (xform == nullptr) {
        std::cerr << "Failed to create identity transform for Test 4\n";
        return false;
    }

    // Input RGB: R=0, G=0, B=0
    const cmsInt32Number R = 0;
    const cmsInt32Number G = 0;
    const cmsInt32Number B = 0;

    const cmsFloat64Number X = 0.0;
    const cmsFloat64Number Y = 0.0;
    // Correct Z would be 0.0 for B=0
    const cmsFloat64Number Z_correct = 0.0;

    // Make Z incorrect
    const cmsFloat64Number Z_bad = Z_correct + 0.5;

    const cmsFloat64Number err = 0.01;

    cmsInt32Number res = CheckOneRGB_f(xform, R, G, B, X, Y, Z_bad, err);

    cmsDeleteTransform(xform);

    return res == 0;
}

// Simple test harness
struct TestCase {
    const char* name;
    bool (*func)();
};

// Main function: runs all tests and reports results
int main() {
    std::vector<TestCase> tests = {
        {"CheckOneRGB_f_identity_true", test_CheckOneRGB_f_identity_true},
        {"CheckOneRGB_f_identity_false_X", test_CheckOneRGB_f_identity_false_X},
        {"CheckOneRGB_f_identity_false_Y", test_CheckOneRGB_f_identity_false_Y},
        {"CheckOneRGB_f_identity_false_Z", test_CheckOneRGB_f_identity_false_Z},
    };

    int passed = 0;
    for (const auto& tc : tests) {
        bool ok = tc.func();
        if (ok) {
            std::cout << "[OK]   " << tc.name << "\n";
            ++passed;
        } else {
            std::cout << "[FAIL] " << tc.name << "\n";
        }
    }

    std::cout << "Summary: " << passed << " / " << tests.size() << " tests passed.\n";
    return (passed == (int)tests.size()) ? 0 : 1;
}