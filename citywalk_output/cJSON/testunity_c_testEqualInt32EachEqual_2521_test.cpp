// Test suite: Additional unit tests for the focal testEqualInt32EachEqual behavior
// This file provides extra test cases (in addition to those already present in testunity.c)
// to improve coverage of the core macro TEST_ASSERT_EACH_EQUAL_INT32 for UNITY_INT32 data.
// The tests are written for the Unity framework and are intended to be compiled together
// with the existing testunity.c in a C++11 capable project (no GTest/GMock).

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Optional: provide empty setup/teardown to keep Unity's lifecycle consistent.
void setUp(void)   { /* no global state to initialize in these tests */ }
void tearDown(void) { /* no global state to cleanup in these tests */ }

// Test 1: SINGLE element arrays should pass when the single element equals the expected value.
void testEqualInt32EachEqual_SingleElement(void)
{
    // Given a single element array with the value 8
    UNITY_INT32 p0[] = {8};
    // When we assert that every element equals 8 in a length-1 array
    // Then the assertion should pass
    TEST_ASSERT_EACH_EQUAL_INT32(8, p0, 1);
}

// Test 2: ALL elements are the same value across four positions; should pass for length 4.
void testEqualInt32EachEqual_AllFourSame(void)
{
    // Given an array with all elements equal to 8
    UNITY_INT32 p0[] = {8, 8, 8, 8};
    // When we assert that every element equals 8 in a length-4 array
    // Then the assertion should pass
    TEST_ASSERT_EACH_EQUAL_INT32(8, p0, 4);
}

// Test 3: A different value shared across all elements (e.g., 65537) with full length.
void testEqualInt32EachEqual_All65537(void)
{
    // Given an array with all elements equal to 65537
    UNITY_INT32 p1[] = {65537, 65537, 65537, 65537};
    // When we assert that every element equals 65537 in a length-4 array
    // Then the assertion should pass
    TEST_ASSERT_EACH_EQUAL_INT32(65537, p1, 4);
}

// Test 4: Negative and positive values are mixed but the first N elements are all equal
//         to the same value (-3) across the first three positions.
void testEqualInt32EachEqual_NegativeAcrossThree(void)
{
    // Given an array where first three elements are -3 and the fourth is 2
    UNITY_INT32 p2[] = {-3, -3, -3, 2};
    // When we assert that the first three elements equal -3
    // Then the assertion should pass for length 3
    TEST_ASSERT_EACH_EQUAL_INT32(-3, p2, 3);
}

// Test 5: Mixed values with the first element matching and length 1
// The test should pass by only verifying the first element.
void testEqualInt32EachEqual_OneFirstOnly(void)
{
    // Given an array where the first element is 1 and others are arbitrary
    UNITY_INT32 p3[] = {1, 50, 60, 70};
    // When we assert that the first element equals 1 in a length-1 array
    // Then the assertion should pass
    TEST_ASSERT_EACH_EQUAL_INT32(1, p3, 1);
}

// Test runner (main) to execute the above tests along with the existing test suite in testunity.c.
// Note: This follows the instruction to call test methods from main without relying solely on a test harness.
int main(void)
{
    UNITY_BEGIN();

    // Run the existing focal test (from testunity.c) to ensure it remains correct in this suite.
    // This symbol is expected to be linked from testunity.c.
    extern void testEqualInt32EachEqual(void);
    RUN_TEST(testEqualInt32EachEqual);

    // Run additional tests to strengthen coverage of TEST_ASSERT_EACH_EQUAL_INT32 usage.
    RUN_TEST(testEqualInt32EachEqual_SingleElement);
    RUN_TEST(testEqualInt32EachEqual_AllFourSame);
    RUN_TEST(testEqualInt32EachEqual_All65537);
    RUN_TEST(testEqualInt32EachEqual_NegativeAcrossThree);
    RUN_TEST(testEqualInt32EachEqual_OneFirstOnly);

    return UNITY_END();
}