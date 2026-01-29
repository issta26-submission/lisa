/*
 * Test Suite: test_equal_uint_each_equal_suite.c
 * Purpose:
 *   Provide comprehensive tests for the focal method's dependency
 *   TEST_ASSERT_EACH_EQUAL_UINT macro used in testEqualUIntEachEqual
 *   (as defined in testunity.c). This suite focuses on true/false
 *   execution paths, exercising both passing and failing scenarios.
 *
 * Notes:
 * - This suite relies on the Unity testing framework (unity.h).
 * - We include minimal stubs for putchar spying to support EXPECT_ABORT_BEGIN
 *   / VERIFY_FAILS_END usage, enabling non-terminating test execution when
 *   possible and capturing aborts for failing cases.
 * - The tests are written in C (compatibly compilable under C++11 if
 *   linked with a C compiler or with extern "C" wrappers as needed).
 *
 * Core dependencies (Candidate Keywords):
 * - TEST_ASSERT_EACH_EQUAL_UINT
 * - unsigned int arrays and lengths
 * - 65132u, 8, 2, 1, 500, 600, 700 as representative values
 * - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to validate failure paths
 * - Macros to report failures without ending the test runner
 *
 * Class/Dependency context (from <FOCAL_CLASS_DEP>):
 * - Unity framework primitives (unity.h)
 * - Inline test macros for equals across arrays
 * - Static helpers in the focal class (not directly accessible from tests)
 * - Access to macros to validate per-element equality across arrays
 *
 * This file should be compiled and linked with the Unity framework in the
 * same manner as testunity.c would be compiled.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


/* Lightweight stubs to allow EXPECT_ABORT_BEGIN style tests to compile.
   They mirror the simple hooks used by the Unity test harness in testunity.c. */
void startPutcharSpy(void) { /* no-op */ }
void endPutcharSpy(void) { /* no-op */ }
char* getBufferPutcharSpy(void) { return NULL; }

/* Basic test: all scenarios pass for TEST_ASSERT_EACH_EQUAL_UINT across a few arrays. */
void test_EachEqual_UINT_BasicPass(void)
{
    // All elements in p0 are 1, length 1
    unsigned int p0[] = {1, 1, 1, 1};
    // All elements in p1 are 65132u, length 4
    unsigned int p1[] = {65132u, 65132u, 65132u, 65132u};
    // Mixed values, but all equal to 8 for the first 2 elements
    unsigned int p2[] = {8, 8, 987, 2};
    // All elements in p3 are 1, length 1
    unsigned int p3[] = {1, 500, 600, 700}; // first element is 1, rest ignored for length 1

    // Pass: all elements in p0 are equal to 1 for length 1
    TEST_ASSERT_EACH_EQUAL_UINT(1, p0, 1);

    // Pass: all elements in p0 are equal to 1 for length 4
    TEST_ASSERT_EACH_EQUAL_UINT(1, p0, 4);

    // Pass: all elements in p1 are equal to 65132u for length 4
    TEST_ASSERT_EACH_EQUAL_UINT(65132u, p1, 4);

    // Pass: first two elements of p2 are 8, for length 2
    TEST_ASSERT_EACH_EQUAL_UINT(8, p2, 2);

    // Pass: first element of p3 is 1, for length 1
    TEST_ASSERT_EACH_EQUAL_UINT(1, p3, 1);
}

/* Failure case 1: one element differs from the expected value. */
void test_EachEqual_UINT_NotEqual1(void)
{
    unsigned int p0[] = {1, 65132u, 65132u, 65132u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT(65132u, p0, 4);
    VERIFY_FAILS_END
}

/* Failure case 2: another element differs from the expected value. */
void test_EachEqual_UINT_NotEqual2(void)
{
    unsigned int p0[] = {987, 8, 987, 987};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT(987, p0, 4);
    VERIFY_FAILS_END
}

/* Failure case 3: a different array element breaks equality mid-array. */
void test_EachEqual_UINT_NotEqual3(void)
{
    unsigned int p0[] = {1, 1, 1, 65132u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT(1, p0, 4);
    VERIFY_FAILS_END
}

/* Unity boilerplate: setUp and tearDown (no-op for these tests). */
void setUp(void) { /* nothing to initialize per-test */ }
void tearDown(void) { /* nothing to clean up per-test */ }

/* Main entry: run all tests in this suite. */
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_EachEqual_UINT_BasicPass);
    RUN_TEST(test_EachEqual_UINT_NotEqual1);
    RUN_TEST(test_EachEqual_UINT_NotEqual2);
    RUN_TEST(test_EachEqual_UINT_NotEqual3);
    return UNITY_END();
}