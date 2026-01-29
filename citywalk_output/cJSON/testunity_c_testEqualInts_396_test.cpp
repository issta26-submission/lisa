/*
  Test Suite: Focused coverage for testEqualInts behavior using Unity (C unit testing)
  Context:
  - This file provides additional unit tests aimed at exercising integer equality checks
    that are exercised by the focal method testEqualInts in testunity.c.
  - It uses Unity's TEST_ASSERT_EQUAL_INT macro and also demonstrates negative tests
    using EXPECT_ABORT_BEGIN/VERIFY_FAILS_END semantics as defined in the project.
  - The suite is written to be compiled under C++11 (extern "C" linkage where necessary)
    and relies on unity.h being available in the include path.

  Candidate Keywords (core dependencies of the focal method):
  - v0, v1: integer variables used for equality checks
  - p0, p1: integer pointers
  - *p0, *p1: dereferenced pointer values
  - 19467, 1837, -27365: example literals used in assertions
  - TEST_ASSERT_EQUAL_INT: assertion primitive for int equality
  - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: mechanism to assert expected failures
  - 1234, 1234, 10, -5, -6: alternative literals / scenarios to broaden coverage
  - NULL / address comparisons are not the focus here, but pointer indirection is tested
  - The tests emphasize both true (pass) and false (fail) branches of equality predicates
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations to ensure C linkage for the Unity test runner
void testEqualInts_BasicEquality(void);
void testEqualInts_WithPointersAndLiterals(void);
void testNotEqualInts_BasicFailure(void);
void testNotEqualInts_Negatives(void);
void testNotEqualInts_Pointers(void);

#ifdef __cplusplus
}
#endif

/* 
  Test 1: Basic equality with direct ints and pointers
  - Verifies that two equal ints compare equal
  - Includes comparisons between literals, variables, and their dereferenced pointers
  - Demonstrates the core functionality that testEqualInts is designed to validate
*/
void testEqualInts_BasicEquality(void) {
    int a = 19467;
    int b = 19467;
    int *pa = &a;
    int *pb = &b;

    // True branch: identical literals
    TEST_ASSERT_EQUAL_INT(19467, 19467);

    // True branch: variables with identical values
    TEST_ASSERT_EQUAL_INT(a, b);

    // True branch: dereferenced pointers yielding equal values
    TEST_ASSERT_EQUAL_INT(*pa, *pb);

    // True branch: cross-comparison between a variable and a dereferenced pointer
    TEST_ASSERT_EQUAL_INT(a, *pb);
}

/* 
  Test 2: Equality across mixed sources (variables, literals, and pointers)
  - Confirms consistency of equality checks when combining different sources
  - Ensures pointers dereference to the same value as the corresponding variable/literal
*/
void testEqualInts_WithPointersAndLiterals(void) {
    int v0 = 1234;
    int v1 = 1234;
    int *p0 = &v0;
    int *p1 = &v1;

    // Value-based equality across different sources
    TEST_ASSERT_EQUAL_INT(v0, v1);
    TEST_ASSERT_EQUAL_INT(1234, v0);
    TEST_ASSERT_EQUAL_INT(1234, *p1);
    TEST_ASSERT_EQUAL_INT(*p0, *p1);
}

/* 
  Test 3: Negative test - basic failure case for integer equality
  - Verifies that a deliberate mismatch triggers an abort in the test
  - Uses the expected abort mechanism to allow continued test execution
*/
void testNotEqualInts_BasicFailure(void) {
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT(1, 2);
    VERIFY_FAILS_END
}

/* 
  Test 4: Negative test - negative numbers equality failure
  - Ensures that inequality with negative values is handled correctly by the assertion macro
*/
void testNotEqualInts_Negatives(void) {
    int a = -5;
    int b = -6;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT(a, b);
    VERIFY_FAILS_END
}

/* 
  Test 5: Negative test - pointer-based inequality failure
  - Compares two distinct integers via pointers to force a failure on equality
*/
void testNotEqualInts_Pointers(void) {
    int a = 10;
    int b = 11;
    int *pa = &a;
    int *pb = &b;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_INT(*pa, *pb);
    VERIFY_FAILS_END
}

int main(void) {
    UNITY_BEGIN();

    // Run focused tests related to testEqualInts coverage
    RUN_TEST(testEqualInts_BasicEquality);
    RUN_TEST(testEqualInts_WithPointersAndLiterals);
    RUN_TEST(testNotEqualInts_BasicFailure);
    RUN_TEST(testNotEqualInts_Negatives);
    RUN_TEST(testNotEqualInts_Pointers);

    return UNITY_END();
}