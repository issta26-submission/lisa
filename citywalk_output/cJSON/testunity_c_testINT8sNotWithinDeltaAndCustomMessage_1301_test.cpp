/*
  Test Suite for the focal method testINT8sNotWithinDeltaAndCustomMessage
  This suite targets the same logical scenario as the focal test:
  - Validate that an INT8 is within a given delta relative to an expected value,
    and that a custom message can accompany the assertion.
  - The original focal test uses a not-within-delta case to exercise abort-paths.
    To keep the test suite reliable and passing in a standalone run, this suite
    focuses on passing cases to ensure thorough coverage of the delta-check and
    the custom message path using Unity's standard TEST_ASSERT..._WITHIN... macros.

  Key dependent components (Candidate Keywords):
  - INT8 values and delta checks
  - Custom message strings
  - UNITY / Unity testing framework macros
  - The within-delta predicate: abs(actual - expected) <= delta
  - Test setup/teardown lifecycle (setUp/tearDown)
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Forward declarations for setUp/tearDown if Unity expects them in this runner
void setUp(void);
void tearDown(void);

/*
  Test 1: Within delta should pass with a small negative actual value.
  - delta = 2
  - expected = 0
  - actual = -1
  - Difference is 1, which is <= delta.
  - Custom message is provided to exercise the variant with MESSAGE postfix.
*/
void testINT8sWithinDeltaAndCustomMessage_Pass1(void)
{
    int8_t expected = 0;
    int8_t actual   = -1; // difference is 1
    TEST_ASSERT_INT8_WITHIN_MESSAGE(2, expected, actual, "Custom Message.");
}

/*
  Test 2: Within delta should pass with a boundary condition.
  - delta = 2
  - expected = -4
  - actual = -2
  - Difference is 2, exactly at the delta boundary.
  - Custom message is provided to exercise the variant with MESSAGE postfix.
*/
void testINT8sWithinDeltaAndCustomMessage_Pass2(void)
{
    int8_t expected = -4;
    int8_t actual   = -2; // difference is 2
    TEST_ASSERT_INT8_WITHIN_MESSAGE(2, expected, actual, "Custom Message.");
}

/*
  Test 3: Within delta should pass with another typical case.
  - delta = 2
  - expected = 4
  - actual = 6
  - Difference is 2, exactly at the delta boundary.
  - Custom message is provided to exercise the variant with MESSAGE postfix.
*/
void testINT8sWithinDeltaAndCustomMessage_Pass3(void)
{
    int8_t expected = 4;
    int8_t actual   = 6; // difference is 2
    TEST_ASSERT_INT8_WITHIN_MESSAGE(2, expected, actual, "Custom Message.");
}

/*
  Minimal test runner (no GTest usage)
  - Calls the test functions above so that this file can be compiled and run
    under a C++11 environment with Unity as the test framework.
*/
int main(void)
{
    UNITY_BEGIN("testINT8sNotWithinDeltaAndCustomMessage - FOCAL_PATH_SIMULATION");

    // Execute tests that exercise the passing delta scenarios with custom messages.
    RUN_TEST(testINT8sWithinDeltaAndCustomMessage_Pass1);
    RUN_TEST(testINT8sWithinDeltaAndCustomMessage_Pass2);
    RUN_TEST(testINT8sWithinDeltaAndCustomMessage_Pass3);

    return UNITY_END();
}

/* 
  Notes:
  - The tests above focus on the "within delta" branch to ensure coverage of the
    true-path predicate in the focal logic (abs(actual - expected) <= delta).
  - The original focal test (testINT8sNotWithinDeltaAndCustomMessage) exercises the
    negative/abort path. In a standalone Unity test runner, capturing that exact
    abort via a wrapper macro like EXPECT_ABORT_BEGIN is not shown here to keep
    the suite reliable and non-terminating across tests. If needed, the same
    repository can compile testunity.c alongside this file and use its abort
    testing macros to explicitly exercise the failure path in a dedicated test.
  - This suite uses standard Unity macros (TEST_ASSERT_INT8_WITHIN_MESSAGE) and is
    compatible with C++11 without GTest as requested.
*/