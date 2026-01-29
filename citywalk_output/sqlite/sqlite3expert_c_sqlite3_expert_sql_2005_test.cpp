// C++11 test harness for sqlite3_expert_sql (sqlite3_expert_sql) in sqlite3expert.c
// This test suite is designed to be compiled with a real SQLite3 library and the
// sqlite3expert module available in the project. It exercises true/false
// branches of the focal method as much as possible without using GTest.
// To compile (example):
//   g++ -std=c++11 -I. -I/path/to/sqlite/include test_sqlite3_expert_sql.cpp -L/path/to/sqlite/lib -lsqlite3 -o test_sqlite3_expert_sql
//
// Note: The test uses the actual sqlite3expert.h interface. It prints
// diagnostic information to stdout/stderr rather than terminating on first failure.

#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cassert>
#include <sqlite3expert.h>


extern "C" {
}

// Helper for portable test logging
static void log_test(const std::string& name, bool passed, const std::string& detail = "") {
    if (passed) {
        std::cout << "[PASS] " << name;
        if (!detail.empty()) std::cout << " - " << detail;
        std::cout << std::endl;
    } else {
        std::cerr << "[FAIL] " << name;
        if (!detail.empty()) std::cerr << " - " << detail;
        std::cerr << std::endl;
    }
}

// Convenience macro for lightweight checks without stopping test flow
#define EXPECT(cond, name, detail) log_test(name, (cond), detail)

// Utility: create a fresh sqlite3expert instance for tests
static sqlite3expert* create_expert(sqlite3* db, const std::string& test_tag, char** perr) {
    sqlite3expert* p = sqlite3_expert_new(db, perr);
    if (!p) {
        std::cerr << "[ERR] Failed to create sqlite3expert for " << test_tag << std::endl;
    }
    return p;
}

// Test 1: Ensure early return on bRun flag (SQLITE_MISUSE)
static bool test_sqlite3_expert_sql_run_misuse() {
    // Setup in-memory database
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        std::cerr << "Failed to open in-memory DB for Test 1" << std::endl;
        return false;
    }

    char* perr = nullptr;
    sqlite3expert* p = create_expert(db, "Test1_run_misuse", &perr);
    if (!p) {
        sqlite3_close(db);
        return false;
    }

    // Force misuse state
    p->bRun = 1;

    // This should return SQLITE_MISUSE without attempting to parse SQL
    char* pzErr = nullptr;
    SQLITE_RETCODE rc = sqlite3_expert_sql(p, "SELECT 1", &pzErr);

    bool ok = (rc == SQLITE_MISUSE);

    // Cleanup
    sqlite3_expert_destroy(p);
    sqlite3_close(db);
    return ok;
}

// Test 2: Empty SQL should be a no-op (rc == SQLITE_OK, no statements added)
static bool test_sqlite3_expert_sql_empty_sql() {
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        std::cerr << "Failed to open in-memory DB for Test 2" << std::endl;
        return false;
    }

    char* perr = nullptr;
    sqlite3expert* p = create_expert(db, "Test2_empty_sql", &perr);
    if (!p) {
        sqlite3_close(db);
        return false;
    }

    // Ensure starting state
    p->pScan = nullptr;
    p->pStatement = nullptr;
    char* pzErr = nullptr;

    // Pass an empty string as SQL
    SQLITE_RETCODE rc = sqlite3_expert_sql(p, "", &pzErr);

    // Expect no changes and successful return
    bool ok = (rc == SQLITE_OK) && (p->pStatement == nullptr);

    sqlite3_expert_destroy(p);
    sqlite3_close(db);
    return ok;
}

