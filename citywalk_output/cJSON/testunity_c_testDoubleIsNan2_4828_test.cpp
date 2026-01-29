/***** File: test_double_is_nan2_run.cpp *****/
/*
Candidate Keywords (Step 1):
- UNITY_EXCLUDE_DOUBLE
- TEST_ASSERT_DOUBLE_IS_NOT_NAN
- 0.0 / d_zero
- d_zero (UNITY_DOUBLE)
- EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
- TEST_IGNORE
- startPutcharSpy / endPutcharSpy (Unity IO spy)
- Static/namespace handling: testDoubleIsNan2 resides in testunity.c under Unity test framework
- Unity test harness integration (RUN_TEST, UnityBegin, UnityEnd)

This test suite exercises the focal method testDoubleIsNan2 located in testunity.c
under the non-UNITY_EXCLUDE_DOUBLE path. It uses Unity's RUN_TEST harness to
execute the focal test in a standard, non-terminating manner within the Unity
framework.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal test under test: defined in testunity.c
void testDoubleIsNan2(void);
#ifdef __cplusplus
}
#endif

int main(void)
{
    // Begin Unity test session for the focal test in a non-excluded build
    UnityBegin("test_double_is_nan2_run_no_exclude");
    // Execute the focal test
    RUN_TEST(testDoubleIsNan2);
    // End Unity test session
    return UnityEnd();
}


/***** End File: test_double_is_nan2_run.cpp *****/


/***** File: test_double_is_nan2_ignore_run.cpp *****/
/*
Candidate Keywords (Step 1):
- UNITY_EXCLUDE_DOUBLE
- TEST_ASSERT_DOUBLE_IS_NOT_NAN
- 0.0 / d_zero
- d_zero (UNITY_DOUBLE)
- EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
- TEST_IGNORE
- startPutcharSpy / endPutcharSpy (Unity IO spy)
- Static/namespace handling: testDoubleIsNan2 resides in testunity.c under Unity test framework
- Unity test harness integration (RUN_TEST, UnityBegin, UnityEnd)

This test suite exercises the focal method testDoubleIsNan2 located in testunity.c
under the Unity compile-time path where UNITY_EXCLUDE_DOUBLE is defined. In this
path, the focal test is expected to call TEST_IGNORE() and be skipped by the test
harness. This provides coverage for the “excluded-double” branch of the focal
method (Step 2: Class Dependencies). It uses Unity's RUN_TEST to execute the
focal test within a separate build configuration.
*/


#ifdef __cplusplus
extern "C" {
#endif
// Focal test under test: defined in testunity.c
void testDoubleIsNan2(void);
#ifdef __cplusplus
}
#endif

// Define UNITY_EXCLUDE_DOUBLE to force the exclusion path inside testDoubleIsNan2
#define UNITY_EXCLUDE_DOUBLE

int main(void)
{
    // Begin Unity test session for the excluded-double variant
    UnityBegin("test_double_is_nan2_run_with_exclude");
    // Execute the focal test which should be ignored due to UNITY_EXCLUDE_DOUBLE
    RUN_TEST(testDoubleIsNan2);
    // End Unity test session
    return UnityEnd();
}


/***** End File: test_double_is_nan2_ignore_run.cpp *****/