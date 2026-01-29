/*
 * test_doubles_not_equal_actual_inf.cpp
 *
 * This test is focused on validating the focal method
 * testDoublesNotEqualActualInf() which resides in testunity.c.
 * The test suite is written to be compiled in a C++11 environment
 * without GTest, using the Unity framework.
 *
 * Notes aligned with the provided analysis:
 * - The focal test contains a preprocessor branch on UNITY_EXCLUDE_DOUBLE.
 *   When doubles are excluded, the test should be ignored (TEST_IGNORE()).
 * - We expose the focal function with C linkage so the Unity harness can link correctly.
 * - A single test entry is exercised here; Unity's EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
 *   semantics within the focal test handle the abort/failure expectations.
 * - This driver is intended to be built together with testunity.c in the same
 *   translation unit or linked together; UnityBegin/UnityEnd wrap the test run.
 */

#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of the focal test from testunity.c
void testDoublesNotEqualActualInf(void);

#ifdef __cplusplus
}
#endif


int main(void)
{
    // Initialize the Unity test framework for the focal test file.
    // The string "testunity.c" mirrors the original file context.
    UnityBegin("testunity.c");

    // Explanatory comments for test coverage:
    // - When UNITY_EXCLUDE_DOUBLE is NOT defined, the focal test will attempt
    //   to compare 321.642 with 1.0 / d_zero where d_zero == 0.0, producing Inf
    //   and a failed assertion. This path exercises the "double is enabled" branch.
    // - When UNITY_EXCLUDE_DOUBLE is defined, the focal test uses TEST_IGNORE(),
    //   thus the test is skipped. This path exercises the "double is excluded" branch.
    //
    // We deliberately invoke the focal test in both configurations (via compile-time
    // macro) by simply calling RUN_TEST here. Depending on the macro, Unity will
    // either execute it (and catch failure) or ignore it.
    
    // Run the focal test for doubles not equal to actual Inf.
    // The test itself handles both branches via preprocessor guards.
    RUN_TEST(testDoublesNotEqualActualInf);

    // Finalize the Unity test run and return the appropriate exit code.
    return UnityEnd();
}