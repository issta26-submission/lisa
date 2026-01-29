// Test suite for sqlite3PagerRekey(DbPage *pPg, Pgno iNew, u16 flags)
// This test targets the focal method from pager.c and its minimal dependencies.
// It uses a small, self-contained harness (no GTest) with C linkage to allow
// linking against the actual sqlite3PagerRekey implementation in the production
// codebase. The test uses non-terminating, custom assertions and a mock for
// sqlite3PcacheMove to verify side effects without requiring the full SQLite
// cache subsystem.
//
// Notes based on the provided focal method:
// - The function asserts that pPg->pgno != iNew.
// - It updates pPg->flags to the given flags.
// - It calls sqlite3PcacheMove(pPg, iNew) to move the page in the cache.
//
// Given the assert, testing the false-path (pgno == iNew) would terminate the
// test process. To preserve test execution, we implement an assertion-escape path
// using SIGABRT handling to demonstrate the assertion triggers, while keeping
// the primary (true-path) behavior thoroughly exercised.

#include <csignal>
#include <cstdio>
#include <cinttypes>
#include <sqliteInt.h>
#include <wal.h>
#include <csetjmp>


// Step 1: Minimal, compatible type and function definitions to interface with sqlite3PagerRekey.
// We declare the exact signature as extern "C" to match the production linkage.

extern "C" {
    // Lightweight definitions mirroring the parts of the real SQLite types used by sqlite3PagerRekey.
    typedef unsigned short u16;
    typedef unsigned int Pgno;

    // Test harness' DbPage layout must align with what sqlite3PagerRekey expects
    // for the fields actually accessed (pgno and flags).
    struct DbPage {
        Pgno pgno;
        u16  flags;
        // Additional fields in the real struct are ignored by the focal method
        // (we only rely on pgno and flags for this test).
    };

    // Focal method under test (assumed to be linked from pager.c)
    void sqlite3PagerRekey(DbPage *pPg, Pgno iNew, u16 flags);

    // Dependency that the focal method calls. We provide a test shim below.
    void sqlite3PcacheMove(DbPage *pPg, Pgno iNew);
}

// Global state used by the mock for sqlite3PcacheMove
static int g_move_calls = 0;
static DbPage *g_move_last_pg = nullptr;
static Pgno g_move_last_iNew = 0;

// Mock implementation of sqlite3PcacheMove to capture its usage during tests.
extern "C" void sqlite3PcacheMove(DbPage *pPg, Pgno iNew) {
    g_move_calls++;
    g_move_last_pg = pPg;
    g_move_last_iNew = iNew;
}

// Helpers for assertion-like checks (non-terminating, to maximize code execution)
static int g_all_tests_passed = 1;
#define TEST_OK(cond, msg) do { if(!(cond)) { \
    fprintf(stderr, "TEST FAILED: %s at %s:%d -- %s\n", #cond, __FILE__, __LINE__, msg); \
    g_all_tests_passed = 0; \
} } while(0)

// Global jump buffer and signal handling for catching the abort from assert in the false-path
static jmp_buf g_jmp_buf;
static void sigabrt_handler(int) {
    // Jump back to test harness when an abort is triggered
    longjmp(g_jmp_buf, 1);
}

// Test 1: Happiness path - pPg->pgno != iNew, flags are updated, and sqlite3PcacheMove is invoked correctly.
// Expects that the function:
 // - does not modify pPg->pgno
 // - updates pPg->flags to the provided flags
 // - calls sqlite3PcacheMove(pPg, iNew) exactly once with the right arguments
static bool run_happy_path_test() {
    // Arrange
    DbPage p;
    p.pgno = 5;
    p.flags = 0;

    g_move_calls = 0;
    g_move_last_pg = nullptr;
    g_move_last_iNew = 0;

    // Act
    sqlite3PagerRekey(&p, 7, (u16)0x1234);

    // Assert
    bool ok = true;
    if (p.pgno != 5) {
        fprintf(stderr, "HappyPath: pgno changed unexpectedly (expected 5, got %u)\n", p.pgno);
        ok = false;
    }
    if (p.flags != (u16)0x1234) {
        fprintf(stderr, "HappyPath: flags not updated correctly (expected 0x1234, got 0x%04x)\n", p.flags);
        ok = false;
    }
    if (g_move_calls != 1) {
        fprintf(stderr, "HappyPath: sqlite3PcacheMove call count mismatch (expected 1, got %d)\n", g_move_calls);
        ok = false;
    }
    if (g_move_last_pg != &p) {
        fprintf(stderr, "HappyPath: sqlite3PcacheMove called with wrong DbPage pointer\n");
        ok = false;
    }
    if (g_move_last_iNew != (Pgno)7) {
        fprintf(stderr, "HappyPath: sqlite3PcacheMove called with wrong iNew (expected 7, got %u)\n", g_move_last_iNew);
        ok = false;
    }
    return ok;
}

// Test 2: Assert-path coverage - ensure the false-path (pgno == iNew) triggers an abort.
// We catch the abort via a signal handler and longjmp to the test harness.
// This test demonstrates that the assertion is indeed triggered without crashing the
// entire test suite.
static bool run_assert_failure_test() {
    // Arrange
    DbPage p;
    p.pgno = 7;
    p.flags = 0;

    // Install signal handler for SIGABRT to catch the abort
    struct sigaction old_act, new_act;
    new_act.sa_handler = sigabrt_handler;
    sigemptyset(&new_act.sa_mask);
    new_act.sa_flags = 0;
    if (sigaction(SIGABRT, &new_act, &old_act) != 0) {
        fprintf(stderr, "AssertFailureTest: failed to install SIGABRT handler\n");
        return false;
    }

    // Use setjmp/longjmp to recover from the abort inside sqlite3PagerRekey
    int jmp_rc = setjmp(g_jmp_buf);
    if (jmp_rc == 0) {
        // Act: This should trigger an assertion and abort
        sqlite3PagerRekey(&p, 7, (u16)0xFFFF);
        // If we return here, the abort did not occur as expected
        fprintf(stderr, "AssertFailureTest: expected abort did not occur\n");
        // Restore previous handler for cleanliness
        sigaction(SIGABRT, &old_act, nullptr);
        return false;
    } else {
        // We arrived here via longjmp from the SIGABRT handler
        // Restore previous handler
        sigaction(SIGABRT, &old_act, nullptr);
        return true;
    }
}

// Entry point to run all tests
int main() {
    printf("Starting sqlite3PagerRekey unit tests (C++11, no GTest)\n");

    bool ok1 = run_happy_path_test();
    if (ok1) {
        printf("Test 1 (HappyPath) PASSED\n");
    } else {
        printf("Test 1 (HappyPath) FAILED\n");
    }

    bool ok2 = run_assert_failure_test();
    if (ok2) {
        printf("Test 2 (AssertFailure) PASSED\n");
    } else {
        printf("Test 2 (AssertFailure) FAILED\n");
    }

    // Aggregate results
    int total = 2;
    int passed = (ok1 ? 1 : 0) + (ok2 ? 1 : 0);
    printf("Summary: %d/%d tests passed\n", passed, total);
    return (passed == total) ? 0 : 1;
}

// End of test suite for sqlite3PagerRekey
// The tests rely on the production function sqlite3PagerRekey and a mock for sqlite3PcacheMove.
// They exercise:
// - The true-path behavior (flags update and move cache invocation)
// - The false-path (assertion) behavior via signal handling to avoid terminating the test suite
//   and to demonstrate that the assertion would be triggered under invalid input.