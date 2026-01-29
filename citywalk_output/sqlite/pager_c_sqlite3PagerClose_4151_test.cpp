// C++11 test suite for the SQLite internal function sqlite3PagerClose
// Note: This test uses the public SQLite C API to drive scenarios that exercise
// the pager close logic indirectly via real database operations (open/close).
// No Google Test is used; a lightweight, non-terminating assertion framework is provided.

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <wal.h>


// Lightweight, non-terminating assertion helpers
static int g_failCount = 0;

#define TEST_PRINT(...) do { std::fprintf(stderr, __VA_ARGS__); } while(0)
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { TEST_PRINT("  [FAIL] " msg "\n"); ++g_failCount; } } while(0)
#define EXPECT_EQ(a, b, msg) do { if(((a) != (b))) { TEST_PRINT("  [FAIL] " msg " (expected=%d, actual=%d)\n", (int)(b), (int)(a)); ++g_failCount; } } while(0)
#define EXPECT_STRNEQ(a, b, msg) do { if(std::string(a) == std::string(b)) { TEST_PRINT("  [FAIL] " msg " (strings equal)\n"); ++g_failCount; } } while(0)

// Helper: create a unique temporary database path
static std::string createTempDbPath() {
#ifdef _WIN32
    // Simple Windows-friendly temp path
    char tmpPath[MAX_PATH];
    if(GetTempPathA(MAX_PATH, tmpPath) == 0) {
        return "temp.db";
    }
    std::string path = std::string(tmpPath) + "sqlite_test_" + std::to_string(std::rand()) + ".db";
    return path;
#else
    // POSIX: use mkstemp-like approach with tmpnam
    char tmpName[L_tmpnam];
    if(std::tmpnam(tmpName) == nullptr) {
        return "/tmp/sqlite_test.db";
    }
    // Ensure a .db suffix
    std::string path = tmpName;
    path += ".db";
    // Remove if exists (cleanup)
    std::remove(path.c_str());
    return path;
#endif
}

// Helper: execute SQL and ignore results; returns SQLITE_OK on success
static int execSql(sqlite3* db, const char* sql) {
    char* err = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &err);
    if(err) {
        TEST_PRINT("SQL error: %s\n", err);
        sqlite3_free(err);
    }
    return rc;
}

// Helper: query a single integer result
static bool queryInt(sqlite3* db, const char* sql, int& out) {
    sqlite3_stmt* stmt = nullptr;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    int rc = sqlite3_step(stmt);
    bool ok = false;
    if(rc == SQLITE_ROW) {
        out = sqlite3_column_int(stmt, 0);
        ok = true;
    }
    sqlite3_finalize(stmt);
    return ok;
}

