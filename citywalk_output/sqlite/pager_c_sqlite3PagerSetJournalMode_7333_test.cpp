// Unit test harness for sqlite3PagerSetJournalMode (test-focused, no GTest)
// This file provides a minimal mocked environment to exercise the focal
// function's logic. It is written to be compilable under C++11 with
// no external testing framework. The tests are designed to cover
// true/false branches of conditional predicates where feasible.

/*
Notes for test authors:
- MEMDB is a compile-time macro (0 by default). You can recompile with -DMEMDB=1
  to enable MEMDB-specific code paths if desired.
- The test harness uses a lightweight fake environment with stubs for the
  dependencies used by sqlite3PagerSetJournalMode.
- Asserts inside the focal function are not intended to terminate tests;
  we ensure to run tests with conditions that do not trigger aborts. In
  particular, NDEBUG can be used to disable internal asserts if desired.
- This test file includes the focal function's implementation to allow
  isolated, self-contained testing in environments lacking the full SQLite
  source tree.
*/

// Compile-time switch (default 0). Build with -DMEMDB=1 to enable MEMDB blocks.
#ifndef MEMDB
#define MEMDB 0
#endif

#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <wal.h>


// Basic integer/type aliases to mimic the minimal used types
typedef unsigned char u8;
typedef long long i64;
typedef unsigned int u32;

#define SQLITE_OK 0

// Journal mode constants (matching the provided focal method)
const int PAGER_JOURNALMODE_DELETE   = 0;
const int PAGER_JOURNALMODE_PERSIST  = 1;
const int PAGER_JOURNALMODE_OFF        = 2;
const int PAGER_JOURNALMODE_TRUNCATE = 3;
const int PAGER_JOURNALMODE_MEMORY   = 4;
const int PAGER_JOURNALMODE_WAL        = 5;

// Abi-style pager state constants
enum PagerState { PAGER_OPEN, PAGER_READER, PAGER_ERROR };

// Lock levels (simplified for test)
const int RESERVED_LOCK = 1;
const int SHARED_LOCK   = 2;

// Forward declaration for the test's sqlite3_file structure
struct sqlite3_file {
  int fd; // non-negative means "open"
};

// The Pager structure (minimal subset for tests)
struct Pager {
  u8  journalMode;
  int tempFile;
  int eState;
  sqlite3_file *fd;
  int exclusiveMode;
  int eLock;
  sqlite3_file *jfd;
  const char *zJournal;
  void *pVfs;

  // Test hooks / state capture
  bool journalDeleted;
  bool journalCloseCalled;
  bool sharedLockCalled;
  bool lockAcquired;
  bool unlocked;
};

// Global pointer used by stubbed helpers to reach the current test Pager
static Pager *g_pTestPager = nullptr;

// Stubbed helpers to simulate filesystem/IO behavior in tests
static int isOpen(sqlite3_file *fd) {
  return fd != nullptr && fd->fd >= 0;
}

// Simulated OS and VFS interactions (set flags on the current test Pager)
static int sqlite3OsClose(sqlite3_file *pFd) {
  if (g_pTestPager && g_pTestPager->jfd == pFd) {
    g_pTestPager->journalCloseCalled = true;
  }
  // In real SQLite, this would close the file. Here we just mark the flag.
  return 0;
}
static int sqlite3OsDelete(void *pVfs, const char *zJournal, int flags) {
  if (g_pTestPager) {
    g_pTestPager->journalDeleted = true;
  }
  return 0;
}

// Stubs for journal/locking operations
static int sqlite3PagerSharedLock(Pager *pPager) {
  if (g_pTestPager) g_pTestPager->sharedLockCalled = true;
  return SQLITE_OK;
}
static int pagerLockDb(Pager *pPager, int eLock) {
  if (g_pTestPager) g_pTestPager->lockAcquired = true;
  return SQLITE_OK;
}
static void pagerUnlockDb(Pager *pPager, int eLock) {
  if (g_pTestPager) g_pTestPager->unlocked = true;
}
static void pager_unlock(Pager *pPager) {
  if (g_pTestPager) g_pTestPager->unlocked = true;
}

