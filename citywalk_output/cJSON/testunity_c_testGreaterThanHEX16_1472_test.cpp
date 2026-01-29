// Test suite for the focal method: testGreaterThanHEX16
// This suite is written in C++11 compatible style but uses the Unity C test framework.
// It focuses on exercising the HEX16 "greater than" assertion macro in both false and true paths.
// The tests are designed to be non-GMock and rely on Unity's non-terminating assertions
// where appropriate, matching the project's constraints.

// Notes on dependencies and design decisions (mapped from the provided focus).
// - The CORE dependency here is the Unity testing framework (unity.h) and its macros like
//   TEST_ASSERT_GREATER_THAN_HEX16, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END, etc.
// - We simulate both failing and passing scenarios for TEST_ASSERT_GREATER_THAN_HEX16 by:
//     a) invoking the focal function's code path that should fail the first assertion, and
//     b) executing explicit passing cases that should succeed the macro.
// - Static helpers from the Unity harness (startPutcharSpy, endPutcharSpy, etc.) are expected
//   to be provided by the existing test environment; we rely on Unity's conventions to
//   initialize/tear down correctly via UnityBegin/UnityEnd in main.

// Include Unity header in a way compatible with C++11 (extern "C" wrapper to prevent name mangling).
extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

}

// Since this file is compiled as C++, ensure the Unity test runner is initialized correctly.
// The tests below intentionally cover both the failing (false) and passing (true) scenarios
// of the HEX16 greater-than assertion macro.


// Test: Validate that the focal method's first assertion (v0 > v1) fails as written.
// This mirrors the actual focal function's behavior where v0 = 0x0000 and v1 = 0xFFFF.
void testGreaterThanHEX16_FalsePaths(void)
{
    // Arrange values as in the focal method
    UNITY_UINT16 v0 = 0x0000;
    UNITY_UINT16 v1 = 0xFFFF;
    UNITY_UINT16 *p0 = &v0;
    UNITY_UINT16 *p1 = &v1;

    // The focal method uses four evaluations which are all false for the given values.
    // We wrap the first assertion in EXPECT_ABORT_BEGIN to verify that the code aborts
    // as Unity would when a TEST_ASSERT_GREATER_THAN_HEX16 fails.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_HEX16(v0, v1);
    VERIFY_FAILS_END
}

// Test: Validate that TEST_ASSERT_GREATER_THAN_HEX16 can succeed (true path) when input is ordered correctly.
// This exercises the "true" branch(s) of the macro to ensure coverage across both outcomes.
void testGreaterThanHEX16_PassCases(void)
{
    // Arrange values such that the macro evaluates to true in all four forms used by the focal test.
    UNITY_UINT16 a = 0xFFFF; // max
    UNITY_UINT16 b = 0x0000; // min
    UNITY_UINT16 *pa = &a;
    UNITY_UINT16 *pb = &b;

    // All of these should pass without abort since a > b
    TEST_ASSERT_GREATER_THAN_HEX16(a, b);
    TEST_ASSERT_GREATER_THAN_HEX16(*pa, b);
    TEST_ASSERT_GREATER_THAN_HEX16(a, *pb);
    TEST_ASSERT_GREATER_THAN_HEX16(*pa, *pb);
}

// Minimal main to run the suite using Unity's test runner.
// This avoids any external test framework and adheres to a simple C++11 main pattern.
int main(void)
{
    // Initialize Unity test framework
    UnityBegin("testGreaterThanHEX16_suite.cpp");

    // Run tests
    RUN_TEST(testGreaterThanHEX16_FalsePaths);
    RUN_TEST(testGreaterThanHEX16_PassCases);

    // Finish and return result
    return UnityEnd();
}