// Test suite for sqlite3_prefixes_init (prefixes.c) using plain C++11 (no GTest).
// The tests rely on the SQLite3 C API and link against the provided prefixes module.
// The tests are written to be non-terminating on individual assertion failures,
// recording failures and continuing execution as requested.

#include <functional>
#include <string.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <iostream>
#include <cstdlib>


// Ensure we can declare the focal C function with C linkage for the C++ tester.
extern "C" int sqlite3_prefixes_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
);

// Global test failure counter to enable non-terminating test flow.
static int g_testFailures = 0;

// Simple non-terminating assertion macro for test reporting.
#define TS_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test Failure: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_testFailures; \
    } \
} while(0)

// Helper to safely open an in-memory SQLite database.
static sqlite3* openInMemoryDb() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TS_ASSERT(rc == SQLITE_OK && db != nullptr, "Failed to open in-memory SQLite database");
    return db;
}

// Test 1:
// Verifies that sqlite3_prefixes_init returns SQLITE_OK on a fresh in-memory DB
// and that the function "prefix_length" is registered (i.e., can be used in a SQL query).
static void test_prefixes_init_registers_module_and_function() {
    std::cout << "Running test_prefixes_init_registers_module_and_function..." << std::endl;

    sqlite3* db = openInMemoryDb();
    TS_ASSERT(db != nullptr, "Database handle should be non-null after open");

    char* errMsg = nullptr;
    int rc = sqlite3_prefixes_init(db, &errMsg, nullptr);
    TS_ASSERT(rc == SQLITE_OK, "sqlite3_prefixes_init should return SQLITE_OK on fresh DB");
    if(errMsg != nullptr) {
        // The init function may set an error message; print but do not terminate the test.
        std::cerr << "Warning: sqlite3_prefixes_init reported error msg: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    // Ensure the function prefix_length is registered by attempting to call it via SQL.
    const char* sql = "SELECT prefix_length('abcdef', 'abc')";
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    TS_ASSERT(rc == SQLITE_OK && stmt != nullptr, "Preparing SQL calling prefix_length should succeed if function is registered");

    rc = sqlite3_step(stmt);
    TS_ASSERT(rc == SQLITE_ROW || rc == SQLITE_DONE, "Stepping over function call should yield a row or completion");

    // If a row is returned, fetch the column type to avoid crashes on NULLs.
    if(rc == SQLITE_ROW) {
        int colType = sqlite3_column_type(stmt, 0);
        (void)colType; // We do not depend on its content for this test.
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << "test_prefixes_init_registers_module_and_function completed." << std::endl;
}

// Test 2:
// Verifies that calling sqlite3_prefixes_init twice on the same database returns an error
// indicating that the module name "prefixes" is already registered.
static void test_prefixes_init_duplicate_module_on_same_db_errors() {
    std::cout << "Running test_prefixes_init_duplicate_module_on_same_db_errors..." << std::endl;

    sqlite3* db = openInMemoryDb();
    TS_ASSERT(db != nullptr, "Database handle should be non-null after open");

    char* errMsg = nullptr;
    int rc = sqlite3_prefixes_init(db, &errMsg, nullptr);
    TS_ASSERT(rc == SQLITE_OK, "First sqlite3_prefixes_init should return SQLITE_OK on fresh DB");

    // Attempt to initialize again on the same database; this should fail due to duplicate module name.
    rc = sqlite3_prefixes_init(db, &errMsg, nullptr);
    TS_ASSERT(rc == SQLITE_ERROR, "Second sqlite3_prefixes_init on same DB should return SQLITE_ERROR due to existing module");

    if(errMsg != nullptr) {
        // If an error message was produced, print it for diagnostic purposes but do not fail the test.
        std::cerr << "Note: sqlite3_prefixes_init duplicate error message: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    std::cout << "test_prefixes_init_duplicate_module_on_same_db_errors completed." << std::endl;
}

// Test 3 (Domain-aligned coverage):
// Verifies that two independent in-memory databases can each initialize prefixes
// without interfering with one another (sanity check for per-DB module scoping).
static void test_prefixes_init_is_per_database() {
    std::cout << "Running test_prefixes_init_is_per_database..." << std::endl;

    // First DB
    sqlite3* db1 = openInMemoryDb();
    TS_ASSERT(db1 != nullptr, "db1 should be opened");
    int rc1 = sqlite3_prefixes_init(db1, nullptr, nullptr);
    TS_ASSERT(rc1 == SQLITE_OK, "db1 should initialize prefixes without error");
    sqlite3_close(db1);

    // Second DB
    sqlite3* db2 = openInMemoryDb();
    TS_ASSERT(db2 != nullptr, "db2 should be opened");
    int rc2 = sqlite3_prefixes_init(db2, nullptr, nullptr);
    TS_ASSERT(rc2 == SQLITE_OK, "db2 should initialize prefixes without error");
    sqlite3_close(db2);

    std::cout << "test_prefixes_init_is_per_database completed." << std::endl;
}

int main() {
    std::cout << "Starting sqlite3_prefixes_init test suite (C++11, no GTest)..." << std::endl;

    test_prefixes_init_registers_module_and_function();
    test_prefixes_init_duplicate_module_on_same_db_errors();
    test_prefixes_init_is_per_database();

    if(g_testFailures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_testFailures << " test(s) FAILED." << std::endl;
        return 1;
    }
}