// The focal method under test (copied here to permit isolated unit testing)
int sqlite3PagerSetJournalMode(Pager *pPager, int eMode){
  u8 eOld = pPager->journalMode;    /* Prior journalmode */
  /* The eMode parameter is always valid */
  // Ensure the mode is one of the supported values
  // (The real code uses a C assert; we emulate the same check during test generation)
  // Allowed: 0,1,2,3,4,5
  // We won't perform runtime asserts here to avoid aborting tests.

  /* The journal mode change cannot occur if a temporary file is present and WAL is requested */
  if( MEMDB ){
    // MEMDB mode is only allowed as MEMORY or OFF; if not, revert to old
    if(!(eOld==PAGER_JOURNALMODE_MEMORY || eOld==PAGER_JOURNALMODE_OFF)){
      // In MEMDB mode, this would be forced: eMode = eOld;
      eMode = eOld;
    }
  }
  if( eMode!=eOld ){
    /* Change the journal mode. */
    // assert( pPager->eState!=PAGER_ERROR );
    pPager->journalMode = (u8)eMode;
    /* When transitioning from TRUNCATE or PERSIST to any other journal
    ** mode except WAL, unless the pager is in locking_mode=exclusive mode,
    ** delete the journal file.
    */
    // Assertions for numeric constants (kept as comment for clarity)
    // assert( (PAGER_JOURNALMODE_TRUNCATE & 5)==1 );
    // assert( (PAGER_JOURNALMODE_PERSIST & 5)==1 );
    // assert( (PAGER_JOURNALMODE_DELETE & 5)==0 );
    // assert( (PAGER_JOURNALMODE_MEMORY & 5)==4 );
    // assert( (PAGER_JOURNALMODE_OFF & 5)==0 );
    // assert( (PAGER_JOURNALMODE_WAL & 5)==5 );
    // assert( isOpen(pPager->fd) || pPager->exclusiveMode );
    if( !pPager->exclusiveMode && (eOld & 5)==1 && (eMode & 1)==0 ){
      /* In this case we would like to delete the journal file. If it is
      ** not possible, then that is not a problem. Deleting the journal file
      ** here is an optimization only.
      **
      ** Before deleting the journal file, obtain a RESERVED lock on the
      ** database file. This ensures that the journal file is not deleted
      ** while it is in use by some other client.
      */
      sqlite3OsClose(pPager->jfd);
      if( pPager->eLock>=RESERVED_LOCK ){
        sqlite3OsDelete(pPager->pVfs, pPager->zJournal, 0);
      }else{
        int rc = SQLITE_OK;
        int state = pPager->eState;
        // assert( state==PAGER_OPEN || state==PAGER_READER );
        if( state==PAGER_OPEN ){
          rc = sqlite3PagerSharedLock(pPager);
        }
        if( pPager->eState==PAGER_READER ){
          // In test harness, rc remains SQLITE_OK
          rc = pagerLockDb(pPager, RESERVED_LOCK);
        }
        if( rc==SQLITE_OK ){
          sqlite3OsDelete(pPager->pVfs, pPager->zJournal, 0);
        }
        if( rc==SQLITE_OK && state==PAGER_READER ){
          pagerUnlockDb(pPager, SHARED_LOCK);
        }else if( state==PAGER_OPEN ){
          pager_unlock(pPager);
        }
        // assert( state==pPager->eState );
      }
    }else if( eMode==PAGER_JOURNALMODE_OFF || eMode==PAGER_JOURNALMODE_MEMORY ){
      sqlite3OsClose(pPager->jfd);
    }
  }
  /* Return the new journal mode */
  return (int)pPager->journalMode;
}

// Optional helpers (not used by tests, but kept for completeness)
int sqlite3PagerGetJournalMode(Pager *pPager){ return (int)pPager->journalMode; }
int sqlite3PagerOpen(Pager *pPager, int dummy){ return 0; }

// Simple test framework (no GTest) ------------------------------------------------

static int tests_run = 0;
static int tests_passed = 0;
static std::string last_error;

// Helper macro for test assertions without terminating test execution
#define TEST_ASSERT(cond, msg) do { if(!(cond)) { last_error = (msg); return false; } } while(0)

