/*
 * Enhanced test suite for the focal method testLessThanINT16
 * This file adds additional non-GTest based tests to exercise
 * various input scenarios for the Unity macro TEST_ASSERT_LESS_THAN_INT16.
 * It is designed to be compiled with C++11 (as the project requires)
 * and uses the Unity framework (unity.h) directly.
 *
 * Key Candidate Keywords (from the focal method):
 * - UNITY_INT16, v0, v1, p0, p1
 * - TEST_ASSERT_LESS_THAN_INT16
 * - Pointer dereferencing: *p0, *p1
 * - Primitive values: 32767, -32768, etc.
 *
 * Notes:
 * - All tests that should pass use direct values or pointers that satisfy the condition.
 * - Tests that should fail use EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to ensure
 *   non-terminating assertions are exercised without stopping the test run.
 * - A small setUp/tearDown is provided for completeness and future extension.
 * - The test runner is written in main() following the guidance to invoke tests
 *   explicitly from main (no GTest used).
 */

extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

}


// Forward declarations for Unity setup/teardown (no state required here)
void setUp(void);
void tearDown(void);

// Candidate Keywords (for documentation and traceability)
//
// - UNITY_INT16 v0, v1; UNITY_INT16 *p0, *p1
// - v0, v1 values chosen to cover edge ranges: -32768 and 32767
// - TEST_ASSERT_LESS_THAN_INT16(verifies left < right for 16-bit signed)
//
// These tests focus on exercising true branches (passes) and false branches (fails)
// using both direct values and pointer dereferencing.

void setUp(void) {
    // No global or per-test setup required for these tests
}

void tearDown(void) {
    // No per-test teardown actions required
}

// Test 1: Pass scenario using primitive 16-bit values (left < right)
void testLessThanINT16_PassesWithPrimitiveAndPointers(void) {
    UNITY_INT16 a = -32768; // minimum 16-bit signed
    UNITY_INT16 b = 32767;  // maximum 16-bit signed
    UNITY_INT16 *pa = &a;
    UNITY_INT16 *pb = &b;

    // Primitive comparison passes
    TEST_ASSERT_LESS_THAN_INT16(a, b);
    // Pointer dereferenced comparison passes
    TEST_ASSERT_LESS_THAN_INT16(*pa, b);
    TEST_ASSERT_LESS_THAN_INT16(a, *pb);
    TEST_ASSERT_LESS_THAN_INT16(*pa, *pb);
}

// Test 2: Pass scenario with a negative left and positive right (broadened case)
void testLessThanINT16_PassesNegativeToPositive(void) {
    UNITY_INT16 a = -1;
    UNITY_INT16 b = 1;
    UNITY_INT16 *pa = &a;
    UNITY_INT16 *pb = &b;

    TEST_ASSERT_LESS_THAN_INT16(a, b);
    TEST_ASSERT_LESS_THAN_INT16(*pa, b);
    TEST_ASSERT_LESS_THAN_INT16(a, *pb);
    TEST_ASSERT_LESS_THAN_INT16(*pa, *pb);
}

// Test 3: Fail scenario - equal values should not satisfy "less than"
void testLessThanINT16_FailsWhenEqual(void) {
    UNITY_INT16 a = 5;
    UNITY_INT16 b = 5;
    UNITY_INT16 *pa = &a;
    UNITY_INT16 *pb = &b;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT16(a, b);
    VERIFY_FAILS_END
}

// Test 4: Fail scenario - left greater than right (primitive case)
void testLessThanINT16_FailsWhenFirstGreaterThanSecond(void) {
    UNITY_INT16 a = 10;
    UNITY_INT16 b = -5;
    UNITY_INT16 *pa = &a;
    UNITY_INT16 *pb = &b;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT16(a, b);
    VERIFY_FAILS_END
}

// Test 5: Fail scenario - dereferenced values do not satisfy the condition
void testLessThanINT16_FailsWhenDereferenced(void) {
    UNITY_INT16 a = 1;
    UNITY_INT16 b = -2;
    UNITY_INT16 *pa = &a;
    UNITY_INT16 *pb = &b;

    // Dereferenced comparison fails when left >= right
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_LESS_THAN_INT16(*pa, *pb);
    VERIFY_FAILS_END
}

// Basic test runner invoking all tests defined above
int main(void) {
    UnityBegin("testLessThanINT16_suite.cpp");

    // Register tests
    RUN_TEST(testLessThanINT16_PassesWithPrimitiveAndPointers);
    RUN_TEST(testLessThanINT16_PassesNegativeToPositive);
    RUN_TEST(testLessThanINT16_FailsWhenEqual);
    RUN_TEST(testLessThanINT16_FailsWhenFirstGreaterThanSecond);
    RUN_TEST(testLessThanINT16_FailsWhenDereferenced);

    return UnityEnd();
}