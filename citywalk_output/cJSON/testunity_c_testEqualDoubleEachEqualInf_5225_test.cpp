/***********************************************
  FOCAL TEST SUITE: testEqualDoubleEachEqualInf
  Context:
  - Focal method under test: testEqualDoubleEachEqualInf (located in testunity.c)
  - Core dependencies and keywords inspired by the focal code:
      Candidate Keywords: d_zero, 1.0/d_zero (infinity), p0 (array), TEST_ASSERT_EACH_EQUAL_DOUBLE,
                         UNITY_EXCLUDE_DOUBLE, TEST_IGNORE, 2-element predicate
  - The focal test uses a conditional UNITY_EXCLUDE_DOUBLE to either run the check
    or skip the test (TEST_IGNORE). It then creates an array:
      double p0[] = {1.0 / d_zero, 1.0 / d_zero, 25.4, 0.253};
    and asserts that the first 2 elements equal 1.0/d_zero via TEST_ASSERT_EACH_EQUAL_DOUBLE.
  - This wrapper suite provides cross-file entry points to invoke the focal test
    in a way that is friendly to a typical Unity-based test harness in C/C++11.

  Approach:
  - Expose the focal test as an extern "C" symbol so a C++11 test file can invoke it.
  - Provide two wrapper test entry points:
      1) testEqualDoubleEachEqualInf_Wrapper: direct delegation to the focal test.
      2) testEqualDoubleEachEqualInf_IgnoreOrCall: respects UNITY_EXCLUDE_DOUBLE to
         ensure the ignore-branch is exercised when doubles are excluded.
  - All wrappers rely on UNITY's TEST_IGNORE when doubles are excluded, exactly mirroring
    the focal method's intended behavior.

  This file is intended to be compiled and linked with testunity.c (the focal test source)
  to extend the Unity test suite without modifying testunity.c directly.
***********************************************/

/* Include Unity's API to use TEST_IGNORE and related macros. */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
/* Forward declaration of the focal test function defined in testunity.c.
   The function signature matches the focal method provided. */
void testEqualDoubleEachEqualInf(void);
#ifdef __cplusplus
}
#endif

/* 
   Wrapper 1: Expose a straightforward entry point that delegates directly
   to the focal test. This allows the Unity runner to register a separate test
   case for the focal behavior without duplicating its logic.
*/
void testEqualDoubleEachEqualInf_Wrapper(void)
{
    // Direct delegation to the focal test function.
    // If UNITY_EXCLUDE_DOUBLE is defined in the build, the focal test will skip
    // accordingly, matching the original intent.
    testEqualDoubleEachEqualInf();
}

/* 
   Wrapper 2: Provides an additional wrapper that explicitly mirrors the
   conditional path for Unity's double support.
   - If UNITY_EXCLUDE_DOUBLE is defined, the test will be ignored (TEST_IGNORE()).
   - Otherwise, it will invoke the focal test.
*/
void testEqualDoubleEachEqualInf_IgnoreOrCall(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    testEqualDoubleEachEqualInf();
#endif
}