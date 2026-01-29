// Minimal C++11 test suite for the focal method: Check2StageLUT
// This test harness intentionally avoids any external test framework (e.g., GTest).
// It uses a lightweight, non-terminating assertion mechanism and reports a summary.

#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Import C dependencies. The focal method and its helpers live in testcms2.c/testcms2.h.
extern "C" {
}

// Simple non-terminating assertion helper
static int gFailures = 0;

static void AssertIntEq(const char* testName, int expected, int actual) {
    if (expected != actual) {
        std::cerr << "TEST FAILED: " << testName
                  << " (expected " << expected << ", got " << actual << ")" << std::endl;
        ++gFailures;
    } else {
        std::cout << "TEST PASSED: " << testName << std::endl;
    }
}

// Test 1: Basic correctness validation for Check2StageLUT
// Reason: Ensure the function returns the expected success indicator when using
// the identity matrix and identity CLUT, mirroring the focal implementation.
static void test_Check2StageLUT_BasicReturn() {
    cmsInt32Number res = Check2StageLUT();
    AssertIntEq("Check2StageLUT basic return is 1 (success)", 1, (int)res);
}

// Test 2: Consistency check across multiple invocations
// Reason: Verify that repeated calls yield the same result, indicating stable behavior.
// This helps exercise the function under repeated usage, which can catch
// side effects or cleanup issues present in a single run.
static void test_Check2StageLUT_Consistency() {
    for (int i = 0; i < 3; ++i) {
        cmsInt32Number res = Check2StageLUT();
        char testName[80];
        std::snprintf(testName, sizeof(testName), "Check2StageLUT consistency run %d", i);
        AssertIntEq(testName, 1, (int)res);
    }
}

// Main: Run tests and report a final exit code
int main(int argc, char* argv[]) {
    // Execute the test suite
    test_Check2StageLUT_BasicReturn();
    test_Check2StageLUT_Consistency();

    // Summary
    if (gFailures) {
        std::cerr << "TOTAL FAILURES: " << gFailures << std::endl;
        return 1; // Non-zero exit indicates test failures
    } else {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    }
}