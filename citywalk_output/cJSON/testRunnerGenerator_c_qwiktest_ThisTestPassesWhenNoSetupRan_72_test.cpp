/*
 * Test suite for the focal method:
 *   qwiktest_ThisTestPassesWhenNoSetupRan(void)
 *
 * This file uses the Unity C unit testing framework to exercise the focal method
 * within a C++11 compatible test harness. It relies on the external global
 * CounterSetup (as used by the focal test) to verify the "no setup ran" condition.
 *
 * Notes:
 * - The test suite calls the focal test via Unity's RUN_TEST to ensure proper
 *   integration with Unity's setup/teardown lifecycle.
 * - Two test scenarios are provided:
 *   1) Positive: CounterSetup is 0; the focal test should pass.
 *   2) Negative: CounterSetup is forced to a non-zero value; the focal test should fail.
 *      We verify failure by inspecting UnityFailureCount (a Unity internal counter).
 * - All representative dependencies are accessed with C linkage where required.
 * - This file is designed to compile under C++11 as requested.
 */

// Include Unity headers and the required project headers with C linkage
#ifdef __cplusplus
extern "C" {
#endif
#include <unity.h>
#include <CException.h>
#include <Defs.h>
#include <stdio.h>

#ifdef __cplusplus
}
#endif

// Declare the focal function and its dependent static/global state
extern int CounterSetup; // presumed global or accessible state used by the focal test
extern void qwiktest_ThisTestPassesWhenNoSetupRan(void);

// Unity's internal failure counter (may be defined in Unity's implementation)
extern int UnityFailureCount;

#ifdef __cplusplus
extern "C" {
#endif
// Ensure this symbol is visible to C linkage for cross-language usage
#ifdef __cplusplus
}
#endif

/*
 * Test 1: Positive scenario
 * - Ensure a clean state for CounterSetup (0) before invoking the focal test.
 * - Run the focal test inside Unity's test harness to properly capture its
 *   assertions. The focal test asserts that CounterSetup == 0.
 * - After RUN_TEST returns, verify that no Unity failures were recorded for this test.
 */
static void test_qwiktest_PassesWhenNoSetupRan_WithCounterSetupZero(void)
{
    // Domain knowledge: CounterSetup must be 0 to reflect "no setup ran"
    CounterSetup = 0;
    // Reset Unity's failure counter for an isolated run
    UnityFailureCount = 0;

    // Execute the focal test within Unity's framework
    RUN_TEST(qwiktest_ThisTestPassesWhenNoSetupRan);

    // If the focal test passed, there should be no recorded failures
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, UnityFailureCount,
        "Focal test reported failures despite CounterSetup being 0");
}

/*
 * Test 2: Negative scenario (coverage)
 * - Simulate "setup ran" by setting CounterSetup to a non-zero value.
 * - Run the focal test and verify that it fails (Unity should record a failure).
 * - We use UnityFailureCount as a proxy to detect failure without altering focal code.
 *   This enhances coverage by exercising the failure path.
 */
static void test_qwiktest_FailsWhenCounterSetupNonZero(void)
{
    // Simulate that setup has run by setting the counter non-zero
    CounterSetup = 1;
    UnityFailureCount = 0;

    // Execute the focal test within Unity's framework
    RUN_TEST(qwiktest_ThisTestPassesWhenNoSetupRan);

    // Validate that a failure was indeed recorded
    if (UnityFailureCount == 0) {
        TEST_FAIL_MESSAGE("Expected focal test to fail when CounterSetup != 0, but it did not.");
    }
}

/*
 * The main entry point for this test suite.
 * It initializes Unity, runs the test cases, and returns the result.
 */
int main(void)
{
    // Begin Unity test framework
    UNITY_BEGIN("testRunnerGenerator.cpp");

    // Run positive coverage test
    RUN_TEST(test_qwiktest_PassesWhenNoSetupRan_WithCounterSetupZero);

    // Run negative coverage test to increase coverage of the failure path
    RUN_TEST(test_qwiktest_FailsWhenCounterSetupNonZero);

    // End Unity test and return the aggregated result
    return UNITY_END();
}