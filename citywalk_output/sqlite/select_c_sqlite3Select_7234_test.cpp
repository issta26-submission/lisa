// Lightweight C++11 test suite for exercising SQLite's sqlite3Select behavior
// via the public SQLite API, focusing on diverse query constructs that
// exercise branches that sqlite3Select handles internally.
// This test uses the in-process SQLite C API (sqlite3_*). It does not rely
// on Google Test; instead it uses simple assertion helpers to validate results.
// Note: This test targets coverage by issuing real SQL statements that drive
// the internal sqlite3Select flow within SQLite's engine.

#include <vector>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Simple non-terminating assertion macro for test results.
// On mismatch, prints a helpful message but does not abort the program.
#define EXPECT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "Test Failed: " << (msg) \
                  << " | Expected: " << (expected) \
                  << " | Actual: " << (actual) << std::endl; \
        gFailures++; \
    } \
} while(0)

static int gFailures = 0;

// Helper: Convert a SQLite column value to a string; NULL becomes "NULL".
static std::string columnToString(sqlite3_stmt* stmt, int idx) {
    const unsigned char* text = sqlite3_column_text(stmt, idx);
    if (text == nullptr) return "NULL";
    return std::string(reinterpret_cast<const char*>(text));
}

// Helper: Execute a SQL query and fetch all results as a 2D vector of strings.
// Each inner vector represents a row; each string is a column value (with NULL as "NULL").
static std::vector<std::vector<std::string>> fetchQueryResults(sqlite3* db, const std::string& sql) {
    sqlite3_stmt* stmt = nullptr;
    std::vector<std::vector<std::string>> results;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return results;
    }

    int rc;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int ncol = sqlite3_column_count(stmt);
        std::vector<std::string> row;
        row.reserve(ncol);
        for (int i = 0; i < ncol; i++) {
            row.push_back(columnToString(stmt, i));
        }
        results.push_back(row);
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Error stepping through results: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return results;
}

// Helper: Print results for debugging (optional).
static void printResults(const std::vector<std::vector<std::string>>& rows) {
    for (const auto& r : rows) {
        for (size_t i = 0; i < r.size(); ++i) {
            if (i) std::cout << " | ";
            std::cout << r[i];
        }
        std::cout << std::endl;
    }
}

// Setup the in-memory database with a small dataset to exercise sqlite3Select paths.
static void setupDatabase(sqlite3* db) {
    const char* setupSQL =
        "CREATE TABLE t1 (id INTEGER PRIMARY KEY, name TEXT, val INTEGER);"
        "INSERT INTO t1 (name, val) VALUES "
        "('Alice', 10),"
        "('Bob', 20),"
        "('Alice', 30),"
        "('Charlie', 40),"
        "(NULL, 50),"
        "('Eve', 60);"
        "CREATE TABLE t2 (id INTEGER PRIMARY KEY, name TEXT);"
        "INSERT INTO t2 (name) VALUES "
        "('Alice'),"
        "('Bob'),"
        "('Alice'),"
        "('Dora');";

    char* errmsg = nullptr;
    int rc = sqlite3_exec(db, setupSQL, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Setup error: " << errmsg << std::endl;
        sqlite3_free(errmsg);
        std::exit(1);
    }
}

