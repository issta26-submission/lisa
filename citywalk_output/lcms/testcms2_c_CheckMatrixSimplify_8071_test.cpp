// This test suite targets the focal method: CheckMatrixSimplify from the provided
// testCMS codebase. The goal is to exercise the method under realistic conditions
// without requiring GoogleTest. We use a lightweight, non-terminating assertion
// style so tests continue executing to improve coverage. The tests are written in
// C++11 and call the C function CheckMatrixSimplify via an extern "C" declaration.
// The tests focus on branch coverage for the NULL-profiles check (the false/true
// branches of the predicate pIn == NULL || pOut == NULL) by simulating a missing
// ICC profile file ibm-t61.icc. They also perform repeated invocations to ensure
// stability across multiple calls.
//
// Important notes for maintainers:
// - The environment must have the LittleCMS-like API available, as used by the
//   focal function (cmsCreate_sRGBProfile, cmsOpenProfileFromFile, etc.).
// - The test simulates the missing file scenario by temporarily renaming
//   ibm-t61.icc if it exists. When missing, CheckMatrixSimplify should return 0.
// - We avoid modifying private state, and we only rely on public C linkage of the
//   focal method (via extern "C").
// - This suite does not use GTest; tests are invoked from main() and report via
//   standard output. Static/global test helpers are kept minimal and scoped.

#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <testcms2.h>
#include <cerrno>


// Forward declaration of the focal method from the C source (C linkage required)
extern "C" int CheckMatrixSimplify(void);

// Helper: check if a file exists
static bool fileExists(const char* path) {
    std::ifstream f(path);
    return f.good();
}

// Helper: non-terminating assertion style
static void expectTrue(bool condition, const char* testDesc) {
    if (!condition) {
        std::cerr << "TEST FAILED: " << testDesc << std::endl;
    } else {
        std::cout << "TEST PASSED: " << testDesc << std::endl;
    }
}

// Test 1: Verify behavior when the ICC profile file ibm-t61.icc is missing.
// This triggers the false branch of the predicate (pIn != NULL && pOut != NULL)
// to fail and should cause CheckMatrixSimplify to return 0.
static bool test_CheckMatrixSimplify_MissingFile() {
    const char* iccFile = "ibm-t61.icc";
    const char* iccBackup = "ibm-t61.icc.bak";

    bool existed = fileExists(iccFile);

    // If the file exists, rename it to simulate a missing file scenario.
    if (existed) {
        if (std::rename(iccFile, iccBackup) != 0) {
            std::cerr << "ERROR: Failed to rename " << iccFile
                      << " to " << iccBackup << " (errno=" << errno << ")" << std::endl;
            return false;
        }
        std::cout << "INFO: Renamed " << iccFile << " to " << iccBackup
                  << " to simulate missing ICC profile." << std::endl;
    } else {
        std::cout << "INFO: ICC file " << iccFile << " not present; proceeding with test." << std::endl;
    }

    // Call the focal method
    int result = CheckMatrixSimplify();

    // Restore the original file if it existed
    if (existed) {
        if (std::rename(iccBackup, iccFile) != 0) {
            std::cerr << "ERROR: Failed to restore " << iccBackup
                      << " back to " << iccFile << " (errno=" << errno << ")" << std::endl;
            // Even if restoration fails, we still want to report test outcome.
        } else {
            std::cout << "INFO: Restored " << iccBackup << " back to " << iccFile << std::endl;
        }
    }

    // Expected: function should return 0 if either pIn or pOut is NULL due to missing ICC
    bool passed = (result == 0);
    expectTrue(passed, "CheckMatrixSimplify returns 0 when ICC profile ibm-t61.icc is missing");
    return passed;
}

// Test 2: Basic stability check - invoke the focal method multiple times to ensure
// no crashes or undefined behavior when profiles may or may not be present.
// We just ensure the function returns a valid integer (0 or 1) across calls.
static bool test_CheckMatrixSimplify_MultipleCalls_Stability() {
    const int repetitions = 5;
    bool allPassed = true;

    for (int i = 0; i < repetitions; ++i) {
        int result = CheckMatrixSimplify();
        bool valid = (result == 0 || result == 1);
        if (!valid) {
            std::cerr << "ERROR: CheckMatrixSimplify returned invalid value "
                      << result << " on iteration " << i << std::endl;
        }
        expectTrue(valid, "CheckMatrixSimplify returns a valid 0/1 on repeated call");
        if (!valid) allPassed = false;
    }

    return allPassed;
}

// Test 3: Sanity check - ensure that the function can be called without prior setup
// and without crashing even if the ICC files are not in an ideal state.
// This is a light-weight check leveraging the existing API contract.
static bool test_CheckMatrixSimplify_SanityCall() {
    // Direct call; we do not enforce a specific outcome, just verify no crash.
    int result = CheckMatrixSimplify();
    bool valid = (result == 0 || result == 1);
    expectTrue(valid, "CheckMatrixSimplify sanity call yields valid 0/1 result");
    return valid;
}

int main() {
    std::cout << "=== Test Suite: CheckMatrixSimplify (C integration) ===" << std::endl;

    int totalTests = 3;
    int passedTests = 0;

    // Run Test 1: Missing file scenario (branch coverage for the NULL profile path)
    if (test_CheckMatrixSimplify_MissingFile()) ++passedTests;
    else {
        std::cout << "Test 1 (Missing ICC file) did not pass." << std::endl;
    }

    // Run Test 2: Stability across multiple invocations
    if (test_CheckMatrixSimplify_MultipleCalls_Stability()) ++passedTests;
    else {
        std::cout << "Test 2 (Stability across calls) did not pass." << std::endl;
    }

    // Run Test 3: Sanity check on a single call
    if (test_CheckMatrixSimplify_SanityCall()) ++passedTests;
    else {
        std::cout << "Test 3 (Sanity call) did not pass." << std::endl;
    }

    std::cout << "Summary: " << passedTests << " / " << totalTests << " tests passed." << std::endl;
    return (passedTests == totalTests) ? 0 : 1;
}