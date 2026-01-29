/*
Unit test suite for the Die function (C-style variadic) using a forked-process approach
to safely test exit(...) behavior and stdout output without terminating the test runner.

Approach:
- Call Die from a child process created via fork(). The child redirects stdout to a pipe,
  invokes Die with predefined format strings, and exits with status 1 as Die does.
- The parent reads the captured output from the pipe and asserts:
  - The child exited with code 1.
  - The stdout content matches the expected formatted string (including the leading/trailing newlines).
- This avoids modifying the original Die implementation and keeps tests executable under C++11
  without using a testing framework like GTest.

Notes:
- We assume the presence of the header and implementation for Die in testcms2.h and testcms2.c
  (as per <FOCAL_CLASS_DEP> in the prompt). Die is expected to be a C function:
  extern "C" void Die(const char* Reason, ...);
- The tests use a small set of cases to cover true/false branches of formatting via vsprintf
  and the exit behavior.
- All tests are self-contained and use only standard library facilities.

Compile-time assumptions:
- The project compiles under C++11.
- The test cms2 components are linked with this test harness (i.e., testcms2.h and testcms2.c are part of the build).

Code below:
*/

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <cassert>
#include <stdlib.h>
#include <unistd.h>


extern "C" void Die(const char* Reason, ...); // C-style variadic function from the focal module

// Helper: In the child process, decide which Die call to perform based on test case name.
static void DieCaller(const char* caseName) {
    if (caseName == nullptr) {
        Die("CASE_NAME_IS_NULL");
    } else if (strcmp(caseName, "CASE1") == 0) {
        // Basic formatting with int and string
        Die("Failure: %d - %s", 404, "NotFound");
    } else if (strcmp(caseName, "CASE2") == 0) {
        // Simple string without formatting
        Die("Simple fail");
    } else if (strcmp(caseName, "CASE3") == 0) {
        // Multiple types with a floating value
        Die("A=%d, B=%s, C=%.2f", -7, "example", 3.14159);
    } else {
        Die("Unknown test case");
    }
    // If Die somehow returns (not expected), exit to avoid hanging the child.
    _exit(2);
}

// Run a single test case by forking a child that calls Die, capturing stdout.
static bool runDieTestCase(int testIndex, const std::string& expectedOutput) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return false;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return false;
    }

    if (pid == 0) {
        // Child process
        // Redirect stdout to the pipe's write end
        dup2(pipefd[1], STDOUT_FILENO);
        // Close unused ends
        close(pipefd[0]);
        close(pipefd[1]);

        // Determine test case name based on testIndex
        const char* caseName = (testIndex == 1) ? "CASE1" :
                               (testIndex == 2) ? "CASE2" :
                               (testIndex == 3) ? "CASE3" : nullptr;

        // Pass the test-case name to DieCaller
        DieCaller(caseName);

        // If Die somehow returns, exit with error
        _exit(3);
    } else {
        // Parent process
        // Close write end; only read from pipe
        close(pipefd[1]);

        // Wait for child to finish
        int status = 0;
        waitpid(pid, &status, 0);

        // Read all output from the pipe
        std::string output;
        char buffer[256];
        ssize_t r;
        while ((r = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output.append(buffer, static_cast<size_t>(r));
        }
        close(pipefd[0]);

        // Child should exit with status 1 due to Die(exit(1))
        bool exitedNormally = WIFEXITED(status);
        int exitCode = exitedNormally ? WEXITSTATUS(status) : -1;

        // Assertions/Checks:
        // 1) Process exited with code 1
        if (!(exitedNormally && exitCode == 1)) {
            std::cerr << "Test CASE" << testIndex << " failed: child did not exit with code 1. "
                      << "Status: " << status << " ExitCode: " << exitCode << "\n";
            std::cerr << "Captured output: " << output << "\n";
            return false;
        }

        // 2) Output matches expected (including leading newline and trailing newline)
        if (output != expectedOutput) {
            std::cerr << "Test CASE" << testIndex << " failed: unexpected output.\n"
                      << "Expected: [" << expectedOutput << "]\n"
                      << "Actual  : [" << output << "]\n";
            return false;
        }

        return true;
    }
    // Should not reach here
    return false;
}

int main() {
    // Each test case defines the expected stdout content including the leading newline
    // and the trailing newline produced by the Die function's printf("\n%s\n", ...)

    // Test Case 1: BASIC formatting with int and string
    if (!runDieTestCase(1, "\nFailure: 404 - NotFound\n")) {
        std::cerr << "Die Test Case 1 failed.\n";
        return 1;
    }
    // Test Case 2: Simple string without formatting
    if (!runDieTestCase(2, "\nSimple fail\n")) {
        std::cerr << "Die Test Case 2 failed.\n";
        return 2;
    }
    // Test Case 3: Multiple arguments including an integer, string, and float
    if (!runDieTestCase(3, "\nA=-7, B=example, C=3.14\n")) {
        std::cerr << "Die Test Case 3 failed.\n";
        return 3;
    }

    std::cout << "All Die tests passed.\n";
    return 0;
}