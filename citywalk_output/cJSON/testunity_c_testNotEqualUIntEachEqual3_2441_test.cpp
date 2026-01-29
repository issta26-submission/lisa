/*
  Candidate Keywords (from the focal method analyzed)
  - TEST_ASSERT_EACH_EQUAL_UINT
  - EXPECT_ABORT_BEGIN
  - VERIFY_FAILS_END
  - unsigned int (array element type)
  - p0 (array under test)
  - 4 (length)
  - 1, 65132u (example values used in the focal test)
  - The Unity test macros and structure (Unity framework)
  - testNotEqualUIntEachEqual3 (focal method name in testunity.c)
  - Non-terminating assertions via EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
  - Memory literals / array literals
  - The concept of per-element equality testing for unsigned ints
*/

#include <limits.h>
#include <unity.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Forward declarations (optional in Unity main runners; keep for clarity)
void testNotEqualUIntEachEqual3_ExtendedMismatchFirst(void);
void testNotEqualUIntEachEqual3_ExtendedMismatchSecond(void);
void testNotEqualUIntEachEqual3_ExtendedMismatchThird(void);
void testNotEqualUIntEachEqual3_ExtendedMismatchFourth(void);
void testNotEqualUIntEachEqual3_ExtendedAllEqualShouldPass(void);
void testNotEqualUIntEachEqual3_ExtendedMultipleMismatches(void);
void testNotEqualUIntEachEqual3_ExtendedWithMaxValues(void);

/* 
  Test 1: Mismatch at the first element
  Expectation: TEST_ASSERT_EACH_EQUAL_UINT should fail and abort.
*/
void testNotEqualUIntEachEqual3_ExtendedMismatchFirst(void)
{
    {
        unsigned int p0[4] = {2u, 1u, 1u, 1u}; // first element differs
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EACH_EQUAL_UINT(1u, p0, 4);
        VERIFY_FAILS_END
    }
}

/* 
  Test 2: Mismatch at the second element
  Expectation: Failure and abort.
*/
void testNotEqualUIntEachEqual3_ExtendedMismatchSecond(void)
{
    {
        unsigned int p0[4] = {1u, 0u, 1u, 1u}; // second element differs
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EACH_EQUAL_UINT(1u, p0, 4);
        VERIFY_FAILS_END
    }
}

/* 
  Test 3: Mismatch at the third element
  Expectation: Failure and abort.
*/
void testNotEqualUIntEachEqual3_ExtendedMismatchThird(void)
{
    {
        unsigned int p0[4] = {1u, 1u, 2u, 1u}; // third element differs
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EACH_EQUAL_UINT(1u, p0, 4);
        VERIFY_FAILS_END
    }
}

/* 
  Test 4: Mismatch at the fourth element
  This case mirrors the focal test but uses a different mismatch value.
  Expectation: Failure and abort.
*/
void testNotEqualUIntEachEqual3_ExtendedMismatchFourth(void)
{
    {
        unsigned int p0[4] = {1u, 1u, 1u, UINT_MAX}; // last element differs (extended bound)
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EACH_EQUAL_UINT(1u, p0, 4);
        VERIFY_FAILS_END
    }
}

/* 
  Test 5: All elements equal to the expected value (pass path)
  This exercises the true branch where the assertion would pass (no abort).
*/
void testNotEqualUIntEachEqual3_ExtendedAllEqualShouldPass(void)
{
    {
        unsigned int p0[4] = {1u, 1u, 1u, 1u}; // all equal to 1
        TEST_ASSERT_EACH_EQUAL_UINT(1u, p0, 4); // should pass without abort
    }
}

/* 
  Test 6: Multiple mismatches across the array
  Ensures that the macro detects a mismatch regardless of position and aborts.
*/
void testNotEqualUIntEachEqual3_ExtendedMultipleMismatches(void)
{
    {
        unsigned int p0[4] = {1u, 2u, 3u, 1u}; // multiple mismatches
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EACH_EQUAL_UINT(1u, p0, 4);
        VERIFY_FAILS_END
    }
}

/* 
  Test 7: Edge values using UINT_MAX
  Verifies behavior when the array contains boundary unsigned int values.
*/
void testNotEqualUIntEachEqual3_ExtendedWithMaxValues(void)
{
    {
        unsigned int p0[4] = {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX - 1u};
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EACH_EQUAL_UINT(UINT_MAX, p0, 4);
        VERIFY_FAILS_END
    }
}

/* 
  Note on execution
  - This file provides an extended suite focused specifically on testNotEqualUIntEachEqual3 behavior
    by varying element positions and values to maximize coverage of the failure path.
  - In a standard Unity setup, you would compile this as a separate test binary or integrate these
    tests into the existing testunity.c runner. If integrating, ensure the main runner calls RUN_TEST
    for each of the testNotEqualUIntEachEqual3_Extended... functions.
  - All tests use non-terminating assertions (EXPECT_ABORT_BEGIN) to maximize code-path coverage
    while still validating that a failure occurs as expected.
*/

/* Minimal setup/teardown to satisfy Unity while not affecting test logic */
void setUp(void) {}
void tearDown(void) {}

/* Optional: Define a simple runner if you compile this file standalone
   (Uncomment if you want to run this file independently)
*/
/*
int main(void)
{
    UnityBegin("NotEqualUIntEachEqual3_ExtendedSuite.c");
    RUN_TEST(testNotEqualUIntEachEqual3_ExtendedMismatchFirst);
    RUN_TEST(testNotEqualUIntEachEqual3_ExtendedMismatchSecond);
    RUN_TEST(testNotEqualUIntEachEqual3_ExtendedMismatchThird);
    RUN_TEST(testNotEqualUIntEachEqual3_ExtendedMismatchFourth);
    RUN_TEST(testNotEqualUIntEachEqual3_ExtendedAllEqualShouldPass);
    RUN_TEST(testNotEqualUIntEachEqual3_ExtendedMultipleMismatches);
    RUN_TEST(testNotEqualUIntEachEqual3_ExtendedWithMaxValues);
    return UnityEnd();
}
*/