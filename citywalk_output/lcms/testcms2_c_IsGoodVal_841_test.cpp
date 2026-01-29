// test_IsGoodVal.cpp
// Unit tests for the CMS function: cmsBool IsGoodVal(const char *title, cmsFloat64Number in, cmsFloat64Number out, cmsFloat64Number max)
// This test suite is designed for C++11 compatibility without using GTest.
// It relies on the existing test harness provided in testcms2.h/testcms2.c.
// The tests use a lightweight, non-terminating assertion mechanism and a small test runner.

#include <cmath>
#include <cstdio>
#include <testcms2.h>


// Include the test harness header that declares helper functions and types used by the focal method.
// The header provides definitions for cmsBool, cmsFloat64Number, etc.

// Declare the focal function we are testing (C linkage to match the C implementation).
extern "C" cmsBool IsGoodVal(const char* title, cmsFloat64Number in, cmsFloat64Number out, cmsFloat64Number max);

// MaxErr is a global maintained by the test harness; we use it to verify side-effect behavior.
extern cmsFloat64Number MaxErr;

// Simple, non-terminating assertion/logging system
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Helper to compare doubles with a small tolerance
static bool approxEq(cmsFloat64Number a, cmsFloat64Number b, cmsFloat64Number tol = 1e-9) {
    return std::fabs(static_cast<double>(a) - static_cast<double>(b)) < static_cast<double>(tol);
}

// Record the result of a test case
static void record_result(const char* test_name, bool passed) {
    ++g_total_tests;
    if (passed) {
        ++g_passed_tests;
    } else {
        printf("Test failed: %s\n", test_name);
    }
}

// Test 1: Err = |in - out| <= max, Err <= MaxErr (no update to MaxErr, should return TRUE)
static void test_IsGoodVal_NoMaxUpdate_True() {
    // Arrange
    // Ensure the "first" branch (Err > MaxErr) does not execute.
    MaxErr = 2.0;                 // large existing max error
    const char* title = "NoMaxUpdateTrue";
    cmsFloat64Number in = 0.5;
    cmsFloat64Number out = 0.7;     // Err = 0.2
    cmsFloat64Number max = 1.0;     // Err (0.2) <= max, and <= MaxErr (2.0)

    // Act
    cmsBool result = IsGoodVal(title, in, out, max);

    // Assert
    bool ok = (result != 0);
    bool maxUnchanged = approxEq(MaxErr, 2.0);

    record_result("IsGoodVal_NoMaxUpdate_True", ok && maxUnchanged);
}

// Test 2: Err > MaxErr (update) and Err <= max (no fail), should return TRUE and MaxErr updated
static void test_IsGoodVal_UpdateMaxButTrue() {
    // Arrange
    MaxErr = 0.0;                   // ensure first-branch is taken
    const char* title = "UpdateMaxButTrue";
    cmsFloat64Number in = 0.6;
    cmsFloat64Number out = 0.9;     // Err = 0.3
    cmsFloat64Number max = 1.0;     // Err <= max

    // Act
    cmsBool result = IsGoodVal(title, in, out, max);

    // Assert
    bool ok = (result != 0);
    bool maxUpdated = approxEq(MaxErr, 0.3); // MaxErr should have been updated to 0.3 (Err)

    record_result("IsGoodVal_UpdateMaxButTrue", ok && maxUpdated);
}

// Test 3: Err > max triggers failure path (return FALSE) and updates MaxErr to Err
static void test_IsGoodVal_ErrGreaterThanMax() {
    // Arrange
    MaxErr = 0.1;                   // small current max
    const char* title = "ErrGreaterThanMax";
    cmsFloat64Number in = 1.5;
    cmsFloat64Number out = 1.0;     // Err = 0.5
    cmsFloat64Number max = 0.4;     // Err > max -> should Fail and return FALSE

    // Act
    cmsBool result = IsGoodVal(title, in, out, max);

    // Assert
    bool failPath = (result == 0);
    bool maxUpdated = approxEq(MaxErr, 0.5); // Err (0.5) should update MaxErr since 0.5 > 0.1

    record_result("IsGoodVal_ErrGreaterThanMax", failPath && maxUpdated);
}

// Simple test runner
int main(int argc, char* argv[]) {
    // Run all test cases
    test_IsGoodVal_NoMaxUpdate_True();
    test_IsGoodVal_UpdateMaxButTrue();
    test_IsGoodVal_ErrGreaterThanMax();

    // Summary
    printf("Test Summary: %d total, %d passed\n", g_total_tests, g_passed_tests);

    // Return non-zero if any test failed to aid integration with build systems
    return (g_passed_tests == g_total_tests) ? 0 : 1;
}