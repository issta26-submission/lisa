// Unit tests for sqlite3VtabBegin (C API) using a handcrafted C++11 test harness.
// This test suite does not rely on GoogleTest/GMock and uses simple EXPECT_* macros.
// The test targets the behavior of sqlite3VtabBegin in vtab.c by providing lightweight
// mocks for the dependent structures (sqlite3, VTable, VTab, sqlite3_module) and a
// small fake sqlite3VtabInSync function to control branches.

// Note: This test suite assumes the focal function is compiled together with vtab.c
// in the same build, and uses the same type layout as described in the problem
// (VTable, VTab, sqlite3_module, sqlite3 in the test harness).

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cstdint>


// Lightweight test harness macros (non-terminating assertions)
static int g_test_failures = 0;
#define EXPECT_EQ(a, b) do { if((a) != (b)) { ++g_test_failures; std::cerr << "EXPECT_EQ failed: " #a " != " #b " (line " << __LINE__ << ")\n"; } } while(0)
#define EXPECT_TRUE(a) do { if(!(a)) { ++g_test_failures; std::cerr << "EXPECT_TRUE failed: " #a " is false (line " << __LINE__ << ")\n"; } } while(0)
#define EXPECT_FALSE(a) do { if((a)) { ++g_test_failures; std::cerr << "EXPECT_FALSE failed: " #a " is true (line " << __LINE__ << ")\n"; } } while(0)

// Domain-specific: minimal, self-contained definitions matching vtab.c usage
// The actual types in vtab.c are likely defined similarly in its TU; we mirror them here.

extern "C" {
  // Forward declaration of the focal function (C linkage)
  int sqlite3VtabBegin(struct sqlite3 *db, struct VTable *pVTab);
  // The function sqlite3VtabInSync is usually defined elsewhere in SQLite.
  // We provide a test control so that tests can influence the in-sync state.
  int sqlite3VtabInSync(struct sqlite3 *db);
}

// Forward declare minimal structures to match vtab.c usage
struct sqlite3 {
  int nVTrans;
  struct VTable **aVTrans;
  int nStatement;
  int nSavepoint;
  int inSync; // control flag for sqlite3VtabInSync stub
};

// VTab represents the inner tab object that holds a pointer to the module
struct VTab {
  const struct sqlite3_module *pModule;
};

// VTable represents the wrapper object passed to sqlite3VtabBegin
struct VTable {
  VTab *pVtab;
  int iSavepoint;
};

// sqlite3_module with xBegin and xSavepoint callbacks
struct sqlite3_module {
  int (*xBegin)(void *pVtab);
  int (*xSavepoint)(void *pVtab, int iSavepoint);
};

// Implementation of sqlite3VtabInSync used by sqlite3VtabBegin.
// This stub returns nonzero when db->inSync is nonzero.
extern "C" int sqlite3VtabInSync(struct sqlite3 *db) {
  return db ? db->inSync : 0;
}

// Test helper: backend module implementations

// Module A: xBegin -> SQLITE_OK, xSavepoint -> SQLITE_OK (normal path)
static int xBegin_A(void *pVtab) { (void)pVtab; return 0; }      // SQLITE_OK
static int xSavepoint_A(void *pVtab, int i) { (void)pVtab; (void)i; return 0; }

// Module B: xBegin -> SQLITE_ERROR (simulate failure), xSavepoint -> SQLITE_OK
static int xBegin_B(void *pVtab) { (void)pVtab; return 1; }       // SQLITE_ERROR (non-OK)
static int xSavepoint_B(void *pVtab, int i) { (void)pVtab; (void)i; return 0; }

// Module C: xBegin -> SQLITE_OK, xSavepoint is NULL (no savepoint handling)
static int xBegin_C(void *pVtab) { (void)pVtab; return 0; }

// Global to observe xSavepoint calls for Test 4
static int g_lastSavepointCalledWith = -1;

// Redefine xSavepoint_A to capture the argument when called
static int xSavepoint_A_capture(void *pVtab, int i) {
  (void)pVtab;
  g_lastSavepointCalledWith = i;
  return 0;
}

// Replace module A's xSavepoint with capture version
static struct sqlite3_module moduleA = { xBegin_A, xSavepoint_A_capture };
static struct sqlite3_module moduleB = { xBegin_B, xSavepoint_B };
static struct sqlite3_module moduleC = { xBegin_C, nullptr }; // NULL savepoint

// Helper: initialize a minimal sqlite3-like db for tests
static void init_db(struct sqlite3 *db) {
  db->nVTrans = 0;
  db->aVTrans = new VTable*[4];
  for (int i = 0; i < 4; ++i) db->aVTrans[i] = nullptr;
  db->nStatement = 0;
  db->nSavepoint = 0;
  db->inSync = 0;
}

