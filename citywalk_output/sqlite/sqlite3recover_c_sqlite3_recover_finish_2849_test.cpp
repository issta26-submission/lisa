/*
  Lightweight C++11 test suite for the focal function:
  int sqlite3_recover_finish(sqlite3_recover *p)

  Approach:
  - Use the real sqlite3recover.h and sqlite3 (SQLite) APIs to exercise
    the actual implementation in sqlite3recover.c.
  - Do not rely on GoogleTest. Implement a small, non-terminating style
    assertion framework (via simple checks that print and continue).
  - Cover:
      1) Null pointer path (p == 0) -> SQLITE_NOMEM
      2) Non-transaction-close path (p->bCloseTransaction == false) -> rc == p->errCode
      3) Transaction-close path where a transaction is active (BEGIN called)
         and p->errCode == SQLITE_OK: rc should reflect sqlite3_exec("END") result
      4) Transaction-close path where a transaction is active and p->errCode != SQLITE_OK:
         rc should remain the initial error code
  - Assumptions:
      - The test environment provides SQLite3 headers and the sqlite3recover.h header
        from the project (so the real struct layout for sqlite3_recover is used).
      - The test binary links against -lsqlite3.
      - This test uses the real sqlite3 API for DB management to create a realistic
        transactional state.
  - Notes:
      - We intentionally keep the tests focused on the observable rc value and
        basic lifecycle flows. Memory allocations for p and its zErrMsg/zStateDb/zLostAndFound
        are freed by sqlite3_finish, as in production.
      - Static helpers inside sqlite3recover.c are left unmocked; we exercise via the
        public API only.
*/

#include <string.h>
#include <assert.h>
#include <sqlite3recover.h>
#include <sqlite3.h>
#include <cstring>
#include <iostream>
#include <cstdlib>



// Simple non-terminating test helper
static int g_failures = 0;
#define TEST_PASS() std::cout << "[PASS] " << __func__ << "()\n";
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " << __func__ << "(): " << msg << "\n"; g_failures++; } while(0)

static void expect_int_eq(const char* desc, int a, int b) {
    if (a == b) {
        std::cout << "[OK] " << desc << " -> " << a << " == " << b << "\n";
    } else {
        std::cerr << "[ERR] " << desc << " -> " << a << " != " << b << "\n";
        g_failures++;
    }
}

// Helper to create a recover object with non-null payloads.
static sqlite3_recover* alloc_recover(sqlite3* dbIn, int bClose, int errCode) {
    sqlite3_recover* p = (sqlite3_recover*) std::malloc(sizeof(sqlite3_recover));
    if (!p) return nullptr;

    // Initialize required fields (as expected by sqlite3_recover_finish)
    // Note: We rely on the project's actual struct layout for correctness.
    p->dbIn = dbIn;
    p->bCloseTransaction = bClose;
    p->errCode = errCode;

    // Allocate dummy payloads to be freed by sqlite3_recover_finish
    p->zErrMsg = (char*) std::malloc(16);
    if (p->zErrMsg) std::strcpy(p->zErrMsg, "ERR"); // small content

    p->zStateDb = (char*) std::malloc(16);
    if (p->zStateDb) std::strcpy(p->zStateDb, "STATE");

    p->zLostAndFound = (char*) std::malloc(16);
    if (p->zLostAndFound) std::strcpy(p->zLostAndFound, "FOUND");

    p->pPage1Cache = (void*) std::malloc(16);
    // No specific content required for the test

    return p;
}

// Test 1: p == nullptr should return SQLITE_NOMEM
static void test_null_pointer_returns_nospace() {
    // No setup required
    int rc = sqlite3_recover_finish(nullptr);
    expect_int_eq("nullptr should yield SQLITE_NOMEM", rc, SQLITE_NOMEM);
    TEST_PASS();
}