// Test 1: WAL path should be exercised when WAL mode is enabled.
// We verify data persistence across a restart via WAL-enabled journaling.
static void test_pager_close_wal_persistence() {
    TEST_PRINT("Test 1: WAL path close and data persistence\n");
    std::string dbPath = createTempDbPath();
    sqlite3* db = nullptr;

    // Open on disk database
    int rc = sqlite3_open(dbPath.c_str(), &db);
    EXPECT_TRUE(db != nullptr, "Open disk DB should succeed");
    if(db == nullptr) return;

    // Attempt to enable WAL; skip test if WAL not supported
    char* err = nullptr;
    rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL;", nullptr, nullptr, &err);
    if(err) {
        /* If WAL pragma is not supported, skip test rather than fail. */
        TEST_PRINT("  WAL not supported in this build (skipping test).\n");
        sqlite3_free(err);
        sqlite3_close(db);
        return;
    }
    // If rc != SQLITE_OK, skip as WAL not available
    EXPECT_EQ(rc, SQLITE_OK, "Enable WAL mode should succeed");

    // Create a table and insert a value
    rc = sqlite3_exec(db, "CREATE TABLE t(x INTEGER); INSERT INTO t(x) VALUES(123);", nullptr, nullptr, &err);
    if(err){
        TEST_PRINT("  SQL error: %s\n", err);
        sqlite3_free(err);
    }
    EXPECT_TRUE(rc == SQLITE_OK, "Create table and insert in WAL mode should succeed");

    // Close the database (this should close the pager, WAL, etc.)
    sqlite3_close(db);

    // Reopen and verify persistence
    rc = sqlite3_open(dbPath.c_str(), &db);
    EXPECT_TRUE(db != nullptr, "Reopen disk DB after WAL close should succeed");
    if(db == nullptr) {
        // Cleanup temp file
        std::remove(dbPath.c_str());
        return;
    }

    // Query the value
    int value = -1;
    rc = sqlite3_exec(db, "SELECT x FROM t;", nullptr, nullptr, &err);
    if(err){
        TEST_PRINT("  SQL error on select: %s\n", err);
        sqlite3_free(err);
    }
    // We must fetch via prepared statement; simpler to use a SELECT with callback
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT x FROM t;", -1, &stmt, nullptr);
    EXPECT_TRUE(rc == SQLITE_OK, "Prepare select should succeed");
    if(rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        value = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    EXPECT_EQ(value, 123, "Persisted value should be 123 after WAL close and reopen");

    sqlite3_close(db);
    // Cleanup
    std::remove(dbPath.c_str());
}

// Test 2: MEMDB path (in-memory database) should close without persistence
// We verify that data does not persist after closing an in-memory database.
static void test_pager_close_memdb_no_persist() {
    TEST_PRINT("Test 2: MEMDB close should not persist data\n");
    sqlite3* db = nullptr;

    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr, "Open in-memory DB should succeed");
    if(db == nullptr) return;

    // Create table and insert
    char* err = nullptr;
    rc = sqlite3_exec(db, "CREATE TABLE t(y INTEGER); INSERT INTO t(y) VALUES(42);", nullptr, nullptr, &err);
    if(err) {
        TEST_PRINT("  SQL error: %s\n", err);
        sqlite3_free(err);
    }
    EXPECT_TRUE(rc == SQLITE_OK, "In-memory create and insert should succeed");

    // Close in-memory DB
    sqlite3_close(db);

    // Re-open in-memory DB and ensure the table does not exist (no persistence)
    rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr, "Reopen in-memory DB should succeed");
    if(db == nullptr) return;

    // Query for the table; it should not exist
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT name FROM sqlite_master WHERE type='table' AND name='t';", -1, &stmt, nullptr);
    bool tableExists = false;
    if(rc == SQLITE_OK) {
        int step = sqlite3_step(stmt);
        if(step == SQLITE_ROW) {
            tableExists = true;
        }
    }
    sqlite3_finalize(stmt);
    EXPECT_TRUE(!tableExists, "In-memory DB should not persist table across connections");

    sqlite3_close(db);
}

// Test 3: Disk-based database without WAL should persist data across close/reopen
static void test_pager_close_disk_persist_no_wal() {
    TEST_PRINT("Test 3: Disk-based DB without WAL persists data after close/reopen\n");
    std::string dbPath = createTempDbPath();
    sqlite3* db = nullptr;

    int rc = sqlite3_open(dbPath.c_str(), &db);
    EXPECT_TRUE(db != nullptr, "Open disk DB should succeed");
    if(db == nullptr) return;

    // Ensure WAL is not forced
    rc = sqlite3_exec(db, "PRAGMA journal_mode=DELETE;", nullptr, nullptr, nullptr);
    if(rc != SQLITE_OK) {
        // If we can't set mode, proceed anyway; persistence should still hold by default
    }

    // Create table and insert
    char* err = nullptr;
    rc = sqlite3_exec(db, "CREATE TABLE t(z INTEGER); INSERT INTO t(z) VALUES(7);", nullptr, nullptr, &err);
    if(err){
        TEST_PRINT("  SQL error: %s\n", err);
        sqlite3_free(err);
    }
    EXPECT_TRUE(rc == SQLITE_OK, "Create and insert should succeed on disk DB");

    sqlite3_close(db);

    // Re-open and verify
    rc = sqlite3_open(dbPath.c_str(), &db);
    EXPECT_TRUE(db != nullptr, "Reopen disk DB should succeed");
    if(db == nullptr) {
        std::remove(dbPath.c_str());
        return;
    }

    int value = -1;
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT z FROM t;", -1, &stmt, nullptr);
    if(rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
        value = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    EXPECT_EQ(value, 7, "Persisted value should be present after close/reopen (no WAL)");

    sqlite3_close(db);
    std::remove(dbPath.c_str());
}

int main() {
    // Seed for unique temp paths
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Run tests
    test_pager_close_wal_persistence();
    test_pager_close_memdb_no_persist();
    test_pager_close_disk_persist_no_wal();

    // Summary
    if(g_failCount == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << g_failCount << " test(s) failed.\n";
        return 1;
    }
}