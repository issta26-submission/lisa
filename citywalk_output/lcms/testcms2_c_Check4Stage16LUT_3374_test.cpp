// Test suite for the focal method: Check4Stage16LUT
// This suite is written in C++11 without any external testing framework (no GTest).
// It directly calls the C function Check4Stage16LUT exposed from testcms2.c via C linkage.
// The tests aim to verify the basic contract of the focal method (non-zero return on success)
// and to exercise stability across repeated invocations.
//
// Key dependencies (Candidate Keywords) inferred from the focal method:
// - cmsPipelineAlloc
// - DbgThread
// - AddIdentityMatrix
// - AddIdentityCLUT16
// - Add3GammaCurves
// - CheckFullLUT
// - lut stage count (4) used in CheckFullLUT
// These are the elements that underlie the focal method's behavior.

#include <iostream>
#include <string>
#include <testcms2.h>


// Declare the focal method with C linkage to link against the C implementation.
// The true return type in the C code is cmsInt32Number; using int is appropriate for C linkage.
extern "C" int Check4Stage16LUT(void);

// Simple, lightweight test harness (non-terminating assertions).
// Keeps a tally of passed/failed tests and reports after running all tests.

static int g_totalTests = 0;
static int g_failedTests = 0;

// Helper to run a named test function and report the result.
// The test function should return true on success, false on failure.
static void RunTest(const std::string& testName, bool (*testFn)()) {
    ++g_totalTests;
    bool ok = testFn();
    if (ok) {
        std::cout << "PASS: " << testName << std::endl;
    } else {
        std::cerr << "FAIL: " << testName << std::endl;
        ++g_failedTests;
    }
}

/*
Test 1: Basic sanity check for Check4Stage16LUT
- Rationale: The focal method is expected to construct a 3x3 LUT pipeline with
  an identity matrix, a 16-entry CLUT, and 3 gamma curves, then verify via CheckFullLUT.
- Assertion: The function should return a non-zero value indicating success under normal conditions.
- Notes: This test exercises the intended happy-path and ensures the function produces a valid result.
*/
static bool test_Check4Stage16LUT_basic() {
    int ret = Check4Stage16LUT();
    // Expect a successful check (non-zero). If the library changes, this assertion will catch it.
    if (ret != 0) {
        return true;
    } else {
        // Provide a clear diagnostic for easier debugging.
        std::cerr << "Check4Stage16LUT returned 0 (expected non-zero on success)." << std::endl;
        return false;
    }
}

/*
Test 2: Stability across multiple invocations
- Rationale: Repeated invocations should not crash or degrade, and should consistently
  return a non-zero value for a valid LUT configuration.
- Assertion: Each call returns non-zero; if any invocation fails, the test fails.
- Notes: This helps detect issues related to resource reuse, state leakage, or memory problems
  across multiple executions of the focal method.
*/
static bool test_Check4Stage16LUT_repeated() {
    const int iterations = 10;
    for (int i = 0; i < iterations; ++i) {
        int ret = Check4Stage16LUT();
        if (ret == 0) {
            std::cerr << "Check4Stage16LUT failed on iteration " << i << std::endl;
            return false;
        }
    }
    return true;
}

// Entry point for running the tests
int main() {
    std::cout << "Starting Check4Stage16LUT test suite..." << std::endl;

    // Run tests (non-terminating: continue after failures)
    RunTest("Check4Stage16LUT_basic", test_Check4Stage16LUT_basic);
    RunTest("Check4Stage16LUT_repeated", test_Check4Stage16LUT_repeated);

    // Summary
    std::cout << "Test results: " << (g_totalTests - g_failedTests) << " / "
              << g_totalTests << " tests passed." << std::endl;
    if (g_failedTests > 0) {
        std::cerr << g_failedTests << " test(s) failed." << std::endl;
        return 1;
    }
    return 0;
}