// C++11 test suite for the focal method: signal_handler(int signum)
// This test suite exercises the signal_handler behavior without using GoogleTest.
// It relies on the existing C implementation in the project under test and does
// not modify private state directly. The tests are written to be run as a
// standalone executable alongside the project code (pngvalid.c).
//
// Notes:
// - SIGTERM path is tested by forking a child; the child should exit(1).
// - Other signals are expected to throw an exception (via the project-defined Throw).
// - Unknown/non-handled signals should also throw an exception.
// - The test harness prints PASS/FAIL but continues executing other tests.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <math.h>


// Declare the focal function with C linkage so the linker can resolve it.
// The actual implementation is in pngvalid.c (or linked object).
extern "C" void signal_handler(int signum);

// Simple non-fatal test harness to run tests sequentially and report status.
static void print_header(const std::string& s) {
    std::cout << "\n=== " << s << " ===" << std::endl;
}

// Test 1: SIGTERM must terminate the process with exit code 1.
// Implemented via fork to keep the parent test runner alive.
static bool test_SIGTERM_exits_with_code1(void) {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "[WARN] fork() failed: errno=" << errno << std::endl;
        return false;
    }
    if (pid == 0) {
        // Child process: invoke signal_handler with SIGTERM.
        // Expected: exit(1) is called by the handler.
        signal_handler(SIGTERM);
        // If, for some reason, it returns, terminate abnormally to fail the test.
        _exit(2);
    } else {
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "[ERR] waitpid() failed: errno=" << errno << std::endl;
            return false;
        }
        // Child should have exited with code 1.
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
            return true;
        } else {
            std::cerr << "[FAIL] test_SIGTERM_exits_with_code1: child exited status=";
            if (WIFEXITED(status))
                std::cerr << "exit(" << WEXITSTATUS(status) << ")";
            else if (WIFSIGNALED(status))
                std::cerr << "killed by signal " << WTERMSIG(status);
            else
                std::cerr << "unknown";
            std::cerr << std::endl;
            return false;
        }
    }
}

// Helper: test that signaling a given signum throws an exception.
// The exact exception type is implementation-defined; we catch all.
static bool test_signal_throws_exception(int signum, const std::string& name) {
    try {
        signal_handler(signum);
        // If no exception, that's a failure for this test.
        std::cerr << "[FAIL] test_" << name << ": expected exception but none thrown" << std::endl;
        return false;
    } catch (...) {
        // Exception was thrown as expected.
        return true;
    }
}

// Test 2-7: Each of the following signals should throw (abort paths).
static bool test_SIGABRT_throws_exception(void) {
    return test_signal_throws_exception(SIGABRT, "SIGABRT_throws_exception");
}
static bool test_SIGFPE_throws_exception(void) {
    return test_signal_throws_exception(SIGFPE, "SIGFPE_throws_exception");
}
static bool test_SIGILL_throws_exception(void) {
    return test_signal_throws_exception(SIGILL, "SIGILL_throws_exception");
}
static bool test_SIGINT_throws_exception(void) {
    return test_signal_throws_exception(SIGINT, "SIGINT_throws_exception");
}
static bool test_SIGSEGV_throws_exception(void) {
    return test_signal_throws_exception(SIGSEGV, "SIGSEGV_throws_exception");
}
static bool test_unknown_signal_throws_exception(void) {
    // Use a clearly unknown signal number to exercise the default branch.
    int unknown = 9999;
    return test_signal_throws_exception(unknown, "unknown_signal_throws_exception");
}

// Test 8: Default/predictable behavior for an obviously unknown signum (ensure non-terminating path is exercised).
static bool test_SIGUNKNOWN_throws_exception(void) {
    // Reuse the unknown-numeric-signum test as an explicit case.
    int unknown = 12345;
    return test_signal_throws_exception(unknown, "SIGUNKNOWN_throws_exception");
}

int main() {
    print_header("signal_handler unit tests (C focal method)");

    int total = 0;
    int passed = 0;

    auto run = [&](bool ok, const std::string& testName) {
        ++total;
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << testName << std::endl;
        } else {
            std::cout << "[FAIL] " << testName << std::endl;
        }
    };

    // Run tests in a sequence to maximize code coverage paths.
    bool t1 = test_SIGTERM_exits_with_code1();
    run(t1, "SIGTERM_exits_with_code1");

    bool t2 = test_SIGABRT_throws_exception();
    run(t2, "SIGABRT_throws_exception");

    bool t3 = test_SIGFPE_throws_exception();
    run(t3, "SIGFPE_throws_exception");

    bool t4 = test_SIGILL_throws_exception();
    run(t4, "SIGILL_throws_exception");

    bool t5 = test_SIGINT_throws_exception();
    run(t5, "SIGINT_throws_exception");

    bool t6 = test_SIGSEGV_throws_exception();
    run(t6, "SIGSEGV_throws_exception");

    bool t7 = test_unknown_signal_throws_exception();
    run(t7, "unknown_signal_throws_exception");

    bool t8 = test_SIGUNKNOWN_throws_exception();
    run(t8, "SIGUNKNOWN_throws_exception_again");

    // Summary
    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed." << std::endl;
    // Return non-zero if any test failed to aid CI systems.
    return (passed == total) ? 0 : 1;
}