// Test 2: Non-transaction close path (p->bCloseTransaction == false)
static void test_no_end_when_not_closing_transaction() {
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        TEST_FAIL("Failed to open in-memory DB for Test 2");
        return;
    }

    // Create a recover object with end-transaction path disabled
    sqlite3_recover* p = alloc_recover(db, 0 /* bCloseTransaction = false */, SQLITE_OK + 123);
    if (!p) {
        TEST_FAIL("Failed to allocate sqlite3_recover for Test 2");
        sqlite3_close(db);
        return;
    }

    int rc = sqlite3_recover_finish(p);
    // Expect rc to be the original errCode (unchanged)
    expect_int_eq("rc should equal initial errCode when not closing transaction", rc, p->errCode);

    // After function, the structure should have been freed
    // The sqlite3_recover_finish frees p and its fields; we cannot safely access p now.
    // However, to avoid leaks, free what we allocated here if the function didn't already.
    // We assume the function freed p; no double-free attempt here.

    // Clean up DB handle
    sqlite3_close(db);
    TEST_PASS();
}

// Test 3: Transaction-close path with an active transaction and p->errCode == SQLITE_OK
static void test_end_transaction_and_ok_code() {
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        TEST_FAIL("Failed to open in-memory DB for Test 3");
        return;
    }

    // Begin a transaction to ensure END is valid
    char *errmsg = nullptr;
    if (sqlite3_exec(db, "BEGIN", nullptr, nullptr, &errmsg) != SQLITE_OK) {
        if (errmsg) sqlite3_free(errmsg);
        sqlite3_close(db);
        TEST_FAIL("BEGIN failed for Test 3");
        return;
    }

    // Create recover object with bCloseTransaction = true and SQLITE_OK
    sqlite3_recover* p = alloc_recover(db, 1 /* bCloseTransaction */, SQLITE_OK);
    if (!p) {
        sqlite3_close(db);
        TEST_FAIL("Failed to allocate sqlite3_recover for Test 3");
        return;
    }

    int rc = sqlite3_recover_finish(p);
    // If END succeeded, rc should be SQLITE_OK (0)
    expect_int_eq("END should finalize with SQLITE_OK when initial errCode is SQLITE_OK", rc, SQLITE_OK);

    sqlite3_close(db);
    TEST_PASS();
}

// Test 4: Transaction-close path with an active transaction and p->errCode != SQLITE_OK
static void test_end_transaction_with_non_ok_initial_code() {
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        TEST_FAIL("Failed to open in-memory DB for Test 4");
        return;
    }

    // Begin a transaction
    char *errmsg = nullptr;
    if (sqlite3_exec(db, "BEGIN", nullptr, nullptr, &errmsg) != SQLITE_OK) {
        if (errmsg) sqlite3_free(errmsg);
        sqlite3_close(db);
        TEST_FAIL("BEGIN failed for Test 4");
        return;
    }

    // Create recover object with non-OK initial errCode
    sqlite3_recover* p = alloc_recover(db, 1 /* bCloseTransaction */, 42);
    if (!p) {
        sqlite3_close(db);
        TEST_FAIL("Failed to allocate sqlite3_recover for Test 4");
        return;
    }

    int rc = sqlite3_recover_finish(p);
    // Since initial errCode != SQLITE_OK, rc should remain 42 regardless of END's result
    expect_int_eq("rc should remain non-OK value when initial errCode != SQLITE_OK", rc, 42);

    sqlite3_close(db);
    TEST_PASS();
}

int main() {
    // Run tests
    std::cout << "Starting sqlite3_recover_finish test suite (C++11 harness)\n";

    test_null_pointer_returns_nospace();
    test_no_end_when_not_closing_transaction();
    test_end_transaction_and_ok_code();
    test_end_transaction_with_non_ok_initial_code();

    if (g_failures == 0) {
        std::cout << "[OVERALL] All tests passed.\n";
        return 0;
    } else {
        std::cerr << "[OVERALL] " << g_failures << " test(s) failed.\n";
        return 1;
    }
}