// This test suite targets the focal method: spec_ThisTestPassesWhenNormalSetupRan
// and its class dependencies as expressed in the provided focal code.
// The key dependency is CounterSetup and the normal setup path that should
// cause CounterSetup to be 1 when the focal test runs.


// Step 1 - Candidate Keywords extracted from the focal method and dependencies:
// - CounterSetup
// - Normal Setup
// - spec_ThisTestPassesWhenNormalSetupRan
// - TEST_ASSERT_EQUAL_MESSAGE
// - "Normal Setup Wasn't Run"
// - setUp / tearDown (Unity test lifecycle hooks)
// - UNITY / unity.h
// - Defs.h (project definitions)
// These keywords guide the test scenarios and assertions to cover the focal behavior.

#include <unity.h>
#include <CException.h>
#include <Defs.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal function under test (as declared in the C test runner file)
void spec_ThisTestPassesWhenNormalSetupRan(void);

// External dependency used by the focal test, expected to be manipulated by setup/teardown
extern int CounterSetup;
#ifdef __cplusplus
}
#endif

// Step 2: Test Suite for spec_ThisTestPassesWhenNormalSetupRan
// The goal is to verify that the normal setup path ran and set CounterSetup to 1
// before the focal test assertion is evaluated.

// Test 1: Verify that when the focal test is executed (which should trigger the normal setup),
// CounterSetup becomes 1, indicating that normal setup indeed ran prior to the assertion.
// This test uses Unity's RUN_TEST to execute the focal test within the same test harness.
void test_SpecRunsWithNormalSetup(void)
{
    // Precondition: ensure a clean slate before invoking the focal test
    CounterSetup = 0;

    // Run the focal test which internally relies on the normal setup to set CounterSetup to 1
    RUN_TEST(spec_ThisTestPassesWhenNormalSetupRan);

    // Assertion: the focal test expects CounterSetup == 1 when normal setup ran
    // If the focal test failed its assertion, RUN_TEST would mark this test as failed.
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, CounterSetup, "CounterSetup should be 1 after normal setup ran");
}

// Test 2: Repeat the focal test run to ensure stability across multiple invocations
// and that the normal setup consistently sets CounterSetup to 1 before the assertion.
void test_SpecRunsMultipleTimesWithNormalSetup(void)
{
    // First run
    CounterSetup = 0;
    RUN_TEST(spec_ThisTestPassesWhenNormalSetupRan);
    TEST_ASSERT_EQUAL_INT(1, CounterSetup);

    // Second run (should behave the same; Unity will reset per-test setup call)
    CounterSetup = 0;
    RUN_TEST(spec_ThisTestPassesWhenNormalSetupRan);
    TEST_ASSERT_EQUAL_INT(1, CounterSetup);
}

// Optional: a minimal sanity test to ensure the interface exists and can be invoked.
// This does not interfere with coverage goals but documents the dependency.
void test_SpecInterfaceAvailability(void)
{
    // No-op precondition: just ensure the symbol is linkable and callable
    TEST_ASSERT_NOT_NULL((void*)spec_ThisTestPassesWhenNormalSetupRan);
}

// Step 3: Test Case Refinement
// - The tests above exercise the focal method in-context via RUN_TEST to simulate
//   the normal Unity-driven test lifecycle (setUp is invoked prior to the focal test).
// - We isolate the behavior by resetting CounterSetup before each RUN_TEST invocation.
// - We purposely use non-terminating checks where possible; however, Unity's TEST_ASSERT
//   macros are used here to validate the focal behavior. If a failure occurs, Unity marks
//   that specific test as failed but does not crash the entire test suite due to RUN_TEST.
// - We avoid private methods/fields access and rely on the public interface (CounterSetup, spec_*).

int main(void)
{
    UNITY_BEGIN();

    // Execute tests that validate the focal method with normal setup
    RUN_TEST(test_SpecRunsWithNormalSetup);
    RUN_TEST(test_SpecRunsMultipleTimesWithNormalSetup);
    RUN_TEST(test_SpecInterfaceAvailability);

    return UNITY_END();
}