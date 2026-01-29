// Test suite for the focal method: Check1DLERP4
// This file provides a small, self-contained C++11 test harness
// that exercises the Check1DLERP4() function implemented in testcms2.c.
// It does not rely on GTest or other external testing frameworks.
// The tests compare Check1DLERP4() against a direct invocation of Check1D(4, FALSE, 0)
// and also verify deterministic behavior across multiple invocations.

#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Prototypes for the focal method and its dependent function.
// Declared with C linkage to match the implementation in testcms2.c.
extern "C" int Check1DLERP4(void);
extern "C" int Check1D(int nNodesToCheck, int Down, int max_err);

// Global counters for test results
static int gTestsRun = 0;
static int gFailures = 0;

// Utility to log a failure with a consistent format
static void logFailure(const char* testName, const char* message) {
    std::cerr << "[FAIL] " << testName << " - " << message << std::endl;
    ++gFailures;
}

// Test 1:
// Purpose: Validate that Check1DLERP4() returns exactly the same result as
//          a direct call to Check1D(4, FALSE, 0).
// Rationale: Check1DLERP4 is a thin wrapper around Check1D with fixed parameters.
// This test ensures there is no divergent behavior introduced by the wrapper.
bool test_Check1DLERP4_Equals_Check1D() {
    const char* testName = "Check1DLERP4_Equals_Check1D";
    ++gTestsRun;

    int wrapperResult = Check1DLERP4();
    int directResult  = Check1D(4, 0, 0); // FALSE == 0

    if (wrapperResult != directResult) {
        std::sprintf(nullptr, "%d", wrapperResult); // avoid unused warning if compiled differently
        std::cerr << "[ERROR] " << testName
                  << ": Check1DLERP4() = " << wrapperResult
                  << ", Check1D(4, FALSE, 0) = " << directResult
                  << std::endl;
        ++gFailures;
        return false;
    }

    std::cout << "[OK] " << testName << std::endl;
    return true;
}

// Test 2:
// Purpose: Ensure deterministic behavior of Check1DLERP4() across consecutive calls.
// Rationale: If internal caching or static state affects results, this test detects
//           non-deterministic behavior which would indicate a potential bug.
bool test_Check1DLERP4_Deterministic() {
    const char* testName = "Check1DLERP4_Deterministic";
    ++gTestsRun;

    int first  = Check1DLERP4();
    int second = Check1DLERP4();

    if (first != second) {
        std::cerr << "[ERROR] " << testName
                  << ": First call = " << first
                  << ", Second call = " << second
                  << std::endl;
        ++gFailures;
        return false;
    }

    std::cout << "[OK] " << testName << std::endl;
    return true;
}

// Entry point for the test suite
int main() {
    std::cout << "Starting tests for Check1DLERP4..." << std::endl;

    // Run tests
    (void)test_Check1DLERP4_Equals_Check1D();
    (void)test_Check1DLERP4_Deterministic();

    // Summary
    std::cout << "Tests run: " << gTestsRun
              << ", Failures: " << gFailures << std::endl;

    // Non-terminating assertions are preferred; return code reflects failures.
    return (gFailures > 0) ? 1 : 0;
}