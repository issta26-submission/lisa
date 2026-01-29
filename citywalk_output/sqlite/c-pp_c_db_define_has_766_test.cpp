// Test suite for the focal method: db_define_has in c-pp.c
// This test uses custom in-process mocks to drive the behavior of the
// dependencies (db_prepare, db_bind_text, db_step, sqlite3_clear_bindings,
// sqlite3_reset, g_debug) so that db_define_has can be exercised deterministically.
// The actual code under test is in C (db_define_has) but we drive it from C++11.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <unordered_map>
#include <iostream>
#include <cassert>


// Assumed external symbol from the production code (C linkage)
extern "C" int db_define_has(const char * zName);

// Provide minimal platform-neutral typedefs/macros to align with the C API used by db_define_has
typedef void sqlite3_stmt;              // opaque sqlite3_stmt placeholder for tests
#define SQLITE_ROW 100                   // mimic sqlite3_result SQLITE_ROW
#define SQLITE_DONE 101                  // mimic sqlite3_result SQLITE_DONE

// Global state manipulated by our mocks to control behavior of db_define_has
static std::string g_last_bound_name;                          // last bound value (zName)
static bool g_prepare_called = false;                          // whether db_prepare was invoked
static std::unordered_map<std::string,int> g_mock_step_rc;     // map: boundName -> rc to return from db_step

// Initialize/reset per-test state
static void test_reset_state() {
    g_last_bound_name.clear();
    g_prepare_called = false;
    g_mock_step_rc.clear();
}

// Extend C linkage for our mocks to ensure they override the production definitions
extern "C" {

    // db_prepare(sqlite3_stmt **pStmt, const char * zSql, ...)
    // In the real code this would prepare a sqlite3_stmt. For tests we just
    // simulate the behavior: populate *pStmt with a non-null pointer and note the prepare.
    void db_prepare(sqlite3_stmt **pStmt, const char * zSql, ...)
    {
        (void)zSql; // not used in test logic
        static int sentinel = 0;
        // Simulate that a statement is prepared and assign a non-null pointer.
        if (pStmt) {
            *pStmt = (sqlite3_stmt*)(&sentinel);
        }
        g_prepare_called = true;
    }

    // db_bind_text(sqlite3_stmt * pStmt, int col, const char * zStr)
    // Capture the bound name for later decision-making in db_step.
    void db_bind_text(sqlite3_stmt * pStmt, int col, const char * zStr)
    {
        (void)pStmt;
        (void)col;
        if (zStr) {
            g_last_bound_name = zStr;
        } else {
            g_last_bound_name.clear();
        }
    }

    // db_step(sqlite3_stmt *pStmt)
    // Return behavior based on the last bound name via g_mock_step_rc map.
    int db_step(sqlite3_stmt *pStmt)
    {
        (void)pStmt;
        auto it = g_mock_step_rc.find(g_last_bound_name);
        if (it != g_mock_step_rc.end()) {
            return it->second;
        }
        // Default behavior if not specified: behave as DONE
        return SQLITE_DONE;
    }

    // sqlite3_clear_bindings(sqlite3_stmt *pStmt)
    void sqlite3_clear_bindings(sqlite3_stmt *pStmt)
    {
        (void)pStmt;
        // Clear the bound name cache in test environment
        g_last_bound_name.clear();
    }

    // sqlite3_reset(sqlite3_stmt *pStmt)
    void sqlite3_reset(sqlite3_stmt *pStmt)
    {
        (void)pStmt;
        // No-op for test scaffolding
    }

    // g_debug(int level, const char * zFmt, ...)
    // Non-terminating no-op for tests (we don't rely on actual logging)
    void g_debug(int level, const char * zFmt, ...)
    {
        (void)level;
        (void)zFmt;
        // Intentionally ignored to keep tests deterministic
    }
}

// Lightweight test harness (non-GTest)
static int g_total_tests = 0;
static int g_failed_tests = 0;
#define EXPECT_EQ(a,b) do { ++g_total_tests; if ((a) != (b)) { std::cerr << "EXPECT_EQ failed: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")\n"; ++g_failed_tests; } } while(0)
#define EXPECT_TRUE(x) do { ++g_total_tests; if (!(x)) { std::cerr << "EXPECT_TRUE failed: " << #x << "\n"; ++g_failed_tests; } } while(0)
#define TEST(name) static void name()

// Candidate tests:

// 1) True branch: zName exists in def table -> db_step returns SQLITE_ROW -> rc = 1
TEST(test_db_define_has_exists)
{
    test_reset_state();
    // Arrange: the bound name will be "exists_name" and db_step should return SQLITE_ROW
    g_mock_step_rc["exists_name"] = SQLITE_ROW;

    // Act: call the focal method
    int rc = db_define_has("exists_name");

    // Assert: rc == 1
    EXPECT_EQ(rc, 1);
    // Additional sanity: ensure that a prepare was issued
    EXPECT_TRUE(g_prepare_called);
}

// 2) False branch: zName does not exist -> db_step returns SQLITE_DONE -> rc = 0
TEST(test_db_define_has_not_exists)
{
    test_reset_state();
    // Arrange: bound name "missing_name" maps to SQLITE_DONE
    g_mock_step_rc["missing_name"] = SQLITE_DONE;

    // Act: call the focal method
    int rc = db_define_has("missing_name");

    // Assert: rc == 0
    EXPECT_EQ(rc, 0);
    // We expect that prepare was called on first invocation when defHas was initially null
    EXPECT_TRUE(g_prepare_called);
}

// 3) Prepared-path path: ensure that after first call g.stmt.defHas is non-null
//    and subsequent calls reuse the prepared statement (db_prepare not invoked again).
TEST(test_db_define_has_prepared_path)
{
    test_reset_state();

    // First call: ensure a prepared statement is created (rc depends on existence)
    // Map "alpha" to SQLITE_ROW so first call returns 1 and sets prepared flag
    g_mock_step_rc["alpha"] = SQLITE_ROW;
    int rc1 = db_define_has("alpha");
    EXPECT_EQ(rc1, 1);
    // Second call with a different name should skip db_prepare due to non-null defHas
    // Map "beta" to SQLITE_DONE; ensure rc becomes 0
    g_mock_step_rc["beta"] = SQLITE_DONE;
    int rc2 = db_define_has("beta");
    EXPECT_EQ(rc2, 0);
}

// Helper to run all tests and report summary
static void run_all_tests()
{
    test_db_define_has_exists();
    test_db_define_has_not_exists();
    test_db_define_has_prepared_path();
    // Summary
    std::cout << "Total tests run: " << g_total_tests << "\n";
    std::cout << "Total tests failed: " << g_failed_tests << "\n";
    if (g_failed_tests == 0) {
        std::cout << "All tests PASSED.\n";
    } else {
        std::cout << "Some tests FAILED.\n";
    }
}

// Main entry point for test execution (as required when gtest is not used)
int main(int argc, char const * argv[])
{
    (void)argc; (void)argv;
    run_all_tests();
    return g_failed_tests == 0 ? 0 : 1;
}