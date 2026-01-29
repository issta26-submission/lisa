// Test suite for the focal method sqlite3BtreeCursorUnpin
// This C++11 test file acts as a lightweight test harness (no GTest).
// It relies on the project's internal BtCursor type and BTCF_Pinned flag
// exposed via btreeInt.h. Tests are designed to be executable in a
// standalone manner and aim to maximize code coverage with non-terminating
// assertions wherever feasible. Where a true assertion would terminate the
// test, we isolate that path in a separate process to observe abort behavior.

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <btreeInt.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>


// Step 1: Candidate Keywords (extracted from the focal method)
// - BtCursor: the cursor structure used by the B-Tree implementation
// - curFlags: a field inside BtCursor that stores status flags
// - BTCF_Pinned: a flag indicating the cursor is pinned
// - sqlite3BtreeCursorUnpin: the function under test, which clears BTCF_Pinned

// Import the actual BtCursor type and BTCF_Pinned from the project headers.
// The header is C and uses its own internal definitions. We include it in an
// extern "C" block to prevent C++ name mangling.
extern "C" {
}

// Declare the focal function explicitly for linkage, assuming C linkage.
extern "C" void sqlite3BtreeCursorUnpin(BtCursor *pCur);

// Helper: basic boolean test result print
static void printResult(const char* testName, bool passed) {
    if (passed) {
        std::cout << "[OK] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

// Test 1: True branch - ensure that when pinned, Unpin clears BTCF_Pinned
// This covers the primary, non-error path of the function.
static bool test_true_branch_basic() {
    // Prepare a cursor that is pinned
    BtCursor cur;
    cur.curFlags = BTCF_Pinned;

    // Call the function under test
    sqlite3BtreeCursorUnpin(&cur);

    // Assert that BTCF_Pinned bit is cleared and other flags are untouched
    bool isCleared = ((cur.curFlags & BTCF_Pinned) == 0);
    bool otherFlagsIntact = (cur.curFlags & ~BTCF_Pinned) == cur.curFlags;

    // We expect BTCF_Pinned to be cleared; other bits should remain (none here)
    return isCleared && otherFlagsIntact;
}

// Test 2: True branch (with extra flags) - ensure other bits are preserved while clearing pinned
// Verifies non-destructive behavior of clearing only the pinned flag.
static bool test_true_branch_with_extra_flag() {
    BtCursor cur;
    cur.curFlags = BTCF_Pinned | 0x2;  // add an extra flag to test preservation

    sqlite3BtreeCursorUnpin(&cur);

    // The pinned bit should be cleared, the extra flag (0x2) should remain
    bool pinnedCleared = (cur.curFlags & BTCF_Pinned) == 0;
    bool extraFlagRemains = (cur.curFlags & 0x2) != 0;

    return pinnedCleared && extraFlagRemains;
}

// Test 3: False branch - ensure that the function asserts when the cursor is not pinned
// The assert() would terminate the process; we isolate this in a child process
// so the parent can observe the abort without terminating the test suite.
static bool test_false_branch_assertion_triggered() {
    pid_t pid = fork();
    if (pid == -1) {
        // Fork failed; consider as test failure
        return false;
    } else if (pid == 0) {
        // Child process: this should trigger an assertion and abort
        BtCursor cur;
        cur.curFlags = 0;  // not pinned

        // This call is expected to trigger an assertion failure (abort)
        sqlite3BtreeCursorUnpin(&cur);

        // If it returns, the assertion did not terminate (unexpected)
        _exit(0);
    } else {
        // Parent process: wait for child to terminate or abort
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            return false;
        }

        // If the child was terminated by a signal corresponding to an assertion (SIGABRT)
        // we consider the test as having observed the expected abort.
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT) {
            return true;
        }

        // If the child exited normally or was killed by another signal, it's a failure for this test
        return false;
    }
}

// Main test runner
int main() {
    int total = 0;
    int passed = 0;

    // Test 1: basic true path
    bool t1 = test_true_branch_basic();
    total++; if (t1) { passed++; printResult("test_true_branch_basic", true); } else { printResult("test_true_branch_basic", false); }

    // Test 2: true path with extra flag
    bool t2 = test_true_branch_with_extra_flag();
    total++; if (t2) { passed++; printResult("test_true_branch_with_extra_flag", true); } else { printResult("test_true_branch_with_extra_flag", false); }

    // Test 3: false path (assertion) - isolated in child process
    bool t3 = test_false_branch_assertion_triggered();
    total++; if (t3) { passed++; printResult("test_false_branch_assertion_triggered", true); } else { printResult("test_false_branch_assertion_triggered", false); }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}