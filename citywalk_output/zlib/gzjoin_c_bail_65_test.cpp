// Test suite for the focal bail function in gzjoin.c
// Approach:
// - Include gzjoin.c into this translation unit with main renamed to test_main to avoid duplicate mains.
// - Override the exit path by macro-mocking exit with a test-friendly mock_exit that records status.
// - Capture stderr output via a pipe to verify the exact error message produced by bail.
// - Call bail directly from tests (inside the same translation unit) and verify:
//   a) exit status passed to exit() is as expected (1 in this case)
//   b) the produced stderr message matches "gzjoin error: <why1><why2>, output incomplete\n"
//
// Notes:
// - This test uses POSIX APIs (pipe, dup2, read) and should compile under C++11 on a POSIX system.
// - No GoogleTest or other test frameworks are used; a lightweight, non-terminating assertion approach is implemented.

#include <cstdio>
#include <cstring>
#include <gzjoin.c>
#include <fcntl.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


// 1) Rename main in gzjoin.c to avoid clash with our test main, then include the focal source.
//    We also override exit(...) within that translation unit by defining a macro before inclusion.

static int last_exit_status = -1;

// Forward declaration for the bail function defined inside gzjoin.c.
// The bail function signature is: local int bail(char *why1, char *why2)
extern "C" int bail(char *why1, char *why2); // assume C linkage for the symbol in the included file

// 2) Mock exit to intercept termination. We provide a small mock_exit and replace exit with it.
static int mock_exit(int status) {
    last_exit_status = status;
    return 0; // ensure we don't terminate when bail calls exit
}
#define exit(status) mock_exit(status)  // intercept any exit(...) calls in bail

// Rename main in gzjoin.c to test_main to avoid conflict with our test harness main
#define main test_main
static int test_main(int argc, char **argv); // forward-declare the renamed entry (provided by gzjoin.c)
#undef main

// 3) Helper: Run bail with given why1/why2 and capture stderr output.
//    Returns true if bail invoked exit with expected status and captured matches.
static bool run_bail_and_capture(const char* why1, const char* why2, std::string& capturedOutput, int expectedStatus) {
    last_exit_status = -1;

    // Set up a pipe to capture stderr
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        perror("pipe");
        return false;
    }

    // Backup current stderr
    int saved_stderr = dup(fileno(stderr));
    if (saved_stderr == -1) {
        perror("dup");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return false;
    }

    // Redirect stderr to pipe write end
    if (dup2(pipe_fd[1], fileno(stderr)) == -1) {
        perror("dup2");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(saved_stderr);
        return false;
    }
    close(pipe_fd[1]); // not needed after dup2

    // Call the bail function (cast away const safely for test inputs)
    bail(const_cast<char*>(why1), const_cast<char*>(why2));

    // Flush and read captured output
    fflush(stderr);
    capturedOutput.clear();
    char buffer[1024];
    ssize_t n;
    // Read until EOF
    while ((n = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
        capturedOutput.append(buffer, buffer + n);
    }

    // Restore stderr
    if (dup2(saved_stderr, fileno(stderr)) == -1) {
        perror("dup2 restore");
        // continue cleanup
    }
    close(saved_stderr);
    close(pipe_fd[0]);

    // Verify the exit status captured by mock_exit
    return (last_exit_status == expectedStatus);
}

// 4) Lightweight test harness
static int tests_run = 0;
static int tests_passed = 0;

static void log_test_result(const std::string& testName, bool passed, const std::string& details = "") {
    ++tests_run;
    if (passed) {
        ++tests_passed;
        std::cout << "[PASS] " << testName << details << "\n";
    } else {
        std::cout << "[FAIL] " << testName << details << "\n";
    }
}

// Test 1: bail("foo","bar") should print "gzjoin error: foobar, output incomplete\n" and call exit(1)
static void test_bail_basic_concat() {
    const char* w1 = "foo";
    const char* w2 = "bar";
    std::string output;
    bool ok = run_bail_and_capture(w1, w2, output, 1);

    std::string expected = std::string("gzjoin error: ") + w1 + w2 + ", output incomplete\n";

    bool pass = ok && (output == expected);

    if (!ok) {
        log_test_result("bail_basic_concat/exit_status", false, " - expected exit status 1 but got different");
        return;
    }
    if (output != expected) {
        log_test_result("bail_basic_concat/output", false, " - output mismatch: expected '" + expected + "' but got '" + output + "'");
        return;
    }
    log_test_result("bail_basic_concat", true);
}

// Test 2: bail with empty why1 should still format correctly
static void test_bail_empty_why1() {
    const char* w1 = "";
    const char* w2 = "bar";
    std::string output;
    bool ok = run_bail_and_capture(w1, w2, output, 1);

    std::string expected = std::string("gzjoin error: ") + w1 + w2 + ", output incomplete\n"; // results in "gzjoin error: bar, output incomplete\n" but w1 is empty
    bool pass = ok && (output == expected);

    if (!ok) {
        log_test_result("bail_empty_why1/exit_status", false, " - expected exit status 1 but got different");
        return;
    }
    if (output != expected) {
        log_test_result("bail_empty_why1/output", false, " - output mismatch: expected '" + expected + "' but got '" + output + "'");
        return;
    }
    log_test_result("bail_empty_why1", true);
}

// Test 3: bail with empty why2 should still format correctly
static void test_bail_empty_why2() {
    const char* w1 = "foo";
    const char* w2 = "";
    std::string output;
    bool ok = run_bail_and_capture(w1, w2, output, 1);

    std::string expected = std::string("gzjoin error: ") + w1 + w2 + ", output incomplete\n"; // results in "gzjoin error: foo, output incomplete\n" but w2 is empty
    bool pass = ok && (output == expected);

    if (!ok) {
        log_test_result("bail_empty_why2/exit_status", false, " - expected exit status 1 but got different");
        return;
    }
    if (output != expected) {
        log_test_result("bail_empty_why2/output", false, " - output mismatch: expected '" + expected + "' but got '" + output + "'");
        return;
    }
    log_test_result("bail_empty_why2", true);
}

// Entry point for the test suite
int main() {
    // Run tests
    test_bail_basic_concat();
    test_bail_empty_why1();
    test_bail_empty_why2();

    // Summary
    std::cout << "\nTests run: " << tests_run << ", Passed: " << tests_passed << "\n";
    if (tests_run == tests_passed) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests failed.\n";
        return 1;
    }
}