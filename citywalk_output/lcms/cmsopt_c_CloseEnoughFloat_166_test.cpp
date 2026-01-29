/*
  Test suite for the focal method: CloseEnoughFloat
  - Target: cmsBool CloseEnoughFloat(cmsFloat64Number a, cmsFloat64Number b)
  - Behavior: returns true if fabs(b - a) < 0.00001f, otherwise false
  - Notes:
      - The threshold is 0.00001 (float literal) but operands are double.
      - Boundary behavior: when diff == 0.00001, result is false.
      - Covers positive, negative, equal, boundary and near-boundary cases.
  - This test is written for a C++11 environment without Google Test.
*/

#include <lcms2_internal.h>
#include <cstdio>
#include <cmath>


// Bring in the C declaration for the focal function.
// We provide minimal typedefs to match the function's signature.
// This assumes the cmsBool is compatible with int and cmsFloat64Number with double.
extern "C" {
    typedef int cmsBool;
    typedef double cmsFloat64Number;

    // Declaration of the function under test.
    cmsBool CloseEnoughFloat(cmsFloat64Number a, cmsFloat64Number b);
}

// Lightweight test harness (non-terminating assertions)
// to allow multiple tests to run and accumulate failures.
static int g_total = 0;
static int g_failures = 0;

static void expect_true_bool(bool v, const char* expr, const char* file, int line) {
    ++g_total;
    if (!v) {
        ++g_failures;
        printf("Assertion failed: %s at %s:%d\n", expr, file, line);
    }
}

#define EXPECT_TRUE(x) expect_true_bool((x), #x, __FILE__, __LINE__)
#define EXPECT_FALSE(x) expect_true_bool(!(x), "!(" #x ")", __FILE__, __LINE__)

// Specialized double equality for readability in tests
static void expect_eq_double(double a, double b, const char* a_str, const char* b_str, const char* file, int line) {
    ++g_total;
    if (!(a == b)) {
        ++g_failures;
        printf("Assertion failed: %s == %s, actual %g vs %g at %s:%d\n",
               a_str, b_str, a, b, file, line);
    }
}
#define EXPECT_EQ_DOUBLE(a, b) expect_eq_double((double)(a), (double)(b), #a, #b, __FILE__, __LINE__)

// Individual test cases for CloseEnoughFloat

// Test that identical numbers return true
static void test_CloseEnoughFloat_IdenticalNumbers() {
    cmsFloat64Number a = 1.23456789;
    cmsFloat64Number b = 1.23456789;
    cmsBool res = CloseEnoughFloat(a, b);
    EXPECT_TRUE(res);
    // Explanation: diff = 0, which is < 1e-5, so should be true.
}

// Test that a value very close but below the threshold returns true
static void test_CloseEnoughFloat_BelowThreshold() {
    cmsFloat64Number a = 0.0;
    cmsFloat64Number b = 0.000009; // diff = 9e-6 < 1e-5
    cmsBool res = CloseEnoughFloat(a, b);
    EXPECT_TRUE(res);
}

// Test that a value exactly at threshold returns false (boundary behavior)
static void test_CloseEnoughFloat_AtThresholdBoundary() {
    cmsFloat64Number a = 0.0;
    cmsFloat64Number b = 0.00001; // diff = 1e-5, should be false
    cmsBool res = CloseEnoughFloat(a, b);
    EXPECT_FALSE(res);
}

// Test that a value just above the threshold returns false
static void test_CloseEnoughFloat_AboveThreshold() {
    cmsFloat64Number a = 0.0;
    cmsFloat64Number b = 0.00002; // diff = 2e-5 > 1e-5
    cmsBool res = CloseEnoughFloat(a, b);
    EXPECT_FALSE(res);
}

// Test with negative numbers to ensure absolute difference is computed correctly
static void test_CloseEnoughFloat_NegativeNumbers() {
    cmsFloat64Number a = -1.0;
    cmsFloat64Number b = -1.000009; // diff = 9e-6 < 1e-5
    cmsBool res = CloseEnoughFloat(a, b);
    EXPECT_TRUE(res);
}

// Test with larger values to ensure comparison still works across magnitudes
static void test_CloseEnoughFloat_LargeNumbers() {
    cmsFloat64Number a = 12345.6789;
    cmsFloat64Number b = 12345.67895; // diff ~ 5e-5 > 1e-5 => false
    cmsBool res = CloseEnoughFloat(a, b);
    EXPECT_FALSE(res);
}

// Run all tests and report summary
static void run_all_tests() {
    test_CloseEnoughFloat_IdenticalNumbers();
    test_CloseEnoughFloat_BelowThreshold();
    test_CloseEnoughFloat_AtThresholdBoundary();
    test_CloseEnoughFloat_AboveThreshold();
    test_CloseEnoughFloat_NegativeNumbers();
    test_CloseEnoughFloat_LargeNumbers();
}

int main() {
    run_all_tests();

    printf("CloseEnoughFloat tests completed. Total: %d, Failures: %d\n", g_total, g_failures);
    // Return non-zero if any test failed to aid integration with CI systems.
    return (g_failures == 0) ? 0 : 1;
}