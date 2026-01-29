// Comprehensive unit tests for sqlite3PagerCloseWal using a self-contained test harness.
// This file provides a minimal, mock environment to exercise the focal function
// sqlite3PagerCloseWal(Pager*, sqlite3*) without requiring the full SQLite build.
// It is written in C++11 and uses a lightweight, custom test framework (no GTest).

#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <wal.h>
#include <cassert>


// Domain-specific simplifications and lightweight types to mimic SQLite environment.
typedef unsigned char u8;
typedef unsigned int u32;

#define SQLITE_OK 0
#define SQLITE_ERROR 1
#define SHARED_LOCK 0

// Pager mode constant (simplified)
#define PAGER_JOURNALMODE_WAL 1

// Forward declarations to allow the focal function to be defined in this file.
struct Pager;
struct sqlite3;

// Minimal Pager structure with only the fields used by sqlite3PagerCloseWal.
struct Pager {
  int journalMode;
  void *pWal;
  const char *zWal;
  void *pVfs;
  int walSyncFlags;
  u32 pageSize;
  void *pTmpSpace;
  int exclusiveMode;
};

// Lightweight sqlite3 struct placeholder.
struct sqlite3 {
  int dummy;
};

// Global state to drive mocked dependencies
static int g_rc_pagerLockDb = SQLITE_OK;
static int g_logexists = 0;
static int g_rc_sqlite3OsAccess = SQLITE_OK;
static int g_pagerOpenWal_should_succeed = 1;
static int g_rc_pagerExclusiveLock = SQLITE_OK;
static int g_rc_sqlite3WalClose = SQLITE_OK;
static int g_walClose_call_count = 0;
static int g_pagerUnlockDb_call_count = 0;

// Mocked dependency implementations (stubs) to support sqlite3PagerCloseWal behavior.
// They are deliberately simple and driven by the global state above.

static int pagerLockDb(Pager *pPager, int eLock) {
  // Return configured rc to simulate lock acquisition success/failure.
  (void)pPager; // unused in mock
  (void)eLock;
  return g_rc_pagerLockDb;
}

static int sqlite3OsAccess(void *pVfs, const char *zName, int flags, int *pExists) {
  (void)pVfs;
  (void)flags;
  if(pExists) *pExists = g_logexists;
  return g_rc_sqlite3OsAccess;
}

static int pagerOpenWal(Pager *pPager) {
  // When WAL is opened, simulate that pWal becomes non-NULL.
  if(pPager) {
    pPager->pWal = (void*)0xDEADBEEF; // non-NULL sentinel
  }
  return g_pagerOpenWal_should_succeed ? SQLITE_OK : SQLITE_ERROR;
}

static int pagerExclusiveLock(Pager *pPager) {
  (void)pPager;
  return g_rc_pagerExclusiveLock;
}

static int sqlite3WalClose(void *pWal, sqlite3 *db, int walSyncFlags, u32 pageSize, u8 *pTmp) {
  (void)pWal; (void)db; (void)walSyncFlags; (void)pageSize; (void)pTmp;
  ++g_walClose_call_count;
  return g_rc_sqlite3WalClose;
}

static void pagerFixMaplimit(Pager *pPager) {
  (void)pPager;
  // No-op in mock
}

static int pagerUnlockDb(Pager *pPager, int eLock) {
  (void)pPager; (void)eLock;
  ++g_pagerUnlockDb_call_count;
  return SQLITE_OK;
}

// The focal function under test, implemented here in this harness to enable controlled testing.
// Normally this would be linked from the real pager.c, but for unit testing isolation we re-define it here.
int sqlite3PagerCloseWal(Pager *pPager, sqlite3 *db){
  int rc = SQLITE_OK;
  assert( pPager->journalMode==PAGER_JOURNALMODE_WAL );
  /* If the log file is not already open, but does exist in the file-system,
  ** it may need to be checkpointed before the connection can switch to
  ** rollback mode. Open it now so this can happen.
  */
  if( !pPager->pWal ){
    int logexists = 0;
    rc = pagerLockDb(pPager, SHARED_LOCK);
    if( rc==SQLITE_OK ){
      rc = sqlite3OsAccess(
          pPager->pVfs, pPager->zWal, SQLITE_OK, &logexists
      );
    }
    if( rc==SQLITE_OK && logexists ){
      rc = pagerOpenWal(pPager);
    }
  }
   
  /* Checkpoint and close the log. Because an EXCLUSIVE lock is held on
  ** the database file, the log and log-summary files will be deleted.
  */
  if( rc==SQLITE_OK && pPager->pWal ){
    rc = pagerExclusiveLock(pPager);
    if( rc==SQLITE_OK ){
      rc = sqlite3WalClose(pPager->pWal, db, pPager->walSyncFlags,
                           pPager->pageSize, (u8*)pPager->pTmpSpace);
      pPager->pWal = 0;
      pagerFixMaplimit(pPager);
      if( rc && !pPager->exclusiveMode ) pagerUnlockDb(pPager, SHARED_LOCK);
    }
  }
  return rc;
}

