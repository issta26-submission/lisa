/*
  C++11 compliant test harness for the focal Unity test method:
    testNotEqualHEX16EachEqual3(void)
  located in testunity.c

  Rationale:
  - Step 1: Understand that the focal method asserts that all elements in a
    given unsigned short array are equal to a provided value (8 here). The test
    purposely uses an array where one element differs, and wraps the assertion
    with EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to verify Unity reports a failure.

  - Step 2: Build a small test suite in C++ that invokes the focal test
    alongside related not-equal variants to improve coverage of this behavior.

  - Step 3: Use Unity's C API directly from C++11, with an extern "C" linkage
    for the C-based test functions to avoid name mangling.

  Notes:
  - This harness does not depend on GTest. It uses the provided Unity macros.
  - To compile and run:
      g++ -std=c++11 -I<path-to-unity-header> -c test_suite_focal_not_equal_hex16.cpp
      gcc -c testunity.c
      g++ -o run_tests test_suite_focal_not_equal_hex16.o testunity.o -lm
    Ensure the Unity C source (testunity.c) is compiled and linked as part of the build.

  - The harness focuses on exercising the following related tests:
    testNotEqualHEX16EachEqual1, testNotEqualHEX16EachEqual2, testNotEqualHEX16EachEqual3
    so that we cover multiple not-equal permutations for the HEX16-each-equal check.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Forward declarations of the focal test and related variants.
// They are defined in testunity.c (C code). We declare them with C linkage.
void testNotEqualHEX16EachEqual1(void);
void testNotEqualHEX16EachEqual2(void);
void testNotEqualHEX16EachEqual3(void);
#ifdef __cplusplus
}
#endif

// Unity setup/teardown hooks (no-op for this harness)
void setUp(void)   { /* Optional per-test setup */ }
void tearDown(void)  { /* Optional per-test teardown */ }

// Domain note:
// - We are explicitly targeting non-GTest environments; Unity remains the test harness.
// - The static members in the focal class are part of the Unity test file's static context.
//   Access to those is via the tested macros within testunity.c, not from here.

int main(void) {
    // Begin Unity test run
    UNITY_BEGIN();

    /*
      Comprehensive test invocation plan:
      - Execute related not-equal HEX16-each-equal tests to increase coverage
        of edge cases and ensure consistent failure behavior when a mismatch exists.
    */

    // 1) Run the first not-equal-HEX16-EACH-EQUAL test case
    // This validates that if the first non-matching element is encountered early,
    // Unity properly aborts the test as expected.
    RUN_TEST(testNotEqualHEX16EachEqual1);
    // 2) Run the second not-equal-HEX16-EACH-EQUAL test case
    // This ensures the failure paths for a different array composition are handled.
    RUN_TEST(testNotEqualHEX16EachEqual2);
    // 3) Run the focal not-equal-HEX16-EACH-EQUAL test case (the one of interest)
    // This is the primary target of the focal method under test.
    RUN_TEST(testNotEqualHEX16EachEqual3);

    // Additional optional coverage (commented out by default to keep suite focused):
    // RUN_TEST(testNotEqualHEX16EachEqual4);
    // RUN_TEST(testNotEqualHEX16EachEqual5);

    // End Unity test run
    return UNITY_END();
}

/*
  Explanatory notes for future maintainers:
  - The harness delegates actual test logic to the C test functions defined in testunity.c.
  - Each RUN_TEST invocation corresponds to a separate Unity test function that
    wraps a scenario where an array of unsigned short values is checked to ensure
    all elements are equal to a given value. These particular tests purposely
    include a non-matching element to trigger a failure, which the Unity framework
    captures as an expected abort.
  - By including all three related not-equal variants, we improve coverage of
    different array compositions and ensure consistent failure detection across
    variations.