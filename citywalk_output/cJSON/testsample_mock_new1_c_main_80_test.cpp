// test_main_suite.cpp
// Purpose: Provide a focused test harness for the focal main() workflow
// defined in testsample_mock_new1.c. This harness drives the two primary
// Unity-based tests that the focal main expects to invoke.
//
// Notes:
// - This file is C++11 compatible and links against the existing Unity/CMock
//   infrastructure used by the project (unity.h, cmock.h, etc.).
// - The actual test functions test_TheFirstThingToTest and
//   test_TheSecondThingToTest are assumed to be defined elsewhere (as in the
//   original testsample_mock_new1.c or associated test sources). We declare them
//   here as extern "C" so the linker can resolve them correctly when building
//   the binary.
// - The harness mirrors the structure of the focal main: begin Unity, run the
//   two tests with their respective line numbers, free memory, and end Unity.

#include <unity.h>
#include <setjmp.h>
#include <cmock.h>
#include <funky.h>
#include <one.h>
#include <two.h>
#include <iostream>
#include <CException.h>
#include <Mockstanky.h>
#include <stdio.h>


// Ensure C headers are linked with C linkage when included in C++
extern "C" {
}

// Prototypes for the two test functions that the focal main would invoke.
// These are expected to be defined in the project (likely in the same test suite
// or in testsample_mock_new1.c). Declared as extern "C" to avoid C++ name mangling.
extern "C" void test_TheFirstThingToTest(void);
extern "C" void test_TheSecondThingToTest(void);

// Helper function to run the entire test suite that the focal main would run.
// This mimics the structure inside testsample_mock_new1.c's main(), but from C++.
static int run_all_tests(void)
{
  // Begin a Unity test session for the test data file that the focal main references.
  // This aligns with the focal main's UnityBegin("testdata/mocksample.c");
  UnityBegin("testdata/mocksample.c");

  // Execute the first test case as defined by the focal main.
  // The numeric argument corresponds to the original TestLineNum usage in RUN_TEST.
  // Comment: This verifies the first major behavior/branch covered by TheFirstThingToTest.
  RUN_TEST(test_TheFirstThingToTest, 21);

  // Execute the second test case as defined by the focal main.
  // Comment: This validates the secondary behavior/branch covered by TheSecondThingToTest.
  RUN_TEST(test_TheSecondThingToTest, 43);

  // Finalize and free any mock globals/memory used during tests.
  CMock_Guts_MemFreeFinal();

  // End Unity test session and return the result code.
  return UnityEnd();
}

int main(void)
{
  // Entry point for the C++ test binary. Delegates to the Unity-based runner.
  // This keeps the test harness self-contained and executable without modifying
  // the original testsample_mock_new1.c main.
  return run_all_tests();
}

// Explanatory notes for future maintainers:
// - If you introduce additional test scenarios for the focal main, extend this file
//   by declaring additional extern "C" test functions (e.g., test_TheThirdThingToTest)
//   and add corresponding RUN_TEST invocations within run_all_tests(), ensuring
//   line number consistency with the original test suite's expectations.
// - Ensure the included headers (unity.h, cmock.h, etc.) are available on the
//   include path and that the build links against the C runtime as required by Unity.
// - This harness maintains compatibility with C test doubles/mocks (Mockstanky.h, etc.)
//   via C linkage, avoiding C++ name mangling issues.