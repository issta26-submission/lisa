// C++11 test suite for sqlite3Fts5StorageSync using lightweight custom test harness.
// This test does not rely on Google Test. It uses a small non-terminating assertion
// mechanism to run multiple tests in a single executable.
// The test uses fake/fourth-party style stubs for the external sqlite3-dependent
// functions to drive behavior of sqlite3Fts5StorageSync in a controlled way.
//
// Assumptions:
// - The real project provides the Fts5Storage, Fts5Config, and Fts5Index types via its headers.
// - We rely on those types being compatible with the actual implementation of sqlite3Fts5StorageSync.
// - We override a few external functions (sqlite3_last_insert_rowid, sqlite3_set_last_insert_rowid,
//   sqlite3Fts5IndexSync) with test doubles to control behavior.
// - We keep the logic minimal while covering key branches of sqlite3Fts5StorageSync.
//
// How to run (conceptually):
// g++ -std=c++11 -Isrc -I<path-to-sqlite-headers> test_fts5_storage_sync.cpp fts5_storage.c -o test_suite
// Then run ./test_suite
//
// Important: This file focuses on the test logic and should be compiled together with
// the real fts5_storage.c and its header dependencies in your project environment.

#include <vector>
#include <fts5Int.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Forward declare SINCE we are testing a C API under C linkage.
// We assume the project provides these types (Fts5Storage, Fts5Config, Fts5Index) via its headers.
// If your build environment has the real headers, include them here instead of re-declaring.
extern "C" {

// Forward declarations for SQLite-like types used by sqlite3Fts5StorageSync.
// We avoid pulling in the full sqlite3.h in this test harness to minimize coupling.
typedef struct sqlite3 sqlite3;
typedef long long i64;

// Minimal forward declarations of the essential types from the project.
typedef struct Fts5Config {
  sqlite3 *db;
} Fts5Config;

typedef struct Fts5Index {
  // Intentionally left minimal for test harness.
  int dummy;
} Fts5Index;

typedef struct Fts5Storage {
  Fts5Config *pConfig;
  Fts5Index *pIndex;
  int bTotalsValid;
} Fts5Storage;

// Prototype of the focal function under test from the project's C source.
int sqlite3Fts5StorageSync(Fts5Storage *p);

// Lightweight constants to represent SQLite return codes used by the focal method.
#define SQLITE_OK 0
#define SQLITE_ERROR 1

// Test harness state (accessed by test doubles).
static i64 g_last_insert_rowid_expected = 0;       // value returned by sqlite3_last_insert_rowid
static i64 g_last_insert_rowid_seen = 0;             // value passed to sqlite3_set_last_insert_rowid
static int g_indexSync_rc = SQLITE_OK;               // control return code of sqlite3Fts5IndexSync

// Test doubles (override external dependencies used by sqlite3Fts5StorageSync).

// Simulate sqlite3_last_insert_rowid(db) -> returns a deterministic value.
i64 sqlite3_last_insert_rowid(sqlite3 *db) {
  (void)db; // unused in test harness
  return g_last_insert_rowid_expected;
}

// Simulate sqlite3_set_last_insert_rowid(db, iRowid) -> capture the value passed in tests.
void sqlite3_set_last_insert_rowid(sqlite3 *db, i64 iRowid) {
  (void)db;
  g_last_insert_rowid_seen = iRowid;
  // No return value.
}

// Simulate sqlite3Fts5IndexSync(pIndex) -> controlled by g_indexSync_rc
int sqlite3Fts5IndexSync(Fts5Index *pIndex) {
  (void)pIndex;
  return g_indexSync_rc;
}

} // extern "C"

///////////////////////
// Lightweight Test Framework
///////////////////////

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) << "\n"; \
    ++g_failed_tests; \
  } \
  ++g_total_tests; \
} while(0)

#define TEST_PASS(msg) do { \
  std::cout << "[PASS] " << __FUNCTION__ << ": " << (msg) << "\n"; \
} while(0)

///////////////////////
// Unit Tests for sqlite3Fts5StorageSync
///////////////////////

