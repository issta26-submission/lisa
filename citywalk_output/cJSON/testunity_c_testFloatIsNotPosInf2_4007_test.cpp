/*
  Test Suite: test_float_not_pos_inf2_suite.cpp

  This suite focuses on the focal method:
    void testFloatIsNotPosInf2(void)

  The focal method is defined in testunity.c and is guarded by UNITY_EXCLUDE_FLOAT.
  To validate both code paths (float tests included vs ignored via UNITY_EXCLUDE_FLOAT),
  this suite provides two wrappers that exercise the focal test under the current
  compilation configuration.

  How this suite works:
  - If UNITY_EXCLUDE_FLOAT is defined at compile time, the focal test calls TEST_IGNORE(),
    causing Unity to mark the test as ignored. The wrapper asserts that Unity.CurrentTestIgnored
    is set, thereby validating the "ignored" path.
  - If UNITY_EXCLUDE_FLOAT is not defined, the focal test executes its assertion
    TEST_ASSERT_FLOAT_IS_NOT_INF(2.0f); and the wrapper validates that no failure was reported.

  Notes:
  - This test suite uses Unity's RUN_TEST framework and requires the Unity core (unity.c / unity.h)
    to be built and linked with this translation unit.
  - The focal test is declared with C linkage to ensure correct linkage when compiled in a C++
    environment.
  - The approach respects the provided domain knowledge constraints (no GTest, standalone code,
    and coverage of both compile-time branches of the focal test).
  - The test suite is designed to be compiled alongside testunity.c (and Unity's sources) in a
    single binary.

  Candidate Keywords identified from the focal method and dependencies:
  - testFloatIsNotPosInf2, UNITY_EXCLUDE_FLOAT, TEST_IGNORE, TEST_ASSERT_FLOAT_IS_NOT_INF
  - UNITY_FLOAT, Unity.CurrentTestFailed, Unity.CurrentTestIgnored
  - RUN_TEST, UnityBegin, UnityEnd
  - testunity.c, Unity framework integration
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal test function located in testunity.c
void testFloatIsNotPosInf2(void);
#ifdef __cplusplus
}
#endif

/*
  Wrapper for the case when float tests are NOT excluded.
  This ensures that the focal test runs and does not trigger any failure.
  Expects that the focal test completes without calling TEST_FAIL or similar.
*/
#ifndef UNITY_EXCLUDE_FLOAT
static void testFloatIsNotPosInf2_NormalWrapper(void)
{
    // Reset any previous failure state for a clean check
    // Note: Unity's internal state is reused across tests; this ensures a clean assertion result.
    // If the focal test passes, Unity.CurrentTestFailed should remain 0.
    // The wrapper itself is a Unity test; it delegates the actual verification to the focal test.
    testFloatIsNotPosInf2();
    // Assert that no failure occurred within the focal test invocation
    TEST_ASSERT_EQUAL_INT(0, Unity.CurrentTestFailed);
}
#else
/*
  Wrapper for the case when float tests ARE excluded (UNITY_EXCLUDE_FLOAT is defined).
  We still invoke the focal test to exercise the ignore path and verify that Unity marks the test as ignored.
*/
static void testFloatIsNotPosInf2_ExcludedWrapper(void)
{
    // Execute focal test; with UNITY_EXCLUDE_FLOAT defined, it should call TEST_IGNORE()
    testFloatIsNotPosInf2();
    // After ignore, Unity should reflect that state in CurrentTestIgnored
    TEST_ASSERT_EQUAL_INT(1, Unity.CurrentTestIgnored);
}
#endif

/* 
  Test Runner
  - Depending on the compile-time macro UNITY_EXCLUDE_FLOAT, we run the corresponding wrapper.
  - The test suite name explains purpose and helps with test reporting.
*/
int main(void)
{
    // Initialize Unity test framework
    UnityBegin("test_float_not_pos_inf2_suite");

#ifdef UNITY_EXCLUDE_FLOAT
    // When UNITY_EXCLUDE_FLOAT is defined, verify the ignore path.
    RUN_TEST(testFloatIsNotPosInf2_ExcludedWrapper);
#else
    // When UNITY_EXCLUDE_FLOAT is not defined, verify the normal path.
    RUN_TEST(testFloatIsNotPosInf2_NormalWrapper);
#endif

    // Finalize and return test results
    return UnityEnd();
}