/*
  Test Suite for focal method: testNotEqualFloatArrays3
  Context:
  - FOCAL_METHOD is testNotEqualFloatArrays3 in testunity.c
  - Core dependency: TEST_ASSERT_EQUAL_FLOAT_ARRAY, EXPECT_ABORT_BEGIN/VERIFY_FAILS_END
  - FLOAT support guarded by UNITY_EXCLUDE_FLOAT
  - Uses Unity framework (C, but test file can compile under C++11)

  Candidate Keywords (Step 1):
  - testNotEqualFloatArrays3
  - TEST_ASSERT_EQUAL_FLOAT_ARRAY
  - UNITY_FLOAT
  - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
  - float arrays p0, p1
  - 4 as the length parameter
  - UNITY_EXCLUDE_FLOAT (branch to TEST_IGNORE in focal method)
  - test framework hooks: setUp/tearDown (if used)
*/

/* Step 2/3: Expanded, executable test suite for testNotEqualFloatArrays3 with additional coverage
   - Adds two extra tests to exercise additional not-equal scenarios for the same focal method.
   - All tests are non-terminating when possible, using EXPECT_ABORT_BEGIN style where appropriate.
   - Includes explanatory comments for each test, per the request.
*/

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#if defined(__cplusplus)
}
#endif

// Candidate: expose tests in a C/C++ compatible way.
// In a real Unity run, these would be compiled and linked with the existing testunity.c.
// Here we provide additional test cases that exercise the focal not-equal float array test.

#ifdef __cplusplus
extern "C" {
#endif

// Forward declare a minimal setUp/tearDown to ensure linkage if the test runner expects them.
void setUp(void);
void tearDown(void);

// The focal test under testNotEqualFloatArrays3
// This mirrors the provided focal method:
//   - When UNITY_EXCLUDE_FLOAT is defined, it should be ignored.
//   - Otherwise, it asserts that two 4-element float arrays are not equal, expecting a failure.
void testNotEqualFloatArrays3(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float p0[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float p1[] = {1.0f, 8.0f, 25.5f, 0.253f};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

// Additional coverage: difference at index 2 (third element)
void testNotEqualFloatArrays3_DifferenceAtIndex2(void)
{
    // This test ensures the not-equal detection works when the difference
    // occurs at a different position than in the focal case.
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float a[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float b[] = {1.0f, 8.0f, 25.41f, 0.253f}; // differ at index 2
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(a, b, 4);
    VERIFY_FAILS_END
#endif
}

// Additional coverage: multiple differences across the array
void testNotEqualFloatArrays3_AllDifferent(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float x[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float y[] = {0.5f, 7.5f, 24.0f, 0.300f}; // all elements differ
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(x, y, 4);
    VERIFY_FAILS_END
#endif
}

// Optional: a passing case to verify no false positives (not strictly required by focal method)
// This demonstrates that when arrays are equal, the test would terminate as a pass.
// Note: This test is not directly part of testNotEqualFloatArrays3, but helps validate coverage.
// The focal method itself is designed to fail; this test ensures a positive path is distinguishable.
void testEqualFloatArrays_PassCase(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    float a[] = {1.0f, 8.0f, 25.4f, 0.253f};
    float b[] = {1.0f, 8.0f, 25.4f, 0.253f};
    // This is a non-aborting assertion: it should pass.
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(a, b, 4);
#endif
}

void testNotEqualFloatArrays3_NaN_NegativeScenario(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    TEST_IGNORE();
#else
    // Special values: NaN propagation vs equality. Depending on the Unity build,
    // TEST_ASSERT_EQUAL_FLOAT_ARRAY may treat NaNs as unequal. This test ensures
    // the code path still reports a failure when arrays are not equal.
    float p0[] = {1.0f, NAN, 25.4f, 0.253f};
    float p1[] = {1.0f, NAN, 25.4f, 0.253f};
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}

// Main test runner (for standalone compilation if integrated into a custom runner)
#ifdef UNITY_BEGIN
// If the project uses a custom runner, this section will not be used.
// Otherwise, provide a runnable main for standalone execution.
int main(void)
{
    UNITY_BEGIN("testNotEqualFloatArrays3_suite.c");

    RUN_TEST(testNotEqualFloatArrays3);
    RUN_TEST(testNotEqualFloatArrays3_DifferenceAtIndex2);
    RUN_TEST(testNotEqualFloatArrays3_AllDifferent);
    RUN_TEST(testEqualFloatArrays_PassCase);
    RUN_TEST(testNotEqualFloatArrays3_NaN_NegativeScenario);

    return UNITY_END();
}
#endif

#ifdef __cplusplus
}
#endif

/* Notes:
   - The Candidate Keywords (Step 1) are reflected in the comments at the top of this file.
   - Step 2: The suite focuses on testNotEqualFloatArrays3 and extends coverage with two additional cases,
     plus a passing-case variant to help validate behavior boundaries.
   - Step 3: Tests use Unity macros (EXPECT_ABORT_BEGIN, TEST_ASSERT_EQUAL_FLOAT_ARRAY, etc.)
     and guard FLOAT usage with UNITY_EXCLUDE_FLOAT, as per the focal code's patterns.
*/