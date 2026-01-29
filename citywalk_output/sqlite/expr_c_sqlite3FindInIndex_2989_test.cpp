// sqlite3FindInIndex - Unit test suite (C++11) without GoogleTest
// This test suite exercises the high-level behavior that sqlite3FindInIndex
// drives in the query planning phase by inspecting the public query plan output.
// It does not rely on private internal states directly, but instead validates
// that the planner selects appropriate IN-operator handling (index usage, ROWID path, etc.)
// via EXPLAIN QUERY PLAN. The tests are designed to be executed with the system
// sqlite3 library provided in the test environment.

#include <functional>
#include <vector>
#include <memory>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>



// Simple non-terminating test harness
#define LOG_ERROR(msg) std::cerr << "ERROR: " << msg << std::endl;
#define LOG_INFO(msg)  std::cout << "INFO: "  << msg << std::endl;

// Simple assertion macro that does not exit on failure
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { LOG_ERROR("Test failed: " msg); return false; } \
} while(0)
#define EXPECT_FALSE(cond, msg) do { \
    if((cond)) { LOG_ERROR("Test failed: " msg); return false; } \
} while(0)

// Helper: execute a SQL script (DDL/DML) on an open database
static bool execSql(sqlite3* db, const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if( rc != SQLITE_OK ){
        LOG_ERROR("sqlite3_exec failed: " << (errMsg ? errMsg : "unknown error"));
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// Helper: Run EXPLAIN QUERY PLAN on a SQL statement and return the textual plan
static std::string explainPlan(sqlite3* db, const std::string& sql) {
    std::string explainSql = "EXPLAIN QUERY PLAN " + sql;
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, explainSql.c_str(), -1, &stmt, nullptr);
    if( rc != SQLITE_OK ){
        LOG_ERROR("Failed to prepare explain plan: " << sqlite3_errmsg(db));
        return "";
    }

    std::string plan;
    while( (rc = sqlite3_step(stmt)) == SQLITE_ROW ){
        int cols = sqlite3_column_count(stmt);
        if( cols > 0 ){
            // The last column is the textual detail for the plan
            const unsigned char* txt = sqlite3_column_text(stmt, cols - 1);
            if( txt ){
                plan += reinterpret_cast<const char*>(txt);
                plan += "\n";
            }
        }
    }
    if( rc != SQLITE_DONE ){
        LOG_ERROR("Explain plan step failed: " << sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    return plan;
}

// Test 1: IN with a subquery that should use an index on the RHS
// Expectation: The planner uses the index on t2(y) for the IN-OPERATOR.
// Plan should contain the index name "idx_t2_y".
static bool test_findInIndex_subquery_uses_index() {
    LOG_INFO("Test 1: IN with subquery should use RHS index when available");

    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if( rc != SQLITE_OK || !db ){
        LOG_ERROR("Unable to open in-memory SQLite DB.");
        return false;
    }

    // Setup schema and data
    const std::string setup =
        "CREATE TABLE t1(x INTEGER);"
        "CREATE TABLE t2(y INTEGER);"
        "CREATE INDEX idx_t2_y ON t2(y);"
        "INSERT INTO t1(x) VALUES (1),(2),(3);"
        "INSERT INTO t2(y) VALUES (1),(2);";

    if(!execSql(db, setup)) {
        sqlite3_close(db);
        return false;
    }

    // Explain plan for IN with subquery
    const std::string sql = "SELECT 1 FROM t1 WHERE x IN (SELECT y FROM t2)";
    std::string plan = explainPlan(db, sql);

    sqlite3_close(db);

    // Requirement: plan should reference the RHS index name
    bool containsIndexName = plan.find("idx_t2_y") != std::string::npos;
    EXPECT_TRUE(containsIndexName, "Plan should mention index idx_t2_y for IN-OPERATOR");
    return true;
}

// Test 2: IN with a constant list and no index on the LHS
// Expectation: The planner should not rely on an index for the IN with a constant list.
// We verify absence of "USING INDEX" in the explain plan.
static bool test_findInIndex_list_no_index_usage() {
    LOG_INFO("Test 2: IN with constant list should not use index when none exists");

    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if( rc != SQLITE_OK || !db ){
        LOG_ERROR("Unable to open in-memory SQLite DB.");
        return false;
    }

    // Setup: create a simple table with no index
    const std::string setup =
        "CREATE TABLE t1(a INTEGER);"
        "INSERT INTO t1(a) VALUES (1),(2),(3);";

    if(!execSql(db, setup)) {
        sqlite3_close(db);
        return false;
    }

    // Explain plan for IN with a literal list
    const std::string sql = "SELECT 1 FROM t1 WHERE a IN (1,2,3)";
    std::string plan = explainPlan(db, sql);

    sqlite3_close(db);

    // Expectation: No index should be used; search for "USING INDEX" should fail
    bool usesIndex = plan.find("USING INDEX") != std::string::npos;
    EXPECT_FALSE(usesIndex, "Plan should not use an index for IN with literal list when no index exists");
    return true;
}

// Test 3: IN with subquery returning ROWIDs should use ROWID SEARCH path
// Expectation: The planner uses ROWID-based search on the RHS table.
// Plan should contain "ROWID SEARCH ON TABLE t2" (or variant containing ROWID).
static bool test_findInIndex_subquery_rowid_path() {
    LOG_INFO("Test 3: IN with subquery returning ROWIDs should use ROWID path on RHS table");

    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if( rc != SQLITE_OK || !db ){
        LOG_ERROR("Unable to open in-memory SQLite DB.");
        return false;
    }

    // Setup: t1 and t2 with a ROWID-based RHS
    const std::string setup =
        "CREATE TABLE t1(x INTEGER);"
        "CREATE TABLE t2(a INTEGER);"
        "INSERT INTO t1(x) VALUES (10);"
        "INSERT INTO t2(a) VALUES (1),(2);";

    if(!execSql(db, setup)) {
        sqlite3_close(db);
        return false;
    }

    // The RHS query selects the ROWID (i.e., pseudo-column)
    const std::string sql = "SELECT 1 FROM t1 WHERE x IN (SELECT rowid FROM t2)";
    std::string plan = explainPlan(db, sql);

    sqlite3_close(db);

    // We expect the ROWID search path to appear
    bool hasRowidPath = plan.find("ROWID SEARCH ON TABLE t2") != std::string::npos
                     || plan.find("ROWID SEARCH ON TABLE t2 FOR IN-OPERATOR") != std::string::npos;
    EXPECT_TRUE(hasRowidPath, "Plan should show ROWID SEARCH on RHS table t2 for IN-OPERATOR");
    return true;
}

// Entry-point: Run all tests
int main() {
    int total = 0;
    int passed = 0;

    auto runTest = [&](const std::string& name, const std::function<bool()>& f){
        total++;
        bool ok = f();
        if(ok) {
            ++passed;
            std::cout << "[PASS] " << name << std::endl;
        } else {
            std::cout << "[FAIL] " << name << std::endl;
        }
    };

    runTest("test_findInIndex_subquery_uses_index", test_findInIndex_subquery_uses_index);
    runTest("test_findInIndex_list_no_index_usage", test_findInIndex_list_no_index_usage);
    runTest("test_findInIndex_subquery_rowid_path", test_findInIndex_subquery_rowid_path);

    std::cout << "Tests passed " << passed << " / " << total << std::endl;
    return (passed == total) ? 0 : 1;
}