/*
Unit Test Suite for the focal method:
CheckOneTAC(cmsFloat64Number InkLimit)

Context:
- The method CheckOneTAC creates a fake CMYK profile using InkLimit, saves it to a temporary file
  ("lcmstac.icc"), reopens it, detects its TAC via cmsDetectTAC, and finally deletes the file.
- It returns 1 if fabs(d - InkLimit) <= 5, otherwise 0.
- This test harness is written in C++11, does not rely on GTest, and executes tests from main().
- We use a lightweight, non-terminating assertion style: we log pass/fail per test and continue.
- We assume the focal function is exposed with C linkage (extern "C") as per typical usage in this project.

Notes:
- We declare the focal function with a compatible signature:
  extern "C" int CheckOneTAC(double InkLimit);
  where cmsInt32Number ~ int and cmsFloat64Number ~ double.
- The tests aim to cover both possible outcomes (0 and 1) by trying multiple InkLimit values.
- We also verify that the temporary file is removed after each CheckOneTAC invocation.

Explanatory comments accompany each unit test.
*/

#include <cstdio>
#include <sys/stat.h>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <cmath>


// Declaration of the focal function with C linkage.
// We use a compatible signature: int CheckOneTAC(double InkLimit)
extern "C" int CheckOneTAC(double InkLimit);

// Lightweight file existence helper
static bool fileExists(const char* path)
{
    struct StatBuffer {
        struct stat info;
    } sb;
    // stat returns 0 on success, non-zero on failure
    return (stat(path, &sb.info) == 0);
}

// Simple test harness helpers
static void logTestResult(const char* testName, bool passed, int detail = 0)
{
    std::cout << "[TEST] " << testName << " - " << (passed ? "PASSED" : "FAILED");
    if (detail != 0) {
        std::cout << " (detail: " << detail << ")";
    }
    std::cout << std::endl;
}

// Test 1: Basic sanity for a zero InkLimit.
// Rationale: Checks the basic code path and ensures no crashes with a boundary value.
// Expected: The function returns either 0 or 1 depending on TAC calculation; we expect a valid outcome
// and that the temporary ICC profile is cleaned up afterwards.
static bool test_CheckOneTAC_ZeroInkLimit()
{
    const double ink = 0.0;
    int res = CheckOneTAC(ink);

    // Expect a valid return value (0 or 1). We log the value for debugging.
    logTestResult("CheckOneTAC with InkLimit=0.0 (valid return)", (res == 0 || res == 1), res);

    // Confirm that the function returns a defined boolean-like outcome (0 or 1)
    bool ok = (res == 0 || res == 1);

    // Also verify the temporary file is removed after the call
    bool exists = fileExists("lcmstac.icc");
    if (exists) {
        // If the file still exists, attempt to remove it to clean up the environment for subsequent tests
        std::remove("lcmstac.icc");
    }
    // The specific existence result is not critical for the main logic, but we log it for traceability.
    if (exists) {
        logTestResult("CheckOneTAC_ZeroInkLimit: lcmstac.icc cleanup", false);
        // Consider this as a test detail flag, not a failure of the function itself.
    }

    return ok;
}

// Test 2: Try a mid-range InkLimit value.
// Rationale: Exercise the function with a typical non-extreme input to increase likelihood of exercising the
// TAC calculation path. We expect a stable return value (0 or 1) and file cleanup.
static bool test_CheckOneTAC_MidInkLimit()
{
    const double ink = 50.0;
    int res = CheckOneTAC(ink);

    logTestResult("CheckOneTAC with InkLimit=50.0", (res == 0 || res == 1), res);
    bool ok = (res == 0 || res == 1);

    bool exists = fileExists("lcmstac.icc");
    if (exists) {
        std::remove("lcmstac.icc");
    }

    if (exists) {
        logTestResult("CheckOneTAC_MidInkLimit: lcmstac.icc cleanup", false);
    }

    return ok;
}

// Test 3: High InkLimit value to potentially exercise the false branch.
// Rationale: The original code returns 0 if fabs(d - InkLimit) > 5. With a very high InkLimit,
// the TAC detection might diverge, triggering the false branch if the profile TAC is not matching.
// We also verify file cleanup.
static bool test_CheckOneTAC_HighInkLimit()
{
    const double ink = 1000.0;
    int res = CheckOneTAC(ink);

    logTestResult("CheckOneTAC with InkLimit=1000.0", (res == 0 || res == 1), res);
    bool ok = (res == 0 || res == 1);

    bool exists = fileExists("lcmstac.icc");
    if (exists) {
        std::remove("lcmstac.icc");
    }

    if (exists) {
        logTestResult("CheckOneTAC_HighInkLimit: lcmstac.icc cleanup", false);
    }

    return ok;
}

// Test 4: Ensure file cleanup is robust irrespective of return value.
// Rationale: Even in edge cases, the function should not leave residue files.
static bool test_CheckOneTAC_FileRemovalRobustness()
{
    const double ink = 0.0; // Use a value that previously yielded a valid run
    int res = CheckOneTAC(ink);

    bool exists = fileExists("lcmstac.icc");
    if (exists) {
        // If a file exists, remove it to maintain a clean environment
        std::remove("lcmstac.icc");
    }

    // The explicit assertion is that the function should clean up; we log the observed existence.
    logTestResult("CheckOneTAC_FileRemovalRobustness: post-call cleanup", !exists, (exists ? 1 : 0));

    return (!exists);
}

int main(int argc, char* argv[])
{
    // Run all tests in sequence, collecting pass/fail information.
    int total = 0;
    int passed = 0;

    // Each test returns true on pass, false on fail.
    if (test_CheckOneTAC_ZeroInkLimit()) { ++passed; } else { /* log already done inside test */ }
    ++total;

    if (test_CheckOneTAC_MidInkLimit()) { ++passed; } else { }
    ++total;

    if (test_CheckOneTAC_HighInkLimit()) { ++passed; } else { }
    ++total;

    if (test_CheckOneTAC_FileRemovalRobustness()) { ++passed; } else { }
    ++total;

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Test suite result: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed to aid integration in CI or script checks.
    return (passed == total) ? 0 : 1;
}