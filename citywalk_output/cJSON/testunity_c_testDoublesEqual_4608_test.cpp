/*
Step 1: Program Understanding and Candidate Keywords
- Focal method under test: testDoublesEqual(void)
- Key dependent components:
  - Macro UNITY_EXCLUDE_DOUBLE controls whether the double-compare tests are executed or the test is ignored.
  - Assertion macro TEST_ASSERT_EQUAL_DOUBLE used to compare doubles exactly.
  - The four numeric literals used in the focal test:
    187245123456.0
    187241234567.5 vs 187241234567.6
    9273.2512345649 vs 9273.25123455699
    -726.12345693724 vs -726.1234569374
- Class/Module dependencies (from provided <FOCAL_CLASS_DEP> block):
  - Unity framework interfaces (unity.h)
  - Support macros for controlling test flow (TEST_IGNORE, TEST_ASSERT_EQUAL_DOUBLE)
  - The focal function itself is defined in testunity.c and is externally visible as testDoublesEqual(void)
- Domain knowledge alignment:
  - We will build a small test suite in C++11 that links against testunity.c’s object, invoking testDoublesEqual through Unity’s RUN_TEST mechanism and also provide an additional direct test that exercises the same double assertions using Unity’s macros.
*/

#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
// The focal test function defined in testunity.c
extern void testDoublesEqual(void);
#ifdef __cplusplus
}
#endif

// Additional direct test to exercise the same double equality checks in a separate test
static void testDoublesEqual_Direct(void)
{
#ifndef UNITY_EXCLUDE_DOUBLE
    // True-branch coverage: these should pass exactly as the focal test asserts
    TEST_ASSERT_EQUAL_DOUBLE(187245123456.0, 187245123456.0);
    TEST_ASSERT_EQUAL_DOUBLE(187241234567.5, 187241234567.6);
    TEST_ASSERT_EQUAL_DOUBLE(9273.2512345649, 9273.25123455699);
    TEST_ASSERT_EQUAL_DOUBLE(-726.12345693724, -726.1234569374);
#else
    // Ensure this path is exercised when doubles are excluded
    TEST_IGNORE();
#endif
}

int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Run the focal test from testunity.c
    // This validates the actual implementation of testDoublesEqual including its compile-time branching
    RUN_TEST(testDoublesEqual);

    // Run an additional direct test to exercise the same doubles-equality assertions
    // This helps improve coverage by exercising the assertion paths outside of the focal wrapper
    RUN_TEST(testDoublesEqual_Direct);

    // Finalize Unity tests
    return UNITY_END();
}