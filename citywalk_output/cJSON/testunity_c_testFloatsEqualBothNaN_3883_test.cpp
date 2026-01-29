/* File: test_floats_equal_both_nan_with_float.cpp
 * Purpose: Validate the focal method testFloatsEqualBothNaN(void) when floats are enabled.
 * This builds a dedicated Unity test runner that calls the focal test directly.
 * Two builds are intended:
 *  - This one: without UNITY_EXCLUDE_FLOAT defined (uses NaN equality path).
 *  - A second runner can be built with -DUNITY_EXCLUDE_FLOAT to exercise the ignore path.
 *
 * Candidate Keywords (Step 1):
 * - UNITY_FLOAT,  f_zero,  0.0f,  NaN
 * - UNITY_EXCLUDE_FLOAT, TEST_IGNORE, TEST_ASSERT_EQUAL_FLOAT
 * - testFloatsEqualBothNaN (focal method)
 * - unity.h, RUN_TEST, UNITY_BEGIN, UNITY_END
 * - extern "C" linkage for C function from testunity.c
 *
 * This file is intended to be compiled with Unity's sources (unity.c) and linked accordingly.
 */
 
#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Include Unity headers (C interface; wrap in extern "C" for C++ compilation)
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Forward declaration of the focal test (defined in testunity.c)
#ifdef __cplusplus
extern "C" {
#endif
void testFloatsEqualBothNaN(void);
#ifdef __cplusplus
}
#endif

// Lightweight standalone runner for C++11
int main(void)
{
    // Start a Unity test run for the focal NaN-equality test (float path)
    UNITY_BEGIN("testFloatsEqualBothNaN_with_float");
    RUN_TEST(testFloatsEqualBothNaN);
    // End the test run and return result
    return UNITY_END();
}
 
/* End of test_floats_equal_both_nan_with_float.cpp */


/* File: test_floats_equal_both_nan_with_float_excluded.cpp
 * Purpose: Validate the focal method testFloatsEqualBothNaN(void) when UNITY_EXCLUDE_FLOAT is defined.
 * This forces the focal test to take the ignore path (TEST_IGNORE()).
 *
 * This file mirrors the structure of the previous runner but defines UNITY_EXCLUDE_FLOAT
 * to exercise the "excluded" branch within the focal method.
 *
 * Candidate Keywords (Step 1, extended for alternate path):
 * - UNITY_EXCLUDE_FLOAT (forces ignore path)
 * - TEST_IGNORE, TEST_ASSERT_EQUAL_FLOAT
 * - 0.0f / f_zero NaN scenario
 * - testFloatsEqualBothNaN (focal method)
 * - extern "C" linkage for C function from testunity.c
 *
 * Build note: Compile this file with -DUNITY_EXCLUDE_FLOAT to ensure the ignore branch executes.
 */
 
#define UNITY_EXCLUDE_FLOAT

// Include Unity headers (C interface; wrap in extern "C" for C++ compilation)
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Forward declaration of the focal test (defined in testunity.c)
#ifdef __cplusplus
extern "C" {
#endif
void testFloatsEqualBothNaN(void);
#ifdef __cplusplus
}
#endif

// Runner
int main(void)
{
    // Start a Unity test run for the focal NaN-equality test (float path is excluded)
    UNITY_BEGIN("testFloatsEqualBothNaN_with_float_excluded");
    RUN_TEST(testFloatsEqualBothNaN);
    // End the test run and return result
    return UNITY_END();
}
 
/* End of test_floats_equal_both_nan_with_float_excluded.cpp */