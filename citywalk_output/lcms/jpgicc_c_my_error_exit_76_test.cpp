/*
  Unit test suite for the focal method: my_error_exit (j_common_ptr cinfo)
  Context:
  - The function formats an error message via cinfo->err->format_message and then calls FatalError(buffer).
  - We implement a minimal C++11 test harness (no Google Test) that mocks FatalError
    and provides two test scenarios to validate the interaction with the mocked
    format_message callback.
  - This test relies on libjpeg's type definitions exposed via jpeglib.h.
  - Tests are executed from main() and use non-terminating assertions (boolean checks).
*/

#include <cstring>
#include <iccjpeg.h>
#include <cstdio>
#include <jpeglib.h>
#include <iostream>
#include <string>
#include <utils.h>


// Include libjpeg types for structures used by the focal method

// Forward declaration of the focal method (C linkage)
extern "C" void my_error_exit (j_common_ptr cinfo);

// Global test state to capture FatalError invocations
static bool g_fatal_called = false;
static std::string g_lastFatalMessage;

// Overridden FatalError to intercept calls and capture the message without terminating the test binary
extern "C" void FatalError(const char* msg)
{
    g_fatal_called = true;
    g_lastFatalMessage = msg ? msg : "";
}

/*
  Test helpers:
  - Two format_message implementations to simulate different error messages produced by
    the error manager. They conform to the expected signature:
      void format_message(j_common_ptr cinfo, char* buffer)
*/
static void formatMessage1(j_common_ptr /*cinfo*/, char* buffer)
{
    // Write a deterministic test message into the provided buffer
    std::snprintf(buffer, JMSG_LENGTH_MAX, "Test message 1");
}

static void formatMessage2(j_common_ptr /*cinfo*/, char* buffer)
{
    std::snprintf(buffer, JMSG_LENGTH_MAX, "Test message 2");
}

/*
  Helper to reset test state before each test case
*/
static void reset_test_state()
{
    g_fatal_called = false;
    g_lastFatalMessage.clear();
}

/*
  Test 1: Basic flow
  - Configure the mock error manager to use formatMessage1.
  - Call my_error_exit and verify that FatalError was invoked with "Test message 1".
*/
static bool test_my_error_exit_basic()
{
    reset_test_state();

    // Prepare a minimal jpeg_common_struct with an error manager
    jpeg_common_struct cinfo;
    std::memset(&cinfo, 0, sizeof(cinfo));

    jpeg_error_mgr err;
    std::memset(&err, 0, sizeof(err));

    // Install the mock format_message implementation
    err.format_message = formatMessage1;

    // Link the error manager into the common structure
    cinfo.err = &err;

    // Call the focal method (C linkage)
    my_error_exit(reinterpret_cast<j_common_ptr>(&cinfo));

    // Validate that FatalError was called with the expected message
    bool passed = g_fatal_called && (g_lastFatalMessage == "Test message 1");
    return passed;
}

/*
  Test 2: Different message path
  - Configure the mock error manager to use formatMessage2.
  - Call my_error_exit and verify that FatalError was invoked with "Test message 2".
*/
static bool test_my_error_exit_with_second_message()
{
    reset_test_state();

    jpeg_common_struct cinfo;
    std::memset(&cinfo, 0, sizeof(cinfo));

    jpeg_error_mgr err;
    std::memset(&err, 0, sizeof(err));

    err.format_message = formatMessage2;
    cinfo.err = &err;

    my_error_exit(reinterpret_cast<j_common_ptr>(&cinfo));

    bool passed = g_fatal_called && (g_lastFatalMessage == "Test message 2");
    return passed;
}

/*
  Simple test harness
  - Runs defined tests from main(), reports pass/fail, and returns appropriate exit code.
  - Uses non-terminating checks; prints clear results for manual inspection.
*/
int main(int argc, char* argv[])
{
    (void)argc; (void)argv;

    int total = 0;
    int passed = 0;

    auto run = [&](bool ok, const char* testName){
        total++;
        if (ok) {
            std::cout << "[PASS] " << testName << "\n";
            passed++;
        } else {
            std::cout << "[FAIL] " << testName << "\n";
        }
    };

    run(test_my_error_exit_basic(), "test_my_error_exit_basic");
    run(test_my_error_exit_with_second_message(), "test_my_error_exit_with_second_message");

    std::cout << "Summary: " << passed << "/" << total << " tests passed.\n";

    // Non-terminating: return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}