/*
Test 1: TotalsValid is true and IndexSync succeeds.
- Setup:
  - p->pConfig->db is non-null (stubbed through dummy pointer)
  - p->pIndex is non-null
  - p->bTotalsValid = 1
  - g_last_insert_rowid_expected = 99 (value returned by sqlite3_last_insert_rowid)
  - g_indexSync_rc = SQLITE_OK
- Expectations:
  - sqlite3Fts5StorageSync returns SQLITE_OK
  - p->bTotalsValid becomes 0
  - sqlite3_set_last_insert_rowid called with 99
*/
void test_Sync_TotalsValid_True_IndexSync_OK() {
  // Allocate and set up storage structures (use real types from the project in your build)
  Fts5Config *pConfig = new Fts5Config();
  // db pointer can be any non-null to simulate a real DB handle; not used by test doubles
  pConfig->db = reinterpret_cast<sqlite3*>(0x1); // dummy non-null
  Fts5Index *pIndex = new Fts5Index();

  Fts5Storage *p = new Fts5Storage();
  p->pConfig = pConfig;
  p->pIndex = pIndex;
  p->bTotalsValid = 1;

  // Preconditions for test
  g_last_insert_rowid_expected = 99;
  g_last_insert_rowid_seen = -1;
  g_indexSync_rc = SQLITE_OK;

  int rc = sqlite3Fts5StorageSync(p);

  TEST_ASSERT(rc == SQLITE_OK, "rc should be SQLITE_OK when index sync succeeds");
  TEST_ASSERT(p->bTotalsValid == 0, "bTotalsValid should be cleared to 0 after successful SaveTotals");
  TEST_ASSERT(g_last_insert_rowid_seen == 99, "set_last_insert_rowid should be called with iLastRowid (99)");

  delete p;
  delete pIndex;
  delete pConfig;

  TEST_PASS("TotTotalsValid True, IndexSync OK path covered.");
}

/*
Test 2: TotalsValid is false (skip SaveTotals) and IndexSync succeeds.
- Setup:
  - p->bTotalsValid = 0
  - g_indexSync_rc = SQLITE_OK
- Expectations:
  - rc == SQLITE_OK
  - No SaveTotals path invoked; bTotalsValid remains 0
  - LastInsertRowid still set to the value returned by sqlite3_last_insert_rowid
*/
void test_Sync_TotalsValid_False_IndexSync_OK() {
  Fts5Config *pConfig = new Fts5Config();
  pConfig->db = reinterpret_cast<sqlite3*>(0x2);
  Fts5Index *pIndex = new Fts5Index();

  Fts5Storage *p = new Fts5Storage();
  p->pConfig = pConfig;
  p->pIndex = pIndex;
  p->bTotalsValid = 0;

  g_last_insert_rowid_expected = 7;
  g_last_insert_rowid_seen = -1;
  g_indexSync_rc = SQLITE_OK;

  int rc = sqlite3Fts5StorageSync(p);

  TEST_ASSERT(rc == SQLITE_OK, "rc should be SQLITE_OK when index sync succeeds without totals");
  TEST_ASSERT(p->bTotalsValid == 0, "bTotalsValid should remain 0 when it was already false");
  TEST_ASSERT(g_last_insert_rowid_seen == 7, "set_last_insert_rowid should be called with iLastRowid (7)");

  delete p;
  delete pIndex;
  delete pConfig;

  TEST_PASS("TotalsValid False, IndexSync OK path covered.");
}

/*
Test 3: TotalsValid is true and IndexSync fails.
- Setup:
  - p->bTotalsValid = 1
  - g_indexSync_rc = SQLITE_ERROR
- Expectations:
  - rc != SQLITE_OK (non-zero)
  - bTotalsValid should be cleared to 0 if SaveTotals returned SQLITE_OK
  - LastInsertRowid should still be set (value from sqlite3_last_insert_rowid)
  Note: We assume SaveTotals returns SQLITE_OK in the real code path.
*/
void test_Sync_TotalsValid_True_IndexSync_Fail() {
  Fts5Config *pConfig = new Fts5Config();
  pConfig->db = reinterpret_cast<sqlite3*>(0x3);
  Fts5Index *pIndex = new Fts5Index();

  Fts5Storage *p = new Fts5Storage();
  p->pConfig = pConfig;
  p->pIndex = pIndex;
  p->bTotalsValid = 1;

  g_last_insert_rowid_expected = 1234;
  g_last_insert_rowid_seen = -1;
  g_indexSync_rc = SQLITE_ERROR;

  int rc = sqlite3Fts5StorageSync(p);

  // Even if index sync failed, rc should reflect the failure
  TEST_ASSERT(rc != SQLITE_OK, "rc should indicate failure when index sync fails");
  // We expect SaveTotals to have succeeded (in real code) and cleared totals valid flag
  TEST_ASSERT(p->bTotalsValid == 0 || p->bTotalsValid == 0, "bTotalsValid should be 0 after attempted save (best-effort check)");
  TEST_ASSERT(g_last_insert_rowid_seen == 1234, "set_last_insert_rowid should be called with iLastRowid (1234) even on error path");

  delete p;
  delete pIndex;
  delete pConfig;

  TEST_PASS("TotalsValid True, IndexSync Fail path covered.");
}

///////////////////////
// Main entrypoint to run tests
///////////////////////
int main() {
  std::cout << "Starting sqlite3Fts5StorageSync tests...\n";

  // Run test cases
  test_Sync_TotalsValid_True_IndexSync_OK();
  test_Sync_TotalsValid_False_IndexSync_OK();
  test_Sync_TotalsValid_True_IndexSync_Fail();

  // Summary
  std::cout << "\nTest run complete. "
            << g_total_tests << " tests executed, "
            << g_failed_tests << " failures.\n";

  // Return non-zero if any test failed
  return (g_failed_tests > 0) ? 1 : 0;
}