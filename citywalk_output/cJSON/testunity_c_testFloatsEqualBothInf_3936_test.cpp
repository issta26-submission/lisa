// test_floats_inf.cpp
// Unit tests for the focal method testFloatsEqualBothInf defined in testunity.c
// This file uses the Unity C framework from a C++11 test harness.
// Note: This translation unit declares the focal function as extern "C" to avoid name mangling.

#ifdef __cplusplus
extern "C" {
#endif
// Focal method under test located in testunity.c
void testFloatsEqualBothInf(void);
#ifdef __cplusplus
}
#endif

#include <cmath>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// -----------------------------------------------------------------------------
// Test 1: Integration via the focal method
// This wrapper simply invokes the focal test. If the focal test passes under
// the current compile-time configuration (FLOAT support enabled), this test
// will pass. If the focal test fails (e.g., an assertion inside TEST_ASSERT_EQUAL_FLOAT
// fails), Unity will mark this test as failed.
// This directly exercises the focal's intended behavior and its interaction with
// Unity's assertion macros.
// -----------------------------------------------------------------------------
static void test_FloatsEqualBothInf_ViaFocal(void)
{
    // Delegate to the focal test. The focal's own branches (either TEST_IGNORE or
    // TEST_ASSERT_EQUAL_FLOAT) will drive the outcome.
    testFloatsEqualBothInf();
}

// -----------------------------------------------------------------------------
// Test 2: Direct INF equality check (standalone, independent of the focal's
// internal macro branching). This validates the "true" branch when floats
// are enabled.
// If UNITY_EXCLUDE_FLOAT is defined, the focal test would ignore, but this test
// provides an explicit check in case the binary is compiled with floats enabled.
// -----------------------------------------------------------------------------
static void test_FloatsEqualBothInf_DirectComputation(void)
{
#ifndef UNITY_EXCLUDE_FLOAT
    // Compute +INF twice and compare with the Unity float assertion
    // Using IEEE 754 behaviour: 1.0f / 0.0f should yield +INFINITY
    float a = 1.0f / 0.0f;
    float b = 1.0f / 0.0f;
    // This mirrors the focal's logic: two infinities produced the same value should compare equal
    TEST_ASSERT_EQUAL_FLOAT(a, b);
#else
    // If floats are excluded, the focal test would be ignored. Mark this test as ignored.
    TEST_IGNORE_MESSAGE("FLOAT support is excluded (Unity compiled with UNITY_EXCLUDE_FLOAT).");
#endif
}

// -----------------------------------------------------------------------------
// Optional: Additional coverage test for INF handling in C++11 context
// This is a supplementary check to ensure that INF values are handled correctly
// in a standalone test, without relying on the focal's internal macro.
// -----------------------------------------------------------------------------
static void test_FloatsInfDirectIsInf(void)
{
#ifndef UNITY_EXCLUDE_FLOAT
    float inf = 1.0f / 0.0f;
    // isinf should be true for +INF
    TEST_ASSERT_TRUE(std::isinf(inf));
#else
    TEST_IGNORE_MESSAGE("FLOAT support is excluded (Unity compiled with UNITY_EXCLUDE_FLOAT).");
#endif
}

// -----------------------------------------------------------------------------
// Unity test runner entry point
// As per Unity usage, we define a main to execute the tests in this translation unit.
// This main can be compiled and linked with testunity.c (which defines the focal test).
// The focal's test will be executed as part of RUN_TEST calls below.
// -----------------------------------------------------------------------------
int main(void)
{
    UNITY_BEGIN("test_floats_inf_suite");

    // Register tests
    RUN_TEST(test_FloatsEqualBothInf_ViaFocal);       // Integration path via focal
    RUN_TEST(test_FloatsEqualBothInf_DirectComputation); // Standalone INF equality check
    RUN_TEST(test_FloatsInfDirectIsInf);              // Additional direct INF check

    return UNITY_END();
}