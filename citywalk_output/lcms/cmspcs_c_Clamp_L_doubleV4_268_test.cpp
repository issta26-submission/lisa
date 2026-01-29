/* 
   Test suite for Clamp_L_doubleV4 (cmsFloat64Number Clamp_L_doubleV4(cmsFloat64Number L))
   - Domain: Clamps input L to the inclusive range [0, 100].
   - Key behavior: 
       if (L < 0) L = 0;
       if (L > 100.0) L = 100.0;
       return L;
   - This test focuses on exercising both conditional branches and the boundary cases.
   - We assume the function is provided in cmspcs.c with C linkage. We declare extern "C" to link correctly from C++ test code.
   - Types are aliased to keep test independent of the original typedefs in the real project.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <limits>


// Candidate Keywords extracted from the focal method and dependencies
// - L, Clamp_L_doubleV4, 0, 100, conditional branches, boundary values,
// - cmsFloat64Number, type clamping, return value, C linkage, linking to C file

// Domain knowledge utilities
typedef double cmsFloat64Number;
typedef unsigned short cmsUInt16Number;

// Declaration of the function under test (assumes C linkage in the CMYK CMS project)
extern "C" cmsFloat64Number Clamp_L_doubleV4(cmsFloat64Number L);

// Simple non-terminating assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Compare two floating point numbers with tolerance to avoid strict exact equality
static bool almost_equal(cmsFloat64Number a, cmsFloat64Number b, cmsFloat64Number tol = 1e-9)
{
    // Handle NaN edge cases gracefully
    if (std::isnan(a) && std::isnan(b)) return true;
    if (std::isnan(a) || std::isnan(b)) return false;
    return std::fabs(a - b) <= tol;
}

// Report PASS/FAIL for a single test case
static void report(const char* name, cmsFloat64Number expected, cmsFloat64Number actual)
{
    ++g_total_tests;
    if (almost_equal(expected, actual)) {
        std::cout << "[PASS] " << name
                  << " | expected: " << expected
                  << " actual: " << actual << std::endl;
    } else {
        ++g_failed_tests;
        std::cout << "[FAIL] " << name
                  << " | expected: " << expected
                  << " actual: " << actual << std::endl;
    }
}

// Helper to run a single test case
static void run_test_case(const char* name, cmsFloat64Number input, cmsFloat64Number expected)
{
    cmsFloat64Number result = Clamp_L_doubleV4(input);
    report(name, expected, result);
}

// Entry point for tests
int main()
{
    // Test Case 1: L is negative -> clamps to 0.0
    // Rationale: Triggers the first conditional branch (L < 0)
    run_test_case("Clamp_L_doubleV4_negative_input_clamps_to_zero", -5.0, 0.0);

    // Test Case 2: L is exactly 0.0 -> remains 0.0
    // Rationale: Falls through both conditionals (no clamping needed)
    run_test_case("Clamp_L_doubleV4_zero_input_remains_zero", 0.0, 0.0);

    // Test Case 3: L is within range (e.g., 50.0) -> unchanged
    // Rationale: No conditional branches taken
    run_test_case("Clamp_L_doubleV4_mid_input_stays_mid", 50.0, 50.0);

    // Test Case 4: L is exactly 100.0 -> remains 100.0
    // Rationale: Second conditional false (L > 100 false) but edge value
    run_test_case("Clamp_L_doubleV4_upper_boundary_input_remains_100", 100.0, 100.0);

    // Test Case 5: L is above 100.0 -> clamps to 100.0
    // Rationale: Triggers the second conditional branch (L > 100.0)
    run_test_case("Clamp_L_doubleV4_above_upper_bound_clamps_to_100", 150.0, 100.0);

    // Optional: test a tiny epsilon above 100.0 to ensure clamp to 100
    run_test_case("Clamp_L_doubleV4_above_upper_bound_tiny_excess_clamps_to_100", 
                  100.000000001, 100.0);

    // Summary
    std::cout << "Test Summary: " << g_total_tests
              << " tests, " << g_failed_tests << " failed." << std::endl;

    return (g_failed_tests == 0) ? 0 : 1;
}