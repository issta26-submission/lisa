/*
  Focal Test Suite for testNotEqualHEX8EachEqual1 (adapted for C++11 with Unity)

  Step 1: Candidate Keywords (core dependencies observed from the focal method)
  - p0 (unsigned char array)
  - 253u, 254u (byte values used in the test)
  - 4 (array length passed to TEST_ASSERT_EACH_EQUAL_HEX8)
  - TEST_ASSERT_EACH_EQUAL_HEX8 (Unity assertion under test)
  - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END (abort-guard macros used to verify expected failure)
  - unsigned char (byte-level data type)
  - TEST_ASSERT_EACH_EQUAL_HEX8 macro usage pattern
  - The macro and test pattern rely on Unity's non-terminating test design for negative tests

  Step 2: This file provides a small, self-contained suite that focuses on the focal
  testNotEqualHEX8EachEqual1 behavior by re-creating the same failure scenario
  in separate tests (to ensure coverage across different variant inputs) without
  invoking the existing testunity.c test directly.

  Step 3: Domain knowledge considerations
  - The tests use non-terminating assertions (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END)
    to maximize code execution across the suite.
  - The tests are written to compile under C++11 as part of a Unity-based test
    harness (no GTest).
  - Static dependencies in the focal class (UNITY macros and helpers) are assumed
    available via unity.h included below.

  Note: This file is intended to be compiled alongside Unity's sources. It does not modify
  testunity.c; rather, it re-creates the focal scenario in separate test cases for
  robust coverage.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Candidate keywords are represented above; here we focus tests on the HEX8 not-equal scenario.
// Class dependencies (Unity) are included via "unity.h".
// Domain knowledge: Use non-terminating assertions to maximize code coverage.

void setUp(void)
{
    // Optional: reset any global Unity state if needed
}
void tearDown(void)
{
    // Optional: cleanup after each test
}

/*
  Test 1: Replicate the focal scenario exactly as in testNotEqualHEX8EachEqual1
  Purpose: Ensure that TEST_ASSERT_EACH_EQUAL_HEX8(253u, p0, 4) aborts when a single
           element differs from the expected 253.
*/
void testNotEqualHEX8EachEqual1_Focal(void)
{
    unsigned char p0[] = {253u, 253u, 254u, 253u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX8(253u, p0, 4);
    VERIFY_FAILS_END
}

/*
  Test 2: Variant where a different element causes the failure path to trigger.
  Purpose: Validate that a single deviation in the 4-element array reliably aborts.
*/
void testNotEqualHEX8EachEqual1_Variant2(void)
{
    unsigned char p0[] = {253u, 253u, 254u, 252u}; // third element differs and last also differs
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX8(253u, p0, 4);
    VERIFY_FAILS_END
}

/*
  Test 3: Another variant with a different misplaced value to ensure consistent failure
  across varying positions of the non-matching byte.
*/
void testNotEqualHEX8EachEqual1_Variant3(void)
{
    unsigned char p0[] = {254u, 253u, 254u, 253u}; // first element differs
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX8(253u, p0, 4);
    VERIFY_FAILS_END
}

/*
  Test Runner
  Note: If Unity uses the newer RUN_TEST macro pattern, this main function will
  execute all tests in this file. It does not rely on any private/static state
  beyond what the Unity framework provides.
*/
int main(void)
{
    UnityBegin("FocalSuite_testNotEqualHEX8EachEqual1");
    RUN_TEST(testNotEqualHEX8EachEqual1_Focal);
    RUN_TEST(testNotEqualHEX8EachEqual1_Variant2);
    RUN_TEST(testNotEqualHEX8EachEqual1_Variant3);
    return UnityEnd();
}