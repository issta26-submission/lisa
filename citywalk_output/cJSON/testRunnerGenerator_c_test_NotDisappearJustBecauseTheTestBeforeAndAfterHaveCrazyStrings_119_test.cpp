/*
Purpose:
- Provide a focused Unity-based test suite for the focal method:
  void test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings(void)
  located in testRunnerGenerator.c
- The suite invokes the focal test through a Unity test wrapper to ensure proper integration with the Unity harness.
- It also includes a second wrapper to verify idempotency by invoking the focal test twice.
- This file is designed to compile under C++11 as a standalone test runner, without GTest.

Key Candidate Keywords (Step 1 insights):
- test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings
- TEST_ASSERT_TRUE_MESSAGE
- Unity, RUN_TEST, setUp, tearDown
- Defs.h, CException (USE_CEXCEPTION path)
- 1 Should be True
- CrazyStrings, BeforeAndAfter
- The focal test’s purpose is to verify a trivial assertion remains true regardless of test order.
*/

#include <unity.h>
#include <CException.h>
#include <Defs.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
/* Prototype of the focal method under test (located in testRunnerGenerator.c) */
void test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings(void);
#ifdef __cplusplus
}
#endif

/* 
   Wrapper test 1:
   - Purpose: Execute the focal test once to validate the normal execution path within the Unity harness.
   - Rationale: Ensures the test can run without crashes and that the assertion passes as expected.
*/
static void test_NotDisappearWrapper(void)
{
    // Invoke the focal test directly. It asserts true with a descriptive message.
    test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings();
}

/* 
   Wrapper test 2:
   - Purpose: Execute the focal test twice to check for potential state leakage between invocations.
   - Rationale: Some test frameworks or environments risk static/global state leakage; calling twice helps exercise repeated runs.
*/
static void test_NotDisappearWrapper_Twice(void)
{
    test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings();
    test_NotDisappearJustBecauseTheTestBeforeAndAfterHaveCrazyStrings();
}

/* Unity setup/teardown hooks (empty by design for this focused suite) */
void setUp(void) { }
void tearDown(void) { }

/* Main entry point for this self-contained test runner.
   This file does not rely on the project-wide main; it provides an isolated runner
   that executes the focal test via wrappers. */
int main(void)
{
    UNITY_BEGIN();

    // Run the focal test via a single wrapper to ensure it integrates with Unity.
    RUN_TEST(test_NotDisappearWrapper);

    // Run the focal test via a second wrapper to validate repeated invocation behavior.
    RUN_TEST(test_NotDisappearWrapper_Twice);

    return UNITY_END();
}