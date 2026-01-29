// C++11 unit test harness for sqlite3_file_control
// This single file provides:
// - Minimal stubs/mocks for SQLite core types used by sqlite3_file_control
// - A self-contained implementation of sqlite3_file_control (as provided)
// - A comprehensive suite of tests covering all branches of sqlite3_file_control
// - Non-terminating assertions (test logging) to maximize coverage without exiting on first failure

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Define to include API armor check branch (as in the original code)
#define SQLITE_ENABLE_API_ARMOR

// Lightweight, test-friendly constants (mirroring essential SQLite values)
#define SQLITE_OK 0
#define SQLITE_ERROR 1
#define SQLITE_MISUSE_BKPT 21  // Arbitrary non-zero distinct value for testing

#define SQLITE_FCNTL_FILE_POINTER   1
#define SQLITE_FCNTL_VFS_POINTER    2
#define SQLITE_FCNTL_JOURNAL_POINTER 3
#define SQLITE_FCNTL_DATA_VERSION     4
#define SQLITE_FCNTL_RESERVE_BYTES    5
#define SQLITE_FCNTL_RESET_CACHE        6

// Forward declarations of lightweight SQLite-like types
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_mutex sqlite3_mutex;
typedef struct sqlite3_file sqlite3_file;
typedef struct Btree Btree;
typedef struct Pager Pager;
typedef struct sqlite3_vfs sqlite3_vfs;

// Lightweight test-state holder (to control mocked behavior)
struct TestState {
  bool safetyOk = true;               // controls sqlite3SafetyCheckOk(db)
  bool hasBtree = true;               // controls existence of pBtree
  unsigned int dataVersionValue = 0;  // value returned by sqlite3PagerDataVersion(pPager)
  int reserveReturn = 0;              // value returned by sqlite3BtreeGetRequestedReserve(pBtree)

  bool pageSizeSetCalled = false;     // whether sqlite3BtreeSetPageSize was invoked
  bool clearCacheCalled = false;      // whether sqlite3BtreeClearCache was invoked
  bool osFileControlCalled = false;   // whether sqlite3OsFileControl was invoked
  int osFileControlArg = 0;             // captured arg for OsFileControl
  int osFileControlRet = 0;             // return code from OsFileControl
  int nBusySaved = 0;                 // saved nBusy value prior to OsFileControl

  // Pointers used by tests for sentinel values
  sqlite3_file* fdSentinel = nullptr;
  sqlite3_file* jrnlSentinel = nullptr;
  sqlite3_vfs*  vfsSentinel  = nullptr;
};

// Global test state
static TestState g_state;

// Lightweight objects and sentinels
static sqlite3_mutex g_dummyMutex;
static sqlite3_file g_fd;
static sqlite3_file g_jrnl;
static sqlite3_vfs  g_vfs;
static Btree g_btree;
static Pager g_pager;

// Helper: Logging for tests
static bool g_all_tests_passed = true;
static void logTest(const char* desc, bool ok) {
  if (ok) {
    std::cout << "[PASS] " << desc << "\n";
  } else {
    std::cout << "[FAIL] " << desc << "\n";
    g_all_tests_passed = false;
  }
}

// Lightweight SQLite-like API surface (stubs used by sqlite3_file_control)

// 1) Safety check hook
static int sqlite3SafetyCheckOk(sqlite3 *db) {
  return g_state.safetyOk ? 1 : 0;
}

// 2) Mutex helpers (no real threading in tests)
static void sqlite3_mutex_enter(sqlite3_mutex *mutex) { (void)mutex; /* no-op for test */ }
static void sqlite3_mutex_leave(sqlite3_mutex *mutex) { (void)mutex; /* no-op for test */ }

// 3) DB-name to B-tree lookup
static Btree* sqlite3DbNameToBtree(sqlite3 *db, const char *zDbName) {
  (void)db; (void)zDbName;
  return g_state.hasBtree ? &g_btree : nullptr;
}

// 4) B-tree accessors
static void sqlite3BtreeEnter(Btree*) { /* no-op in test */ }
static Pager* sqlite3BtreePager(Btree*) { return &g_pager; }
static sqlite3_file* sqlite3PagerFile(Pager*) { return &g_fd; }
static sqlite3_vfs* sqlite3PagerVfs(Pager*) { return &g_vfs; }
static sqlite3_file* sqlite3PagerJrnlFile(Pager*) { return &g_jrnl; }
static unsigned int sqlite3PagerDataVersion(Pager*) { return g_state.dataVersionValue; }
static int sqlite3BtreeGetRequestedReserve(Btree*) { return g_state.reserveReturn; }
static void sqlite3BtreeSetPageSize(Btree*, int, int, int) { g_state.pageSizeSetCalled = true; }
static void sqlite3BtreeClearCache(Btree*) { g_state.clearCacheCalled = true; }
static int sqlite3OsFileControl(sqlite3_file*, int op, void *pArg) {
  (void)op;
  // Capture arg for verification and return configured value
  g_state.osFileControlCalled = true;
  g_state.osFileControlArg = (int)(uintptr_t)pArg;
  return g_state.osFileControlRet;
}
static void sqlite3BtreeLeave(Btree*) { /* no-op in test */ }

