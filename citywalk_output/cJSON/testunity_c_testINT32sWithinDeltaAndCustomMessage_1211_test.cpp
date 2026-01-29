// test_focal_INT32WithinDeltaAndCustomMessage.cpp
// A focused C++11 test suite exercising the focal Unity test method
// testINT32sWithinDeltaAndCustomMessage() defined in testunity.c

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Forward declaration of the focal test function defined in testunity.c
// The function calls TEST_ASSERT_INT32_WITHIN_MESSAGE(1, 5000, 5001, "Custom Message.")
extern "C" void testINT32sWithinDeltaAndCustomMessage(void);

// Internal counter to demonstrate that the wrapper test executed
static int g_wrapperExecutionCount = 0;

// Set up/tear down stubs (Unity-compatible)
void setUp(void)
{
    // No shared state required for these tests; placeholder for completeness
}

void tearDown(void)
{
    // Ensure counter does not indicate leakage between tests
    // (No action required, but present to mirror Unity-style tests)
}

// Wrapper test that directly invokes the focal test function.
//
// Rationale:
// - This ensures the exact focal path (within delta with a custom message)
//   is executed under the Unity harness.
// - The test will pass if the focal function's assertion passes.
// - If the focal assertion fails, Unity will mark this test as failed.
// - This wrapper is minimal and keeps the dependency on the focal function explicit.
void test_RunFocal_INT32WithinDeltaAndCustomMessage(void)
{
    testINT32sWithinDeltaAndCustomMessage();
    g_wrapperExecutionCount++;
    // Basic assertion to ensure the wrapper executed the focal path at least once
    TEST_ASSERT_EQUAL_INT(1, g_wrapperExecutionCount);
}

// Direct usage of the underlying Unity macro without going through the focal function.
// This helps exercise the true-path branch of a similar assertion macro and
// increases coverage of the assertion machinery (values chosen to pass).
void testDirectMacro_INT32WithinDelta_Pass(void)
{
    // This should pass: absolute difference is 1, delta is 1
    TEST_ASSERT_INT32_WITHIN(1, 5000, 5001);
}

// Direct usage of the intermediate macro with a boundary-case that still passes.
// Demonstrates the branch where the delta exactly matches the observed difference.
void testDirectMacro_INT32WithinDelta_BoundaryZero(void)
{
    // This should pass: delta is 0 and both values are identical
    TEST_ASSERT_INT32_WITHIN(0, -12345, -12345);
}

// Main runner for the test suite.
// NOTE: Each RUN_TEST registers a test with the Unity framework.
// The focal test is invoked via a wrapper to keep test organization clear.
int main(void)
{
    UnityBegin("test_focal_INT32WithinDeltaAndCustomMessage.cpp");

    // Run the focal method wrapper (exercises the focal test function itself)
    RUN_TEST(test_RunFocal_INT32WithinDeltaAndCustomMessage);

    // Additional direct macro-based tests to broaden coverage of related code
    RUN_TEST(testDirectMacro_INT32WithinDelta_Pass);
    RUN_TEST(testDirectMacro_INT32WithinDelta_BoundaryZero);

    return UnityEnd();
}