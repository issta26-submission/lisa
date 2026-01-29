// test_equal_int8s_suite.cpp
// A focused test suite for the focal method testEqualInt8s,
// implemented to run under the Unity testing framework without GTest.
// This file is written in C++11-compatible syntax but uses Unity's C macros.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Dependency note:
// - Unity framework headers are required (unity.h).
// - The focal method under test mirrors the behavior of TEST_ASSERT_EQUAL_INT8
//   across direct values, pointers, and values differing outside the 8-bit range.

void setUp(void)   { /* Optional per-test setup */ }
void tearDown(void){ /* Optional per-test cleanup */ }

// Test 1: Replicate the basic scenario in testEqualInt8s
// Verifies that two UNITY_INT8 values initialized to the same 8-bit value compare equal
// and that their derivatives via pointers also compare equal.
void testEqualInt8s_Basics(void)
{
    int8_t v0, v1;
    int8_t *p0, *p1;

    // Initialize both integers to the same 0x22 value
    v0 = 0x22;
    v1 = 0x22;

    // Pointers to the two variables
    p0 = &v0;
    p1 = &v1;

    // Core assertions replicating focal method's checks
    TEST_ASSERT_EQUAL_INT8(0x22, 0x22);  // literal vs literal
    TEST_ASSERT_EQUAL_INT8(v0, v1);      // direct variable comparison
    TEST_ASSERT_EQUAL_INT8(0x22, v1);    // literal vs variable
    TEST_ASSERT_EQUAL_INT8(v0, 0x22);    // variable vs literal
    TEST_ASSERT_EQUAL_INT8(*p0, v1);     // dereferenced pointer vs another variable
    TEST_ASSERT_EQUAL_INT8(*p0, *p1);    // dereferenced pointers equal
    TEST_ASSERT_EQUAL_INT8(*p0, 0x22);   // dereferenced pointer vs literal
}

// Test 2: Ensure equality across values that differ outside the 8-bit range
// This mirrors the logic in testEqualInt8sWhenThereAreDifferencesOutside8Bits
// where 0x321 and 0x421 are both truncated to 0x21 in int8 context.
void testEqualInt8s_WhenThereAreDifferencesOutside8Bits(void)
{
    // These literals are wider than 8 bits; Unity's INT8 comparison should truncate them
    // and treat them as equal after truncation.
    TEST_ASSERT_EQUAL_INT8(0x321, 0x421);   // both truncate to 0x21
    TEST_ASSERT_EQUAL_INT8(0xFF21, 0x0021); // both truncate to 0x21
}

// Optional negative test (commented out by default to avoid aborting the test run):
// If you want to explicitly verify a failure path, you can uncomment and run this
// in a controlled Unity environment that supports EXPECT_ABORT_BEGIN-like handling.
// void testNotEqualInt8s_ShouldFail(void)
// {
//     int8_t a = -127;
//     int8_t b = -126; // clearly not equal in 8-bit context
//     EXPECT_ABORT_BEGIN
//     TEST_ASSERT_EQUAL_INT8(a, b);
//     VERIFY_FAILS_END
// }

// Main harness: Run the tests using Unity's runner in a standalone manner.
// Following domain knowledge guidance: invoke test methods from main.
int main(void)
{
    UnityBegin("test_equal_int8s_suite.cpp");

    RUN_TEST(testEqualInt8s_Basics);
    RUN_TEST(testEqualInt8s_WhenThereAreDifferencesOutside8Bits);

    // If you add more tests, remember to register them here with RUN_TEST.

    return UnityEnd();
}