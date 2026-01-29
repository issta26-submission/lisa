// Minimal C++11 unit tests for the focal function: sqlite3_backup_finish
// This test harness provides lightweight stubs for the required SQLite-like
// structures and functions so that sqlite3_backup_finish can be exercised
// in isolation without depending on the full SQLite project.
//
// The tests cover:
 // - Null input short-circuit
 // - Destination DB NULL path
 // - Destination DB present path with non-DONE rc
 // - isAttached detach path via pager backup list
 //
 // Note: This is a self-contained test harness; it mocks only what is needed
 // to drive sqlite3_backup_finish through its internal branches.

#include <cstdio>
#include <btreeInt.h>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Domain knowledge constraints implemented in test harness.
// Lightweight emulation of necessary SQLite components.

// API-like constants
#define SQLITE_OK 0
#define SQLITE_DONE 101

// Forward declarations of structures (minimal subset for tests)
struct sqlite3;
struct sqlite3_backup;
struct Btree;
struct sqlite3Pager;

// Typedefs to mirror usage in backup_finish
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_backup sqlite3_backup;
typedef struct Btree Btree;
typedef struct sqlite3Pager sqlite3Pager;

// Lightweight mutex representation (no-op in tests)
typedef void* sqlite3_mutex;

// Minimal sqlite3 object with a place to record last error and a dummy mutex
struct sqlite3 {
  sqlite3_mutex mutex;
  int lastError;
};

// B-tree node-like structure with a back-reference to its pager and a counter
struct Btree {
  int nBackup;        // number of backups attached to this Btree (as used by finish)
  sqlite3Pager* pager; // associated pager for this Btree
};

// Pager structure capable of holding a linked list head for backups
struct sqlite3Pager {
  sqlite3_backup** pBackupHead; // pointer to the head slot that points to the first backup
};

// SQLite backup control object (the subject under test)
struct sqlite3_backup {
  sqlite3* pSrcDb; // source database handle
  sqlite3* pDestDb; // destination database handle (may be NULL)
  Btree* pSrc;     // source B-tree
  Btree* pDest;    // destination B-tree (not used heavily here)
  int rc;            // return code to mimic internal rc
  int isAttached;    // indicates if this backup is attached to a pager
  sqlite3_backup* pNext; // next backup in the list (for pager bookkeeping)
};

// Lightweight helpers to simulate internal SQLite API used by sqlite3_backup_finish

static void sqlite3_mutex_enter(void* mutex) {
  // no-op for test
  (void)mutex;
}
static void sqlite3BtreeEnter(Btree* p) {
  (void)p;
}
static void sqlite3BtreeLeave(Btree* p) {
  (void)p;
}
static void sqlite3BtreeRollback(Btree* pDest, int rc, int ignored) {
  (void)rc;
  (void)ignored;
  // In test we don't simulate a rollback unless needed for coverage
  (void)pDest;
}
static void sqlite3Error(sqlite3* db, int rc) {
  if (db) db->lastError = rc;
}
static void sqlite3LeaveMutexAndCloseZombie(sqlite3* db) {
  // no-op for test
  (void)db;
}
static void sqlite3_free(void* p) {
  free(p);
}
static sqlite3Pager* sqlite3BtreePager(Btree* p) {
  return p ? p->pager : nullptr;
}
static sqlite3_backup** sqlite3PagerBackupPtr(sqlite3Pager* p) {
  return p ? p->pBackupHead : nullptr;
}

