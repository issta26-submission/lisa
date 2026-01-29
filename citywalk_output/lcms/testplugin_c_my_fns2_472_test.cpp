// Test suite for the focal method my_fns2 in testplugin.c
// This test is written in C++11 without any external testing framework (GTest not allowed).
// It uses a minimal non-terminating assertion approach and a small test harness.
// The tests assume the availability of the following from the project's headers:
// - TYPE_TAN macro
// - testcms2.h providing necessary declarations
// The focal function is expected to be linked with the test executable.

#include <iostream>
#include <cmath>
#include <testcms2.h>
#include <cstdlib>


// Include project header to get macros like TYPE_TAN and any needed declarations.
// This header is expected to define TYPE_TAN and possibly M_PI (we will not rely on M_PI directly).

extern "C" {
    // Declare the focal function with C linkage for proper linkage in C++.
    cmsFloat64Number my_fns2(cmsInt32Number Type, const cmsFloat64Number Params[], cmsFloat64Number R);
}

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// EXPECT_NEAR: compares two floating-point numbers within a tolerance without aborting on failure
#define EXPECT_NEAR(actual, expected, tol) do { \
    ++g_total_tests; \
    double _a = static_cast<double>(actual); \
    double _e = static_cast<double>(expected); \
    double _tol = static_cast<double>(tol); \
    if (std::fabs(_a - _e) > _tol) { \
        ++g_failed_tests; \
        std::cout << "[EXPECT_NEAR FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " expected near " << _e << " but got " << _a \
                  << " (tol=" << _tol << ")" << std::endl; \
    } \
} while(0)

// EXPECT_EQ: exact equality for simple types (e.g., -1.0 return case)
#define EXPECT_EQ(actual, expected) do { \
    ++g_total_tests; \
    double _a = static_cast<double>(actual); \
    double _e = static_cast<double>(expected); \
    if (_a != _e) { \
        ++g_failed_tests; \
        std::cout << "[EXPECT_EQ FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " expected " << _e << " but got " << _a << std::endl; \
    } \
} while(0)

// Helper: tests summary print
static void print_summary() {
    std::cout << "Test results: " << g_total_tests << " ran, " 
              << g_failed_tests << " failed." << std::endl;
    if (g_failed_tests == 0) {
        std::cout << "[ALL TESTS PASSED]" << std::endl;
    } else {
        std::cout << "[SOME TESTS FAILED]" << std::endl;
    }
}

// PI constant for computing expected values (independent of M_PI macro availability)
static constexpr long double PI = 3.141592653589793238462643383279502884L;

// Test 1: TYPE_TAN branch - verify Params[0] * tan(R * PI) is computed correctly
// R = 0.25 => tan(pi/4) = 1, so result == Params[0]
static void test_my_fns2_type_tan_basic() {
    // Prepare inputs
    cmsFloat64Number params[1];
    params[0] = 2.0;           // arbitrary non-zero scale
    cmsInt32Number Type = TYPE_TAN;
    cmsFloat64Number R = 0.25;   // R * PI = PI/4

    // Expected result
    const double expected = static_cast<double>(params[0]) * std::tan(static_cast<double>(R) * static_cast<double>(PI));

    // Actual result
    cmsFloat64Number result = my_fns2(Type, params, R);

    // Assertion
    EXPECT_NEAR(static_cast<double>(result), expected, 1e-12);
    // Note: this test ensures true-branch (TYPE_TAN) is exercised and uses Params[0]
}

// Test 2: -TYPE_TAN branch - verify atan(R) / (PI * Params[0])
// Choose R = 1.0 => atan(1) = PI/4; expected = (PI/4) / (PI * Params[0]) = 1 / (4 * Params[0])
static void test_my_fns2_type_neg_tan_basic() {
    cmsFloat64Number params[1];
    params[0] = 2.0;
    cmsInt32Number Type = -TYPE_TAN;
    cmsFloat64Number R = 1.0;

    const double expected = std::atan(static_cast<double>(R)) / (static_cast<double>(PI) * static_cast<double>(params[0]));
    cmsFloat64Number result = my_fns2(Type, params, R);

    EXPECT_NEAR(static_cast<double>(result), expected, 1e-12);
}

// Test 3: Default/unknown Type branch - ensure returns -1.0
static void test_my_fns2_type_default_branch() {
    cmsFloat64Number params[1];
    params[0] = 1.0;
    cmsInt32Number Type = 9999;    // Unknown type
    cmsFloat64Number R = 0.5;

    cmsFloat64Number result = my_fns2(Type, params, R);

    EXPECT_EQ(result, -1.0);
    // This test covers the default branch
}

// Step 3: Additional coverage - ensure multiple Type values are handled (extra safety)
static void test_my_fns2_type_other_values() {
    // A couple of random tests to exercise more paths with TYPE_TAN and -TYPE_TAN
    cmsFloat64Number params[1];
    params[0] = 0.5;

    // TYPE_TAN with R = 0 -> tan(0) = 0
    {
        cmsInt32Number Type = TYPE_TAN;
        cmsFloat64Number R = 0.0;
        cmsFloat64Number res = my_fns2(Type, params, R);
        EXPECT_NEAR(static_cast<double>(res), 0.0, 1e-12);
    }

    // -TYPE_TAN with R = -1.0
    {
        cmsInt32Number Type = -TYPE_TAN;
        cmsFloat64Number R = -1.0;
        cmsFloat64Number res = my_fns2(Type, params, R);
        const double expected = std::atan(static_cast<double>(R)) / (static_cast<double>(PI) * static_cast<double>(params[0]));
        EXPECT_NEAR(static_cast<double>(res), expected, 1e-12);
    }
}

int main(void) {
    // Step 2 & 3: Run the comprehensive test suite for my_fns2
    // The tests rely on external linkage to my_fns2 via testcms2.h
    test_my_fns2_type_tan_basic();
    test_my_fns2_type_neg_tan_basic();
    test_my_fns2_type_default_branch();
    test_my_fns2_type_other_values();

    // Print summary and exit with non-zero if failures occurred
    print_summary();
    return g_failed_tests;
}