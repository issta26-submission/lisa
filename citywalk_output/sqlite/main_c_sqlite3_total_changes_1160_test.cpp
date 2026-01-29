// Test suite for the focal method sqlite3_total_changes using a lightweight C++11 harness.
// The tests exercise the SQLite C API via an in-memory database and verify
// that sqlite3_total_changes(db) correctly reflects changes made by SQL statements.
// No GTest is used; a small, non-terminating assertion mechanism is implemented.

#include <sqlite3.h>
#include <iostream>
#include <sqliteInt.h>
#include <string>


// Lightweight test harness (non-terminating assertions)
static int g_test_failures = 0;

static void test_assert(bool cond, const std::string &msg) {
    if (!cond) {
        std::cerr << "ASSERT FAILED: " << msg << std::endl;
        ++g_test_failures;
    } else {
        // Optional: uncomment to get per-test success messages
        // std::cout << "PASS: " << msg << std::endl;
    }
}

// Helper: run a single SQL statement (ignores result). Returns true on success.
static bool run_sql(sqlite3* db, const char* sql) {
    char* errmsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        if (errmsg) {
            std::cerr << "SQLite error: " << errmsg << " (while executing: " << sql << ")" << std::endl;
            sqlite3_free(errmsg);
        } else {
            std::cerr << "SQLite error: code " << rc << " (while executing: " << sql << ")" << std::endl;
        }
        return false;
    }
    return true;
}

// Test 1: Validate that a freshly opened in-memory database reports zero total changes.
static void test_initially_zero() {
    // Purpose: sqlite3_total_changes should be 0 on a brand-new DB connection.
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    test_assert(rc == SQLITE_OK && db != nullptr, "Open in-memory DB should succeed");

    int total = sqlite3_total_changes(db);
    test_assert(total == 0, "Initial total_changes should be 0 for a new DB");
    test_assert(total == (int)sqlite3_total_changes64(db), "total_changes should match 64-bit version on new DB");

    sqlite3_close(db);
}

// Test 2: After a single INSERT, total_changes should increment by 1.
static void test_single_insert() {
    // Purpose: Ensure that one insertion yields a single change reported by sqlite3_total_changes.
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    test_assert(rc == SQLITE_OK && db != nullptr, "Open in-memory DB for single-insert test should succeed");

    // Create a simple table
    test_assert(run_sql(db, "CREATE TABLE t (id INTEGER PRIMARY KEY, v TEXT);"), "CREATE TABLE should succeed");

    // Perform a single INSERT
    test_assert(run_sql(db, "INSERT INTO t (v) VALUES ('first');"), "Single INSERT should succeed");

    int total = sqlite3_total_changes(db);
    test_assert(total == 1, "Total changes after one INSERT should be 1");
    test_assert(total == (int)sqlite3_total_changes64(db), "total_changes64 should match total_changes after one INSERT");

    sqlite3_close(db);
}

// Test 3: After multiple operations (two inserts followed by an update affecting both rows), total_changes should accumulate correctly.
static void test_multiple_operations() {
    // Purpose: Verify cumulative behavior across multiple statements:
    // - Two separate INSERTs -> +2 changes
    // - An UPDATE affecting both rows -> +2 changes
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    test_assert(rc == SQLITE_OK && db != nullptr, "Open in-memory DB for multiple operations test should succeed");

    test_assert(run_sql(db, "CREATE TABLE t (id INTEGER PRIMARY KEY, v TEXT);"), "CREATE TABLE should succeed");

    // First INSERT
    test_assert(run_sql(db, "INSERT INTO t (v) VALUES ('A');"), "First INSERT should succeed");
    // Second INSERT
    test_assert(run_sql(db, "INSERT INTO t (v) VALUES ('B');"), "Second INSERT should succeed");

    int total = sqlite3_total_changes(db);
    test_assert(total == 2, "Total changes after two INSERTs should be 2");

    // Update both rows (modify value; should affect 2 rows)
    test_assert(run_sql(db, "UPDATE t SET v = 'C' WHERE id IN (SELECT id FROM t);"), "UPDATE affecting both rows should succeed");

    total = sqlite3_total_changes(db);
    test_assert(total == 4, "Total changes after 2 INSERTs and an UPDATE should be 4");
    test_assert(total == (int)sqlite3_total_changes64(db), "total_changes64 should match after updates");

    sqlite3_close(db);
}

// Test 4: After closing and reopening a database, total_changes should reset to 0 for a new connection.
static void test_reset_after_close() {
    // Purpose: Ensure that sqlite3_total_changes is per-connection and resets on a new connection.
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    test_assert(rc == SQLITE_OK && db != nullptr, "Open in-memory DB for reset test should succeed");

    int total = sqlite3_total_changes(db);
    test_assert(total == 0, "Total changes on fresh DB should be 0");

    sqlite3_close(db);

    // Reopen a new connection and verify total_changes is again 0
    sqlite3* db2 = nullptr;
    rc = sqlite3_open(":memory:", &db2);
    test_assert(rc == SQLITE_OK && db2 != nullptr, "Reopen in-memory DB should succeed");
    total = sqlite3_total_changes(db2);
    test_assert(total == 0, "Total changes on newly opened DB should be 0 in a new connection");
    sqlite3_close(db2);
}

// Entry point: run all tests and summarize results.
int main() {
    std::cout << "Starting sqlite3_total_changes unit tests (C++11 harness)" << std::endl;

    test_initially_zero();
    test_single_insert();
    test_multiple_operations();
    test_reset_after_close();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED: sqlite3_total_changes" << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " TEST(S) FAILED for sqlite3_total_changes" << std::endl;
        return 1;
    }
}