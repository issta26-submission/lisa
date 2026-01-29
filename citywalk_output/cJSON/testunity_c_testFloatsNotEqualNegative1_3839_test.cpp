// Test Suite for the focal method: testFloatsNotEqualNegative1
// This suite targets the focal unit test located in testunity.c.
// It verifies that two different floating point values are treated as not equal
// and that Unity's abort/fail handling is exercised correctly.
// The test follows the Unity framework conventions (Unity_BEGIN/UNITY_END, RUN_TEST).

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// Since testunity.c defines the focal test with C linkage, declare it with C linkage for C++ compilation.
extern "C" void testFloatsNotEqualNegative1(void);

// Candidate Keywords captured from the focal method and its dependencies
// - UNITY_EXCLUDE_FLOAT
// - TEST_IGNORE
// - EXPECT_ABORT_BEGIN
// - TEST_ASSERT_EQUAL_FLOAT
// - VERIFY_FAILS_END
// - TEST_PROTECT
// - startPutcharSpy / endPutcharSpy (utility hooks used by Unity macros)
// - Unity (global state, CurrentTestFailed, CurrentTestLineNumber, etc.)
// - -9273.9649f, -9273.0049f (float literals used in the test)
// - TEST_FAIL_MESSAGE (used elsewhere in test suite for messaging)

// Main entry point for the test suite
int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Execute the focal test
    // This test specifically checks that two different floats are not equal
    // and that an assertion failure is properly detected (or ignored if FLOAT tests are excluded).
    RUN_TEST(testFloatsNotEqualNegative1);

    // Finalize Unity test run and return status
    return UNITY_END();
}