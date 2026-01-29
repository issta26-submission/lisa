/*
Candidate Keywords extracted from the focal method and its dependencies:
- CounterTeardown (the static/global counter checked by the test)
- spec_ThisTestPassesWhenNormalTeardownRan (target under test)
- Normal Teardown (teardown phase whose side-effect updates CounterTeardown)
- TEST_ASSERT_EQUAL_MESSAGE (Unity assertion used inside the focal method)
- "Normal Teardown Wasn't Run" (assert message in the focal test)
- Unity test framework (unity.h) and mocks (mockMock.h)
- Defs.h (project-specific definitions)
- mockMock.h (mock dependencies)
This test suite focuses on exercising the focal method by manipulating the CounterTeardown
state and invoking spec_ThisTestPassesWhenNormalTeardownRan to verify the correct behavior
in both the true and false branches of the underlying condition.
*/

#include <unity.h>
#include <mockMock.h>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

// Dependencies and focal function declaration (C linkage)

// Focal function under test (C code)
void spec_ThisTestPassesWhenNormalTeardownRan(void);

// Global counter used by the focal test (must be accessible across translation units)
extern int CounterTeardown;

#ifdef __cplusplus
}
#endif

// Test 1: Verify that when CounterTeardown == 1, the focal test passes.
// Rationale: This reflects the "normal teardown" scenario where the teardown ran as expected.
void test_spec_ThisTestPassesWhenNormalTeardownRan_CounterOne_Passes(void)
{
    // Arrange: ensure the teardown counter indicates a normal teardown occurred
    CounterTeardown = 1;

    // Act: run the focal test
    spec_ThisTestPassesWhenNormalTeardownRan();

    // Assert: The focal test should pass (Unity assertion inside the focal method will succeed)
    // Note: If the assertion failed inside the focal test, Unity would record a failure and continue.
}

// Test 2: Verify that when CounterTeardown != 1, the focal test reports a failure.
// Rationale: This exercises the false branch scenario where the normal teardown did not occur.
void test_spec_ThisTestPassesWhenNormalTeardownRan_CounterNotOne_Fails(void)
{
    // Arrange: simulate that normal teardown did not occur
    CounterTeardown = 0;

    // Act: run the focal test
    spec_ThisTestPassesWhenNormalTeardownRan();

    // Assert: Expect a failure reported by Unity for the mismatch.
    // The test harness will capture this as a failed test while allowing subsequent tests to run.
}

// Main function implementing a simple Unity-based test runner.
// Note: This follows the requirement to "call test methods from the main function" without using a separate test framework runner.
int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Execute tests (cover both the true and false branches)
    RUN_TEST(test_spec_ThisTestPassesWhenNormalTeardownRan_CounterOne_Passes);
    RUN_TEST(test_spec_ThisTestPassesWhenNormalTeardownRan_CounterNotOne_Fails);

    // Return the test run results
    return UNITY_END();
}