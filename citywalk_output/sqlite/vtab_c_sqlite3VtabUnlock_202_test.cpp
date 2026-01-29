// Minimal C++11 unit test suite for the focal function sqlite3VtabUnlock
// This test suite does not rely on any external testing framework (GTest, etc.).
// It creates lightweight stubs of the necessary SQLite structures and functions
// to exercise the branches of sqlite3VtabUnlock as defined in the prompt.
//
// Notes:
// - We implement the focal function sqlite3VtabUnlock in this translation unit
//   along with minimal type definitions to enable compilation.
// - We test true/false branches of key predicates:
//   * pVTab->nRef reaching 0 vs. remaining >0
//   * pVTab->pVtab being non-null vs null
//   * db->eOpenState being SQLITE_STATE_OPEN vs SQLITE_STATE_ZOMBIE
// - We verify side effects by counting calls to xDisconnect, sqlite3VtabModuleUnref,
//   and sqlite3DbFree, rather than inspecting internal private state after free.

#include <iostream>
#include <sqliteInt.h>
#include <cassert>


// Domain-specific lightweight stubs and types to model the necessary SQLite-like API.

struct Module;            // Forward declaration for Module used by sqlite3_vtab
struct sqlite3_vtab {
  Module *pModule;        // Module instance associated with this virtual table
};

struct Module {
  void (*xDisconnect)(sqlite3_vtab*); // Callback invoked on disconnect
};

struct sqlite3 {
  int eOpenState;           // Open state (OPEN or ZOMBIE)
};

// Minimal VTable structure matching the expected layout used by sqlite3VtabUnlock
struct VTable {
  sqlite3 *db;                // Pointer to database connection
  int nRef;                   // Reference count for the virtual table
  sqlite3_vtab *pVtab;         // Pointer to underlying sqlite3_vtab (may be NULL)
  Module *pMod;                // Module pointer for this VTable
};

// Forward declarations of functions used within sqlite3VtabUnlock
void sqlite3VtabModuleUnref(sqlite3 *db, Module *pMod);
void sqlite3DbFree(sqlite3 *db, void *p);

// Focal function under test (reproduced as provided)
void sqlite3VtabUnlock(VTable *pVTab){
  sqlite3 *db = pVTab->db;
  assert( db );
  assert( pVTab->nRef>0 );
  assert( db->eOpenState==0  // SQLITE_STATE_OPEN
       || db->eOpenState==1 ); // SQLITE_STATE_ZOMBIE
  pVTab->nRef--;
  if( pVTab->nRef==0 ){
    sqlite3_vtab *p = pVTab->pVtab;
    if( p ){
      p->pModule->xDisconnect(p);
    }
    sqlite3VtabModuleUnref(pVTab->db, pVTab->pMod);
    sqlite3DbFree(db, pVTab);
  }
}

// Global counters and helpers to capture side effects
static int g_disconnect_calls = 0;
static int g_vtabUnref_calls = 0;
static int g_dbFree_calls = 0;

// Per-test reset
static void reset_counters() {
  g_disconnect_calls = 0;
  g_vtabUnref_calls = 0;
  g_dbFree_calls = 0;
}

// xDisconnect implementation used by tests
static void test_xDisconnect(sqlite3_vtab* p) {
  (void)p; // suppress unused-parameter warning
  g_disconnect_calls++;
}

// sqlite3VtabModuleUnref replacement to observe unref calls
void sqlite3VtabModuleUnref(sqlite3 *db, Module *pMod) {
  (void)db;
  (void)pMod;
  g_vtabUnref_calls++;
}

// sqlite3DbFree replacement to observe frees and to deallocate VTable
void sqlite3DbFree(sqlite3 *db, void *p) {
  (void)db;
  VTable *pVTab = static_cast<VTable*>(p);
  // In a real scenario this would free memory allocated for VTab
  // Here we simulate by deleting the object if it was heap-allocated.
  delete pVTab;
  g_dbFree_calls++;
}

// Test 1: nRef == 0 after unlock and pVtab != NULL -> xDisconnect invoked, unref called, and VTab freed
bool test_case_unlock_zero_ref_with_nonnull_pVtab() {
  reset_counters();

  // Prepare module with xDisconnect
  Module mod;
  mod.xDisconnect = test_xDisconnect;

  // Prepare sqlite3_vtab that uses the module
  sqlite3_vtab vt;
  vt.pModule = &mod;

  // Prepare VTable on heap so sqlite3DbFree can delete it
  VTable *pVTab = new VTable();
  sqlite3 db;
  db.eOpenState = 0; // OPEN
  pVTab->db = &db;
  pVTab->nRef = 1;
  pVTab->pVtab = &vt;       // non-null, should trigger disconnect
  pVTab->pMod = &mod;

  sqlite3VtabUnlock(pVTab);

  // Validate side effects
  bool ok = true;
  if (g_disconnect_calls != 1) {
    std::cerr << "Test 1: Expected 1 disconnect call, got " << g_disconnect_calls << "\n";
    ok = false;
  }
  if (g_vtabUnref_calls != 1) {
    std::cerr << "Test 1: Expected 1 VTabUnref call, got " << g_vtabUnref_calls << "\n";
    ok = false;
  }
  if (g_dbFree_calls != 1) {
    std::cerr << "Test 1: Expected 1 DbFree call, got " << g_dbFree_calls << "\n";
    ok = false;
  }

  if (!ok) {
    std::cerr << "Test Case 1 FAILED\n";
  }
  return ok;
}

