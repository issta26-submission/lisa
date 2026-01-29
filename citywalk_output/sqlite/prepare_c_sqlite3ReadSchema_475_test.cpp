/*
  Test Suite for sqlite3ReadSchema(Parse *pParse)

  Step 1 - Candidate Keywords extracted from the focal method:
  - sqlite3ReadSchema
  - Parse
  - sqlite3
  - sqlite3Init
  - sqlite3_mutex_held
  - db->init.busy
  - db->noSharedCache
  - db->mDbFlags
  - DBFLAG_SchemaKnownOk
  - pParse->zErrMsg, pParse->rc, pParse->nErr
  - SQLITE_OK
  - pParse->db

  Step 2/3 - The tests exercise the following code paths:
  - Path where db->init.busy is true (early exit, RC remains SQLITE_OK)
  - Path where db->init.busy is false and sqlite3Init returns SQLITE_OK
    - Sub-path where db->noSharedCache is true (flag DBFLAG_SchemaKnownOk is set)
    - Sub-path where db->noSharedCache is false (no change to mDbFlags)
  Note: This suite assumes a real SQLite environment is available and uses
  actual sqlite3/Parse structures. It uses a lightweight, non-terminating
  assertion approach suitable for C++11 without Google Test.
*/

#include <vector>
#include <memory>
#include <sstream>
#include <string>
#include <sqliteInt.h>
#include <iostream>


extern "C" {
  // Public API
  #include "sqlite3.h"
  // Internal headers are used to access the Parse structure and internal flag
  // definitions. If your environment places sqliteInt.h in a different path,
  // adjust include paths accordingly.
  #include "sqliteInt.h"
}

/* Lightweight test harness (non-terminating assertions)
   - Collect failures and report at the end.
*/
static std::vector<std::string> g_failures;

static void recordFailure(const std::string &msg) {
  g_failures.push_back(msg);
}

template <typename T, typename U>
static void expect_eq(const T& a, const U& b, const std::string& msg) {
  if (!(a == b)) {
    std::ostringstream oss;
    oss << msg << " | expected: " << a << " == " << b;
    g_failures.push_back(oss.str());
  }
}

static void printSummary() {
  if (g_failures.empty()) {
    std::cout << "All tests PASSED.\n";
  } else {
    std::cout << g_failures.size() << " test failures:\n";
    for (size_t i = 0; i < g_failures.size(); ++i) {
      std::cout << "  [" << i+1 << "] " << g_failures[i] << "\n";
    }
  }
}

/* Forward declaration of sqlite3ReadSchema from the focal module.
   We rely on the function signature from the real sqlite3ReadSchema. */
extern "C" int sqlite3ReadSchema(Parse *pParse);

/* Helper: run a single test and return whether it passed */
static bool runTest(const std::string &name) {
  if (!g_failures.empty()) {
    // Keep running tests to maximize coverage per run, as per non-terminating
    // assertions requirement. We still report at the end.
  }
  (void)name; // silence unused in case future enhancements are added
  return true;
}

/* Test 1: Busy path
   - Set db->init.busy = 1; sqlite3ReadSchema should short-circuit and return SQLITE_OK.
   - pParse.rc should remain unchanged from its pre-call value.
   - pParse.nErr should stay at 0.
*/
static void Test_ReadSchema_BusyPath() {
  // Open an in-memory database
  sqlite3 *db = nullptr;
  int rcOpen = sqlite3_open(":memory:", &db);
  if (rcOpen != SQLITE_OK || db == nullptr) {
    recordFailure("Test_ReadSchema_BusyPath: sqlite3_open failed");
    return;
  }

  // Prepare a Parse structure (as provided by sqlite3)
  Parse pParse;
  // Initialize parse fields used by sqlite3ReadSchema
  pParse.db = db;
  pParse.zErrMsg = nullptr;
  pParse.rc = 0xDEADBEEF; // sentinel to ensure no change happens
  pParse.nErr = 0;

  // Simulate "busy" state
  db->init.busy = 1;

  // Acquire the database mutex to satisfy the assertion in sqlite3ReadSchema
  sqlite3_mutex_enter(db->mutex);

  int rc = sqlite3ReadSchema(&pParse);

  sqlite3_mutex_leave(db->mutex);

  // Expected: rc == SQLITE_OK; pParse.rc remains 0xDEADBEEF; pParse.nErr == 0
  expect_eq(rc, SQLITE_OK, "Test_ReadSchema_BusyPath: rc should be SQLITE_OK");
  expect_eq(pParse.rc, (int)0xDEADBEEF, "Test_ReadSchema_BusyPath: pParse.rc should remain unchanged");
  expect_eq(pParse.nErr, 0, "Test_ReadSchema_BusyPath: pParse.nErr should remain 0");

  sqlite3_close(db);
}

