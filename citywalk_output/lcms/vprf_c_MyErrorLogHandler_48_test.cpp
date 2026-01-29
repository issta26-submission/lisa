/*
Unit test suite for the focal method MyErrorLogHandler located in vprf.c.

Key goals:
- Verify true/false branches of the verbose logging predicate (Verbose >= 0).
- Validate correct formatting of the log line using ProgramName and Text.
- Ensure no output is produced when Verbose < 0.
- Validate multiple invocations produce multiple log lines.
- Run tests without GTest, using a simple in-file test harness.
- Use only C++11 standard library plus provided external interfaces.

Notes:
- The test harness provides minimal external definitions required by the focal C function:
  - cmsContext and cmsUInt32Number typedefs (as compatible C++ types).
  - extern "C" declaration for MyErrorLogHandler.
  - External/global variables Verbose and ProgramName (with test-provided definitions).
- stderr is captured using a POSIX pipe to validate the exact log output.
- All tests are executed from main(); assertion style is non-terminating: failures are recorded and reported, but do not abort execution.
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <utils.h>


// The focal function is defined in vprf.c (C code). We declare it with C linkage.
extern "C" {

// Typedefs assumed by vprf.c (mapped to compatible C++ types for linking)
typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;

// Focal function under test
void MyErrorLogHandler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text);
}

// External variables used by the focal function (must be provided by test harness)
extern int Verbose;
extern const char* ProgramName;

// Provide definitions for dependencies used by the focal function.
// These definitions satisfy linkage for the test binary.
int Verbose = -1;                 // Default to not printing
const char* ProgramName = "TestProg"; // Default program name used in log formatting

// ---------------------------------------------------------------------------
// Test harness utilities
// ---------------------------------------------------------------------------

// Simple test state
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Helper to report test results (non-terminating)
static void report_test_result(const char* test_name, bool passed, const char* details = "")
{
    ++g_tests_run;
    if (passed) {
        std::cout << "TEST PASSED: " << test_name << std::endl;
    } else {
        ++g_tests_failed;
        std::cerr << "TEST FAILED: " << test_name;
        if (details && std::strlen(details) > 0) {
            std::cerr << " -- " << details;
        }
        std::cerr << std::endl;
    }
}

// Capture all output written to stderr during the execution of a callable sequence.
// The caller provides the text(s) to be logged by MyErrorLogHandler.
static std::string capture_stderr_for_calls(int verbose_setting, const char* texts[], int text_count)
{
    // Set verbose level for the test
    Verbose = verbose_setting;

    // Prepare pipes to capture stderr
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return "";
    }

    // Save original stderr and redirect to pipe write end
    int saved_stderr = dup(fileno(stderr));
    if (saved_stderr == -1) {
        perror("dup");
        return "";
    }

    // Redirect stderr to pipe write end
    fflush(stderr);
    if (dup2(pipefd[1], fileno(stderr)) == -1) {
        perror("dup2");
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }
    // Close the original write end descriptor as it's now duplicated to stderr
    close(pipefd[1]);

    // Execute the provided text logging(s)
    for (int i = 0; i < text_count; ++i) {
        const char* t = texts[i];
        // Call the focal function with a benign context and error code
        MyErrorLogHandler(nullptr, 0, t);
    }

    // Ensure all output is flushed and restore stderr
    fflush(stderr);
    if (dup2(saved_stderr, fileno(stderr)) == -1) {
        perror("dup2 restore");
        close(saved_stderr);
        close(pipefd[0]);
        return "";
    }
    close(saved_stderr);

    // Read from the pipe the captured output
    std::string captured;
    char buffer[1024];
    ssize_t n;
    // After restoring, read until EOF
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        captured.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return captured;
}

// Convenience: capture output for a single log call
static std::string capture_single_log(int verbose_setting, const char* text)
{
    const char* texts[1] = { text };
    return capture_stderr_for_calls(verbose_setting, texts, 1);
}

// Convenience: capture output for two log calls in a single capture
static std::string capture_two_logs(int verbose_setting, const char* text1, const char* text2)
{
    const char* texts[2] = { text1, text2 };
    return capture_stderr_for_calls(verbose_setting, texts, 2);
}

// ---------------------------------------------------------------------------
// Test cases
// ---------------------------------------------------------------------------

// Test 1: When Verbose >= 0, the log line should be printed with the correct format.
// Verifies ProgramName and Text are embedded in the log output.
static void test_verbose_prints_log_line()
{
    const char* test_text = "Hello, World!";
    // Ensure verbose is enabled
    int verbose_setting = 0;

    std::string expected = std::string("[") + ProgramName + "]: " + test_text + "\n";

    // Run capture for a single log call
    std::string captured = capture_single_log(verbose_setting, test_text);

    bool pass = (captured == expected);
    std::string details;
    if (!pass) {
        details = "Expected: " + expected + "Actual: " + captured;
    }

    report_test_result("test_verbose_prints_log_line", pass, details.c_str());
}

// Test 2: When Verbose < 0, no output should be produced to stderr.
// Verifies that the early return path is exercised.
static void test_no_log_when_verbose_negative()
{
    const char* test_text = "Should not be printed";
    int verbose_setting = -1;

    std::string captured = capture_single_log(verbose_setting, test_text);

    bool pass = captured.empty();
    std::string details = captured.empty() ? "" : ("Unexpected output: " + captured);
    report_test_result("test_no_log_when_verbose_negative", pass, details.c_str());
}

// Test 3: Multiple log calls in a single capture should produce multiple lines.
// Verifies that subsequent calls continue to print in the expected format.
static void test_multiple_logs_produce_multiple_lines()
{
    const char* text1 = "First line";
    const char* text2 = "Second line";
    int verbose_setting = 0;

    std::string expected = std::string("[") + ProgramName + "]: " + text1 + "\n"
                       + std::string("[") + ProgramName + "]: " + text2 + "\n";

    std::string captured = capture_two_logs(verbose_setting, text1, text2);

    bool pass = (captured == expected);
    std::string details;
    if (!pass) {
        details = "Expected:\n" + expected + "Actual:\n" + captured;
    }

    report_test_result("test_multiple_logs_produce_multiple_lines", pass, details.c_str());
}

// Optional: Touch a test to ensure static-like behavior expectations (no private members in this C file).
// This test ensures the test harness exercises static-like usage by simulating repeated calls.
// Note: The code under test uses static-like behavior inside the C file; we rely on repeated calls test above.
// Keeping this as a placeholder demonstrates coverage intent without private-access requirements.
static void test_repeated_calls_do_not_crash()
{
    int verbose_setting = 0;
    const char* texts[2] = { "Repeat 1", "Repeat 2" };
    // Single capture for two calls to exercise state across multiple invocations
    std::string captured = capture_stderr_for_calls(verbose_setting, texts, 2);
    // We expect two lines corresponding to both messages
    std::string expected = std::string("[") + ProgramName + "]: " + texts[0] + "\n"
                       + std::string("[") + ProgramName + "]: " + texts[1] + "\n";
    bool pass = (captured == expected);
    report_test_result("test_repeated_calls_do_not_crash", pass,
                       pass ? "" : ("Expected:\n" + expected + "Actual:\n" + captured).c_str());
}

// ---------------------------------------------------------------------------
// Main: run all tests
// ---------------------------------------------------------------------------

int main()
{
    // Run test cases
    test_verbose_prints_log_line();
    test_no_log_when_verbose_negative();
    test_multiple_logs_produce_multiple_lines();
    test_repeated_calls_do_not_crash();

    // Summary
    std::cout << "Total tests run: " << g_tests_run << std::endl;
    std::cout << "Total tests failed: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}