/*
 * Test suite for the parameterized focal method: test_TheseShouldAllPass(int Num)
 * 
 * Strategy:
 * - Adapt the C-style parameterized test to Unity (a C unit testing framework) by
 *   providing wrapper test functions with no parameters.
 * - Cover true and false branches of the predicate Num < 100 by exercising values
 *   below, at, and above the threshold.
 * - Invoke related class dependency tests (these should be declared extern) to
 *   ensure broader integration coverage as described in the provided dependencies.
 * - Keep tests non-terminating and rely on Unity to recover from assertion failures.
 * - Implement a main() that runs all wrapper tests, as gtest-style is not allowed.
 *
 * Candidate Keywords captured from the focal method:
 * - Num, 100, TEST_ASSERT_TRUE, predicate, branch coverage
 * - TheseShouldAllPass, TheseShouldAllFail, TheseAreEveryOther, NormalPassesStillWork,
 *   NormalFailsStillWork
 *
 * Note: This file is intended to be compiled as C++11 but exercises C-style Unity tests.
 */

#include <setjmp.h>
#include <unity.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
// External declarations for class dependencies used by the focal method.
// These are provided by the focal file and should exist in the project.
void test_TheseShouldAllPass(int Num);
void test_TheseShouldAllFail(int Num);
void test_TheseAreEveryOther(int Num);
void test_NormalPassesStillWork(void);
void test_NormalFailsStillWork(void);
#ifdef __cplusplus
}
#endif

// Wrapper tests to adapt parameterized functions to Unity's void(void) signature.

/*
 * These tests exercise the true branch of the predicate Num < 100
 * by invoking test_TheseShouldAllPass with Num values strictly below 100.
 */

// Below threshold: 0
static void test_AllPass_case0(void) {
    // Rationale: Confirms pass path for a clearly true predicate.
    test_TheseShouldAllPass(0);
}

// Below threshold: 50
static void test_AllPass_case50(void) {
    // Rationale: Confirms pass path for another true-case value.
    test_TheseShouldAllPass(50);
}

// Below threshold: 99
static void test_AllPass_case99(void) {
    // Rationale: Edge of the true branch; 99 < 100 should pass.
    test_TheseShouldAllPass(99);
}

/*
 * These tests exercise the false branch of the predicate Num < 100
 * by invoking test_TheseShouldAllPass with Num values >= 100.
 * These cases are intended to verify that the assertion is triggered by the focal method.
 */

// Above or at the threshold: 100
static void test_AllPass_case100(void) {
    // Rationale: Triggers the false branch of the predicate (100 < 100 is false).
    test_TheseShouldAllPass(100);
}

/*
 * Additional tests for the related dependency that may also be part of the scenario.
 * These help ensure broader coverage in conjunction with the focal method.
 */

// TheseShouldAllFail: attempt to provoke failure scenarios via the dependent test
static void test_TheseShouldAllFail_case0(void) {
    test_TheseShouldAllFail(0);
}

static void test_TheseShouldAllFail_case150(void) {
    test_TheseShouldAllFail(150);
}

/*
 * TheseAreEveryOther: test coverage for alternate-case behavior
 * by exercising different Num values.
 */

// Odd case 1
static void test_EveryOther_case1(void) {
    test_TheseAreEveryOther(1);
}

// Even case 2
static void test_EveryOther_case2(void) {
    test_TheseAreEveryOther(2);
}

/*
 * Normal passes/fails suite to ensure related functionality remains intact.
 */

// Normal passes path
static void test_NormalPassesStillWork_wrapper(void) {
    test_NormalPassesStillWork();
}

// Normal fails path
static void test_NormalFailsStillWork_wrapper(void) {
    test_NormalFailsStillWork();
}

/*
 * Unity test runner entry point
 * Executes all defined wrapper tests to achieve parameterized test coverage.
 */
int main(void) {
    UNITY_BEGIN();

    // True-branch coverage for Num < 100
    RUN_TEST(test_AllPass_case0);
    RUN_TEST(test_AllPass_case50);
    RUN_TEST(test_AllPass_case99);

    // False-branch coverage for Num < 100
    RUN_TEST(test_AllPass_case100);

    // Additional coverage for dependent tests (TheseShouldAllFail)
    RUN_TEST(test_TheseShouldAllFail_case0);
    RUN_TEST(test_TheseShouldAllFail_case150);

    // Branch/alternate scenario coverage (EveryOther)
    RUN_TEST(test_EveryOther_case1);
    RUN_TEST(test_EveryOther_case2);

    // Normal passes/fails integration checks
    RUN_TEST(test_NormalPassesStillWork_wrapper);
    RUN_TEST(test_NormalFailsStillWork_wrapper);

    return UNITY_END();
}