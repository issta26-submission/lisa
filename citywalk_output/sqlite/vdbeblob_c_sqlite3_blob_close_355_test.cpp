// Test suite for sqlite3_blob_close implemented in vdbeblob.c
// This test is designed to work with a C++11 build that links against the SQLite library.
// It does not use GTest; instead, it provides a lightweight, non-terminating test harness
// that reports pass/fail results to stdout and continues execution to maximize coverage.

#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Global test failure counter
static int g_test_failures = 0;

// Simple non-terminating assertion reporter
static void report_result(const std::string& test_name, bool passed)
{
    if (passed) {
        std::cout << "[PASSED] " << test_name << std::endl;
    } else {
        std::cout << "[FAILED] " << test_name << std::endl;
        ++g_test_failures;
    }
}

// Helper: execute a statement and return whether it succeeded
static bool exec_sql(sqlite3* db, const std::string& sql)
{
    char *errmsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        if (errmsg) {
            std::cerr << "SQL error: " << errmsg << " (while executing: " << sql << ")" << std::endl;
            sqlite3_free(errmsg);
        } else {
            std::cerr << "SQL error (unknown) (while executing: " << sql << ")" << std::endl;
        }
        return false;
    }
    return true;
}

// Test 1: Ensure non-null blob pointer path closes correctly.
// Steps:
//  - Create an in-memory database and a table with a BLOB column.
//  - Insert a row with a blob value.
//  - Open a blob using sqlite3_blob_open to obtain a sqlite3_blob* backed by an Incrblob.
//  - Call sqlite3_blob_close on the obtained blob and expect SQLITE_OK.
// Expected: sqlite3_blob_close returns SQLITE_OK for a valid blob handle.
static void test_blob_close_with_valid_blob(sqlite3* db)
{
    const std::string test_name = "sqlite3_blob_close with valid blob";

    // Create table and insert a blob value
    if (!exec_sql(db, "CREATE TABLE IF NOT EXISTS t (id INTEGER PRIMARY KEY, b BLOB);"
                       "INSERT INTO t(b) VALUES (X'01020304');")) {
        report_result(test_name, false);
        return;
    }

    // Open a blob on row id = 1, column b
    sqlite3_blob *blob = nullptr;
    int rc_open = sqlite3_blob_open(db, "main", "t", "b", 1, 0, &blob);
    if (rc_open != SQLITE_OK || blob == nullptr) {
        std::cerr << "Failed to open blob for test: rc_open=" << rc_open
                  << ", blob=" << (blob ? "non-null" : "null") << std::endl;
        report_result(test_name, false);
        return;
    }

    // Now close the blob using the function under test
    int rc_close = sqlite3_blob_close(blob);
    if (rc_close == SQLITE_OK) {
        report_result(test_name, true);
    } else {
        std::cerr << "sqlite3_blob_close returned error code: " << rc_close << std::endl;
        report_result(test_name, false);
    }
}

// Test 2: Ensure NULL blob pointer path returns SQLITE_OK.
// Steps:
//  - Call sqlite3_blob_close with a NULL pointer and expect SQLITE_OK.
// Expected: sqlite3_blob_close(NULL) should return SQLITE_OK and not crash.
static void test_blob_close_with_null_pointer()
{
    const std::string test_name = "sqlite3_blob_close with NULL pointer";

    int rc = sqlite3_blob_close(nullptr);
    report_result(test_name, rc == SQLITE_OK);
}

// Entry point for tests
int main()
{
    // Initialize in-memory database for the blob-close tests
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Failed to open in-memory SQLite database. rc=" << rc << std::endl;
        return 1;
    }

    // Run tests
    test_blob_close_with_valid_blob(db);
    test_blob_close_with_null_pointer();

    // Cleanup
    sqlite3_close(db);

    // Summary
    if (g_test_failures == 0) {
        std::cout << "[SUMMARY] All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "[SUMMARY] Failures: " << g_test_failures << std::endl;
        return 1;
    }
}