/*
 Candidate Keywords:
 - UNITY_INT8, TEST_ASSERT_LESS_THAN_INT8
 - Pointers: UNITY_INT8 *p0, *p1 and dereferencing
 - Boundary values for int8: -128 and 127
 - Non-terminating assertions: EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
 - Coverage goals: all variants of TEST_ASSERT_LESS_THAN_INT8 with direct values and via pointers
 - Domain considerations: compile under C++11, use Unity-style macros, no GTest
 - Test harness: implement multiple test cases and a main runner invoking RUN_TEST
*/

// This test suite focuses on the focal method testLessThanINT8.
// It creates several scenarios to robustly exercise TEST_ASSERT_LESS_THAN_INT8
// both with direct values and with pointer dereferencing, including passing
// and failing cases. The failing cases are captured using EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
// to avoid terminating the entire test run.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Forward declarations of test cases for linkage clarity (extern "C" for C linkage in C++ builds)
#ifdef __cplusplus
extern "C" {
#endif

void testLessThanINT8_PassDirect(void);
void testLessThanINT8_PassPointerDereferenced(void);
void testLessThanINT8_PassCrossDereferenced(void);
void testLessThanINT8_PassBothDereferenced(void);

void testLessThanINT8_FailDirect(void);
void testLessThanINT8_FailPointerDereferenced(void);
void testLessThanINT8_FailCrossDereferenced(void);
void testLessThanINT8_FailBothDereferenced(void);

#ifdef __cplusplus
}
#endif

// setUp and tearDown stubs to align with Unity expectations
void setUp(void) {}
void tearDown(void) {}

// Implementations

// Test 1: Direct comparison should pass when first value is less than second
// Coverage: direct value path
void testLessThanINT8_PassDirect(void)
{
    // v0 < v1 where v0 = -128 (min int8), v1 = 127 (max int8)
    UNITY_INT8 v0 = -128;
    UNITY_INT8 v1 = 127;

    // All four variations should pass
    TEST_ASSERT_LESS_THAN_INT8(v0, v1);
}

// Test 2: Dereferenced first operand should pass when *p0 < v1
// Coverage: pointer indirection on lhs only
void testLessThanINT8_PassPointerDereferenced(void)
{
    UNITY_INT8 v0 = -128;
    UNITY_INT8 v1 = 127;
    UNITY_INT8 *p0 = &v0;
    TEST_ASSERT_LESS_THAN_INT8(*p0, v1);
}

// Test 3: Dereferenced second operand should pass when v0 < *p1
// Coverage: pointer indirection on rhs only
void testLessThanINT8_PassCrossDereferenced(void)
{
    UNITY_INT8 v0 = -128;
    UNITY_INT8 v1 = 127;
    UNITY_INT8 *p1 = &v1;
    TEST_ASSERT_LESS_THAN_INT8(v0, *p1);
}

// Test 4: Both operands dereferenced should pass when *p0 < *p1
// Coverage: pointer indirection on both sides
void testLessThanINT8_PassBothDereferenced(void)
{
    UNITY_INT8 v0 = -128;
    UNITY_INT8 v1 = 127;
    UNITY_INT8 *p0 = &v0;
    UNITY_INT8 *p1 = &v1;
    TEST_ASSERT_LESS_THAN_INT8(*p0, *p1);
}

// Test 5: Direct comparison failing path (should abort). Capture failure without stopping test suite.
void testLessThanINT8_FailDirect(void)
{
    // v0 = 127, v1 = -128; 127 < -128 is false
    UNITY_INT8 v0 = 127;
    UNITY_INT8 v1 = -128;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT8(v0, v1);
    VERIFY_FAILS_END
}

// Test 6: Dereferenced lhs failing path
void testLessThanINT8_FailPointerDereferenced(void)
{
    UNITY_INT8 v0 = 127;
    UNITY_INT8 v1 = -128;
    UNITY_INT8 *p0 = &v0;
    TEST_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT8(*p0, v1);
    VERIFY_FAILS_END
}

// Test 7: Dereferenced rhs failing path
void testLessThanINT8_FailCrossDereferenced(void)
{
    UNITY_INT8 v0 = 127;
    UNITY_INT8 v1 = -128;
    UNITY_INT8 *p1 = &v1;
    TEST_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT8(v0, *p1);
    VERIFY_FAILS_END
}

// Test 8: Both operands dereferenced failing path
void testLessThanINT8_FailBothDereferenced(void)
{
    UNITY_INT8 v0 = 127;
    UNITY_INT8 v1 = -128;
    UNITY_INT8 *p0 = &v0;
    UNITY_INT8 *p1 = &v1;
    TEST_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT8(*p0, *p1);
    VERIFY_FAILS_END
}

// Main runner to execute the test suite
int main(void)
{
    // Begin Unity test run
    UnityBegin("testLessThanINT8_suite");

    // Pass cases
    RUN_TEST(testLessThanINT8_PassDirect);
    RUN_TEST(testLessThanINT8_PassPointerDereferenced);
    RUN_TEST(testLessThanINT8_PassCrossDereferenced);
    RUN_TEST(testLessThanINT8_PassBothDereferenced);

    // Fail cases (expected aborts) - these ensure negative paths are covered
    RUN_TEST(testLessThanINT8_FailDirect);
    RUN_TEST(testLessThanINT8_FailPointerDereferenced);
    RUN_TEST(testLessThanINT8_FailCrossDereferenced);
    RUN_TEST(testLessThanINT8_FailBothDereferenced);

    // End Unity test run
    return UnityEnd();
}