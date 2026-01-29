// test_check3d_trilinear16.cpp
// Lightweight C++11 test harness for the focal function:
// cmsInt32Number Check3DinterpolationTrilinear16(void)
// - No GTest/GMock, only standard C++11 and provided interface.
// - The test follows non-terminating assertions style: it reports pass/fail but does not abort.
// - Extern "C" is used to correctly link against the C function in testcms2.c.

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Forward declaration of the focal function (from testcms2.c).
// Ensure C linkage to avoid name mangling when linking with C++ code.
extern "C" cmsInt32Number Check3DinterpolationTrilinear16(void);

// Type alias for a test function pointer (no parameters, returns bool).
using TestFn = bool (*)();

// Helper macro-like utility for reporting test results without terminating execution.
static void ReportResult(const char* testName, bool passed)
{
    if (passed) {
        std::cout << "[OK] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

// Test 1: Basic invocation and expected success path.
// This directly exercises the focal method and expects it to return 1 (success).
// Rationale: Check3DinterpolationTrilinear16 runs a exhaustive comparison over all inputs
// and should complete with success if the interpolation is implemented correctly.
static bool Test_Check3DinterpolationTrilinear16_RunOnce()
{
    // Call the focal method.
    cmsInt32Number result = Check3DinterpolationTrilinear16();

    // Expecting success (return value == 1).
    bool pass = (result == 1);

    // Explanatory comment: This test verifies the primary, intended behavior.
    // It exercises the full 0..0xffff loop and ensures all channels match their inputs.
    ReportResult("Check3DinterpolationTrilinear16_RunOnce", pass);
    return pass;
}

// Test 2: Re-entrant invocation stability.
// Call the focal method twice in succession to ensure that internal global state
// (if any) does not cause a crash or inconsistent results. Both invocations should
// succeed independently.
static bool Test_Check3DinterpolationTrilinear16_Reentrant()
{
    cmsInt32Number r1 = Check3DinterpolationTrilinear16();
    cmsInt32Number r2 = Check3DinterpolationTrilinear16();

    bool pass = (r1 == 1) && (r2 == 1);

    ReportResult("Check3DinterpolationTrilinear16_Reentrant", pass);
    return pass;
}

// Simple test harness wrapper to map tests to a small suite structure.
struct TestCase {
    const char* name;
    TestFn    fn;
};

// Declare the test cases in the suite.
static TestCase g_tests[] = {
    { "Check3DinterpolationTrilinear16_RunOnce", Test_Check3DinterpolationTrilinear16_RunOnce },
    { "Check3DinterpolationTrilinear16_Reentrant", Test_Check3DinterpolationTrilinear16_Reentrant }
};

int main()
{
    const int totalTests = sizeof(g_tests) / sizeof(g_tests[0]);
    int failures = 0;

    // Run all tests; do not abort on first failure to maximize coverage.
    for (int i = 0; i < totalTests; ++i) {
        // Each test is responsible for printing its own result via ReportResult.
        bool ok = g_tests[i].fn();
        if (!ok) ++failures;
    }

    // Summary: non-terminating style (does not throw).
    if (failures == 0) {
        std::cout << "[SUMMARY] ALL " << totalTests << " tests PASSED." << std::endl;
    } else {
        std::cout << "[SUMMARY] " << (totalTests - failures) << " passed, " << failures
                  << " failed out of " << totalTests << " tests." << std::endl;
    }

    // Return non-zero if any test failed, typical convention for CI.
    return failures;
}