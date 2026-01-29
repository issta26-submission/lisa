/*
Goal:
- Provide a self-contained C++11 test harness for the focal method test_ThisTestAlwaysFails
  defined in testRunnerGeneratorWithMocks.c, using a macro override strategy to capture the
  failure message instead of terminating the test prematurely.
- The harness includes the focal file content with the TEST_FAIL_MESSAGE macro redefined
  to record the message. This allows verification of the message content and the call count
  without requiring a full Unity test runner.
- This approach adheres to the constraint of not using Google Test and provides explanatory
  comments for each unit-test-like check performed in main().
- The code is designed to compile under a C++11 compiler and relies on the project headers
  present in the provided snippet (Defs.h, mockMock.h, etc.). It directly includes the focal
  C source to override macro behavior during compilation.
*/

// Include standard C++ headers for input/output and string handling
#include <unity.h>
#include <cstring>
#include <testRunnerGeneratorWithMocks.c>
#include <cassert>
#include <mockMock.h>
#include <cstdio>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


// Static storage to capture the failure message from the focal test when TEST_FAIL_MESSAGE is overridden.
static const char* g_failed_message = nullptr;
static int g_failure_count = 0;

// Macro override: redirect TEST_FAIL_MESSAGE to record the message instead of terminating the test.
// This macro is defined before including the focal source so that all uses inside the focal method
// get redirected accordingly.
#define TEST_FAIL_MESSAGE(MSG) do { g_failed_message = MSG; g_failure_count++; } while(0)

// Expose the focal source content with the macro override in C linkage to avoid name mangling issues
extern "C" {

// The focal test and its dependent stub functions live in this included file.
// By redefining TEST_FAIL_MESSAGE above, calls to TEST_FAIL_MESSAGE within test_ThisTestAlwaysFails
// will instead populate g_failed_message and increment g_failure_count.

} // extern "C"

int main(void) {
    // Unit Test 1: Verify that test_ThisTestAlwaysFails triggers the expected failure message
    //               and that the failure counter increments exactly once for this invocation.
    // Reset captured state
    g_failed_message = nullptr;
    g_failure_count = 0;

    // Execute the focal test function
    test_ThisTestAlwaysFails();

    // Check that exactly one failure was captured and that the message matches the expected one.
    bool unit1_passed = (g_failure_count == 1) &&
                        (g_failed_message != nullptr) &&
                        (std::strcmp(g_failed_message, "This Test Should Fail") == 0);

    if (unit1_passed) {
        std::printf("Unit Test 1 PASSED: Captured expected failure message: '%s'\n", g_failed_message);
    } else {
        std::printf("Unit Test 1 FAILED: Expected message 'This Test Should Fail', "
                    "got '%s' (count=%d)\n",
                    (g_failed_message ? g_failed_message : "NULL"),
                    g_failure_count);
        return 1; // Non-zero return indicates test failure
    }

    // Unit Test 2: Re-run the focal test to ensure deterministic behavior of the capture mechanism.
    // Reset captured state again
    g_failed_message = nullptr;
    g_failure_count = 0;

    // Execute the focal test function again
    test_ThisTestAlwaysFails();

    // Verify the same outcome as Unit Test 1
    bool unit2_passed = (g_failure_count == 1) &&
                        (g_failed_message != nullptr) &&
                        (std::strcmp(g_failed_message, "This Test Should Fail") == 0);

    if (unit2_passed) {
        std::printf("Unit Test 2 PASSED: Re-run produced the same failure message as Unit Test 1.\n");
        return 0; // Success
    } else {
        std::printf("Unit Test 2 FAILED: Expected message 'This Test Should Fail', "
                    "got '%s' (count=%d)\n",
                    (g_failed_message ? g_failed_message : "NULL"),
                    g_failure_count);
        return 1; // Failure
    }
}