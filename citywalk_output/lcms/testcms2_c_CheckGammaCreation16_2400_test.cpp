/*
  Test suite for the focal method CheckGammaCreation16 and related gamma utilities
  as used in testcms2.c environment.

  Step 1 - Candidate Keywords (represent core dependent components):
  - LinGamma, cmsBuildGamma, cmsEvalToneCurve16, cmsFreeToneCurve
  - CheckGammaEstimation, cmsToneCurve
  - DbgThread (thread contextFor gamma creation)
  - gamma values: 1.0 (true path), 2.2 (false path test for CheckGammaEstimation)
  - Fail (internal function used by the focal method on mismatch)
  - Test harness: Check, cmsInt32Number return types, cmsFloat64Number

  Step 2/3 - Tests cover:
  - CheckGammaCreation16 true path (normative behavior)
  - CheckGammaEstimation true/false branches via direct tests of dependency
  - Main driver uses the provided test harness to maximize coverage and maintain style consistency
  - Note: This file is designed to be compiled in a C++11 environment but uses the C API of LittleCMS
*/

extern "C" {
#include <stdlib.h>
#include <lcms2.h>
#include <testcms2.h>

}

// Expose test functions with C linkage for the harness
extern "C" {

// Wrapper to exercise the focal method directly; returns 1 on success, 0 on failure.
// This function delegates to the actual CheckGammaCreation16 in the tested module.
cmsInt32Number Test_CheckGammaCreation16_Works(void) {
    return CheckGammaCreation16();
}

// Direct test for the true branch of CheckGammaEstimation when gamma matches.
// Builds a gamma of 1.0 and verifies estimation with 1.0 yields true.
cmsInt32Number Test_CheckGammaEstimation_True(void) {
    cmsToneCurve* c = cmsBuildGamma(DbgThread(), 1.0);
    cmsInt32Number ok = CheckGammaEstimation(c, 1.0);
    cmsFreeToneCurve(c);
    // Return 1 if estimation is correct, 0 otherwise
    return ok ? 1 : 0;
}

// Direct test for the false branch of CheckGammaEstimation when gamma does not match.
// Builds a gamma of 2.2 and verifies estimation with 1.0 yields false.
cmsInt32Number Test_CheckGammaEstimation_False(void) {
    cmsToneCurve* c = cmsBuildGamma(DbgThread(), 2.2);
    cmsInt32Number ok = CheckGammaEstimation(c, 1.0);
    cmsFreeToneCurve(c);
    // Return 1 if estimation incorrectly passes (should be 0),
    // otherwise return 0 to indicate the false branch was taken.
    return ok ? 0 : 1;
}

// Optional: Additional targeted tests can be added here following the same pattern.

} // extern "C"

// Main driver invoking the test harness to collect results.
// If the harness supports multiple tests, we register them here to maximize coverage.
int main(int argc, char* argv[]) {

    // Test: GammaCreation16 - true path (identity lin gamma test)
    Check("CheckGammaCreation16_Works", Test_CheckGammaCreation16_Works);

    // Test: GammaEstimation - true branch
    Check("CheckGammaEstimation_True", Test_CheckGammaEstimation_True);

    // Test: GammaEstimation - false branch
    Check("CheckGammaEstimation_False", Test_CheckGammaEstimation_False);

    // Program exits; harness reports individual test outcomes.
    return 0;
}