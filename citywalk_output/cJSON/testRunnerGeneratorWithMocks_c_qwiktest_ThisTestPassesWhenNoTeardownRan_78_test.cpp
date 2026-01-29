/*
Unit test suite for qwiktest_ThisTestPassesWhenNoTeardownRan (located in testRunnerGeneratorWithMocks.c)

Overview:
- This suite exercises the focal function which asserts that no teardown has occurred
  (CounterTeardown == 0) via Unity's TEST_ASSERT_EQUAL_MESSAGE macro.
- The test is designed to run under Unity (C harness). It is compiled with C++11
  (hence the extern "C" guards for C symbols).
- The test sets up the precondition (CounterTeardown == 0) and then invokes the focal
  function. The test passes if the focal function completes without triggering an assertion
  failure.
- Note: For safety and simplicity, this suite runs the focal test in isolation to avoid
  incidental teardown side-effects from the Unity harness itself.
- Static members: The test directly accesses the global CounterTeardown variable to prepare
  the precondition. This mirrors how the focal method depends on a static/global counter.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <unity.h>
#include <mockMock.h>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


// The dependent counter used by the focal method to detect teardown.
// It is assumed to be defined in the production code (Defs.h / associated C file).
extern int CounterTeardown;

// The focal function under test (C linkage for compatibility with Unity).
void qwiktest_ThisTestPassesWhenNoTeardownRan(void);

#ifdef __cplusplus
}
#endif

// A single test that validates the focal function passes when no teardown has run.
// Domain-specific notes:
// - Sets the precondition CounterTeardown = 0 to reflect "no teardown ran" state.
// - Invokes the focal function which uses TEST_ASSERT_EQUAL_MESSAGE to enforce the condition.
// - If the assertion inside the focal function fails, Unity will record a test failure.
// - This test exercises the true-branch scenario of the condition predicate.
static void test_qwiktest_ThisTestPassesWhenNoTeardownRan_NoTeardown(void)
{
    // Precondition: ensure teardown counter is zero before invoking the focal method.
    CounterTeardown = 0;

    // Invoke the focal method. If CounterTeardown is not zero inside, the focal method
    // will fail the assertion, and this test will fail as a result.
    qwiktest_ThisTestPassesWhenNoTeardownRan();
}

// Optionally, you could add more tests to explore other preconditions (e.g., non-zero teardown)
// but such tests would require capturing Unity's failure state or isolating the call.
// For simplicity and safety in a single-test run, we focus on the canonical passing case.

int main(void)
{
    UNITY_BEGIN();

    // Run the primary test that ensures the "no teardown" condition passes.
    RUN_TEST(test_qwiktest_ThisTestPassesWhenNoTeardownRan_NoTeardown);

    // Finalize Unity test run and return status.
    return UNITY_END();
}