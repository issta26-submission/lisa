/*
Unit test suite for the focal method testCaseFinish in lsmtest2.c (C function).
This test suite is implemented in C++11 and does not rely on a testing framework.
It focuses on covering both branches of the conditional in testCaseFinish:
- rc == 0 -> prints "Ok\n"
- rc != 0 -> prints "FAILED\n"
The tests capture stdout by redirecting the process' stdout to a pipe,
invoke the C function via an extern "C" linkage, and then verify the captured output.

Key concepts addressed (Candidate Keywords):
- testCaseFinish, rc, stdout, printf, fflush
- Branch coverage: rc == 0 and rc != 0
- C linkage from C++: extern "C" void testCaseFinish(int)
- Non-terminating assertions (tests report pass/fail but do not abort)
*/

#include <lsmtest.h>
#include <string>
#include <unistd.h>
#include <cstring>
#include <iostream>


// Declare the focal function with C linkage to avoid name mangling.
// The function is defined in a C source file (lsmtest2.c or similar).
extern "C" void testCaseFinish(int rc);

static std::string captureTestCaseFinish(int rc) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, return empty output to indicate error in test
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        // Could not duplicate stdout
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return "";
    }
    close(pipefd[1]); // The write end is duplicated to stdout

    // Invoke the focal function
    testCaseFinish(rc);

    // Ensure all output is flushed
    fflush(stdout);

    // Read captured output from the read end of the pipe
    std::string output;
    char buf[256];
    ssize_t n;
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
        output.append(buf, static_cast<size_t>(n));
    }

    // Restore stdout
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);
    close(pipefd[0]);

    return output;
}

// Test: rc == 0 should produce "Ok\n"
static bool test_case_finish_zero() {
    std::string out = captureTestCaseFinish(0);
    if (out == "Ok\n") {
        std::cout << "[PASS] testCaseFinish(0) emitted expected output: Ok\\n" << std::endl;
        return true;
    } else {
        std::cerr << "[FAIL] testCaseFinish(0) expected 'Ok\\n', got: '"
                  << out << "'" << std::endl;
        return false;
    }
}

// Test: rc != 0 should produce "FAILED\n"
static bool test_case_finish_nonzero() {
    std::string out = captureTestCaseFinish(1);
    if (out == "FAILED\n") {
        std::cout << "[PASS] testCaseFinish(1) emitted expected output: FAILED\\n" << std::endl;
        return true;
    } else {
        std::cerr << "[FAIL] testCaseFinish(1) expected 'FAILED\\n', got: '"
                  << out << "'" << std::endl;
        return false;
    }
}

int main() {
    // Run tests non-terminatingly; report results without exiting early
    int total_passed = 0;
    int total_tests = 2;

    if (test_case_finish_zero()) total_passed++;

    if (test_case_finish_nonzero()) total_passed++;

    // Final summary
    if (total_passed == total_tests) {
        std::cout << "[SUMMARY PASS] All tests passed (" << total_passed << "/" << total_tests << ")" << std::endl;
        return 0;
    } else {
        std::cerr << "[SUMMARY FAIL] Passed " << total_passed << " out of " << total_tests << " tests." << std::endl;
        return 1;
    }
}