// Test suite for the focal function bad_parameter_count in makepng.c
// This test suite uses POSIX process isolation (fork) to capture stderr
// and verify that bad_parameter_count prints the expected message and exits
// with status 1. No GTest is used; a small in-code test harness is provided.

#include <unistd.h>
#include <stdint.h>
#include <vector>
#include <stddef.h>
#include <string.h>
#include <string>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <functional>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Prototyping the focal function with C linkage.
// The real signature uses png_const_charp (a const char*), but for test purposes
// this declaration is sufficient and safe due to C linkage.
extern "C" void bad_parameter_count(const char* what, int nparams);

// Helper function to run bad_parameter_count in a separate child process
// and capture its stderr output along with the exit code.
static bool invoke_bad_parameter_count(const char* what, int nparams,
                                      std::string& output, int& exit_code)
{
    int fds[2];
    if (pipe(fds) != 0) {
        perror("pipe");
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return false;
    }

    if (pid == 0) {
        // Child process
        // Redirect stderr to the write end of the pipe
        dup2(fds[1], STDERR_FILENO);
        // Close unused ends
        close(fds[0]);
        close(fds[1]);

        // Call the focal function; it should print to stderr and exit(1)
        bad_parameter_count(what, nparams);

        // If function returns (unexpected), exit with success to avoid hanging tests
        _exit(0);
    } else {
        // Parent process
        // Close the write end; we'll read from the read end
        close(fds[1]);

        // Read all data from the child process' stderr
        char buf[512];
        ssize_t r;
        output.clear();
        while ((r = read(fds[0], buf, sizeof(buf))) > 0) {
            output.append(buf, static_cast<size_t>(r));
        }
        close(fds[0]);

        // Wait for child to finish and capture exit status
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return false;
        }
        if (WIFEXITED(status))
            exit_code = WEXITSTATUS(status);
        else
            exit_code = -1;

        return true;
    }
}

// Lightweight test harness: run a test case and report result without aborting
static bool test_case(const char* what, int nparams,
                      const std::string& expected_output,
                      int expected_exit)
{
    std::string output;
    int exit_code = -999;
    bool ok = invoke_bad_parameter_count(what, nparams, output, exit_code);

    if (!ok) {
        std::cerr << "Test failed: unable to invoke bad_parameter_count(what=\""
                  << what << "\", nparams=" << nparams << ").\n";
        return false;
    }

    bool match = (output == expected_output) && (exit_code == expected_exit);
    if (!match) {
        std::cerr << "Test mismatch for what=\"" << what
                  << "\", nparams=" << nparams << ":\n"
                  << "  Captured output: [" << output << "]\n"
                  << "  Exit code: " << exit_code
                  << " (expected " << expected_exit << ")\n"
                  << "  Expected output: [" << expected_output << "]\n";
    }
    return match;
}

int main() {
    int total = 0;
    int passed = 0;

    // Test 1: Typical case with a simple parameter; verify message formatting and exit status
    // What to insert: "size", number of parameters: 2
    // Expected: "--insert size: bad parameter count 2\n" and exit code 1
    total++;
    if (test_case("size", 2, "--insert size: bad parameter count 2\n", 1)) {
        ++passed;
    }

    // Test 2: Zero parameters, ensure correct formatting
    // What to insert: "foo", number of parameters: 0
    // Expected: "--insert foo: bad parameter count 0\n" and exit code 1
    total++;
    if (test_case("foo", 0, "--insert foo: bad parameter count 0\n", 1)) {
        ++passed;
    }

    // Test 3: Empty what string to ensure message still formats correctly
    // What to insert: "" (empty), number of parameters: 5
    // Expected: "--insert : bad parameter count 5\n" and exit code 1
    total++;
    if (test_case("", 5, "--insert : bad parameter count 5\n", 1)) {
        ++passed;
    }

    // Test 4: Negative parameter count to cover signed integer formatting
    // What to insert: "neg", number of parameters: -5
    // Expected: "--insert neg: bad parameter count -5\n" and exit code 1
    total++;
    if (test_case("neg", -5, "--insert neg: bad parameter count -5\n", 1)) {
        ++passed;
    }

    std::cout << "Tests run: " << total << ", Passed: " << passed << "\n";
    return (passed == total) ? 0 : 1;
}