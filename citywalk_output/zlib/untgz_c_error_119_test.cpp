// Test suite for the focal method error(const char *msg) in untgz.c
// - Uses a custom test harness (no gtest/gmock)
// - Overrides the process exit to allow testing of exit(1) behavior
// - Captures stderr output to verify the exact printed message
// - Demonstrates multiple scenarios (different prog values and messages)

#include <cstdio>
#include <cstring>
#include <string.h>
#include <setjmp.h>
#include <errno.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>


// Forward declare the focal function and its global 'prog' symbol from the C file.
// The error function has C linkage in the original untgz.c.
extern "C" void error(const char *msg);
extern "C" char *prog;

// Provide a definition for the external 'prog' so tests can mutate it.
char *prog = nullptr;

// Jump buffer and exit status used to simulate/ intercept exit(1)
static jmp_buf g_jmpbuf;
static int g_exit_status = 0;

// Override the C library exit function to intercept termination without
// terminating the test runner. This function must have C linkage.
extern "C" int exit(int status) {
    // Record the exit status and long jump back to the test harness
    g_exit_status = status;
    longjmp(g_jmpbuf, 1);
    // Unreachable, but keeps compiler happy
    return status;
}

// Helper: Run a single test for error(msg) with given prog_name and message.
// Captures what is written to stderr and returns whether the exit occurred with status 1.
// Outputs the captured stderr to 'captured_output' for assertion by the test.
bool run_error_test(const char* prog_name, const char* message, std::string& captured_output) {
    // Set up a pipe to capture stderr
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        // Pipe creation failed; report as error (no termination of test suite)
        captured_output.clear();
        return false;
    }

    // Save current stderr and redirect it to the pipe's write end
    int old_stderr = dup(2);
    if (old_stderr == -1) {
        close(pipe_fd[0]); close(pipe_fd[1]);
        captured_output.clear();
        return false;
    }
    dup2(pipe_fd[1], 2); // Redirect stderr to pipe
    close(pipe_fd[1]);  // Close unused end in test harness; write end is now 2 (stderr)

    // Initialize program name for the focal function
    prog = (char*)prog_name;

    // Reset exit status and jump buffer
    g_exit_status = -1;

    // Test: call error(msg) and expect a long jump due to exit(1)
    int jmp_ret = setjmp(g_jmpbuf);
    if (jmp_ret == 0) {
        // First time: call the focal function. It should print to stderr and exit(1),
        // which will be intercepted by our overridden exit() and cause a longjmp.
        error(message);
        // If control returns here, the function did not exit as expected.
        // Restore file descriptors before returning
        dup2(old_stderr, 2);
        close(old_stderr);
        return false;
    } else {
        // We returned here from longjmp() invoked by our overridden exit()
        // Read what was written to stderr through the pipe
        // Ensure the read end is open
        int read_fd = pipe_fd[0];
        // Read available data (non-blocking fallback; the producer should have written)
        char buffer[2048];
        ssize_t len = 0;
        // Use a small loop to drain the pipe (in case of partial writes)
        ssize_t total = 0;
        while ((len = read(read_fd, buffer + total, sizeof(buffer) - total - 1)) > 0) {
            total += len;
            if (total >= (ssize_t)sizeof(buffer) - 1) break;
        }
        // Null-terminate
        buffer[total] = '\0';
        captured_output.assign(buffer);

        // Restore file descriptors
        dup2(old_stderr, 2);
        close(old_stderr);
        close(read_fd);
        // Verify that exit() was invoked with status 1
        return (g_exit_status == 1);
    }
}

// Simple helper to print test result with a conventional format
void print_test_result(const char* test_name, bool pass, const std::string& expected, const std::string& actual) {
    if (pass) {
        printf("[PASS] %s\n", test_name);
    } else {
        printf("[FAIL] %s\n", test_name);
        printf("  Expected: \"%s\"\n", expected.c_str());
        printf("  Actual  : \"%s\"\n", actual.c_str());
    }
}

// Entry point: run a set of focused tests for the error() function
int main() {
    // Test 1: Basic usage - prog is "myprog" and message is "hello"
    {
        std::string captured;
        bool ok = run_error_test("myprog", "hello", captured);
        const std::string expected = "myprog: hello\n";
        print_test_result("error() prints correct message and exits (prog='myprog', msg='hello')",
                          ok && captured == expected, expected, captured);
    }

    // Test 2: Empty message - ensure formatting still prints prog and colon
    {
        std::string captured;
        bool ok = run_error_test("myprog", "", captured);
        const std::string expected = "myprog: \n";
        print_test_result("error() prints with empty message (prog='myprog', msg='')",
                          ok && captured == expected, expected, captured);
    }

    // Test 3: Different program name to verify dynamic program name handling
    {
        std::string captured;
        bool ok = run_error_test("otherprog", "status", captured);
        const std::string expected = "otherprog: status\n";
        print_test_result("error() prints with alternative prog name (prog='otherprog', msg='status')",
                          ok && captured == expected, expected, captured);
    }

    // Summary (basic): If all tests passed, return 0; otherwise return non-zero
    // Note: We can't easily compute a total here without a more elaborate harness;
    // we approximate by using the results of the three tests above.
    // Since print_test_result already reports pass/fail, we can return success only if all passed.
    // Re-run tests to accumulate status
    bool all_pass = true;
    {
        std::string c;
        if (!run_error_test("myprog", "hello", c) || c != "myprog: hello\n") all_pass = false;
    }
    {
        std::string c;
        if (!run_error_test("myprog", "", c) || c != "myprog: \n") all_pass = false;
    }
    {
        std::string c;
        if (!run_error_test("otherprog", "status", c) || c != "otherprog: status\n") all_pass = false;
    }

    if (all_pass) {
        printf("[SUMMARY] ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("[SUMMARY] SOME TESTS FAILED\n");
        return 1;
    }
}