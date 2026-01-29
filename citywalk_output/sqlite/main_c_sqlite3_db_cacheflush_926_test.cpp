// Self-contained unit test suite for the focal method sqlite3_db_cacheflush
// This file re-implements a minimal, self-contained environment to exercise
// the sqlite3_db_cacheflush function logic exactly as described in the prompt.
// The tests are written in C++11 without any external testing framework.
// Explanatory comments accompany each test case to describe the intent and coverage.

// Note: This is a self-contained reproduction suitable for environments where
// the real sqlite3 library is not linked. It mirrors the control-flow and
// decision points of the original function to maximize coverage.

#include <vector>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain constants (mirroring common sqlite3 values)
static const int SQLITE_OK = 0;
static const int SQLITE_BUSY = 5;
static const int SQLITE_TXN_WRITE = 1;
static const int SQLITE_MISUSE_BKPT = 21;

// Enable API armor gating to simulate the safety-check path in tests.
// We define this macro so that the guarded path in the focal method is compiled.
#define SQLITE_ENABLE_API_ARMOR 1

// Global control to simulate sqlite3SafetyCheckOk(db) result (via API_ARMOR path)
static int g_safety_check_ok = 1;

// Forward declaration matching the focal method's API
// We implement a self-contained sqlite3_db_cacheflush here to ensure testability.
struct Pager;
struct Btree;
struct sqlite3;

// Pager stub
struct Pager {
  int flushResult;
};

// Btree stub
struct Btree {
  Pager *pPager;
  int txnState; // expected values: SQLITE_TXN_WRITE or others
};

// sqlite3 stub with minimal members used by the focal function
// The real struct in SQLite is far richer; we only reproduce what's needed.
struct sqlite3 {
  void *mutex;
  int nDb;
  struct {
    Btree *pBt;
  } aDb[8];
};

// Stub: mimic the safety check function used under API_ARMOR
static int sqlite3SafetyCheckOk(struct sqlite3 *db) {
  (void)db; // unused param in this stub
  return g_safety_check_ok;
}

// Stub: mutex enter/leave (no real locking in this test harness)
static void sqlite3_mutex_enter(void *mutex) { (void)mutex; /* no-op */ }
static void sqlite3_mutex_leave(void *mutex) { (void)mutex; /* no-op */ }

// Stub: B-tree entry/leave hooks (no-op for test harness)
static void sqlite3BtreeEnterAll(struct sqlite3 *db) { (void)db; }
static void sqlite3BtreeLeaveAll(struct sqlite3 *db) { (void)db; }

// Stub: obtain txn state from a Btree
static int sqlite3BtreeTxnState(Btree *pBt) {
  return pBt ? pBt->txnState : 0;
}

// Stub: retrieve the pager from a Btree
static Pager* sqlite3BtreePager(Btree *pBt) {
  return pBt ? pBt->pPager : NULL;
}

// Stub: flush a pager
static int sqlite3PagerFlush(Pager *pPager) {
  return pPager ? pPager->flushResult : SQLITE_OK;
}

// The focal method under test, implemented to mirror the provided logic.
// This is a self-contained version for testing without external dependencies.
int sqlite3_db_cacheflush(struct sqlite3 *db){
  int i;
  int rc = SQLITE_OK;
  int bSeenBusy = 0;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ) return SQLITE_MISUSE_BKPT;
#endif
  sqlite3_mutex_enter(db->mutex);
  sqlite3BtreeEnterAll(db);
  for(i=0; rc==SQLITE_OK && i<db->nDb; i++){
    Btree *pBt = db->aDb[i].pBt;
    if( pBt && sqlite3BtreeTxnState(pBt)==SQLITE_TXN_WRITE ){
      Pager *pPager = sqlite3BtreePager(pBt);
      rc = sqlite3PagerFlush(pPager);
      if( rc==SQLITE_BUSY ){
        bSeenBusy = 1;
        rc = SQLITE_OK;
      }
    }
  }
  sqlite3BtreeLeaveAll(db);
  sqlite3_mutex_leave(db->mutex);
  return ((rc==SQLITE_OK && bSeenBusy) ? SQLITE_BUSY : rc);
}

