/*
  Comprehensive test harness for the focal method:
  testPassShouldEndImmediatelyWithPass located in testunity.c

  Step 1: Candidate Keywords (represented here as comments for traceability)
  - TEST_PASS
  - TEST_FAIL_MESSAGE
  - Unity (Unity's test framework)
  - UNITY_BEGIN / RUN_TEST / UNITY_END (Unity's test runner macros)
  - startPutcharSpy / endPutcharSpy (I/O capture utilities in the focal test file)
  - TEST_PROTECT and related macros (test protection)
  - The focal class dependencies are embedded in testunity.c via UNITY macros;
    this harness exercises the focal function within the Unity test harness.

  Step 2: Unit Test Generation
  - The goal is to verify that calling testPassShouldEndImmediatelyWithPass ends the test
    immediately (i.e., after TEST_PASS(), it should not execute subsequent TEST_FAIL_MESSAGE).

  Step 3: Test Case Refinement
  - The test is implemented as a Unity-based wrapper test that invokes the focal test
    function in a Unity context to ensure correct control-flow behavior.

  Note:
  - This file is written in C++11 for the host, but it relies on the C Unity-based
    test under testunity.c. The focal function is declared with C linkage.
  - Do not use GTest. This harness uses Unity's own framework.
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
    // Focal method located in testunity.c (extern "C" to avoid name mangling)
    void testPassShouldEndImmediatelyWithPass(void);

    // Optional Unity lifecycle hooks typically defined in Unity harness
    void setUp(void);
    void tearDown(void);
#ifdef __cplusplus
}
#endif

// Wrapper test to exercise the focal method within Unity's test context.
// Explanatory: Invoking testPassShouldEndImmediatelyWithPass should cause
//          the test to end immediately (via TEST_PASS), and the subsequent
//          TEST_FAIL_MESSAGE should never be reached.
void testWrapper_TestPassEndsImmediately(void)
{
    // This call should return normally, with the focal test having ended the test
    // on its own when TEST_PASS() is invoked inside.
    testPassShouldEndImmediatelyWithPass();
}

// Additional explanatory test (optional) to demonstrate integration with Unity's runner.
// This test intentionally does not exercise any new behavior beyond invoking the focal test.
// It serves to validate that the wrapper test is properly registered and executed within Unity.
void testWrapper_IntegrityOfWrapperRegistration(void)
{
    testPassShouldEndImmediatelyWithPass(); // Re-invoke the focal test for redundancy
    // If the focal test aborted, Unity would halt this test; reaching here implies
    // the focal test ended as expected.
}

int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Register and run the wrapper tests
    RUN_TEST(testWrapper_TestPassEndsImmediately);
    RUN_TEST(testWrapper_IntegrityOfWrapperRegistration);

    // Finalize and return the test results
    return UNITY_END();
}