// The focal method under test (replicated in the test harness)
int sqlite3_backup_finish(sqlite3_backup *p){
  sqlite3_backup **pp;                 /* Ptr to head of pagers backup list */
  sqlite3 *pSrcDb;                     /* Source database connection */
  int rc;                              /* Value to return */
  /* Enter the mutexes */
  if( p==0 ) return SQLITE_OK;
  pSrcDb = p->pSrcDb;
  sqlite3_mutex_enter(pSrcDb->mutex);
  sqlite3BtreeEnter(p->pSrc);
  if( p->pDestDb ){
    sqlite3_mutex_enter(p->pDestDb->mutex);
  }
  /* Detach this backup from the source pager. */
  if( p->pDestDb ){
    p->pSrc->nBackup--;
  }
  if( p->isAttached ){
    pp = sqlite3PagerBackupPtr(sqlite3BtreePager(p->pSrc));
    assert( pp!=0 );
    while( *pp!=p ){
      pp = &(*pp)->pNext;
      assert( pp!=0 );
    }
    *pp = p->pNext;
  }
  /* If a transaction is still open on the Btree, roll it back. */
  sqlite3BtreeRollback(p->pDest, SQLITE_OK, 0);
  /* Set the error code of the destination database handle. */
  rc = (p->rc==SQLITE_DONE) ? SQLITE_OK : p->rc;
  if( p->pDestDb ){
    sqlite3Error(p->pDestDb, rc);
    /* Exit the mutexes and free the backup context structure. */
    sqlite3LeaveMutexAndCloseZombie(p->pDestDb);
  }
  sqlite3BtreeLeave(p->pSrc);
  if( p->pDestDb ){
    /* EVIDENCE-OF: R-64852-21591 The sqlite3_backup object is created by a
    ** call to sqlite3_backup_init() and is destroyed by a call to
    ** sqlite3_backup_finish(). */
    sqlite3_free(p);
  }
  sqlite3LeaveMutexAndCloseZombie(pSrcDb);
  return rc;
}

// Lightweight test harness utilities
static int g_total = 0;
static int g_failed = 0;

// Simple non-terminating assertions for test
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    ++g_failed; \
    std::cerr << "EXPECT_TRUE failed: " << __FILE__ << ":" << __LINE__ << "\n"; \
  } \
  ++g_total; \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if(!((a)==(b))){ \
    ++g_failed; \
    std::cerr << "EXPECT_EQ failed: " << #a << " != " << #b \
              << " (" << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
  } \
  ++g_total; \
} while(0)


// Helpers to build test scenarios

// Test 1: Null input should return SQLITE_OK without side effects
void test_backup_finish_null_input() {
  // Reset counters
  g_total = g_failed = 0;

  int rc = sqlite3_backup_finish(nullptr);

  EXPECT_EQ(rc, SQLITE_OK);
  // No crash should happen, nothing else to verify
  std::cout << "Test 1 (null input) completed. Passed checks: " << (g_total - g_failed) << "\n";
}

// Test 2: Destination DB is NULL; rc == SQLITE_DONE leads to SQLITE_OK, and no free happens
void test_backup_finish_no_dest_no_error() {
  g_total = g_failed = 0;

  // Prepare source DB and its mutex
  sqlite3 src{};
  int dummy_mutex;
  src.mutex = (sqlite3_mutex)&dummy_mutex; // non-null

  // Prepare source B-tree with a pager
  Btree srcBt{};
  sqlite3Pager pager{};
  srcBt.nBackup = 0;
  pager.pBackupHead = nullptr; // not used in this test
  srcBt.pager = &pager;

  // Prepare backup object with no destination
  sqlite3_backup backup{};
  backup.pSrcDb = &src;
  backup.pDestDb = nullptr;
  backup.pSrc = &srcBt;
  backup.rc = SQLITE_DONE; // will be interpreted as OK
  backup.pDest = nullptr;
  backup.isAttached = 0;
  backup.pNext = nullptr;

  // pSrc.pDest null, so we should not call sqlite3_free
  // Also, pSrc->nBackup should not be decremented (no dest DB)
  int beforeNBackup = backup.pSrc->nBackup;
  int rc = sqlite3_backup_finish(&backup);

  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_TRUE(backup.isAttached == 0);
  EXPECT_EQ(backup.pSrc->nBackup, beforeNBackup);

  std::cout << "Test 2 (no dest) completed. RC=" << rc << ", passed checks: " << (g_total - g_failed) << "\n";
}

