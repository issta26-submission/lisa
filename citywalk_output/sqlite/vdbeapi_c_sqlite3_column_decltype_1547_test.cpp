/*
  Candidate Keywords (core dependencies and concepts used by sqlite3_column_decltype):
  - sqlite3_stmt
  - sqlite3_column_decltype
  - COLNAME_DECLTYPE (type indicator used when asking for "declared type" of a column)
  - sqlite3_column_count
  - sqlite3_prepare_v2
  - sqlite3_exec
  - sqlite3_finalize
  - sqlite3_open / sqlite3_close (for resource management in tests if needed)
  - sqlite3_column_decltype16 (alternative UTF-16 variant)
  - The vdbeapi.c function we are targeting is sqlite3_column_decltype, which internally calls a static helper columnName(...)
  - Domain: declared type retrieval for SQLite result-set columns
  - Interaction: sqlite3_stmt (prepared statement), table schema definitions
  - Expected behavior: return a const char* string that represents the declared type of the column in the result-set schema
  - Note: We test against realistic usage using a real SQLite database to exercise the function through the public API.

  Important: This test suite uses the real SQLite C API to exercise sqlite3_column_decltype by creating actual tables
  and prepared statements. It is written in C++11 but uses C APIs from SQLite. It does not depend on GTest;
  it uses a small, non-terminating assertion framework to maximize coverage.
*/

/* 
   This test file provides:
   - A lightweight non-terminating test harness (CHECK macro style) to collect failures without aborting
   - Two test cases:
     1) Basic table with a INT and TEXT column
     2) Another table with REAL and BLOB columns
   - Each test checks that sqlite3_column_decltype(stmt, idx) returns the expected declared type strings
     (allowing for canonical forms used by SQLite, e.g., "INT" vs "INTEGER" where applicable)
   - Real SQLite usage: uses sqlite3_open, sqlite3_exec to create tables, sqlite3_prepare_v2 to prepare statements.
*/

#include <vector>
#include <opcodes.h>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Include the public SQLite API header. Ensure the linked library provides these symbols.

// Minimal non-terminating test framework
namespace test_framework {

// Simple failure recorder
static int g_failure_count = 0;
static std::vector<std::string> g_failure_messages;

static void logFailure(const std::string &msg) {
    g_failure_count++;
    g_failure_messages.push_back(msg);
    std::cerr << "TEST FAILURE: " << msg << std::endl;
}

// Lightweight assertion that does not abort the test run
#define CHECK_TRUE(cond, msg) do { \
    if(!(cond)) { test_framework::logFailure(std::string("CHECK_TRUE failed: ") + (msg)); } \
} while(0)

#define CHECK_STR_EQUAL(expected, actual, msg) do { \
    if((actual) == nullptr) { \
        test_framework::logFailure(std::string("CHECK_STR_EQUAL failed (null actual): ") + (msg)); \
    } else if(std::strcmp((expected), (actual)) != 0) { \
        test_framework::logFailure(std::string("CHECK_STR_EQUAL failed: expected '") + (expected) + "', got '" + (actual) + "'. " + (msg)); \
    } \
} while(0)

static void printSummary() {
    std::cout << "\n==================== Test Summary ====================\n";
    std::cout << "Total failures: " << g_failure_count << "\n";
    if(!g_failure_messages.empty()) {
        std::cout << "Failure details:\n";
        for(const auto &m : g_failure_messages) std::cout << " - " << m << "\n";
    } else {
        std::cout << "All tests passed (no failures reported by non-terminating harness).\n";
    }
    std::cout << "=====================================================\n";
}

} // namespace test_framework

// Convenience alias
using test_framework::CHECK_TRUE;
using test_framework::CHECK_STR_EQUAL;
using test_framework::g_failure_count;

