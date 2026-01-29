/*
Unit test suite for the focal method Fail located in fast_float_testbed.c

Overview:
- The Fail function formats a variable-argument message using vsprintf and then calls FatalErrorQuit.
- In this test suite we cannot observe the internal formatted string directly (FatalErrorQuit in this testbed is a stub), so we isolate each call in a separate process.
- We fork a child process for each test case; the child executes a specific Fail invocation. If Fail triggers a crash (e.g., due to improper variadic usage or buffer issues), the child will terminate abnormally, and the test will be reported as failed.
- This approach ensures we exercise the variadic path, formatting behavior, and basic error handling without terminating the test runner.
- We do not rely on GoogleTest or other frameworks per instructions; instead we implement a lightweight test harness using fork/wait.

Notes:
- We declare Fail with C linkage to match the implementation in the C file.
- We cover a set of representative formatting scenarios to exercise common code paths:
  1) Simple message without formatting
  2) Integer formatting
  3) String formatting
  4) Mixed formatting (int, string, float)
  5) Literal percent sign
  6) Very long format string to exercise large outputs
- Each test is run in its own process via fork to prevent a crash in one test from terminating the entire test suite.

Build instructions (conceptual):
- Compile this test file with a C++11 compiler and link against the C file containing Fail (fast_float_testbed.c).
  Example (conceptual, adjust to your build system):
    g++ -std=c++11 test_fail.cpp fast_float_testbed.c -o test_fail_suite
- Run ./test_fail_suite; observe output for PASS/FAIL per test.

Code (test_fail.cpp):

*/

#include <cstring>
#include <fast_float_internal.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>


extern "C" void Fail(const char* frm, ...); // Focal method under test (variadic)

static int g_test_id = -1; // Will be used by child to select test case

// Child routine: each case calls Fail with a specific formatting scenario.
// This runs in a separate process to isolate crashes.
static void run_test_child(int test_id) {
    switch (test_id) {
        case 0:
            // Case 0: Simple message without any format specifiers
            Fail("Simple failure");
            break;

        case 1:
            // Case 1: Formatting with an integer
            Fail("Error code %d", 42);
            break;

        case 2:
            // Case 2: Formatting with a string
            Fail("Message: %s", "timeout");
            break;

        case 3:
            // Case 3: Multiple format specifiers (int, string, float)
            Fail("a=%d, b=%s, c=%.2f", 5, "x", 3.14);
            break;

        case 4:
            // Case 4: Literal percent signs (%%) should not require extra args
            Fail("Percent sign: %% done");
            break;

        case 5: {
            // Case 5: Very long format string to test buffer handling
            // Create a long string of ~1100 'A' characters to ensure large inputs
            char long_fmt[1200];
            std::memset(long_fmt, 'A', 1100);
            long_fmt[1100] = '\0';
            Fail(long_fmt);
            break;
        }

        default:
            // Do nothing for unknown test IDs
            break;
    }
}

// Lightweight test harness using fork to isolate crashes.
// Returns true if test passed (child exited cleanly), false otherwise.
static bool execute_test_in_isolated_process(int test_id, const char* test_name) {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "[ERROR] Fork failed for test: " << test_name << "\n";
        return false;
    }

    if (pid == 0) {
        // Child process
        g_test_id = test_id;
        run_test_child(test_id);
        // If Fail returns (which is allowed), exit with success.
        _exit(0);
    } else {
        // Parent process: wait for child to finish
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "[ERROR] waitpid failed for test: " << test_name << "\n";
            return false;
        }

        // Analyze exit status
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            std::cout << "[PASS] test: " << test_name << "\n";
            return true;
        } else if (WIFSIGNALED(status)) {
            std::cerr << "[FAIL] test: " << test_name
                      << " - Crashed (signal " << WTERMSIG(status) << ")\n";
            return false;
        } else {
            int exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
            std::cerr << "[FAIL] test: " << test_name
                      << " - Unexpected exit code: " << exit_code << "\n";
            return false;
        }
    }
}

int main() {
    // Define test suite: names and corresponding test IDs
    const int NUM_TESTS = 6;
    const char* test_names[NUM_TESTS] = {
        "Simple failure (no formatting)",
        "Integer formatting",
        "String formatting",
        "Mixed formatting (int, string, float)",
        "Literal percent sign",
        "Long format string (buffer stress test)"
    };

    bool all_passed = true;

    // Run each test in isolation
    for (int i = 0; i < NUM_TESTS; ++i) {
        bool ok = execute_test_in_isolated_process(i, test_names[i]);
        if (!ok) {
            all_passed = false;
        }
    }

    // Summary
    if (all_passed) {
        std::cout << "\nAll tests passed.\n";
        return 0;
    } else {
        std::cout << "\nSome tests failed. See above for details.\n";
        return 1;
    }
}