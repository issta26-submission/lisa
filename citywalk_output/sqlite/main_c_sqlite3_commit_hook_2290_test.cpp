// Unit tests for sqlite3_commit_hook (focal method) using a real SQLite3 API.
// This test suite is written in C++11 without any external testing framework.
// It relies on the public SQLite API to exercise the commit hook behavior.
// Note: This test focuses on the observable API behavior (hook invocation and
// return values) rather than internal static/private state.
// To run this, link with the system-provided SQLite library (e.g., -lsqlite3).

#include <sqlite3.h>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Simple non-terminating assertion helper to accumulate test results.
static int tests_passed = 0;
static int tests_failed = 0;

static void expect_true(bool cond, const char* msg) {
    if (cond) {
        ++tests_passed;
    } else {
        ++tests_failed;
        std::cerr << "EXPECT_TRUE failed: " << msg << std::endl;
    }
}

// Global helpers for hook behavior tracking in tests using the real SQLite API.
static int g_hookCalls = 0;
static void* g_lastHookArg = nullptr;

// Simple commit hook function used in tests to verify invocation
static int testCommitHook(void* pArg) {
    ++g_hookCalls;
    g_lastHookArg = pArg;
    return 0;
}

// A second hook function to test the "old callback" return value behavior
static int testCommitHookAlt(void* /*pArg*/) {
    // Intentionally does nothing besides existing hook mechanics
    return 0;
}

// Test 1: Basic contract
// - Attaching a commit hook should return the previous hook (NULL initially).
// - A committed transaction should trigger the hook exactly once.
// - The argument passed to the hook should be exactly what was provided at attach time.
static bool test_commit_hook_basic_behavior() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Failed to open in-memory DB: " << sqlite3_errmsg(db) << std::endl;
        if (db) sqlite3_close(db);
        return false;
    }

    // Reset tracking state
    g_hookCalls = 0;
    g_lastHookArg = nullptr;

    // Attach the commit hook for the first time
    void* expectedOld = sqlite3_commit_hook(db, testCommitHook, (void*)0xDEADC0DE);
    // Initially, there was no previous hook
    expect_true(expectedOld == nullptr, "Initial commit_hook should return NULL as old hook");

    // Ensure we pass a distinct argument to the hook and that it propagates to the hook
    // Prepare a transaction that will commit
    const char* sql = "CREATE TABLE t(x); BEGIN; INSERT INTO t(x) VALUES(1); COMMIT;";
    char* errmsg = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        if (errmsg) sqlite3_free(errmsg);
        std::cerr << "Transaction commit failed: " << (errmsg ? errmsg : "unknown") << std::endl;
        sqlite3_close(db);
        return false;
    }

    // The commit hook should have been called exactly once
    expect_true(g_hookCalls == 1, "Commit hook should be invoked once on commit");
    // The pArg provided to the commit hook should be the one we passed
    expect_true(g_lastHookArg == (void*)0xDEADC0DE, "Commit hook argument should match the one supplied at attach time");

    // Clean up
    sqlite3_close(db);
    // If no failures reported, this test passes
    return true;
}

// Test 2: Old-return behavior when attaching a new hook
// - After attaching a new hook, the return value should be the previously attached hook.
static bool test_commit_hook_old_return_value() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Failed to open in-memory DB: " << sqlite3_errmsg(db) << std::endl;
        if (db) sqlite3_close(db);
        return false;
    }

    // Attach first hook (hookA)
    void* old1 = sqlite3_commit_hook(db, testCommitHook, nullptr);
    // There should be no previous hook
    if (old1 != nullptr) {
        std::cerr << "Expected NULL as old hook on first attach, got: " << old1 << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Attach second hook (hookAlt) and verify that old hook is hookA (i.e., testCommitHook)
    void* old2 = sqlite3_commit_hook(db, testCommitHookAlt, nullptr);
    // Expect old2 to be the previously installed hook (testCommitHook)
    // Since function pointers compare by address, compare to the known address of testCommitHook
    extern int testCommitHook(void*); // forward declare to take address as pointer
    // We can't reliably compare function pointers across translation units without the address;
    // However, within this test compilation unit, testCommitHook's address should be stable.
    bool oldMatchesTestHook = (old2 == (void*)testCommitHook);
    expect_true(oldMatchesTestHook, "Second attach should return first hook as old value");

    // Clean up
    sqlite3_close(db);
    return true;
}

// Main entry to run tests
int main() {
    std::cout << "Starting sqlite3_commit_hook unit tests (C++11, no GTest)..." << std::endl;

    bool r1 = test_commit_hook_basic_behavior();
    std::cout << "[Test 1] basic_behavior: " << (r1 ? "PASS" : "FAIL") << std::endl;

    bool r2 = test_commit_hook_old_return_value();
    std::cout << "[Test 2] old_return_value: " << (r2 ? "PASS" : "FAIL") << std::endl;

    int total = tests_passed + tests_failed;
    std::cout << "Tests run: " << total
              << ", Passed: " << tests_passed
              << ", Failed: " << tests_failed << std::endl;

    return (tests_failed == 0) ? 0 : 1;
}