// Test 1: Validate sqlite3_column_decltype on a table with INT and TEXT columns
static void test_decltype_on_simple_table(sqlite3* db) {
    // Create a simple table with explicit declared types
    const char *zCreate1 = "CREATE TABLE t1(a INT, b TEXT);";
    char *err = nullptr;
    int rc = sqlite3_exec(db, zCreate1, nullptr, nullptr, &err);
    if(rc != SQLITE_OK) {
        test_framework::logFailure(std::string("setup failed in test_decltype_on_simple_table: ") + (err ? err : "unknown"));
        // If creation failed, bail this test gracefully
        if(err) sqlite3_free(err);
        return;
    }
    // Prepare a statement that selects from the table
    const char *zQuery = "SELECT a, b FROM t1";
    sqlite3_stmt *pStmt = nullptr;
    rc = sqlite3_prepare_v2(db, zQuery, -1, &pStmt, nullptr);
    if(rc != SQLITE_OK || pStmt == nullptr) {
        test_framework::logFailure("prepare failed in test_decltype_on_simple_table");
        if(pStmt) sqlite3_finalize(pStmt);
        return;
    }

    // sqlite3_column_decltype returns the declared type of the result column
    const char *pDecl0 = sqlite3_column_decltype(pStmt, 0);
    const char *pDecl1 = sqlite3_column_decltype(pStmt, 1);

    // The exact textual representation is implementation-defined, but SQLite preserves the declared type
    // In our setup, a INT should appear as "INT" (or possibly "INTEGER"); TEXT should appear as "TEXT".
    bool ok0 = false;
    if(pDecl0 != nullptr) {
        if(std::strcmp(pDecl0, "INT") == 0) ok0 = true;
        if(std::strcmp(pDecl0, "INTEGER") == 0) ok0 = true;
    }
    bool ok1 = false;
    if(pDecl1 != nullptr) {
        if(std::strcmp(pDecl1, "TEXT") == 0) ok1 = true;
    }

    CHECK_TRUE(ok0, "Expected column 0 decltype to be INT or INTEGER in test_decltype_on_simple_table");
    CHECK_TRUE(ok1, "Expected column 1 decltype to be TEXT in test_decltype_on_simple_table");

    // Cleanup
    sqlite3_finalize(pStmt);
    // Note: Tables persist only for the lifetime of db; will be closed by caller
}

// Test 2: Validate sqlite3_column_decltype on a table with REAL and BLOB columns
static void test_decltype_on_real_blob_table(sqlite3* db) {
    // Create a second table with REAL and BLOB
    const char *zCreate2 = "CREATE TABLE t2(x REAL, y BLOB);";
    char *err = nullptr;
    int rc = sqlite3_exec(db, zCreate2, nullptr, nullptr, &err);
    if(rc != SQLITE_OK) {
        test_framework::logFailure(std::string("setup failed in test_decltype_on_real_blob_table: ") + (err ? err : "unknown"));
        if(err) sqlite3_free(err);
        return;
    }

    // Prepare a statement that selects from the second table
    const char *zQuery = "SELECT x, y FROM t2";
    sqlite3_stmt *pStmt = nullptr;
    rc = sqlite3_prepare_v2(db, zQuery, -1, &pStmt, nullptr);
    if(rc != SQLITE_OK || pStmt == nullptr) {
        test_framework::logFailure("prepare failed in test_decltype_on_real_blob_table");
        if(pStmt) sqlite3_finalize(pStmt);
        return;
    }

    const char *pDecl0 = sqlite3_column_decltype(pStmt, 0);
    const char *pDecl1 = sqlite3_column_decltype(pStmt, 1);

    bool ok0 = false;
    if(pDecl0 != nullptr) {
        // For REAL, SQLite typically uses "REAL" as the declared type
        if(std::strcmp(pDecl0, "REAL") == 0) ok0 = true;
        // Some schemas may canonicalize to "DOUBLE" in rare cases; accept if encountered
        if(std::strcmp(pDecl0, "DOUBLE") == 0) ok0 = true;
        // Also accept "FLOAT" if that is what appears in some environments
        if(std::strcmp(pDecl0, "FLOAT") == 0) ok0 = true;
    }

    bool ok1 = false;
    if(pDecl1 != nullptr) {
        if(std::strcmp(pDecl1, "BLOB") == 0) ok1 = true;
        // Some environments may report "BINARY" for BLOB alias; be permissive
        if(std::strcmp(pDecl1, "BINARY") == 0) ok1 = true;
    }

    CHECK_TRUE(ok0, "Expected column 0 decltype to be REAL/DOUBLE/FLOAT in test_decltype_on_real_blob_table");
    CHECK_TRUE(ok1, "Expected column 1 decltype to be BLOB/BINARY in test_decltype_on_real_blob_table");

    // Cleanup
    sqlite3_finalize(pStmt);
}

// Entry point to run tests
int main() {
    // Domain knowledge: Use in-memory database for isolated tests
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if(rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Failed to open in-memory SQLite database. rc=" << rc << "\n";
        return 1;
    }

    // Run tests
    test_decltype_on_simple_table(db);
    test_decltype_on_real_blob_table(db);

    // Finalize and close
    sqlite3_close(db);

    // Print summary and return non-zero if any failures occurred
    test_framework::printSummary();
    return (g_failure_count > 0) ? 2 : 0;
}