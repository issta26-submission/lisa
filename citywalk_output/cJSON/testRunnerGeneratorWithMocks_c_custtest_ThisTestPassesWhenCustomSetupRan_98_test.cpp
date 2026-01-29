// test_suite_custtest.cpp
// This test suite targets the focal method:
// custtest_ThisTestPassesWhenCustomSetupRan
// which asserts that CounterSetup equals 2, i.e., that the custom setup ran.
// The suite is designed to be compiled with C++11 and Unity (no GTest).

// Expose C symbols from the focal codebase to C++
extern "C" {
#include <stddef.h>
#include <unity.h>
#include <mockMock.h>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


// Focal function under test (provided by the codebase)
void custtest_ThisTestPassesWhenCustomSetupRan(void);

// Externally defined counter that the focal test relies on
// (Assumed to be accessible with C linkage from the linked translation unit)
extern int CounterSetup;
}

// ---------------------------------------------------------------------------
// Step 1: Candidate Keywords (from the focal method and its dependencies)
// - CounterSetup: the global/static counter checked by the focal test
// - TEST_ASSERT_EQUAL_MESSAGE: assertion used by the focal test
// - "Custom Setup Wasn't Run": the error message used in the assertion
// - custom_setup: potential hook that increments/setup counter
// - custtest_ThisTestPassesWhenCustomSetupRan: focal test under scrutiny
// ---------------------------------------------------------------------------

// Test suite using Unity's RUN_TEST mechanism (gives structured test execution)
// If desired, tests can be expanded to cover more scenarios around CounterSetup.


// Test 1: Verify that when CounterSetup is 2, the focal test passes and does not modify CounterSetup unexpectedly.
// This directly exercises the true branch of the focal assertion.
void test_Custtest_PassesWhenCustomSetupRan_CounterSetupIsTwo(void)
{
    // Arrange: simulate that the custom setup has run by setting CounterSetup to 2
    CounterSetup = 2;

    // Act: invoke the focal test function
    custtest_ThisTestPassesWhenCustomSetupRan();

    // Assert: ensure that CounterSetup remains 2 after the focal test invocation
    // This provides an additional non-intrusive check beyond the focal assertion itself.
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, CounterSetup, "CounterSetup changed after custtest_ThisTestPassesWhenCustomSetupRan");
}

// Test 2: Verify that calling the focal test multiple times with CounterSetup == 2 remains stable.
// This helps exercise repeated execution paths and confirms no unintended side effects.
// Note: The focal test itself would pass on each invocation when CounterSetup == 2.
void test_Custtest_PassesWhenCustomSetupRan_MultipleConsecutiveCalls(void)
{
    // Arrange: ensure the precondition
    CounterSetup = 2;

    // Act: perform the focal test twice
    custtest_ThisTestPassesWhenCustomSetupRan();
    custtest_ThisTestPassesWhenCustomSetupRan();

    // Assert: CounterSetup should still be 2 after two consecutive runs
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, CounterSetup, "CounterSetup changed after consecutive calls to custtest_ThisTestPassesWhenCustomSetupRan");
}

// Optional: a simple smoke test to ensure the test environment compiles and runs.
// This test does not assert on behavior of the focal method, but confirms the test harness is wired.
void test_Smoke_TestEnvironment(void)
{
    // No setup required; this test ensures the harness can execute at least one test.
    TEST_ASSERT_TRUE_MESSAGE(1, "Smoke test placeholder should always pass (1 == 1)");
}

// ---------------------------------------------------------------------------
// Minimal Unity test runner (since gtest is not allowed per domain knowledge).
// We explicitly define a main to run the tests from this file.
// ---------------------------------------------------------------------------

int main(void)
{
    // Begin a Unity test run
    UnityBegin("test_suite_custtest");

    // Register and run tests
    RUN_TEST(test_Custtest_PassesWhenCustomSetupRan_CounterSetupIsTwo);
    RUN_TEST(test_Custtest_PassesWhenCustomSetupRan_MultipleConsecutiveCalls);
    RUN_TEST(test_Smoke_TestEnvironment);

    // End the Unity test run and return appropriate status
    return UnityEnd();
}