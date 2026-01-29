// C++11 unit test suite for the C function: usage() from png2pnm.c
// Notes:
// - This test redirects stderr to a pipe via a forked child to capture exact output.
// - It compares the captured output against the expected usage message.
// - No GTest is used; a lightweight test harness with non-terminating assertions is implemented.

#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Declare the C function to be tested with C linkage.
extern "C" void usage();

// Lightweight test harness (non-terminating assertions)
static int test_count = 0;
static int failure_count = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        ++test_count; \
        if (!(cond)) { \
            std::cerr << "Test failure: " << (msg) << std::endl; \
            ++failure_count; \
        } \
    } while (0)

// Helper function to capture the output written to stderr by calling usage()
// This implementation uses a fork and a pipe to reliably capture stdout/stderr output.
std::string capture_usage_output() {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Pipe creation failed
        return "";
    }

    pid_t pid = fork();
    if (pid < 0) {
        // Fork failed
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    if (pid == 0) {
        // Child process: redirect stderr to the write end of the pipe and execute usage()
        // Close read end in child
        close(pipefd[0]);
        // Redirect stderr to pipe's write end
        if (dup2(pipefd[1], STDERR_FILENO) == -1) {
            _exit(1);
        }
        // Close the original write end as it's now duplicated
        close(pipefd[1]);

        // Call the function under test
        usage();

        // Ensure all data is flushed
        fflush(stderr);
        _exit(0);
    } else {
        // Parent process: read from the pipe after child finishes
        // Close write end in parent
        close(pipefd[1]);

        // Wait for child to finish
        int status = 0;
        waitpid(pid, &status, 0);

        // Read all data from the read end
        std::string output;
        char buffer[4096];
        ssize_t bytes = 0;
        while ((bytes = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output.append(buffer, static_cast<size_t>(bytes));
        }
        close(pipefd[0]);

        return output;
    }
}

// Test 1: Verify that usage() outputs the exact expected help text to stderr.
void test_usage_exact_output() {
    // Expected exact output as produced by the provided focal method
    const char* expected =
        "PNG2PNM\n"
        "   by Willem van Schaik, 1999\n"
        "Usage:  png2pnm [options] <file>.png [<file>.pnm]\n"
        "   or:  ... | png2pnm [options]\n"
        "Options:\n"
        "   -r[aw]   write pnm-file in binary format (P4/P5/P6) (default)\n"
        "   -n[oraw] write pnm-file in ascii format (P1/P2/P3)\n"
        "   -a[lpha] <file>.pgm write PNG alpha channel as pgm-file\n"
        "   -h | -?  print this help-information\n";

    std::string output = capture_usage_output();

    TEST_ASSERT(output == expected, "usage() output did not match the expected help text exactly.");
}

// Test 2: Ensure usage() output is stable across multiple invocations (no static-state leakage).
void test_usage_stability_on_multiple_calls() {
    std::string first = capture_usage_output();
    std::string second = capture_usage_output();

    TEST_ASSERT(!first.empty(), "First capture of usage() produced empty output.");
    TEST_ASSERT(!second.empty(), "Second capture of usage() produced empty output.");
    TEST_ASSERT(first == second, "Consecutive captures of usage() output differ; potential static state or buffering issue.");
}

// Entry point for the test suite
int main() {
    test_usage_exact_output();
    test_usage_stability_on_multiple_calls();

    if (failure_count == 0) {
        std::cout << "All tests passed: " << test_count << " tests run." << std::endl;
        return 0;
    } else {
        std::cerr << "Tests failed: " << failure_count << " failures out of " << test_count << " tests." << std::endl;
        return 1;
    }
}