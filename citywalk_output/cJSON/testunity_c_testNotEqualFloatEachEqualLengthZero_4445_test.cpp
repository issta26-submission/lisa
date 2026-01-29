/* 
   File: test_NotEqualFloatEachEqualLengthZero_WithFloat.c
   Purpose:
   - Execute the focal testNotEqualFloatEachEqualLengthZero() in a build where UNITY_EXCLUDE_FLOAT is NOT defined.
   - This path exercises the float-based assertion path inside the focal method, which uses TEST_ASSERT_EACH_EQUAL_FLOAT and its surrounding EXPECT_ABORT_BEGIN/VERIFY_FAILS_END blocks.
   - The test is designed to validate that the focal test behaves as a non-aborting unit test wrapper around the Unity assertion when a zero-length array is involved, respecting Unity’s expected abort semantics for failing assertions within the test.
   - See below for Candidate Keywords and dependencies extracted from the focal method and its dependencies.

   Candidate Keywords (core dependencies the test relies on):
   - testNotEqualFloatEachEqualLengthZero: focal method under test
   - float, 0.0f: test data used inside focal method
   - TEST_ASSERT_EACH_EQUAL_FLOAT: Unity assertion under test
   - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: Unity failure-capturing wrappers used by the focal method
   - p0: local array in focal method
   - UNITY_EXCLUDE_FLOAT: macro that toggles between abort-path and ignore-path inside the focal method
   - unity.h: Unity framework header
   - extern void testNotEqualFloatEachEqualLengthZero(void): declaration for the focal test
   - main/test runner: Unity test harness entry point (RUN_TEST macro)

   Dependencies inside the focal class file (as seen in <FOCAL_CLASS_DEP>):
   - #include "unity.h"
   - #include <string.h>
   - #include <stdint.h>
   - UNITY_FLOAT and related typedefs (conditioned by UNITY_EXCLUDE_FLOAT)
   - Macros: EXPECT_ABORT_BEGIN, VERIFY_FAILS_END, TEST_ASSERT_EACH_EQUAL_FLOAT
   - static/global state used by Unity test harness (e.g., Unity.*)
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


/* 
   Note: The focal function is defined in testunity.c and uses a macro-controlled path
   depending on whether UNITY_EXCLUDE_FLOAT is defined. This test file is intended to be
   compiled and linked with UNITY_EXCLUDE_FLOAT undefined (default) so that the float path
   (TEST_ASSERT_EACH_EQUAL_FLOAT) is exercised.
*/
extern void testNotEqualFloatEachEqualLengthZero(void);

int main(void)
{
    /* 
       Explanation:
       - This runner executes the focal test as-is. The focal test itself wraps the float-path
         assertion with EXPECT_ABORT_BEGIN / VERIFY_FAILS_END, so Unity should capture the
         expected abort/failure and not treat it as a fatal error for this test binary.
    */
    UNITY_BEGIN();
    RUN_TEST(testNotEqualFloatEachEqualLengthZero);
    return UNITY_END();
}
--------------------------------------------------
// File: test_NotEqualFloatEachEqualLengthZero_WithFloatExcluded.c
/*
   Purpose:
   - Execute the focal testNotEqualFloatEachEqualLengthZero() in a build where UNITY_EXCLUDE_FLOAT is defined.
   - This path exercises the ignore-path inside the focal method, ensuring that when the
     float path is excluded, the test simply ignores (TEST_IGNORE) and does not perform the float comparison.
   - This helps verify that the code properly handles the static/compile-time switch for the
     float feature and that the test harness still runs deterministically.

   Candidate Keywords (core dependencies the test relies on):
   - testNotEqualFloatEachEqualLengthZero: focal method under test
   - UNITY_EXCLUDE_FLOAT: macro toggling the float path to TEST_IGNORE()
   - TEST_IGNORE: Unity macro used when the float path is excluded
   - p0: local array placeholder in focal method (present but not executed in float-excluded path)
   - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: not used for the ignore path (path taken leads to no abort)
   - unity.h: Unity framework header
   - extern void testNotEqualFloatEachEqualLengthZero(void): declaration for the focal test
*/

#define UNITY_EXCLUDE_FLOAT

/* 
   Note: The focal function is defined in testunity.c and uses UNITY_EXCLUDE_FLOAT to pick the
   ignore path. This binary must be built with UNITY_EXCLUDE_FLOAT defined to exercise that path.
*/
extern void testNotEqualFloatEachEqualLengthZero(void);

int main(void)
{
    /*
       Explanation:
       - This runner exercises the ignore-path of the focal test by compiling with UNITY_EXCLUDE_FLOAT defined.
       - The focal test should call TEST_IGNORE() and return without aborting, allowing the test
         harness to continue cleanly.
    */
    UNITY_BEGIN();
    RUN_TEST(testNotEqualFloatEachEqualLengthZero);
    return UNITY_END();
}