// Test suite for the focal method: cmsFloat64Number ab2float2(cmsUInt16Number v)
// This file provides a lightweight, framework-free unit test harness (no GTest)
// intended to be compiled with C++11. It imports the project internal headers
// and calls the C function directly. The tests aim for high coverage of the
// transformation: ab2float2(v) = (v / 256.0) - 128.0, for v in [0, 65535].
//
// Step 1 (Program Understanding & Candidate Keywords):
// - Candidate Keywords: ab2float2, cmsUInt16Number, cmsFloat64Number, v, 256.0, -128.0
// - Core dependency: the conversion from a 16-bit unsigned input to a double-valued
//   float by applying v/256.0 and subtracting 128.0. No branching exists inside
//   ab2float2, so test cases focus on boundary and representative values.
// - Related constructs in the focal class dependencies include L2float2, L2Fix2, etc,
//   but ab2float2 stands alone for this unit test.
// - Static/class-private members are not involved here; ab2float2 is a free function
//   exposed via the C interface in the library.

// Step 2 & 3: Test implementation
#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cmath>


// Import project internal definitions to obtain type aliases and the function prototype.
// The path may vary depending on the build system. Adjust if necessary.

// Function prototype for the focal method (C linkage)
extern "C" cmsFloat64Number ab2float2(cmsUInt16Number v);

// Simple test harness (no external testing framework)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Record the result of a test case
static void record_result(const std::string& testName, bool passed, const std::string& details = "")
{
    ++g_total_tests;
    if (passed) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        ++g_failed_tests;
        std::cerr << "[FAIL] " << testName;
        if (!details.empty()) std::cerr << " - " << details;
        std::cerr << std::endl;
    }
}

// Compare doubles with a tolerance to accommodate floating point arithmetic
#define ASSERT_NEAR(testName, expected, actual, tol) do { \
    double a = static_cast<double>(actual); \
    double e = static_cast<double>(expected); \
    double diff = std::fabs(a - e); \
    bool ok = diff <= (tol); \
    if (!ok) { \
        record_result(testName, false, "expected=" + std::to_string(e) + " actual=" + std::to_string(a) + " diff=" + std::to_string(diff)); \
    } else { \
        record_result(testName, true); \
    } \
} while (0)

// Test 1: v = 0 -> ab2float2(0) should be -128.0
// Verifies the lower bound mapping is correct and exact
void test_ab2float2_min()
{
    // Domain boundary: minimum possible input
    cmsUInt16Number v = 0;
    cmsFloat64Number res = ab2float2(v);
    ASSERT_NEAR("ab2float2_min (v=0) -> -128.0", -128.0, static_cast<double>(res), 1e-12);
}

// Test 2: v = 32768 -> ab2float2(32768) should be 0.0
// This is a convenient midpoint where v/256 == 128
void test_ab2float2_mid()
{
    cmsUInt16Number v = 32768;
    cmsFloat64Number res = ab2float2(v);
    ASSERT_NEAR("ab2float2_mid (v=32768) -> 0.0", 0.0, static_cast<double>(res), 1e-12);
}

// Test 3: v = 65535 -> ab2float2(65535) should be 127.99609375
// Ensures correct handling of maximum input value
void test_ab2float2_max()
{
    cmsUInt16Number v = 65535;
    cmsFloat64Number res = ab2float2(v);
    ASSERT_NEAR("ab2float2_max (v=65535) -> 127.99609375", 127.99609375, static_cast<double>(res), 1e-12);
}

// Test 4: v = 32769 -> ab2float2(32769) should be 0.00390625
// Tests behavior just above the midpoint
void test_ab2float2_near_mid_plus()
{
    cmsUInt16Number v = 32769;
    cmsFloat64Number res = ab2float2(v);
    ASSERT_NEAR("ab2float2_near_mid_plus (v=32769) -> 0.00390625", 0.00390625, static_cast<double>(res), 1e-12);
}

// Test 5: v = 32767 -> ab2float2(32767) should be -0.00390625
// Tests behavior just below the midpoint
void test_ab2float2_near_mid_minus()
{
    cmsUInt16Number v = 32767;
    cmsFloat64Number res = ab2float2(v);
    ASSERT_NEAR("ab2float2_near_mid_minus (v=32767) -> -0.00390625", -0.00390625, static_cast<double>(res), 1e-12);
}

// Main: run all tests and report summary
int main()
{
    // Step 3: Execute all tests
    test_ab2float2_min();
    test_ab2float2_mid();
    test_ab2float2_max();
    test_ab2float2_near_mid_plus();
    test_ab2float2_near_mid_minus();

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Exit code 0 if all tests passed, non-zero otherwise
    return (g_failed_tests == 0) ? 0 : 1;
}