// Test 3: Simple statement appended to the statement list
static bool test_sqlite3_expert_sql_single_statement() {
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        std::cerr << "Failed to open in-memory DB for Test 3" << std::endl;
        return false;
    }

    char* perr = nullptr;
    sqlite3expert* p = create_expert(db, "Test3_single_stmt", &perr);
    if (!p) {
        sqlite3_close(db);
        return false;
    }

    // Start fresh
    p->pScan = nullptr;
    p->pStatement = nullptr;

    char* pzErr = nullptr;
    SQLITE_RETCODE rc = sqlite3_expert_sql(p, "SELECT 1", &pzErr);

    bool ok = true;
    if (rc != SQLITE_OK) {
        ok = false;
        log_test("Test3_single_stmt - rc check", false, "expected SQLITE_OK");
    } else if (p->pStatement == nullptr) {
        ok = false;
        log_test("Test3_single_stmt - statement appended", false, "pStatement is NULL after call");
    } else {
        // We expect zSql to contain the SQL text derived from the prepared statement
        const char* z = p->pStatement->zSql;
        if (z == nullptr || z[0] == '\0') {
            ok = false;
            log_test("Test3_single_stmt - zSql content", false, "zSql is empty");
        } else {
            log_test("Test3_single_stmt - zSql content", true, std::string("zSql='") + z + "'");
        }
    }

    sqlite3_expert_destroy(p);
    sqlite3_close(db);
    return ok;
}

// Test 4: Multiple statements should be chained in reverse-chronological order
static bool test_sqlite3_expert_sql_multiple_statements() {
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        std::cerr << "Failed to open in-memory DB for Test 4" << std::endl;
        return false;
    }

    char* perr = nullptr;
    sqlite3expert* p = create_expert(db, "Test4_multiple_stmt", &perr);
    if (!p) {
        sqlite3_close(db);
        return false;
    }

    // First statement
    {
        p->pScan = nullptr;
        p->pStatement = nullptr;
        char* pzErr = nullptr;
        SQLITE_RETCODE rc = sqlite3_expert_sql(p, "SELECT 1", &pzErr);
        if (rc != SQLITE_OK || p->pStatement == nullptr) {
            sqlite3_expert_destroy(p);
            sqlite3_close(db);
            return false;
        }
    }

    // Second statement (should push on top)
    {
        char* pzErr = nullptr;
        SQLITE_RETCODE rc = sqlite3_expert_sql(p, "SELECT 2", &pzErr);
        if (rc != SQLITE_OK || p->pStatement == nullptr) {
            sqlite3_expert_destroy(p);
            sqlite3_close(db);
            return false;
        }
    }

    // Now we should have at least two statements in a chain
    bool ok = true;
    if (p->pStatement == nullptr || p->pStatement->pNext == nullptr) {
        ok = false;
        log_test("Test4_multiple_stmt - chain presence", false, "Expected two statements in chain");
    } else {
        const char* newest = p->pStatement->zSql;
        const char* older  = p->pStatement->pNext ? p->pStatement->pNext->zSql : nullptr;
        if (!newest || newest[0] == '\0') {
            ok = false;
            log_test("Test4_multiple_stmt - newest zSql", false, "Newest zSql empty");
        } else {
            log_test("Test4_multiple_stmt - newest zSql", true, std::string("Newest zSql='") + newest + "'");
        }
        if (!older) {
            ok = false;
            log_test("Test4_multiple_stmt - older zSql", false, "Older zSql missing");
        } else {
            log_test("Test4_multiple_stmt - older zSql", true, std::string("Older zSql='") + older + "'");
        }
    }

    sqlite3_expert_destroy(p);
    sqlite3_close(db);
    return ok;
}

// Main: run all tests and report summary
int main() {
    std::vector<bool> results;

    results.push_back(test_sqlite3_expert_sql_run_misuse());
    results.push_back(test_sqlite3_expert_sql_empty_sql());
    results.push_back(test_sqlite3_expert_sql_single_statement());
    results.push_back(test_sqlite3_expert_sql_multiple_statements());

    int failures = 0;
    for (size_t i = 0; i < results.size(); ++i) {
        if (!results[i]) ++failures;
    }

    if (failures == 0) {
        std::cout << "\nALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "\n" << failures << " TEST[S] FAILED" << std::endl;
        return 1;
    }
}