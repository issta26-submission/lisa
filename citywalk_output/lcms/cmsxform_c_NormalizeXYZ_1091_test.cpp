// Test suite for NormalizeXYZ in cmsxform.c
// This test uses a minimal harness (no GTest) and runs under C++11.
// It assumes the presence of the LittleCMS internal header (lcms2_internal.h)
// to supply the cmsCIEXYZ type. The tests call NormalizeXYZ via C linkage.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>


// Include library headers to obtain the cmsCIEXYZ definition.
// This header should be available in the include path when compiling against LittleCMS.

extern "C" {
    // Function under test: NormalizeXYZ
    void NormalizeXYZ(cmsCIEXYZ* Dest);
}

// Simple floating-point comparison with tolerance.
static bool approx_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Test 1: All components start greater than 2 and should be reduced multiple times.
// Expected final values: each component becomes 0.5 after 3 divisions by 10.
bool test_AllLarge_ShouldNormalizeTo0_5() {
    // Initialize with large values
    cmsCIEXYZ d;
    d.X = 500.0;
    d.Y = 500.0;
    d.Z = 500.0;

    NormalizeXYZ(&d);

    bool ok = approx_equal(d.X, 0.5) &&
              approx_equal(d.Y, 0.5) &&
              approx_equal(d.Z, 0.5);

    if (!ok) {
        std::cerr << "FAIL: test_AllLarge_ShouldNormalizeTo0_5 - "
                  << "Got X=" << d.X << ", Y=" << d.Y << ", Z=" << d.Z << "\n";
    }
    return ok;
}

// Test 2: Mixed values where only the first iteration satisfies the loop predicate.
// Expected final values: X becomes 10, Y becomes 0.4, Z becomes 0.3 after 1 iteration.
bool test_MixedValues_OneIterationOnly() {
    cmsCIEXYZ d;
    d.X = 100.0;
    d.Y = 4.0;
    d.Z = 3.0;

    NormalizeXYZ(&d);

    bool ok = approx_equal(d.X, 10.0) &&
              approx_equal(d.Y, 0.4) &&
              approx_equal(d.Z, 0.3);

    if (!ok) {
        std::cerr << "FAIL: test_MixedValues_OneIterationOnly - "
                  << "Got X=" << d.X << ", Y=" << d.Y << ", Z=" << d.Z << "\n";
    }
    return ok;
}

// Test 3: Not all components are greater than 2 initially; the loop should not execute.
// Expected final values: input values remain unchanged.
bool test_NoLoop_WhenXNotLarge() {
    cmsCIEXYZ d;
    d.X = 1.0;
    d.Y = 3.0;
    d.Z = 5.0;

    NormalizeXYZ(&d);

    bool ok = approx_equal(d.X, 1.0) &&
              approx_equal(d.Y, 3.0) &&
              approx_equal(d.Z, 5.0);

    if (!ok) {
        std::cerr << "FAIL: test_NoLoop_WhenXNotLarge - "
                  << "Got X=" << d.X << ", Y=" << d.Y << ", Z=" << d.Z << "\n";
    }
    return ok;
}

// Test 4: All components exactly equal to 2; loop condition uses > 2, so no iteration.
// Expected final values: input values remain unchanged.
bool test_NoLoop_WhenAllEqual2() {
    cmsCIEXYZ d;
    d.X = 2.0;
    d.Y = 2.0;
    d.Z = 2.0;

    NormalizeXYZ(&d);

    bool ok = approx_equal(d.X, 2.0) &&
              approx_equal(d.Y, 2.0) &&
              approx_equal(d.Z, 2.0);

    if (!ok) {
        std::cerr << "FAIL: test_NoLoop_WhenAllEqual2 - "
                  << "Got X=" << d.X << ", Y=" << d.Y << ", Z=" << d.Z << "\n";
    }
    return ok;
}

int main() {
    int total_tests = 4;
    int failed_tests = 0;

    std::cout << "Running NormalizeXYZ test suite...\n";

    if (!test_AllLarge_ShouldNormalizeTo0_5()) ++failed_tests;
    if (!test_MixedValues_OneIterationOnly()) ++failed_tests;
    if (!test_NoLoop_WhenXNotLarge()) ++failed_tests;
    if (!test_NoLoop_WhenAllEqual2()) ++failed_tests;

    int passed = total_tests - failed_tests;
    std::cout << "NormalizeXYZ tests completed. Passed: " << passed
              << ", Failed: " << failed_tests << "\n";

    return failed_tests ? 1 : 0;
}