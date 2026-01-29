// Test suite for the focal method: sqlite3_recover_init_sql
// This test suite is designed for C++11 without using GTest.
// It relies on the presence of SQLite3 and sqlite3recover.h in the build environment.

#include <string.h>
#include <assert.h>
#include <sqlite3recover.h>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <iostream>



// Simple non-terminating test harness
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg)                                                        \
    do {                                                                                \
        if (!(cond)) {                                                                  \
            std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - "            \
                      << (msg) << std::endl;                                           \
            ++g_tests_failed;                                                           \
        } else {                                                                        \
            ++g_tests_passed;                                                           \
        }                                                                               \
    } while (0)

static int dummySql(void* ctx, const char* z) {
    (void)ctx;
    (void)z;
    // Minimal implementation: not used by tests beyond pointer passing
    return 0;
}

/*
Test 1: Basic happy-path test
- Open an in-memory SQLite database.
- Provide a valid xSql callback and a non-null context.
- Call sqlite3_recover_init_sql and expect a non-null sqlite3_recover* handle.
*/
static void test_basic_init_sql_happy_path() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr && rc == SQLITE_OK, "Failed to open in-memory SQLite DB");

    if (db) {
        sqlite3_recover *rec = sqlite3_recover_init_sql(db, "main", dummySql, nullptr);
        EXPECT_TRUE(rec != nullptr, "sqlite3_recover_init_sql should return non-null on valid inputs");

        if (rec) {
            // Best-effort cleanup; ignore return value from finish if not strictly needed
            sqlite3_recover_finish(rec);
        }
        sqlite3_close(db);
    }
}

/*
Test 2: Long zDb string handling
- Open an in-memory SQLite database.
- Use an unusually long database name to exercise potential string handling paths.
- Expect a non-null sqlite3_recover* handle.
*/
static void test_init_sql_with_long_zdb() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr && rc == SQLITE_OK, "Failed to open in-memory SQLite DB for long zDb test");

    if (db) {
        // Create a very long zDb string
        std::string longDbName(4096, 'a'); // 4KB of 'a'
        sqlite3_recover *rec = sqlite3_recover_init_sql(db, longDbName.c_str(), dummySql, nullptr);
        EXPECT_TRUE(rec != nullptr, "sqlite3_recover_init_sql should handle long zDb names");

        if (rec) {
            sqlite3_recover_finish(rec);
        }
        sqlite3_close(db);
    }
}

/*
Test 3: Multiple consecutive initializations
- Open an in-memory SQLite database.
- Call sqlite3_recover_init_sql twice with different zDb values.
- Verify both calls succeed and return distinct recover handles.
*/
static void test_multiple_consecutive_inits() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr && rc == SQLITE_OK, "Failed to open in-memory SQLite DB for multiple init test");

    if (db) {
        sqlite3_recover *rec1 = sqlite3_recover_init_sql(db, "db1", dummySql, nullptr);
        sqlite3_recover *rec2 = sqlite3_recover_init_sql(db, "db2", dummySql, nullptr);

        EXPECT_TRUE(rec1 != nullptr, "First sqlite3_recover_init_sql should return non-null");
        EXPECT_TRUE(rec2 != nullptr, "Second sqlite3_recover_init_sql should return non-null");
        EXPECT_TRUE(rec1 != rec2, "Two recover handles should be distinct");

        if (rec1) sqlite3_recover_finish(rec1);
        if (rec2) sqlite3_recover_finish(rec2);
        sqlite3_close(db);
    }
}

// Entry point for running tests
int main() {
    std::cout << "Running tests for sqlite3_recover_init_sql wrapper..." << std::endl;

    test_basic_init_sql_happy_path();
    test_init_sql_with_long_zdb();
    test_multiple_consecutive_inits();

    std::cout << "Test results: " << g_tests_passed << " passed, "
              << g_tests_failed << " failed." << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}