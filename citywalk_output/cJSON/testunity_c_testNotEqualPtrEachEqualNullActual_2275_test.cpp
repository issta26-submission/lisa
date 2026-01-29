// test_ptr_suite.cpp
// A dedicated test suite for validating pointer-array equality checks (Unity macros)
// focusing on the scenario exercised by the focal method testNotEqualPtrEachEqualNullActual
// and related edge-cases using Unity's TEST_ASSERT_EACH_EQUAL_PTR macro.
//
// This suite is self-contained in terms of Unity usage (it links against unity.c and testunity.c
// in the project) and provides focused test cases to increase coverage around pointer comparison
// logic without relying on GTest. It is written in C++11 compatible syntax as required.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Forward declaration of the focal test from the provided file (for completeness in case we want to reference it).
// We do NOT directly RUN this focal test here to avoid changing the existing global Unity test harness.
// extern void testNotEqualPtrEachEqualNullActual(void);

// Test 1: Non-equal pointers in a two-element array should abort (pointer1 != pointer2)
void test_NotEqual_PtrEachEqualPtrArray_WithDifferentElements_ShouldAbort(void)
{
    // Arrange
    // A and B are distinct objects; their addresses should be different.
    char A = 1;
    char B = 2;
    // p0 is an array of two pointers to char, i.e., char*[]
    // The first element points to A, the second to B (different addresses).
    char* p0[] = { &A, &B };

    // Act & Assert
    // We expect TEST_ASSERT_EACH_EQUAL_PTR to fail because p0[1] != &A
    // Use Unity's abort capture macros to verify that the assertion aborts as expected.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p0, 2);
    VERIFY_FAILS_END
}

// Test 2: Pointer array contains a NULL element; should abort since NULL != &A
void test_NotEqual_PtrEachEqualPtrArray_WithNullElement_ShouldAbort(void)
{
    // Arrange
    char A = 1;
    // p0[0] points to A, p0[1] is NULL
    char* p0[] = { &A, NULL };

    // Act & Assert
    // This should abort because the second element is NULL and does not equal &A
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p0, 2);
    VERIFY_FAILS_END
}

// Test 3: All pointers in the array are identical to the expected pointer; should pass
void test_EachEqualPtrArray_AllMatches_ShouldPass(void)
{
    // Arrange
    char A = 1;
    // Both elements point to the same address
    char* p0[] = { &A, &A, &A };

    // Act & Assert
    // All elements equal to &A; the assertion should pass without abort
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p0, 3);
}

// C++11/CMake compatible main to run the above tests in isolation
// This provides an independent test runner that can be compiled and run standalone.
// It is especially useful when GTest is not desired and Unity is the chosen framework.

int main(void)
{
    UNITY_BEGIN();

    // Register tests that exercise pointer equality scenarios
    RUN_TEST(test_NotEqual_PtrEachEqualPtrArray_WithDifferentElements_ShouldAbort);
    RUN_TEST(test_NotEqual_PtrEachEqualPtrArray_WithNullElement_ShouldAbort);
    RUN_TEST(test_EachEqualPtrArray_AllMatches_ShouldPass);

    return UNITY_END();
}