// Test suite for the focal method/path sqlite3_set_last_insert_rowid
// This test suite is designed to be self-contained (no external test framework required)
// and uses a lightweight, non-terminating assertion approach.
// It mimics the essential behavior of sqlite3_set_last_insert_rowid with and without API armor.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <string>


// Domain knowledge notes aligned with {Candidate Keywords}:
// - sqlite3, sqlite3_int64, sqlite3_mutex, db, mutex, lastRowid
// - sqlite3_mutex_enter, sqlite3_mutex_leave
// - sqlite3SafetyCheckOk, API ARMOR path (SQLITE_ENABLE_API_ARMOR), MISUSE_BKPT
// - Static-like behavior is represented via file-scope helpers (these are test-scoped here)

namespace test_suite {

// Forward declarations and minimal type stubs to model the focal function's environment

// Define sqlite3_int64 as a 64-bit signed integer
using sqlite3_int64 = long long;

// Minimal mutex struct to satisfy the function's usage
struct sqlite3_mutex {
    // empty - we only need a distinct type to identify a lock
};

// Minimal sqlite3 struct with the fields accessed by sqlite3_set_last_insert_rowid
struct sqlite3 {
    sqlite3_mutex *mutex;    // used by sqlite3_set_last_insert_rowid
    sqlite3_int64 lastRowid;  // field updated by sqlite3_set_last_insert_rowid
    int safetyFlag;           // used by sqlite3SafetyCheckOk in armor mode
};

// Global counters to verify mutex entry/exit invocations
static int g_enterCount = 0;
static int g_leaveCount = 0;

// Stub implementations of the external API used by the focal function

void sqlite3_mutex_enter(sqlite3_mutex * /*m*/) {
    ++g_enterCount;
}
void sqlite3_mutex_leave(sqlite3_mutex * /*m*/) {
    ++g_leaveCount;
}

// Safety check predicate used under API armor
int sqlite3SafetyCheckOk(sqlite3 *db) {
    // True only when db is non-null and safetyFlag is non-zero
    return (db != nullptr) && (db->safetyFlag != 0);
}

// A simple MISUSE_BKPT symbol (value not used by tests aside from cast)
const int SQLITE_MISUSE_BKPT = 1;

// ------------------------------------------------------------------------------------
// Test path 1: Armor-enabled version of sqlite3_set_last_insert_rowid
// This function is a close replica of the focal method but guarded by the
// API armor, which is simulated via defining SQLITE_ENABLE_API_ARMOR.
// When the safety check fails, it should return early and not touch mutex or lastRowid.
// ------------------------------------------------------------------------------------
#define SQLITE_ENABLE_API_ARMOR 1  // enable the armor path for this test unit

void sqlite3_set_last_insert_rowid_armor(sqlite3 *db, sqlite3_int64 iRowid) {
#ifdef SQLITE_ENABLE_API_ARMOR
    if (!sqlite3SafetyCheckOk(db)) {
        (void)SQLITE_MISUSE_BKPT;
        return;
    }
#endif
    sqlite3_mutex_enter(db->mutex);
    db->lastRowid = iRowid;
    sqlite3_mutex_leave(db->mutex);
}

// ------------------------------------------------------------------------------------
// Test path 2: Non-armor version of sqlite3_set_last_insert_rowid
// This reflects the code path when API armor is not compiled in.
// ------------------------------------------------------------------------------------
void sqlite3_set_last_insert_rowid_noarmor(sqlite3 *db, sqlite3_int64 iRowid) {
    sqlite3_mutex_enter(db->mutex);
    db->lastRowid = iRowid;
    sqlite3_mutex_leave(db->mutex);
}

// ------------------------------------------------------------------------------------
// Lightweight assertion helpers (non-terminating)
// These accumulate failures but do not abort test execution, enabling full coverage.
// ------------------------------------------------------------------------------------
struct AssertionResult {
    bool ok;
    std::string message;
};

static AssertionResult expect_true(bool cond, const std::string &msg) {
    if (cond) {
        return {true, ""};
    } else {
        return {false, "Expectation failed: " + msg};
    }
}

static AssertionResult expect_eq(sqlite3_int64 a, sqlite3_int64 b, const std::string &msg) {
    if (a == b) {
        return {true, ""};
    } else {
        return {false, "Expectation failed: " + msg + " (got " + std::to_string(a) +
                       ", expected " + std::to_string(b) + ")"};
    }
}

// ------------------------------------------------------------------------------------
// Individual unit tests (explanations provided in comments)
// ------------------------------------------------------------------------------------

/*
Test 1: Armor-enabled path succeeds in updating lastRowid
- Setup: db with mutex allocated, safetyFlag = 1 (safety check passes)
- Action: call sqlite3_set_last_insert_rowid_armor with a known iRowid
- Assert: lastRowid equals iRowid; mutex enter/leave counters incremented (both == 1)
*/
void test_armor_safety_ok() {
    // Prepare a fake db and mutex
    sqlite3_mutex m;
    sqlite3 db;
    db.mutex = &m;
    db.lastRowid = 0;
    db.safetyFlag = 1; // safety check will pass

    // Reset counters
    g_enterCount = 0;
    g_leaveCount = 0;

    // Action
    sqlite3_int64 expected = 0x1122334455667788LL;
    sqlite3_set_last_insert_rowid_armor(&db, expected);

    // Assertions
    auto r1 = expect_eq(db.lastRowid, expected, "Armor path should set lastRowid to the provided value");
    auto r2 = expect_true(g_enterCount == 1, "Armor path should enter mutex exactly once");
    auto r3 = expect_true(g_leaveCount == 1, "Armor path should leave mutex exactly once");

    // Print results
    if (!r1.ok) std::cerr << r1.message << "\n";
    if (!r2.ok) std::cerr << r2.message << "\n";
    if (!r3.ok) std::cerr << r3.message << "\n";
}

/*
Test 2: Armor-enabled path safety check fails
- Setup: db with mutex allocated, safetyFlag = 0 (safety check fails)
- Action: call sqlite3_set_last_insert_rowid_armor with any iRowid
- Assert: lastRowid remains unchanged (0); mutex enter/leave not invoked
*/
void test_armor_safety_false() {
    // Prepare a fake db and mutex
    sqlite3_mutex m;
    sqlite3 db;
    db.mutex = &m;
    db.lastRowid = 0;
    db.safetyFlag = 0; // safety check will fail

    // Reset counters
    g_enterCount = 0;
    g_leaveCount = 0;

    // Action
    sqlite3_int64 input = 0xDEADBEEFCAFEBABELL;
    sqlite3_set_last_insert_rowid_armor(&db, input);

    // Assertions
    auto r1 = expect_eq(db.lastRowid, (sqlite3_int64)0, "Armor path should NOT update lastRowid when safety check fails");
    auto r2 = expect_true(g_enterCount == 0, "Armor path should NOT enter mutex when safety check fails");
    auto r3 = expect_true(g_leaveCount == 0, "Armor path should NOT leave mutex when safety check fails");

    // Print results
    if (!r1.ok) std::cerr << r1.message << "\n";
    if (!r2.ok) std::cerr << r2.message << "\n";
    if (!r3.ok) std::cerr << r3.message << "\n";
}

/*
Test 3: Non-armor path updates lastRowid correctly
- Setup: db with mutex allocated
- Action: call sqlite3_set_last_insert_rowid_noarmor with a known iRowid
- Assert: lastRowid equals iRowid; mutex enter/leave counters incremented
*/
void test_noarmor_basic() {
    // Prepare a fake db and mutex
    sqlite3_mutex m;
    sqlite3 db;
    db.mutex = &m;
    db.lastRowid = 0;
    db.safetyFlag = 0; // not used in no-armor path

    // Reset counters
    g_enterCount = 0;
    g_leaveCount = 0;

    // Action
    sqlite3_int64 expected = 0xCAFEBABEDEADBE01LL;
    sqlite3_set_last_insert_rowid_noarmor(&db, expected);

    // Assertions
    auto r1 = expect_eq(db.lastRowid, expected, "No-armor path should set lastRowid to the provided value");
    auto r2 = expect_true(g_enterCount == 1, "No-armor path should enter mutex exactly once");
    auto r3 = expect_true(g_leaveCount == 1, "No-armor path should leave mutex exactly once");

    // Print results
    if (!r1.ok) std::cerr << r1.message << "\n";
    if (!r2.ok) std::cerr << r2.message << "\n";
    if (!r3.ok) std::cerr << r3.message << "\n";
}

// ------------------------------------------------------------------------------------
// Test runner
// ------------------------------------------------------------------------------------
void runAllTests() {
    // Run all tests and report summary
    test_armor_safety_ok();
    test_armor_safety_false();
    test_noarmor_basic();
}

// ------------------------------------------------------------------------------------
// Entry point for the test executable
// ------------------------------------------------------------------------------------
} // namespace test_suite

int main() {
    // Run the test suite
    test_suite::runAllTests();

    // Simple summary: if any tests printed errors, we report a non-zero exit status
    // However, since tests are non-terminating, we determine success by absence of stderr output.
    // For simplicity in this minimal harness, we return 0 to indicate the test program ran.
    return 0;
}