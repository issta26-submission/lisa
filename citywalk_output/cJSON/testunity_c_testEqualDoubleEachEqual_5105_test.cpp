/*
  Test Suite: Focused tests for testEqualDoubleEachEqual (focal method)
  Context:
  - The focal method under test is testEqualDoubleEachEqual located in testunity.c
  - It exercises the Unity assertion macro TEST_ASSERT_EACH_EQUAL_DOUBLE with various arrays
  - We provide a minimal, self-contained C++11 test suite that targets the same functionality
  - No GTest is used; Unity is kept as the test framework
  - This file is designed to compile with the existing Unity-based project when linked accordingly

  Stepwise rationale (mapped to {Candidate Keywords}):
  - Dependent components: TEST_ASSERT_EACH_EQUAL_DOUBLE, double arrays, array sizes
  - Flows to cover: positive paths where all elements match the expected value
  - Domain considerations: handle multiple arrays and sizes to exercise the macro properly
  - Static members / non-private API: not applicable here; we rely on the Unity API exposed via unity.h
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

// Forward declarations to ensure C linkage can be used with RUN_TEST in C++
void testEqualDoubleEachEqual_AllOnes(void);
void testEqualDoubleEachEqual_AllOnes4(void);
void testEqualDoubleEachEqual_ThreeTwentyFour(void);
void testEqualDoubleEachEqual_SingleOne(void);
void testEqualDoubleEachEqual_Negative123(void);

#if !defined(UNITY_EXCLUDE_DOUBLE)  // If doubles are excluded, these tests will be skipped by the build configuration
// Test 1: All elements are 1.0, length 4
// Expected: All elements match the delta == default UNITY_DOUBLE tolerance
void testEqualDoubleEachEqual_AllOnes(void)
{
    double p0[] = {1.0, 1.0, 1.0, 1.0};
    TEST_ASSERT_EACH_EQUAL_DOUBLE(1.0, p0, 4);
}

// Test 2: All elements are 1.0, length 1
// Redundant usage ensuring single-element path is covered
void testEqualDoubleEachEqual_AllOnes4(void)
{
    double p0[] = {1.0, 1.0, 1.0, 1.0}; // keep array structure consistent
    TEST_ASSERT_EACH_EQUAL_DOUBLE(1.0, p0, 1);
}

// Test 3: Three elements are 25.4, array length 3
// Only the first 3 elements are validated against 25.4
void testEqualDoubleEachEqual_ThreeTwentyFour(void)
{
    double p2[] = {25.4, 25.4, 25.4, -0.2};
    TEST_ASSERT_EACH_EQUAL_DOUBLE(25.4, p2, 3);
}

// Test 4: First element is 1.0; array contains -23.0, 25.0, -0.26 as remaining
// Validates the first element only
void testEqualDoubleEachEqual_SingleOne(void)
{
    double p3[] = {1.0, -23.0, 25.0, -0.26};
    TEST_ASSERT_EACH_EQUAL_DOUBLE(1.0, p3, 1);
}

// Test 5: Negative example using a different constant to validate multiple data points
// All elements are -0.123 in this case
void testEqualDoubleEachEqual_Negative123(void)
{
    double p1[] = {-0.123, -0.123, -0.123, -0.123};
    TEST_ASSERT_EACH_EQUAL_DOUBLE(-0.123, p1, 4);
}
#endif // UNITY_EXCLUDE_DOUBLE

// --------------------------------------------------------------------------------------
// Test Runner
// --------------------------------------------------------------------------------------

int main(void)
{
#if !defined(UNITY_EXCLUDE_DOUBLE)
    // Initialize Unity test framework
    UnityBegin("test_equal_double_each_equal_suite.cpp");

    // Register and run tests
    RUN_TEST(testEqualDoubleEachEqual_AllOnes);
    RUN_TEST(testEqualDoubleEachEqual_AllOnes4);
    RUN_TEST(testEqualDoubleEachEqual_ThreeTwentyFour);
    RUN_TEST(testEqualDoubleEachEqual_SingleOne);
    RUN_TEST(testEqualDoubleEachEqual_Negative123);

    // End and return test result
    return UnityEnd();
#else
    // If doubles are excluded, still provide a minimal main so the test suite compiles
    // and reports that double-based tests were excluded
    UnityBegin("test_equal_double_each_equal_suite.cpp (Doubles Excluded)");
    return UnityEnd();
#endif
}