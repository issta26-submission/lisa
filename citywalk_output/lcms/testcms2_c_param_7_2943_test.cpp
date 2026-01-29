/*
  Unit Test Suite for cmsFloat32Number param_7(cmsFloat32Number x, const cmsFloat64Number Params[])
  - This test suite is designed to be compiled alongside the focal code (testcms2.c) in a C++11 environment without GTest.
  - It uses a lightweight, non-terminating assertion style to maximize coverage.
  - Key concepts covered:
    - Calculation correctness for typical inputs
    - Handling of edge cases where domain errors may occur (NaN results)
    - Floating-point result comparison with a tolerance
  - Candidate Keywords derived from the focal method: cmsFloat32Number, cmsFloat64Number, x, Params, Params[0..4], log10, pow, Val
  - Dependencies:
    - The focal method is exposed with C linkage. We declare it as extern "C" to link against testcms2.c.
    - We rely only on the C++ standard library (<cmath>, <iostream>, <functional>) for tests.
*/

#include <functional>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cmath>


// Typedefs matching the assumed signatures in testcms2.c
typedef float  cmsFloat32Number;
typedef double cmsFloat64Number;

// Declaration of the focal method with C linkage.
// The real implementation is in testcms2.c (or linked in the build).
extern "C" cmsFloat32Number param_7(cmsFloat32Number x, const cmsFloat64Number Params[]);

// Simple test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Utility: approximate equality for floating point comparisons
static bool approx_equal(float a, float b, float tol = 1e-6f)
{
    // Handle NaN comparison gracefully: NaN is not equal to anything, so fallback to false
    if (std::isnan(a) || std::isnan(b)) return false;
    return std::fabs(static_cast<double>(a) - static_cast<double>(b)) <= static_cast<double>(tol);
}

// Wrapper to log test results without aborting on failure
#define LOG_TEST(name, condition)                                  \
    do {                                                           \
        g_total_tests++;                                           \
        if (condition) {                                           \
            std::cout << "[PASS] " << name << "\n";               \
        } else {                                                   \
            g_failed_tests++;                                      \
            std::cout << "[FAIL] " << name << "\n";               \
        }                                                          \
    } while (0)

// Test 1: Normal operation with a representative set of Params and x
static void test_param7_normal_case1()
{
    // Candidate Keywords: x, Params[0], Params[1], Params[2], Params[3], Params[4], log10, pow
    const cmsFloat32Number x = 2.0f;
    cmsFloat64Number Params[5];
    Params[0] = 2.0;   // exponent to x
    Params[1] = 3.0;   // multiplier of log10(...)
    Params[2] = 4.0;   // multiplier inside log10(...)
    Params[3] = 1.0;   // additive inside log10(...)
    Params[4] = 0.5;   // final additive term

    cmsFloat32Number res = param_7(x, Params);

    // Compute expected value using higher-precision long double arithmetic
    long double inner = Params[2] * std::pow((long double)x, (long double)Params[0]) + Params[3];
    long double expected_long = Params[1] * std::log10(inner) + Params[4];
    float expected = static_cast<float>(expected_long);

    bool ok = approx_equal(static_cast<float>(res), expected);
    LOG_TEST("param_7_normal_case1", ok);
}

// Test 2: Normal operation with x = 0 (pow(0, Params[0]) simplifies to 0 for Params[0] > 0)
static void test_param7_normal_case2()
{
    const cmsFloat32Number x = 0.0f;
    cmsFloat64Number Params[5];
    Params[0] = 2.0;   // exponent to x
    Params[1] = 3.0;   // multiplier
    Params[2] = 4.0;   // inside log10(...)
    Params[3] = 1.0;   // inside log10(...)
    Params[4] = 0.5;   // final add

    cmsFloat32Number res = param_7(x, Params);

    long double inner = Params[2] * std::pow((long double)x, (long double)Params[0]) + Params[3];
    long double expected_long = Params[1] * std::log10(inner) + Params[4];
    float expected = static_cast<float>(expected_long);

    bool ok = approx_equal(static_cast<float>(res), expected);
    LOG_TEST("param_7_normal_case2", ok);
}

// Test 3: Domain error scenario leading to NaN (log10 of a non-positive argument)
static void test_param7_domain_nan()
{
    // Force inner argument to be negative so log10 is undefined
    const cmsFloat32Number x = 1.0f;
    cmsFloat64Number Params[5];
    Params[0] = 1.0;    // exponent
    Params[1] = 1.0;    // multiplier
    Params[2] = 1.0;    // multiplier inside log10
    Params[3] = -2.0;   // makes inner negative for x^Params[0] = 1
    Params[4] = 0.0;    // final add

    cmsFloat32Number res = param_7(x, Params);

    bool is_nan = std::isnan(res);
    LOG_TEST("param_7_domain_nan", is_nan);
}

// Test 4: Another normal case to ensure broader coverage with different Params
static void test_param7_normal_case3()
{
    const cmsFloat32Number x = 3.0f;
    cmsFloat64Number Params[5];
    Params[0] = 0.5;   // sqrt
    Params[1] = 2.0;   // multiplier
    Params[2] = 1.5;   // inside log10(...)
    Params[3] = 0.3;   // inside log10(...)
    Params[4] = -0.7;  // final add

    cmsFloat32Number res = param_7(x, Params);

    long double inner = Params[2] * std::pow((long double)x, (long double)Params[0]) + Params[3];
    long double expected_long = Params[1] * std::log10(inner) + Params[4];
    float expected = static_cast<float>(expected_long);

    bool ok = approx_equal(static_cast<float>(res), expected);
    LOG_TEST("param_7_normal_case3", ok);
}

// Runner
int main()
{
    std::cout << "Running unit tests for cmsFloat32Number param_7(...)" << std::endl;

    test_param7_normal_case1();
    test_param7_normal_case2();
    test_param7_domain_nan();
    test_param7_normal_case3();

    int total = g_total_tests;
    int failed = g_failed_tests;
    int passed = total - failed;

    std::cout << "\nTest Summary: " << passed << " passed, "
              << failed << " failed, out of " << total << " tests." << std::endl;

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}