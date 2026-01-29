// Note: This is a self-contained C++11 test suite focusing on the semantics
// of comparing two float arrays where NaN values should compare as equal
// when both positions contain NaN. It mirrors the intent of the focal method
// testEqualFloatArraysNaN from testunity.c, but using a lightweight, executable
// C++ test harness (no GTest) to ensure high coverage and standalone execution.
//
// Step 1 (Program Understanding & Candidate Keywords):
// - Core ideas: NaN-aware float array comparison, handling of zero length arrays,
//   static float NaN construction, and the presence of UNITY_FLOAT-like semantics
//   (represented here by the concept of comparing float elements with NaN-equality).
// - Candidate Keywords: NaN, TEST_ASSERT_EQUAL_FLOAT_ARRAY, UNITY_FLOAT, f_zero,
//   std::isnan, quiet_NaN, testEqualFloatArraysNaN, test infrastructure.
//
// Step 2 (Unit Test Generation):
// - Implement tests that cover: basic NaN-equality, multiple NaN positions, mismatch
//   due to a non-NaN difference, and zero-length arrays. Each test is self-contained
//   and uses a small, deterministic helper to compare arrays with NaN-awareness.
// - These tests are executable without relying on the Unity framework, but preserve
//   the spirit of the original focal test.
//
// Step 3 (Test Case Refinement):
// - Use standard C++11 facilities (std::isnan, std::numeric_limits) to generate
//   NaN values.
// - Provide clear comments describing the purpose of each test.
// - Ensure the test suite can be compiled with a C++11 compiler.

#include <cmath>
#include <cstddef>
#include <unity.h>
#include <limits>
#include <iostream>
#include <functional>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (for reference/documentation within code)
/*
  { 
    UNITY_FLOAT, f_zero, TEST_ASSERT_EQUAL_FLOAT_ARRAY, NaN, std::isnan,
    quiet_NaN, testEqualFloatArraysNaN, UNITY_EXCLUDE_FLOAT, 0.0f / f_zero
  }
*/

// Utility: equality check for two float arrays with NaN treated as equal
static bool equalFloatArraysWithNaN(const float* a, const float* b, std::size_t n)
{
    if (n == 0) {
        // Zero-length arrays are considered equal
        return true;
    }
    if (a == nullptr || b == nullptr) {
        // If either pointer is null while length is non-zero, treat as not equal
        return false;
    }
    for (std::size_t i = 0; i < n; ++i) {
        float va = a[i];
        float vb = b[i];
        // If both are NaN, treat as equal at this position
        if (std::isnan(va) && std::isnan(vb)) {
            continue;
        }
        if (va != vb) {
            return false;
        }
    }
    return true;
}

// Simple test harness (non-GTest)
static int g_total = 0;
static int g_passed = 0;
static void logTestResult(const std::string& testName, bool passed)
{
    ++g_total;
    if (passed) {
        ++g_passed;
        std::cout << "[PASS]  " << testName << "\n";
    } else {
        std::cout << "[FAIL]  " << testName << "\n";
    }
}

// Focal method-inspired tests (explanations provided per test)

// Test 1: Basic NaN equality in corresponding positions
// This mirrors the intended scenario where both arrays contain NaN at the
// same index, and the array as a whole should be considered equal.
static bool test_equal_float_arrays_nan_basic()
{
    const float nan = std::numeric_limits<float>::quiet_NaN();
    float p0[] = {1.0f, nan, 25.4f, 0.253f};
    float p1[] = {1.0f, nan, 25.4f, 0.253f};

    bool equal = equalFloatArraysWithNaN(p0, p1, 4);
    return equal;
}

// Test 2: NaN equality across multiple NaN positions
// Ensure that multiple NaN entries align as equal across the arrays.
static bool test_equal_float_arrays_nan_multiple_positions()
{
    const float nan = std::numeric_limits<float>::quiet_NaN();
    float a[] = {nan, 3.14f, nan, nan};
    float b[] = {nan, 3.14f, nan, nan};

    return equalFloatArraysWithNaN(a, b, 4);
}

// Test 3: Mismatch when a non-NaN value differs (NaN vs non-NaN is still handled)
// The presence of a non-NaN mismatch should cause the equality check to fail.
static bool test_not_equal_float_arrays_nan_mismatch()
{
    const float nan = std::numeric_limits<float>::quiet_NaN();
    float a[] = {1.0f, nan, 25.4f, 0.253f};
    float b[] = {1.0f, 2.0f, 25.4f, 0.253f}; // difference at index 1 is non-NaN

    return !equalFloatArraysWithNaN(a, b, 4);
}

// Test 4: Zero-length arrays should be considered equal
static bool test_equal_float_arrays_nan_length_zero()
{
    // Pointers can be nullptr for zero length; our helper handles n == 0
    const float* a = nullptr;
    const float* b = nullptr;
    return equalFloatArraysWithNaN(a, b, 0);
}

// Test 5: NaN in one array only should not be considered equal
static bool test_not_equal_float_arrays_nan_one_side()
{
    const float nan = std::numeric_limits<float>::quiet_NaN();
    float a[] = {nan, 1.0f, 2.0f};
    float b[] = {nan, 1.0f, 3.0f}; // difference at last element
    return !equalFloatArraysWithNaN(a, b, 3);
}

// Main
int main()
{
    // Run and log each test, with comments indicating purpose.
    {
        // Test 1: Basic NaN equality
        bool ok = test_equal_float_arrays_nan_basic();
        logTestResult("test_equal_float_arrays_nan_basic", ok);
    }

    {
        // Test 2: NaN equality across multiple positions
        bool ok = test_equal_float_arrays_nan_multiple_positions();
        logTestResult("test_equal_float_arrays_nan_multiple_positions", ok);
    }

    {
        // Test 3: Mismatch when a non-NaN value differs
        bool ok = test_not_equal_float_arrays_nan_mismatch();
        logTestResult("test_not_equal_float_arrays_nan_mismatch", ok);
    }

    {
        // Test 4: Zero-length arrays considered equal
        bool ok = test_equal_float_arrays_nan_length_zero();
        logTestResult("test_equal_float_arrays_nan_length_zero", ok);
    }

    {
        // Test 5: NaN in only one array should fail
        bool ok = test_not_equal_float_arrays_nan_one_side();
        logTestResult("test_not_equal_float_arrays_nan_one_side", ok);
    }

    // Summary
    std::cout << "Total: " << g_total << ", Passed: " << g_passed
              << ", Failed: " << (g_total - g_passed) << "\n";

    return (g_total == g_passed) ? 0 : 1;
}