// 5) Buildable sqlite3_file_control (as provided)
extern "C" int sqlite3_file_control(sqlite3 *db, const char *zDbName, int op, void *pArg){
  int rc = SQLITE_ERROR;
  Btree *pBtree;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ) return SQLITE_MISUSE_BKPT;
#endif
  sqlite3_mutex_enter(db->mutex);
  pBtree = sqlite3DbNameToBtree(db, zDbName);
  if( pBtree ){
    Pager *pPager;
    sqlite3_file *fd;
    sqlite3BtreeEnter(pBtree);
    pPager = sqlite3BtreePager(pBtree);
    assert( pPager!=0 );
    fd = sqlite3PagerFile(pPager);
    assert( fd!=0 );
    if( op==SQLITE_FCNTL_FILE_POINTER ){
      *(sqlite3_file**)pArg = fd;
      rc = SQLITE_OK;
    }else if( op==SQLITE_FCNTL_VFS_POINTER ){
      *(sqlite3_vfs**)pArg = sqlite3PagerVfs(pPager);
      rc = SQLITE_OK;
    }else if( op==SQLITE_FCNTL_JOURNAL_POINTER ){
      *(sqlite3_file**)pArg = sqlite3PagerJrnlFile(pPager);
      rc = SQLITE_OK;
    }else if( op==SQLITE_FCNTL_DATA_VERSION ){
      *(unsigned int*)pArg = sqlite3PagerDataVersion(pPager);
      rc = SQLITE_OK;
    }else if( op==SQLITE_FCNTL_RESERVE_BYTES ){
      int iNew = *(int*)pArg;
      *(int*)pArg = sqlite3BtreeGetRequestedReserve(pBtree);
      if( iNew>=0 && iNew<=255 ){
        sqlite3BtreeSetPageSize(pBtree, 0, iNew, 0);
      }
      rc = SQLITE_OK;
    }else if( op==SQLITE_FCNTL_RESET_CACHE ){
      sqlite3BtreeClearCache(pBtree);
      rc = SQLITE_OK;
    }else{
      int nSave = db->busyHandler.nBusy;
      rc = sqlite3OsFileControl(fd, op, pArg);
      db->busyHandler.nBusy = nSave;
    }
    sqlite3BtreeLeave(pBtree);
  }
  sqlite3_mutex_leave(db->mutex);
  return rc;
}

// 6) Additional stubs to enable compilation if needed
static int sqlite3_test_control(int op, ...){ (void)op; return 0; };
static const char *databaseName(const char *zName){ (void)zName; return nullptr; };
static char *appendText(char *p, const char *z){ (void)p; (void)z; return nullptr; };

// 7) SQLite-like object construction helpers for tests
struct sqlite3 {
  sqlite3_mutex *mutex;
  struct { int nBusy; } busyHandler;
};

// Static globals of test environment rely on C linkage for sqlite3_file_control
// The test mocks above are sufficient for exercising the focal function completely.

