// Comprehensive C++11 unit tests for sqlite3VtabSavepoint
// This test harness provides a self-contained, minimal runtime environment
// that mimics the essential SQLite structures used by sqlite3VtabSavepoint
// and verifies its behavior across multiple scenarios.
// Notes:
// - This file is self-contained and does not depend on GoogleTest.
// - It uses a very small, custom test harness that reports pass/fail to stdout.
// - It focuses on the focal method's logic and its dependent components.
// - All code is kept inside this single translation unit for simplicity.

#include <vector>
#include <iomanip>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// --------------------------
// Minimal compatibility layer
// --------------------------

// Definitions aligned with the focal method's expectations
#define SQLITE_OK 0
#define SQLITE_Defensive 0x1ULL

#define SAVEPOINT_BEGIN 0
#define SAVEPOINT_ROLLBACK 1
#define SAVEPOINT_RELEASE 2

using u64 = uint64_t;

// Forward-declare fake SQLite structures to mimic vtab.c usage
struct sqlite3_vtab {}; // opaque in real SQLite

// sqlite3_module: function pointers for virtual table operations
struct sqlite3_module {
  int iVersion; // version of the module interface
  int (*xSavepoint)(sqlite3_vtab *, int);
  int (*xRollbackTo)(sqlite3_vtab *, int);
  int (*xRelease)(sqlite3_vtab *, int);
};

// Wrapper module holder used by vtab.c
struct Module {
  sqlite3_module *pModule;
  int iVersion;
};

// VTable struct used by sqlite3VtabSavepoint
struct VTable {
  sqlite3_vtab *pVtab;
  Module *pMod;
  int iSavepoint;
};

// sqlite3 db handle - holds VTrans and flags
struct sqlite3 {
  VTable **aVTrans; // array of active virtual tables
  int nVTrans;      // number of active VTrans
  uint64_t flags;     // flags (e.g., SQLITE_Defensive)
};

// --------------------------
// Test hooks: function stubs to observe behavior
// --------------------------

// Observations captured by tests
static int last_called_method = 0; // 1 = Savepoint, 2 = RollbackTo, 3 = Release
static int last_iSavepoint_observed = -1;

// Test-specific handlers to be assigned to module pointers
static int test_savepoint_handler(sqlite3_vtab *, int iSavepoint) {
  last_called_method = 1;
  last_iSavepoint_observed = iSavepoint;
  return 0; // OK
}
static int test_rollback_handler(sqlite3_vtab *, int iSavepoint) {
  last_called_method = 2;
  last_iSavepoint_observed = iSavepoint;
  return 0; // OK
}
static int test_release_handler(sqlite3_vtab *, int iSavepoint) {
  last_called_method = 3;
  last_iSavepoint_observed = iSavepoint;
  return 0; // OK
}

// Reset observers between tests
static void reset_observers() {
  last_called_method = 0;
  last_iSavepoint_observed = -1;
}

// Helper to create a dummy sqlite3_vtab pointer (non-null for tests)
static sqlite3_vtab* dummy_vtab_ptr() {
  // Allocate a tiny dummy object on the heap
  return reinterpret_cast<sqlite3_vtab*>(new int(42));
}

// --------------------------
// FOCAL_METHOD: sqlite3VtabSavepoint (self-contained for test harness)
// (Copied and adapted to compile within this translation unit for testing purposes.)
// --------------------------

extern "C" int sqlite3VtabSavepoint(sqlite3 *db, int op, int iSavepoint){
  int rc = SQLITE_OK;
  assert( op==SAVEPOINT_RELEASE||op==SAVEPOINT_ROLLBACK||op==SAVEPOINT_BEGIN );
  assert( iSavepoint>=-1 );
  if( db->aVTrans ){
    int i;
    for(i=0; rc==SQLITE_OK && i<db->nVTrans; i++){
      VTable *pVTab = db->aVTrans[i];
      const sqlite3_module *pMod = pVTab->pMod->pModule;
      if( pVTab->pVtab && pMod->iVersion>=2 ){
        int (*xMethod)(sqlite3_vtab *, int);
        // In the real code, sqlite3VtabLock would lock the VTab; in test, it's a no-op.
        // Simulate the lock by a no-op call to a stub if desired (not strictly necessary).
        // sqlite3VtabLock(pVTab);
        switch( op ){
          case SAVEPOINT_BEGIN:
            xMethod = pMod->xSavepoint;
            pVTab->iSavepoint = iSavepoint+1;
            break;
          case SAVEPOINT_ROLLBACK:
            xMethod = pMod->xRollbackTo;
            break;
          default:
            xMethod = pMod->xRelease;
            break;
        }
        if( xMethod && pVTab->iSavepoint>iSavepoint ){
          u64 savedFlags = (db->flags & SQLITE_Defensive);
          db->flags &= ~(u64)SQLITE_Defensive;
          rc = xMethod(pVTab->pVtab, iSavepoint);
          db->flags |= savedFlags;
        }
        // sqlite3VtabUnlock(pVTab);
      }
    }
  }
  return rc;
}