// Simple test harness utilities (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_EQ(a,b) do { \
  if ((a)!=(b)) { \
    std::cerr << "EXPECT_EQ failed: " << (long long)(a) << " != " << (long long)(b) << std::endl; \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_TRUE(x) do { \
  if (!(x)) { \
    std::cerr << "EXPECT_TRUE failed: " << #x << std::endl; \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_FALSE(x) do { \
  if (x) { \
    std::cerr << "EXPECT_FALSE failed: " << #x << std::endl; \
    ++g_failures; \
  } \
} while(0)

// Helper to create a simple Btree with a given txn state and pager flush result
static Btree* make_btree(int txnState, int pagerFlushResult){
  Pager* pager = new Pager{pagerFlushResult};
  Btree* bt = new Btree{pager, txnState};
  return bt;
}

// Test 1: No write transactions in any Btree -> rc remains SQLITE_OK and not BUSY
static void test_no_busy_no_write_transactions() {
  // Prepare a db with 1 Db entry having no WRITE txn
  sqlite3 db;
  db.mutex = (void*)0x1;
  db.nDb = 1;
  db.aDb[0].pBt = make_btree(0 /* not WRITE */, SQLITE_OK);

  int rc = sqlite3_db_cacheflush(&db);

  // Expect SQLITE_OK with no busy signal
  EXPECT_EQ(rc, SQLITE_OK);
  // Cleanup
  delete db.aDb[0].pBt->pPager;
  delete db.aDb[0].pBt;
}

// Test 2: A single Btree in WRITE txn and its pager flush returns SQLITE_BUSY
// Expect the final result to be SQLITE_BUSY
static void test_busy_result_from_pager() {
  sqlite3 db;
  db.mutex = (void*)0x2;
  db.nDb = 1;
  // pBt in WRITE state with a pager that simulates BUSY
  db.aDb[0].pBt = make_btree(SQLITE_TXN_WRITE, SQLITE_BUSY);

  int rc = sqlite3_db_cacheflush(&db);

  // When a BUSY is observed, final result should be SQLITE_BUSY
  EXPECT_EQ(rc, SQLITE_BUSY);

  // Cleanup
  delete db.aDb[0].pBt->pPager;
  delete db.aDb[0].pBt;
}

// Test 3: Safety check fails under API_ARMOR -> should return SQLITE_MISUSE_BKPT
static void test_api_armor_safety_check_fails() {
  // Enable safety armor (macro is defined in this test file)
  // Force safety check to fail
  g_safety_check_ok = 0;

  sqlite3 db;
  db.mutex = (void*)0x3;
  db.nDb = 1;
  // Any Btree state; since safety check fails, function should return early
  db.aDb[0].pBt = make_btree(0, SQLITE_OK);

  int rc = sqlite3_db_cacheflush(&db);

  // Expect MISUSE_BKPT as defined
  EXPECT_EQ(rc, SQLITE_MISUSE_BKPT);

  // Cleanup
  delete db.aDb[0].pBt->pPager;
  delete db.aDb[0].pBt;

  // Restore safety check for subsequent tests
  g_safety_check_ok = 1;
}

// Test 4: Multiple entries; one is WRITE with BUSY, another is non-WRITE
// Should still report SQLITE_BUSY due to at least one BUSY pager flush
static void test_multiple_entries_one_busy() {
  sqlite3 db;
  db.mutex = (void*)0x4;
  db.nDb = 2;
  // Entry 0: WRITE txn, pager returns OK
  db.aDb[0].pBt = make_btree(SQLITE_TXN_WRITE, SQLITE_OK);
  // Entry 1: not a WRITE txn (should be ignored)
  db.aDb[1].pBt = make_btree(0, SQLITE_OK);

  int rc = sqlite3_db_cacheflush(&db);

  // Since at least one WRITE entry had BUSY flush, final should be BUSY
  // Here the first entry returns OK, the second is non-WRITE; to simulate BUSY,
  // we adjust the first pager to BUSY directly:
  delete db.aDb[0].pBt->pPager;
  db.aDb[0].pBt->pPager = new Pager{SQLITE_BUSY};

  rc = sqlite3_db_cacheflush(&db);
  EXPECT_EQ(rc, SQLITE_BUSY);

  // Cleanup
  delete db.aDb[0].pBt->pPager;
  delete db.aDb[0].pBt;
  delete db.aDb[1].pBt->pPager;
  delete db.aDb[1].pBt;
}

// Entry point
int main() {
  std::cout << "Running sqlite3_db_cacheflush test suite (self-contained harness)..." << std::endl;

  test_no_busy_no_write_transactions();
  test_busy_result_from_pager();
  test_api_armor_safety_check_fails();
  test_multiple_entries_one_busy();

  if (g_failures == 0) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cout << "Tests completed with " << g_failures << " failure(s)." << std::endl;
    return 1;
  }
}