// Test 1: If db is in-sync, sqlite3VtabBegin should return SQLITE_LOCKED
void test_locked_when_in_sync() {
  struct sqlite3 db;
  init_db(&db);

  // Prepare a VTab with Module A (works)
  VTab tab;
  tab.pModule = &moduleA;
  VTable vt;
  vt.pVtab = &tab;
  vt.iSavepoint = 0;
  VTable *pVTab = &vt;

  // Put pVTab into aVTrans? not needed for this test
  // Put db inSync = 1 to trigger SQLITE_LOCKED
  db.inSync = 1;

  int rc = sqlite3VtabBegin(&db, pVTab);
  EXPECT_EQ(rc, 6); // SQLITE_LOCKED
  // Clean up
  delete[] db.aVTrans;
}

// Test 2: When pVTab is NULL, should return SQLITE_OK immediately
void test_null_pvtab_returns_ok() {
  struct sqlite3 db;
  init_db(&db);

  int rc = sqlite3VtabBegin(&db, nullptr);
  EXPECT_EQ(rc, 0); // SQLITE_OK
  delete[] db.aVTrans;
}

// Test 3: If pVTab is already present in db->aVTrans, should return SQLITE_OK early
void test_already_in_trans_skips_begin() {
  struct sqlite3 db;
  init_db(&db);

  // Prepare a VTab with Module A
  VTab tab;
  tab.pModule = &moduleA;
  VTable vt;
  vt.pVtab = &tab;
  vt.iSavepoint = 0;
  VTable *pVTab = &vt;

  // Place into aVTrans array to simulate already-tracked transaction
  db.aVTrans[0] = pVTab;
  db.nVTrans = 1;

  int rc = sqlite3VtabBegin(&db, pVTab);
  EXPECT_EQ(rc, 0); // SQLITE_OK
  // Ensure no modification to nVTrans (still 1)
  EXPECT_EQ(db.nVTrans, 1);
  delete[] db.aVTrans;
}

// Test 4: Normal path: xBegin succeeds, addToVTrans occurs, and xSavepoint is invoked when iSvpt > 0
void test_begin_success_with_savepoint() {
  struct sqlite3 db;
  init_db(&db);

  // Prepare a VTab with Module A (xSavepoint will capture the iSvpt-1)
  VTab tab;
  tab.pModule = &moduleA;
  VTable vt;
  vt.pVtab = &tab;
  vt.iSavepoint = 0;
  VTable *pVTab = &vt;

  // Ensure moduleA's xSavepoint is active (capture)
  // Reset global capture
  g_lastSavepointCalledWith = -1;

  // Set db state to have a non-zero iSvpt: nStatement + nSavepoint
  db.nStatement = 2;
  db.nSavepoint = 0; // iSvpt = 2
  // Ensure not already in trans
  int rc = sqlite3VtabBegin(&db, pVTab);

  EXPECT_EQ(rc, 0); // SQLITE_OK
  // After successful begin, the vtab should be added to aVTrans
  EXPECT_TRUE(db.nVTrans == 1);
  EXPECT_TRUE(db.aVTrans[0] == pVTab);
  // pVTab should have iSavepoint set to iSvpt (2)
  EXPECT_EQ(pVTab->iSavepoint, 2);
  // xSavepoint should have been called with iSvpt-1 (1)
  EXPECT_EQ(g_lastSavepointCalledWith, 1);

  delete[] db.aVTrans;
}

// Test 5: If xBegin fails, rc propagates and the vtab is not added to aVTrans
void test_begin_failure_propagates_and_no_add() {
  struct sqlite3 db;
  init_db(&db);

  // Prepare a VTab with Module B (xBegin returns error)
  VTab tab;
  tab.pModule = &moduleB;
  VTable vt;
  vt.pVtab = &tab;
  vt.iSavepoint = 0;
  VTable *pVTab = &vt;

  int rc = sqlite3VtabBegin(&db, pVTab);
  // Expect non-OK due to module B's xBegin returning SQLITE_ERROR (1)
  EXPECT_EQ(rc, 1);

  // Should not have added to aVTrans
  EXPECT_EQ(db.nVTrans, 0);

  delete[] db.aVTrans;
}

// Test runner
int main() {
  test_locked_when_in_sync();
  test_null_pvtab_returns_ok();
  test_already_in_trans_skips_begin();
  test_begin_success_with_savepoint();
  test_begin_failure_propagates_and_no_add();

  if(g_test_failures == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cerr << g_test_failures << " test(s) failed.\n";
    return 1;
  }
}