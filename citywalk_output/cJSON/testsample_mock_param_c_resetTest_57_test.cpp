/*
  Test Suite: resetTest() - testsample_mock_param.c

  Purpose:
  - Exercise the focal method resetTest(void) from testsample_mock_param.c.
  - Since internal CMock_Init/Verify/Destroy are implemented as static helpers in the
    focal file, external observation of their exact call order cannot be asserted
    from another translation unit. Therefore, the tests focus on ensuring that
    resetTest can be invoked without crashing and can be called repeatedly without
    side effects.

  Approach rationale (mapped to Candidate Keywords from Step 1):
  - resetTest: The function under test.
  - CMock_Verify / CMock_Destroy: Internal static helpers invoked by resetTest.
  - tearDown / setUp: Test lifecycle hooks invoked by resetTest in a fixed order.
  - Static scope: Acknowledgement that internal helpers are static and not directly observable.
  - Observability: We cannot observe internal order due to static functions; instead, we
    exercise the API surface and ensure stability across calls.

  Design Notes (Domain Knowledge considerations applied):
  - Use only standard library facilities (i.e., iostream for output in this C++ test driver).
  - No GTest; a lightweight, self-contained test driver is provided (as per Step 3 guidance).
  - The test driver is written in C++11 and invokes the C-based focal function via extern "C".
  - Tests are designed to be executable from main() with clear, commented output.

  How to build (conceptual):
  - Compile testsample_mock_param.c with a C compiler as part of the test build.
  - Compile this test driver (this file) with a C++11 compiler and link both objects together.

  Limitations:
  - Direct verification of the exact order of internal calls (Verify, Destroy, tearDown, Init, setUp)
    is not feasible here due to static linkage of those helpers in the focal file.
  - The tests validate non-crashing behavior and repeatability.
*/

#include <unity.h>
#include <setjmp.h>
#include <cmock.h>
#include <funky.h>
#include <iostream>
#include <cstdlib>
#include <Mockstanky.h>
#include <stdio.h>


// Declaration of the focal function under test.
// It is defined in testsample_mock_param.c and compiled alongside this test.
extern "C" void resetTest(void);

/*
  Test 1: test_resetTest_runs_once_no_crash
  - Rationale: Calling resetTest once should not crash the program.
  - What it proves: The function can be invoked in isolation without producing runtime errors.
*/
void test_resetTest_runs_once_no_crash() {
    // Execute the focal function. If a crash occurs, the test run will terminate abnormally.
    // Because internal helper functions are static within testsample_mock_param.c,
    // we cannot observe their invocation order from here. We rely on successful return.
    std::cout << "Running test_resetTest_runs_once_no_crash...\n";
    resetTest();
    std::cout << "Completed: test_resetTest_runs_once_no_crash (no crash observed)\n";
}

/*
  Test 2: test_resetTest_runs_twice_no_crash
  - Rationale: Repeated invocation should also not crash and should be idempotent in terms of API usage.
  - What it proves: The focal function remains stable under consecutive calls.
*/
void test_resetTest_runs_twice_no_crash() {
    std::cout << "Running test_resetTest_runs_twice_no_crash (first call)...\n";
    resetTest();  // First invocation
    std::cout << "Running test_resetTest_runs_twice_no_crash (second call)...\n";
    resetTest();  // Second invocation
    std::cout << "Completed: test_resetTest_runs_twice_no_crash (two consecutive calls)\n";
}

/*
  Test Runner:
  - main() invokes the above tests in a simple sequential manner, suitable for environments
    where GTest or Unity test runners are not desired or available.
  - Explanatory comments accompany each test to clarify intent and observed limitations.
*/
int main() {
    // Run Test 1
    test_resetTest_runs_once_no_crash();

    // Run Test 2
    test_resetTest_runs_twice_no_crash();

    std::cout << "All tests executed. If no crash occurred, resetTest() handled both calls gracefully.\n";
    return 0;
}