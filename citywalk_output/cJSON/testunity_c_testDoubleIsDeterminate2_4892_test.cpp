/*
 Candidate Keywords (Step 1):
 - UNITY_EXCLUDE_DOUBLE, TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE
 - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
 - TEST_IGNORE
 - unity test macros, UnityBegin/UnityEnd
 - testDoubleIsDeterminate2 (focal method under test context)
 - true/false coverage paths via compile-time gate
 - non-GTest (GTest-free) C-style Unity framework
 - C++11 compatible test file (no GMock/GTest)
 - Expose test behavior for when double is included vs excluded
 
 This file provides a small, focused test-suite to validate the focal behavior
 of testDoubleIsDeterminate2() by exercising both branches of the compile-time
 gate UNITY_EXCLUDE_DOUBLE. It uses Unity's non-terminating assertion style
 to maximize coverage without stopping test execution.
*/

// Domain knowledge hints embedded as comments for clarity (Step 3):
// - Compile-time gate UNITY_EXCLUDE_DOUBLE controls whether double support is compiled in.
// - The focal test uses TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(-88.3) inside an
//   EXPECT_ABORT_BEGIN/VERIFY_FAILS_END wrapper to verify that the value is determinately
//   determinate (i.e., the assertion should fail and abort).
// - When UNITY_EXCLUDE_DOUBLE is defined, the test should be ignored (TEST_IGNORE()).
// - The tests should be executable in a C++11 project without GTest, using Unity's API.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


 /*
  Test 1 (ShouldAbort_WhenDoubleIsNotExcluded):
  - Under normal conditions (UNITY_EXCLUDE_DOUBLE not defined), testDoubleIsDeterminate2()
    is expected to abort because TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(-88.3) will fail.
  This test uses Unity's EXPECT_ABORT_BEGIN/VERIFY_FAILS_END to catch the abort
  and verify correct behavior without terminating the entire test suite.
 */
#ifndef UNITY_EXCLUDE_DOUBLE
void testDoubleIsDeterminate2_ShouldAbort_WhenDoubleIsNotExcluded(void)
{
    // Explanation: this test ensures that the focal assertion indeed aborts for a determinate double value.
    // We wrap the assertion with Unity's abort-catching macros to allow the test suite to continue.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_DOUBLE_IS_NOT_DETERMINATE(-88.3);
    VERIFY_FAILS_END
}
#else
/*
 Test 2 (ShouldBeIgnored_WhenDoubleIsExcluded):
 - When UNITY_EXCLUDE_DOUBLE is defined, the focal test path is compiled to skip
   execution (as per the focal's code path). We replicate that behavior here by issuing
   TEST_IGNORE(), allowing the test runner to report the test as ignored rather than failed.
*/
void testDoubleIsDeterminate2_ShouldBeIgnored_WhenDoubleIsExcluded(void)
{
    TEST_IGNORE();
}
#endif

/* 
 main() driver to run the focused tests.
 The specific test compiled depends on the UNITY_EXCLUDE_DOUBLE macro state.
*/
int main(void)
{
    UNITY_BEGIN();

#ifdef UNITY_EXCLUDE_DOUBLE
    // Double support is excluded; run the test that verifies the test is ignored.
    RUN_TEST(testDoubleIsDeterminate2_ShouldBeIgnored_WhenDoubleIsExcluded);
#else
    // Double support is included; run the test that verifies abort behavior.
    RUN_TEST(testDoubleIsDeterminate2_ShouldAbort_WhenDoubleIsNotExcluded);
#endif

    return UNITY_END();
}