/*
  Test Suite: testLessThanINT coverage
  Context:
  - focal method: testLessThanINT (in testunity.c)
  - dependencies: UNITY_INT (and related Unity macros), pointers, and the TEST_ASSERT_LESS_THAN_INT family
  - Objective: provide a focused suite that exercises less-than predicates for INT-type in a variety of scenarios
  - Note: This suite is written to be compiled with Unity in a C/C++ project (C++11 compatible). It uses a non-terminating, safe test approach to maximize coverage.
  - Candidate Keywords (Step 1): UNITY_INT, UNITY_INT*, v0, v1, p0, p1, TEST_ASSERT_LESS_THAN_INT
*/

/* 
  If compiled under C++, ensure C linkage for Unity headers.
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

/* -------------------------------------------------------------------------
   Candidate Keywords mapping to test focus:
   - UNITY_INT, v0, v1, p0, p1 represent value and pointer operands
   - TEST_ASSERT_LESS_THAN_INT is the core predicate under test
   - The tests exercise direct values, pointer dereferenced values, and mixed pointer/value forms
   ------------------------------------------------------------------------- */

/* Test 1: Direct values where the first is less than the second (direct comparison) */
void testLessThanINT_directTrue(void)
{
    // Arrange: choose values such that v0 < v1
    UNITY_INT v0 = 100;
    UNITY_INT v1 = 200;

    // Act & Assert: direct values
    TEST_ASSERT_LESS_THAN_INT(v0, v1);

    // No pointers involved here; ensures the direct path works
}

/* Test 2: Indirect via pointers where *p0 < v1 (pointer value path) */
void testLessThanINT_pointerTrue(void)
{
    // Arrange
    UNITY_INT v0 = 50;
    UNITY_INT v1 = 150;
    UNITY_INT* p0 = &v0;

    // Act & Assert: using *p0 and v1
    TEST_ASSERT_LESS_THAN_INT(*p0, v1);
}

/* Test 3: Indirect via pointers where v0 < *p1 (pointer value path with second param as pointer) */
void testLessThanINT_mixedPointerTrue(void)
{
    // Arrange
    UNITY_INT v0 = 300;
    UNITY_INT v1 = 301;
    UNITY_INT* p1 = &v1;

    // Act & Assert: first operand direct, second via pointer
    TEST_ASSERT_LESS_THAN_INT(v0, *p1);
}

/* Test 4: Both operands via pointers to confirm pointer-dereferenced path (p0 < p1) */
void testLessThanINT_pointerDirectMixedTrue(void)
{
    // Arrange
    UNITY_INT a = -42;
    UNITY_INT b = 13;
    UNITY_INT* p0 = &a;
    UNITY_INT* p1 = &b;

    // Act & Assert: both operands via pointers
    TEST_ASSERT_LESS_THAN_INT(*p0, *p1);
}

/* Test 5: Edge case with negative numbers where the relation holds (negative direct case) */
void testLessThanINT_negativeDirectTrue(void)
{
    // Arrange
    UNITY_INT v0 = -10;
    UNITY_INT v1 = -5;

    // Act & Assert
    TEST_ASSERT_LESS_THAN_INT(v0, v1);
}

/* Test 6: Mixed negative/positive case where relation holds (negative vs positive) */
void testLessThanINT_negativePositiveTrue(void)
{
    // Arrange
    UNITY_INT v0 = -999;
    UNITY_INT v1 = 0;

    // Act & Assert
    TEST_ASSERT_LESS_THAN_INT(v0, v1);
}

/* 
  Main runner for this focused test suite.
  This avoids relying on a pre-existing main from testunity.c and allows
  integration into projects that compile multiple test files together.
*/
int main(void)
{
    UNITY_BEGIN("testLessThanINT Suite");

    RUN_TEST(testLessThanINT_directTrue);
    RUN_TEST(testLessThanINT_pointerTrue);
    RUN_TEST(testLessThanINT_mixedPointerTrue);
    RUN_TEST(testLessThanINT_pointerDirectMixedTrue);
    RUN_TEST(testLessThanINT_negativeDirectTrue);
    RUN_TEST(testLessThanINT_negativePositiveTrue);

    return UNITY_END();
}