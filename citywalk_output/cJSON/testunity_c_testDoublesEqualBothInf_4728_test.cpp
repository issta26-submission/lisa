/*
Step 1: Program Understanding and Candidate Keywords
- Focal method: testDoublesEqualBothInf in testunity.c
- Core behavior: Verifies that 1.0 / d_zero equals itself when d_zero is 0.0, i.e., testing Inf handling for doubles.
- Relevant constants/macros from dependencies:
  - UNITY_DOUBLE d_zero (0.0) used to generate Inf
  - UNITY_EXCLUDE_DOUBLE guard to optionally exclude double tests
  - TEST_ASSERT_EQUAL_DOUBLE macro used to compare doubles
  - setUp/tearDown lifecycle for Unity tests
- Candidate Keywords: testDoublesEqualBothInf, UNITY_EXCLUDE_DOUBLE, d_zero, TEST_ASSERT_EQUAL_DOUBLE, Inf, 1.0/0.0, UnityBegin, RUN_TEST, TEST_IGNORE

Step 2: Unit Test Generation
- We create a small Unity-based test harness in C++11 that can execute the focal test (testDoublesEqualBothInf) via RUN_TEST.
- Additionally, to strengthen coverage around the double-INF behavior without modifying testunity.c, we add a supplementary direct test (testInfinityEqualityDirect) that independently constructs an Inf via 1.0/0.0 and asserts its equality with itself using TEST_ASSERT_EQUAL_DOUBLE.
- This approach ensures the essential Inf-handling path for doubles is exercised, even if UNITY_EXCLUDE_DOUBLE is not toggled at runtime (the focal test path itself is compiled with that guard).

Step 3: Test Case Refinement
- The added test case (testInfinityEqualityDirect) is designed to maximize coverage by exercising Inf generation and equality without relying on internal static state.
- The harness uses the standard Unity test workflow (UnityBegin, RUN_TEST, UnityEnd) to maintain consistency with existing Unity-based tests and to ensure correct setup/teardown semantics.
- The tests are annotated with comments explaining intent and coverage focus.

Code (C++11 compatible test harness invoking the focal test and an additional direct Inf-equality test)
*/

// Compile-time note: This harness assumes Unity is available in the build and that testunity.c provides
// the focal testDoublesEqualBothInf function. The direct test uses IEEE-compliant Inf behavior.

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

#ifdef __cplusplus
}
#endif

// Forward declare the focal test from testunity.c (C linkage)
extern "C" void testDoublesEqualBothInf(void);

// Optional: expose setUp/tearDown if RUN_TEST requires them to be present
#ifdef __cplusplus
extern "C" {
#endif
void setUp(void);
void tearDown(void);
#ifdef __cplusplus
}
#endif

// Additional direct test to independently verify Inf equality for doubles.
// This supplements the focal test by constructing Inf directly (1.0/0.0) and asserting equality.
static void testInfinityEqualityDirect(void)
{
#ifndef UNITY_EXCLUDE_DOUBLE
    // Generate positive infinity by dividing a non-zero double by zero
    double posInfA = 1.0 / 0.0;
    double posInfB = 1.0 / 0.0;

    // Ensure two Inf results are considered equal by the Unity assertion
    TEST_ASSERT_EQUAL_DOUBLE(posInfA, posInfB);
#else
    // If doubles are excluded, skip this test gracefully.
    TEST_IGNORE();
#endif
}

// Entry point: harness to run focal test and the supplementary direct Inf equality test
int main(void)
{
    // Initialize the Unity test framework
    UnityBegin("test_doubles_equal_both_inf_suite");

    // Run the focal test from testunity.c
    // This test exercises the exact code path: TEST_ASSERT_EQUAL_DOUBLE(1.0 / d_zero, 1.0 / d_zero)
    RUN_TEST(testDoublesEqualBothInf);

    // Run an independent Inf-equality test to strengthen coverage for double-Inf behavior
    RUN_TEST(testInfinityEqualityDirect);

    // Complete the test run and return the exit status
    return UnityEnd();
}