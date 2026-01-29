/*
 * C++11 unit test suite for sqlite3LookasideUsed (status.c)
 * 
 * Notes:
 * - This test file integrates with the focal implementation by including the
 *   status.c source directly within the test translation unit. This ensures
 *   access to internal SQLite-like structures (sqlite3, LookasideSlot, etc.)
 *   and the static helper countLookasideSlots() that sqlite3LookasideUsed()
 *   relies upon.
 * - The tests exercise key branches and behavior of sqlite3LookasideUsed:
 *     a) when all lookaside pointers are NULL (nInit/nFree = 0, no TWOSIZE lookaside),
 *     b) the highwater value is written when pHighwater != NULL (for the NULL-pointer
 *        scenario we expect highwater == nSlot since nInit == 0).
 * - The test suite uses a lightweight, non-terminating assertion approach to
 *   maximize code coverage while avoiding external test frameworks (no GTest).
 * - To compare with the project’s actual behavior, the code relies on the real
 *   types and layout defined inside status.c (e.g., sqlite3, LookasideSlot, etc.).
 * - Static helpers and non-exported behavior are exercised through direct calls
 *   to sqlite3LookasideUsed in the same translation unit.
 * - Build note: compile this test file as a C++11 translation unit and ensure
 *   that the build links against the same status.c source so the symbol
 *   sqlite3LookasideUsed is resolved.
 */

#include <vector>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <status.c>
#include <iostream>
#include <cstdlib>


// Include the focal implementation so the test runs in the same translation unit.
// This exposes the sqlite3LookasideUsed function, its dependencies, and the
// necessary types (sqlite3, LookasideSlot, etc.) defined within status.c.
extern "C" {
}

// Simple non-terminating test harness
static int gFailures = 0;
static int gTestsRun = 0;

static void logPass(const std::string &msg) {
    std::cout << "[PASS] " << msg << std::endl;
}

static void logFail(const std::string &msg) {
    std::cerr << "[FAIL] " << msg << std::endl;
    ++gFailures;
}

static void expect(bool condition, const std::string &msg) {
    ++gTestsRun;
    if (condition) {
        logPass(msg);
    } else {
        logFail(msg);
    }
}

// Helper to run all tests; keep tests isolated by initializing a fresh sqlite3 db per test
static void test_all_null_pointers_returns_full_usage_and_highwater() {
    // Test: All lookaside pointers NULL; nSlot should be the returned value and highwater
    // (if pHighwater != NULL) should reflect nSlot as well.
    // Setup: db.lookaside.nSlot = some value; all pointers NULL.
    sqlite3 db;
    // Initialize the lookaside structure (fields come from status.c's sqlite3 definition)
    // The exact layout is defined in the included status.c; we initialize the fields
    // that are read by sqlite3LookasideUsed.
    db.lookaside.pInit = NULL;
    db.lookaside.pFree = NULL;
#ifndef SQLITE_OMIT_TWOSIZE_LOOKASIDE
    db.lookaside.pSmallInit = NULL;
    db.lookaside.pSmallFree = NULL;
#endif
    db.lookaside.nSlot = 7; // arbitrary non-zero value to exercise the formula

    int highwater = -1;
    int used = sqlite3LookasideUsed(&db, &highwater);

    // Expected behavior:
    // - nInit = countLookasideSlots(pInit) => 0 (NULL)
    // - nFree = countLookasideSlots(pFree) => 0 (NULL)
    // - if pHighwater != NULL, *pHighwater = nSlot - nInit => 7 - 0 = 7
    // - return value = nSlot - (nInit + nFree) => 7 - (0 + 0) = 7
    expect(used == 7, "sqlite3LookasideUsed returns nSlot when all lookaside pointers are NULL");
    expect(highwater == 7, "sqlite3LookasideUsed writes correct highwater when pHighwater is non-NULL and all pointers NULL");
}

static void test_all_null_pointers_with_null_highwater_pointer() {
    // Test: All lookaside pointers NULL and pHighwater == NULL
    sqlite3 db;
    db.lookaside.pInit = NULL;
    db.lookaside.pFree = NULL;
#ifndef SQLITE_OMIT_TWOSIZE_LOOKASIDE
    db.lookaside.pSmallInit = NULL;
    db.lookaside.pSmallFree = NULL;
#endif
    db.lookaside.nSlot = 5;

    // Call with pHighwater = NULL to ensure no write occurs
    int used = sqlite3LookasideUsed(&db, NULL);

    // Expect the return to equal nSlot with all counters zeroed
    // and no crash when pHighwater is NULL
    expect(used == 5, "sqlite3LookasideUsed returns nSlot with pHighwater = NULL and all pointers NULL");
}

int main() {
    std::cout << "Starting sqlite3LookasideUsed test suite..." << std::endl;

    test_all_null_pointers_returns_full_usage_and_highwater();
    test_all_null_pointers_with_null_highwater_pointer();

    // Summary
    std::cout << "Tests run: " << gTestsRun << ", Failures: " << gFailures << std::endl;

    // Return non-zero if any test failed
    return gFailures ? 1 : 0;
}