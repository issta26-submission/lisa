/*
 * Test Suite for focal method: testGreaterThanINT
 * Context:
 * - The focal function demonstrates usage of the Unity testing macro:
 *   TEST_ASSERT_GREATER_THAN_INT(a, b) which asserts that b > a.
 * - The original test in testunity.c demonstrates four equivalent uses with
 *   inline and pointer-derived values.
 *
 * This suite provides:
 * - Explicit passing scenario (all four usages pass).
 * - Explicit failing scenario (all four usages fail when second <= first).
 * - Pointer-only usage and pointer+value mixes to exercise the macro with
 *   different argument evaluation paths.
 *
 * Notes:
 * - This file is written to be compiled under C++11 as part of a Unity-based
 *   test harness. It uses extern "C" for the Unity C API when compiled as C++.
 * - No GTest/GMock is used.
 * - The tests are designed to be non-terminating (using Unity's EXPECT_ABORT_BEGIN
 *   only in intentional-failure cases) to maximize code coverage.
 *
 * Candidate Keywords (from the focal method):
 * - UNITY_INT, v0, v1
 * - UNITY_INT*, p0, p1
 * - &v0, &v1, *p0, *p1
 * - TEST_ASSERT_GREATER_THAN_INT
 * - Pointers and dereferenced operands
 * - The concept that the second parameter is the value that must be greater than the first
 */

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

#ifdef __cplusplus
}
#endif

// Candidate Keywords (for traceability in test design)
// UNITY_INT, v0, v1, p0, p1, &v0, &v1, *p0, *p1, TEST_ASSERT_GREATER_THAN_INT

// Test 1: All four usages pass with inline values (mirroring the focal method's positive scenario)
void testGreaterThanINT_AllCasesPass(void)
{
    // Scenario mirrors testGreaterThanINT() in testunity.c
    // v0 = 302, v1 = 3334; both pointers point to respective values
    UNITY_INT v0 = 302;
    UNITY_INT v1 = 3334;
    UNITY_INT *p0 = &v0;
    UNITY_INT *p1 = &v1;

    // All four invocations should pass since v1 > v0
    TEST_ASSERT_GREATER_THAN_INT(v0, v1);
    TEST_ASSERT_GREATER_THAN_INT(*p0, v1);
    TEST_ASSERT_GREATER_THAN_INT(v0, *p1);
    TEST_ASSERT_GREATER_THAN_INT(*p0, *p1);
}

// Test 2: All four usages fail when the second value is not greater than the first
void testGreaterThanINT_AllCasesFail(void)
{
    // In this scenario, v0 > v1, so the assertion should fail
    UNITY_INT v0 = 3334;
    UNITY_INT v1 = 302;
    UNITY_INT *p0 = &v0;
    UNITY_INT *p1 = &v1;

    // Expect the first assertion to fail and abort the test
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_INT(v0, v1);
    VERIFY_FAILS_END
}

// Test 3: Pointer-only usage with all-pass condition (dereferenced operands only)
void testGreaterThanINT_PointerOnlyAllPass(void)
{
    // Use pointers for both operands only (dereferenced values)
    UNITY_INT v0 = 2;
    UNITY_INT v1 = 5;
    UNITY_INT *p0 = &v0;
    UNITY_INT *p1 = &v1;

    // All four dereferenced-operand checks should pass since v1 > v0
    TEST_ASSERT_GREATER_THAN_INT(*p0, *p1);
    TEST_ASSERT_GREATER_THAN_INT(*p0, v1);
    TEST_ASSERT_GREATER_THAN_INT(v0, *p1);
    TEST_ASSERT_GREATER_THAN_INT(v0, v1);
}

// Test 4: Pointer-only usage with intentional failure (to exercise abort path on
// first failing assertion when working with pointers)
void testGreaterThanINT_PointerOnlyFail(void)
{
    // v0 > v1 so first assertion would fail; use values to ensure failure
    UNITY_INT v0 = 10;
    UNITY_INT v1 = 3;
    UNITY_INT *p0 = &v0;
    UNITY_INT *p1 = &v1;

    // Expect the first assertion to fail
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_INT(*p0, *p1);
    VERIFY_FAILS_END
}

// Minimal main to run the suite without GTest
int main(void)
{
    // In a C++11 environment, Unity requires a calling context from main.
    // We explicitly invoke the tests from main to maximize compatibility.
    UnityBegin("testGreaterThanINT_suite");

    // Run tests
    RUN_TEST(testGreaterThanINT_AllCasesPass);
    RUN_TEST(testGreaterThanINT_AllCasesFail);
    RUN_TEST(testGreaterThanINT_PointerOnlyAllPass);
    RUN_TEST(testGreaterThanINT_PointerOnlyFail);

    return UnityEnd();
}