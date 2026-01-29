/*
Step 1 - Program Understanding (Candidate Keywords)
Candidates extracted from the focal method UnityAssertFloatsWithin and its dependencies:
- UnityAssertFloatsWithin
- UnityFloatsWithin
- UnityTestResultsFailBegin
- UNITY_PRINT_EXPECTED_AND_ACTUAL_FLOAT
- UnityAddMsgIfSpecified
- RETURN_IF_FAIL_OR_IGNORE (macro behavior)
- UNITY_FLOAT / UNITY_DOUBLE / UNITY_LINE_TYPE
- delta, expected, actual comparisons for floats
- msg (optional message)
These keywords guide the test cases to exercise the passing path, failing path, and edge conditions.

Step 2 - Unit Test Generation (Unity-based test harness in C++11, without GTest)
The tests exercise UnityAssertFloatsWithin by:
- Pass cases where actual is within delta of expected
- Fail cases where actual is outside delta
- Edge cases like exact equality with delta zero
- Large delta to ensure non-breaking passes
- Negative values to verify sign handling with delta

Step 3 - Test Case Refinement (Domain knowledge)
- Use a small delta for typical float tolerance
- Use zero delta for exact equality
- Use a large delta to ensure acceptance
- Ensure tests are non-terminating (no premature aborts)
- Use the Unity test runner (UnityBegin, RUN_TEST, UnityEnd)
- Provide explanatory comments for each test

Code below is a standalone C++11 test file that uses Unity's test framework to call the focal UnityAssertFloatsWithin function. It assumes unity.c/h are available in the project and linked accordingly.
*/

#include <stddef.h>
#include <cstdint>
#include <unity.h>


extern "C" {
}

/*
 Forward declaration is not strictly required if unity.h provides it, but we keep
 the explicit declaration to ensure linkage correctness across C++ files.
 The UnityAssertFloatsWithin function is defined in the C file unity.c.
*/
#ifdef __cplusplus
extern "C" {
#endif
void UnityAssertFloatsWithin(const UNITY_FLOAT delta,
                             const UNITY_FLOAT expected,
                             const UNITY_FLOAT actual,
                             const char* msg,
                             const UNITY_LINE_TYPE lineNumber);
#ifdef __cplusplus
}
#endif

// Test 1: Pass when actual is within a small delta of expected
static void test_UnityAssertFloatsWithin_PassWithinSmallDelta(void)
{
    // delta = 0.01, difference = 1.0000001 - 1.0 = 0.0000001 < delta
    UnityAssertFloatsWithin((UNITY_FLOAT)0.01f,
                            (UNITY_FLOAT)1.0f,
                            (UNITY_FLOAT)1.0000001f,
                            NULL,
                            __LINE__);
}

// Test 2: Fail when actual is outside the delta
static void test_UnityAssertFloatsWithin_FailOutsideDelta(void)
{
    // delta = 0.00001, difference = 10.0 vs 10.1 -> 0.1 > delta
    UnityAssertFloatsWithin((UNITY_FLOAT)0.00001f,
                            (UNITY_FLOAT)10.0f,
                            (UNITY_FLOAT)10.1f,
                            NULL,
                            __LINE__);
}

// Test 3: Pass exact equality with zero delta
static void test_UnityAssertFloatsWithin_PassExactEquality(void)
{
    // delta = 0.0, exact equality required
    UnityAssertFloatsWithin((UNITY_FLOAT)0.0f,
                            (UNITY_FLOAT)3.14159f,
                            (UNITY_FLOAT)3.14159f,
                            NULL,
                            __LINE__);
}

// Test 4: Pass with a large delta to ensure broad tolerance
static void test_UnityAssertFloatsWithin_PassWithLargeDelta(void)
{
    // delta = 1000.0, actual far from expected but within huge tolerance
    UnityAssertFloatsWithin((UNITY_FLOAT)1000.0f,
                            (UNITY_FLOAT)0.0f,
                            (UNITY_FLOAT)999.0f,
                            NULL,
                            __LINE__);
}

// Test 5: Pass with negative numbers and small delta
static void test_UnityAssertFloatsWithin_PassWithNegativeActual(void)
{
    // delta = 0.01, expected -2.50, actual -2.49 (difference 0.01)
    UnityAssertFloatsWithin((UNITY_FLOAT)0.01f,
                            (UNITY_FLOAT)(-2.50f),
                            (UNITY_FLOAT)(-2.49f),
                            NULL,
                            __LINE__);
}

// Minimal test runner using Unity's framework
int main(void)
{
    // Start the test framework
    UnityBegin("unity_assert_floats_within_tests.cpp");

    // Run tests, line numbers approximate the location of each RUN_TEST call
    RUN_TEST(test_UnityAssertFloatsWithin_PassWithinSmallDelta, __LINE__);
    RUN_TEST(test_UnityAssertFloatsWithin_FailOutsideDelta, __LINE__);
    RUN_TEST(test_UnityAssertFloatsWithin_PassExactEquality, __LINE__);
    RUN_TEST(test_UnityAssertFloatsWithin_PassWithLargeDelta, __LINE__);
    RUN_TEST(test_UnityAssertFloatsWithin_PassWithNegativeActual, __LINE__);

    // End and report results
    return UnityEnd();
}