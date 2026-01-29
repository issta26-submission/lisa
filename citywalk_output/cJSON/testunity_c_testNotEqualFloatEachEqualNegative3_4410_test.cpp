/*
 * Test Suite Wrapper for Focal Method: testNotEqualFloatEachEqualNegative3
 * 
 * Purpose:
 *   Provide a C++11 based driver that executes the focal Unity test
 *   function defined in testunity.c: testNotEqualFloatEachEqualNegative3(void)
 *   using the Unity test framework without relying on GTest.
 *
 * Notes:
 *   - The focal test is designed to exercise the NOT EQUAL path for a
 *     float array where one element mismatches the expected value.
 *   - The focal function contains a compile-time branch controlled by
 *     UNITY_EXCLUDE_FLOAT. The test wrapper uses RUN_TEST to invoke the
 *     focal function under the active compilation flags.
 *   - This wrapper declares the focal function with C linkage to allow
 *     cross-translation-unit usage from C++.
 *   - The code adheres to C++11 compatibility and uses Unity's
 *     non-terminating test macros (EXPECT_ABORT_BEGIN / VERIFY_FAILS_END)
 *     within the focal test itself.
 */

// Include Unity framework header (C-compat)
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Forward declaration of the focal test function defined in testunity.c
void testNotEqualFloatEachEqualNegative3(void);
#ifdef __cplusplus
}
#endif

int main(void)
{
    // Initialize Unity test framework and run the focal test.
    // This harness simply executes the focal test function and reports
    // its result to Unity's test runner.
    UnityBegin("testNotEqualFloatEachEqualNegative3_suite");
    // Run the focal test that verifies the NOT EQUAL path for float arrays.
    RUN_TEST(testNotEqualFloatEachEqualNegative3);
    return UnityEnd();
}

/*
Explanation of test strategy:
- The focal test testNotEqualFloatEachEqualNegative3 exercises the following:
  - When UNITY_EXCLUDE_FLOAT is not defined, it creates an array of four floats:
      p0 = {-8.0f, -8.0f, -8.0f, -0.253f}
    and asserts that all elements are equal to -8.0f using TEST_ASSERT_EACH_EQUAL_FLOAT(-8.0f, p0, 4).
  - Since one element (-0.253f) differs, the assertion should fail, and the test intentionally
    exercises Unity's abort/fail handling via EXPECT_ABORT_BEGIN and VERIFY_FAILS_END.
  - If UNITY_EXCLUDE_FLOAT is defined, the test will call TEST_IGNORE(), and the focal test
    effectively becomes an ignored test in Unity.
- This wrapper ensures high coverage by executing the focal test under the Unity test runner
  in a controlled, repeatable manner without requiring GTest or private test hooks. 
*/