// Lightweight test framework (no GTest). Simple PASS/FAIL reporting.

#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "TEST FAILED: " << msg << " (Line " << __LINE__ << ")\n"; \
  } else { \
    std::cerr << "TEST PASSED: " << msg << "\n"; \
  } \
} while(0)

static void reset_globals() {
  g_rc_pagerLockDb = SQLITE_OK;
  g_logexists = 0;
  g_rc_sqlite3OsAccess = SQLITE_OK;
  g_pagerOpenWal_should_succeed = 1;
  g_rc_pagerExclusiveLock = SQLITE_OK;
  g_rc_sqlite3WalClose = SQLITE_OK;
  g_walClose_call_count = 0;
  g_pagerUnlockDb_call_count = 0;
}

// Test 1: When WAL is not yet opened and the log exists, OpenWal is invoked and WAL is closed successfully.
void test_closeWal_opensWal_and_closes_ok() {
  reset_globals();

  Pager p;
  p.journalMode = PAGER_JOURNALMODE_WAL;
  p.pWal = nullptr;            // WAL not yet opened
  p.zWal = "testWal";
  p.pVfs = nullptr;
  p.walSyncFlags = 0;
  p.pageSize = 4096;
  p.pTmpSpace = nullptr;
  p.exclusiveMode = 0;

  // Drive mocks
  g_logexists = 1;                       // log exists
  g_pagerOpenWal_should_succeed = 1;     // OpenWal should succeed
  g_rc_pagerLockDb = SQLITE_OK;
  g_rc_sqlite3OsAccess = SQLITE_OK;
  g_rc_pagerExclusiveLock = SQLITE_OK;
  g_rc_sqlite3WalClose = SQLITE_OK;

  sqlite3 db;
  int rc = sqlite3PagerCloseWal(&p, &db);

  TEST_ASSERT(rc == SQLITE_OK, "CloseWal should return SQLITE_OK when WAL closed successfully");
  TEST_ASSERT(p.pWal == nullptr, "WAL should be NULL afterCloseWal (closed)");
  TEST_ASSERT(g_walClose_call_count == 1, "sqlite3WalClose should be invoked exactly once");
  TEST_ASSERT(g_pagerUnlockDb_call_count == 0, "pagerUnlockDb should not be called when close succeeds");
}

// Test 2: If ExclusiveLock fails, WAL closing should not proceed.
void test_closeWal_exclusiveLock_fails_no_wal_close() {
  reset_globals();

  Pager p;
  p.journalMode = PAGER_JOURNALMODE_WAL;
  p.pWal = (void*)0x1234;
  p.zWal = "testWal2";
  p.pVfs = nullptr;
  p.walSyncFlags = 0;
  p.pageSize = 4096;
  p.pTmpSpace = nullptr;
  p.exclusiveMode = 0;

  g_rc_pagerLockDb = SQLITE_OK;
  g_rc_pagerExclusiveLock = SQLITE_ERROR; // simulate failure
  g_rc_sqlite3WalClose = SQLITE_OK;

  sqlite3 db;
  int rc = sqlite3PagerCloseWal(&p, &db);

  TEST_ASSERT(rc == SQLITE_ERROR, "CloseWal should propagate rc from exclusive lock failure");
  TEST_ASSERT(p.pWal != nullptr, "WAL should remain non-NULL when exclusive lock fails");
  TEST_ASSERT(g_walClose_call_count == 0, "sqlite3WalClose should not be called when exclusive lock fails");
}