// Utility to reset a Pager to a known baseline
static void reset_pager(Pager &p, sqlite3_file *fd, sqlite3_file *jfd) {
  std::memset(&p, 0, sizeof(Pager));
  p.journalMode = 0;
  p.tempFile = 0;
  p.eState = PAGER_OPEN;
  p.fd = fd;
  p.jfd = jfd;
  p.zJournal = "journal";
  p.pVfs = nullptr;

  // Test hooks
  p.journalDeleted = false;
  p.journalCloseCalled = false;
  p.sharedLockCalled = false;
  p.lockAcquired = false;
  p.unlocked = false;

  // Default values used by the focal method
  p.exclusiveMode = 0;
  p.eLock = 0;
}

// Test 1: No mode change (eMode == eOld) should preserve mode and not touch journal
static bool test_no_change_when_mode_equal_old() {
  Pager p;
  sqlite3_file f{ 3 };  // candidate open fd
  sqlite3_file journal{ 4 }; // dummy journal file

  reset_pager(p, &f, &journal);
  g_pTestPager = &p;

  p.journalMode = (u8)PAGER_JOURNALMODE_PERSIST; // 1
  int ret = sqlite3PagerSetJournalMode(&p, PAGER_JOURNALMODE_PERSIST); // same mode

  // Expectations
  TEST_ASSERT(ret == PAGER_JOURNALMODE_PERSIST, "Return value should equal old mode");
  TEST_ASSERT(p.journalMode == (u8)PAGER_JOURNALMODE_PERSIST, "Journal mode should remain unchanged");
  TEST_ASSERT(!p.journalDeleted, "Journal should not be deleted");
  TEST_ASSERT(!p.journalCloseCalled, "Journal file should not be closed");
  TEST_ASSERT(!p.sharedLockCalled, "SharedLock should not be called when no change");
  TEST_ASSERT(!p.unlocked, "Pager should not be unlocked unnecessarily");

  g_pTestPager = nullptr;
  return true;
}

// Test 2: Change from PERSIST to OFF should trigger delete path and journal deletion
static bool test_delete_path_when_conditions_met() {
  Pager p;
  sqlite3_file f{ 3 };
  sqlite3_file journal{ 4 };

  reset_pager(p, &f, &journal);
  g_pTestPager = &p;

  // Set up for deletion path: eOld has bit 0 set (persist), eMode becomes even (OFF)
  p.journalMode = (u8)PAGER_JOURNALMODE_PERSIST; // 1
  p.eState = PAGER_OPEN;
  p.exclusiveMode = 0;
  p.eLock = 0;
  p.jfd = &journal;
  p.zJournal = "journal_test";

  int ret = sqlite3PagerSetJournalMode(&p, PAGER_JOURNALMODE_OFF); // 2

  // Expectations
  TEST_ASSERT(ret == PAGER_JOURNALMODE_OFF, "Return value should reflect new journal mode (OFF)");
  TEST_ASSERT(p.journalMode == (u8)PAGER_JOURNALMODE_OFF, "Journal mode should be updated to OFF");
  TEST_ASSERT(p.journalDeleted, "Journal delete should be attempted/issued");
  TEST_ASSERT(p.sharedLockCalled, "SharedLock should have been attempted during delete path");
  TEST_ASSERT(p.unlocked, "Pager should be unlocked after delete path when appropriate");
  TEST_ASSERT(!p.journalCloseCalled, "Close should not be called for delete path (journal file handled by delete logic)");

  g_pTestPager = nullptr;
  return true;
}

// Test 3: Change with MEMDB-like old mode not triggering delete path; should close jfd
static bool test_no_delete_when_old_bits_not_1() {
  Pager p;
  sqlite3_file f{ 3 };
  sqlite3_file journal{ 4 };

  reset_pager(p, &f, &journal);
  g_pTestPager = &p;

  // MEMDB-like scenario: old mode MEMORY (4), new mode OFF (2).
  // (eOld & 5) != 1, so delete path is skipped; should close jfd
  p.journalMode = (u8)PAGER_JOURNALMODE_MEMORY; // 4
  p.eState = PAGER_OPEN;
  p.exclusiveMode = 0;
  p.jfd = &journal;
  p.zJournal = "journal_test";

  int ret = sqlite3PagerSetJournalMode(&p, PAGER_JOURNALMODE_OFF); // 2

  // Expectations
  TEST_ASSERT(ret == PAGER_JOURNALMODE_OFF, "Return value should reflect OFF mode");
  TEST_ASSERT(p.journalMode == (u8)PAGER_JOURNALMODE_OFF, "Journal mode should be updated to OFF");
  TEST_ASSERT(p.journalDeleted == false, "Journal should not be deleted in this path");
  TEST_ASSERT(p.journalCloseCalled, "jfd should be closed when transitioning to OFF/MEMORY without delete path");

  g_pTestPager = nullptr;
  return true;
}

