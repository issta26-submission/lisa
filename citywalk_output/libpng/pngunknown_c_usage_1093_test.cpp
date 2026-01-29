// Unit test suite for the focal method: usage(const char *program, const char *reason)
// This test suite is designed for a POSIX-like environment (uses fork/pipe) since
// the focal method prints to stderr and then calls exit(99). To verify behavior
// without terminating the test runner, tests are executed in a separate process.
// The suite uses a lightweight, non-terminating assertion style (log-and-continue).

#include <unistd.h>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <string>
#include <functional>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>


// The focal function is assumed to be provided by the project under test.
// We declare it with C linkage to match the original function signature.
extern "C" void usage(const char *program, const char *reason);

// Lightweight test harness (non-GTest, non-GMock, POSIX fork-based test isolation)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper macro: record a failed assertion with a message
#define LOG_FAIL(msg) do { std::cerr << "  [FAIL] " << (msg) << std::endl; ++g_failed_tests; } while(0)

// Helper macro: assert condition without terminating the test executable
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { LOG_FAIL(msg); } \
    ++g_total_tests; \
} while(0)

// Helper: perform the usage test in a separate process and capture stderr output.
// Returns true if the test could be performed (child process spawned and waited), false otherwise.
// Captured content from stderr is returned via 'captured', and the child exit status via 'exit_code'.
bool perform_usage_test(const std::string& program, const std::string& reason,
                        std::string& captured, int& exit_code)
{
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        std::cerr << "Failed to create pipe for IPC" << std::endl;
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        // Fork failed
        std::cerr << "Failed to fork for isolation" << std::endl;
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return false;
    }

    if (pid == 0) {
        // Child process: redirect stderr to pipe and invoke usage()
        dup2(pipe_fd[1], STDERR_FILENO);
        // Close unused ends
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        // Call the focal function. This will print to stderr and call exit(99).
        usage(program.c_str(), reason.c_str());

        // If, for some reason, usage returns, exit with a distinct status to indicate abnormality.
        _exit(77);
    }

    // Parent process
    // Close write end and read from read end
    close(pipe_fd[1]);
    char buffer[4096];
    captured.clear();
    ssize_t n;
    while ((n = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
        captured.append(buffer, (size_t)n);
    }
    close(pipe_fd[0]);

    int status = 0;
    if (waitpid(pid, &status, 0) != pid) {
        std::cerr << "Failed to wait for child process" << std::endl;
        return false;
    }

    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        // Child terminated due to a signal; treat as test failure to avoid false positives
        exit_code = -1;
    } else {
        exit_code = -1;
    }

    // The test could be performed
    return true;
}

// Test 1: Basic usage message contains program and reason
bool test_usage_basic()
{
    // Heaven-case: standard tokens
    const std::string program = "mytestprog";
    const std::string reason = "basic-reason";

    std::string captured;
    int exit_code = -100;

    bool ok = perform_usage_test(program, reason, captured, exit_code);
    ASSERT_TRUE(ok, "Failed to perform usage test (IPC or fork failure)");

    // Validate exit code is exactly 99 as mandated by usage()
    if (exit_code != 99) {
        LOG_FAIL("Usage test: expected exit code 99, got " << exit_code);
        ok = false;
    }

    // Validate that the emitted message contains the known parts
    // The message format (from the focal method) is:
    // "pngunknown: %s: usage:\n %s [--strict] --default|{(CHNK|default|all)=(default|discard|if-safe|save)} testfile.png\n"
    // We verify that 'pngunknown', the reason, and the program appear in the output.
    if (captured.find("pngunknown: " + reason + ": usage:\n " + program) == std::string::npos) {
        LOG_FAIL("Usage test: expected message to contain 'pngunknown: " << reason
                 << ": usage:' and program name. Captured:\n" << captured);
        ok = false;
    }

    return ok;
}

// Test 2: Usage message handles reason with spaces and special characters
bool test_usage_reason_with_spaces()
{
    const std::string program = "prog-with-spaces";
    const std::string reason = "reason with spaces & special#chars";

    std::string captured;
    int exit_code = -1;

    bool ok = perform_usage_test(program, reason, captured, exit_code);
    ASSERT_TRUE(ok, "Failed to perform usage test (IPC or fork failure) for spaces case");

    if (exit_code != 99) {
        LOG_FAIL("Usage (spaces) test: expected exit code 99, got " << exit_code);
        ok = false;
    }

    // Ensure the captured message includes the exact reason text and program
    if (captured.find("pngunknown: " + reason + ": usage:\n " + program) == std::string::npos) {
        LOG_FAIL("Usage (spaces) test: output missing expected reason/program content. Captured:\n" << captured);
        ok = false;
    }

    return ok;
}

// Simple test runner
int main(void)
{
    std::cout << "Running usage() unit tests (POSIX-only):" << std::endl;

    // Run tests
    if (!test_usage_basic()) {
        std::cerr << "Test 'test_usage_basic' failed." << std::endl;
    } else {
        std::cout << "Test 'test_usage_basic' passed." << std::endl;
    }

    if (!test_usage_reason_with_spaces()) {
        std::cerr << "Test 'test_usage_reason_with_spaces' failed." << std::endl;
    } else {
        std::cout << "Test 'test_usage_reason_with_spaces' passed." << std::endl;
    }

    // Summary
    std::cout << "Total tests executed: 2" << std::endl;
    std::cout << "Total failures: " << g_failed_tests << std::endl;

    // Return nonzero on any failure to reflect status to automated harnesses
    return (g_failed_tests > 0) ? 1 : 0;
}

/*
Notes:
- This test suite focuses on the focal method 'usage' by exercising its behavior
  in a separate process to avoid terminating the test runner via exit(99).
- It asserts that:
  - The child exits with code 99.
  - The emitted error message contains the expected program and reason tokens.
- The tests are designed to be self-contained and compilable under C++11 without GTest.
- The test harness uses POSIX APIs (fork, pipe, dup2, waitpid) and may be skipped on non-POSIX platforms.
- Static/global helpers and the test runner avoid touching private/internal state of the
  production code beyond what is exercised by 'usage' itself.
- Candidate keywords from the focal method and its dependencies (as per Step 1) include:
  "pngunknown", "usage", "program", "reason", "--strict", "--default", "{(CHNK|default|all)=(default|discard|if-safe|save)}",
  "testfile.png" and related tokens. These are reflected in the expected usage message content that the tests verify.
*/