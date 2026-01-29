/*
Step 1 notes (Candidate Keywords extracted from the focal method):
- cmsInt32Number Check3StageLUT(void)
- cmsPipelineAlloc
- DbgThread
- AddIdentityMatrix
- AddIdentityCLUTfloat
- Add3GammaCurves
- CheckFullLUT
- lut (cmsPipeline*)
- 3 (ExpectedStages parameter to CheckFullLUT)

These elements define the core dependencies and flow of Check3StageLUT:
- Allocate a 3x3 pipeline with cmsPipelineAlloc(DbgThread(), 3, 3)
- Initialize with identity matrix and identity CLUT (float)
- Apply 3 gamma curves with gamma value 1.0
- Validate using CheckFullLUT(lut, 3)

The tests below rely on the actual Little CMS API surfaces exposed via testcms2.h.
*/

#include <iostream>
#include <cstdint>
#include <testcms2.h>


extern "C" {
}

/*
Test harness for Check3StageLUT:
- We avoid terminating the test on failure (non-terminating assertions).
- We implement three tests:
  1) Basic return value sanity for Check3StageLUT (0 or 1 expected)
  2) Correctness check by mirroring the function's internals and comparing results
  3) Idempotence: multiple calls yield identical results
*/

// Helper to log test results without aborting on failure
static void logTestResult(const char* name, bool ok)
{
    if (ok) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        std::cerr << "[FAIL] " << name << std::endl;
    }
}

// Test 1: Basic sanity check for Check3StageLUT return value.
// Rationale: The function should return a valid cmsInt32Number (commonly 0 or 1 in this codebase).
static bool Test_Check3StageLUT_Basic_ReturnValue(void)
{
    cmsInt32Number res = Check3StageLUT();
    bool isValid = (res == 0 || res == 1);
    if (!isValid) {
        std::cerr << "Check3StageLUT returned invalid value: " << res << std::endl;
    }
    logTestResult("Test_Check3StageLUT_Basic_ReturnValue", isValid);
    return isValid;
}

// Test 2: Compare Check3StageLUT result with a manually constructed equivalent pipeline.
// Rationale: Build the same pipeline steps as inside Check3StageLUT and compare results.
// This validates that Check3StageLUT implements the expected sequence/logic.
static bool Test_Check3StageLUT_EqualsManualPipeline(void)
{
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    if (lut == nullptr) {
        std::cerr << "Failed to allocate cmsPipeline in test." << std::endl;
        logTestResult("Test_Check3StageLUT_EqualsManualPipeline", false);
        return false;
    }

    AddIdentityMatrix(lut);
    AddIdentityCLUTfloat(lut);
    Add3GammaCurves(lut, 1.0);

    cmsInt32Number expected = CheckFullLUT(lut, 3);
    cmsInt32Number actual   = Check3StageLUT();

    cmsPipelineFree(DbgThread(), lut);

    bool ok = (expected == actual);
    if (!ok) {
        std::cerr << "Manual pipeline check mismatch: expected=" << expected
                  << " actual=" << actual << std::endl;
    }
    logTestResult("Test_Check3StageLUT_EqualsManualPipeline", ok);
    return ok;
}

// Test 3: Idempotence check.
// Rationale: Repeated calls to Check3StageLUT should yield the same result.
static bool Test_Check3StageLUT_Idempotence(void)
{
    cmsInt32Number first  = Check3StageLUT();
    cmsInt32Number second = Check3StageLUT();
    bool ok = (first == second);
    if (!ok) {
        std::cerr << "Check3StageLUT idempotence failed: first=" << first
                  << " second=" << second << std::endl;
    }
    logTestResult("Test_Check3StageLUT_Idempotence", ok);
    return ok;
}

int main(void)
{
    // Run all tests and produce a summary
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool ok) {
        ++total;
        if (ok) ++passed;
        logTestResult(name, ok);
    };

    // Execute tests
    run("Test_Check3StageLUT_Basic_ReturnValue", Test_Check3StageLUT_Basic_ReturnValue());
    run("Test_Check3StageLUT_EqualsManualPipeline", Test_Check3StageLUT_EqualsManualPipeline());
    run("Test_Check3StageLUT_Idempotence", Test_Check3StageLUT_Idempotence());

    // Summary
    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}