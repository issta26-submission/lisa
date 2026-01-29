/***************************************************************
 * Test Suite: Focused tests for the focal method
 * Focal Method: testEqualInt16EachEqual (located in testunity.c)
 *
 * Objective:
 * - Reproduce the semantics of Unity's TEST_ASSERT_EACH_EQUAL_INT16
 * - Cover true and false branches for the dependent component
 * - Validate static data usage conceptually (via local arrays)
 *
 * Candidate Keywords (derived from the focal method and its usage):
 * - testEqualInt16EachEqual
 * - UNITY_INT16 / int16_t
 * - TEST_ASSERT_EACH_EQUAL_INT16
 * - p0, p1, p2, p3 (arrays used by the focal test)
 * - EACH_EQUAL semantics (checking all elements against a single value)
 *
 * Dependencies:
 * - This is a self-contained C++11 test harness that emulates a
 *   subset of the Unity macros for the sake of test coverage
 * - No GTest; no external dependencies required for this suite
 *
 * Notes:
 * - The real Unity-based tests call non-terminating assertions.
 * - To maximize coverage without aborting on first failure, this
 *   suite uses non-throwing checks and records pass/fail counts.
 ***************************************************************/

#include <cstddef>
#include <unity.h>
#include <cassert>
#include <iostream>
#include <functional>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Lightweight non-terminating assertion framework (non-GTest)
static int g_totalTests = 0;
static int g_failedTests = 0;

// Simple "expect" helper: records a failure but does not exit
static void expect_true(bool cond, const char* description)
{
    ++g_totalTests;
    if (!cond)
    {
        ++g_failedTests;
        std::cerr << "EXPECT_TRUE failed: " << (description ? description : "condition false") << std::endl;
    }
}

// Helper to mimic Unity's int16 equality across an array (all elements equal to 'value')
static bool equal_int16_arr(const int16_t* arr, size_t len, int16_t value)
{
    if (arr == nullptr && len > 0)
        return false;
    for (size_t i = 0; i < len; ++i)
    {
        if (arr[i] != value)
            return false;
    }
    return true;
}

// Exposed "focal" test, adapted for standalone C++11 execution
// This function mirrors the logic of testEqualInt16EachEqual from testunity.c
static void focal_test_equalInt16EachEqual_adapted()
{
    // p0: all elements equal to 1 (length 4)
    const int16_t p0[] = {1, 1, 1, 1};
    // p1: all elements equal to 987 (length 4)
    const int16_t p1[] = {987, 987, 987, 987};
    // p2: first three elements equal to -1 (length 3); last element is 2
    const int16_t p2[] = {-1, -1, -1, 2};
    // p3: first element 1, rest arbitrary (length 4)
    const int16_t p3[] = {1, 5, 600, 700};

    // True branches: all elements in each specified slice are equal to the given value
    expect_true(equal_int16_arr(p0, 1, 1), "p0[0] should be 1 (length 1 check)");
    // length 4: all four elements should be 1
    expect_true(equal_int16_arr(p0, 4, 1), "p0[0..3] should all be 1 (length 4 check)");

    // All four elements of p1 should be 987
    expect_true(equal_int16_arr(p1, 4, 987), "p1[0..3] should all be 987");

    // First three elements of p2 should be -1
    expect_true(equal_int16_arr(p2, 3, -1), "p2[0..2] should be -1 (length 3 check)");

    // First element of p3 should be 1
    expect_true(equal_int16_arr(p3, 1, 1), "p3[0] should be 1 (length 1 check)");
}

// Negative tests: ensure failures are detected but do not abort test execution

//  Null actual array with non-zero length -> should fail
static void focal_test_notEqualInt16EachEqual_NullActual()
{
    const int16_t* p = NULL;
    // Expect failure when array pointer is NULL and length > 0
    bool res = equal_int16_arr(p, 4, 1);
    expect_true(!res, "NULL array should not be considered equal (length 4)");
}

//  Case: array with a different 4th element
static void focal_test_notEqualInt16EachEqual_DiffLastElement()
{
    const int16_t p[] = {1, 1, 1, -2};
    bool res = equal_int16_arr(p, 4, 1);
    expect_true(!res, "p[0..3] not all equal to 1 (last element differs)");
}

//  Case: array with a mid-element difference
static void focal_test_notEqualInt16EachEqual_DiffMid()
{
    const int16_t p[] = {-5, -5, -1, -5};
    bool res = equal_int16_arr(p, 4, -5);
    expect_true(!res, "p[0..3] not all equal to -5 (mid element differs)");
}

//  Case: first element differs
static void focal_test_notEqualInt16EachEqual_FirstDiff()
{
    const int16_t p[] = {1, 88, 88, 88};
    bool res = equal_int16_arr(p, 4, 88);
    expect_true(!res, "p[0..3] not all equal to 88 (first element differs)");
}

// Function to run all tests and print a summary
static void run_all_tests()
{
    // True-case test
    focal_test_equalInt16EachEqual_adapted();

    // Negative-case tests
    focal_test_notEqualInt16EachEqual_NullActual();
    focal_test_notEqualInt16EachEqual_DiffLastElement();
    focal_test_notEqualInt16EachEqual_DiffMid();
    focal_test_notEqualInt16EachEqual_FirstDiff();
}

// Entry point
int main()
{
    run_all_tests();

    // Summary
    std::cout << "Total tests: " << g_totalTests << "\n";
    std::cout << "Failed tests: " << g_failedTests << "\n";

    // Return non-zero if any test failed
    return (g_failedTests != 0) ? 1 : 0;
}

/*
Notes for future extension:
- If needed, additional tests can be added to extend coverage of the
  "EACH_EQUAL" semantics for int16, int8, uint16 etc., by reusing
  the helper equal_int16_arr with different arrays and expected values.
- This harness is intentionally lightweight and self-contained to meet
  the "no GTest" constraint while providing clear, explainable test cases. 
*/