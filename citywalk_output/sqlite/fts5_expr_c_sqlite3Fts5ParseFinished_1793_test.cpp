// High-quality C++11 unit test suite for the focal method:
// sqlite3Fts5ParseFinished(Fts5Parse *pParse, Fts5ExprNode *p)
// This test harness does not depend on GoogleTest. It uses a small
// in-house non-terminating assertion mechanism to maximize code coverage
// and exercises both the "normal" path and the assertion-triggering path
// (the latter via a separate process to capture abort).

#include <fts5Int.h>
#include <sys/types.h>
#include <cstdio>
#include <fts5parse.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>


// Forward declarations to mirror the focal types used by sqlite3Fts5ParseFinished.
// These are intentionally minimal to keep tests isolated from the complete
// C project headers.
struct Fts5ExprNode;
struct Fts5Parse {
    Fts5ExprNode *pExpr;
};

// Ensure linkage with the focal C function.
// The function is defined in a C source file (fts5_expr.c). We declare it
// as extern "C" to avoid C++ name mangling.
extern "C" void sqlite3Fts5ParseFinished(Fts5Parse *pParse, Fts5ExprNode *p);

// Lightweight in-house test harness (non-terminating assertions)
static int g_failures = 0;
#define CHECK(cond) do { if(!(cond)) { \
    std::cerr << "Check failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
    ++g_failures; \
} } while(0)


// Test 1: Basic behavior - when pParse->pExpr is initially null, the function
// should set it to the provided expression node.
void test_ParseFinished_BasicSetsExpr() {
    // Arrange
    Fts5Parse pParse;
    pParse.pExpr = nullptr; // precondition for the assertion in sqlite3Fts5ParseFinished
    Fts5ExprNode exprNode;    // arbitrary node instance

    // Act
    sqlite3Fts5ParseFinished(&pParse, &exprNode);

    // Assert
    // The core expectation: pParse->pExpr must now point to the provided node.
    CHECK(pParse.pExpr == &exprNode);
    // Additional sanity: the pointer should not be null after the call.
    CHECK(pParse.pExpr != nullptr);
    // This test ensures the "true" branch of the important assertion.
}

// Test 2 (non-Windows): Assertion-triggering path.
// The function asserts that pParse->pExpr == 0. If this condition is false,
// the assertion triggers and aborts the process. We verify this by running
// the call in a separate process and expecting an abnormal termination.
#ifndef _WIN32
void test_ParseFinished_AssertsOnNonZeroExpr() {
    // Skip if fork isn't available
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed; skipping assertion-path test.\n";
        ++g_failures;
        return;
    }

    if (pid == 0) {
        // Child process: set non-zero pParse->pExpr and call the function.
        Fts5Parse pParse;
        Fts5ExprNode exprNode;
        pParse.pExpr = reinterpret_cast<Fts5ExprNode*>(0xDEADBEEF); // non-zero
        // This call should trigger the assert in sqlite3Fts5ParseFinished
        sqlite3Fts5ParseFinished(&pParse, &exprNode);
        // If we reach here, the assertion did not abort, which is a failure for this test.
        _exit(0); // indicate "no abort occurred" (test failure in parent)
    } else {
        // Parent process: wait for child and verify it terminated due to abort
        int status;
        if (waitpid(pid, &status, 0) < 0) {
            std::cerr << "waitpid failed; skipping assertion-path test.\n";
            ++g_failures;
            return;
        }

        // The expected outcome: child terminated due to SIGABRT (from assert)
        if (WIFSIGNALED(status)) {
            int sig = WTERMSIG(status);
            if (sig == SIGABRT) {
                // Test passes: assertion triggered as expected.
            } else {
                std::cerr << "Child terminated due to unexpected signal: " << sig << "\n";
                ++g_failures;
            }
        } else if (WIFEXITED(status)) {
            // Child exited normally (no abort). This means the assertion did not fire,
            // which is a failure for this test path.
            int exitCode = WEXITSTATUS(status);
            (void)exitCode; // suppress unused warning
            std::cerr << "Child exited normally with code " << exitCode
                      << " (expected abnormal termination due to assertion).\n";
            ++g_failures;
        } else {
            std::cerr << "Child terminated abnormally without a recognizable status.\n";
            ++g_failures;
        }
    }
}
#else
void test_ParseFinished_AssertsOnNonZeroExpr() {
    // On Windows, fork is not available. We cannot reliably test the assertion path here
    // without a more elaborate IPC/test framework. Mark as skipped.
    std::cout << "Test ParseFinished_AssertsOnNonZeroExpr skipped on Windows.\n";
}
#endif


// Entry point
int main() {
    std::cout << "Running sqlite3Fts5ParseFinished test suite (C++11 mock harness)\n";

    test_ParseFinished_BasicSetsExpr();
    test_ParseFinished_AssertsOnNonZeroExpr();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << "Total failures: " << g_failures << "\n";
    }

    // Return non-zero if any test failed to aid CI systems.
    return g_failures;
}