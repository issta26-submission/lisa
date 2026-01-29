/*
  Test Suite: sqlite3_column_name
  Context:
  - Focal method under test: sqlite3_column_name(sqlite3_stmt *pStmt, int N)
    Implementation (public API) delegates to an internal helper: columnName(pStmt, N, 0, COLNAME_NAME)
  - We cannot access the internal static columnName implementation directly, but the public API is exposed by SQLite.
  - Strategy: Drive the public API through a real, in-memory SQLite database and verify that
    sqlite3_column_name returns expected column names for prepared statements.

  Step 1 (Program Understanding) Candidate Keywords
  - sqlite3_column_name, sqlite3_stmt, columnName, COLNAME_NAME (as used by the focal method)
  - sqlite3_prepare_v2, sqlite3_open, sqlite3_close, sqlite3_finalize
  - In-memory DB (-memory:), prepared statement column naming, alias handling
  - Boundary: out-of-range N returns NULL (expected behavior for invalid column index)
  - Data integrity: string comparison for column names (ASCII/UTF-8)
  - Non-terminating assertions: use a lightweight custom test harness (no GTest)

  Step 2 (Unit Test Generation): We test:
  - Basic case with two aliased columns: "SELECT 1 AS foo, 2 AS bar" -> column 0 == "foo", column 1 == "bar"
  - Boundary case: out-of-range index (e.g., 2) returns NULL
  - Another basic alias case: "SELECT 'alpha' AS A, 3 AS B" -> "A", "B"
  - Boundary: negative index (-1) returns NULL (if SQLite returns NULL for invalid index)
  - Ensure resources are released (finalize statement, close DB)

  Step 3 (Test Case Refinement):
  - Lightweight, non-terminating assertions (collect failures and continue)
  - Minimal dependencies: only standard library and SQLite C API
  - Tests executed from main()
  - Use explicit descriptive comments for each test

  Notes:
  - The tests rely on the public SQLite API and linking against libsqlite3.
  - This approach exercises the observable behavior of sqlite3_column_name without needing direct access to static internals.
*/

#include <opcodes.h>
#include <sqlite3.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Lightweight, non-terminating test harness
static int g_total = 0;
static int g_failures = 0;

static bool strings_equal(const char* a, const char* b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    return std::strcmp(a, b) == 0;
}

// Expectation helper for string comparisons (NULL-safe)
static void expect_str_eq(const char* got, const char* expected, const char* desc) {
    ++g_total;
    if (!strings_equal(got, expected)) {
        ++g_failures;
        std::cerr << "TEST FAIL: " << desc << "\n"
                  << "  Expected: " << (expected ? std::string(expected) : std::string("<NULL>")) << "\n"
                  << "  Got:      " << (got ? std::string(got) : std::string("<NULL>")) << "\n";
    } else {
        // Test passed; optional verbose success message can be added here.
    }
}

// Expect NULL result
static void expect_nullptr(const char* got, const char* desc) {
    ++g_total;
    if (got != nullptr) {
        ++g_failures;
        std::cerr << "TEST FAIL: " << desc << "\n"
                  << "  Expected: <NULL>\n"
                  << "  Got:      " << (got ? got : "<NULL>") << "\n";
    }
}

// Test 1: Basic alias names
// SQL: SELECT 1 AS foo, 2 AS bar
// Expect: column 0 name == "foo"; column 1 name == "bar"; column 2 is out-of-bounds -> NULL
static void test_column_name_basic_aliases() {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Skipping test_column_name_basic_aliases: cannot open in-memory DB\n";
        return;
    }

    const char* sql = "SELECT 1 AS foo, 2 AS bar";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK || stmt == nullptr) {
        std::cerr << "Skipping test_column_name_basic_aliases: cannot prepare statement\n";
        sqlite3_close(db);
        return;
    }

    // Validate names
    const char* name0 = sqlite3_column_name(stmt, 0);
    const char* name1 = sqlite3_column_name(stmt, 1);
    const char* name2 = sqlite3_column_name(stmt, 2);  // out of range; expect NULL

    expect_str_eq(name0, "foo", "Column 0 should be named 'foo'");
    expect_str_eq(name1, "bar", "Column 1 should be named 'bar'");
    expect_nullptr(name2, "Column 2 (out-of-bounds) should be NULL");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Test 2: Additional alias coverage with single quotes in data
// SQL: SELECT 'alpha' AS A, 3 AS B
// Expect: column 0 name == "A"; column 1 name == "B"; column 2 NULL
static void test_column_name_additional_aliases() {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Skipping test_column_name_additional_aliases: cannot open in-memory DB\n";
        return;
    }

    const char* sql = "SELECT 'alpha' AS A, 3 AS B";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK || stmt == nullptr) {
        std::cerr << "Skipping test_column_name_additional_aliases: cannot prepare statement\n";
        sqlite3_close(db);
        return;
    }

    const char* name0 = sqlite3_column_name(stmt, 0);
    const char* name1 = sqlite3_column_name(stmt, 1);
    const char* name2 = sqlite3_column_name(stmt, 2);

    expect_str_eq(name0, "A", "Column 0 should be named 'A'");
    expect_str_eq(name1, "B", "Column 1 should be named 'B'");
    expect_nullptr(name2, "Column 2 (out-of-bounds) should be NULL");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Test 3: Boundary with negative index
// SQL: SELECT 1 AS foo
// Expect: column 0 name == "foo"; column -1 is NULL
static void test_column_name_negative_index() {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Skipping test_column_name_negative_index: cannot open in-memory DB\n";
        return;
    }

    const char* sql = "SELECT 1 AS foo";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK || stmt == nullptr) {
        std::cerr << "Skipping test_column_name_negative_index: cannot prepare statement\n";
        sqlite3_close(db);
        return;
    }

    const char* name0 = sqlite3_column_name(stmt, 0);
    const char* nameNeg1 = sqlite3_column_name(stmt, -1);

    expect_str_eq(name0, "foo", "Column 0 should be named 'foo'");
    expect_nullptr(nameNeg1, "Column -1 (negative index) should be NULL");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Run all tests and report summary
static int run_all_tests() {
    g_total = 0;
    g_failures = 0;

    test_column_name_basic_aliases();
    test_column_name_additional_aliases();
    test_column_name_negative_index();

    std::cout << "sqlite3_column_name tests completed. "
              << "Total: " << g_total
              << ", Failures: " << g_failures
              << std::endl;
    return g_failures;
}

int main() {
    return run_all_tests();
}