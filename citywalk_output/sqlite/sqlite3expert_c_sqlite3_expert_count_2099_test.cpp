#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <sqlite3expert.h>


// Include the C header with C linkage to ensure proper linking with the C function.
// This header is expected to expose the sqlite3expert type and IdxStatement type.
extern "C" {
}

// Simple, non-terminating test harness to collect and report results.
// This avoids aborting on first failure and maximizes code coverage.
static int g_total_tests = 0;
static int g_passed_tests = 0;

static void record_result(const char* testName, bool passed) {
    ++g_total_tests;
    if (passed) ++g_passed_tests;
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << testName << std::endl;
}

// Test 1: When pStatement is null, sqlite3_expert_count should return 0 (true path false statement).
// This covers the false-branch of the condition in sqlite3_expert_count.
static void test_noStatementReturnsZero() {
    sqlite3expert p;
    p.pStatement = nullptr;
    int result = sqlite3_expert_count(&p);
    record_result("test_noStatementReturnsZero", result == 0);
}

// Test 2: When pStatement is non-null and iId is 0, sqlite3_expert_count should return 1.
// This verifies the true-branch with a minimal non-negative iId value.
static void test_statementWithIIdZero() {
    IdxStatement* st = new IdxStatement;
    st->iId = 0;

    sqlite3expert p;
    p.pStatement = st;

    int result = sqlite3_expert_count(&p);
    record_result("test_statementWithIIdZero", result == 1);

    delete st;
}

// Test 3: When pStatement is non-null and iId is a positive value, sqlite3_expert_count should return iId+1.
// This ensures normal increment behavior for typical iId values.
static void test_statementWithIIdPositive() {
    IdxStatement* st = new IdxStatement;
    st->iId = 7;

    sqlite3expert p;
    p.pStatement = st;

    int result = sqlite3_expert_count(&p);
    record_result("test_statementWithIIdPositive", result == 8);

    delete st;
}

int main() {
    // Execute all tests
    test_noStatementReturnsZero();
    test_statementWithIIdZero();
    test_statementWithIIdPositive();

    // Summary report
    std::cout << "Summary: " << g_passed_tests << " / " << g_total_tests << " tests passed." << std::endl;
    // Return non-zero if any test failed
    return (g_passed_tests == g_total_tests) ? 0 : 1;
}