// Test suite for the focal method: sqlite3_column_double
// Context: The method is part of the SQLite/Vdbe layer and relies on
// underlying internal mechanisms (columnMem, sqlite3_value_double, columnMallocFailure, etc.)
// In a real project, sqlite3_column_double would fetch a column value from the
// current VM row, convert it to double, and then return it. We test this behavior
// by driving the public SQLite C API (sqlite3_open, sqlite3_prepare_v2, sqlite3_step,
// sqlite3_column_double, etc.) against a simple in-memory database.
//
// Notes:
// - This test uses the actual SQLite C API to exercise the integration path that
//   sqlite3_column_double encapsulates.
// - We implement a tiny, non-terminating test harness (no GTest) suitable for a
//   C++11 build as requested.
// - Each unit test is designed to cover a different realistic scenario (double literal,
//   integer, text that can be converted to double, NULL, multiple columns).
// - Tests are independent: each test opens its own in-memory database and closes it.

#include <vector>
#include <cstdio>
#include <opcodes.h>
#include <cmath>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


extern "C" {
  #include "sqlite3.h"
}

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

static void log_error(const char* msg) {
    fprintf(stderr, "ERROR: %s\n", msg);
}

static void log_info(const char* msg) {
    printf("INFO: %s\n", msg);
}

// Helper assertion: floating point near check
static bool almost_equal(double a, double b, double tol = 1e-12) {
    return std::fabs(a - b) <= tol;
}

// Helper to run a single SQL query and check a specific column value against expected
static bool check_column_double(sqlite3* db,
                                const char* sql,
                                int columnIndex,
                                double expected,
                                double tol = 1e-12) {
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        char buf[256];
        std::snprintf(buf, sizeof(buf), "prepare failed for query '%s': %s", sql, sqlite3_errmsg(db));
        log_error(buf);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        // Not the expected row; finalize and fail
        char buf[256];
        std::snprintf(buf, sizeof(buf), "step did not yield a row for query '%s'", sql);
        log_error(buf);
        sqlite3_finalize(stmt);
        return false;
    }

    double val = sqlite3_column_double(stmt, columnIndex);

    // Finalize the statement
    rc = sqlite3_finalize(stmt);
    (void)rc; // suppress unused warning in release builds

    if (!almost_equal(val, expected, tol)) {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
                      "value mismatch for query '%s' column %d: expected %.12f, got %.12f",
                      sql, columnIndex, expected, val);
        log_error(buf);
        return false;
    }

    return true;
}

// Test 1: Literal double value from a SELECT
// This exercises the normal path where the VM has a real numeric literal
// and sqlite3_column_double should return the exact value.
static bool test_literal_double() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        log_error("sqlite3_open failed in test_literal_double");
        if (db) sqlite3_close(db);
        return false;
    }

    const char* sql = "SELECT 3.25";
    bool ok = check_column_double(db, sql, 0, 3.25);

    sqlite3_close(db);
    return ok;
}

// Test 2: Integer value converted to double
// sqlite3_column_double should convert integer to double (e.g., 7 -> 7.0).
static bool test_integer_to_double() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        log_error("sqlite3_open failed in test_integer_to_double");
        if (db) sqlite3_close(db);
        return false;
    }

    const char* sql = "SELECT 7";
    bool ok = check_column_double(db, sql, 0, 7.0);

    sqlite3_close(db);
    return ok;
}

// Test 3: Text that can be converted to double
// Strings like '2.75' should be converted to their numeric value by SQLite.
// This exercises the path where sqlite3_value_double operates on a MEM with TEXT.
static bool test_text_to_double() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        log_error("sqlite3_open failed in test_text_to_double");
        if (db) sqlite3_close(db);
        return false;
    }

    const char* sql = "SELECT '2.75'";
    bool ok = check_column_double(db, sql, 0, 2.75);

    sqlite3_close(db);
    return ok;
}

// Test 4: NULL value yields 0.0 for sqlite3_column_double
// This aligns with SQLite's behavior of returning 0.0 when the value is NULL.
static bool test_null_value() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        log_error("sqlite3_open failed in test_null_value");
        if (db) sqlite3_close(db);
        return false;
    }

    const char* sql = "SELECT NULL";
    bool ok = check_column_double(db, sql, 0, 0.0);

    sqlite3_close(db);
    return ok;
}

// Test 5: Multiple columns in one row
// Ensure sqlite3_column_double can fetch values from different columns in the same row.
// This also validates that the function handles column indices correctly (0-based).
static bool test_multiple_columns() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        log_error("sqlite3_open failed in test_multiple_columns");
        if (db) sqlite3_close(db);
        return false;
    }

    const char* sql = "SELECT 1.1, '2.2', NULL";

    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        log_error("prepare failed in test_multiple_columns");
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        log_error("step did not yield a row in test_multiple_columns");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    double v0 = sqlite3_column_double(stmt, 0);
    double v1 = sqlite3_column_double(stmt, 1);
    double v2 = sqlite3_column_double(stmt, 2);

    int finalize_rc = sqlite3_finalize(stmt);
    (void)finalize_rc;
    sqlite3_close(db);

    bool ok = almost_equal(v0, 1.1) && almost_equal(v1, 2.2) && almost_equal(v2, 0.0);
    if (!ok) {
        log_error("value mismatch in test_multiple_columns for one row with multiple columns");
    }
    return ok;
}

// Candidate keyword mapping (conceptual):
// - sqlite3_column_double: the focal method under test
// - sqlite3_value_double, columnMem, columnMallocFailure: internal dependencies that
//   sqlite3_column_double relies on in the real SQLite code path.
// - sqlite3_stmt: the statement object used to fetch the current row's column data.
// - In these integration-style tests, we exercise the actual SQLite API to indirectly
//   validate the behavior of sqlite3_column_double, ensuring alignment with expected user
//  -visible semantics for double extraction.

int main() {
    std::vector< bool (*)() > tests = {
        test_literal_double,
        test_integer_to_double,
        test_text_to_double,
        test_null_value,
        test_multiple_columns
    };

    g_tests_run = 0;
    g_tests_failed = 0;

    int testIndex = 1;
    for (auto t : tests) {
        ++g_tests_run;
        bool passed = false;
        // Each test is independent; print a header for clarity
        printf("Test %d: ", testIndex++);
        fflush(stdout);
        passed = t();
        if (passed) {
            printf("PASSED\n");
        } else {
            printf("FAILED\n");
            ++g_tests_failed;
        }
    }

    // Summary
    printf("\nTEST SUMMARY: %d ran, %d failed\n", g_tests_run, g_tests_failed);
    // Non-terminating exit: return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}