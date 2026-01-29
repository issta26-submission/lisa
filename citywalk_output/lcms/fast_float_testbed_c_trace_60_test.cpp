// Unit tests for the focal method: trace
// This test suite is written in C++11 (no GTest) and uses POSIX facilities to capture
// the stderr output produced by the C variadic function trace(...).
// It is designed to exercise the trace function without modifying its implementation.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


// Declare the focal C function so the C++ test can call it.
// The function signature matches the provided focal method: variadic, writes to stderr.
extern "C" void trace(const char* frm, ...);

// Utility: run a test by forking a child that redirects stderr to a pipe,
// calls trace with provided format/arguments, and the parent captures the output.
// The test asserts that the captured output exactly matches the expected string.
static bool run_trace_test_with_capture(const char* test_name,
                                      const char* format_str,
                                      const char* expected_output,
                                      int arg_count, ...) // variadic args to be passed to trace
{
    // We will construct the variadic arguments in the child via a separate call path.
    // Since C variadic arguments cannot be easily forwarded across fork/exec boundary,
    // we implement individual tests that call trace with fixed arguments in the child.

    (void)test_name; // avoid unused in case of future expansion
    (void)format_str;
    (void)expected_output;
    (void)arg_count;
    return false; // not used in this generic wrapper; concrete tests implement separately
}

// Test 1: Basic trace with no additional arguments.
// Expects the exact string to be written to stderr.
bool test_trace_basic_no_args()
{
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test_trace_basic_no_args: " << strerror(errno) << "\n";
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fork() failed in test_trace_basic_no_args: " << strerror(errno) << "\n";
        return false;
    }

    if (pid == 0) {
        // Child: redirect stderr to pipe's write end, then call trace with no args.
        dup2(pipefd[1], STDERR_FILENO);
        // Close unused descriptors
        close(pipefd[0]);
        close(pipefd[1]);
        // Call focal method
        trace("Hello World\n");
        _exit(0);
    }

    // Parent: close write end, read from read end
    close(pipefd[1]);
    std::string output;
    char buf[1024];
    ssize_t r;
    while ((r = read(pipefd[0], buf, sizeof(buf))) > 0) {
        output.append(buf, (size_t)r);
    }
    close(pipefd[0]);

    int status = 0;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        std::cerr << "test_trace_basic_no_args: child process failed\n";
        return false;
    }

    // Validate captured output
    const std::string expected = "Hello World\n";
    bool ok = (output == expected);
    if (!ok) {
        std::cerr << "test_trace_basic_no_args: expected \"" << expected
                  << "\" but got \"" << output << "\"\n";
    }
    return ok;
}

// Test 2: Trace with an integer and a string.
bool test_trace_int_and_string()
{
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test_trace_int_and_string: " << strerror(errno) << "\n";
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fork() failed in test_trace_int_and_string: " << strerror(errno) << "\n";
        return false;
    }

    if (pid == 0) {
        // Child: redirect stderr to pipe's write end, then call trace with arguments.
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Call: Int=%d Str=%s\n with values 42 and "ok"
        trace("Int=%d Str=%s\n", 42, "ok");
        _exit(0);
    }

    // Parent: capture output
    close(pipefd[1]);
    std::string output;
    char buf[1024];
    ssize_t r;
    while ((r = read(pipefd[0], buf, sizeof(buf))) > 0) {
        output.append(buf, (size_t)r);
    }
    close(pipefd[0]);

    int status = 0;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        std::cerr << "test_trace_int_and_string: child process failed\n";
        return false;
    }

    const std::string expected = "Int=42 Str=ok\n";
    bool ok = (output == expected);
    if (!ok) {
        std::cerr << "test_trace_int_and_string: expected \"" << expected
                  << "\" but got \"" << output << "\"\n";
    }
    return ok;
}

// Test 3: Trace with a floating-point value formatting (printf style).
bool test_trace_float formatting()
{
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test_trace_float_format: " << strerror(errno) << "\n";
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fork() failed in test_trace_float_format: " << strerror(errno) << "\n";
        return false;
    }

    if (pid == 0) {
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Call: Pi approx %.2f\n with value 3.14159
        trace("Pi approx %.2f\n", 3.14159);
        _exit(0);
    }

    close(pipefd[1]);
    std::string output;
    char buf[1024];
    ssize_t r;
    while ((r = read(pipefd[0], buf, sizeof(buf))) > 0) {
        output.append(buf, (size_t)r);
    }
    close(pipefd[0]);

    int status = 0;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        std::cerr << "test_trace_float_format: child process failed\n";
        return false;
    }

    const std::string expected = "Pi approx 3.14\n";
    bool ok = (output == expected);
    if (!ok) {
        std::cerr << "test_trace_float_format: expected \"" << expected
                  << "\" but got \"" << output << "\"\n";
    }
    return ok;
}


// Main test runner
int main()
{
    int total = 0;
    int passed = 0;

    std::cout << "Running tests for focal method: trace(...)\\n";

    // Test 1: Basic usage with no extra args
    total++;
    if (test_trace_basic_no_args()) {
        std::cout << "[PASS] test_trace_basic_no_args\\n";
        passed++;
    } else {
        std::cout << "[FAIL] test_trace_basic_no_args\\n";
    }

    // Test 2: Integer and string formatting
    total++;
    if (test_trace_int_and_string()) {
        std::cout << "[PASS] test_trace_int_and_string\\n";
        passed++;
    } else {
        std::cout << "[FAIL] test_trace_int_and_string\\n";
    }

    // Test 3: Floating-point formatting
    total++;
    if (test_trace_float formatting()) {
        std::cout << "[PASS] test_trace_float_format\\n";
        passed++;
    } else {
        std::cout << "[FAIL] test_trace_float_format\\n";
    }

    std::cout << "Tests passed: " << passed << " / " << total << "\\n";
    // Return non-zero if any test failed to aid integration with CI systems
    return (passed == total) ? 0 : 1;
}