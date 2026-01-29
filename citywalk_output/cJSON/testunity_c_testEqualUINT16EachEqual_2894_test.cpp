/***************************************************************
 * Test Suite: Expanded coverage for focal method testEqualUINT16EachEqual
 * Context: The focal method under test is testEqualUINT16EachEqual
 *        (located in testunity.c). We create an independent
 *        Unity-based test suite to exercise its core behavior
 *        (i.e., that TEST_ASSERT_EACH_EQUAL_UINT16 correctly
 *        verifies that every element in the array equals the
 *        provided value for a given length).
 *
 * Notes per instructions:
 *  - The suite is written for C++11 compilation but uses the Unity
 *    C unit test framework. We wrap the include for C linkage.
 *  - We focus on positive-path coverage to ensure the focal method
 *    behaves correctly under typical, valid inputs.
 *  - Explanatory comments are added to each unit test.
 *  - The code aims to be self-contained and compilable in a project
 *    that already includes Unity (Unity must be linked with this test).
 *
 * Candidate Keywords extracted from the focal method:
 *  - TEST_ASSERT_EACH_EQUAL_UINT16
 *  - p0, p1, p2, p3 (array operands)
 *  - 65132u, 987, 1, 1, 500, 600, 700 (test values)
 *  - unsigned short (UINT16)
 *  - 4, 1, 3 (lengths)
 *
 * Dependencies:
 *  - Unity framework headers (unity.h)
 *  - Core types: unsigned short
 *
 * This file does not depend on private/static members of the original
 * testunity.c file and uses only the public Unity API.
 ***************************************************************/

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

// Declarations for setup/teardown if the Unity harness expects them.
// They are optional here since we only rely on positive-path tests.
void setUp(void);
void tearDown(void);

/* ---------------------- Test Prototypes ---------------------- */

// Expanded positive-path tests for testEqualUINT16EachEqual
void testEqualUINT16EachEqual_Extended1(void); // All first 4 elements equal to 65132
void testEqualUINT16EachEqual_Extended2(void); // All 4 elements equal to 987
void testEqualUINT16EachEqual_Extended3(void); // First 3 elements equal to 1, last is  , 2
void testEqualUINT16EachEqual_Extended4(void); // All 4 elements equal to 1 for a different array
void testEqualUINT16EachEqual_Extended5(void); // Additional array length 2 with identical values

/* ---------------------- Test Implementations ---------------------- */

 // Test 1: All elements of p0 are 65132; length = 4
 // This mirrors the focal test's first expectation.
void testEqualUINT16EachEqual_Extended1(void)
{
    // Prepare data: 4 elements all equal to 65132
    unsigned short p0[] = {65132u, 65132u, 65132u, 65132u};
    // This should pass: all elements match the scalar value
    TEST_ASSERT_EACH_EQUAL_UINT16(65132u, p0, 4);
}

// Test 2: All elements of p0 are 987; length = 4
void testEqualUINT16EachEqual_Extended2(void)
{
    // Prepare data: 4 elements all equal to 987
    unsigned short p1[] = {987, 987, 987, 987};
    // This should pass: all elements match the scalar value
    TEST_ASSERT_EACH_EQUAL_UINT16(987u, p1, 4);
}

// Test 3: Mixed scenario within array where first three are 1 and the last is 2; length = 3
// This ensures that the macro only checks the first 'count' elements and matches the
// provided value for those positions.
void testEqualUINT16EachEqual_Extended3(void)
{
    unsigned short p2[] = {1, 1, 1, 2};
    // Check only the first three elements; all are 1
    TEST_ASSERT_EACH_EQUAL_UINT16(1u, p2, 3);
}

// Test 4: All elements are 1 for a separate array; length = 1
void testEqualUINT16EachEqual_Extended4(void)
{
    unsigned short p3[] = {1, 500, 600, 700};
    // Check only the first element; it is 1
    TEST_ASSERT_EACH_EQUAL_UINT16(1u, p3, 1);
}

// Test 5: Additional array length 2 with identical values 123 and 123
// To verify a shorter array path where all elements match
void testEqualUINT16EachEqual_Extended5(void)
{
    unsigned short p4[] = {123u, 123u};
    TEST_ASSERT_EACH_EQUAL_UINT16(123u, p4, 2);
}

/* ---------------------- Test Runner ---------------------- */

int main(void)
{
    UNITY_BEGIN();

    // Run the extended positive-path tests
    RUN_TEST(testEqualUINT16EachEqual_Extended1);
    RUN_TEST(testEqualUINT16EachEqual_Extended2);
    RUN_TEST(testEqualUINT16EachEqual_Extended3);
    RUN_TEST(testEqualUINT16EachEqual_Extended4);
    RUN_TEST(testEqualUINT16EachEqual_Extended5);

    return UNITY_END();
}