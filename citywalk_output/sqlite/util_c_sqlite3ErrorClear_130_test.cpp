// Unit test suite for sqlite3ErrorClear in util.c
// This test suite is designed for a C++11 environment without using GTest.
// It uses lightweight hand-written test cases and a small test harness to
// verify the behavior of the focal function.
// Notes:
// - We provide minimal stubs for sqlite3 and sqlite3_value to exercise the code
//   path in sqlite3ErrorClear without requiring the full SQLite build.
// - The tests cover:
//     1) The case where db->pErr is NULL (pErr not touched; only header fields updated).
//     2) The case where db->pErr is non-NULL (sqlite3ValueSetNull is invoked on pErr).
// - The static/internal aspects of the real SQLite code are not exercised directly
//   here; this harness focuses on the observable effects of sqlite3ErrorClear.
//

#include <math.h>
#include <cstdio>
#include <cstdbool>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// -----------------------------------------------------------------------------
// Minimal SQLite-like type definitions and constants for the test harness
// -----------------------------------------------------------------------------

// SQLite's internal error code for "OK".
// In the real SQLite, SQLITE_OK is defined as 0 in sqlite3.h.
// We mirror that here for test purposes.
static const int SQLITE_OK = 0;

// Forward declare the minimal sqlite3 and sqlite3_value types to be used by
// sqlite3ErrorClear and sqlite3ValueSetNull. The actual util.c code will be
// compiled against a header that defines the real layout, so we must ensure
// the fields used by sqlite3ErrorClear exist and are accessible.
typedef struct sqlite3_value sqlite3_value;
typedef struct sqlite3 sqlite3;

// Minimal value type used by sqlite3ErrorClear to hold a NULL/not-NULL state.
struct sqlite3_value {
    bool isNull;
};

// Minimal database structure with only the fields used by sqlite3ErrorClear.
struct sqlite3 {
    int errCode;
    int errByteOffset;
    sqlite3_value *pErr;
};

// -----------------------------------------------------------------------------
// Prototypes for the focal function under test (from util.c) and its helper(s)
// -----------------------------------------------------------------------------

// The focal function under test. It is defined in util.c in the real project.
// We declare it here to allow calling it from the test harness.
extern "C" void sqlite3ErrorClear(sqlite3 *db);

// The helper function used by sqlite3ErrorClear to nullify the pErr value when non-null.
// We provide a test-specific implementation in this translation unit to enable
// deterministic testing without pulling in the entire SQLite codebase.
extern "C" void sqlite3ValueSetNull(sqlite3_value *p);

// -----------------------------------------------------------------------------
// Test hook: provide a test-specific implementation of sqlite3ValueSetNull
// It marks that the function was invoked and marks the sqlite3_value as NULL.
// This allows verification that sqlite3ErrorClear triggers the expected path
// when db->pErr is non-NULL.
// -----------------------------------------------------------------------------

// Global flag so tests can verify if sqlite3ValueSetNull was invoked.
bool valueSetNullCalled = false;

// C linkage to ensure symbol resolution matches sqlite3ValueSetNull used by sqlite3ErrorClear.
extern "C" void sqlite3ValueSetNull(sqlite3_value *p) {
    valueSetNullCalled = true;
    if (p != nullptr) {
        p->isNull = true;
    }
}

// -----------------------------------------------------------------------------
// Lightweight test cases for sqlite3ErrorClear
// -----------------------------------------------------------------------------

// Test 1: When db->pErr is NULL, sqlite3ErrorClear should set errCode to SQLITE_OK
// and errByteOffset to -1, and should not attempt to call sqlite3ValueSetNull.
bool test_sqlite3ErrorClear_withNullPErr() {
    // Arrange
    sqlite3 db;
    db.errCode = -123;       // arbitrary non-OK value to ensure change
    db.errByteOffset = 999;    // arbitrary non-default value to ensure change
    db.pErr = nullptr;         // pErr is NULL -> no call to sqlite3ValueSetNull

    valueSetNullCalled = false;

    // Act
    sqlite3ErrorClear(&db);

    // Assert
    bool passed = true;
    if (db.errCode != SQLITE_OK) {
        std::cerr << "[Test 1] FAIL: errCode expected " << SQLITE_OK
                  << " but got " << db.errCode << "\n";
        passed = false;
    }
    if (db.errByteOffset != -1) {
        std::cerr << "[Test 1] FAIL: errByteOffset expected -1 but got "
                  << db.errByteOffset << "\n";
        passed = false;
    }
    if (db.pErr != nullptr) {
        std::cerr << "[Test 1] FAIL: pErr expected to be nullptr but was non-null\n";
        passed = false;
    }
    if (valueSetNullCalled) {
        std::cerr << "[Test 1] FAIL: sqlite3ValueSetNull should not have been called\n";
        passed = false;
    }
    return passed;
}

// Test 2: When db->pErr is non-NULL, sqlite3ErrorClear should call sqlite3ValueSetNull
// and set pErr's isNull flag to true, in addition to updating errCode and errByteOffset.
bool test_sqlite3ErrorClear_withPErr() {
    // Arrange
    sqlite3_value v;
    v.isNull = false;

    sqlite3 db;
    db.errCode = 42;        // some non-OK initial value
    db.errByteOffset = 7;     // some non-default value
    db.pErr = &v;            // non-NULL pErr

    valueSetNullCalled = false;

    // Act
    sqlite3ErrorClear(&db);

    // Assert
    bool passed = true;
    if (db.errCode != SQLITE_OK) {
        std::cerr << "[Test 2] FAIL: errCode expected " << SQLITE_OK
                  << " but got " << db.errCode << "\n";
        passed = false;
    }
    if (db.errByteOffset != -1) {
        std::cerr << "[Test 2] FAIL: errByteOffset expected -1 but got "
                  << db.errByteOffset << "\n";
        passed = false;
    }
    if (db.pErr != &v) {
        std::cerr << "[Test 2] FAIL: pErr pointer changed unexpectedly\n";
        passed = false;
    }
    if (!v.isNull) {
        std::cerr << "[Test 2] FAIL: pErr->isNull expected to be true\n";
        passed = false;
    }
    if (!valueSetNullCalled) {
        std::cerr << "[Test 2] FAIL: sqlite3ValueSetNull should have been called\n";
        passed = false;
    }
    return passed;
}

// -----------------------------------------------------------------------------
// Test runner
// -----------------------------------------------------------------------------

int main() {
    int totalTests = 2;
    int passedTests = 0;

    std::cout << "Running sqlite3ErrorClear unit tests (minimal harness)..." << std::endl;

    if (test_sqlite3ErrorClear_withNullPErr()) {
        std::cout << "Test 1 (NULL pErr) PASSED" << std::endl;
        ++passedTests;
    } else {
        std::cout << "Test 1 (NULL pErr) FAILED" << std::endl;
    }

    if (test_sqlite3ErrorClear_withPErr()) {
        std::cout << "Test 2 (non-NULL pErr) PASSED" << std::endl;
        ++passedTests;
    } else {
        std::cout << "Test 2 (non-NULL pErr) FAILED" << std::endl;
    }

    std::cout << "Summary: " << passedTests << " / " << totalTests << " tests passed." << std::endl;

    return (passedTests == totalTests) ? 0 : 1;
}