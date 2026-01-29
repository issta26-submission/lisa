// testRunnerGenerator_tests.cpp
// A focused test suite for the focal method: qwiktest_ThisTestPassesWhenNoTeardownRan
// This suite uses the Unity C framework from within a C++11 test file (no gtest).
// It manipulates the external CounterTeardown state and invokes the focal method.
// The tests include explanatory comments for clarity and maintainability.

#include <unity.h>
#include <iostream>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


extern "C" {
    // Include Unity framework and dependencies with C linkage
    #include "unity.h"
    #include "Defs.h" // Provides CounterTeardown (extern int CounterTeardown;)
    // Declaration of the focal method under test (C linkage)
    void qwiktest_ThisTestPassesWhenNoTeardownRan(void);
    // Expose the external counter so tests can control its value
    extern int CounterTeardown;
}

// Test 1: Verify that the focal test passes when no teardown has run (CounterTeardown == 0)
// This directly exercises the intended path of the focal method.
void test_qwiktest_NoTeardownRan_PassesWhenTeardownZero(void)
{
    // Arrange: Ensure the teardown counter indicates 'no teardown'
    CounterTeardown = 0;

    // Act: Run the focal method under test
    qwiktest_ThisTestPassesWhenNoTeardownRan();

    // Assert: The focal method uses Unity's assertion to verify the condition.
    // After the call, CounterTeardown should remain 0 to indicate no teardown ran.
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, CounterTeardown, "CounterTeardown should remain 0 after test passes");
}

// Test 2: Idempotency check for the focal test under clean teardown state
// This ensures repeated invocations with the same clean state do not alter the outcome.
void test_qwiktest_NoTeardownRan_Idempotent(void)
{
    // Arrange: Start with no teardown
    CounterTeardown = 0;

    // Act: First invocation
    qwiktest_ThisTestPassesWhenNoTeardownRan();
    // Assert: First invocation should pass with CounterTeardown == 0
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, CounterTeardown, "First invocation: CounterTeardown should be 0");

    // Act: Second invocation with the same clean state
    CounterTeardown = 0; // Re-assert clean state before second call
    qwiktest_ThisTestPassesWhenNoTeardownRan();

    // Assert: Second invocation should also pass without modifying CounterTeardown
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, CounterTeardown, "Second invocation: CounterTeardown should remain 0");
}

// Minimal Unity test runner compatible with C++11
int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Run the focused tests for the focal method
    RUN_TEST(test_qwiktest_NoTeardownRan_PassesWhenTeardownZero);
    RUN_TEST(test_qwiktest_NoTeardownRan_Idempotent);

    // End Unity test run and return result
    return UNITY_END();
}