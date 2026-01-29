// Test suite (C++11) for the focal method testEqualDoubleArraysNaN
// This suite provides NaN-aware double-array equality checks to reflect
// the behavior exercised by the Unity test in testunity.c without using GTest.
//
// Important: The original focal test relies on Unity's TEST_ASSERT_EQUAL_DOUBLE_ARRAY
// to compare two arrays containing NaN values. Unity treats NaNs in corresponding
// positions as equal for this specific test scenario. To emulate the intent in C++,
// we implement a small NaN-aware comparison function and validate pass/fail outcomes
// with a tiny test harness.
// 
// Candidate Keywords (core dependencies and concepts):
// - double, NaN semantics, is NaN (NaN == NaN is false in C++ normally)
// - arrays of double, element-wise comparison with NaN handling
// - test name semantics, pass/fail accounting
// - no GTest, self-contained test harness

#include <cmath>
#include <cstddef>
#include <unity.h>
#include <limits>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords mapping to implementation:
// - NaN-aware comparison between two double arrays
// - Support for 4-element arrays (as in the focal test example)
static bool is_nan_double(double x) {
    // Simple NaN check without touching std::isnan to remain portable across compilers
    return x != x;
}

// Compares two arrays of doubles, treating NaNs at the same positions as equal.
// Normal equality (a[i] == b[i]) is used when both are non-NaN.
static bool equal_double_arrays_with_nan(const double* a, const double* b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (a[i] == b[i]) {
            // Exactly equal (handles all finite values and identical bit patterns)
            continue;
        }
        // If both are NaN, treat as equal for the purpose of the test
        if (is_nan_double(a[i]) && is_nan_double(b[i])) {
            continue;
        }
        // Otherwise, arrays differ
        return false;
    }
    return true;
}

// Simple test harness to report pass/fail without terminating on first failure.
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

static void log_failure(const char* testName, const char* message, int line) {
    std::cerr << "FAIL: " << testName << " at line " << line << " - " << message << std::endl;
}

// Assert helper for our NaN-aware array comparison
static void assert_equal_double_arrays_with_nan(const double* a, const double* b, size_t n, const char* testName) {
    ++g_tests_run;
    if (equal_double_arrays_with_nan(a, b, n)) {
        ++g_tests_passed;
    } else {
        ++g_tests_failed;
        log_failure(testName, "Double arrays are not equal under NaN-aware comparison", __LINE__);
    }
}

// Focal test variants (translated into C++ tests with explanatory comments)
// 1) Baseline: both arrays contain finite values and are equal
void test_baseline_equal_finite_arrays(void) {
    // Purpose: ensure that two identical finite arrays pass the NaN-aware comparison
    double p0[] = {1.0, 0.0, 25.4, 0.253};
    double p1[] = {1.0, 0.0, 25.4, 0.253};
    assert_equal_double_arrays_with_nan(p0, p1, 4, __func__);
}

// 2) NaN present in both arrays at the same position should pass (emulates Unity test with NaN)
void test_equal_double_arrays_nan_both_positions_pass(void) {
    // Purpose: verify that NaN values at matching positions are treated as equal
    const double nan = std::numeric_limits<double>::quiet_NaN();
    double p0[] = {1.0, nan, 25.4, 0.253};
    double p1[] = {1.0, nan, 25.4, 0.253};
    assert_equal_double_arrays_with_nan(p0, p1, 4, __func__);
}

// 3) NaN in one array vs finite in the other should fail
void test_equal_double_arrays_nan_mismatch_should_fail(void) {
    // Purpose: ensure mismatch where only one side is NaN is reported as not equal
    const double nan = std::numeric_limits<double>::quiet_NaN();
    double p0[] = {1.0, nan, 25.4, 0.253};
    double p1[] = {1.0, 0.0, 25.4, 0.253}; // second element finite
    assert_equal_double_arrays_with_nan(p0, p1, 4, __func__);
}

// 4) All finite values but not all equal should fail
void test_equal_double_arrays_all_finite_not_equal_should_fail(void) {
    // Purpose: ensure plain inequality is detected when Finite values differ
    double p0[] = {1.0, 0.0, 25.4, 0.253};
    double p1[] = {1.0, 0.0, 25.40, 0.254}; // last element differs slightly
    assert_equal_double_arrays_with_nan(p0, p1, 4, __func__);
}

// Helper to print final summary
static void print_summary() {
    std::cout << "\nTest run summary:\n";
    std::cout << "Total tests run: " << g_tests_run << "\n";
    std::cout << "Passed: " << g_tests_passed << "\n";
    std::cout << "Failed: " << g_tests_failed << "\n";
}

// Entry point
int main() {
    // Run focused tests that exercise NaN handling in double-array equality
    test_baseline_equal_finite_arrays();
    test_equal_double_arrays_nan_both_positions_pass();
    test_equal_double_arrays_nan_mismatch_should_fail();
    test_equal_double_arrays_all_finite_not_equal_should_fail();

    print_summary();

    // Return non-zero if any test failed, to allow CI to detect failure
    return (g_tests_failed == 0) ? 0 : 1;
}