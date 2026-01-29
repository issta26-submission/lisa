/*
Unit Test Suite for CheckStoredIdentities (from testcms2.c)

Overview:
- This test harness is written in C++11 (no GTest) and targets the focal method:
  CheckStoredIdentities(void)  (declared as extern "C" to match C linkage)

- We rely on the actual implementation provided in testcms2.c. The function:
  - Creates temporary ICC profiles on disk (abstractv2.icc and abstractv4.icc)
  - Performs a series of CMS operations
  - Cleans up temporary files before returning

- Due to the nature of the code under test (external library calls and file I/O),
  triggering the true/false branches of internal predicates programmatically is not
  trivial from outside. The test suite focuses on:
  - Verifying the function returns the expected success code (1) in a normal run
  - Verifying that intermediate files are cleaned up (i.e., removed) after execution
  - Re-running to ensure no residue/files are left and behavior is stable

- This harness uses lightweight non-terminating assertions (does not abort on failure)
  and prints test results to stdout/stderr for easy inspection.

Notes on dependencies:
- The focal function is in the C file testcms2.c; we link against it.
- We declare the function prototype with C linkage to match the implementation.

Candidate keywords (Step 1): cmsCreateLab4ProfileTHR, DbgThread, cmsCreateTransformTHR, cmsTransform2DeviceLink,
cmsSaveProfileToFile, cmsCloseProfile, cmsDeleteTransform, cmsOpenProfileFromFileTHR, TYPE_Lab_8, TYPE_Lab_DBL,
INTENT_RELATIVE_COLORIMETRIC, CheckSeveralLab, abstractv2.icc, abstractv4.icc (file I/O, cleanup),
CheckStoredIdentities (focal method)

This test suite validates the high-level behavior and cleanup semantics of CheckStoredIdentities.

Usage:
- Build with the testcms2.c source included in the same target.
- Run the resulting executable. It prints test pass/fail statuses.

Code:
*/

#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cstdlib>


// Declaration of the focal C function with C linkage
extern "C" int CheckStoredIdentities(void);

// Lightweight test harness (non-terminating assertions)

// Simple assertion macro: prints failure but continues execution
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILED: " << (msg) << " [Function: " << __FUNCTION__ << "]" << std::endl; \
        g_tests_failed++; \
    } else { \
        /* optional success log can be enabled here */ \
    } \
    g_tests_run++; \
} while(0)

// Global counters for test results
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Helper: check if a file exists
static bool fileExists(const std::string& path) {
    std::ifstream ifs(path.c_str(), std::ifstream::in);
    return ifs.good();
}

// Helper: remove a file if it exists (best effort)
static void safeRemove(const std::string& path) {
    std::remove(path.c_str());
}

// Test 1: Verify CheckStoredIdentities returns 1 and cleans up temporary ICC files
static void test_CheckStoredIdentities_ReturnsOneAndCleansUp() {
    // Act
    int rc = CheckStoredIdentities();

    // Assert: normal/expected success path should return 1
    TEST_ASSERT(rc == 1, "CheckStoredIdentities should return 1 on success");

    // After execution, temporary files should be removed (cleanup)
    bool v2_exists = fileExists("abstractv2.icc");
    bool v4_exists = fileExists("abstractv4.icc");

    TEST_ASSERT(!v2_exists, "temporary file abstractv2.icc should be cleaned up (not exist)");
    TEST_ASSERT(!v4_exists, "temporary file abstractv4.icc should be cleaned up (not exist)");
}

// Test 2: Re-run to ensure no residue and consistent behavior
static void test_CheckStoredIdentities_IdempotentCleanup() {
    // Act
    int rc = CheckStoredIdentities();

    // Assert: second run should also return 1
    TEST_ASSERT(rc == 1, "Second run: CheckStoredIdentities should return 1 on success");

    // After second execution, verify cleanup again
    bool v2_exists = fileExists("abstractv2.icc");
    bool v4_exists = fileExists("abstractv4.icc");

    TEST_ASSERT(!v2_exists, "Second run cleanup: abstractv2.icc should be removed");
    TEST_ASSERT(!v4_exists, "Second run cleanup: abstractv4.icc should be removed");
}

// Main entry: run all tests
int main() {
    std::cout << "Running unit tests for CheckStoredIdentities..." << std::endl;

    // Ensure a clean slate
    safeRemove("abstractv2.icc");
    safeRemove("abstractv4.icc");

    // Run tests
    test_CheckStoredIdentities_ReturnsOneAndCleansUp();
    test_CheckStoredIdentities_IdempotentCleanup();

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << ", Failures: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed > 0) ? 1 : 0;
}