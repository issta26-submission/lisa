// tests_infcover.cpp
// Lightweight C++11 test suite for the focal method main() in infcover.c
// No GoogleTest/GTest required. Tests are executed by running this binary.
// The suite attempts to execute the target binary (infcover or a.out) and
// captures its stderr output to verify the zlib version string is emitted
// and that the program exits with code 0.
//
// Notes:
// - We rely on spawning a separate process to avoid modifying or linking against
//   the focal code directly. This keeps the test self-contained and compatible
//   with environments where including the C source would be problematic.
// - The test checks that the emitted string matches a version-like pattern
//   (digits with dots, e.g., "1.2.11") followed by a newline as produced by
//   the original code: fprintf(stderr, "%s\n", zlibVersion()).
// - If the target binary isn't found, tests will report a failure rather than
//   crashing the test runner.
//
// Step-by-step alignment with the provided instructions:
// Step 1: Program Understanding
// - The focal method main prints the zlib version to stderr and then calls several
//   cover_* helpers. Our tests focus on ensuring the zlib version string is emitted
//   and that the program exits with status 0.
// Step 2: Unit Test Generation
// - Tests target only the main() behavior by executing the binary in a separate process.
// - We consider the existence of candidate binaries that might be produced by the
//   build (./infcover or ./a.out).
// Step 3: Test Case Refinement
// - Use POSIX process control (fork/exec) to capture stderr without modifying the
//   target code. Validate version format via a regex matcher.
// - Provide clear comments for each test case to explain intent and outcomes.

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <regex>
#include <vector>
#include <inftrees.h>
#include <zlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <inflate.h>
#include <sys/wait.h>


// Simple helper to check if a file exists and is executable.
static bool file_exists_and_executable(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return false;
    // Check executable bit for user, group, or others
    if (S_ISDIR(st.st_mode)) return false;
    if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH))
        return true;
    return false;
}

// Execute the target binary and capture its stderr output.
// Returns true if the binary was launched (even if it exits non-zero).
static bool run_target_capture_stderr(const std::string& path, std::string &captured, int &exitCode) {
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        std::cerr << "Failed to create pipe: " << strerror(errno) << "\n";
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Failed to fork: " << strerror(errno) << "\n";
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return false;
    }

    if (pid == 0) {
        // Child process: redirect stderr to the pipe and exec the target
        // Redirect only STDERR; leave stdout unaffected (not used by the target)
        if (dup2(pipe_fd[1], STDERR_FILENO) == -1) {
            _exit(127);
        }
        // Close unused ends
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        // Execute the target binary
        execl(path.c_str(), path.c_str(), (char*)nullptr);
        // If exec fails
        _exit(127);
    } else {
        // Parent process
        close(pipe_fd[1]); // Close write end; we'll read from read end
        char buffer[4096];
        ssize_t r;
        captured.clear();
        // Read until EOF
        while ((r = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            captured.append(buffer, static_cast<size_t>(r));
        }
        close(pipe_fd[0]);
        // Wait for child to finish
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            exitCode = WEXITSTATUS(status);
        } else {
            // If terminated abnormally
            exitCode = -1;
        }
        return true;
    }
}

// Trims trailing whitespace characters (newline, carriage return, space) from a string.
static std::string rtrim(const std::string& s) {
    size_t end = s.find_last_not_of(" \t\r\n");
    if (end == std::string::npos) return "";
    return s.substr(0, end + 1);
}

// Test 1: Verify that running the focal binary prints a valid zlib version string to stderr
// and exits with code 0.
static bool test_main_prints_version_and_exits_zero() {
    // Candidate binaries produced by the build system.
    const std::vector<std::string> candidates = {
        "./infcover",
        "./a.out",
        "infcover",
        "a.out"
    };

    std::string output;
    int exitCode = -1;
    bool found = false;
    for (const auto& path : candidates) {
        if (!file_exists_and_executable(path)) continue;
        found = true;
        if (run_target_capture_stderr(path, output, exitCode)) {
            // We successfully launched the binary; stop scanning other candidates.
            break;
        } else {
            // If execution failed for this candidate, try the next one.
            found = false;
        }
    }

    if (!found) {
        std::cerr << "Test skipped: No executable target binary (infcover or a.out) found.\n";
        return false; // Consider as failed to keep strictness; caller may decide.
    }

    // Expect exit code 0
    if (exitCode != 0) {
        std::cerr << "Test failed: Target binary exited with code " << exitCode << ".\n";
        return false;
    }

    // Expect the output to contain a version-like string ending with newline.
    // The target prints: fprintf(stderr, "%s\n", zlibVersion());
    std::string trimmed = rtrim(output);
    // zlibVersion is typically of the form "X.Y.Z" where X,Y,Z are integers.
    std::regex re(R"(^\d+(\.\d+)+$)");
    if (trimmed.empty()) {
        std::cerr << "Test failed: No stderr output captured from target.\n";
        return false;
    }

    // The actual print includes a newline; we removed trailing whitespace, so pattern matches digits with dots.
    if (!std::regex_match(trimmed, re)) {
        std::cerr << "Test failed: Output does not look like a zlib version. Got: '" << trimmed << "'\n";
        return false;
    }

    // If we reach here, the test passes.
    std::cout << "Test passed: zlib version string captured as '" << trimmed << "' with exit code 0.\n";
    return true;
}

// Entry point for the test suite.
// We keep the suite minimal and self-contained, focusing on the focal main() behavior.
// The output is a concise summary of test results.
int main(void) {
    int total = 0;
    int passed = 0;

    // Test 1
    ++total;
    if (test_main_prints_version_and_exits_zero()) {
        ++passed;
    }

    // Summary
    std::cout << "Tests passed: " << passed << " / " << total << "\n";

    return (passed == total) ? 0 : 1;
}