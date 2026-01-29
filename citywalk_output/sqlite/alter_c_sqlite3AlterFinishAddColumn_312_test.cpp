/*
Unit test suite for the focal method sqlite3AlterFinishAddColumn, implemented as
part of the SQLite ALTER TABLE ADD COLUMN pathway.

Strategy:
- Use the public SQLite C interface to exercise behavior that ultimately exercises
  sqlite3AlterFinishAddColumn via real-world usage, since direct internal unit testing
  of static/internal helpers in alter.c would require the full internal SQLite build.
- Test a mixture of success and failure scenarios by performing actual ALTER TABLE
  operations and verifying results through queries. This approach exercises true
  and false branches of the relevant logic from a consumer perspective.
- Implement a lightweight custom test harness (no GTest) that runs in main and
  reports per-test results, while using non-terminating assertions (i.e., do not
  abort on first failure) so as to maximize code execution and coverage.

Notes:
- The tests assume a working SQLite3 library is linked (e.g., -lsqlite3).
- The tests focus on behavior that is affected by sqlite3AlterFinishAddColumn logic, such
  as constraints around NOT NULL, DEFAULTs, PRIMARY KEY, STORED, and basic column addition
  semantics. They do not mock internal static helpers due to visibility restrictions.

Code below is self-contained for test execution and includes explanatory comments for each test.
*/

#include <vector>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Lightweight test harness: non-terminating assertion recording
struct TestCase {
    std::string name;
    bool (*func)();
};

class SQLiteTestSuite {
public:
    void addTest(const std::string& name, bool (*func)()) {
        tests.push_back({name, func});
    }

    void run() {
        int total = (int)tests.size();
        int passed = 0;
        std::cout << "Running " << total << " test(s) for sqlite3AlterFinishAddColumn behavior via public API\n";
        for (const auto& t : tests) {
            bool ok = true;
            try {
                ok = t.func();
            } catch (...) {
                ok = false;
            }
            if (ok) {
                std::cout << "[PASS]  " << t.name << "\n";
                ++passed;
            } else {
                std::cout << "[FAIL]  " << t.name << "\n";
            }
        }
        std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";
    }

private:
    std::vector<TestCase> tests;
};

// Helper: run SQL and capture error message if needed
static int run_sql(sqlite3* db, const char* sql) {
    char* errmsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    return rc;
}

// Helper: Execute a SELECT returning two columns, expecting a single row.
// Returns true if a row is produced and both columns match expected values.
static bool fetch_row_two_ints(sqlite3* db, const char* sql, int expected_a, int expected_b) {
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    rc = sqlite3_step(stmt);
    bool success = true;
    if (rc == SQLITE_ROW) {
        int a = sqlite3_column_int(stmt, 0);
        int b = sqlite3_column_int(stmt, 1);
        if (a != expected_a || b != expected_b) {
            std::cerr << "Row values mismatch: expected (" << expected_a << "," << expected_b
                      << "), got (" << a << "," << b << ")\n";
            success = false;
        }
    } else {
        std::cerr << "No row returned by query.\n";
        success = false;
    }
    sqlite3_finalize(stmt);
    return success;
}

// Helper: Execute a SELECT returning a single textual column, expecting a single row with text.
static bool fetch_row_text(sqlite3* db, const char* sql, const std::string& expected_text) {
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    rc = sqlite3_step(stmt);
    bool ok = true;
    if (rc == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (!text) {
            ok = false;
        } else {
            std::string got(reinterpret_cast<const char*>(text));
            if (got != expected_text) {
                std::cerr << "Text mismatch: expected '" << expected_text
                          << "', got '" << got << "'\n";
                ok = false;
            }
        }
    } else {
        std::cerr << "No row returned for text fetch.\n";
        ok = false;
    }
    sqlite3_finalize(stmt);
    return ok;
}

/*
Test 1: Add a normal column with a default value.
- Expectation: Existing row(s) should get the default value for the new column.
- Rationale: Triggers the normal path where a non-NOT NULL, non-PRIMARY KEY column with a default value is added.
*/
static bool test_AddNormalColumnWithDefault() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Open db failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    bool ok = true;
    // Create base table
    ok &= (run_sql(db, "CREATE TABLE t(a INTEGER);") == SQLITE_OK);
    // Insert a row
    ok &= (run_sql(db, "INSERT INTO t(a) VALUES(1);") == SQLITE_OK);
    // Add a normal column with DEFAULT
    ok &= (run_sql(db, "ALTER TABLE t ADD COLUMN b INTEGER DEFAULT 5;") == SQLITE_OK);
    // Verify existing row has the default value for the new column
    ok &= fetch_row_two_ints(db, "SELECT a, b FROM t;", 1, 5);

    sqlite3_close(db);
    return ok;
}