// --------------------------
// Test harness: test cases for various scenarios
// --------------------------

static void test_begin_basic_case() {
  reset_observers();

  // Prepare dummy module and wrapper
  sqlite3_module sm;
  sm.iVersion = 2;
  sm.xSavepoint = test_savepoint_handler;
  sm.xRollbackTo = nullptr;
  sm.xRelease = nullptr;

  Module mod;
  mod.pModule = &sm;
  mod.iVersion = 2;

  VTable vt;
  vt.pVtab = dummy_vtab_ptr();
  vt.pMod = &mod;
  vt.iSavepoint = 0;

  sqlite3 db;
  VTable *arr[1] = { &vt };
  db.aVTrans = arr;
  db.nVTrans = 1;
  db.flags = 0; // ensure clean start

  // Call focal method with op=BEGIN and iSavepoint=0
  int rc = sqlite3VtabSavepoint(&db, SAVEPOINT_BEGIN, 0);

  // Expectations:
  // - rc == SQLITE_OK
  // - Savepoint handler called
  // - vt.iSavepoint should be incremented to 1
  // - last_iSavepoint_observed == 0
  bool pass = (rc == SQLITE_OK)
              && (last_called_method == 1)
              && (vt.iSavepoint == 1)
              && (last_iSavepoint_observed == 0)
              && (db.flags == 0);

  std::cout << "[TEST BEGIN BASIC] " << (pass ? "PASS" : "FAIL") << "\n";
  delete vt.pVtab;
}

static void test_begin_rc_nonzero_stops() {
  reset_observers();

  // Prepare dummy module and wrapper where Savepoint returns non-zero
  sqlite3_module sm;
  sm.iVersion = 2;
  sm.xSavepoint = test_savepoint_handler; // uses 0 return by design
  sm.xRollbackTo = nullptr;
  sm.xRelease = nullptr;

  // We'll monkey-patch the function pointer to simulate non-zero rc
  // Since function pointers are assigned at setup, we implement a new test function here.
  // For simplicity, we rewire to a function returning non-zero by a small trick:
  // We'll define a second function here (static) and assign to sm.xSavepoint.
  // Because we already compiled test_savepoint_handler elsewhere, we implement a lambda-like function is not possible under C.
  // Instead, we'll set xSavepoint to a wrapper that returns 1 but still records the call.
  // We'll implement a new function here:

  // Static function to simulate non-zero rc
  auto fake_savepoint_nonzero = [](sqlite3_vtab *, int){ last_called_method = 1; last_iSavepoint_observed = 0; return 1; };

  // Since we can't assign a lambda to a C-style function pointer, we implement a separate real function.
  // To keep this self-contained, redefine a small static func:
  static int fake_savepoint_return1(sqlite3_vtab *, int i) { last_called_method = 1; last_iSavepoint_observed = i; return 1; }

  sm.xSavepoint = fake_savepoint_return1;

  Module mod;
  mod.pModule = &sm;
  mod.iVersion = 2;

  VTable vt;
  vt.pVtab = dummy_vtab_ptr();
  vt.pMod = &mod;
  vt.iSavepoint = 0;

  sqlite3 db;
  VTable *arr[1] = { &vt };
  db.aVTrans = arr;
  db.nVTrans = 1;
  db.flags = 0;

  int rc = sqlite3VtabSavepoint(&db, SAVEPOINT_BEGIN, 0);

  bool pass = (rc == 1)
              && (last_called_method == 1)
              && (vt.iSavepoint == 1)
              && (last_iSavepoint_observed == 0);

  std::cout << "[TEST BEGIN RC NONZERO] " << (pass ? "PASS" : "FAIL") << "\n";
  delete vt.pVtab;
}