// Test 3: Destination DB present; rc != SQLITE_DONE should propagate error, and backup memory freed
static int g_free_called = 0;
static void* test_sqlite3_free(void* p) {
  ++g_free_called;
  free(p);
  return nullptr;
}
void test_backup_finish_with_dest_rc_not_done() {
  g_total = g_failed = 0;
  g_free_called = 0;

  // Source DB with mutex
  sqlite3 src{};
  int m = 7;
  src.mutex = &m;

  // Source Btree
  Btree srcBt{};
  sqlite3Pager pager{};
  srcBt.nBackup = 1;
  pager.pBackupHead = nullptr;
  srcBt.pager = &pager;

  // Destination DB with mutex
  sqlite3 dest{};
  int m2 = 8;
  dest.mutex = &m2;
  dest.lastError = 0;

  // Destination objects
  Btree destBt{};
  destBt.nBackup = 0;
  destBt.pager = nullptr;

  // Backup object
  sqlite3_backup backup{};
  backup.pSrcDb = &src;
  backup.pDestDb = &dest;
  backup.pSrc = &srcBt;
  backup.pDest = &destBt;
  backup.rc = 5; // not SQLITE_DONE
  backup.isAttached = 0;
  backup.pNext = nullptr;

  // We expect rc = 5 to be returned and dest.get lastError set to 5
  int rc = sqlite3_backup_finish(&backup);

  EXPECT_EQ(rc, 5);
  EXPECT_TRUE(dest.lastError == 5);
  // backup should be freed since pDestDb is non-null
  EXPECT_TRUE(g_free_called == 1);

  std::cout << "Test 3 (dest non-DONE rc) completed. RC=" << rc << ", freeCalled=" << g_free_called
            << ", passed checks: " << (g_total - g_failed) << "\n";
}

// Test 4: isAttached path detaches from the pager's backup list
void test_backup_finish_detach_from_pager_list() {
  g_total = g_failed = 0;

  // Setup source DB and mutex
  sqlite3 src{};
  int m = 1;
  src.mutex = &m;

  // Pager with a head pointer to backups
  sqlite3Pager pager{};
  sqlite3_backup* head = nullptr; // will be head of the list
  pager.pBackupHead = &head;

  // Create the backup to be finished; it will be in the pager's list
  sqlite3_backup* be = (sqlite3_backup*)malloc(sizeof(sqlite3_backup));
  be->pSrcDb = &src;
  be->pDestDb = nullptr; // destination not needed for detach path
  Btree srcBt{};
  srcBt.nBackup = 1;
  srcBt.pager = &pager; // link
  be->pSrc = &srcBt;
  be->pDest = nullptr;
  be->rc = SQLITE_OK;
  be->isAttached = 1; // must detach
  be->pNext = nullptr;

  // Put be into the pager's backup list: head -> be
  head = be;
  // Also ensure some other item after be to verify proper detachment in general
  // but for simplicity keep a single element.

  int rc = sqlite3_backup_finish(be);

  // After finish, head should be be->pNext (nullptr in this test)
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_TRUE(head == be->pNext);

  std::cout << "Test 4 (detach from pager list) completed. RC=" << rc
            << ", head==be->pNext? " << (head == be->pNext ? "yes" : "no")
            << ", passed checks: " << (g_total - g_failed) << "\n";

  // Cleanup
  free(be);
}

// Entry point to run tests
int main() {
  std::cout << "Starting sqlite3_backup_finish unit tests (custom harness) ...\n";

  test_backup_finish_null_input();
  test_backup_finish_no_dest_no_error();
  test_backup_finish_with_dest_rc_not_done();
  test_backup_finish_detach_from_pager_list();

  int total = g_total;
  int failed = g_failed;
  std::cout << "Tests completed. Total: " << total << ", Failed: " << failed << "\n";

  return (failed == 0) ? 0 : 1;
}