/*
Test 2: Attempt to add a NOT NULL column without a default.
- Expectation: The operation should fail (SQLITE_ERROR), as per logic preventing NOT NULL without default.
*/
static bool test_AddNotNullWithoutDefaultShouldFail() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Open db failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    bool ok = true;
    ok &= (run_sql(db, "CREATE TABLE t(a INTEGER);") == SQLITE_OK);
    // This ALTER should fail per the code path (NOT NULL without default)
    rc = sqlite3_exec(db, "ALTER TABLE t ADD COLUMN c INTEGER NOT NULL;", nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK) {
        std::cerr << "Expected ALTER TABLE ... NOT NULL without default to fail, but it succeeded.\n";
        ok = false;
    }
    sqlite3_close(db);
    return ok;
}

/*
Test 3: Attempt to add a PRIMARY KEY column.
- Expectation: The operation should fail (SQLITE_ERROR), as PRIMARY KEY cannot be added via ALTER.
*/
static bool test_AddPrimaryKeyShouldFail() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Open db failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    bool ok = true;
    ok &= (run_sql(db, "CREATE TABLE t(a INTEGER);") == SQLITE_OK);
    // This ALTER should fail per the code path (PRIMARY KEY not allowed)
    rc = sqlite3_exec(db, "ALTER TABLE t ADD COLUMN d INTEGER PRIMARY KEY;", nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK) {
        std::cerr << "Expected ALTER TABLE ... PRIMARY KEY to fail, but it succeeded.\n";
        ok = false;
    }
    sqlite3_close(db);
    return ok;
}

/*
Test 4: Attempt to add a STORED column.
- Expectation: The operation should fail (SQLITE_ERROR), as STORED columns are not allowed to be added via ALTER.
*/
static bool test_AddStoredShouldFail() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Open db failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    bool ok = true;
    ok &= (run_sql(db, "CREATE TABLE t(a INTEGER);") == SQLITE_OK);
    rc = sqlite3_exec(db, "ALTER TABLE t ADD COLUMN s INTEGER STORED;", nullptr, nullptr, nullptr);
    if (rc == SQLITE_OK) {
        std::cerr << "Expected ALTER TABLE ... STORED to fail, but it succeeded.\n";
        ok = false;
    }
    sqlite3_close(db);
    return ok;
}

/*
Test 5: Add a column with a non-nullable DEFAULT text value and verify rows after old rows receive the default.
- This verifies normal DEFAULT behavior on string types as well.
*/
static bool test_AddColumnWithTextDefault() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Open db failed: " << sqlite3_errmsg(db) << "\n";
        return false;
    }

    bool ok = true;
    ok &= (run_sql(db, "CREATE TABLE t(a INTEGER);") == SQLITE_OK);
    ok &= (run_sql(db, "INSERT INTO t(a) VALUES(10);") == SQLITE_OK);
    // Add a new TEXT column with a non-empty DEFAULT
    ok &= (run_sql(db, "ALTER TABLE t ADD COLUMN b TEXT DEFAULT 'hello';") == SQLITE_OK);
    // If default propagates to existing rows, should read 'hello' for column b
    ok &= fetch_row_text(db, "SELECT b FROM t LIMIT 1;", "hello");

    sqlite3_close(db);
    return ok;
}

int main() {
    SQLiteTestSuite suite;
    // Step 2: Unit Test Generation guided by sqlite3AlterFinishAddColumn behavior
    // - We implement tests via the public API to cover both success and failure branches
    // - Each test includes explanatory comments about what scenario it covers

    suite.addTest("AddNormalColumnWithDefault", test_AddNormalColumnWithDefault);
    suite.addTest("AddNotNullWithoutDefaultShouldFail", test_AddNotNullWithoutDefaultShouldFail);
    suite.addTest("AddPrimaryKeyShouldFail", test_AddPrimaryKeyShouldFail);
    suite.addTest("AddStoredShouldFail", test_AddStoredShouldFail);
    suite.addTest("AddColumnWithTextDefault", test_AddColumnWithTextDefault);

    suite.run();
    return 0;
}