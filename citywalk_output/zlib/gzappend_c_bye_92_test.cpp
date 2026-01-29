// C++11 test suite for the focal method "bye" in gzappend.c
// Notes:
// - The focal method prints an error message to stderr and calls exit(1).
// - To test such a function without terminating the test runner, we override exit
//   via a macro before including the gzappend.c file in this translation unit.
// - We capture stderr by redirecting it to a pipe and examine the output.
// - We run a small internal test harness (no external testing framework).

#include <cstdio>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <setjmp.h>
#include <gzappend.c>
#include <sys/stat.h>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


using std::string;
using std::vector;
using std::cout;
using std::endl;

// Global jump buffer for simulating non-local exit (bye calls exit(1)).
static jmp_buf test_env;

// We capture the exit status that would be passed to exit(...)
static int test_exit_status = 0;

// Test exit function used by the overridden exit macro.
// It records the status and returns control to the test harness via longjmp.
static void test_exit(int status) {
    test_exit_status = status;
    longjmp(test_env, 1);
}

// Override exit to route through test_exit instead of terminating the process.
// Must be defined before including gzappend.c so that the function bye (and any
// calls to exit within it) use this override.
#define exit(status) test_exit(status)

// Forward declaration of bye matching the expected signature in gzappend.c.
// The actual definition in gzappend.c is declared as "local void bye(char*, char*)"
// where "local" is commonly defined as static. Since our test code includes gzappend.c
// in the same translation unit, this forward declaration aligns with the function's
// signature after macro expansion.
static void bye(char *msg1, char *msg2);

// Include the focal C file containing bye and other helpers.
// NOTE: gzappend.c also contains a main() function in many implementations.
// If that main collides with this test harness, you must adapt your build (e.g.,
// compile gzappend.c as part of a separate translation unit and expose bye).

// Undefine exit macro to avoid affecting subsequent code in this file inadvertently.
#undef exit

// Helper to perform a single bye test case by capturing stderr and exit status.
static string run_bye_test_case(const string& msg1, const string& msg2, int &outStatus) {
    // Prepare a pipe to capture stderr output.
    int pipe_fds[2];
    if (pipe(pipe_fds) != 0) {
        perror("pipe");
        exit(1);
    }

    // Redirect stderr to the write end of the pipe.
    int saved_stderr = dup(STDERR_FILENO);
    if (dup2(pipe_fds[1], STDERR_FILENO) == -1) {
        perror("dup2");
        exit(1);
    }
    close(pipe_fds[1]);

    // Make stderr unbuffered to ensure timely capture.
    setvbuf(stderr, NULL, _IONBF, 0);

    // Reset the test exit status before running the function.
    test_exit_status = 0;

    // Set up non-local jump and invoke bye. If bye calls exit(1), control will
    // return here via longjmp.
    if (setjmp(test_env) == 0) {
        // Call the focal method with provided messages.
        bye((char*)msg1.c_str(), (char*)msg2.c_str());
        // If bye returns (which it should not), we treat this as a failure.
        // We simulate a failure exit code.
        test_exit(2); // Any non-1 value would indicate unexpected path.
    }

    // Restore stderr and close the pipe read end.
    if (dup2(saved_stderr, STDERR_FILENO) == -1) {
        perror("dup2 restore");
        exit(1);
    }
    close(saved_stderr);

    // Read captured output from the pipe.
    string captured;
    char buf[256];
    ssize_t n;
    while ((n = read(pipe_fds[0], buf, sizeof(buf))) > 0) {
        captured.append(buf, (size_t)n);
    }
    close(pipe_fds[0]);

    // Return exit status captured by test_exit.
    outStatus = test_exit_status;
    return captured;
}

// Lightweight test runner
static bool assert_equal(const string& actual, const string& expected, const string& testName) {
    if (actual == expected) {
        cout << "[PASS] " << testName << endl;
        return true;
    } else {
        cout << "[FAIL] " << testName << "\n"
             << "  Expected: \"" << expected << "\"\n"
             << "  Actual  : \"" << actual << "\"\n";
        return false;
    }
}

// Entry point for running the tests
int main() {
    cout << "Running bye() test suite for gzappend.c (C++ test harness)" << endl;

    // Expected outputs are the concatenation of "gzappend error: ", msg1, msg2, and newline.
    vector<std::tuple<string,string,string>> tests = {
        // msg1, msg2, expected output
        std::make_tuple(string("foo"), string("bar"), string("gzappend error: foobar\n")),
        std::make_tuple(string(""),    string("baz"), string("gzappend error: baz\n")),
        std::make_tuple(string("hello"), string(""),    string("gzappend error: hello\n")),
        std::make_tuple(string("!@#"), string("$%^"), string("gzappend error: !@#$%\n"))
    };

    int total = 0;
    int passed = 0;

    for (size_t i = 0; i < tests.size(); ++i) {
        const string& m1 = std::get<0>(tests[i]);
        const string& m2 = std::get<1>(tests[i]);
        const string& expected = std::get<2>(tests[i]);
        int status = 0;
        string output = run_bye_test_case(m1, m2, status);

        // We expect exit status to be 1 for bye path (as per bye() implementation).
        bool ok = (status == 1) && (output == expected);
        total++;
        if (ok) {
            ++passed;
            cout << "[TEST " << (i+1) << "] OK" << endl;
        } else {
            // Provide detailed failure information already emitted by assert_equal.
            cout << "[TEST " << (i+1) << "] FAILED (status=" << status << ")" << endl;
        }
        // Additionally, we can call a stricter check helper to show mismatch clearly.
        // We reuse assert_equal for the exact string check.
        if (output != expected) {
            assert_equal(output, expected, "output mismatch for test " + std::to_string(i+1));
        } else {
            // If the output matches, still ensure status is 1
            if (status != 1) {
                cout << "[FAIL] test " << (i+1) << " expected exit status 1, got " << status << endl;
                // Not counting here as pass
            }
        }
    }

    cout << "Summary: " << passed << " / " << total << " tests passed." << endl;
    return (passed == total) ? 0 : 1;
}