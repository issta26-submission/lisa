// Unit test suite for sqlite3SelectPrep (a focal method) implemented in a minimal mock environment.
// This test harness mimics the essential control-flow of sqlite3SelectPrep to enable focused testing
// of true/false branches without requiring the full SQLite codebase.
//
// Notes:
// - We implement a local copy of sqlite3SelectPrep and its dependent functions to allow precise
//   observation of call order and early returns.
// - We use non-terminating assertions implemented as test checks that log results rather than abort.
// - All tests are self-contained in this single file and can be compiled with a C++11 compiler.
// - The tests focus on branch coverage: mallocFailed early exit, HasTypeInfo early exit, and
//   the normal path calling Expand -> Resolve -> AddTypeInfo in order, including cases where
//   Expand/Resolve set pParse->nErr to simulate error propagation.

#include <functional>
#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain-dependencies (simplified)
struct Parse;
struct Db;
struct NameContext;
struct Select;

// Global log used by the stubbed dependent functions to record call order
static std::vector<std::string> g_callLog;

// Flags to simulate error injection within dependent functions
static bool g_expand_sets_nerr = false;
static bool g_resolve_sets_nerr = false;

// Lightweight non-terminating test assertion helper
static int g_testFailures = 0;
static void expect_true(bool cond, const std::string &msg) {
    if (!cond) {
        std::cerr << "EXPECT_TRUE FAILED: " << msg << "\n";
        ++g_testFailures;
    }
}

// Lightweight logging helper
static void logCall(const std::string &name) {
    g_callLog.push_back(name);
}

// Minimal type definitions to mirror the focal function's dependencies
struct Db {
    bool mallocFailed;
    Parse *pParse; // for the assert pParse->db->pParse == pParse
    Db() : mallocFailed(false), pParse(nullptr) {}
};

struct Parse {
    Db *db;
    int nErr;
    Parse() : db(nullptr), nErr(0) {}
};

struct NameContext {
    // Intentionally empty for unit-testing hooks
};

struct Select {
    unsigned int selFlags;
    Select() : selFlags(0) {}
};

// Constant representing the SF_HasTypeInfo flag
static const unsigned int SF_HasTypeInfo = 1;

// Stubbed dependent functions. They are not real SQLite implementations; they are
// traces that allow us to validate the call order and early-exit behavior.

void sqlite3SelectExpand(Parse *pParse, Select *pSel) {
    (void)pSel; // unused in this mock
    logCall("Expand");
    if (pParse && g_expand_sets_nerr) {
        pParse->nErr = 1;
    }
}

void sqlite3ResolveSelectNames(Parse *pParse, Select *pSel, NameContext *pOuterNC) {
    (void)pSel; (void)pOuterNC;
    logCall("Resolve");
    if (pParse && g_resolve_sets_nerr) {
        pParse->nErr = 1;
    }
}

void sqlite3SelectAddTypeInfo(Parse *pParse, Select *pSel) {
    (void)pSel;
    logCall("AddTypeInfo");
    // No error injection in AddTypeInfo for the purposes of these tests
}

// The focal method, implemented here to enable targeted unit testing without pulling in the
// entire SQLite build system. The implementation mirrors the provided snippet exactly.
void sqlite3SelectPrep(
  Parse *pParse,         /* The parser context */
  Select *p,             /* The SELECT statement being coded. */
  NameContext *pOuterNC  /* Name context for container */
){
    // Critical preconditions (mirroring the original code's asserts)
    assert( p != 0 || pParse->db->mallocFailed );
    assert( pParse->db->pParse == pParse );
    if( pParse->db->mallocFailed ) return;
    if( p->selFlags & SF_HasTypeInfo ) return;

    sqlite3SelectExpand(pParse, p);
    if( pParse->nErr ) return;
    sqlite3ResolveSelectNames(pParse, p, pOuterNC);
    if( pParse->nErr ) return;
    sqlite3SelectAddTypeInfo(pParse, p);
}

// Helper to reset test environment
static void resetTestEnvironment(Parse &pParse, Db &db, Select &sel) {
    // Reset the mock DB and parser state
    db.mallocFailed = false;
    db.pParse = &pParse;
    pParse.db = &db;
    pParse.nErr = 0;
    sel.selFlags = 0;
    g_callLog.clear();
    g_expand_sets_nerr = false;
    g_resolve_sets_nerr = false;
}

