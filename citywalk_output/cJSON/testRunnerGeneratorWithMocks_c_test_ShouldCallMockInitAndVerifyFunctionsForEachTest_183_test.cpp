/*
 * Test Suite: test_MockLifecycleSuite.c
 * Purpose:
 *   Provide a focused set of unit tests for the focal method:
 *     test_ShouldCallMockInitAndVerifyFunctionsForEachTest(void)
 *   located in testRunnerGeneratorWithMocks.c.
 *   The suite manipulates the Unity framework state and the mock counters
 *   to exercise both the normal (passing) path and various failing paths.
 *
 * Notes:
 *   - This file assumes Unity (unity.h) and the mock counters are available
 *     from the linked project (Defs.h, mockMock.h, etc.).
 *   - We rely on the focal method to perform internal assertions via Unity
 *     macros. The tests here only drive the counters/state and invoke the focal
 *     method to validate its behavior.
 *   - The benchmarked function is linked from the separate C file and must be
 *     compiled and linked together with this test suite.
 */

// Include Unity framework and project headers
#include <unity.h>
#include <mockMock.h>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


#ifdef USE_CEXCEPTION
#endif

// Forward declaration of the focal method under test (defined in testRunnerGeneratorWithMocks.c)
extern void test_ShouldCallMockInitAndVerifyFunctionsForEachTest(void);

/*
 * Helper: set up empty setUp/tearDown required by Unity.
 * (These can be no-ops for this focused suite.)
 */
void setUp(void)   { /* no setup required for these tests */ }
void tearDown(void) { /* no teardown required for these tests */ }

/*
 * Test 1: Normal scenario where all counters align with Unity's expectations.
 * Expectation:
 *   - mockMock_Init_Counter == Unity.NumberOfTests
 *   - mockMock_Verify_Counter == (Unity.NumberOfTests - Unity.TestFailures)
 *   - mockMock_Destroy_Counter == (Unity.NumberOfTests - 1)
 *   - CMockMemFreeFinalCounter == 0
 * This should pass and exercise the "happy path" of the focal method.
 */
void test_NormalScenario_CountersAlign(void)
{
    // Configure Unity state
    Unity.NumberOfTests = 5;
    Unity.TestFailures = 1; // so passesOrIgnores == 4

    // Configure mock counters to match the expectations derived in the focal method
    mockMock_Init_Counter      = 5; // Unity.NumberOfTests
    mockMock_Verify_Counter    = 4; // Unity.NumberOfTests - Unity.TestFailures
    mockMock_Destroy_Counter   = 4; // Unity.NumberOfTests - 1
    CMockMemFreeFinalCounter    = 0; // must remain 0 until end

    // Invoke the focal method
    test_ShouldCallMockInitAndVerifyFunctionsForEachTest();
}

/*
 * Test 2: Init counter mismatch should cause a failure in the focal method.
 * Intent: Ensure the first assertion (Init counter) detects mismatch.
 */
void test_InitCounterMismatch_ShouldFail(void)
{
    Unity.NumberOfTests = 3;
    Unity.TestFailures = 0; // passesOrIgnores == 3

    mockMock_Init_Counter    = 2; // mismatch on purpose
    mockMock_Verify_Counter  = 3;
    mockMock_Destroy_Counter = 2;
    CMockMemFreeFinalCounter  = 0;

    test_ShouldCallMockInitAndVerifyFunctionsForEachTest();
    // This test should fail inside the focal method due to Init counter mismatch.
}

/*
 * Test 3: Verify counter mismatch should cause a failure in the focal method.
 * Intent: Ensure the second assertion (Verify counter) detects mismatch.
 */
void test_VerifyCounterMismatch_ShouldFail(void)
{
    Unity.NumberOfTests = 4;
    Unity.TestFailures = 0; // passesOrIgnores == 4

    mockMock_Init_Counter    = 4;
    mockMock_Verify_Counter  = 3; // mismatch here
    mockMock_Destroy_Counter = 3;
    CMockMemFreeFinalCounter  = 0;

    test_ShouldCallMockInitAndVerifyFunctionsForEachTest();
    // This test should fail inside the focal method due to Verify counter mismatch.
}

/*
 * Test 4: Destroy counter mismatch should cause a failure in the focal method.
 * Intent: Ensure the third assertion (Destroy counter) detects mismatch.
 */
void test_DestroyCounterMismatch_ShouldFail(void)
{
    Unity.NumberOfTests = 5;
    Unity.TestFailures = 0; // passesOrIgnores == 5

    mockMock_Init_Counter      = 5;
    mockMock_Verify_Counter    = 5;
    mockMock_Destroy_Counter   = 3; // mismatch here
    CMockMemFreeFinalCounter    = 0;

    test_ShouldCallMockInitAndVerifyFunctionsForEachTest();
    // This test should fail inside the focal method due to Destroy counter mismatch.
}

/*
 * Test 5: MemFreeFinal counter should remain zero until the end.
 * Intent: Ensure the final assertion detects that MemFreeFinal should not be called prematurely.
 */
void test_MemFreeFinalCounterNotZero_ShouldFail(void)
{
    Unity.NumberOfTests = 2;
    Unity.TestFailures = 0; // passesOrIgnores == 2

    mockMock_Init_Counter     = 2;
    mockMock_Verify_Counter   = 2;
    mockMock_Destroy_Counter  = 1;
    CMockMemFreeFinalCounter   = 1; // premature non-zero

    test_ShouldCallMockInitAndVerifyFunctionsForEachTest();
    // This test should fail inside the focal method due to MemFreeFinalCounter != 0.
}

/*
 * Main entry point to run the test suite.
 * Each RUN_TEST associates a test function with Unity's test runner.
 */
int main(void)
{
    UnityBegin("test_MockLifecycleSuite.c");

    RUN_TEST(test_NormalScenario_CountersAlign);
    RUN_TEST(test_InitCounterMismatch_ShouldFail);
    RUN_TEST(test_VerifyCounterMismatch_ShouldFail);
    RUN_TEST(test_DestroyCounterMismatch_ShouldFail);
    RUN_TEST(test_MemFreeFinalCounterNotZero_ShouldFail);

    return UnityEnd();
}