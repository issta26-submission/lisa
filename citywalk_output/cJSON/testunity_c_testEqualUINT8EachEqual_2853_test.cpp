/*
  Test Suite: Extended coverage for testEqualUINT8EachEqual in testunity.c
  - This file adds targeted tests to exercise additional scenarios for
    the UNITY_UINT8/EACH_EQUAL_UINT8 pathway.
  - Domain knowledge highlights:
    - Core components: UNITY_UINT8, TEST_ASSERT_EACH_EQUAL_UINT8
    - Test macros: EXPECT_ABORT_BEGIN / VERIFY_FAILS_END (non-terminating style)
    - Static and dependencies are handled by existing Unity setup in testunity.c
  - This suite is designed to be compiled with a C++11 capable toolchain (per project notes)
    but avoids GTest and uses Unity-style assertions as requested.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Forward declarations for testfunctions with C linkage to avoid name mangling
void testEqualUINT8EachEqual_Extended_AllElementsZero(void);
void testNotEqualUINT8EachEqual_Extended_FailsOnSecondElem(void);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Minimal Unity-based test harness to run the added tests standalone.
// This does not modify existing testunity.c main logic; it provides an isolated
// harness to validate additional scenarios for testEqualUINT8EachEqual.
int main(void)
{
    UnityBegin("test_equal_uint8_each_equal_suite.cpp");
    RUN_TEST(testEqualUINT8EachEqual_Extended_AllElementsZero);
    RUN_TEST(testNotEqualUINT8EachEqual_Extended_FailsOnSecondElem);
    return UnityEnd();
}

// Test 1: All elements in the array are zero and should pass when expected is zero.
// This extends coverage for the 4-element case where all values match the expectation.
void testEqualUINT8EachEqual_Extended_AllElementsZero(void)
{
    // Candidate Keywords: UNITY_UINT8, TEST_ASSERT_EACH_EQUAL_UINT8, p0, count
    UNITY_UINT8 p0[] = {0u, 0u, 0u, 0u};
    // All elements equal to 0, count = 4
    TEST_ASSERT_EACH_EQUAL_UINT8(0u, p0, 4);
}

// Test 2: Negative path for UINT8-EACH_EQUAL: mismatch occurs at a later index (second element)
// This ensures non-terminating test flow still records a failure correctly.
void testNotEqualUINT8EachEqual_Extended_FailsOnSecondElem(void)
{
    // Candidate Keywords: UNITY_UINT8, TEST_ASSERT_EACH_EQUAL_UINT8, p0, count
    UNITY_UINT8 p0[] = {0u, 1u, 0u, 0u};
    // Expect the test to abort due to a mismatch at index 1 (second element)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_UINT8(0u, p0, 4);
    VERIFY_FAILS_END
}

#ifdef __cplusplus
}
#endif