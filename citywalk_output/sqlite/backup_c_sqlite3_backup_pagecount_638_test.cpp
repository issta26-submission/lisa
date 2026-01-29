/*
Step 1: Program Understanding (Candidate Keywords)
- sqlite3_backup: the focal type used by sqlite3_backup_pagecount
- nPagecount: internal counter exposed by sqlite3_backup
- sqlite3_backup_pagecount: the function under test
- NULL/nullptr: edge-case input for API armor path
- SQLITE_ENABLE_API_ARMOR and SQLITE_MISUSE_BKPT: compile-time guard and misuse breakpoint
- Internal dependencies: sqliteInt.h, btreeInt.h (struct layout and related internals)
- API armor branch: tests should cover both presence and absence of the NULL check under armor
- Domain constraints: purely standard C++11 test harness; no GTest; static helpers visible within the test file; use a lightweight non-terminating assertion model

This test suite focuses on the exact behavior of sqlite3_backup_pagecount:
- When p is non-null, returns p->nPagecount
- When p is null and API armor is enabled, returns 0 (and touches SQLITE_MISUSE_BKPT)

The tests assume access to the internal sqlite3_backup structure (via sqliteInt.h) to set nPagecount for deterministic verification.
*/

#include <cstdio>
#include <btreeInt.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>


// Import internal SQLite definitions to access sqlite3_backup and its fields.
// These headers are part of the SQLite source tree and are included here to enable
// direct construction of sqlite3_backup objects for unit testing.
extern "C" {

// Forward declaration (in case not provided via headers in this translation unit)
int sqlite3_backup_pagecount(sqlite3_backup *p);
}

// Lightweight non-terminating test harness
static int g_failures = 0;

static void log_fail(const std::string& test_name, const std::string& detail) {
    std::cerr << "[FAIL] " << test_name << ": " << detail << "\n";
    ++g_failures;
}

static void log_ok(const std::string& test_name) {
    std::cout << "[OK] " << test_name << "\n";
}

// Helper to compare integers without terminating the test run
static void check_int(const std::string& test_name, int expected, int actual) {
    if (expected != actual) {
        log_fail(test_name, "expected " + std::to_string(expected) +
                            ", got " + std::to_string(actual));
    } else {
        log_ok(test_name);
    }
}

// Test 1: Non-null pointer should return the pagecount stored in the object
static void test_backup_pagecount_non_null_returns_value() {
    // Arrange
    sqlite3_backup b;
    // Initialize the internal field that sqlite3_backup_pagecount reads
    // This relies on the internal definition of sqlite3_backup exposing nPagecount.
    b.nPagecount = 7;

    // Act
    int result = sqlite3_backup_pagecount(&b);

    // Assert
    check_int("backup_pagecount_non_null_returns_value", 7, result);
}

// Test 2 (optional under API armor): NULL pointer should return 0 when API armor is enabled
// This branch is guarded by SQLITE_ENABLE_API_ARMOR; if not defined, this test is skipped.
#ifdef SQLITE_ENABLE_API_ARMOR
static void test_backup_pagecount_null_with_armor() {
    // Arrange & Act
    int result = sqlite3_backup_pagecount(nullptr);

    // Assert
    check_int("backup_pagecount_null_with_armor", 0, result);
}
#endif

// Test 3: Non-null pointer with a different pagecount value (ensures general correctness)
static void test_backup_pagecount_various_values() {
    sqlite3_backup b;
    b.nPagecount = 0;
    int r0 = sqlite3_backup_pagecount(&b);
    check_int("backup_pagecount_zero_value", 0, r0);

    b.nPagecount = 12345;
    int r1 = sqlite3_backup_pagecount(&b);
    check_int("backup_pagecount_large_value", 12345, r1);
}

// Entry point for the test suite
int main() {
    std::cout << "Starting sqlite3_backup_pagecount unit tests (C++11, no GTest)\n";

    // Run tests
    test_backup_pagecount_non_null_returns_value();

#ifdef SQLITE_ENABLE_API_ARMOR
    test_backup_pagecount_null_with_armor();
#endif

    test_backup_pagecount_various_values();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}