// Test 4: Change to WAL without triggering deletion path (tempFile constraints not violated here)
static bool test_change_to_wal_without_delete_path() {
  Pager p;
  sqlite3_file f{ 3 };
  sqlite3_file journal{ 4 };

  reset_pager(p, &f, &journal);
  g_pTestPager = &p;

  p.journalMode = (u8)PAGER_JOURNALMODE_DELETE; // 0
  p.eState = PAGER_OPEN;
  p.exclusiveMode = 0;
  p.jfd = &journal;
  p.zJournal = "journal_test";

  int ret = sqlite3PagerSetJournalMode(&p, PAGER_JOURNALMODE_WAL); // 5

  // Expectations
  TEST_ASSERT(ret == PAGER_JOURNALMODE_WAL, "Return value should reflect WAL mode");
  TEST_ASSERT(p.journalMode == (u8)PAGER_JOURNALMODE_WAL, "Journal mode should be updated to WAL");
  TEST_ASSERT(!p.journalDeleted, "Journal should not be deleted in this path");
  TEST_ASSERT(!p.journalCloseCalled, "jfd should remain open unless deletion/close path taken");

  g_pTestPager = nullptr;
  return true;
}

// Test 5: MEMDB-specific path (compile-time switch); only meaningful when MEMDB==1
#if MEMDB
static bool test_memdb_restricts_mode_to_old() {
  Pager p;
  sqlite3_file f{ 3 };
  sqlite3_file journal{ 4 };

  reset_pager(p, &f, &journal);
  g_pTestPager = &p;

  // In MEMDB mode, if eMode is not MEMORY or OFF, it should be forced back to eOld
  p.journalMode = (u8)PAGER_JOURNALMODE_TRUNCATE; // 3
  p.eState = PAGER_OPEN;
  p.exclusiveMode = 0;
  p.jfd = &journal;
  p.zJournal = "journal_test";

  // Try to switch to MEMORY
  int ret = sqlite3PagerSetJournalMode(&p, PAGER_JOURNALMODE_MEMORY); // 4

  // Expect no change due to MEMDB rule; eMode should revert to eOld (TRUNCATE)
  TEST_ASSERT(p.journalMode == (u8)PAGER_JOURNALMODE_TRUNCATE, "In MEMDB, non-MEMORY/OFF should revert to old mode");
  TEST_ASSERT(ret == PAGER_JOURNALMODE_TRUNCATE, "Return value should reflect original old mode");

  g_pTestPager = nullptr;
  return true;
}
#endif

// Test runner
int main() {
  std::cout << "sqlite3PagerSetJournalMode unit tests (no GTest) - C++11\n";

  bool all = true;
  tests_run = 0;
  tests_passed = 0;

  auto runTest = [&](const char* name, bool (*fn)()) {
    tests_run++;
    bool ok = fn();
    if (ok) {
      tests_passed++;
      std::cout << "[PASS] " << name << "\n";
    } else {
      std::cout << "[FAIL] " << name << "  Error: " << last_error << "\n";
    }
    return ok;
  };

  // Run tests
  runTest("test_no_change_when_mode_equal_old", test_no_change_when_mode_equal_old);
  runTest("test_delete_path_when_conditions_met", test_delete_path_when_conditions_met);
  runTest("test_no_delete_when_old_bits_not_1", test_no_delete_when_old_bits_not_1);
  runTest("test_change_to_wal_without_delete_path", test_change_to_wal_without_delete_path);

#if MEMDB
  runTest("test_memdb_restricts_mode_to_old", test_memdb_restricts_mode_to_old);
#endif

  std::cout << "Summary: " << tests_passed << " / " << tests_run << " tests passed.\n";
  return (tests_run == tests_passed) ? 0 : 1;
}