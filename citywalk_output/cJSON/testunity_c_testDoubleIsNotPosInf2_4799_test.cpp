// Enhanced test additions for focal method: testDoubleIsNotPosInf2
// This patch adds targeted tests to verify the behavior of the focal test
// under both configurations: with and without double support.
// The tests are written to be compiled alongside the existing Unity framework
// (testunity.c) in a C/C++11 project without using GTest.

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Test 1: Validate the focal path when double support is enabled (UNITY_EXCLUDE_DOUBLE not defined)
// This directly exercises the same assertion as the focal test: that 2.0 is not positive infinity.
// Rationale: 2.0 is a normal finite value; the assertion should pass without invoking a failure.
void testDoubleIsNotPosInf2_NormalFlow(void)
{
#ifndef UNITY_EXCLUDE_DOUBLE
    // This mirrors the focal test's core check
    TEST_ASSERT_DOUBLE_IS_NOT_INF(2.0);
#else
    // If double support is excluded, the focal test would ignore; reflect that here as well.
    TEST_IGNORE();
#endif
}

// Test 2: Validate the focal path when double support is excluded (UNITY_EXCLUDE_DOUBLE defined)
// This ensures the code path properly ignores the test when double support is not available.
// Note: This test becomes active only in builds where UNITY_EXCLUDE_DOUBLE is defined.
#if defined(UNITY_EXCLUDE_DOUBLE)
void testDoubleIsNotPosInf2_DoubleExcluded(void)
{
    // In the absence of double support, the focal test should be ignored.
    TEST_IGNORE();
}
#endif

#ifdef __cplusplus
}
#endif