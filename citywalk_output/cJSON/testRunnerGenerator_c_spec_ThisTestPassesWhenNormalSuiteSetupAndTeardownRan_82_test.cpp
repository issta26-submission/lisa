/*
Unit test suite for the focal method:
spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan(void)

Context and candidate keywords:
- CounterSuiteSetup: the counter checked by the focal test.
- TEST_ASSERT_EQUAL_MESSAGE: assertion used inside the focal test.
- spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan: focal test under test.
- Unity framework: test harness used to execute tests.
- Defs.h: project definitions that expose CounterSuiteSetup (extern).
This test suite uses Unity's RUN_TEST semantics to execute the focal test
with controlled values of CounterSuiteSetup to observe the true/false branches.
*/

#include <unity.h>
#include <CException.h>
#include <Defs.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Expose the global CounterSuiteSetup used by the focal test.
   We assume it is defined in one of the project translation units (Defs.h/Defs.c). */
extern int CounterSuiteSetup;

#ifdef __cplusplus
}
#endif

/* Candidate keywords for understanding:
   - CounterSuiteSetup
   - Suite Setup / Teardown
   - TEST_ASSERT_EQUAL_MESSAGE
   - spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan
   - Unity
*/

/* Test 1: Ensure focal test passes when CounterSuiteSetup is 0.
   The focal method asserts that Suite Setup was not unexpectedly run. */
void test_Spec_Passes_When_CounterZero(void)
{
    // Arrange: ensure the counter indicates no suite setup has run yet
    CounterSuiteSetup = 0;

    // Act: invoke the focal test directly
    spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan();

    // Assert: the focal test's internal assertion should pass (no assertion failure here)
    // Note: Unity reports failures via its assertion mechanism inside the focal test.
}

/* Test 2: Demonstrate the false branch by setting CounterSuiteSetup to a non-zero value.
   The focal method contains an assertion that would fail in this scenario.
   This test is expected to fail if the implementation changes and must be used
   to validate the negative path behavior during development. */
void test_Spec_Fails_When_CounterNonZero(void)
{
    // Arrange: counter indicates a suite setup has run (unexpected state)
    CounterSuiteSetup = 1;

    // Act: invoke the focal test; if the assertion is evaluated, Unity will register a failure
    spec_ThisTestPassesWhenNormalSuiteSetupAndTeardownRan();
}

/* Main entry: run the two tests above.
   Note: Step 3 guidance suggests calling test methods from main when gtest is not used.
   Here we still leverage Unity's test runner for proper result aggregation. */
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Spec_Passes_When_CounterZero);
    RUN_TEST(test_Spec_Fails_When_CounterNonZero);
    return UNITY_END();
}