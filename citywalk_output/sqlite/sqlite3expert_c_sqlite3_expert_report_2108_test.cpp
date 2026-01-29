// C++11 test suite for the focal C function sqlite3_expert_report
// Tests are designed to be run without Google Test. A lightweight
// in-file test harness is used to perform non-terminating checks.
// The test suite assumes the availability of sqlite3expert.h and the
// corresponding sqlite3_expert_report implementation in the project.

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sqlite3expert.h>


// Include the focal header to obtain the type definitions and constants.
// It is assumed that sqlite3expert.h provides:
 // - typedef struct sqlite3expert sqlite3expert;
 // - typedef struct IdxStatement IdxStatement;
 // - enum/defines for EXPERT_REPORT_SQL, EXPERT_REPORT_INDEXES, EXPERT_REPORT_PLAN, EXPERT_REPORT_CANDIDATES

// Lightweight test harness
static int g_failures = 0;
static void test_fail(const std::string &msg, const char *file, int line) {
    std::cerr << "Test failure: " << msg << " (" << file << ":" << line << ")\n";
    ++g_failures;
}
#define TEST_CHECK(cond, msg) do { \
    if(!(cond)) { test_fail(msg, __FILE__, __LINE__); } \
} while(0)

// Simple test cases for sqlite3_expert_report

// Test 1: When bRun != 0 and matching iStmt exists, EXPERT_REPORT_SQL should return the SQL text.
void test_report_sql_returns_sql_when_statement_exists() {
    // Arrange
    IdxStatement stmt;
    stmt.iId = 123;
    stmt.zSql = "SELECT * FROM t";
    stmt.zIdx = "idx_t";
    stmt.zEQP = "EXPLAIN QUERY PLAN";
    stmt.pNext = nullptr;

    sqlite3expert p;
    p.bRun = 1;
    p.pStatement = &stmt;
    p.zCandidates = nullptr;

    // Act
    const char *res = sqlite3_expert_report(&p, 123, EXPERT_REPORT_SQL);

    // Assert
    TEST_CHECK(res != nullptr, "EXPERT_REPORT_SQL should return non-null SQL when statement exists");
    if(res) {
        TEST_CHECK(std::string(res) == "SELECT * FROM t",
                   "EXPERT_REPORT_SQL returned unexpected SQL text");
    }
}

// Test 2: When iStmt does not exist, EXPERT_REPORT_SQL should return nullptr.
void test_report_sql_returns_null_when_statement_not_found() {
    // Arrange
    IdxStatement stmt;
    stmt.iId = 123;
    stmt.zSql = "SELECT 1";
    stmt.zIdx = nullptr;
    stmt.zEQP = nullptr;
    stmt.pNext = nullptr;

    sqlite3expert p;
    p.bRun = 1;
    p.pStatement = &stmt;
    p.zCandidates = nullptr;

    // Act
    const char *res = sqlite3_expert_report(&p, 9999, EXPERT_REPORT_SQL);

    // Assert
    TEST_CHECK(res == nullptr, "EXPERT_REPORT_SQL should return null when statement not found");
}

// Test 3: EXPERT_REPORT_INDEXES should return the zIdx from the matching statement.
void test_report_indexes_returns_zidx_when_statement_exists() {
    // Arrange
    IdxStatement stmt;
    stmt.iId = 555;
    stmt.zSql = "SELECT 2";
    stmt.zIdx = "idx_555";
    stmt.zEQP = nullptr;
    stmt.pNext = nullptr;

    sqlite3expert p;
    p.bRun = 1;
    p.pStatement = &stmt;
    p.zCandidates = nullptr;

    // Act
    const char *res = sqlite3_expert_report(&p, 555, EXPERT_REPORT_INDEXES);

    // Assert
    TEST_CHECK(res != nullptr, "EXPERT_REPORT_INDEXES should return non-null zIdx when statement exists");
    if(res) {
        TEST_CHECK(std::string(res) == "idx_555",
                   "EXPERT_REPORT_INDEXES returned unexpected zIdx");
    }
}