// Run all test cases
int main() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open in-memory SQLite DB: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    setupDatabase(db);

    // 1) Simple SELECT
    {
        std::string sql = "SELECT 1 AS one;";
        auto rows = fetchQueryResults(db, sql);
        // Expect 1 row, 1 column with value "1"
        int expectedRowCount = 1;
        int expectedColCount = 1;
        EXPECT_EQ((int)rows.size(), expectedRowCount, "Simple SELECT should return 1 row");
        if (!rows.empty()) {
            EXPECT_EQ((int)rows[0].size(), expectedColCount, "Simple SELECT should return 1 column");
            EXPECT_EQ(rows[0][0], std::string("1"), "Simple SELECT value should be '1'");
        }
    }

    // 2) DISTINCT (including NULL handling)
    {
        std::string sql = "SELECT DISTINCT name FROM t1 ORDER BY name;";
        auto rows = fetchQueryResults(db, sql);
        // Expected distinct names: NULL, Alice, Bob, Charlie, Eve (NULL sorts first in SQLite when ORDER BY ASC)
        std::vector<std::vector<std::string>> expected = {
            {"NULL"},
            {"Alice"},
            {"Bob"},
            {"Charlie"},
            {"Eve"}
        };
        // Compare sizes
        EXPECT_EQ((int)rows.size(), (int)expected.size(), "DISTINCT name count mismatch");
        // Deep compare
        if (rows.size() == expected.size()) {
            for (size_t i = 0; i < expected.size(); ++i) {
                EXPECT_EQ(rows[i].size(), expected[i].size(), "DISTINCT row column count mismatch");
                if (rows[i].size() == expected[i].size()) {
                    EXPECT_EQ(rows[i][0], expected[i][0], "DISTINCT name mismatch at row " + std::to_string(i));
                }
            }
        }
    }

    // 3) GROUP BY
    {
        std::string sql = "SELECT name, COUNT(*) FROM t1 GROUP BY name ORDER BY name;";
        auto rows = fetchQueryResults(db, sql);
        // Expected:
        // NULL,1
        // Alice,2
        // Bob,1
        // Charlie,1
        // Eve,1
        std::vector<std::vector<std::string>> expected = {
            {"NULL", "1"},
            {"Alice", "2"},
            {"Bob", "1"},
            {"Charlie", "1"},
            {"Eve", "1"}
        };
        EXPECT_EQ((int)rows.size(), (int)expected.size(), "GROUP BY row count mismatch");
        if (rows.size() == expected.size()) {
            for (size_t i = 0; i < expected.size(); ++i) {
                EXPECT_EQ(rows[i].size(), expected[i].size(), "GROUP BY column count mismatch at row " + std::to_string(i));
                if (rows[i].size() == expected[i].size()) {
                    EXPECT_EQ(rows[i][0], expected[i][0], "GROUP BY name mismatch at row " + std::to_string(i));
                    EXPECT_EQ(rows[i][1], expected[i][1], "GROUP BY count mismatch at row " + std::to_string(i));
                }
            }
        }
    }

    // 4) HAVING
    {
        std::string sql = "SELECT name, COUNT(*) FROM t1 GROUP BY name HAVING COUNT(*)>1;";
        auto rows = fetchQueryResults(db, sql);
        // Expected: Alice,2
        std::vector<std::vector<std::string>> expected = {
            {"Alice", "2"}
        };
        EXPECT_EQ((int)rows.size(), (int)expected.size(), "HAVING row count mismatch");
        if (rows.size() == expected.size()) {
            EXPECT_EQ(rows[0].size(), expected[0].size(), "HAVING column count mismatch");
            if (rows[0].size() == expected[0].size()) {
                EXPECT_EQ(rows[0][0], expected[0][0], "HAVING name mismatch");
                EXPECT_EQ(rows[0][1], expected[0][1], "HAVING count mismatch");
            }
        }
    }

    // 5) LEFT JOIN (FROM clause join scenario)
    {
        std::string sql =
            "SELECT t1.name, t2.id "
            "FROM t1 LEFT JOIN t2 ON t1.name = t2.name "
            "ORDER BY t1.name, t2.id;";
        auto rows = fetchQueryResults(db, sql);
        // Expected ordering:
        // NULL,NULL
        // Alice,1
        // Alice,3
        // Bob,2
        // Charlie,NULL
        // Eve,NULL
        std::vector<std::vector<std::string>> expected = {
            {"NULL", "NULL"},
            {"Alice", "1"},
            {"Alice", "3"},
            {"Bob", "2"},
            {"Charlie", "NULL"},
            {"Eve", "NULL"}
        };
        EXPECT_EQ((int)rows.size(), (int)expected.size(), "LEFT JOIN row count mismatch");
        if (rows.size() == expected.size()) {
            for (size_t i = 0; i < expected.size(); ++i) {
                EXPECT_EQ(rows[i].size(), expected[i].size(), "LEFT JOIN column count mismatch at row " + std::to_string(i));
                if (rows[i].size() == expected[i].size()) {
                    EXPECT_EQ(rows[i][0], expected[i][0], "LEFT JOIN name mismatch at row " + std::to_string(i));
                    EXPECT_EQ(rows[i][1], expected[i][1], "LEFT JOIN id mismatch at row " + std::to_string(i));
                }
            }
        }
    }

    // 6) Subquery (IN with subselect)
    {
        std::string sql = "SELECT name FROM t1 WHERE id IN (SELECT id FROM t1 WHERE val>25) ORDER BY name;";
        auto rows = fetchQueryResults(db, sql);
        // Expected: NULL, Alice, Charlie, Eve (names corresponding to ids 3,4,5,6; NULL for id5)
        std::vector<std::vector<std::string>> expected = {
            {"NULL"},
            {"Alice"},
            {"Charlie"},
            {"Eve"}
        };
        EXPECT_EQ((int)rows.size(), (int)expected.size(), "SUBQUERY IN row count mismatch");
        if (rows.size() == expected.size()) {
            for (size_t i = 0; i < expected.size(); ++i) {
                EXPECT_EQ(rows[i].size(), expected[i].size(), "SUBQUERY IN column count mismatch at row " + std::to_string(i));
                if (rows[i].size() == expected[i].size()) {
                    EXPECT_EQ(rows[i][0], expected[i][0], "SUBQUERY IN name mismatch at row " + std::to_string(i));
                }
            }
        }
    }

    // 7) ORDER BY with LIMIT
    {
        std::string sql = "SELECT name FROM t1 ORDER BY name LIMIT 3;";
        auto rows = fetchQueryResults(db, sql);
        // Expected: NULL, Alice, Bob
        std::vector<std::vector<std::string>> expected = {
            {"NULL"}, {"Alice"}, {"Bob"}
        };
        EXPECT_EQ((int)rows.size(), (int)expected.size(), "ORDER BY LIMIT row count mismatch");
        if (rows.size() == expected.size()) {
            for (size_t i = 0; i < expected.size(); ++i) {
                EXPECT_EQ(rows[i][0], expected[i][0], "ORDER BY LIMIT value mismatch at row " + std::to_string(i));
            }
        }
    }

    // Finalize
    sqlite3_close(db);

    if (gFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << gFailures << " test(s) failed." << std::endl;
        return 2;
    }
}