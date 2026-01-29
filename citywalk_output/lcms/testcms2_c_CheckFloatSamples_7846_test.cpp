// C++11 Test Suite for the focal method: CheckFloatSamples
// Approach:
// - Use a lightweight, non-terminating assertion style (print on failure, continue).
// - Expose C APIs from the tested code via extern "C" to avoid name mangling.
// - Provide two focused tests:
//    1) Validate that CheckFloatSamples() returns a non-zero value (expected success).
//    2) Sanity-check the auxiliary C API cmsBuildTabulatedToneCurveFloat to ensure a basic,
//
//       non-crashing construction path is usable (memory is freed properly).
// - All tests are invoked from main() as required by the domain knowledge constraints.
// - Minimal, explanatory comments accompany each test to describe intent and behavior.

#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Bring in the C-Test CMS API declarations. The test CMS code is written in C,
// so we wrap the include in extern "C" to ensure proper linkage when compiled as C++.
extern "C" {
                    // Also declares the function under test: CheckFloatSamples(void)
                    // and additional helpers used by the test (cmsBuildTabulatedToneCurveFloat, cmsFreeToneCurve).
}

// Note: We rely on the existing C test harness code included via testcms2.h.
// The function CheckFloatSamples is defined in the focal code segment and uses
// internal helpers (StraightLine, cmsBuildTabulatedToneCurveFloat, TestCurve, cmsFreeToneCurve).

// =====================
// Test 1: CheckFloatSamples_ReturnsOne
// Intent: Ensure the focal method CheckFloatSamples returns a non-zero value
// (i.e., the test curve comparison succeeded). This validates the primary
// execution path and that the function integrates the dependent pieces
// (tone-curve construction, comparison, and cleanup) correctly.
// =====================
bool test_CheckFloatSamples_ReturnsOne() {
    // Call the function under test (C code compiled elsewhere)
    cmsInt32Number result = CheckFloatSamples();

    if (result == 1) {
        // Passed: the function returned the expected "success" indicator.
        return true;
    } else {
        // Non-terminating diagnostic: print details and continue with other tests.
        std::cerr << "[Test] CheckFloatSamples_ReturnsOne: FAILED (expected 1, got "
                  << result << ")" << std::endl;
        return false;
    }
}

// =====================
// Test 2: cmsBuildTabulatedToneCurveFloat_Sanity
// Intent: Perform a small, self-contained sanity check on a related C API
// used by the focal method. This helps exercise the dependent code path that
// builds a tiny tone-curve and frees it, ensuring basic memory management
// and API stability without requiring the full CheckFloatSamples flow.
// This complements Test 1 by increasing coverage of the curve-related helpers.
// =====================
bool test_BuildTabulatedToneCurveFloat_Sanity() {
    // Prepare a tiny 2-point table: maps 0.0 -> 0.0 and 1.0 -> 1.0
    const cmsUInt32Number nPoints = 2;
    cmsFloat32Number y[2] = { 0.0f, 1.0f };

    // Call into the C API to build a tabulated tone curve from the provided samples
    cmsToneCurve* curve = cmsBuildTabulatedToneCurveFloat(NULL, nPoints, y);

    bool ok = (curve != NULL);
    if (curve) {
        // Free the allocated tone curve to avoid leaks
        cmsFreeToneCurve(curve);
    } else {
        std::cerr << "[Test] BuildTabulatedToneCurveFloat_Sanity: FAILED (curve == NULL)" << std::endl;
    }

    return ok;
}

// =====================
// Main: Run the tests
// Domain knowledge instruction: "If gtest isn't allowed, call test methods from the main function."
// This main function executes all defined tests and reports a summary.
// =====================
int main(int argc, char* argv[]) {
    int totalTests = 0;
    int passedTests = 0;

    // Test 1
    std::cout << "Running Test 1: CheckFloatSamples_ReturnsOne..." << std::endl;
    ++totalTests;
    if (test_CheckFloatSamples_ReturnsOne()) {
        ++passedTests;
        std::cout << "[PASS] Test 1" << std::endl;
    } else {
        std::cout << "[FAIL] Test 1" << std::endl;
    }

    // Test 2
    std::cout << "Running Test 2: BuildTabulatedToneCurveFloat_Sanity..." << std::endl;
    ++totalTests;
    if (test_BuildTabulatedToneCurveFloat_Sanity()) {
        ++passedTests;
        std::cout << "[PASS] Test 2" << std::endl;
    } else {
        std::cout << "[FAIL] Test 2" << std::endl;
    }

    // Summary
    std::cout << "Test Summary: " << passedTests << " / " << totalTests << " tests passed." << std::endl;

    // Exit status: 0 if all tests passed, non-zero otherwise
    return (passedTests == totalTests) ? 0 : 1;
}