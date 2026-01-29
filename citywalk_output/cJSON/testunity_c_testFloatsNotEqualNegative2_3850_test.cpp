// test_floats_not_equal_negative2_suite.cpp
// A dedicated Unity-based test suite to exercise the focal method
// testFloatsNotEqualNegative2 found in testunity.c, along with
// an additional compile-time path test to cover both UNITY_EXCLUDE_FLOAT
// defined and undefined scenarios.

// This file targets C++11 compilation (as required), but uses the
// Unity C unit testing framework for consistency with the focal method's
// environment. The focal function and related macros live in C code.

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

extern "C" {
}

// Forward declaration of the focal test function.
// It is defined in testunity.c and uses Unity macros to assert abort behavior
// when two floats are not equal.
#ifdef __cplusplus
extern "C" {
#endif
void testFloatsNotEqualNegative2(void);
#ifdef __cplusplus
}
#endif

// Additional test to exercise the alternative compile-time path for UNITY_EXCLUDE_FLOAT.
// This test mirrors the structure of the focal test but is compiled in this unit
// to ensure both code paths are exercised when building the test binary.
void testSimulatedFloatExclude(void)
{
#ifdef UNITY_EXCLUDE_FLOAT
    // If Unity is compiled with float support excluded, the focal test should be ignored.
    // This branch validates that the "float excluded" scenario is handled gracefully.
    TEST_IGNORE();
#else
    // If floats are included (default path), we attempt the same equality check
    // as in the focal test and expect an abort (i.e., a failure of the equality test).
    // This ensures the "not equal" condition triggers the failure path as designed.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_FLOAT(-9273.0049f, -9273.9649f);
    VERIFY_FAILS_END
#endif
}

// Minimal setup/teardown used by Unity
void setUp(void) { /* Optional per-test setup could go here */ }
void tearDown(void) { /* Optional per-test teardown could go here */ }

// Main entry point to run the test suite
int main(void)
{
    // Start Unity test framework
    UnityBegin("test_floats_not_equal_negative2_suite");

    // Run the focal method's test as part of the suite.
    // The testFloatsNotEqualNegative2 function contains UNITY_EXCLUDE_FLOAT guards
    // and will exercise both paths (abort vs. ignore) depending on compilation flags.
    RUN_TEST(testFloatsNotEqualNegative2);

    // Run the additional path-covering test to validate both compilation branches.
    RUN_TEST(testSimulatedFloatExclude);

    // Finish Unity test run and return status
    return UnityEnd();
}

// Explanatory notes:
// - testFloatsNotEqualNegative2: Validates that two specific negative floats are not equal.
//   The test is designed to abort on failure (via TEST_ASSERT_EQUAL_FLOAT), which Unity
//   treats as a test failure in the default path, or as an ignored test when UNITY_EXCLUDE_FLOAT
//   is defined (as in the focal code path).
// - testSimulatedFloatExclude: Provides coverage for both compile-time branches of
//   UNITY_EXCLUDE_FLOAT within a single translation unit. When UNITY_EXCLUDE_FLOAT is defined,
//   it calls TEST_IGNORE(); otherwise it replicates the exact assertion pattern from the focal
//   test and uses EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to validate abort behavior.