// Test 3: If WAL close returns error and exclusiveMode == 0, UnlockDb should be invoked.
void test_closeWal_walClose_error_triggers_unlock() {
  reset_globals();

  Pager p;
  p.journalMode = PAGER_JOURNALMODE_WAL;
  p.pWal = (void*)0x1234;
  p.zWal = "testWal3";
  p.pVfs = nullptr;
  p.walSyncFlags = 0;
  p.pageSize = 4096;
  p.pTmpSpace = nullptr;
  p.exclusiveMode = 0;

  g_rc_pagerLockDb = SQLITE_OK;
  g_rc_sqlite3OsAccess = SQLITE_OK;
  g_rc_pagerExclusiveLock = SQLITE_OK;
  g_rc_sqlite3WalClose = SQLITE_ERROR; // simulate WAL close failure
  g_pagerUnlockDb_call_count = 0;
  g_walClose_call_count = 0;

  sqlite3 db;
  int rc = sqlite3PagerCloseWal(&p, &db);

  TEST_ASSERT(rc == SQLITE_ERROR, "CloseWal should return error from sqlite3WalClose");
  TEST_ASSERT(g_walClose_call_count == 1, "sqlite3WalClose should be attempted once");
  TEST_ASSERT(g_pagerUnlockDb_call_count == 1, "pagerUnlockDb should be called on wal close error when not in exclusive mode");
  // The WAL should be set to NULL after rc path.
  TEST_ASSERT(p.pWal == nullptr, "WAL should be set to NULL after failed close attempt");
}

// Test 4: When log does not exist, WAL is not opened and nothing more happens.
void test_closeWal_logDoesNotExist_no_action() {
  reset_globals();

  Pager p;
  p.journalMode = PAGER_JOURNALMODE_WAL;
  p.pWal = nullptr;
  p.zWal = "testWalNoExist";
  p.pVfs = nullptr;
  p.walSyncFlags = 0;
  p.pageSize = 4096;
  p.pTmpSpace = nullptr;
  p.exclusiveMode = 0;

  g_logexists = 0; // log not present
  g_rc_pagerLockDb = SQLITE_OK;
  g_rc_sqlite3OsAccess = SQLITE_OK;

  sqlite3 db;
  int rc = sqlite3PagerCloseWal(&p, &db);

  TEST_ASSERT(rc == SQLITE_OK, "CloseWal should succeed when nothing to do");
  TEST_ASSERT(p.pWal == nullptr, "WAL remains NULL when there is nothing to close");
  TEST_ASSERT(g_walClose_call_count == 0, "sqlite3WalClose should not be called when there is no WAL");
}

// Test 5: When OpenWal fails after detecting existing log, CloseWal should propagate error and not call WalClose.
void test_closeWal_openWal_fails_no_walClose() {
  reset_globals();

  Pager p;
  p.journalMode = PAGER_JOURNALMODE_WAL;
  p.pWal = nullptr;
  p.zWal = "testWalOpenFail";
  p.pVfs = nullptr;
  p.walSyncFlags = 0;
  p.pageSize = 4096;
  p.pTmpSpace = nullptr;
  p.exclusiveMode = 0;

  g_logexists = 1;
  g_pagerOpenWal_should_succeed = 0; // OpenWal fails
  g_rc_pagerLockDb = SQLITE_OK;
  g_rc_sqlite3OsAccess = SQLITE_OK;

  sqlite3 db;
  int rc = sqlite3PagerCloseWal(&p, &db);

  TEST_ASSERT(rc == SQLITE_ERROR, "CloseWal should propagate error when OpenWal fails");
  TEST_ASSERT(p.pWal == nullptr, "WAL should remain NULL if OpenWal fails");
  TEST_ASSERT(g_walClose_call_count == 0, "sqlite3WalClose should not be called when OpenWal fails");
}

// Helper: run all tests
int main() {
  std::cout << "Starting sqlite3PagerCloseWal unit tests (self-contained harness)..." << std::endl;

  test_closeWal_opensWal_and_closes_ok();
  test_closeWal_exclusiveLock_fails_no_wal_close();
  test_closeWal_walClose_error_triggers_unlock();
  test_closeWal_logDoesNotExist_no_action();
  test_closeWal_openWal_fails_no_walClose();

  std::cout << "Unit tests completed." << std::endl;
  return 0;
}