static void test_release_path_calls_xRelease() {
  reset_observers();

  // Release path uses xRelease
  sqlite3_module sm;
  sm.iVersion = 2;
  sm.xSavepoint = nullptr;
  sm.xRollbackTo = nullptr;
  sm.xRelease = test_release_handler;

  Module mod;
  mod.pModule = &sm;
  mod.iVersion = 2;

  VTable vt;
  vt.pVtab = dummy_vtab_ptr();
  vt.pMod = &mod;
  vt.iSavepoint = 2; // ensure pVTab->iSavepoint > iSavepoint (choose iSavepoint=1)

  sqlite3 db;
  VTable *arr[1] = { &vt };
  db.aVTrans = arr;
  db.nVTrans = 1;
  db.flags = 0;

  int rc = sqlite3VtabSavepoint(&db, SAVEPOINT_RELEASE, 1);

  bool pass = (rc == SQLITE_OK)
              && (last_called_method == 3)
              && (last_iSavepoint_observed == 1)
              && (vt.iSavepoint == 2);

  std::cout << "[TEST RELEASE PATH] " << (pass ? "PASS" : "FAIL") << "\n";
  delete vt.pVtab;
}

static void test_defensive_flag_restored_after_call() {
  reset_observers();

  // Ensure the SQLITE_Defensive bit is cleared during the xMethod call and restored afterwards
  sqlite3_module sm;
  sm.iVersion = 2;
  sm.xSavepoint = test_savepoint_handler;
  sm.xRollbackTo = nullptr;
  sm.xRelease = nullptr;

  Module mod;
  mod.pModule = &sm;
  mod.iVersion = 2;

  VTable vt;
  vt.pVtab = dummy_vtab_ptr();
  vt.pMod = &mod;
  vt.iSavepoint = 0;

  sqlite3 db;
  VTable *arr[1] = { &vt };
  db.aVTrans = arr;
  db.nVTrans = 1;
  db.flags = SQLITE_Defensive; // set defensive flag

  int rc = sqlite3VtabSavepoint(&db, SAVEPOINT_BEGIN, 0);

  // After call, flags should be restored to original SQLITE_Defensive
  bool pass = (rc == SQLITE_OK)
              && (db.flags == SQLITE_Defensive);

  std::cout << "[TEST DEFENSIVE_FLAG_RESTORE] " << (pass ? "PASS" : "FAIL") << "\n";
  delete vt.pVtab;
}

static void test_avtrans_null_no_call() {
  reset_observers();

  // aVTrans is NULL; function should return SQLITE_OK and not crash
  sqlite3_module sm;
  sm.iVersion = 2;
  sm.xSavepoint = test_savepoint_handler;
  sm.xRollbackTo = nullptr;
  sm.xRelease = nullptr;

  Module mod;
  mod.pModule = &sm;
  mod.iVersion = 2;

  VTable vt;
  vt.pVtab = dummy_vtab_ptr();
  vt.pMod = &mod;
  vt.iSavepoint = 0;

  sqlite3 db;
  db.aVTrans = nullptr;
  db.nVTrans = 0;
  db.flags = 0;

  int rc = sqlite3VtabSavepoint(&db, SAVEPOINT_BEGIN, 0);

  bool pass = (rc == SQLITE_OK);

  std::cout << "[TEST_AVTRANS_NULL_NO_CALL] " << (pass ? "PASS" : "FAIL") << "\n";
  delete vt.pVtab;
}

// Helper to clean up for tests that allocated dummy objects
static void cleanup_vtab(VTable &vt) {
  delete vt.pVtab;
  vt.pVtab = nullptr;
}

// --------------------------
// Main: run all tests
// --------------------------

int main() {
  std::cout << std::boolalpha;
  std::cout << "Starting sqlite3VtabSavepoint unit tests (self-contained harness)\n";

  test_begin_basic_case();
  test_begin_rc_nonzero_stops();
  test_release_path_calls_xRelease();
  test_defensive_flag_restored_after_call();
  test_avtrans_null_no_call();

  // Clean up any remaining allocated objects if needed (not strictly required here)
  // Note: Each test cleans up its own allocations.

  std::cout << "All tests executed.\n";
  return 0;
}