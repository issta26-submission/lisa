// Test suite for focal method testLessThanUINT32
// This suite is designed to be compiled with a C++11 compiler against the Unity C unit testing framework.
// It focuses on exercising the UNITY_UINT32-based less-than predicates via the macro TEST_ASSERT_LESS_THAN_UINT32
// using both direct values and pointer-dereferenced values.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Ensure Unity headers are linked with C linkage when used from C++
extern "C" {
}

// Test 1: True case where v0 < v1 using direct values and via both non-pointer and pointer forms.
// This verifies that the less-than predicate works for unsigned 32-bit values in all four forms.
void testLessThanUINT32_TrueAllForms(void)
{
    // Given two UINT32 values with v0 < v1
    UNITY_UINT32 v0 = 0;
    UNITY_UINT32 v1 = 5;
    UNITY_UINT32 *p0 = &v0;
    UNITY_UINT32 *p1 = &v1;

    // All four variants should pass (no assertion failure)
    TEST_ASSERT_LESS_THAN_UINT32(v0, v1);
    TEST_ASSERT_LESS_THAN_UINT32(*p0, v1);
    TEST_ASSERT_LESS_THAN_UINT32(v0, *p1);
    TEST_ASSERT_LESS_THAN_UINT32(*p0, *p1);
}

// Test 2: Negative case using direct values for the first argument.
// v0 is greater than v1, so TEST_ASSERT_LESS_THAN_UINT32(v0, v1) should fail.
void testLessThanUINT32_False_FirstArg(void)
{
    UNITY_UINT32 v0 = 10;
    UNITY_UINT32 v1 = 5;
    UNITY_UINT32 *p0 = &v0;
    UNITY_UINT32 *p1 = &v1;

    // Expect the assertion to fail when v0 < v1 is evaluated
    TEST_ASSERT_LESS_THAN_UINT32(v0, v1); // This should fail and mark the test as failed
}

// Test 3: Negative case using dereferenced first argument.
// Use pointers and ensure the scenario where *p0 < v1 is false (i.e., *p0 >= v1).
void testLessThanUINT32_False_DerefFirstArg(void)
{
    UNITY_UINT32 v0 = 10;
    UNITY_UINT32 v1 = 5;
    UNITY_UINT32 *p0 = &v0;
    UNITY_UINT32 *p1 = &v1;

    // *p0 (10) < v1 (5) is false; expect failure
    TEST_ASSERT_LESS_THAN_UINT32(*p0, v1);
}

// Test 4: Negative case using both pointers' dereferenced values directly.
// Ensure that TEST_ASSERT_LESS_THAN_UINT32(*p0, *p1) fails when *p0 >= *p1.
void testLessThanUINT32_False_BothPointers(void)
{
    UNITY_UINT32 v0 = 20;
    UNITY_UINT32 v1 = 5;
    UNITY_UINT32 *p0 = &v0;
    UNITY_UINT32 *p1 = &v1;

    // *p0 (20) < *p1 (5) is false; expect failure
    TEST_ASSERT_LESS_THAN_UINT32(*p0, *p1);
}

// Minimal setUp and tearDown to satisfy Unity's test lifecycle (no special setup required here)
void setUp(void)
{
    // No global setup required for these tests
}
void tearDown(void)
{
    // No global teardown required for these tests
}

// Main entry point to run the tests.
// This main is suitable for C++11 compilation with Unity.
int main(void)
{
    UNITY_BEGIN("testLessThanUINT32_suite.cpp");

    // Register tests
    RUN_TEST(testLessThanUINT32_TrueAllForms);
    RUN_TEST(testLessThanUINT32_False_FirstArg);
    RUN_TEST(testLessThanUINT32_False_DerefFirstArg);
    RUN_TEST(testLessThanUINT32_False_BothPointers);

    return UNITY_END();
}