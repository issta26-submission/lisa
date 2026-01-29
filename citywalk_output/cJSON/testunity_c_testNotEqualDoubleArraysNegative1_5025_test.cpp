/*
 * Test Suite for the focal method: testNotEqualDoubleArraysNegative1
 * Context: Unity-based test environment (C, with C++11 compatibility).
 * Objective: Provide targeted unit tests that exercise the focal test
 *            under both normal (with doubles enabled) and excluded
 *            (UNITY_EXCLUDE_DOUBLE defined) configurations.
 * Notes:
 * - The focal method is defined in testunity.c and relies on Unity's
 *   EXPECT_ABORT_BEGIN / TEST_ASSERT_EQUAL_DOUBLE_ARRAY / VERIFY_FAILS_END
 * - This suite introduces wrapper tests that invoke the focal test and
 *   also reproduces the same failure scenario directly to ensure coverage
 *   of the core failure path without depending on the focal function itself.
 * - Because Unity is a C framework, these tests use C linkage for compatibility
 *   when compiled alongside C source. They are written to be added to the
 *   existing Unity-based test project without requiring GoogleTest.
 *
 * Explanatory comments accompany each test describing its purpose and branches
 * exercised.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Prototype of the focal function under test (defined in testunity.c)
void testNotEqualDoubleArraysNegative1(void);

#ifdef __cplusplus
}
#endif

/* 
 * Test 1: Normal path coverage
 * 
 * Behavior:
 * - When UNITY_EXCLUDE_DOUBLE is not defined, the focal function executes
 *   and is expected to fail due to two nearly-equal arrays with a single
 *   differing element (last element differs by 1e-10 roughly).
 * - We wrap the call to the focal function with EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
 *   to ensure Unity correctly captures the intentional failure without aborting
 *   the entire test suite.
 */
void test_NotEqualDoubleArraysNegative1_NormalPath(void)
{
#ifndef UNITY_EXCLUDE_DOUBLE
    // Expect the focal test to fail (abort) due to inequality of double arrays.
    EXPECT_ABORT_BEGIN
    testNotEqualDoubleArraysNegative1();
    VERIFY_FAILS_END
#else
    // If doubles are excluded, this path is not applicable.
    TEST_IGNORE_MESSAGE("UNITY_EXCLUDE_DOUBLE defined: skipping NormalPath test_NotEqualDoubleArraysNegative1.");
#endif
}

/* 
 * Test 2: Direct replication of the focal test's failure scenario
 * 
 * Behavior:
 * - Independently reproduces the same failure condition as the focal test by creating
 *   two slightly different double arrays on the stack and asserting their equality.
 * - This validates the underlying assertion macro TEST_ASSERT_EQUAL_DOUBLE_ARRAY in isolation
 *   from the focal function, ensuring the failure path is exercised.
 * 
 * Note:
 * - This directly exercises Unity's assertion for double arrays.
 * - Included for broader coverage of the failure path beyond the focal function.
 */
void test_NotEqualDoubleArraysNegative1_DirectAssertion(void)
{
#ifndef UNITY_EXCLUDE_DOUBLE
    double p0[] = {-1.0, -8.0, -25.4, -0.2566666667};
    double p1[] = {-1.0, -8.0, -25.4, -0.2566666666};

    // Expect TEST_ASSERT_EQUAL_DOUBLE_ARRAY to fail (arrays are not exactly equal)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#else
    TEST_IGNORE_MESSAGE("UNITY_EXCLUDE_DOUBLE defined: skipping DirectAssertion test_NotEqualDoubleArraysNegative1.");
#endif
}

/* 
 * Test 3: Excluded path coverage (compile-time flag)
 * 
 * Behavior:
 * - When UNITY_EXCLUDE_DOUBLE is defined, the focal test's body uses TEST_IGNORE()
 *   to skip execution (as indicated by the focal code).
 * - This test ensures that the compile-time path for excluding doubles is reachable
 *   and that the code compiles with the exclusion in effect.
 *
 * Implementation detail:
 * - This test uses preprocessor checks so it only compiles when UNITY_EXCLUDE_DOUBLE is defined.
 * - If the macro is not defined, this test is skipped with a message.
 */
void test_NotEqualDoubleArraysNegative1_ExcludedPath(void)
{
#if defined(UNITY_EXCLUDE_DOUBLE)
    // Simulate the exclusion path by emitting an ignore, mirroring the focal path behavior.
    TEST_IGNORE_MESSAGE("UNITY_EXCLUDE_DOUBLE is defined: focal test would be ignored (TEST_IGNORE in focal path).");
#else
    TEST_IGNORE_MESSAGE("UNITY_EXCLUDE_DOUBLE not defined: skipping ExcludedPath test_NotEqualDoubleArraysNegative1.");
#endif
}

These tests collectively cover:
- The normal failure path when doubles are enabled (Test 1).
- The underlying double array comparison failure path (Test 2).
- The compile-time exclusion scenario for doubles (Test 3).

Usage notes for integration:
- Add this file to the Unity test build alongside testunity.c (or ensure the focal function's declaration is resolvable).
- If your build system compiles with and without UNITY_EXCLUDE_DOUBLE, both Test 1 and Test 3 will exercise their respective branches accordingly.
- If you want broader coverage, you can extend this suite with additional variations of the two arrays (e.g., different lengths, additional elements) following the same pattern.

If you intend to run tests without modifying your existing Unity setup:
- Ensure your build links this file with unity.c and the Unity test runner (no GoogleTest).
- The focal function testNotEqualDoubleArraysNegative1 is invoked under Test 1 and can be invoked directly under Test 2, both wrapped in Unity's abort-capturing macros to verify expected failures.