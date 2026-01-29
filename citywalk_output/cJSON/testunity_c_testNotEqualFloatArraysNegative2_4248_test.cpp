// Candidate Keywords and Context
// - Focus: testNotEqualFloatArraysNegative2 from testunity.c
// - Key elements: two float arrays p0 and p1 that are not equal (first elements differ), length 4
// - Core operation under test (conceptually): a function that checks for equality of two float arrays
// - Dependencies mirrored from focal class: basic C-style arrays, float type, and a simple equality check
// - Domain knowledge applied: cover not-equal case, equal case, NaN handling, and zero-length scenarios
// - Non-GTest, self-contained C++11 test harness

#include <cmath>
#include <unity.h>
#include <limits>
#include <cstring>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Lightweight, self-contained test harness (no external frameworks)
using TestFunc = bool(*)();

struct TestCase {
    const char* name;
    TestFunc func;
};

// Helper: exact element-wise float array equality
static bool equal_float_arrays(const float* a, const float* b, size_t n) {
    // Compare element-wise for exact equality (bit-for-bit for primitive floats)
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

// --------------- FOCAL TEST RE-IMPLEMENTATION (Conceptual) ---------------
// The original focal test asserts NOT equal between two arrays and expects a failure.
// Here, we implement equivalent logical checks in a standalone C++ test to verify
// the core behavior: not-equal detection across several scenarios.

// Test 1: Not Equal Float Arrays Negative 2 (p0 != p1 by first element)
static bool test_NotEqualFloatArraysNegative2() {
    float p0[] = {-1.0f, -8.0f, -25.4f, -0.253f};
    float p1[] = {-2.0f, -8.0f, -25.4f, -0.253f};
    // Expectation: arrays are not equal due to first element difference
    return equal_float_arrays(p0, p1, 4) == false;
}

// Test 2: Equal Float Arrays (positive case for equality)
static bool test_EqualFloatArrays() {
    float a[] = {1.0f, -3.5f, 0.0f, 4.25f};
    float b[] = {1.0f, -3.5f, 0.0f, 4.25f};
    return equal_float_arrays(a, b, 4) == true;
}

// Test 3: Not Equal Float Arrays with NaN (NaN handling in equality)
static bool test_NotEqualFloatArraysWithNaN() {
    float a[3];
    float b[3];
    a[0] = 1.0f; a[1] = std::numeric_limits<float>::quiet_NaN(); a[2] = 2.0f;
    b[0] = 1.0f; b[1] = std::numeric_limits<float>::quiet_NaN(); b[2] = 2.0f;
    // NaN != NaN; hence arrays are not equal
    return equal_float_arrays(a, b, 3) == false;
}

// Test 4: Length Zero Case (0-length arrays considered equal)
static bool test_NotEqualFloatArraysLengthZero() {
    float a[1] = {0.0f};
    float b[1] = {0.0f};
    // For length 0, treat as equal (no elements to compare)
    // The function should report equality for n == 0
    return equal_float_arrays(a, b, 0) == true;
}

// --------------- Test Registry and Runner ---------------

static std::vector<TestCase> g_tests = {
    { "test_NotEqualFloatArraysNegative2", test_NotEqualFloatArraysNegative2 },
    { "test_EqualFloatArrays", test_EqualFloatArrays },
    { "test_NotEqualFloatArraysWithNaN", test_NotEqualFloatArraysWithNaN },
    { "test_NotEqualFloatArraysLengthZero", test_NotEqualFloatArraysLengthZero }
};

int main() {
    int total = static_cast<int>(g_tests.size());
    int passed = 0;

    std::cout << "Running " << total << " tests (C++11, self-contained harness)" << std::endl;

    for (const auto& t : g_tests) {
        bool result = false;
        try {
            result = t.func();
        } catch (...) {
            // If a test throws, treat as failure to maximize coverage fidelity
            result = false;
        }
        std::cout << "[ " << (result ? "PASS" : "FAIL") << " ] " << t.name << std::endl;
        if (result) ++passed;
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}