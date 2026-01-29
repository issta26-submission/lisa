// Lightweight C++11 unit test harness for the focal method testNotEqualPtrArrays2
// This file provides a minimal, GTest-free test suite that exercises pointer-array
// equality logic similar to Unity's TEST_ASSERT_EQUAL_PTR_ARRAY used in testunity.c.
// It is designed to compile with a C++11 compiler and run without the GTest framework.

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords extracted from the focal method and its dependencies:
// - A, B, C: local char variables used to create pointer arrays
// - p0, p1: arrays of char* pointers
// - 4: test length for the arrays
// - ptr_arrays_equal: helper to compare pointer arrays by address
// - testNotEqualPtrArrays2: focal test under consideration
// - NULL handling: tests for null pointer arrays
// - Assertions: basic PASS/FAIL signaling without terminating the entire test suite

// Lightweight assertion reporting helpers
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Simple assertion macros (non-terminating) suitable for a compact test harness
#define ASSERT_TRUE(cond) do {                                     \
    if (!(cond)) {                                                 \
        std::cerr << "ASSERT_TRUE failed at " << __FILE__ << ":"     \
                  << __LINE__ << " in " << __FUNCTION__ << std::endl; \
        ++g_tests_failed;                                          \
        return;                                                    \
    }                                                              \
} while (0)

#define ASSERT_FALSE(cond) do {                                    \
    if ( (cond)) {                                                \
        std::cerr << "ASSERT_FALSE failed at " << __FILE__ << ":"    \
                  << __LINE__ << " in " << __FUNCTION__ << std::endl; \
        ++g_tests_failed;                                          \
        return;                                                    \
    }                                                              \
} while (0)


// Helper to compare two arrays of pointers by their addresses
// a and b may be NULL. If both are NULL, they are considered equal.
// If only one is NULL, they are not equal.
static bool ptr_arrays_equal(char** a, char** b, size_t n)
{
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;

    for (size_t i = 0; i < n; ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

/*
 * Test 1: testNotEqualPtrArrays2
 * Reproduces the focal scenario where two pointer arrays are deliberately unequal
 * (first element differs). The test should detect inequality and pass since the
 * expected outcome is a failure of the equality assertion in the original test.
 * We verify that the arrays are not equal using the helper and assert accordingly.
 */
void testNotEqualPtrArrays2(void)
{
    // Local variables as in the focal test
    char A = 1;
    char B = 2;
    char C = 3;

    // p0: { &B, &B, &C, &A }
    char* p0[] = { &B, &B, &C, &A };
    // p1: { &A, &B, &C, &A }
    char* p1[] = { &A, &B, &C, &A };

    // Expected: not equal
    bool eq = ptr_arrays_equal(p0, p1, 4);
    // In original Unity test, this would abort the test; here we simply assert
    // that the arrays are not equal to validate the failure path.
    ASSERT_FALSE(eq);

    // If we reach here, the test passes (inequality detected)
    ++g_tests_run;
}

// Test 2: testNotEqualPtrArrays1
// Variation where the fourth element differs. Verifies general not-equal behavior.
void testNotEqualPtrArrays1(void)
{
    // Local variables
    char A = 1;
    char B = 2;
    char C = 3;

    // p0: { &A, &B, &C, &B }
    char* p0[] = { &A, &B, &C, &B };
    // p1: { &A, &B, &C, &A }
    char* p1[] = { &A, &B, &C, &A };

    bool eq = ptr_arrays_equal(p0, p1, 4);
    ASSERT_FALSE(eq);

    ++g_tests_run;
}

// Test 3: testNotEqualPtrArrays3
// Variation where the sequence differs in the middle. Validates not-equal detection.
void testNotEqualPtrArrays3(void)
{
    // Local variables
    char A = 1;
    char B = 2;
    char C = 3;

    // p0: { &A, &B, &B, &A }
    char* p0[] = { &A, &B, &B, &A };
    // p1: { &A, &B, &C, &A }
    char* p1[] = { &A, &B, &C, &A };

    bool eq = ptr_arrays_equal(p0, p1, 4);
    ASSERT_FALSE(eq);

    ++g_tests_run;
}

// Test 4: testEqualPtrArrays
// A positive case where two pointer arrays are exactly equal.
void testEqualPtrArrays(void)
{
    // Local variables
    char A = 1;
    char B = 2;
    char C = 3;

    // p0 and p1 identical
    char* p0[] = { &A, &B, &C };
    char* p1[] = { &A, &B, &C };

    bool eq = ptr_arrays_equal(p0, p1, 3);
    ASSERT_TRUE(eq);

    ++g_tests_run;
}

// Test 5: testNotEqualPtrArraysNullExpected
// One array is NULL, the other is non-NULL; should detect inequality.
void testNotEqualPtrArraysNullExpected(void)
{
    char A = 1;
    char B = 2;

    char** p0 = NULL;
    char* p1[] = { &A, &B };

    bool eq = ptr_arrays_equal(p0, p1, 2);
    ASSERT_FALSE(eq);

    ++g_tests_run;
}

// Test 6: testNotEqualPtrArraysNullActual
// The opposite nullity scenario: non-NULL vs NULL should be not equal.
void testNotEqualPtrArraysNullActual(void)
{
    char A = 1;
    char B = 2;

    char* p0[] = { &A, &B };
    char** p1 = NULL;

    bool eq = ptr_arrays_equal(p0, p1, 2);
    ASSERT_FALSE(eq);

    ++g_tests_run;
}

// Driver to execute all tests
int main(void)
{
    // Run tests
    testNotEqualPtrArrays2();
    testNotEqualPtrArrays1();
    testNotEqualPtrArrays3();
    testEqualPtrArrays();
    testNotEqualPtrArraysNullExpected();
    testNotEqualPtrArraysNullActual();

    // Summary
    std::cout << "Total tests run: " << g_tests_run << std::endl;
    std::cout << "Total tests failed: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}