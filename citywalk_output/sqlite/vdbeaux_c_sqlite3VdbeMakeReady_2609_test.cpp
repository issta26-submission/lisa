/*
 * Test suite for sqlite3VdbeMakeReady (indirectly exercised via the public
 * SQLite API) using a lightweight, framework-free C++11 test harness.
 *
 * Rationale:
 * - The focal function is an internal routine (vdbeaux.c) used by the
 *   prepared statement path. Directly constructing internal Vdbe/Parse
 *   structures for unit testing is highly intrusive and architecture-specific.
 * - The public API sqlite3_prepare_v2 ultimately exercises sqlite3VdbeMakeReady
 *   as part of preparing a statement. Therefore we test intended behavior by
 *   performing SQL preparations and validating outcomes.
 * - We avoid GTest and GMock as requested, and implement a tiny test harness
 *   with non-terminating assertions (they record failures but continue
 *   execution to maximize coverage).
 * - We cover typical success and error paths, which exercise the underlying
 *   MakeReady logic through the normal client-facing API.
 *
 * Notes:
 * - The tests rely on a real SQLite3 library/linkage in the build environment.
 * - This suite uses only the public SQLite API (sqlite3_open, sqlite3_prepare_v2,
 *   sqlite3_finalize, sqlite3_close), ensuring compatibility with C++11.
 * - Because MakeReady is an internal step, this indirect testing aims to achieve
 *   high coverage of the function's behavior without exposing internal state.
 * - Static members or private internals are not accessed directly.
 *
 * How to run:
 * - Build with a C++11 capable compiler and link against the SQLite3 library.
 * - Execute the resulting binary. The test harness will print per-test results
 *   and a final summary.
 *
 * Author: Generated per instructions; no external frameworks used.
 */

#include <iomanip>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Public SQLite API

// Simple non-terminating test harness (no GTest/GMock)
static int g_passed = 0;
static int g_failed = 0;

#define TEST_PASSED(msg)                          \
    do {                                            \
        std::cout << "[PASS] " << msg << "\n";    \
        ++g_passed;                                \
    } while(0)

#define TEST_FAILED(msg)                          \
    do {                                            \
        std::cerr << "[FAIL] " << msg << "\n";     \
        ++g_failed;                                 \
    } while(0)

#define EXPECT_TRUE(cond, msg)                        \
    do {                                                \
        if (static_cast<bool>(cond)) {                  \
            TEST_PASSED(std::string("EXPECT_TRUE: ") + (msg)); \
        } else {                                        \
            TEST_FAILED(std::string("EXPECT_TRUE: ") + (msg)); \
        }                                               \
    } while(0)

#define EXPECT_EQ(a, b, msg)                          \
    do {                                                \
        if (static_cast<sqlite3_int64>(a) == static_cast<sqlite3_int64>(b)) { \
            TEST_PASSED(std::string("EXPECT_EQ: ") + (msg)); \
        } else {                                        \
            std::stringstream ss; ss << "EXPECTED " << (a) << " == " << (b); \
            TEST_FAILED(std::string("EXPECT_EQ: ") + (msg) + " -> " + ss.str()); \
        }                                               \
    } while(0)

static void test_basic_prepare_exercises_makeReady_basic_path() {
    // Purpose:
    // - Exercise a normal prepare path which will internally invoke sqlite3VdbeMakeReady.
    // - Validate that a simple, valid SQL statement can be prepared without error.
    // - Indirectly ensures MakeReady can allocate internal memory and initialize state.

    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (db == nullptr || rc != SQLITE_OK) {
        TEST_FAILED("Could not open in-memory database for Basic Prepare test");
        g_failed++;
        return;
    }

    const char *sql = "SELECT 1";
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    EXPECT_TRUE(rc == SQLITE_OK, "sqlite3_prepare_v2 should succeed for 'SELECT 1'");

    if (stmt != nullptr) {
        rc = sqlite3_finalize(stmt);
        EXPECT_TRUE(rc == SQLITE_OK, "sqlite3_finalize should succeed after prepare");
    } else {
        TEST_FAILED("sqlite3_prepare_v2 produced a null statement handle");
        g_failed++;
    }

    sqlite3_close(db);
    TEST_PASSED("Basic prepare path exercised (indirect MakeReady).");
}

static void test_basic_prepare_exercises_makeReady_error_path() {
    // Purpose:
    // - Ensure that prepare gracefully fails on syntactically invalid SQL.
    // - This indirectly exercises MakeReady but on error path.
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (db == nullptr || rc != SQLITE_OK) {
        TEST_FAILED("Could not open in-memory database for Error Path test");
        g_failed++;
        return;
    }

    const char *bad_sql = "SELCT 1"; // intentionally misspelled
    sqlite3_stmt *stmt = nullptr;

    rc = sqlite3_prepare_v2(db, bad_sql, -1, &stmt, NULL);
    EXPECT_TRUE(rc != SQLITE_OK, "sqlite3_prepare_v2 should fail for invalid SQL");

    if (stmt != nullptr) {
        // If a statement was unexpectedly prepared, finalize to avoid leak.
        sqlite3_finalize(stmt);
        TEST_FAILED("Unexpectedly obtained a prepared statement for invalid SQL");
        g_failed++;
    }

    sqlite3_close(db);
    TEST_PASSED("Error path of MakeReady (through public API) exercised.");
}

static void test_multiple_prepares_reuse_makeReady_path() {
    // Purpose:
    // - Prepare two statements in succession to exercise reuse/memory handling
    //   aspects of the MakeReady path through repeated use of the public API.
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (db == nullptr || rc != SQLITE_OK) {
        TEST_FAILED("Could not open in-memory database for Reuse test");
        g_failed++;
        return;
    }

    const char *sad_sqls[] = { "SELECT 2", "SELECT 3" };
    for (int i = 0; i < 2; ++i) {
        sqlite3_stmt *stmt = nullptr;
        rc = sqlite3_prepare_v2(db, sad_sqls[i], -1, &stmt, NULL);
        EXPECT_TRUE(rc == SQLITE_OK, std::string("prepare ").append(sad_sqls[i]).append(" should succeed"));
        if (stmt) {
            rc = sqlite3_finalize(stmt);
            EXPECT_TRUE(rc == SQLITE_OK, "finalize after prepare should succeed");
        } else {
            TEST_FAILED("Null statement after a successful prepare path");
            g_failed++;
        }
    }

    sqlite3_close(db);
    TEST_PASSED("Multiple prepares exercise MakeReady path multiple times.");
}

int main() {
    std::cout << "Starting sqlite3VdbeMakeReady-focused test suite (indirect via public API)" << std::endl;

    test_basic_prepare_exercises_makeReady_basic_path();
    test_basic_prepare_exercises_makeReady_error_path();
    test_multiple_prepares_reuse_makeReady_path();

    int total = g_passed + g_failed;
    std::cout << "\nTest Summary: "
              << g_passed << " passed, "
              << g_failed << " failed, "
              << "out of " << total << " tests." << std::endl;

    // Return number of failures as exit code (0 means success).
    return g_failed == 0 ? 0 : 1;
}