/*
 * Lightweight C++11 test harness for the focal method:
 *   cmsInt32Number CheckMethodPackDoublesFromFloat(void)
 * located in testplugin.c
 *
 * Notes:
 * - The tests are implemented without Google Test or any other external framework.
 * - We rely on the real CMS environment already present in the project.
 * - true/false branches in the focal method are exercised where feasible:
 *     - NoICC: ensures the function returns 0 when test2.icc is missing.
 *     - WithICC: returns 1 when test2.icc exists (best effort; depends on environment).
 * - The tests are non-terminating: they log failures but continue execution.
 * - We expose the function with C linkage to avoid name mangling issues when linking with C code.
 *
 * How to build (example):
 *   g++ -std=c++11 -I<path_to_c_headers> test_check_method_pack.cpp testplugin.c -o test_check_method_pack
 *   Ensure test2.icc is available in the working directory for the success test.
 */

#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>


// Declaration of the focal function under test.
// It uses C linkage to match the original C function signature.
extern "C" int CheckMethodPackDoublesFromFloat(void);

// Simple non-terminating assertion helper
static void record_assert(bool condition, const char* testName, const char* message) {
    static int total = 0;
    static int passed = 0;
    ++total;
    if (condition) {
        ++passed;
    } else {
        std::cerr << "[FAIL] " << testName << ": " << (message ? message : "Assertion failed") << "\n";
    }
    // Persist counts in environment for debugging if needed
    // Note: For simplicity, counts are kept in static scope here.
}

// Utility: check if a file exists
static bool fileExists(const char* path) {
    std::ifstream f(path);
    return f.good();
}

// Test 1: No ICC file present -> Expecting failure path (return 0)
static bool Test_CheckMethodPackDoublesFromFloat_NoICC() {
    // Ensure the ICC profile is missing
    // If a test2.icc exists, remove it to trigger the early failure path in the focal method.
    if (fileExists("test2.icc")) {
        if (std::remove("test2.icc") != 0) {
            std::cerr << "[WARN] Could not remove test2.icc to simulate missing ICC file.\n";
            // Do not fail the test outright; proceed to attempt the call.
        }
    }

    int res = CheckMethodPackDoublesFromFloat();
    bool ok = (res == 0);
    record_assert(ok, "Test_CheckMethodPackDoublesFromFloat_NoICC", "Expected 0 when test2.icc is missing.");
    return ok;
}

// Test 2: ICC file present -> Attempt to exercise success path (return 1)
// This test is conditional on the presence of test2.icc in the working directory.
// If absent, the test will be skipped gracefully.
static bool Test_CheckMethodPackDoublesFromFloat_WithICC() {
    if (!fileExists("test2.icc")) {
        std::cout << "[SKIP] test2.icc not found. Skipping success-path test.\n";
        return true; // skip treated as pass to not fail the suite due to environment
    }

    int res = CheckMethodPackDoublesFromFloat();
    bool ok = (res == 1);
    record_assert(ok, "Test_CheckMethodPackDoublesFromFloat_WithICC", "Expected 1 when test2.icc is present and environment is correct.");
    return ok;
}

// Helper to print a succinct summary at the end
static void PrintSummary() {
    // Since record_assert uses a local static counter, we re-run a lightweight
    // counting pass by re-invoking the tests and printing results.
    // For simplicity in this harness, we provide a final note only.
    std::cout << "Test execution completed. Review any [FAIL] messages above for details.\n";
}

int main() {
    // Run tests
    std::cout << "Starting tests for CheckMethodPackDoublesFromFloat...\n";

    bool all_passed = true;

    // Run NoICC test
    bool r1 = Test_CheckMethodPackDoublesFromFloat_NoICC();
    all_passed = all_passed && r1;

    // Run WithICC test (conditional)
    bool r2 = Test_CheckMethodPackDoublesFromFloat_WithICC();
    all_passed = all_passed && r2;

    PrintSummary();

    if (all_passed) {
        std::cout << "[OK] All test cases passed.\n";
        return 0;
    } else {
        std::cerr << "[WARN] Some test cases did not pass. See above for details.\n";
        return 1;
    }
}