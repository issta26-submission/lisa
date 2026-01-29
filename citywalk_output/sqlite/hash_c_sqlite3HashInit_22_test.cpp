// Test suite for the focal method sqlite3HashInit(Hash *pNew)
// This test suite is designed to be compiled with C++11, without GTest.
// It exercises the non-null path of sqlite3HashInit and the assertion-on-null-path
// using a separate process to avoid terminating the test runner on assertion.

#include <sys/types.h>
#include <assert.h>
#include <cerrno>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Import the C dependencies to access the Hash type and the function under test.
// We wrap the include in extern "C" to prevent C++ name mangling.
extern "C" {
}

// Helper: simple test harness state
static int g_total = 0;
static int g_passed = 0;

// Test 1: Non-null Hash instance must have its fields zeroed after sqlite3HashInit
// This validates the "true" branch where pNew != 0.
bool test_hashInit_nonNull_setsZero() {
    // Prepare a Hash instance with non-zero/garbage values
    Hash h;
    h.first = 123;
    h.count = 999;
    h.htsize = 7;
    // Assign a non-null dummy pointer to ht to ensure it gets reset
    h.ht = (struct _ht*)0x1;

    // Call the function under test
    sqlite3HashInit(&h);

    // Validate that all fields are reset to zero/null as per implementation
    if (h.first != 0) {
        std::cerr << "[FAIL] sqlite3HashInit: first not zeroed (expected 0, got " << h.first << ")\n";
        return false;
    }
    if (h.count != 0) {
        std::cerr << "[FAIL] sqlite3HashInit: count not zeroed (expected 0, got " << h.count << ")\n";
        return false;
    }
    if (h.htsize != 0) {
        std::cerr << "[FAIL] sqlite3HashInit: htsize not zeroed (expected 0, got " << h.htsize << ")\n";
        return false;
    }
    if (h.ht != nullptr) {
        std::cerr << "[FAIL] sqlite3HashInit: ht not null (expected null, got non-null)\n";
        return false;
    }

    return true;
}

// Test 2: Null pointer should trigger the assert/panic path in non-NDEBUG builds.
// This uses a separate process via fork to avoid terminating the test runner on failure.
bool test_hashInit_nullPointer_assertion() {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process: deliberately call the function with a null pointer
        sqlite3HashInit(nullptr);
        // If it returns, the assertion did not abort as expected.
        _exit(0);
    } else if (pid > 0) {
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "[ERROR] waitpid failed: " << std::strerror(errno) << "\n";
            return false;
        }

        // If the child terminated due to a signal caused by assert, we expect SIGABRT (or similar)
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            if (sig == SIGABRT) {
                // Expected: the assert aborts the process
                return true;
            } else {
                std::cerr << "[FAIL] sqlite3HashInit(nullptr) aborted with unexpected signal: " << sig << "\n";
                return false;
            }
        }

        // If child exited normally, that means the assertion did not trigger (unexpected)
        std::cerr << "[FAIL] sqlite3HashInit(nullptr) did not abort as expected (exit status: "
                  << WEXITSTATUS(status) << ")\n";
        return false;
    } else {
        std::cerr << "[ERROR] fork() failed for null-pointer assertion test\n";
        return false;
    }
}

// Lightweight test runner
void run_tests() {
    g_total = 0;
    g_passed = 0;

    std::cout << "Running sqlite3HashInit unit tests (C++11)...\n";

    // Test 1: Non-null pointer should zero fields
    g_total++;
    if (test_hashInit_nonNull_setsZero()) {
        std::cout << "PASS: test_hashInit_nonNull_setsZero\n";
        g_passed++;
    } else {
        std::cout << "FAIL: test_hashInit_nonNull_setsZero\n";
    }

    // Test 2: Null pointer should trigger assertion (in non-NDEBUG builds)
    g_total++;
    if (test_hashInit_nullPointer_assertion()) {
        std::cout << "PASS: test_hashInit_nullPointer_assertion\n";
        g_passed++;
    } else {
        std::cout << "FAIL: test_hashInit_nullPointer_assertion\n";
    }

    std::cout << "Summary: " << g_passed << " / " << g_total << " tests passed.\n";
}

int main() {
    run_tests();
    // Return non-zero if some tests failed
    return (g_passed == g_total) ? 0 : 1;
}