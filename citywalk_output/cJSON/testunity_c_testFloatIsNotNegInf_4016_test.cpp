// test_float_not_neg_inf_suite.cpp
// A focused test-suite for the focal method's behavior around negative infinity checks.
// This suite uses the Unity framework (C) but is built to compile under C++11 as requested.
// It covers:
// - The non-NEG_INF path (expected abort when a finite value is tested as NEG_INF).
// - The NEG_INF path (expected to pass when a true NEG_INF is tested).
// - The UNITY_EXCLUDE_FLOAT branch (ensures the test would be ignored when float tests are excluded).
// Note: The focal method testFloatIsNotNegInf is defined in testunity.c. This suite tests the
// underlying predicates/macros it relies on (e.g., TEST_ASSERT_FLOAT_IS_NEG_INF) in isolation.

#include <unity.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (Step 1): UNITY_FLOAT, UNITY_EXCLUDE_FLOAT, TEST_ASSERT_FLOAT_IS_NEG_INF,
// EXPECT_ABORT_BEGIN, VERIFY_FAILS_END, TEST_IGNORE, TEST_IGNORE_MESSAGE, INFINITY
// These keywords represent the core dependent components of the focal method's logic.


// Test 1: Non-NEG_INF path should abort when a finite value is tested as NEG_INF.
// This mirrors the focal method's behavior where -999.876f is not NEG_INF, so the assertion should fail.
void testFloatNotNegInf_FailsOnFiniteValue(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    // When float tests are excluded, the focal test should be ignored/skipped.
    // This exercises the UNITY_EXCLUDE_FLOAT branch.
    TEST_IGNORE();
#else
    // Expect an abort because TEST_ASSERT_FLOAT_IS_NEG_INF(-999.876f) should fail.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_FLOAT_IS_NEG_INF(-999.876f);
    VERIFY_FAILS_END
#endif
}

// Test 2: NEG_INF path should pass when the value is actually negative infinity.
// This exercises the true-case path for the NEG_INF predicate, ensuring coverage of the
// non-abort path when the correct NEG_INF value is provided.
void testFloatNotNegInf_PassesOnNegInf(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    // If floats are excluded, skip this test branch.
    TEST_IGNORE_MESSAGE("FLOAT tests excluded; skipping NEG_INF positive path.");
#else
    // Cast to float to be explicit about type in C/C++ environments.
    float negInfF = -INFINITY;
    TEST_ASSERT_FLOAT_IS_NEG_INF(negInfF); // This should pass (no abort) when not excluded.
#endif
}

// Test 3: Exclusion branch test (compiles to skip when UNITY_EXCLUDE_FLOAT is defined).
// This ensures the suite covers the ignore-path explicitly.
void testFloatNotNegInf_ExcludeBranch(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // If not excluded, this test is trivial and passes.
    TEST_ASSERT_TRUE(1);
#endif
}

// Minimal setup/teardown (required by Unity when linking with Unity-based runners)
void setUp(void)   { /* optional: initialize per-test state if needed */ }
void tearDown(void) { /* optional: cleanup per-test state if needed */ }

// Main function to drive the Unity-based test suite (Step 3 guidance: call tests from main)
int main(void)
{
    UNITY_BEGIN("test_float_not_neg_inf_suite.cpp");

    // Register tests. In this framework, RUN_TEST is used to invoke each test.
    RUN_TEST(testFloatNotNegInf_FailsOnFiniteValue);
    RUN_TEST(testFloatNotNegInf_PassesOnNegInf);
    RUN_TEST(testFloatNotNegInf_ExcludeBranch);

    return UNITY_END();
}