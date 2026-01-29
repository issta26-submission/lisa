/* 
   File: testunity_doubles_not_equal_with_double.cpp

   Purpose:
   - Provide a minimal Unity-based test harness that runs the focal method:
     testDoublesNotEqual(void) located in testunity.c.
   - This variant is built without UNITY_EXCLUDE_DOUBLE, so the focal test
     exercises the expected failure path (TEST_ASSERT_EQUAL_DOUBLE should fail)
     and is captured by Unity's EXPECT_ABORT_BEGIN/VERIFY_FAILS_END construct.

   How this aligns with the task:
   - Step 1: Understand that testDoublesNotEqual compares two doubles that are
     intentionally different, triggering an abort when doubles are not equal.
   - Step 2: The harness directly runs the focal method to validate its behavior
     under the "double comparison failure" scenario.
   - Step 3: The test is kept concise and uses non-private API (as provided by Unity)
     to maximize coverage of the focal path.

   Notes:
   - Build this file with UNITY_EXCLUDE_DOUBLE undefined (default).
   - The linker must include the translation unit that defines testDoublesNotEqual
     (i.e., testunity.c) in the same build.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Forward declaration of the focal test from the provided source file.
void testDoublesNotEqual(void);
#ifdef __cplusplus
}
#endif

int main(void)
{
    // Unity test runner setup
    UNITY_BEGIN();

    // Run the focal test. 
    // This test uses EXPECT_ABORT_BEGIN/VERIFY_FAILS_END to correctly account
    // for the expected abort when the double comparison fails.
    // The test will pass if the abort is triggered and handled as designed.
    RUN_TEST(testDoublesNotEqual);

    return UNITY_END();
}
 /* End of testunity_doubles_not_equal_with_double.cpp */


/* 
   File: testunity_doubles_not_equal_with_double_excluded.cpp

   Purpose:
   - Provide an alternative harness for the same focal method when the
     macro UNITY_EXCLUDE_DOUBLE is defined at compile time.
   - In this mode, testDoublesNotEqual is compiled to bypass the actual double
     comparison and simply call TEST_IGNORE() (as per the focal method's code path).

   How this aligns with the task:
   - Step 1: Understand both branches of the focal method depending on UNITY_EXCLUDE_DOUBLE.
   - Step 2: This harness exercises the "excluded double" branch by ensuring
     the focal test is compiled under UNITY_EXCLUDE_DOUBLE.
   - Step 3: Use a separate build to avoid conflicts with the non-excluded variant.

   Build instructions:
   - Compile this file as part of a separate build with UNITY_EXCLUDE_DOUBLE defined
     for both this harness and the testunity.c translation unit (typically via
     -DUNITY_EXCLUDE_DOUBLE in your compiler/linker flags).

   Notes:
   - This harness mirrors the structure of the previous file but forces the
     double-excluded path, enabling coverage of both code paths in isolation.
*/

#define UNITY_EXCLUDE_DOUBLE

#ifdef __cplusplus
extern "C" {
#endif
void testDoublesNotEqual(void);
#ifdef __cplusplus
}
#endif

int main(void)
{
    // Unity test runner setup
    UNITY_BEGIN();

    // Run the focal test in the UNITY_EXCLUDE_DOUBLE path.
    RUN_TEST(testDoublesNotEqual);

    return UNITY_END();
}
/* End of testunity_doubles_not_equal_with_double_excluded.cpp */