// test_NotEqualHEX8EachEqual2_suite.cpp
// A focused unit test suite for the focal method equivalent to testNotEqualHEX8EachEqual2
// in testunity.c. This suite is written for C++11 compatibility and uses the Unity
// framework (no GoogleTest). It exercises both failure and success paths of
// TEST_ASSERT_EACH_EQUAL_HEX8 with a 4-element unsigned char array.
//
// Notes on dependencies and keywords:
// - Focal macros used: EXPECT_ABORT_BEGIN, VERIFY_FAILS_END, TEST_ASSERT_EACH_EQUAL_HEX8
// - Dependency macros/functions are defined in the Unity-based test harness (extern "C")
// - We provide lightweight stubs for the spy/output-related hooks to satisfy the harness.

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


// Bring in Unity C API with C linkage to preserve symbol names when compiling as C++
extern "C" {
}

// Candidate Keywords extracted from the focal method and its usage
// - TEST_ASSERT_EACH_EQUAL_HEX8
// - 254u (literal value used in the test)
// - p0 (array under test)
// - 4 (length of the array)

static int SetToOneToFailInTearDown;
static int SetToOneMeanWeAlreadyCheckedThisGuy;

// Lightweight setup/teardown to satisfy Unity's expectations.
// These are minimal and do not affect the test outcomes beyond isolation.
void setUp(void)
{
    SetToOneToFailInTearDown = 0;
    SetToOneMeanWeAlreadyCheckedThisGuy = 0;
}

void tearDown(void)
{
    // No tear-down side effects needed for these focused tests.
}

// Spy hooks required by the EXPECT_ABORT_BEGIN / VERIFY_FAILS_END macros.
// The real project defines these to intercept puts/prints during abort scenarios.
// Here we provide no-op implementations to enable test execution in CI.
void startPutcharSpy(void) { /* no-op for testing environment */ }
void endPutcharSpy(void) { /* no-op for testing environment */ }
char* getBufferPutcharSpy(void) { return NULL; }

// =======================================================
// Test 1: Focal test reproduction (failure path)
// This mirrors the original focal testNotEqualHEX8EachEqual2(), which asserts
// that not all four elements of p0 are equal to 254. Since p0 is
// {254, 254, 254, 253}, the assertion should fail and abort the test.
// =======================================================
void testNotEqualHEX8EachEqual2_Cpp(void)
{
    unsigned char p0[] = {254u, 254u, 254u, 253u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX8(254u, p0, 4);
    VERIFY_FAILS_END
}

// =======================================================
// Test 2: Positive path (should pass when all values are equal to 254)
// This validates that the assertion passes when the condition is true
// for all elements in the array.
// =======================================================
void testNotEqualHEX8EachEqual2_AllEqual_ShouldPass(void)
{
    unsigned char p0[] = {254u, 254u, 254u, 254u};
    TEST_ASSERT_EACH_EQUAL_HEX8(254u, p0, 4);
}

// =======================================================
// Test 3: Negative path with a single differing element (should fail)
// This ensures the macro detects any deviation from the expected value.
// =======================================================
void testNotEqualHEX8EachEqual2_MismatchShouldFail(void)
{
    unsigned char p0[] = {254u, 255u, 254u, 254u};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_HEX8(254u, p0, 4);
    VERIFY_FAILS_END
}

// =======================================================
// Test Runner
// Runs the three tests above. Each test is self-contained and uses
// Unity's non-GTest style to satisfy the "no GTest" requirement.
// =======================================================
int main(void)
{
    // Begin a Unity test run. The string is the test suite name.
    UnityBegin("test_NotEqualHEX8EachEqual2_suite");

    // Register tests
    RUN_TEST(testNotEqualHEX8EachEqual2_Cpp);
    RUN_TEST(testNotEqualHEX8EachEqual2_AllEqual_ShouldPass);
    RUN_TEST(testNotEqualHEX8EachEqual2_MismatchShouldFail);

    // End the run and return the result
    return UnityEnd();
}

// The end of the test file. This file intentionally uses Unity's C API from within
// a C++ translation unit. The extern "C" linkage for the Unity header ensures
// correct symbol resolution during linking.