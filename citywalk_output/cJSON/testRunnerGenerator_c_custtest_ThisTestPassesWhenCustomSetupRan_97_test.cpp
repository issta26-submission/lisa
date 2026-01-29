// test_custtest_suite.cpp
// C++11 compliant test harness for the focal Unity-based test:
//       custtest_ThisTestPassesWhenCustomSetupRan
//
// Notes:
// - This harness uses the Unity C unit testing framework to exercise the focal test.
// - The focal test asserts that CounterSetup equals 2, which is expected after
//   running the custom_setup() routine. We expose minimal wrappers to drive the
//   test scenarios from C++ while linking against the existing C test file
//   (testRunnerGenerator.c / class dependencies).
// - We do not use GoogleTest; we rely on Unity and standard C linkage for compatibility.

#include <unity.h>
#include <iostream>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


extern "C" {
  // Include Unity header and rely on Unity's testing API.
  #include "unity.h"

  // External C globals and routines that influence the focal test.
  extern int CounterSetup;          // The counter used by the focal test to verify setup ran
  void custom_setup(void);           // External custom setup function that should run before tests
  void custtest_ThisTestPassesWhenCustomSetupRan(void); // Focal test under test

  // Test wrappers to be registered with Unity's RUN_TEST.
  void test_PassWhenCustomSetupRan(void);
  void test_FailsWithoutCustomSetupRan(void);
}

// Wrapper: Ensure that when custom_setup() is executed prior to the focal test,
// the CounterSetup reaches the expected value (2) and the focal test passes.
// This simulates a correct "Custom Setup Ran" scenario.
extern "C" void test_PassWhenCustomSetupRan(void)
{
    // Reset state
    CounterSetup = 0;

    // Perform the custom setup as the test framework would, then run the focal test.
    custom_setup();

    // Execute the focal test. If CounterSetup != 2 after custom_setup(), Unity will
    // record a failure for this test via TEST_ASSERT_EQUAL_MESSAGE.
    custtest_ThisTestPassesWhenCustomSetupRan();
}

// Wrapper: Validate the negative scenario where custom_setup() is NOT invoked.
// The focal test is expected to fail in this case because CounterSetup would not be 2.
extern "C" void test_FailsWithoutCustomSetupRan(void)
{
    // Reset state
    CounterSetup = 0;

    // Intentionally do not call custom_setup() to simulate missing custom setup.
    // Run the focal test; it should fail the assertion if CounterSetup != 2.
    custtest_ThisTestPassesWhenCustomSetupRan();
}

// Main test runner for Unity
int main(void)
{
    // Begin Unity test run; the argument is the name of the test file or suite.
    UnityBegin("testRunnerGenerator.c");

    // Register tests with Unity's RUN_TEST macro
    // 1) Pass scenario: custom_setup executes and the focal test passes.
    RUN_TEST(test_PassWhenCustomSetupRan);

    // 2) Fail scenario: custom_setup is not executed and the focal test fails.
    RUN_TEST(test_FailsWithoutCustomSetupRan);

    // End Unity test run and return status
    return UnityEnd();
}