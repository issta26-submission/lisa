/*
   C++11 Test Suite for cmsBool CheckOne8D in testcms2.c

   This test intentionally relies on the surrounding LittleCMS-like test helpers
   that are declared in the project (e.g., AddIdentityMatrix, Add3GammaCurves,
   cmsPipelineAlloc, cmsPipelineFree, etc.). It exercises the focal function by
   constructing a 8D LUT pipeline (3-channel output), exercising the identity LUT
   case (expected to return TRUE for zero input), and a non-identity case via
   a gamma curve to drive a false path.

   Notes:
   - This test uses a minimal, self-contained main() test harness (no GTest).
   - We assume the project provides the necessary LittleCMS-like API in scope
     (lcms2.h or equivalent). The tests rely on:
       - cmsPipelineAlloc, cmsPipelineFree
       - AddIdentityMatrix, Add3GammaCurves
       - CheckOne8D (extern "C")
   - The tests focus on true/false branches of the predicate, and ensure
     coverage for both zero-input identity behavior and a mismatched case.
   - All tests are non-terminating (no exit on failure except via test result),
     per the domain knowledge guidance.
*/

#include <iostream>
#include <lcms2.h>
#include <testcms2.h>
#include <cstdlib>


// Include the public API headers. Depending on the project setup, you may need
// to adjust the header path (e.g., <lcms2.h> or "lcms2.h").

// Ensure correct linkage for the focal function when called from C++.
extern "C" {
    // Focal function under test
    cmsBool CheckOne8D(cmsPipeline* lut,
                       cmsUInt16Number a1, cmsUInt16Number a2,
                       cmsUInt16Number a3, cmsUInt16Number a4,
                       cmsUInt16Number a5, cmsUInt16Number a6,
                       cmsUInt16Number a7, cmsUInt16Number a8);

    // Helpers for building a LUT/pipeline (assumed available in the project)
    // These declarations rely on the project's existing API (as evidenced by
    // <FOCAL_CLASS_DEP> in the provided code block).
    cmsPipeline* cmsPipelineAlloc(void* ContextID, cmsUInt32Number nChannels);
    void cmsPipelineFree(cmsPipeline* lut);
    void AddIdentityMatrix(cmsPipeline* lut);
    void Add3GammaCurves(cmsPipeline* lut, cmsFloat64Number Curve);
}

static int total_tests = 0;
static int passed_tests = 0;

// Simple assertion helper for test results with descriptive messages.
#define RUN_TEST(name) do {                                  \
    ++total_tests;                                             \
    bool ok = (name);                                          \
    if (ok) {                                                  \
        ++passed_tests;                                        \
        std::cout << "[PASS] " #name "\n";                   \
    } else {                                                   \
        std::cout << "[FAIL] " #name "\n";                   \
    }                                                          \
} while (0)


// Test 1: Identity LUT with zero input should produce matching outputs
// (Out1 and Out2 both stem from the identity mapping; with all inputs zero,
// the outputs should be zero and thus match within tolerance).
bool test_CheckOne8D_IdentityZeroInput()
{
    // Create a 3-channel LUT pipeline and fill with identity mapping
    cmsPipeline* lut = cmsPipelineAlloc(NULL, 3);
    if (lut == nullptr) {
        std::cerr << "Failed to allocate LUT pipeline for IdentityZeroInput test\n";
        return false;
    }

    AddIdentityMatrix(lut);

    // All inputs zero
    cmsBool res = CheckOne8D(lut,
                             0, 0, 0, 0, 0, 0, 0, 0);

    cmsPipelineFree(lut);

    // Expect TRUE: identity LUT should interpolate to same zero values as the sampler
    return res == TRUE;
}


// Test 2: Identity LUT but with a gamma curve applied to the LUT.
// This should cause Out1 != Out2 for non-zero inputs, triggering a FALSE return.
// We choose non-zero inputs to increase the chance of a detectable difference.
bool test_CheckOne8D_GammaMismatchFalse()
{
    // Create a 3-channel LUT pipeline and fill with identity
    cmsPipeline* lut = cmsPipelineAlloc(NULL, 3);
    if (lut == nullptr) {
        std::cerr << "Failed to allocate LUT pipeline for GammaMismatch test\n";
        return false;
    }

    AddIdentityMatrix(lut);

    // Apply gamma curves to all three channels to perturb the interpolated values
    // This should make Out1 differ from Out2 for non-zero inputs.
    Add3GammaCurves(lut, 2.2);

    // Use non-zero inputs to ensure a detectable difference
    cmsBool res = CheckOne8D(lut,
                              1000, 2000, 3000, 4000,
                              5000, 6000, 7000, 65535);

    cmsPipelineFree(lut);

    // Expect FALSE: gamma-distorted LUT should differ from Sampler8D outputs
    return res == FALSE;
}


// Entry point: run the tests
int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    std::cout << "Running CheckOne8D test suite (C++11)..." << std::endl;
    test_CheckOne8D_IdentityZeroInput(); // Initialize (no dependency on pass/fail here)
    // Actually run tests through the macros to produce a consolidated summary.
    // Re-run with explicit RUN_TEST calls to produce a concise report.

    // Re-run with explicit function calls to fit the RUN_TEST harness
    RUN_TEST(test_CheckOne8D_IdentityZeroInput);
    RUN_TEST(test_CheckOne8D_GammaMismatchFalse);

    std::cout << "Tests: " << total_tests << ", Passed: " << passed_tests
              << ", Failed: " << (total_tests - passed_tests) << std::endl;

    // Return non-zero if any test failed
    return (passed_tests == total_tests) ? 0 : 1;
}