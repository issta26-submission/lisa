/*
  Test suite for the focal method: error(const char *msg)

  Context:
  - The function prints "<prog>: <msg>\n" to stderr and returns 0.
  - It relies on a global symbol `prog` (a const char* to the program name).
  - We provide tests in C++11 (no GoogleTest). A lightweight harness is used.

  Notes:
  - We declare extern "C" for the function under test to link correctly with the C implementation.
  - We define the global symbol `prog` in this test file to control what is printed.
  - Output capture is performed by temporarily redirecting stderr to a pipe, invoking error, then restoring state and reading the captured bytes.
  - Each test case is designed to exercise different inputs for msg and prog to ensure deterministic behavior.
  - The tests aim for non-terminating assertions via a simple PASS/FAIL reporting to maximize coverage without aborting on failure.
*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <inttypes.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>


// Forward declaration of the focal C-style function.
// It's defined in the provided minigzip_fuzzer.c (or equivalent) in the project.
extern "C" int error(const char* msg);
// The global program name used by the focal function.
extern "C" const char* prog;

// Provide a definition for `prog` so the linker has a symbol to bind to.
// Tests will mutate this pointer to simulate different program names.
const char* prog = "unit_test_prog"; // initial default

// ---------------- Test Utilities ----------------

// Capture helper: redirects stderr to a pipe, calls error(msg), restores state,
// and returns the captured output along with the return value of error(msg).
static std::string capture_error_output(int &retVal, const char* message) {
    // Create a pipe for capturing stderr output
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, set a sentinel and return empty output
        retVal = -1;
        return "";
    }

    // Save the current stderr file descriptor
    int saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        // Cleanup and return
        close(pipefd[0]);
        close(pipefd[1]);
        retVal = -1;
        return "";
    }

    // Redirect stderr to the write end of the pipe
    if (dup2(pipefd[1], STDERR_FILENO) == -1) {
        // Cleanup and return
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stderr);
        retVal = -1;
        return "";
    }

    // We no longer need the original write end after dup2
    close(pipefd[1]);

    // Invoke the focal function
    retVal = error(message);

    // Ensure buffered data is flushed to the pipe
    fflush(stderr);

    // Close STDERR to signal EOF to the read end of the pipe
    // This is important to unblock the reader after the writer has finished
    if (close(STDERR_FILENO) == -1) {
        // Restore state afterwards
        dup2(saved_stderr, STDERR_FILENO);
        close(saved_stderr);
        close(pipefd[0]);
        return "";
    }

    // Read the captured data from the read end of the pipe
    std::string captured;
    char buf[1024];
    ssize_t n;
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
        captured.append(buf, static_cast<size_t>(n));
    }

    // Restore original stderr
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    // Cleanup
    close(pipefd[0]);

    return captured;
}

// Simple assertion helper (non-terminating)
static void assert_true(bool cond, const char* testDesc, int &passed, int &failed) {
    if (cond) {
        ++passed;
    } else {
        ++failed;
        std::cerr << "Test failed: " << testDesc << std::endl;
    }
}

// ---------------- Unit Tests ----------------

// Test 1: Ensure that error("hello") prints "<prog>: hello\n" and returns 0
static void test_error_prints_expected_with_message(int &passed, int &failed) {
    // Arrange
    const char* oldProg = prog;
    prog = "testprog1";

    // Act
    int retVal;
    std::string output = capture_error_output(retVal, "hello");

    // Assert
    bool ok_ret = (retVal == 0);
    bool ok_out = (output == "testprog1: hello\n");

    assert_true(ok_ret, "error should return 0 for message 'hello'", passed, failed);
    assert_true(ok_out, "error should print 'testprog1: hello' to stderr", passed, failed);

    // Cleanup
    prog = oldProg;
    // Note: retVal and output are local to this test; no persistent state modified beyond prog
}

// Test 2: Ensure that error with an empty message still prints program name followed by colon and newline
static void test_error_prints_with_empty_message(int &passed, int &failed) {
    // Arrange
    const char* oldProg = prog;
    prog = "empty_msg_prog";

    // Act
    int retVal;
    std::string output = capture_error_output(retVal, "");

    // Assert
    bool ok_ret = (retVal == 0);
    bool ok_out = (output == "empty_msg_prog: \n");

    assert_true(ok_ret, "error should return 0 for empty message", passed, failed);
    assert_true(ok_out, "error should print 'empty_msg_prog: \\n' for empty message", passed, failed);

    // Cleanup
    prog = oldProg;
}

// Test 3: Ensure that changing prog affects the printed prefix (different msg also tested)
static void test_error_affects_prog_prefix(int &passed, int &failed) {
    // Arrange
    const char* oldProg = prog;
    prog = "prog_A";

    // Act
    int retVal;
    std::string outputA = capture_error_output(retVal, "alpha");

    prog = "prog_B";
    int retVal2;
    std::string outputB = capture_error_output(retVal2, "beta");

    // Assert
    bool ok_retA = (retVal == 0);
    bool ok_outA = (outputA == "prog_A: alpha\n");

    bool ok_retB = (retVal2 == 0);
    bool ok_outB = (outputB == "prog_B: beta\n");

    assert_true(ok_retA && ok_outA, "error should reflect 'prog_A' prefix for first call", passed, failed);
    assert_true(ok_retB && ok_outB, "error should reflect 'prog_B' prefix for second call", passed, failed);

    // Cleanup
    prog = oldProg;
}

// ---------------- Test Runner ----------------

int main() {
    int passed = 0;
    int failed = 0;

    std::cout << "Starting unit tests for focal method: int error(const char *msg)" << std::endl;

    test_error_prints_expected_with_message(passed, failed);
    test_error_prints_with_empty_message(passed, failed);
    test_error_affects_prog_prefix(passed, failed);

    std::cout << "Tests completed. Passed: " << passed << ", Failed: " << failed << std::endl;

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}