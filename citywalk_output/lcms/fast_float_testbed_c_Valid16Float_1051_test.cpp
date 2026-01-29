// fast_float_test_valid16float_tests.cpp
// Unit test suite for Valid16Float(a, b) from fast_float_testbed.c
// - No Google Test required
// - Lightweight, non-terminating assertion style
// - Uses only C++11 standard library
// - Calls the focal C function via a C linkage prototype
//
// This test focuses on true/false branches of the predicate inside Valid16Float.
//
// FOCAL_DEPENDENCIES (via includes in the original source):
// - fast_float_internal.h (included in the focal class dependencies)
// - Standard library facilities (cmath, iostream, etc.)

#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// Import dependencies needed to link with the focal method.
// The focal method uses cmsBool, cmsUInt16Number, cmsFloat32Number and EPSILON_FLOAT_TESTS.
// We provide a C-linkage prototype that matches the expected signature of Valid16Float.
extern "C" {
    // In the original codebase these typedefs come from the common CMS headers.
    // We declare compatible aliases here to ensure correct linkage in C++ tests.
    typedef unsigned short cmsUInt16Number;
    typedef float cmsFloat32Number;
    typedef int cmsBool;

    // Focal method under test
    cmsBool Valid16Float(cmsUInt16Number a, cmsFloat32Number b);
}

// Lightweight test harness
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define TEST_ASSERT(cond, desc) do { \
    ++g_tests_run; \
    if (cond) { \
        ++g_tests_passed; \
    } else { \
        std::cerr << "TEST FAILED: " << desc << " (line " << __LINE__ << ")" << std::endl; \
    } \
} while(false)

//
// Test 1: Lower boundary - a=0, b=0.0 should be valid (true branch)
// This ensures the normalized value equals 0.0 and the comparison succeeds.
//
static void test_valid16float_zero_zero() {
    cmsUInt16Number a = 0;
    cmsFloat32Number b = 0.0f;
    cmsBool result = Valid16Float(a, b);
    TEST_ASSERT(result != 0, "Valid16Float(0, 0.0) should be true");
}

// 
// Test 2: Upper boundary - a=0xFFFF, b=1.0 should be valid (true branch)
// Normalized value is exactly 1.0, should satisfy the tolerance.
//
static void test_valid16float_max_one() {
    cmsUInt16Number a = 0xFFFF;
    cmsFloat32Number b = 1.0f;
    cmsBool result = Valid16Float(a, b);
    TEST_ASSERT(result != 0, "Valid16Float(0xFFFF, 1.0) should be true");
}

// 
// Test 3: Mid-range exact match - a=32768, b = (float)a/0xFFFF
// This computes the exact same quantity as the function's internal calculation,
// so the difference should be within EPSILON_FLOAT_TESTS (true branch).
//
static void test_valid16float_mid_exact() {
    cmsUInt16Number a = 32768;
    cmsFloat32Number b = ((cmsFloat32Number)a) / ((cmsFloat32Number)0xFFFF);
    cmsBool result = Valid16Float(a, b);
    TEST_ASSERT(result != 0, "Valid16Float(32768, a/0xFFFF) should be true");
}

// 
// Test 4: False branch - a=0, b slightly off from 0.0 should yield false
// Choose an obvious offset in the domain [0,1]. If the tolerance is small,
// the predicate should fail.
static void test_valid16float_zero_off_center() {
    cmsUInt16Number a = 0;
    cmsFloat32Number b = 0.1f; // large enough offset to exceed EPSILON_FLOAT_TESTS
    cmsBool result = Valid16Float(a, b);
    TEST_ASSERT(result == 0, "Valid16Float(0, 0.1) should be false");
}

// 
// Test 5: False branch - a=32768, b differs from true value by a noticeable amount
// This checks that a near-midpoint value does not pass when b is far from the true ratio.
//
static void test_valid16float_mid_off_center() {
    cmsUInt16Number a = 32768;
    cmsFloat32Number b_true = ((cmsFloat32Number)a) / ((cmsFloat32Number)0xFFFF);
    cmsFloat32Number b = b_true + 0.5f; // significant offset
    cmsBool result = Valid16Float(a, b);
    TEST_ASSERT(result == 0, "Valid16Float(32768, true_value+0.5) should be false");
}

// 
// Test 6: False branch - a=max with a slightly off 1.0 value
// Ensures the function does not erroneously accept near-boundary values that are off by more than EPS.
static void test_valid16float_max_off_boundary() {
    cmsUInt16Number a = 0xFFFF;
    cmsFloat32Number b = 0.999f; // slightly less than 1.0
    cmsBool result = Valid16Float(a, b);
    TEST_ASSERT(result == 0, "Valid16Float(0xFFFF, 0.999) should be false");
}

// Helper to run all tests
static void RunAllTests() {
    test_valid16float_zero_zero();
    test_valid16float_max_one();
    test_valid16float_mid_exact();
    test_valid16float_zero_off_center();
    test_valid16float_mid_off_center();
    test_valid16float_max_off_boundary();
}

int main() {
    // Run tests and report results
    RunAllTests();

    std::cout << "Valid16Float tests run: " << g_tests_run
              << ", passed: " << g_tests_passed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}