// Test 1: mallocFailed path should cause early return without calling any subroutines
static void test_case_mallocFailed_path() {
    Parse pParse;
    Db db;
    Select sel;
    resetTestEnvironment(pParse, db, sel);

    // Simulate malloc failure
    db.mallocFailed = true;
    // p is non-null to satisfy the assertions
    // Call the focal method
    sqlite3SelectPrep(&pParse, &sel, nullptr);

    // Expectations
    expect_true(g_callLog.empty(), "mallocFailed path should not call any subroutines");
    expect_true(pParse.nErr == 0, "mallocFailed path should not set nErr");
    if (g_callLog.empty()) {
        // Pass
    } else {
        std::cerr << "test_case_mallocFailed_path: unexpected calls: ";
        for (auto &s : g_callLog) std::cerr << s << " ";
        std::cerr << "\n";
    }
}

// Test 2: HasTypeInfo flag should cause early return without calling subroutines
static void test_case_hasTypeInfo_path() {
    Parse pParse;
    Db db;
    Select sel;
    resetTestEnvironment(pParse, db, sel);

    // Set the HasTypeInfo flag
    sel.selFlags = SF_HasTypeInfo;

    sqlite3SelectPrep(&pParse, &sel, nullptr);

    // Expectations
    expect_true(g_callLog.empty(), "HasTypeInfo path should not call any subroutines");
    expect_true(pParse.nErr == 0, "HasTypeInfo path should not set nErr");
}

// Test 3: Normal path should call Expand -> Resolve -> AddTypeInfo in that order
static void test_case_normal_path_calls_in_order() {
    Parse pParse;
    Db db;
    Select sel;
    resetTestEnvironment(pParse, db, sel);

    // Normal path: no flags set
    sel.selFlags = 0;

    // Ensure no error injection
    g_expand_sets_nerr = false;
    g_resolve_sets_nerr = false;

    sqlite3SelectPrep(&pParse, &sel, nullptr);

    // Expectations
    std::vector<std::string> expected = {"Expand", "Resolve", "AddTypeInfo"};
    expect_true(g_callLog == expected, "Normal path should call Expand, Resolve, AddTypeInfo in order");
    expect_true(pParse.nErr == 0, "Normal path should not set nErr");
}

// Test 4: If Expand injects an error (nErr), subsequent calls should be skipped
static void test_case_expand_sets_nerr_stops_chain() {
    Parse pParse;
    Db db;
    Select sel;
    resetTestEnvironment(pParse, db, sel);

    sel.selFlags = 0;
    g_expand_sets_nerr = true;     // Expand will set pParse->nErr = 1

    sqlite3SelectPrep(&pParse, &sel, nullptr);

    // Expectations
    std::vector<std::string> expected = {"Expand"};
    expect_true(g_callLog == expected, "When Expand injects error, only Expand should be called");
    expect_true(pParse.nErr == 1, "Expand injects error should set nErr");
}

// Test 5: If Resolve injects an error, path should stop after Resolve
static void test_case_resolve_sets_nerr_stops_after_resolve() {
    Parse pParse;
    Db db;
    Select sel;
    resetTestEnvironment(pParse, db, sel);

    sel.selFlags = 0;
    g_expand_sets_nerr = false;
    g_resolve_sets_nerr = true; // Resolve will set pParse->nErr = 1

    sqlite3SelectPrep(&pParse, &sel, nullptr);

    // Expectations
    std::vector<std::string> expected = {"Expand", "Resolve"};
    expect_true(g_callLog == expected, "Resolve injects error, should stop after Resolve");
    expect_true(pParse.nErr == 1, "Resolve injects error should set nErr");
}

// Test 6: Final path (no errors) should again call all three in order after previous tests
static void test_case_normal_path_again() {
    Parse pParse;
    Db db;
    Select sel;
    resetTestEnvironment(pParse, db, sel);

    sel.selFlags = 0;
    g_expand_sets_nerr = false;
    g_resolve_sets_nerr = false;

    sqlite3SelectPrep(&pParse, &sel, nullptr);

    std::vector<std::string> expected = {"Expand", "Resolve", "AddTypeInfo"};
    expect_true(g_callLog == expected, "Second normal path should call all three in order");
    expect_true(pParse.nErr == 0, "Second normal path should not set nErr");
}

// Main function to run all tests
int main() {
    std::cout << "Starting sqlite3SelectPrep unit tests (mocked environment)\n";

    test_case_mallocFailed_path();
    test_case_hasTypeInfo_path();
    test_case_normal_path_calls_in_order();
    test_case_expand_sets_nerr_stops_chain();
    test_case_resolve_sets_nerr_stops_after_resolve();
    test_case_normal_path_again();

    if (g_testFailures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_testFailures << " TEST(S) FAILED\n";
        return 1;
    }
}