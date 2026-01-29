// Test suite for the focal method: EntriesByGamma in cmsgamma.c
// This test is written in plain C++11 (no GTest) and uses a small, non-terminating
// assertion approach to maximize code coverage while continuing execution.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <limits>


// Declaration of the focal function under test.
// Use extern "C" to ensure correct linkage with the C symbol when compiled with a C++ compiler.
extern "C" {
    // The real project typedefs are cmsFloat64Number and cmsUInt32Number.
    // We map them to standard C++ types for test harness purposes.
    typedef double cmsFloat64Number;
    typedef unsigned int cmsUInt32Number;

    // Function prototype as seen in the focal class dependency section.
    cmsUInt32Number EntriesByGamma(cmsFloat64Number Gamma);
}

// Simple non-terminating test framework (minimal EXPECT macro style).
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(actual, expected) do { \
    ++g_total_tests; \
    if ((actual) != (expected)) { \
        ++g_failed_tests; \
        std::cerr << "TEST FAILURE: " << __FILE__ << ":" << __LINE__ \
                  << " - Expected " << (expected) << " but got " << (actual) \
                  << " (value = " << (actual) << ")\n"; \
    } \
} while (0)

// Helper function to run all test cases
static void run_all_tests();

// Individual test cases with explanatory comments

// Test 1: Gamma exactly 1.0 should be treated as within the tolerance and return 2.
static void test_Gamma_exactly_one_returns_2() {
    // true branch: fabs(Gamma - 1.0) < 0.001
    cmsUInt32Number result = EntriesByGamma(1.0);
    EXPECT_EQ(result, 2);
}

// Test 2: Gamma very close to 1.0 but still within tolerance (e.g., 1.0005) should return 2.
static void test_Gamma_within_tolerance_returns_2() {
    cmsUInt32Number result = EntriesByGamma(1.0005); // difference 0.0005 < 0.001
    EXPECT_EQ(result, 2);
}

// Test 3: Gamma just outside the tolerance (e.g., 0.999) should return 4096.
static void test_Gamma_below_tolerance_returns_4096() {
    cmsUInt32Number result = EntriesByGamma(0.999); // difference 0.001 -> not within
    EXPECT_EQ(result, 4096);
}

// Test 4: Gamma just above the tolerance (e.g., 1.001) should return 4096.
static void test_Gamma_above_tolerance_returns_4096() {
    cmsUInt32Number result = EntriesByGamma(1.001); // difference 0.001 -> not within
    EXPECT_EQ(result, 4096);
}

// Test 5: Negative Gamma should return 4096 (outside tolerance regardless of sign).
static void test_Gamma_negative_returns_4096() {
    cmsUInt32Number result = EntriesByGamma(-2.0);
    EXPECT_EQ(result, 4096);
}

// Test 6: NaN Gamma should return 4096 (comparison with NaN yields false, treated as outside).
static void test_Gamma_nan_returns_4096() {
    cmsUInt32Number result = EntriesByGamma(std::numeric_limits<double>::quiet_NaN());
    EXPECT_EQ(result, 4096);
}

// Test 7: Gamma very close to 1.0 but still outside tolerance (e.g., 1.0009999) should return 2.
static void test_Gamma_within_threshold_boundaries_returns_2() {
    cmsUInt32Number result = EntriesByGamma(1.0009999); // difference ~0.0009999 < 0.001
    EXPECT_EQ(result, 2);
}

// Run all tests and print a short summary
static void run_all_tests() {
    // Execute each test case. If a test fails, the macro already reported the issue.
    test_Gamma_exactly_one_returns_2();
    test_Gamma_within_tolerance_returns_2();
    test_Gamma_below_tolerance_returns_4096();
    test_Gamma_above_tolerance_returns_4096();
    test_Gamma_negative_returns_4096();
    test_Gamma_nan_returns_4096();
    test_Gamma_within_threshold_boundaries_returns_2();
}

int main() {
    run_all_tests();

    // Summary
    std::cout << "Total tests run: " << g_total_tests << "\n";
    std::cout << "Tests passed: " << (g_total_tests - g_failed_tests) << "\n";
    std::cout << "Tests failed: " << g_failed_tests << "\n";

    // Return non-zero if any test failed to aid integration with build systems.
    return g_failed_tests == 0 ? 0 : 1;
}

// Notes for maintainers:
// - The tests focus on the core predicate of EntriesByGamma: the proximity to 1.0 within a 0.001 tolerance.
// - We cover true and false branches, edge cases around the threshold, and a NaN case to ensure robust behavior.
// - Static/internal helpers from the real library are not required by these test cases; we rely on the public signature.
// - No GoogleTest or external frameworks are used as per the requirements. If the actual integration uses a different
//   build system, the extern "C" link-time binding should still be compatible provided the real cmsgamma.c is linked.