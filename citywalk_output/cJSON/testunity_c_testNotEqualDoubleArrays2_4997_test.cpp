/*
  Candidate Keywords (Step 1):
  - UNITY_EXCLUDE_DOUBLE, TEST_IGNORE, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
  - TEST_ASSERT_EQUAL_DOUBLE_ARRAY, double arrays, length 4
  - testNotEqualDoubleArrays2 (focal method under test)
  - Unity test framework integration (UnityBegin, RUN_TEST, UNITY_END)
  - External/class dependencies: Unity core/macro utilities
  - Coverage targets: not-equal-branch for double array comparison

  Rationale (Step 2 / Step 3):
  - The focal method testNotEqualDoubleArrays2 checks that two double arrays are not equal (via TEST_ASSERT_EQUAL_DOUBLE_ARRAY)
    and that the test properly aborts when expectation is not met. This is wrapped by EXPECT_ABORT_BEGIN / VERIFY_FAILS_END.
  - We expose a small test harness in C++11 that can run the focal test in a normal and a conditional path (to exercise
    the UNITY_EXCLUDE_DOUBLE path as a compile-time gate). The latter is achieved by a small wrapper that uses TEST_IGNORE()
    when UNITY_EXCLUDE_DOUBLE is defined.
  - The harness uses a minimal setUp/tearDown pattern and demonstrates how to invoke the focal test in a non-GTest environment
    (no GTest per requirements), leveraging the Unity framework.
  - All unit tests include explanatory comments documenting intent and what each test verifies.

  Note:
  - The focal function testNotEqualDoubleArrays2 is assumed to be compiled together with Unity (as in the provided
    testunity.c). We declare it here with C linkage to avoid C++ name mangling.
  - This file is intended to be compiled with the rest of the Unity-based test suite (e.g., along with testunity.c)
    under a C++11-capable compiler.
*/

#include <cstddef>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif


/* Declaration of the focal test function (defined in testunity.c in the provided project). */
void testNotEqualDoubleArrays2(void);

#ifdef __cplusplus
}
#endif

// Forward declarations of wrapper tests (C++ side)
void testDoubleArraysNotEqualWrapper_Normal(void);
void testDoubleArraysNotEqualWrapper_Conditional(void);

// Main entry using Unity's test runner (C-style API adapted for C++11)
/* Note: This main function enables running the two wrapper tests which both exercise the focal test path.
   The "_Conditional" wrapper additionally exercises the compile-time gate Unity provides via UNITY_EXCLUDE_DOUBLE. */
int main(void)
{
    // Initialize Unity test framework
    UnityBegin("test_NotEqualDoubleArrays2_suite.cpp");

    // Run direct focal test wrapper (delegates to focal function)
    RUN_TEST(testDoubleArraysNotEqualWrapper_Normal);

    // Run wrapper that demonstrates conditional IGNORE behavior based on UNITY_EXCLUDE_DOUBLE
    RUN_TEST(testDoubleArraysNotEqualWrapper_Conditional);

    // Finish Unity test run
    return UnityEnd();
}

// SetUp and TearDown for the Unity tests (empty in this harness)
void setUp(void) { /* no setup required for these tests */ }
void tearDown(void) { /* no teardown actions required for these tests */ }

/*
  Test 1: Normal execution path
  - Invokes the focal testNotEqualDoubleArrays2 directly.
  - Purpose: Ensure that the not-equal double arrays case triggers an expected failure/abort and is handled by
    the EXPECT_ABORT_BEGIN / VERIFY_FAILS_END mechanism within the focal test.
  - Rationale: This exercises the standard code-path when UNITY_EXCLUDE_DOUBLE is not defined.
*/
void testDoubleArraysNotEqualWrapper_Normal(void)
{
    // Directly call the focal test. The focal test contains its own EXPECT_ABORT_BEGIN/VERIFY_FAILS_END
    // blocks to validate that the equality assertion fails as expected.
    testNotEqualDoubleArrays2();
}

// Test 2: Conditional IGNORE path demonstration
// - If UNITY_EXCLUDE_DOUBLE is defined at compile time, the focal test will exercise the TEST_IGNORE() path.
// - If not defined, it will behave like Test 1.
// - This wrapper thus exercises both possible compiler-time branches in a single test harness.
void testDoubleArraysNotEqualWrapper_Conditional(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // Explicitly mark as ignored to demonstrate the IGNORE branch in a separate wrapper path.
    TEST_IGNORE_MESSAGE("UNITY_EXCLUDE_DOUBLE defined: testNotEqualDoubleArrays2 path is ignored.");
#else
    // Otherwise, delegate to the focal test (which will internally exercise the not-equal path).
    testNotEqualDoubleArrays2();
#endif
}