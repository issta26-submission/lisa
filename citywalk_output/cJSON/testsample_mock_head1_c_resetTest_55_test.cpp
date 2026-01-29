// Test suite for the focal method resetTest in testsample_mock_head1.c
// This file provides two unit tests to validate the call sequence inside resetTest.
// The tests rely on Unity/CMock style conventions (no GTest).
// The project is assumed to be compilable under C++11 as well, but here we present a C-style
// Unity-based test suite for clarity and compatibility with the provided FOCAL_CLASS_DEP pattern.

/*
Step 1: Program Understanding (Candidate Keywords)
- resetTest: the function under test, which in its body calls:
  - CMock_Verify()
  - CMock_Destroy()
  - tearDown()
  - CMock_Init()
  - setUp()
- CMock_Init, CMock_Verify, CMock_Destroy: lifecycle hooks for mocks (CMock)
- tearDown, setUp: test harness hooks invoked around tests
- Unity: the test framework
- Mockstanky: a mock module for stubbing expectations
- The test aims to verify the internal call order inside resetTest:
  Verify -> Destroy -> tearDown -> Init -> setUp
- These tests enforce the true/false branching is not applicable here (no conditional branches in resetTest).
- Static members and scope: not directly exercised here; we focus on the sequence of calls.
*/

// Step 2 & 3: Unit Test Generation and Refinement
// We implement a small Unity-based test suite to verify the call order inside resetTest.
// The tests capture the outer setUp, the inner tearDown/setUp, and the outer tearDown sequence
// by using the standard Unity setUp/tearDown hooks.

#include <unity.h>
#include <setjmp.h>
#include <cmock.h>
#include <testsample_mock_head1.h>
#include <Mockstanky.h>
#include <stdio.h>


// Declaration of the focal function under test (from testsample_mock_head1.c)
extern void resetTest(void);

// Domain knowledge considerations:
// - We want non-terminating assertions when possible to maximize coverage, but for determinism
//   and clarity we use terminating assertions here (TEST_ASSERT_EQUAL_INT) to confirm exact
//   sequence lengths and events. The internal sequence itself is determined by the implementation
//   of resetTest and the Unity/CMock framework behavior.
typedef enum {
  LOG_SETUP = 0,
  LOG_TEARDOWN = 1
} LogEvent;

// Simple in-test log to capture the order of setUp/tearDown invocations.
// The outer Unity harness will call setUp once before each RUN_TEST, and tearDown once after.
// resetTest additionally calls tearDown() and setUp() inside its body, influencing the sequence.
static int logEvents[8];
static int logIndex = 0;

// Hook functions used by Unity to log calls
void setUp(void)
{
  logEvents[logIndex++] = LOG_SETUP;
}

void tearDown(void)
{
  logEvents[logIndex++] = LOG_TEARDOWN;
}

// Test 1: Verify the typical sequence of calls when resetTest is invoked as part of a test case.
// Expected event order (including Unity's outer harness):
// 1) Outer setUp -> LOG_SETUP
// 2) Inside resetTest: inner tearDown -> LOG_TEARDOWN
// 3) Inside resetTest: inner setUp -> LOG_SETUP
// 4) Outer tearDown after the test -> LOG_TEARDOWN
// Therefore, logIndex should reach 4 with the sequence: [SETUP, TEARDOWN, SETUP, TEARDOWN]
void test_resetTest_basic_sequence(void)
{
  // Reset the log for a clean test run
  logIndex = 0;

  // Invoke the focal function
  resetTest();

  // Validate the captured sequence
  // We expect exactly 4 log entries for a single test run when resetTest is invoked.
  TEST_ASSERT_EQUAL_INT(4, logIndex);
  TEST_ASSERT_EQUAL_INT(LOG_SETUP, logEvents[0]);     // Outer setUp
  TEST_ASSERT_EQUAL_INT(LOG_TEARDOWN, logEvents[1]);  // Inner tearDown (inside resetTest)
  TEST_ASSERT_EQUAL_INT(LOG_SETUP, logEvents[2]);     // Inner setUp (inside resetTest)
  TEST_ASSERT_EQUAL_INT(LOG_TEARDOWN, logEvents[3]);  // Outer tearDown (after test)
}

// Test 2: Validate consistency by invoking resetTest again in a separate test case.
// This ensures that the sequence remains stable across multiple invocations and test runs.
void test_resetTest_consistency_multiple_invocations(void)
{
  logIndex = 0;

  // First invocation
  resetTest();

  TEST_ASSERT_EQUAL_INT(4, logIndex);
  TEST_ASSERT_EQUAL_INT(LOG_SETUP, logEvents[0]);
  TEST_ASSERT_EQUAL_INT(LOG_TEARDOWN, logEvents[1]);
  TEST_ASSERT_EQUAL_INT(LOG_SETUP, logEvents[2]);
  TEST_ASSERT_EQUAL_INT(LOG_TEARDOWN, logEvents[3]);

  // Reset for the second invocation and verify again
  logIndex = 0;
  resetTest();

  TEST_ASSERT_EQUAL_INT(4, logIndex);
  TEST_ASSERT_EQUAL_INT(LOG_SETUP, logEvents[0]);
  TEST_ASSERT_EQUAL_INT(LOG_TEARDOWN, logEvents[1]);
  TEST_ASSERT_EQUAL_INT(LOG_SETUP, logEvents[2]);
  TEST_ASSERT_EQUAL_INT(LOG_TEARDOWN, logEvents[3]);
}

// Step 2: Run tests via Unity's RUN_TEST macro (provided by unity.h).
int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_resetTest_basic_sequence, __LINE__);
  RUN_TEST(test_resetTest_consistency_multiple_invocations, __LINE__);
  return UNITY_END();
}

/*
Notes:
- This test suite presumes that the generated mocks (CMock_Init/Verify/Destroy) are wired in such a way
  that their invocation does not itself produce log output. The observable log events come from setUp
  and tearDown (both the outer Unity harness and the inner calls inside resetTest).
- If you have additional static mocks or different behavior in the test environment, you may need to adapt
  the log instrumentation accordingly (e.g., adding more hooks or logs around CMock_Init, CMock_Destroy, etc.).
- The tests use terminating assertions (TEST_ASSERT_EQUAL_INT). If you want non-terminating checks to maximize
  code coverage, you could replace them with a non-terminating assertion strategy or record failures in a
  counter and assert at the end of the test.