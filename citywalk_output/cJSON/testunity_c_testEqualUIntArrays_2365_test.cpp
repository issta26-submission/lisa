// This file provides an extended C++11 test suite focused on validating
// the behavior of unsigned int array equality checks using Unity's
// TEST_ASSERT_EQUAL_UINT_ARRAY macro.
// It complements the focal test method testEqualUIntArrays found in testunity.c
// by adding additional scenarios to improve coverage for the dependent components.
// Note: This suite uses Unity's non-terminating failure capture macros
// (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END) to exercise failure paths
// without terminating the whole test run.

#include <cstddef>
#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Ensure Unity's C headers can be included from C++
#if defined(__cplusplus)
extern "C" {
}
#else
#endif

// Candidate Keywords extracted from the focal method (Step 1):
// - unsigned int arrays (p0, p1, p2, p3)
// - element-wise comparison across a given length (n)
// - TEST_ASSERT_EQUAL_UINT_ARRAY macro
// - boundary conditions on length (n=1, n=3, n=4, etc.)
// - success vs. failure paths (pass vs. fail assertions)
// - non-terminating failure capture (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END)

// Test 1: Base scenario mirroring the focal method to ensure baseline success
// across multiple matching and partially matching lengths.
void testEqualUIntArrays_BasePasses(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 987, 65132u}; // identical to p0
    unsigned int p2[] = {1, 8, 987, 2};       // differs in last element
    unsigned int p3[] = {1, 500, 600, 700};   // differs from p0 in elements 2-4

    // All comparisons should pass for the specified lengths
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p0, 1);  // compare first 1 element
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p0, 4);  // compare all 4 elements (identical)
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);  // identical arrays, full length
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p2, 3);  // first 3 elements match
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p3, 1);  // first element matches
}

// Test 2: Failure path when arrays differ in the last element using full length
// This ensures that a mismatch within the bounds of n triggers a failure.
void testNotEqualUIntArrays_FourthElementMismatch(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 8, 987, 65131u}; // last element differs

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 3: Failure path when arrays differ in the first differing element (mid-content)
void testNotEqualUIntArrays_SecondElementMismatch(void)
{
    unsigned int p0[] = {1, 8, 987, 65132u};
    unsigned int p1[] = {1, 9, 987, 65132u}; // second element differs

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 4: Length zero should cause a failure (boundary condition)
// This mirrors tests like testNotEqualIntArraysLengthZero in the provided suite.
void testNotEqualUIntArrays_LengthZero(void)
{
    unsigned int p0[] = {1};
    unsigned int p1[] = {1};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 0);
    VERIFY_FAILS_END
}

// Test 5: Mismatch in the first element within a shorter range
// Ensures that a mismatch within n elements is detected even if later
// elements would differ beyond n.
void testNotEqualUIntArrays_FirstElementMismatchInShortRange(void)
{
    unsigned int p0[] = {1, 2, 3, 4};
    unsigned int p1[] = {9, 2, 3, 4}; // first element differs

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 3); // consider only first 3 elements
    VERIFY_FAILS_END
}

// Optional: Additional test to confirm that when the first n elements match
// but an element within the full length differs beyond n, the macro does not fail.
// This is effectively exercised by Test 2; included here for explicit coverage check.
void testNotEqualUIntArrays_PartialMatchBeyondN(void)
{
    unsigned int p0[] = {1, 2, 3, 4};
    unsigned int p1[] = {1, 2, 3, 99}; // 4th element differs, but we compare only first 3
    // Should pass since only first 3 elements are compared
    TEST_ASSERT_EQUAL_UINT_ARRAY(p0, p1, 3);
}

// Main runner for the standalone suite (only if this file is built independently).
// If integrating into an existing test harness, this main can be omitted.
// It demonstrates how to run only the tests defined above without GTest.
int main(void)
{
    UNITY_BEGIN("test_equal_uint_arrays_suite.cpp");

    RUN_TEST(testEqualUIntArrays_BasePasses);
    RUN_TEST(testNotEqualUIntArrays_FourthElementMismatch);
    RUN_TEST(testNotEqualUIntArrays_SecondElementMismatch);
    RUN_TEST(testNotEqualUIntArrays_LengthZero);
    RUN_TEST(testNotEqualUIntArrays_FirstElementMismatchInShortRange);
    RUN_TEST(testNotEqualUIntArrays_PartialMatchBeyondN);

    return UNITY_END();
}