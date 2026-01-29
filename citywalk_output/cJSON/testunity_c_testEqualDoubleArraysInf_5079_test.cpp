// Test suite for the focal method testEqualDoubleArraysInf
// This suite targets the Unity-based test contained in testunity.c
// It demonstrates how to exercise the focal test under C++11 without GTest.
//
// Key ideas and Candidate Keywords (Step 1):
// - testDoubleInfEqual scenario: arrays containing an Infinity value should compare equal
// - p0 and p1 arrays to compare via TEST_ASSERT_EQUAL_DOUBLE_ARRAY
// - Infinity value usage: std::numeric_limits<double>::infinity()
// - UNITY_EXCLUDE_DOUBLE guard controls whether double-precision tests are compiled
// - TEST_ASSERT_EQUAL_DOUBLE_ARRAY macro is the core assertion used by the focal test
// - Class dependencies: unity.h (Unity test framework), standard library for infinity
// - Static members: not used directly here, but test structure mirrors Unity-style tests
// - Non-terminating assertions: rely on Unity's macros (RUN_TEST, TEST_ASSERT_*), not exiting early
// - Use appropriate test harness to run tests from main()
//
// Note: This file is intended to be compiled with Unity's sources (unity.c) and linked accordingly.

#include <cstddef>
#include <limits>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Bring in Unity as C linkage to avoid name mangling when compiling as C++
extern "C" {
}

// Forward declarations of test helpers (Unity style)
extern void testEqualDoubleArraysInf(void);
extern void testEqualDoubleArraysInf_ShouldFail(void);

// Minimal setUp/tearDown to mimic Unity expectations (no-op here)
void setUp(void) { }
void tearDown(void) { }

// Implementation of tests (mirroring the focal method structure)

// Test 1: Positive case - both arrays contain Infinity and are equal
// This mirrors the focal method's purpose: verifying equal arrays with Inf values.
// If UNITY_EXCLUDE_DOUBLE is defined, this test will be ignored to mimic original behavior.
void testEqualDoubleArraysInf(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // When double is excluded, mark the test as ignored.
    TEST_IGNORE();
#else
    // Domain: Use Infinity for the second element like the focal test (1.0 / 0.0)
    // We avoid depending on d_zero from the original file by using std::numeric_limits<double>::infinity()
    const double inf = std::numeric_limits<double>::infinity();
    const double p0[] = {1.0, inf, 25.4, 0.253};
    const double p1[] = {1.0, inf, 25.4, 0.253};
    // Core assertion: arrays must be equal element-wise
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
#endif
}

// Test 2: Negative/alternative case - intentionally differing at a non-Inf element to
// exercise failure path of the same assertion type. This is not the focal path itself
// but helps improve coverage around the double-array equality assertion.
void testEqualDoubleArraysInf_ShouldFail(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // Respect the same guard; ignore if double support is excluded.
    TEST_IGNORE();
#else
    const double inf = std::numeric_limits<double>::infinity();
    // Deliberate difference in the non-Inf element to trigger a failure
    const double p0[] = {1.0, inf, 25.4, 0.253};
    const double p1[] = {1.0, inf, 25.4, 0.254}; // last element differs
    // This assertion should fail, ensuring the test framework catches mismatches
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
#endif
}

// Entry point for the test runner
int main(void)
{
    // Initialize Unity test framework
    UnityBegin("test_equal_double_infinite.cpp");

    // Execute focal test and a failure-path variant to improve coverage
    RUN_TEST(testEqualDoubleArraysInf);
    RUN_TEST(testEqualDoubleArraysInf_ShouldFail);

    // Finish and return results
    return UnityEnd();
}