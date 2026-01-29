// Lightweight C++11 test suite for the focal method sqlite3GenerateColumnNames
// via public API surface (sqlite3_prepare_v2 / sqlite3_column_name) to validate
// the column-naming behavior implemented in sqlite3GenerateColumnNames.
// Note: We do not rely on GTest or private/internal testing frameworks.
// The tests exercise true/false branches conceptually by verifying column names
// produced for various SELECT clause shapes.

#include <sqlite3.h>
#include <cctype>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <algorithm>



// Utility: case-insensitive string comparison
static bool iequals(const std::string &a, const std::string &b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

// Basic assertion that prints a message and returns false on failure
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " << (msg) << "\n"; \
        return false; \
    } \
} while(0)

// Helper: run a test and print its result
static bool runTest(const char* testName, bool (*testFunc)()) {
    std::cout << "Running " << testName << " ... ";
    bool ok = testFunc();
    if (ok) {
        std::cout << "PASSED\n";
    } else {
        std::cout << "FAILED\n";
    }
    return ok;
}

/*
Step 2/3 (Test focus)
We test the column-naming behavior that sqlite3GenerateColumnNames implements
by using the public API to prepare statements and inspect their reported column
names. This indirectly validates:

- AS clause priority: a column alias is used as the output column name.
- Table column reference without an explicit alias: the column name matches the
  table's column name (e.g., "a" for t(a, ...)).
- Multiple expressions with explicit aliases: each column name matches its alias.
- The special "rowid" naming path for rowid access as a direct expression.

We implement 4 targeted tests:

1) AS alias takes precedence for a literal expression: SELECT 1 AS a -> "a"
2) Two expressions with explicit aliases: SELECT a AS aa, b AS bb -> "aa","bb"
3) Direct table column reference yields the actual column name: SELECT a FROM t -> "a"
4) Rowid path yields "rowid" (case-insensitive match) for a rowid expression: SELECT rowid FROM t -> "rowid"
*/

static bool test_alias_overrides_expression_name() {
    // Reusable in-memory DB
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to open in-memory DB: " << sqlite3_errmsg(db) << "\n";
        if (db) sqlite3_close(db);
        return false;
    }

    // Create a simple table for test 3/4
    rc = sqlite3_exec(db, "CREATE TABLE t(a, b);", nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to create table: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    // Test 1: SELECT 1 AS a; the column name should be the alias "a"
    {
        const char* sql = "SELECT 1 AS a;";
        sqlite3_stmt* stmt = nullptr;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK || !stmt) {
            std::cerr << "Prepare failed for test1: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return false;
        }
        const unsigned char* colNameRaw = sqlite3_column_name(stmt, 0);
        std::string colName = colNameRaw ? reinterpret_cast<const char*>(colNameRaw) : "";
        sqlite3_finalize(stmt);

        ASSERT_TRUE(!colName.empty(), "Column name should not be empty for AS alias case");
        ASSERT_TRUE(colName == "a", ("Expected column name 'a', got '" + colName + "'").c_str());
    }

    // Test 2: SELECT a AS aa, b AS bb FROM t; verify both aliases
    {
        // Populate some data (not required, but ensures valid plan)
        rc = sqlite3_exec(db, "INSERT INTO t(a,b) VALUES (1,2);", nullptr, nullptr, nullptr);
        // Ignore if table has no data; we only verify column names from the statement
        const char* sql = "SELECT a AS aa, b AS bb FROM t;";
        sqlite3_stmt* stmt = nullptr;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK || !stmt) {
            std::cerr << "Prepare failed for test2: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return false;
        }
        const unsigned char* c0 = sqlite3_column_name(stmt, 0);
        const unsigned char* c1 = sqlite3_column_name(stmt, 1);
        std::string name0 = c0 ? reinterpret_cast<const char*>(c0) : "";
        std::string name1 = c1 ? reinterpret_cast<const char*>(c1) : "";
        sqlite3_finalize(stmt);

        ASSERT_TRUE(!name0.empty(), "First column name should not be empty in AS alias case");
        ASSERT_TRUE(!name1.empty(), "Second column name should not be empty in AS alias case");
        ASSERT_TRUE(name0 == "aa", ("Expected first column name 'aa', got '" + name0 + "'").c_str());
        ASSERT_TRUE(name1 == "bb", ("Expected second column name 'bb', got '" + name1 + "'").c_str());
    }

    // Test 3: SELECT a FROM t; column name should be the actual column name "a"
    {
        const char* sql = "SELECT a FROM t;";
        sqlite3_stmt* stmt = nullptr;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK || !stmt) {
            std::cerr << "Prepare failed for test3: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return false;
        }
        const unsigned char* c0 = sqlite3_column_name(stmt, 0);
        std::string name0 = c0 ? reinterpret_cast<const char*>(c0) : "";
        sqlite3_finalize(stmt);

        ASSERT_TRUE(!name0.empty(), "Column name should not be empty for direct table column");
        ASSERT_TRUE(name0 == "a", ("Expected column name 'a', got '" + name0 + "'").c_str());
    }

    // Test 4: SELECT rowid FROM t; verify the special rowid naming
    {
        const char* sql = "SELECT rowid FROM t;";
        sqlite3_stmt* stmt = nullptr;
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK || !stmt) {
            std::cerr << "Prepare failed for test4: " << sqlite3_errmsg(db) << "\n";
            sqlite3_close(db);
            return false;
        }
        const unsigned char* c0 = sqlite3_column_name(stmt, 0);
        std::string name0 = c0 ? reinterpret_cast<const char*>(c0) : "";
        sqlite3_finalize(stmt);

        ASSERT_TRUE(!name0.empty(), "Rowid column name should not be empty");
        ASSERT_TRUE(iequals(name0, "rowid"), ("Expected column name 'rowid', got '" + name0 + "'").c_str());
    }

    sqlite3_close(db);
    return true;
}

// Entry point: run all tests
int main() {
    int all_passed = 1;
    all_passed &= runTest("test_alias_overrides_expression_name", test_alias_overrides_expression_name);
    // If more tests are added, accumulate their results similarly.

    return all_passed ? 0 : 1;
}