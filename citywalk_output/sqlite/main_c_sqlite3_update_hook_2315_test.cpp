// C++11 test suite for the focal method: sqlite3_update_hook
// This test suite exercises the behavior of sqlite3_update_hook as defined
// in the focal code. It uses the public SQLite C API to drive realistic
// scenarios and verifies the hook installation, replacement, invocation
// during updates, and safety-armor behavior when a NULL db is passed.
//
// Notes:
// - This test assumes the SQLite library is available and accessible via -lsqlite3.
// - The tests rely on the public API: sqlite3_open, sqlite3_exec, sqlite3_update_hook, sqlite3_close, etc.
// - We avoid terminating assertions; instead, we log PASS/FAIL and continue to maximize coverage.

#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Include the public SQLite API

// Global state captured by the update hook callback to verify invocation details
static bool g_updateInvoked = false;
static void* g_lastArg = nullptr;
static int g_lastOp = -1;
static std::string g_lastDb;
static std::string g_lastTable;
static sqlite3_int64 g_lastRowid = 0;

// The update hook callback used in tests
static void testUpdateHook(void* pArg, int op, const char* zDb, const char* zName, sqlite3_int64 rowid)
{
    g_updateInvoked = true;
    g_lastArg = pArg;
    g_lastOp = op;
    g_lastDb = zDb ? zDb : "";
    g_lastTable = zName ? zName : "";
    g_lastRowid = rowid;
}

// Simple non-terminating assertion helper
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << msg << std::endl; \
        failures++; \
    } else { \
        std::cout << "[PASS] " << msg << std::endl; \
    } \
} while(0)

static int failures = 0;

// Helper: reset global hook-tracking state before each test
static void resetHookState()
{
    g_updateInvoked = false;
    g_lastArg = nullptr;
    g_lastOp = -1;
    g_lastDb.clear();
    g_lastTable.clear();
    g_lastRowid = 0;
}

// Helper: execute an SQL statement and return whether it succeeded
static bool execSql(sqlite3* db, const char* sql)
{
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if(rc != SQLITE_OK){
        // Print the error for debugging purposes
        if(errMsg){
            std::cerr << "SQL error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        } else {
            std::cerr << "SQL error: code " << rc << std::endl;
        }
        return false;
    }
    return true;
}

// Test 1: Basic hook installation should return NULL (no previous arg)
static void test_basic_hook_install_returns_null()
{
    resetHookState();

    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(rc == SQLITE_OK, "test1: sqlite3_open success");

    void* prev = sqlite3_update_hook(db, testUpdateHook, (void*)0x1234);
    ASSERT_TRUE(prev == nullptr, "test1: initial update_hook should return NULL (no previous arg)");

    // Clean up
    sqlite3_close(db);
}

// Test 2: Replacing the hook should return the previous arg value
static void test_hook_replace_returns_previous()
{
    resetHookState();

    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(rc == SQLITE_OK, "test2: sqlite3_open success");

    // First installation
    void* prev1 = sqlite3_update_hook(db, testUpdateHook, (void*)0x1234);
    ASSERT_TRUE(prev1 == nullptr, "test2: first hook returns NULL");

    // Second installation with a different arg
    void* prev2 = sqlite3_update_hook(db, testUpdateHook, (void*)0x5678);
    ASSERT_TRUE(prev2 == (void*)0x1234, "test2: second hook returns previous arg (0x1234)");

    sqlite3_close(db);
}

// Test 3: Hook invocation on an actual UPDATE operation and arg propagation
static void test_hook_invoked_on_update()
{
    resetHookState();

    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(rc == SQLITE_OK, "test3: sqlite3_open success");

    // Install hook with a known arg
    void* argValue = (void*)0xDEAD;
    void* prev = sqlite3_update_hook(db, testUpdateHook, argValue);
    ASSERT_TRUE(prev == nullptr, "test3: initial hook returns NULL");

    // Create a table and perform an update to trigger the hook
    ASSERT_TRUE(execSql(db, "CREATE TABLE t(i INTEGER);"), "test3: create table t");
    ASSERT_TRUE(execSql(db, "INSERT INTO t(i) VALUES(1);"), "test3: insert row");
    // Update the only row; this should trigger the hook
    ASSERT_TRUE(execSql(db, "UPDATE t SET i = i + 1 WHERE i = 1;"), "test3: perform update to trigger hook");

    // Verify callback was invoked with expected details
    ASSERT_TRUE(g_updateInvoked, "test3: update hook invoked");

    // Arg propagation
    ASSERT_TRUE(g_lastArg == argValue, "test3: callback received correct arg (pArg passed to hook)");
    // Operation code should be SQLITE_UPDATE
    ASSERT_TRUE(g_lastOp == SQLITE_UPDATE, "test3: callback op is SQLITE_UPDATE");
    // Table name should be "t" and DB "main"
    ASSERT_TRUE(g_lastDb == std::string("main"), "test3: callback db is 'main'");
    ASSERT_TRUE(g_lastTable == std::string("t"), "test3: callback table is 't'");
    ASSERT_TRUE(g_lastRowid > 0, "test3: callback rowid > 0");

    sqlite3_close(db);
}

// Test 4: Removing the hook (setting xCallback to NULL) should stop future callbacks
static void test_hook_removed_stops_invocation()
{
    resetHookState();

    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(rc == SQLITE_OK, "test4: sqlite3_open success");

    void* argValue = (void*)0xBEEF;
    void* prev = sqlite3_update_hook(db, testUpdateHook, argValue);
    ASSERT_TRUE(prev == nullptr, "test4: initial hook returns NULL");

    // Now remove the hook by setting NULL callback
    void* prevAfterRemoval = sqlite3_update_hook(db, nullptr, nullptr);
    ASSERT_TRUE(prevAfterRemoval == argValue, "test4: replacing with NULL callback returns previous arg");

    // Reset callback invocation flag and perform an update
    g_updateInvoked = false;
    ASSERT_TRUE(execSql(db, "CREATE TABLE t2(i INTEGER);"), "test4: create table t2");
    ASSERT_TRUE(execSql(db, "INSERT INTO t2(i) VALUES(1);"), "test4: insert row into t2");
    ASSERT_TRUE(execSql(db, "UPDATE t2 SET i = i + 1 WHERE i = 1;"), "test4: perform update on t2");

    // No callback should have occurred
    ASSERT_TRUE(!g_updateInvoked, "test4: no callback invoked after hook removal");

    sqlite3_close(db);
}

// Test 5: Safety check: calling with NULL db should return NULL (API armor path)
static void test_safety_check_with_null_db()
{
    // If API_ARMOR is enabled in the focal build, this should return NULL without crash
    void* ret = sqlite3_update_hook(nullptr, testUpdateHook, (void*)0xFACE);
    bool ok = (ret == nullptr);
    ASSERT_TRUE(ok, "test5: safety check with NULL db returns NULL (no crash)");
}

// Entry point to run all tests
int main()
{
    std::cout << "Starting sqlite3_update_hook unit tests (C++11, no GTest)..." << std::endl;

    test_basic_hook_install_returns_null();
    test_hook_replace_returns_previous();
    test_hook_invoked_on_update();
    test_hook_removed_stops_invocation();
    test_safety_check_with_null_db();

    if(failures == 0) {
        std::cout << "[ALL TESTS PASSED] sqlite3_update_hook behavior is correct." << std::endl;
        return 0;
    } else {
        std::cout << "[TESTS COMPLETED] Failures: " << failures << std::endl;
        return 1;
    }
}