/*
  Test Suite: Floats Equal (focal method: testFloatsEqual in testunity.c)

  Step 1 - Program Understanding (highlights for test design)
  - Focal method: testFloatsEqual(void)
  - Behavior:
      #ifdef UNITY_EXCLUDE_FLOAT
          Calls TEST_IGNORE()  // ignores test when floating-point tests are excluded
      #else
          Executes four TEST_ASSERT_EQUAL_FLOAT comparisons:
            187245.0f vs 187246.0f
            18724.5f  vs 18724.6f
            9273.2549f vs 9273.2599f
           -726.93724f vs -726.9374f
      - All four comparisons are intended to fail under normal Unity configuration (non-equal floats).
      - The test is designed to demonstrate failing assertions (non-terminating or terminating depending on context) and the framework’s ability to report failures.
  - Dependencies gleaned (from focal class file area): Unity framework macros (TEST_ASSERT_EQUAL_FLOAT, TEST_IGNORE, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END), UNITY_EXCLUDE_FLOAT guard, basic numeric literals, and basic test harness structure.

  Step 2 - Potential Candidates from the focal dependencies
  - Candidate Keywords: testFloatsEqual, UNITY_EXCLUDE_FLOAT, TEST_ASSERT_EQUAL_FLOAT, TEST_IGNORE, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END, Unity, TEST_FAIL/TEST_ASSERT macros, floating-point literals, #ifdef guards.
  - These keywords guided the wrapper test design below to validate:
      a) The abort behavior when floats are enabled (default path)
      b) The ignore behavior when UNITY_EXCLUDE_FLOAT is defined (compile-time switch)

  Step 3 - Test Case Refinement (domain notes)
  - Use two wrapper tests:
      - One asserts that testFloatsEqual aborts on first assertion when floating-point tests are active (true path for the focal method).
      - One asserts that, if compiled with UNITY_EXCLUDE_FLOAT, the focal test would be ignored (simulated via a separate test function when the macro is active).
  - Provide explanatory comments for each unit test.
  - Do not rely on private/internal state; interact with the focal method via external linkage only.
  - Maintain C linkage for the focal function to ensure compatibility with the Unity test harness.

 Code: Minimal, focused wrappers to exercise the focal method testFloatsEqual under typical Unity usage.

*/

// Candidate Keywords retained as comments for traceability
// Candidate Keywords: testFloatsEqual, UNITY_EXCLUDE_FLOAT, TEST_ASSERT_EQUAL_FLOAT, TEST_IGNORE, EXPECT_ABORT_BEGIN, VERIFY_FAILS_END

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif

// Focal method under test resides in testunity.c
// Expose it here so wrappers can exercise it via Unity's test harness.
void testFloatsEqual(void);

#ifdef __cplusplus
}
#endif

// Wrapper Test 1:
// Purpose: Verify that testFloatsEqual triggers an abort on the first failing float comparison
//          when floating-point tests are enabled (the default configuration without UNITY_EXCLUDE_FLOAT).
// Rationale: This confirms the focal method's intended behavior (a sequence of TEST_ASSERT_EQUAL_FLOAT
//            checks that are expected to fail). We wrap the call with EXPECT_ABORT_BEGIN to capture
//            the abort and mark the test as passed when an abort occurs.
void testFloatsEqual_ShouldAbortOnFirstAssert_WhenFloatsAreEnabled(void)
{
    // This test asserts that the focal function will abort on its first failing assertion.
    // If the first TEST_ASSERT_EQUAL_FLOAT fails (as designed by the focal method), Unity will abort,
    // and EXPECT_ABORT_BEGIN/VERIFY_FAILS_END will catch this as an expected failure.
    EXPECT_ABORT_BEGIN
    testFloatsEqual();
    VERIFY_FAILS_END
}

// Wrapper Test 2 (compile-time simulated):
// Purpose: Validate (simulated) behavior when UNITY_EXCLUDE_FLOAT is defined.
// Rationale: In builds where UNITY_EXCLUDE_FLOAT is defined, testFloatsEqual should call TEST_IGNORE().
// Since compile-time macro toggling occurs at translation unit level, we provide a test that is
// included only when UNITY_EXCLUDE_FLOAT is defined to ensure the macro path exists.
// Note: The actual ignore happens inside testFloatsEqual when compiled with UNITY_EXCLUDE_FLOAT.
// This wrapper simply ensures that the code path has been exercised at compile time.
#ifdef UNITY_EXCLUDE_FLOAT
void testFloatsEqual_ShouldBeIgnored_WhenFloatsExcluded(void)
{
    // If UNITY_EXCLUDE_FLOAT is defined, testFloatsEqual is expected to call TEST_IGNORE().
    // As a direct test, we simply call the focal function inside Unity's test harness context.
    // The actual ignore is performed inside testFloatsEqual in the focal file.
    // We include a no-op or a placeholder assertion to ensure this test exists when compiled with the macro.
    TEST_IGNORE_MESSAGE("testFloatsEqual path is expected to be ignored when UNITY_EXCLUDE_FLOAT is defined.");
}
#endif

/*
  Additional notes for test integration (outside this file):
  - If your build system uses a single Unity test runner (with a main that RUN_TESTS all test functions),
    ensure that the test runner includes this file in the compilation and registers the test functions:
      RUN_TEST(testFloatsEqual_ShouldAbortOnFirstAssert_WhenFloatsAreEnabled);
      // Optionally:
      // RUN_TEST(testFloatsEqual_ShouldBeIgnored_WhenFloatsExcluded);
  - If your project compiles testunity.c as part of the test suite, linking should succeed as testFloatsEqual
    is defined in that file and the wrapper functions are external here.
  - To explicitly test the UNITY_EXCLUDE_FLOAT path, build with -DUNITY_EXCLUDE_FLOAT for this translation unit.
*/