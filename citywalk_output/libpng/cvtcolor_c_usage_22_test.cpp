/*
Unit Test Suite for the focal method: usage(const char* prog)

Context:
- The focal method prints a usage message to standard error and exits with code 1.
- We cannot call usage directly in the test harness because it terminates the process.
- Approach: fork a child process, redirect its stderr to a pipe, invoke usage(prog) in the child,
  and capture the output in the parent. The child should exit with status 1, which we verify.
- This test suite is implemented in C++11 without Google Test, using a minimal in-house assertion approach.

Key Candidate Keywords (derived from the focal method and its surrounding context):
- usage
- prog
- "%s: usage: %s [-linear|-sRGB] [-gray|-color] component{1,4}\n"
- [-linear|-sRGB]
- [-gray|-color]
- component{1,4}
- exit(1)
- stderr / fprintf(stderr, ...)
- test stability across different prog names

Notes:
- This test suite uses POSIX APIs (fork, pipe, dup2, read, waitpid) which are available on Unix-like systems.
- The test code declares the focal function with C linkage to link correctly with cvtcolor.c.

Compilation and usage:
- Compile the test as a C++11 program and link against the compilation unit that provides:
  extern "C" void usage(const char* prog);
  For example:
    g++ -std=c++11 test_cvtcolor_usage.cpp cvtcolor.c -o test_cvtcolor_usage
  Then run: ./test_cvtcolor_usage

Below is the test code with explanatory comments for each unit test.
*/

#include <sRGB.h>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <fenv.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <math.h>


extern "C" void usage(const char* prog);

// Helper function: spawn a process that calls usage(prog) and capture its stderr.
// Returns true if the child exited with status 1 and captured output is filled.
// Populates 'out' with the full stderr content produced by usage.
bool capture_usage_output(const char* prog, std::string& out) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return false;
    }

    if (pid == 0) {
        // Child process: redirect stderr to the write end of the pipe.
        // Then call the focal function. We expect usage to print and exit(1).
        dup2(pipefd[1], STDERR_FILENO);
        // Close unused file descriptors
        close(pipefd[0]);
        close(pipefd[1]);
        // Call the function under test
        usage(prog);
        // If, unexpectedly, usage returns, exit to avoid hanging tests
        _exit(0);
    } else {
        // Parent process: read from the read end of the pipe.
        close(pipefd[1]); // Close write end in parent
        char buffer[4096];
        std::string captured;
        ssize_t r;
        while ((r = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            captured.append(buffer, static_cast<size_t>(r));
        }
        close(pipefd[0]);

        int status = 0;
        waitpid(pid, &status, 0);

        out = captured;

        // Expect the child to have exited with status 1
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
            return true;
        } else {
            // If the child did not exit with 1, or did not exit normally, test fails
            return false;
        }
    }
}

/*
Test 1: Verify that usage prints the correct formatted message for a given program name
- Purpose: Ensure the exact usage string is produced, including all expected tokens.
- Criteria:
  - The child process exits with status 1.
  - The captured stderr contains the exact prefix:
    "<prog>: usage: <prog> [-linear|-sRGB] [-gray|-color] component{1,4}\n"
  - The captured string contains the tokens "-linear|-sRGB", "-gray|-color", and "component{1,4}".
- Rationale: Places high confidence on string formatting and ensures compatibility with the documented interface.
*/
bool test_usage_message_format_for_prog() {
    const char* prog = "prog";
    std::string captured;
    bool ok = capture_usage_output(prog, captured);
    if (!ok) {
        std::cerr << "[FAIL] test_usage_message_format_for_prog: usage did not exit with status 1 or capture failed.\n";
        std::cerr << "Captured output (if any): " << captured << "\n";
        return false;
    }

    // Expected full message (with newline)
    std::string expected = std::string(prog) + ": usage: " + std::string(prog) +
                           " [-linear|-sRGB] [-gray|-color] component{1,4}\n";

    // Check exact prefix/substring as part of robust verification
    if (captured.find(expected) == std::string::npos) {
        std::cerr << "[FAIL] test_usage_message_format_for_prog: expected exact message not found.\n";
        std::cerr << "Expected to find: " << expected;
        std::cerr << "Actual captured: " << captured << "\n";
        return false;
    }

    // Additional keyword checks to increase coverage
    if (captured.find("-linear|-sRGB") == std::string::npos ||
        captured.find("-gray|-color") == std::string::npos ||
        captured.find("component{1,4}") == std::string::npos) {
        std::cerr << "[FAIL] test_usage_message_format_for_prog: missing one or more required tokens in output.\n";
        std::cerr << "Actual captured: " << captured << "\n";
        return false;
    }

    return true;
}

/*
Test 2: Verify that the program name is correctly reflected in the usage message
- Purpose: Ensure that a different program name (prog2) yields a message that starts with that name,
  i.e., no hard-coded reliance on a single program name.
- Criteria:
  - The child exits with status 1.
  - The captured string starts with "prog2: usage: prog2 ".
- Rationale: Confirms that the function uses the provided prog argument consistently in both places.
*/
bool test_usage_message_includes_prog_name(const char* prog) {
    std::string captured;
    bool ok = capture_usage_output(prog, captured);
    if (!ok) {
        std::cerr << "[FAIL] test_usage_message_includes_prog_name: usage did not exit with status 1 or capture failed for prog=" << prog << "\n";
        return false;
    }
    std::string expectedPrefix = std::string(prog) + ": usage: " + std::string(prog) + " ";
    if (captured.find(expectedPrefix) == std::string::npos) {
        std::cerr << "[FAIL] test_usage_message_includes_prog_name: expected prefix not found for prog=" << prog
                  << ". Actual captured: " << captured << "\n";
        return false;
    }
    return true;
}

/*
Test 3: Convenience wrapper test to validate that changing the program name reproduces the expected behavior
- Purpose: Run test_usage_message_includes_prog_name with a second program name to broaden coverage.
*/
bool test_usage_message_with_different_prog_name() {
    return test_usage_message_includes_prog_name("anotherProg");
}

/*
Main test runner:
- Invokes all tests, prints a concise pass/fail report for each test, and returns a non-zero exit code if any test fails.
- The framework is intentionally minimal as per constraints (no gtest/gmock, no private APIs).
- The tests use non-terminating assertions: they print diagnostic information on failure but do not abort the test suite.
*/
int main() {
    std::vector< std::pair<const char*, bool> > results;

    // Run Test 1
    bool t1 = test_usage_message_format_for_prog();
    results.push_back({ "test_usage_message_format_for_prog", t1 });

    // Run Test 2 (prog="anotherProg")
    bool t2 = test_usage_message_includes_prog_name("anotherProg");
    results.push_back({ "test_usage_message_includes_prog_name(anotherProg)", t2 });

    // Run Test 3
    bool t3 = test_usage_message_with_different_prog_name();
    results.push_back({ "test_usage_message_with_different_prog_name", t3 });

    // Summary
    int passed = 0;
    for (const auto& r : results) {
        if (r.second) {
            std::cout << "[PASS] " << r.first << "\n";
            ++passed;
        } else {
            std::cout << "[FAIL] " << r.first << "\n";
        }
    }
    std::cout << "Summary: " << passed << " / " << results.size() << " tests passed.\n";

    return (passed == static_cast<int>(results.size())) ? 0 : 1;
}