/* Test 2: Not-busy path, Init returns OK, noSharedCache is true
   - After sqlite3ReadSchema, db->mDbFlags should include DBFLAG_SchemaKnownOk.
*/
static void Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheTrue() {
  sqlite3 *db = nullptr;
  int rcOpen = sqlite3_open(":memory:", &db);
  if (rcOpen != SQLITE_OK || db == nullptr) {
    recordFailure("Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheTrue: sqlite3_open failed");
    return;
  }

  Parse pParse;
  pParse.db = db;
  pParse.zErrMsg = nullptr;
  pParse.rc = 0;
  pParse.nErr = 0;

  // Ensure sane initial condition
  db->init.busy = 0;
  db->noSharedCache = 1;
  db->mDbFlags = 0;

  // Lock the mutex to satisfy the assertion in sqlite3ReadSchema
  sqlite3_mutex_enter(db->mutex);

  int rc = sqlite3ReadSchema(&pParse);

  sqlite3_mutex_leave(db->mutex);

  // Expected: rc == SQLITE_OK; pParse.rc == 0; pParse.nErr == 0
  expect_eq(rc, SQLITE_OK, "Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheTrue: rc should be SQLITE_OK");
  expect_eq(pParse.rc, 0, "Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheTrue: pParse.rc should remain 0");
  expect_eq(pParse.nErr, 0, "Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheTrue: pParse.nErr should remain 0");

  // Check that the flag was set
  bool flagSet = (db->mDbFlags & DBFLAG_SchemaKnownOk) != 0;
  expect_eq(flagSet ? 1 : 0, 1, "Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheTrue: DBFLAG_SchemaKnownOk should be set");

  sqlite3_close(db);
}

/* Test 3: Not-busy path, Init returns OK, noSharedCache is false
   - After sqlite3ReadSchema, db->mDbFlags should remain unchanged.
*/
static void Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheFalse() {
  sqlite3 *db = nullptr;
  int rcOpen = sqlite3_open(":memory:", &db);
  if (rcOpen != SQLITE_OK || db == nullptr) {
    recordFailure("Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheFalse: sqlite3_open failed");
    return;
  }

  Parse pParse;
  pParse.db = db;
  pParse.zErrMsg = nullptr;
  pParse.rc = 0;
  pParse.nErr = 0;

  db->init.busy = 0;
  db->noSharedCache = 0;
  db->mDbFlags = 0xABCD; // non-zero initial value to ensure no reset happens
  // Ensure the mutex is held during the call
  sqlite3_mutex_enter(db->mutex);

  int rc = sqlite3ReadSchema(&pParse);

  sqlite3_mutex_leave(db->mutex);

  // Expected: rc == SQLITE_OK; pParse.rc == 0; pParse.nErr == 0
  expect_eq(rc, SQLITE_OK, "Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheFalse: rc should be SQLITE_OK");
  expect_eq(pParse.rc, 0, "Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheFalse: pParse.rc should remain 0");
  expect_eq(pParse.nErr, 0, "Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheFalse: pParse.nErr should remain 0");

  // Flag should remain unchanged (or whatever the initial state was, since noSharedCache == false)
  bool flagSet = (db->mDbFlags & DBFLAG_SchemaKnownOk) != 0;
  // Expect no change due to Init path; verify that the flag is not forcibly set
  expect_eq(flagSet ? 1 : 0, 0, "Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheFalse: DBFLAG_SchemaKnownOk should not be set");

  sqlite3_close(db);
}

int main() {
  // Step 3: Run tests
  Test_ReadSchema_BusyPath();
  Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheTrue();
  Test_ReadSchema_BusyFalse_InitOk_NoSharedCacheFalse();

  printSummary();
  return g_failures.empty() ? 0 : 1;
}