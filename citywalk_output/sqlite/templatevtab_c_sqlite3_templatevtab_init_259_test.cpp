#include <string.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <iostream>


// Lightweight unit test harness for sqlite3_templatevtab_init
// Note: This test suite expects the real templatevtab.c (or equivalent) to be
// compiled and linked with the test binary, along with the sqlite3 library.
// The tests use real sqlite3 database handles to exercise the initialization path.

/*
Test 1: test_sqlite3_templatevtab_init_first_call_ok
- Objective: Verify that the first invocation of sqlite3_templatevtab_init on a fresh in-memory database
  returns SQLITE_OK and registers the templatevtab module successfully.
- Rationale: sqlite3_templatevtab_init is expected to create and register a module named "templatevtab".
  On a clean database, this should succeed.
- Approach: Open an in-memory database, call sqlite3_templatevtab_init with a nullptr pApi, and
  assert that the return code is SQLITE_OK. Clean up resources afterwards.
*/
int test_sqlite3_templatevtab_init_first_call_ok() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "[TEST_FAIL] Failed to open in-memory database. rc=" << rc << "\n";
        if (db) sqlite3_close(db);
        return 1; // test driver error
    }

    char* pzErrMsg = nullptr;
    // Call the focal method with null API pointer (typical for unit testing extension init)
    rc = sqlite3_templatevtab_init(db, &pzErrMsg, nullptr);

    if (pzErrMsg) {
        sqlite3_free(pzErrMsg);
        pzErrMsg = nullptr;
    }

    sqlite3_close(db);

    // Expect success on first initialization
    if (rc != SQLITE_OK) {
        std::cerr <<("[TEST_FAIL] sqlite3_templatevtab_init returned non-OK on first call. rc=" << rc << "\n");
        return 2;
    }

    return 0; // test passed
}

/*
Test 2: test_sqlite3_templatevtab_init_second_call_errors
- Objective: Verify that a second invocation of sqlite3_templatevtab_init on the same database returns an error.
- Rationale: The module named "templatevtab" should already exist after the first call; attempting to
  register the same module again should fail (non-OK rc).
- Approach: Open in-memory database, call sqlite3_templatevtab_init once (expect SQLITE_OK), then call again
  and expect a non-OK return code. Clean up resources afterwards.
*/
int test_sqlite3_templatevtab_init_second_call_errors() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "[TEST_FAIL] Failed to open in-memory database. rc=" << rc << "\n";
        if (db) sqlite3_close(db);
        return 1; // test driver error
    }

    char* pzErrMsg = nullptr;

    // First initialization should succeed
    rc = sqlite3_templatevtab_init(db, &pzErrMsg, nullptr);
    if (pzErrMsg) {
        sqlite3_free(pzErrMsg);
        pzErrMsg = nullptr;
    }
    if (rc != SQLITE_OK) {
        std::cerr << "[TEST_FAIL] sqlite3_templatevtab_init failed on first call. rc=" << rc << "\n";
        sqlite3_close(db);
        return 2;
    }

    // Second initialization should fail (module already exists)
    rc = sqlite3_templatevtab_init(db, &pzErrMsg, nullptr);
    if (pzErrMsg) {
        sqlite3_free(pzErrMsg);
        pzErrMsg = nullptr;
    }
    sqlite3_close(db);

    // Expect non-OK on second attempt
    if (rc == SQLITE_OK) {
        std::cerr << "[TEST_FAIL] sqlite3_templatevtab_init unexpectedly succeeded on second call.\n";
        return 3;
    }

    return 0; // test passed
}

int main() {
    std::cout << "Running unit tests for sqlite3_templatevtab_init...\n";

    int total = 2;
    int passed = 0;

    if (test_sqlite3_templatevtab_init_first_call_ok() == 0) {
        std::cout << "[PASS] test_sqlite3_templatevtab_init_first_call_ok\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_sqlite3_templatevtab_init_first_call_ok\n";
    }

    if (test_sqlite3_templatevtab_init_second_call_errors() == 0) {
        std::cout << "[PASS] test_sqlite3_templatevtab_init_second_call_errors\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_sqlite3_templatevtab_init_second_call_errors\n";
    }

    std::cout << "Tests passed: " << passed << " / " << total << "\n";
    return (passed == total) ? 0 : 1;
}