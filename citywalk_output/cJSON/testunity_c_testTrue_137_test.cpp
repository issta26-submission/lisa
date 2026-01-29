// Test runner for the focal method testTrue, designed to integrate Unity (C unit test framework)
// into a C++11 environment. This runner exercises the focal method as declared in testunity.c
// and demonstrates executing it from multiple test contexts.
// Note: The focal method uses Unity macros TEST_ASSERT and TEST_ASSERT_TRUE, which are defined in unity.h.

#include <unity.h>
#include <cstdio>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Expose Unity C API to C++ by using C linkage for the included Unity headers and focal method.
extern "C" {

// Focal method under test located in testunity.c
void testTrue(void);
}

// Additional wrapper tests to exercise the focal method from separate test contexts.
// These wrappers ensure testTrue can be invoked not only as a standalone test but also
// when called from another Unity test function. This helps validate isolation and re-entry.

static void testWrapper_testTrue(void)
{
    // This wrapper invokes the focal method to ensure it behaves correctly when called
    // from within another Unity test. It should pass because testTrue asserts true.
    testTrue();

    // Non-intrusive, non-fatal assertion to ensure the wrapper test itself remains verifiable.
    // We use a simple pass-through assertion to keep the test suite small and focused.
    TEST_ASSERT(1);
}

// A second wrapper that calls the focal method multiple times to increase execution paths.
// This simulates repeated invocations of the focal method within a single test context.
static void testWrapper_testTrueTwice(void)
{
    testTrue();
    testTrue();
    // Ensure wrapper test itself remains a passing test.
    TEST_ASSERT(1);
}

int main(void)
{
    // Initialize Unity test framework; the argument is the test file name for reporting.
    // We align with the focal class/dependency naming from the provided sources.
    UNITY_BEGIN("test_true_suite.cpp");

    // Run the focal method testTrue as defined in testunity.c.
    // This represents Step 2's primary test case: the true-branch validations of the focal method.
    // The focal method asserts that 1 is true, which should pass without incident.
    RUN_TEST(testTrue);

    // Run additional wrappers to exercise testTrue from alternate contexts.
    // This helps cover cross-context execution and ensures no unintended side effects.
    RUN_TEST(testWrapper_testTrue);

    // Run the second wrapper that calls testTrue twice to broaden execution coverage.
    RUN_TEST(testWrapper_testTrueTwice);

    // End Unity tests and return the overall result.
    return UNITY_END();
}