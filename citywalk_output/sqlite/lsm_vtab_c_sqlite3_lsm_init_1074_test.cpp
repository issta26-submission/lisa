/*
  Unit test suite for the focal method:
  int sqlite3_lsm_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)

  Summary of approach:
  - Uses a real in-memory SQLite database to exercise the integration-setup path.
  - Verifies the normal path (return SQLITE_OK) and a null/non-null api path variant.
  - Verifies that calling the init twice on the same database does not silently succeed (second call should fail due to duplicate module name).
  - Uses a lightweight, non-terminating test harness (no GTest) suited for C++11.
  - Tests are written in C++11 but exercise a C API; linking against libsqlite3 is expected.

  Key {Candidate Keywords} extracted from the focal method and its dependencies:
  - sqlite3, sqlite3_lsm_init, sqlite3_create_module, lsm1Module
  - SQLITE_EXTENSION_INIT2, sqlite3_api_routines, pApi
  - db (sqlite3*), pzErrMsg, SQLITE_OK
  - Extension infrastructure and module registration
  - Non-terminating assertions and test harness narrative

  Notes:
  - This test suite assumes the project is linkable with the SQLite library (e.g., -lsqlite3).
  - The tests focus on observable behavior of sqlite3_lsm_init (return codes) rather than internal static state.
  - We avoid private/static access and rely on public integration points.
*/

#include <string.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <lsm.h>


// Include SQLite API header. Ensure this header is available in the include path.

// Candidate Keywords (for traceability in test design)
// - sqlite3, sqlite3_lsm_init, sqlite3_create_module, lsm1Module
// - SQLITE_EXTENSION_INIT2, sqlite3_api_routines, pApi
// - sqlite3_open, sqlite3_close, SQLITE_OK
// - pzErrMsg, db, pApi, in-memory database, module registration

// Lightweight test reporter (non-terminating; prints results and continues)
struct TestReporter {
    int passed = 0;
    int failed = 0;

    void report(bool condition, const char* testName, const char* message = "") {
        if (condition) {
            ++passed;
            std::cout << "[PASS] " << testName << "\n";
        } else {
            ++failed;
            std::cerr << "[FAIL] " << testName;
            if (message && message[0] != '\0') {
                std::cerr << " - " << message;
            }
            std::cerr << "\n";
        }
    }

    void summarize() const {
        std::cout << "Test Summary: " << passed << " passed, " << failed << " failed\n";
    }
};

// Test 1: Normal path - sqlite3_lsm_init should return SQLITE_OK when called with a valid in-memory db
void test_init_returns_ok(TestReporter& tr) {
    sqlite3* db = nullptr;
    int rc_open = sqlite3_open(":memory:", &db);
    if (rc_open != SQLITE_OK || db == nullptr) {
        tr.report(false, "test_init_returns_ok", "Failed to open in-memory sqlite3 db");
        if (db) sqlite3_close(db);
        return;
    }

    // pApi can be NULL or a dummy pointer; the init path should be robust to both.
    sqlite3_api_routines dummyApi{}; // zero-initialized; not strictly required to be filled
    int rc = sqlite3_lsm_init(db, nullptr, &dummyApi);
    bool ok = (rc == SQLITE_OK);

    tr.report(ok, "test_init_returns_ok", "sqlite3_lsm_init did not return SQLITE_OK on first init");

    sqlite3_close(db);
}

// Test 2: Init path with nullptr pApi - the macro should handle NULL pApi gracefully in practice
void test_init_with_null_pApi_returns_ok(TestReporter& tr) {
    sqlite3* db = nullptr;
    int rc_open = sqlite3_open(":memory:", &db);
    if (rc_open != SQLITE_OK || db == nullptr) {
        tr.report(false, "test_init_with_null_pApi_returns_ok", "Failed to open in-memory sqlite3 db");
        if (db) sqlite3_close(db);
        return;
    }

    // Pass NULL for pApi to simulate extension loading without API table.
    int rc = sqlite3_lsm_init(db, nullptr, nullptr);
    bool ok = (rc == SQLITE_OK);

    tr.report(ok, "test_init_with_null_pApi_returns_ok", "sqlite3_lsm_init did not return SQLITE_OK when pApi is NULL");

    sqlite3_close(db);
}

// Test 3: Calling init twice should not succeed silently the second time (module name collision)
void test_double_init_returns_error(TestReporter& tr) {
    sqlite3* db = nullptr;
    int rc_open = sqlite3_open(":memory:", &db);
    if (rc_open != SQLITE_OK || db == nullptr) {
        tr.report(false, "test_double_init_returns_error", "Failed to open in-memory sqlite3 db");
        if (db) sqlite3_close(db);
        return;
    }

    sqlite3_api_routines dummyApi{};
    int rc1 = sqlite3_lsm_init(db, nullptr, &dummyApi);
    bool firstOk = (rc1 == SQLITE_OK);

    int rc2 = sqlite3_lsm_init(db, nullptr, &dummyApi); // Second attempt on same db
    bool secondNotOk = (rc2 != SQLITE_OK);

    if (!firstOk) {
        tr.report(false, "test_double_init_returns_error", "First init failed unexpectedly");
    } else {
        tr.report(secondNotOk, "test_double_init_returns_error", "Second init unexpectedly succeeded (module name collision expected)");
    }

    sqlite3_close(db);
}

// Main entry: run all tests
int main() {
    TestReporter tr;

    // Execute tests
    test_init_returns_ok(tr);
    test_init_with_null_pApi_returns_ok(tr);
    test_double_init_returns_error(tr);

    // Summary
    tr.summarize();

    // Return non-zero if any test failed
    return (tr.failed == 0) ? 0 : 1;
}