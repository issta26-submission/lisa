// Focal Test Suite for: testNotEqualFloatArraysExpectedNull (located in testunity.c)
// This suite is crafted to exercise the focal method using the Unity C framework
// in a C++11 compatible translation unit. It also documents the key components
// involved in the test for easier maintenance and understanding.
//
// Candidate Keywords (Step 1 mapping to the focal method):
// - p0: NULL float pointer (expected NULL input)
// - p1: float array {1.0f, 8.0f, 25.4f, 0.252f} (non-NULL actual)
// - 4: length of the float array to compare
// - TEST_ASSERT_EQUAL_FLOAT_ARRAY: comparison primitive used by the focal test
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: failure expectation scaffolding
// - UNITY_EXCLUDE_FLOAT: compile-time switch to exclude floating-point tests
//
// Step 2 and 3: We build two test entry points within a single runner:
// 1) The original focal test as declared externally in testunity.c
// 2) An inline duplicate to increase coverage of the same code path (not required
//    for correctness but aids coverage when the focal test is recompiled with
//    different macro configurations in separate builds).

#include <stddef.h>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

extern "C" {
}

// Forward declaration of the focal test from testunity.c
// This will link against the existing implementation which exercises the exact scenario.
extern void testNotEqualFloatArraysExpectedNull(void);

// Inline duplicate test implementing the same scenario to reinforce coverage.
// This mirrors the focal test's behavior without altering its semantics.
static void testNotEqualFloatArraysExpectedNull_Duplicate(void)
{
#ifndef UNITY_EXCLUDE_FLOAT
    float* p0 = NULL;
    float p1[] = {1.0f, 8.0f, 25.4f, 0.252f};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#else
    // If floating-point tests are excluded, mark this test as ignored gracefully.
    TEST_IGNORE();
#endif
}

// Minimal additional test to exercise the runner in a separate translation unit.
// It directly invokes the inline duplicate to ensure both test paths can be exercised
// when compiled together with or without UNITY_EXCLUDE_FLOAT.
static void testInlineDuplicateRegistration(void)
{
    testNotEqualFloatArraysExpectedNull_Duplicate();
}

int main(void)
{
    UNITY_BEGIN("TestNotEqualFloatArraysExpectedNull Suite (with focal method and inline coverage)");
    // Run the focal test from testunity.c
    RUN_TEST(testNotEqualFloatArraysExpectedNull);
    // Run the inline duplicate coverage test
    RUN_TEST(testNotEqualFloatArraysExpectedNull_Duplicate);
    // Optional: run a wrapper to ensure the inline coverage path is exercised
    RUN_TEST(testInlineDuplicateRegistration);
    return UNITY_END();
}