// Test 4: EXPERT_REPORT_PLAN should return the zEQP from the matching statement.
void test_report_plan_returns_eqp_when_statement_exists() {
    // Arrange
    IdxStatement stmt;
    stmt.iId = 888;
    stmt.zSql = "SELECT 3";
    stmt.zIdx = nullptr;
    stmt.zEQP = "PLAN_FOR_888";
    stmt.pNext = nullptr;

    sqlite3expert p;
    p.bRun = 1;
    p.pStatement = &stmt;
    p.zCandidates = nullptr;

    // Act
    const char *res = sqlite3_expert_report(&p, 888, EXPERT_REPORT_PLAN);

    // Assert
    TEST_CHECK(res != nullptr, "EXPERT_REPORT_PLAN should return non-null zEQP when statement exists");
    if(res) {
        TEST_CHECK(std::string(res) == "PLAN_FOR_888",
                   "EXPERT_REPORT_PLAN returned unexpected zEQP");
    }
}

// Test 5: EXPERT_REPORT_CANDIDATES should return the zCandidates string from the p object.
void test_report_candidates_returns_candidates_from_parent() {
    // Arrange
    IdxStatement stmt;
    stmt.iId = 1;
    stmt.zSql = "SELECT 1";
    stmt.zIdx = nullptr;
    stmt.zEQP = nullptr;
    stmt.pNext = nullptr;

    sqlite3expert p;
    p.bRun = 1;
    p.pStatement = &stmt; // statement present but not needed for this report
    p.zCandidates = "CANDIDATES_LIST";

    // Act
    const char *res = sqlite3_expert_report(&p, 1, EXPERT_REPORT_CANDIDATES);

    // Assert
    TEST_CHECK(res != nullptr, "EXPERT_REPORT_CANDIDATES should return non-null candidates string");
    if(res) {
        TEST_CHECK(std::string(res) == "CANDIDATES_LIST",
                   "EXPERT_REPORT_CANDIDATES returned unexpected candidates text");
    }
}

// Test 6: When bRun == 0, all reports should return nullptr regardless of others.
void test_report_when_bRun_zero_returns_null() {
    // Arrange
    IdxStatement stmt;
    stmt.iId = 42;
    stmt.zSql = "SELECT 42";
    stmt.zIdx = "idx_42";
    stmt.zEQP = "PLAN_42";
    stmt.pNext = nullptr;

    sqlite3expert p;
    p.bRun = 0;
    p.pStatement = &stmt;
    p.zCandidates = "CANDIDATES_COULD_BE_here";

    // Act
    const char *res_sql = sqlite3_expert_report(&p, 42, EXPERT_REPORT_SQL);
    const char *res_idx = sqlite3_expert_report(&p, 42, EXPERT_REPORT_INDEXES);
    const char *res_plan = sqlite3_expert_report(&p, 42, EXPERT_REPORT_PLAN);
    const char *res_cand = sqlite3_expert_report(&p, 42, EXPERT_REPORT_CANDIDATES);

    // Assert
    TEST_CHECK(res_sql == nullptr, "When bRun==0, EXPERT_REPORT_SQL should return nullptr");
    TEST_CHECK(res_idx == nullptr, "When bRun==0, EXPERT_REPORT_INDEXES should return nullptr");
    TEST_CHECK(res_plan == nullptr, "When bRun==0, EXPERT_REPORT_PLAN should return nullptr");
    // Even when bRun==0, EXPERT_REPORT_CANDIDATES should typically still return zCandidates,
    // but the implementation returns zRet = p->zCandidates only regardless of pStmt when eReport == CANDIDATES.
    // Since bRun is checked at the very start, it should still be 0 here; however the code path
    // returns 0 before the switch, so test accordingly.
    TEST_CHECK(res_cand == nullptr, "When bRun==0, EXPERT_REPORT_CANDIDATES should return nullptr");
}

// Helper function to run all tests
void run_all_tests() {
    test_report_sql_returns_sql_when_statement_exists();
    test_report_sql_returns_null_when_statement_not_found();
    test_report_indexes_returns_zidx_when_statement_exists();
    test_report_plan_returns_eqp_when_statement_exists();
    test_report_candidates_returns_candidates_from_parent();
    test_report_when_bRun_zero_returns_null();
}

// Main entry point
int main() {
    run_all_tests();

    if(g_failures == 0) {
        std::cout << "ALL TESTS PASSED (" << 6 << " tests)" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " OF 6 TESTS FAILED" << std::endl;
        return 1;
    }
}