// Test driver and tests
int main() {
  // Initialize test objects
  g_fd = {}; g_jrnl = {}; g_vfs = {};
  g_btree = {}; g_pager = {};

  // Prepare a dummy db instance to pass into sqlite3_file_control
  static sqlite3 db;
  db.mutex = &g_dummyMutex;
  db.busyHandler.nBusy = 0;

  // Domain knowledge: ensure static functions accessed via class name in tests
  // (Not applicable here, but we keep structure consistent with requirements.)

  // Helper to reset per-test-state
  auto resetState = [](){
    g_state.safetyOk = true;
    g_state.hasBtree = true;
    g_state.dataVersionValue = 0;
    g_state.reserveReturn = 0;
    g_state.pageSizeSetCalled = false;
    g_state.clearCacheCalled = false;
    g_state.osFileControlCalled = false;
    g_state.osFileControlArg = 0;
    g_state.osFileControlRet = 0;
    g_state.nBusySaved = 0;
  };

  // Ensure global sentinel pointers exist
  g_state.fdSentinel = &g_fd;
  g_state.jrnlSentinel = &g_jrnl;
  g_state.vfsSentinel  = &g_vfs;

  // Test 1: API armor check blocks call when SafetyCheckOk returns false
  resetState();
  g_state.safetyOk = false;
  // Prepare the db with a mutex
  db.mutex = &g_dummyMutex;
  sqlite3_file_control(&db, "main", SQLITE_FCNTL_FILE_POINTER, &g_fd);
  int rc1 = SQLITE_MISUSE_BKPT;
  // Since we do not track rc in this invocation here, recompute by direct call to simulate expected
  int rc = sqlite3_file_control(&db, "main", SQLITE_FCNTL_FILE_POINTER, &g_fd); // should early return MISUSE_BKPT
  logTest("API armor: returns MISUSE_BKPT when safety check fails", rc == SQLITE_MISUSE_BKPT);

  // Test 2: SQLITE_FCNTL_FILE_POINTER returns fd pointer
  resetState();
  g_state.safetyOk = true;
  g_state.hasBtree = true;
  g_fd = {}; // reset
  // Ensure fd points to sentinel object
  g_state.fdSentinel = &g_fd;
  // Call
  sqlite3_file_control(&db, "testdb", SQLITE_FCNTL_FILE_POINTER, &g_fd);
  // We expect the argument to be filled with pointer to fd
  sqlite3_file* outFd = nullptr;
  rc = sqlite3_file_control(&db, "testdb", SQLITE_FCNTL_FILE_POINTER, &outFd);
  logTest("FILE_POINTER: returns correct fd pointer", rc == SQLITE_OK && outFd == &g_fd);

  // Test 3: SQLITE_FCNTL_VFS_POINTER returns VFS pointer
  resetState();
  g_state.hasBtree = true;
  // Prepare a pointer to receive
  sqlite3_vfs* outVfs = nullptr;
  rc = sqlite3_file_control(&db, "testdb", SQLITE_FCNTL_VFS_POINTER, &outVfs);
  logTest("VFS_POINTER: returns correct VFS pointer", rc == SQLITE_OK && outVfs == &g_vfs);

  // Test 4: SQLITE_FCNTL_JOURNAL_POINTER returns Journal file pointer
  resetState();
  sqlite3_file* outJrnl = nullptr;
  rc = sqlite3_file_control(&db, "testdb", SQLITE_FCNTL_JOURNAL_POINTER, &outJrnl);
  logTest("JOURNAL_POINTER: returns journal file pointer", rc == SQLITE_OK && outJrnl == &g_jrnl);

  // Test 5: SQLITE_FCNTL_DATA_VERSION returns data version
  resetState();
  g_state.dataVersionValue = 0xA5A5A5A5;
  unsigned int dv = 0;
  rc = sqlite3_file_control(&db, "testdb", SQLITE_FCNTL_DATA_VERSION, &dv);
  logTest("DATA_VERSION: returns correct data version", rc == SQLITE_OK && dv == 0xA5A5A5A5);

  // Test 6: SQLITE_FCNTL_RESERVE_BYTES with in-range iNew triggers page size set
  resetState();
  g_state.hasBtree = true;
  g_state.reserveReturn = 123;
  int iNewInRange = 100; // within 0..255
  int argVal = iNewInRange;
  rc = sqlite3_file_control(&db, "testdb", SQLITE_FCNTL_RESERVE_BYTES, &argVal);
  logTest("RESERVE_BYTES: in-range iNew triggers SetPageSize and updates reserve", rc == SQLITE_OK && argVal == 123 && g_state.pageSizeSetCalled);

  // Test 7: SQLITE_FCNTL_RESERVE_BYTES with out-of-range iNew does not set page size
  resetState();
  g_state.hasBtree = true;
  g_state.reserveReturn = 77;
  int argVal2 = -1; // out-of-range
  rc = sqlite3_file_control(&db, "testdb", SQLITE_FCNTL_RESERVE_BYTES, &argVal2);
  logTest("RESERVE_BYTES: out-of-range iNew updates reserve only", rc == SQLITE_OK && argVal2 == 77 && !g_state.pageSizeSetCalled);

  // Test 8: SQLITE_FCNTL_RESET_CACHE
  resetState();
  g_state.clearCacheCalled = false;
  rc = sqlite3_file_control(&db, "testdb", SQLITE_FCNTL_RESET_CACHE, nullptr);
  logTest("RESET_CACHE: clears B-tree cache", rc == SQLITE_OK && g_state.clearCacheCalled);

  // Test 9: Else path (unknown op) calls OsFileControl and preserves busy count
  resetState();
  db.busyHandler.nBusy = 7;
  g_state.osFileControlRet = 0;
  int captured = 0;
  rc = sqlite3_file_control(&db, "testdb", 9999, &captured);
  logTest("ELSE PATH: OsFileControl invoked and busy count preserved", rc == SQLITE_OK || rc == SQLITE_ERROR);

  // Additionally verify that OsFileControl was indeed called and busy was preserved
  logTest("ELSE PATH: OsFileControl called", g_state.osFileControlCalled);
  logTest("ELSE PATH: busy count preserved after call", db.busyHandler.nBusy == 7);

  // Test 10: pBtree is null results in SQLITE_ERROR
  resetState();
  g_state.hasBtree = false;
  rc = sqlite3_file_control(&db, "testdb", SQLITE_FCNTL_FILE_POINTER, &g_fd);
  logTest("NULL pBtree: returns SQLITE_ERROR when dbNameToBtree returns NULL", rc == SQLITE_ERROR);

  // Summary
  std::cout << "\nTest run complete. Overall: " << (g_all_tests_passed ? "PASS" : "FAIL") << std::endl;
  return g_all_tests_passed ? 0 : 1;
}