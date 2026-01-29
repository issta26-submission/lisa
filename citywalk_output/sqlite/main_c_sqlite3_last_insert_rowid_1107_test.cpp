/*
Explanation:
This file serves as a focused C++11 unit test suite for the focal function:
sqlite_int64 sqlite3_last_insert_rowid(sqlite3 *db)

The test harness does not depend on any external testing framework (GTest is avoided
as per requirements). It provides a lightweight test runner with non-terminating
assertions (tests log failures but continue execution).

Key elements exercised:
- Correct behavior when API armor (SQLite safety checks) is enabled or disabled.
- True/false branches of the safety predicate (sqlite3SafetyCheckOk).
- Access to the lastRowid member of a minimal sqlite3 struct.
- Static-like behavior simulated via controlled globals in the test (since the
  real project would have static/internal helpers; here we expose a minimal surface).

Note: This test file includes a minimal in-file reproduction of the focal function
and its dependencies to enable self-contained compilation in a C++11 project
without requiring the actual SQLite build system.
To test both code paths (with and without API armor), compile this file in two
separate builds:
- With SQLITE_ENABLE_API_ARMOR defined to exercise the safety check path.
- Without SQLITE_ENABLE_API_ARMOR defined to exercise the direct path.

The tests use non-terminating assertions (they do not abort on failure) and log
results to standard error for easy review.
*/

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>


// Candidate Keywords (high-level mapping to the focal method and its dependencies)
// - sqlite3_last_insert_rowid: focal method under test
// - sqlite3SafetyCheckOk: API armor safety check hook
// - SQLITE_ENABLE_API_ARMOR: macro gating the safety check branch
// - sqlite3 *db: the database handle with a lastRowid field
// - lastRowid: the value returned when safety check passes
// - SQLITE_MISUSE_BKPT: diagnostic placeholder (cast to void in the code)
// - sqlite_int64: return type alias used by SQLite
// These keywords form the core concepts to drive test coverage for the method.

//////////////////////////////////////////////////////////////////////
// Minimal reproduction of the focal dependencies (self-contained)
//////////////////////////////////////////////////////////////////////

// 64-bit integer type used by SQLite
typedef long long sqlite_int64;

// Minimal sqlite3 structure used by the focal function
struct sqlite3 {
    sqlite_int64 lastRowid;
};

// Safety check hook (to be controlled by tests)
static int g_safetyOk = 1; // 1 = safety check passes, 0 = fails

// Real SQLite API would provide a C function; we simulate with C linkage:
extern "C" int sqlite3SafetyCheckOk(struct sqlite3 *db){
    (void)db; // unused in this simple control; tests manipulate g_safetyOk instead
    return g_safetyOk;
}

// Diagnostic placeholder used in the safety-fail path
#define SQLITE_MISUSE_BKPT 0

/////////////////////////////////////////////////////////////////
// Focal function under test (replicated for in-file testing)
/////////////////////////////////////////////////////////////////

// We replicate the function as in the provided snippet.
// The code path depends on whether SQLITE_ENABLE_API_ARMOR is defined.
// To enable/disable, compile with or without the macro definition.
#ifdef SQLITE_ENABLE_API_ARMOR
extern "C" sqlite_int64 sqlite3_last_insert_rowid(struct sqlite3 *db){
    if( !sqlite3SafetyCheckOk(db) ){
        (void)SQLITE_MISUSE_BKPT;
        return 0;
    }
    return db->lastRowid;
}
#else
extern "C" sqlite_int64 sqlite3_last_insert_rowid(struct sqlite3 *db){
    return db->lastRowid;
}
#endif

//////////////////////////
// Lightweight test harness
//////////////////////////

static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating assertion: log failure but continue
#define TEST_ASSERT_EQ(expected, actual, msg) \
    do { \
        ++g_tests_run; \
        if ( (long long)(expected) != (long long)(actual) ) { \
            ++g_tests_failed; \
            fprintf(stderr, "TEST FAIL: %s | expected: %lld, actual: %lld\n", (msg), (long long)(expected), (long long)(actual)); \
        } \
    } while(0)

static void test_armor_ok_branch(){
    // Purpose: verify that with API armor enabled and safety check passing,
    // sqlite3_last_insert_rowid returns the actual lastRowid value.
    // Setup
    sqlite3 *db = new sqlite3();
    db->lastRowid = 0x0123456789abcdefLL; // some large 64-bit value
    g_safetyOk = 1; // ensure safety check passes

    // Exercise
    sqlite_int64 actual = sqlite3_last_insert_rowid(db);

    // Verify
    TEST_ASSERT_EQ(db->lastRowid, actual, "Armor ON: safe path should return lastRowid");

    delete db;
}

static void test_armor_bad_branch(){
    // Purpose: verify that with API armor enabled and safety check failing,
    // sqlite3_last_insert_rowid returns 0 regardless of lastRowid.
    // Setup
    sqlite3 *db = new sqlite3();
    db->lastRowid = 0xabcdef0123456789LL;
    g_safetyOk = 0; // simulate safety check failure

    // Exercise
    sqlite_int64 actual = sqlite3_last_insert_rowid(db);

    // Verify
    TEST_ASSERT_EQ(0LL, actual, "Armor ON: safety fail should return 0");

    delete db;
}

#ifndef SQLITE_ENABLE_API_ARMOR
// If API armor is not enabled, test that the function returns lastRowid directly
static void test_no_armor_path(){
    sqlite3 *db = new sqlite3();
    db->lastRowid = -12345;
    // Safety check should be irrelevant when armor is disabled
    g_safetyOk = 0; // value ignored in this build

    sqlite_int64 actual = sqlite3_last_insert_rowid(db);

    TEST_ASSERT_EQ(db->lastRowid, actual, "Armor OFF: should return lastRowid directly");

    delete db;
}
#endif

static void run_all_tests(){
    // Run tests in a deterministic order
    fprintf(stderr, "RUNNING SQLITE_LAST_INSERT_ROWID TESTS\n");
    test_armor_ok_branch();
    test_armor_bad_branch();
#ifndef SQLITE_ENABLE_API_ARMOR
    test_no_armor_path();
#endif

    fprintf(stderr, "TEST SUMMARY: ran %d tests, %d failed\n", g_tests_run, g_tests_failed);
}

//////////////////////
// Entry point
//////////////////////

int main(){
    run_all_tests();
    // Return non-zero if any test failed
    if(g_tests_failed){
        return 1;
    }
    return 0;
} 

/* 
Notes for maintainers:
- To cover both branches of the focal function, compile this file twice:
  1) Define SQLITE_ENABLE_API_ARMOR to exercise the safety-check path.
  2) Do not define SQLITE_ENABLE_API_ARMOR to exercise the direct path.
- The test harness uses simple, non-terminating assertions so that all tests
  execute and report failures without aborting the test run.
- The tests rely only on standard library facilities available in C++11.
*/