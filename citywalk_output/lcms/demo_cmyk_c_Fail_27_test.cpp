// Test suite for the focal method Fail in demo_cmyk.c
// Notes:
// - The real Fail function prints a formatted string to stdout using vprintf
//   and then calls exit(1). To unit-test it safely without terminating the test
//   process, we override the C library function exit with a custom implementation
//   that throws a C++ exception carrying the exit status. This allows the test
//   harness to catch the exception, inspect the exit code, and continue.
// - We capture stdout by redirecting it to a pipe during the Fail call and then
//   reading the produced output for assertions.
// - This test suite assumes POSIX availability (pipe/dup/dup2/read). It is
//   designed to be compiled with C++11 and linked with the demo_cmyk.c object
//   that contains the Fail function. No Google Test is used; a tiny homegrown
//   harness is provided.

#include <cstring>
#include <lcms2_fast_float.h>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdarg>
#include <sys/types.h>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>


// Declaration of the focal method from the target file.
// Ensure we use C linkage since the function has C-style variadic parameters.
extern "C" void Fail(const char* frm, ...);

// Exception type used to imitate exit interception
struct ExitTrapped {
    int status;
    explicit ExitTrapped(int s) : status(s) {}
};

// Global to capture the exit status when Fail calls exit(1)
static int g_last_exit_status = -1;

// Override the C library exit() function to intercept Fail's exit call.
// We declare it with C linkage to ensure it matches the symbol Fail calls.
extern "C" int exit(int status) {
    g_last_exit_status = status;
    throw ExitTrapped(status);
}

// Helper to capture stdout output during a call to Fail.
// It redirects stdout to a POSIX pipe, executes the code, then restores stdout
// and returns the captured content.
class StdoutCapture {
public:
    StdoutCapture() : saved_fd(-1) {}

    void start() {
        // Create a pipe for stdout redirection
        if (pipe(pipe_fd) != 0) {
            throw std::runtime_error("pipe() failed");
        }
        // Save current stdout
        saved_fd = dup(fileno(stdout));
        if (saved_fd == -1) {
            throw std::runtime_error("dup() failed to save stdout");
        }
        // Redirect stdout to pipe's write end
        if (dup2(pipe_fd[1], fileno(stdout)) == -1) {
            throw std::runtime_error("dup2() failed to redirect stdout");
        }
    }

    std::string stop() {
        // Ensure all output is flushed
        fflush(stdout);

        // Restore original stdout
        if (dup2(saved_fd, fileno(stdout)) == -1) {
            throw std::runtime_error("dup2() failed to restore stdout");
        }
        // Close the write end to signal EOF to the reader
        close(pipe_fd[1]);

        // Read all data from the read end
        std::string captured;
        char buffer[4096];
        ssize_t n;
        while ((n = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            captured.append(buffer, static_cast<size_t>(n));
        }

        // Cleanup
        close(pipe_fd[0]);
        close(saved_fd);

        return captured;
    }

private:
    int pipe_fd[2];
    int saved_fd;
};

//////////////////////////////
// Minimal Test Framework
//////////////////////////////

static int g_test_passed = 0;
static int g_test_failed = 0;

#define ASSERT_EQ(expected, actual, msg) do { \
    if ((expected) == (actual)) { \
        ++g_test_passed; \
    } else { \
        ++g_test_failed; \
        std::cerr << "ASSERT FAILED: " << (msg) << " | expected: " \
                  << (expected) << "  actual: " << (actual) << std::endl; \
    } \
} while(0)

#define ASSERT_STR_EQ(expected, actual, msg) do { \
    if ((expected) == (actual)) { \
        ++g_test_passed; \
    } else { \
        ++g_test_failed; \
        std::cerr << "ASSERT FAILED: " << (msg) << " | expected: \"" \
                  << (expected) << "\"  actual: \"" << (actual) << "\"" << std::endl; \
    } \
} while(0)

//////////////////////////////
// Test Cases
//////////////////////////////

// Test 1: Verify that Fail prints the expected string with a simple string argument
// and exits with code 1.
void test_fail_prints_simple_string() {
    // Reset exit capture
    g_last_exit_status = -1;

    StdoutCapture cap;
    cap.start();

    try {
        Fail("Hello %s\n", "World");
    } catch (const ExitTrapped& e) {
        // Expected: Fail should call exit(1)
    } catch (...) {
        // Unexpected exception type
        std::cerr << "Unexpected exception type caught in test_fail_prints_simple_string" << std::endl;
        ++g_test_failed;
        return;
    }

    std::string output = cap.stop();

    // Assertions
    ASSERT_EQ(1, g_last_exit_status, "Exit status should be 1 after Fail call");
    ASSERT_STR_EQ("Hello World\n", output, "Output should be 'Hello World' with newline");
}

// Test 2: Verify numeric and hex formatting is handled correctly
void test_fail_numeric_and_hex_formatting() {
    g_last_exit_status = -1;

    StdoutCapture cap;
    cap.start();

    try {
        Fail("Value: %d, Hex: 0x%X\n", 255, 255);
    } catch (const ExitTrapped&) {
    } catch (...) {
        std::cerr << "Unexpected exception type in test_fail_numeric_and_hex_formatting" << std::endl;
        ++g_test_failed;
        return;
    }

    std::string output = cap.stop();

    ASSERT_EQ(1, g_last_exit_status, "Exit status should be 1 for numeric/hex formatting");
    ASSERT_STR_EQ("Value: 255, Hex: 0xFF\n", output, "Output formatting for %d and %X");
}

// Test 3: Verify escaping percent sign (%%) is printed correctly
void test_fail_percent_sign_escape() {
    g_last_exit_status = -1;

    StdoutCapture cap;
    cap.start();

    try {
        Fail("Progress: 50%% complete\n");
    } catch (const ExitTrapped&) {
    } catch (...) {
        std::cerr << "Unexpected exception type in test_fail_percent_sign_escape" << std::endl;
        ++g_test_failed;
        return;
    }

    std::string output = cap.stop();

    ASSERT_EQ(1, g_last_exit_status, "Exit status should be 1 for percent sign escape");
    ASSERT_STR_EQ("Progress: 50% complete\n", output, "Output should contain a single percent sign");
}

// Test 4: Verify multiple arguments formatting works (string, int, char)
void test_fail_mixed_arguments() {
    g_last_exit_status = -1;

    StdoutCapture cap;
    cap.start();

    try {
        Fail("Mixed: %s %d %c\n", "A", 1, 'Z');
    } catch (const ExitTrapped&) {
    } catch (...) {
        std::cerr << "Unexpected exception type in test_fail_mixed_arguments" << std::endl;
        ++g_test_failed;
        return;
    }

    std::string output = cap.stop();

    ASSERT_EQ(1, g_last_exit_status, "Exit status should be 1 for mixed arguments");
    ASSERT_STR_EQ("Mixed: A 1 Z\n", output, "Output should format string, int, and char correctly");
}

//////////////////////////////
// Test Runner
//////////////////////////////

int main() {
    std::cout << "Starting Fail() unit tests...\n";

    test_fail_prints_simple_string();
    test_fail_numeric_and_hex_formatting();
    test_fail_percent_sign_escape();
    test_fail_mixed_arguments();

    int total = g_test_passed + g_test_failed;
    std::cout << "Tests run: " << total << ", Passed: " << g_test_passed
              << ", Failed: " << g_test_failed << std::endl;

    if (g_test_failed == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}