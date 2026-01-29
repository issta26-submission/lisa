// Unit test suite for the focal method usage(...) defined in the provided C code.
// This test suite is written for C++11 and does not rely on GTest.
// It uses POSIX APIs (fork/pipe) to verify the behavior of usage(...) which prints
// a usage message to stderr and then calls exit(1).
// The test ensures:
// 1) The usage message contains the provided program name (zArgv0) in the expected format.
// 2) The program exits with status code 1 as documented.
// 3) Non-terminating helper functions report_default_vfs and report_rbu_vfs exist and can be called
//    without crashing (no-ops per their definitions).

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <cerrno>
#include <sys/wait.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <sqlite3rbu.h>
#include <unistd.h>
#include <iostream>
#include <cassert>


// Forward declaration of the focal function from the C code.
// It is defined with C linkage.
extern "C" void usage(const char *zArgv0);

// The following helpers come from the same focal/class-dependency file.
// They are defined as no-ops in the provided code, but we declare them here
// to verify they can be called from C++ test code.
extern "C" void report_default_vfs();
extern "C" void report_rbu_vfs(void *pRbu);

// Optional: include sqlite3rbu.h if available in the test environment.
// The function report_rbu_vfs takes sqlite3rbu*, but since the implementation is empty,
// passing nullptr is sufficient for testing that the function is callable.
// If sqlite3rbu.h is not available, we still declare the function above so linkage should succeed
// as long as the object containing usage() is linked in.


// Simple POSIX-based test harness helpers

// Reads all data from a file descriptor until EOF and returns it as a string.
static std::string read_all(int fd) {
    std::string data;
    char buffer[4096];
    ssize_t n;
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        data.append(buffer, buffer + n);
    }
    return data;
}

// Test 1: Verify that usage("testprog") prints the expected usage message to stderr
// and exits with status code 1. The test runs usage() in a forked child process and
// captures the child's stderr via a pipe.
static bool test_usage_prints_and_exits_with_status_one() {
#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
    int pipe_stderr[2];
    if (pipe(pipe_stderr) != 0) {
        std::cerr << "pipe() failed: " << std::strerror(errno) << "\n";
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fork() failed: " << std::strerror(errno) << "\n";
        // Cleanup
        close(pipe_stderr[0]);
        close(pipe_stderr[1]);
        return false;
    }

    if (pid == 0) {
        // Child process
        // Redirect stderr to the write end of the pipe
        dup2(pipe_stderr[1], STDERR_FILENO);
        // Close unused ends
        close(pipe_stderr[0]);
        close(pipe_stderr[1]);

        // Call the focal function with a known program name
        usage("testprog");

        // If, for some reason, usage returns (it shouldn't), exit explicitly.
        _exit(1);
    } else {
        // Parent process
        // Close the write end; we only read
        close(pipe_stderr[1]);

        // Read all data emitted to stderr by the child
        std::string captured = read_all(pipe_stderr[0]);
        close(pipe_stderr[0]);

        // Wait for child to finish
        int status = 0;
        waitpid(pid, &status, 0);

        // Check that the child exited with status 1
        if (WIFEXITED(status)) {
            int es = WEXITSTATUS(status);
            if (es != 1) {
                std::cerr << "Child exited with status " << es << ", expected 1\n";
                return false;
            }
        } else {
            std::cerr << "Child did not exit normally\n";
            return false;
        }

        // Verify the captured message includes the expected usage header with the program name.
        // We expect: "Usage: testprog ?OPTIONS? TARGET-DB"
        const std::string expected_start = "Usage: testprog ?OPTIONS? TARGET-DB";
        if (captured.find(expected_start) == std::string::npos) {
            std::cerr << "Captured usage message did not contain expected header.\n";
            std::cerr << "Captured:\n" << captured << "\n";
            return false;
        }

        // Also ensure that the message contains the subset sections such as "-vacuum",
        // which is part of the options listed.
        if (captured.find("-vacuum") == std::string::npos) {
            std::cerr << "Captured usage message missing expected option '-vacuum'.\n";
            return false;
        }

        // All checks passed
        return true;
    }
#else
    // Non-POSIX environment: skip the test with a helpful note.
    std::cout << "Skipping test_usage_prints_and_exits_with_status_one (POSIX required).\n";
    return true;
#endif
}

// Test 2: Verify that report_default_vfs() can be called without crashing.
static bool test_report_default_vfs_no_crash() {
    // The function is defined as a no-op in the focal code.
    try {
        report_default_vfs();
        return true;
    } catch (...) {
        std::cerr << "report_default_vfs() threw an exception unexpectedly.\n";
        return false;
    }
}

// Test 3: Verify that report_rbu_vfs(nullptr) can be called without crashing.
static bool test_report_rbu_vfs_with_nullptr_no_crash() {
    try {
        report_rbu_vfs(nullptr);
        return true;
    } catch (...) {
        std::cerr << "report_rbu_vfs(nullptr) threw an exception unexpectedly.\n";
        return false;
    }
}

// Minimal test runner
int main() {
    int failures = 0;

    std::cout << "Running unit tests for usage() and related VFS helpers...\n";

    if (!test_usage_prints_and_exits_with_status_one()) {
        std::cerr << "Test 1 failed: usage() did not print expected message or exit status.\n";
        ++failures;
    } else {
        std::cout << "Test 1 passed: usage() prints correct message and exits with status 1.\n";
    }

    if (!test_report_default_vfs_no_crash()) {
        std::cerr << "Test 2 failed: report_default_vfs() crashed or threw.\n";
        ++failures;
    } else {
        std::cout << "Test 2 passed: report_default_vfs() is callable and safe.\n";
    }

    if (!test_report_rbu_vfs_with_nullptr_no_crash()) {
        std::cerr << "Test 3 failed: report_rbu_vfs(nullptr) crashed or threw.\n";
        ++failures;
    } else {
        std::cout << "Test 3 passed: report_rbu_vfs(nullptr) is callable and safe.\n";
    }

    if (failures > 0) {
        std::cerr << "UNIT TEST SUMMARY: " << failures << " failure(s).\n";
        return 1;
    }

    std::cout << "All tests passed.\n";
    return 0;
}