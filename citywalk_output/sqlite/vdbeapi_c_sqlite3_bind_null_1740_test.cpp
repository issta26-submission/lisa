// Test suite for sqlite3_bind_null using the public SQLite API (no GTest).
// This harness is written in C++11 and relies on the real SQLite library
// to exercise the focal method through its public interface.
//
// Purpose:
// - Validate that binding NULL to a valid parameter results in a NULL result
//   when subsequently selecting that parameter.
// - Validate that binding NULL to an invalid parameter index returns an error
//   code (SQLITE_RANGE) without crashing.
//
// Notes:
// - The tests exercise true/false branches via actual library behavior.
// - We use a lightweight, non-terminating, assertion approach suitable for
//   a custom test harness.

#include <functional>
#include <opcodes.h>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


namespace test_sqlite_bind_null {

// Global flag to indicate test failure for a given test case.
static bool g_test_ok = true;

// Lightweight non-terminating assertion macro.
// Compares two values and records a failure if they differ.
#define EXPECT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        std::cerr << "  [FAIL] " << __FUNCTION__ << ": expected " \
                  << (expected) << " but got " << (actual) << std::endl; \
        g_test_ok = false; \
    } \
} while(0)


// Test 1: Bind NULL to a valid parameter (index 1) and verify that the
//         subsequent SELECT returns NULL for that column.
// Rationale: When sqlite3_bind_null succeeds (rc == SQLITE_OK), the FOCAL
//            METHOD should unbind and the bound value should be NULL in the result.
bool test_bind_null_valid_index() {
    g_test_ok = true;

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    // Open an in-memory database
    if (SQLITE_OK != sqlite3_open(":memory:", &db)) {
        std::cerr << "  [ERROR] sqlite3_open failed" << std::endl;
        return false;
    }

    // Prepare a simple statement with one parameter: SELECT ?1
    const char* zSql = "SELECT ?1";
    if (SQLITE_OK != sqlite3_prepare_v2(db, zSql, -1, &stmt, nullptr)) {
        std::cerr << "  [ERROR] sqlite3_prepare_v2 failed" << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Bind NULL to parameter 1 using the public API (calls sqlite3_bind_null internally)
    int rc_bind = sqlite3_bind_null(stmt, 1);
    EXPECT_EQ(SQLITE_OK, rc_bind);

    // Step to fetch the result row
    int rc_step = sqlite3_step(stmt);
    EXPECT_EQ(SQLITE_ROW, rc_step);

    // The first column should be SQL NULL
    int colType = sqlite3_column_type(stmt, 0);
    EXPECT_EQ(SQLITE_NULL, colType);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return g_test_ok;
}


// Test 2: Bind NULL to an invalid parameter index (index 2 for a statement with 1 parameter)
// Expected behavior: sqlite3_bind_null should return SQLITE_RANGE and not crash.
bool test_bind_null_invalid_index() {
    g_test_ok = true;

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    if (SQLITE_OK != sqlite3_open(":memory:", &db)) {
        std::cerr << "  [ERROR] sqlite3_open failed" << std::endl;
        return false;
    }

    const char* zSql = "SELECT ?1";
    if (SQLITE_OK != sqlite3_prepare_v2(db, zSql, -1, &stmt, nullptr)) {
        std::cerr << "  [ERROR] sqlite3_prepare_v2 failed" << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Attempt to bind NULL to a non-existent parameter index (2)
    int rc_bind = sqlite3_bind_null(stmt, 2);
    // Expect SQLITE_RANGE when index is out of range
    EXPECT_EQ(SQLITE_RANGE, rc_bind);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return g_test_ok;
}


// Runner to execute all test cases and summarize results.
void run_all_tests() {
    struct TestCase {
        const char* name;
        std::function<bool()> func;
    };

    TestCase cases[] = {
        { "test_bind_null_valid_index", test_bind_null_valid_index },
        { "test_bind_null_invalid_index", test_bind_null_invalid_index }
    };

    int total = sizeof(cases) / sizeof(cases[0]);
    int failures = 0;

    for (const auto& c : cases) {
        bool ok;
        // Reset per-test status and run test
        ok = c.func();
        if (!ok) {
            ++failures;
        }
        // Individual test output is produced by EXPECT_EQ as non-terminating assertions
        // and by per-test function logs for errors.
        std::cout << "[TEST] " << c.name << " -> "
                  << (ok ? "PASS" : "FAIL") << std::endl;
    }

    std::cout << "\nSummary: " << total << " tests, "
              << failures << " failures." << std::endl;
}

} // namespace test_sqlite_bind_null


// Entry point
int main() {
    // Run the test suite
    test_sqlite_bind_null::run_all_tests();
    return 0;
}