/*
  Test Suite for IsGoodFixed8_8 in testcms2.c

  Step 1 & 2 (Understanding & Test Generation):
  - focal function: cmsBool IsGoodFixed8_8(const char* title, cmsFloat64Number in, cmsFloat64Number out)
  - behavior: it delegates to IsGoodVal(title, in, out, FIXED_PRECISION_8_8)
  - FIXED_PRECISION_8_8 represents the tolerance for fixed-point 8.8 precision (likely 1/256 = 0.00390625)
  - therefore, the predicate is effectively: |in - out| <= FIXED_PRECISION_8_8
  - test coverage should exercise true/false outcomes especially around the boundary.

  Step 3 (Test Refinement):
  - Design test cases to cover: equal inputs, within tolerance, exactly on boundary, slightly above boundary, and negative/positive combinations.
  - Use a lightweight, non-terminating assertion style (log results instead of aborting).
  - Use C++11, no GTest, link against the C testcms2.c source (via testcms2.h declarations and C linkage).

  Notes:
  - The tests assume testcms2.h provides the relevant types (cmsBool, cmsFloat64Number, etc.).
  - The function IsGoodFixed8_8 is in C, so we declare it with extern "C" to call from C++.
  - The tests are designed to be run from a main() in this file without a third-party test framework.
  - We only rely on the provided dependencies and the standard library.

  Candidate Keywords (extracted from focal method and dependencies):
  - IsGoodVal, FIXED_PRECISION_8_8, cmsBool, cmsFloat64Number
  - Boundary/predicate: abs(in - out) <= FIXED_PRECISION_8_8
*/

#include <iostream>
#include <cmath>
#include <testcms2.h>
#include <iomanip>


// Import project dependencies

// Ensure C linkage for the focal function when called from C++
extern "C" cmsBool IsGoodFixed8_8(const char* title, cmsFloat64Number in, cmsFloat64Number out);

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper to log a test result
static void log_result(const char* test_name, bool passed, double in, double out, bool expected)
{
    ++g_total_tests;
    if (passed)
    {
        std::cout << "[OK]    " << test_name
                  << " | in=" << std::setprecision(12) << in
                  << " out=" << std::setprecision(12) << out
                  << " (expected " << (expected ? "true" : "false") << ")\n";
    }
    else
    {
        ++g_failed_tests;
        std::cerr << "[FAILED] " << test_name
                  << " | in=" << std::setprecision(12) << in
                  << " out=" << std::setprecision(12) << out
                  << " (expected " << (expected ? "true" : "false") << ")\n";
    }
}

// Test 1: Equal in and out should be treated as good (true)
static void test_IsGoodFixed8_8_EqualInputs()
{
    // Explanation:
    // When in == out, the difference is 0 which is <= FIXED_PRECISION_8_8, so IsGoodFixed8_8 should return true.
    const char* title = "Equal in/out";
    double in  = 2.5;
    double out = 2.5;

    cmsBool res = IsGoodFixed8_8(title, in, out);
    bool expected = true;
    bool passed = (res != 0) == expected;
    log_result("IsGoodFixed8_8: Equal inputs", passed, in, out, expected);
}

// Test 2: Within tolerance (difference < FIXED_PRECISION_8_8)
static void test_IsGoodFixed8_8_WithinTolerance()
{
    // Explanation:
    // If out differs slightly within the tolerance (e.g., 0.002), result should be true.
    const char* title = "Within tolerance";
    double in  = 1.0000;
    double out = 1.002; // difference 0.002 < 0.00390625

    cmsBool res = IsGoodFixed8_8(title, in, out);
    bool expected = true;
    bool passed = (res != 0) == expected;
    log_result("IsGoodFixed8_8: Within tolerance", passed, in, out, expected);
}

// Test 3: Exactly on the boundary (difference == FIXED_PRECISION_8_8)
static void test_IsGoodFixed8_8_OnBoundary()
{
    // Explanation:
    // If difference equals the tolerance, it should still be considered good (<=).
    const char* title = "On boundary";
    double in  = 1.0;
    // Using 0.00390625 which is 1/256
    double out = 1.0 + 0.00390625;

    cmsBool res = IsGoodFixed8_8(title, in, out);
    bool expected = true;
    bool passed = (res != 0) == expected;
    log_result("IsGoodFixed8_8: On boundary", passed, in, out, expected);
}

// Test 4: Above tolerance (difference > FIXED_PRECISION_8_8)
static void test_IsGoodFixed8_8_AboveTolerance()
{
    // Explanation:
    // A slight exceedance over the tolerance should yield false.
    const char* title = "Above tolerance";
    double in  = 1.0;
    double out = 1.004; // difference 0.004 > 0.00390625

    cmsBool res = IsGoodFixed8_8(title, in, out);
    bool expected = false;
    bool passed = (res != 0) == expected;
    log_result("IsGoodFixed8_8: Above tolerance", passed, in, out, expected);
}

// Test 5: Negative values and tolerance check (within tolerance)
static void test_IsGoodFixed8_8_NegativeWithinTolerance()
{
    // Explanation:
    // Negative domain should also respect the same tolerance.
    const char* title = "Negative within tolerance";
    double in  = -0.5;
    double out = -0.501; // difference 0.001 <= tolerance

    cmsBool res = IsGoodFixed8_8(title, in, out);
    bool expected = true;
    bool passed = (res != 0) == expected;
    log_result("IsGoodFixed8_8: Negative within tolerance", passed, in, out, expected);
}

// Test 6: Negative beyond tolerance (exceeding)
static void test_IsGoodFixed8_8_NegativeBeyondTolerance()
{
    // Explanation:
    // Negative difference beyond tolerance should yield false.
    const char* title = "Negative beyond tolerance";
    double in  = -0.0;
    double out = -0.01; // difference 0.01 > tolerance

    cmsBool res = IsGoodFixed8_8(title, in, out);
    bool expected = false;
    bool passed = (res != 0) == expected;
    log_result("IsGoodFixed8_8: Negative beyond tolerance", passed, in, out, expected);
}

// Entry point to run all tests
int main()
{
    std::cout << "Running tests for IsGoodFixed8_8 (C function) with C++11 harness...\n";

    test_IsGoodFixed8_8_EqualInputs();
    test_IsGoodFixed8_8_WithinTolerance();
    test_IsGoodFixed8_8_OnBoundary();
    test_IsGoodFixed8_8_AboveTolerance();
    test_IsGoodFixed8_8_NegativeWithinTolerance();
    test_IsGoodFixed8_8_NegativeBeyondTolerance();

    std::cout << "Summary: " << g_total_tests << " tests run, "
              << g_failed_tests << " failures.\n";

    // Return non-zero if any test failed to aid integration with CI systems
    return g_failed_tests;
}