// Test 2: nRef > 0 after unlock -> no disconnect, no unref, no free
bool test_case_unlock_nonzero_ref_no_action() {
  reset_counters();

  Module mod;
  mod.xDisconnect = test_xDisconnect;

  sqlite3_vtab vt;
  vt.pModule = &mod;

  VTable *pVTab = new VTable();
  sqlite3 db;
  db.eOpenState = 0; // OPEN
  pVTab->db = &db;
  pVTab->nRef = 2;          // After decrement -> 1 (non-zero)
  pVTab->pVtab = &vt;
  pVTab->pMod = &mod;

  sqlite3VtabUnlock(pVTab);

  bool ok = true;
  if (g_disconnect_calls != 0) {
    std::cerr << "Test 2: Expected 0 disconnect calls, got " << g_disconnect_calls << "\n";
    ok = false;
  }
  if (g_vtabUnref_calls != 0) {
    std::cerr << "Test 2: Expected 0 VTabUnref calls, got " << g_vtabUnref_calls << "\n";
    ok = false;
  }
  if (g_dbFree_calls != 0) {
    std::cerr << "Test 2: Expected 0 DbFree calls, got " << g_dbFree_calls << "\n";
    ok = false;
  }

  // Cleanup: since VTab wasn't freed, delete to avoid leak in this test
  delete pVTab;

  if (!ok) {
    std::cerr << "Test Case 2 FAILED\n";
  }
  return ok;
}

// Test 3: pVtab == NULL -> still frees (unref + free) but does not call xDisconnect
bool test_case_unlock_with_null_pVtab() {
  reset_counters();

  Module mod;
  mod.xDisconnect = test_xDisconnect;

  sqlite3_vtab vt;
  vt.pModule = &mod;

  VTable *pVTab = new VTable();
  sqlite3 db;
  db.eOpenState = 0; // OPEN
  pVTab->db = &db;
  pVTab->nRef = 1;
  pVTab->pVtab = nullptr;    // NULL, so no disconnect attempt
  pVTab->pMod = &mod;

  sqlite3VtabUnlock(pVTab);

  bool ok = true;
  if (g_disconnect_calls != 0) {
    std::cerr << "Test 3: Expected 0 disconnect calls, got " << g_disconnect_calls << "\n";
    ok = false;
  }
  if (g_vtabUnref_calls != 1) {
    std::cerr << "Test 3: Expected 1 VTabUnref call, got " << g_vtabUnref_calls << "\n";
    ok = false;
  }
  if (g_dbFree_calls != 1) {
    std::cerr << "Test 3: Expected 1 DbFree call, got " << g_dbFree_calls << "\n";
    ok = false;
  }

  if (!ok) {
    std::cerr << "Test Case 3 FAILED\n";
  }
  return ok;
}

// Test 4: db state ZOMBIE still allows disconnect/unref/free when nRef -> 0 and pVtab != NULL
bool test_case_unlock_zero_ref_with_nonnull_pVtab_zombie() {
  reset_counters();

  Module mod;
  mod.xDisconnect = test_xDisconnect;

  sqlite3_vtab vt;
  vt.pModule = &mod;

  VTable *pVTab = new VTable();
  sqlite3 db;
  db.eOpenState = 1; // ZOMBIE
  pVTab->db = &db;
  pVTab->nRef = 1;
  pVTab->pVtab = &vt;
  pVTab->pMod = &mod;

  sqlite3VtabUnlock(pVTab);

  bool ok = true;
  if (g_disconnect_calls != 1) {
    std::cerr << "Test 4: Expected 1 disconnect call, got " << g_disconnect_calls << "\n";
    ok = false;
  }
  if (g_vtabUnref_calls != 1) {
    std::cerr << "Test 4: Expected 1 VTabUnref call, got " << g_vtabUnref_calls << "\n";
    ok = false;
  }
  if (g_dbFree_calls != 1) {
    std::cerr << "Test 4: Expected 1 DbFree call, got " << g_dbFree_calls << "\n";
    ok = false;
  }

  if (!ok) {
    std::cerr << "Test Case 4 FAILED\n";
  }
  return ok;
}

// Entry point: run tests from main as per domain knowledge
int main() {
  std::cout << "Starting sqlite3VtabUnlock unit tests (no GTest).\n";

  bool t1 = test_case_unlock_zero_ref_with_nonnull_pVtab();
  std::cout << "Test Case 1 " << (t1 ? "PASSED" : "FAILED") << "\n";

  bool t2 = test_case_unlock_nonzero_ref_no_action();
  std::cout << "Test Case 2 " << (t2 ? "PASSED" : "FAILED") << "\n";

  bool t3 = test_case_unlock_with_null_pVtab();
  std::cout << "Test Case 3 " << (t3 ? "PASSED" : "FAILED") << "\n";

  bool t4 = test_case_unlock_zero_ref_with_nonnull_pVtab_zombie();
  std::cout << "Test Case 4 " << (t4 ? "PASSED" : "FAILED") << "\n";

  int passed = (t1?1:0) + (t2?1:0) + (t3?1:0) + (t4?1:0);
  std::cout << "Summary: " << passed << "/4 tests passed.\n";

  // Return non-zero if any test failed
  return (t1 && t2 && t3 && t4) ? 0 : 1;
}