// Minimal C++11 test harness for sqlite3BtreeLast
// This file provides a compact, self-contained test suite for the focal method
// sqlite3BtreeLast, along with minimal stubbed dependencies to enable compilation
// without the full SQLite codebase. It uses a lightweight, non-terminating
// assertion approach to maximize coverage across test cases.

#include <vector>
#include <btreeInt.h>
#include <string>
#include <iostream>
#include <cassert>


using namespace std;

// Domain knowledge and test harness guidelines implemented here:
// - Non-terminating expectations: we collect failures and report at end.
// - Only standard library usage included; no external test framework (e.g., GTest).
// - Tests are invoked from main as requested.
// - Static helpers are defined with internal linkage and used by the focal method.

// ------------------------------------------------------------
// Minimal Dependencies and Focal Method Subset (C++11 compatible)
// ------------------------------------------------------------

// Simulated return code for SQLite
#define SQLITE_OK 0

// Cursor state and flag constants (simplified)
#define CURSOR_VALID 1
#define BTCF_AtLast 1  // bit flag indicating "at last" state in BTCF

// Forward-declare minimal types to satisfy sqlite3BtreeLast's signature
struct MemPage;
struct BtCursor;
struct BtShared;
struct Btree;
struct Db;

// Lightweight cursor ownership and mutex checks (test-friendly stubs)
int cursorOwnsBtShared(BtCursor *pCur) { return 1; }     // Always true in tests
int sqlite3_mutex_held(void* mutex) { return 1; }        // Always true in tests

// Lightweight database/mutex structures to satisfy pCur->pBtree->db->mutex access
struct Db { void* mutex; };
struct Btree { Db* db; };
struct BtShared { };

// Minimal MemPage structure used by BtCursor
struct MemPage {
  int nCell;  // number of cells on the page
  int leaf;   // is a leaf page (non-zero = true)
};

// Minimal BtCursor structure required by sqlite3BtreeLast
struct BtCursor {
  int eState;       // state of the cursor
  int curFlags;       // cursor flags (BTCF_AtLast checked)
  int iPage;          // number of pages in the cursor's current path (simplified)
  int ix;             // index of the current cell
  MemPage *pPage;      // pointer to current MemPage
  MemPage **apPage;     // array of MemPage pointers along the path
  int *aiIdx;           // per-page cell index array
  Btree *pBtree;        // associated B-tree
  BtShared *pBt;         // shared cache pointer (unused in tests)
};

// Global test failure collector (non-terminating)
static vector<string> gFailures;

// Helper macro for non-terminating expectations
#define EXPECT_TRUE(cond, desc) do { \
    if(!(cond)) { gFailures.push_back(string("EXPECT_TRUE failed: ") + (desc)); } \
} while(0)


// Forward declaration of the focal function (as in the provided code)
int sqlite3BtreeLast(BtCursor *pCur, int *pRes);

// Stubbed btreeLast function used by sqlite3BtreeLast when not at last
// We implement a simple deterministic behavior to validate integration.
static int btreeLast(BtCursor *pCur, int *pRes) {
  // For testing, set a known value so we can verify propagation
  *pRes = 12345;
  return SQLITE_OK;
}

// Implementation of the focal method sqlite3BtreeLast copied to a self-contained unit
int sqlite3BtreeLast(BtCursor *pCur, int *pRes){
  // In real SQLite, this would be an assertion; in our test harness, we keep behavior deterministic.
  assert( cursorOwnsBtShared(pCur) );
  assert( sqlite3_mutex_held(pCur->pBtree->db->mutex) );
  /* If the cursor already points to the last entry, this is a no-op. */
  if( CURSOR_VALID==pCur->eState && (pCur->curFlags & BTCF_AtLast)!=0 ){
#ifdef SQLITE_DEBUG
    int ii;
    for(ii=0; ii<pCur->iPage; ii++){
      assert( pCur->aiIdx[ii]==pCur->apPage[ii]->nCell );
    }
    assert( pCur->ix==pCur->pPage->nCell-1 || CORRUPT_DB );
    testcase( pCur->ix!=pCur->pPage->nCell-1 );
    assert( pCur->pPage->leaf );
#endif
    *pRes = 0;
    return SQLITE_OK;
  }
  return btreeLast(pCur, pRes);
}


// ------------------------------------------------------------
// Test Harness Utilities
// ------------------------------------------------------------

// Helper to create a simple BtCursor with minimal fields initialized
static BtCursor makeCursorAt(
    int eState,
    int curFlags,
    int iPage,
    int ix
) {
  static int dummyIdxBuf[128];
  static MemPage dummyPageBuf;
  static MemPage *ppages[128] = { nullptr };

  BtCursor c;
  c.eState = eState;
  c.curFlags = curFlags;
  c.iPage = iPage;
  c.ix = ix;
  c.pPage = &dummyPageBuf;
  c.apPage = ppages;
  c.aiIdx = dummyIdxBuf;
  // Link a minimal BtShared/Btree with a dummy mutex
  static Db db;
  static Btree bt;
  static BtShared bs;
  static void* dummyMutex = nullptr;
  db.mutex = dummyMutex; // pointer value (not used for actual locking in tests)
  bt.db = &db;
  c.pBtree = &bt;
  c.pBt = &bs;
  // Tiny page initialization to satisfy potential future checks
  dummyPageBuf.nCell = 1;
  dummyPageBuf.leaf = 1;
  return c;
}

// Helper to clear collected failures and report result
static void reportResults() {
  if(gFailures.empty()) {
    cout << "ALL TESTS PASSED" << endl;
  } else {
    cout << gFailures.size() << " TEST(S) FAILED" << endl;
    for(const string& s : gFailures) {
      cout << "  - " << s << endl;
    }
  }
}

// ------------------------------------------------------------
// Unit Tests
// ------------------------------------------------------------

// Test 1: When the cursor is already at the last entry (AtLast flag set), sqlite3BtreeLast should be a no-op
// and set the result to 0 with SQLITE_OK.
static void test_no_op_last_at_last() {
  // Arrange
  gFailures.clear();

  // Prepare a cursor that is CURSOR_VALID and AtLast is set
  BtCursor cur = makeCursorAt(CURSOR_VALID, BTCF_AtLast, 3, 2);

  int res = -1;
  // Act
  int rc = sqlite3BtreeLast(&cur, &res);

  // Assert: should return SQLITE_OK and set *pRes to 0
  EXPECT_TRUE(rc == SQLITE_OK, "sqlite3BtreeLast should return SQLITE_OK for AtLast path");
  EXPECT_TRUE(res == 0, "sqlite3BtreeLast should set res = 0 when AtLast flag is set on a valid cursor");

  // Clean-up: none allocated dynamically in this simplified test
}

// Test 2: When the cursor is not at the last entry, sqlite3BtreeLast should delegate to btreeLast
// and propagate its result (12345 in this test).
static void test_calls_btreeLast_when_not_at_last() {
  // Arrange
  gFailures.clear();

  // Prepare a cursor that is NOT CURSOR_VALID or AtLast flag not set
  BtCursor cur = makeCursorAt(0 /* not CURSOR_VALID */, 0 /* no AtLast flag */, 0, 0);

  int res = -1;
  // Act
  int rc = sqlite3BtreeLast(&cur, &res);

  // Assert: should return SQLITE_OK and res equals 12345 (from btreeLast stub)
  EXPECT_TRUE(rc == SQLITE_OK, "sqlite3BtreeLast should return SQLITE_OK when delegating to btreeLast");
  EXPECT_TRUE(res == 12345, "sqlite3BtreeLast should propagate the value set by btreeLast (12345)");
}

// Test 3: When the cursor is CURSOR_VALID but AtLast flag is not set, ensure path falls back to btreeLast
static void test_last_path_without_atlast_bit() {
  // Arrange
  gFailures.clear();

  // Cursor in valid state but lacking AtLast bit
  BtCursor cur = makeCursorAt(CURSOR_VALID, 0 /* AtLast not set */, 1, 0);

  int res = -1;
  int rc = sqlite3BtreeLast(&cur, &res);

  EXPECT_TRUE(rc == SQLITE_OK, "sqlite3BtreeLast should return SQLITE_OK when AtLast bit is not set");
  EXPECT_TRUE(res == 12345, "sqlite3BtreeLast should delegate to btreeLast and return its value (12345)");
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main() {
  // Run tests
  test_no_op_last_at_last();
  test_calls_btreeLast_when_not_at_last();
  test_last_path_without_atlast_bit();

  // Report results
  reportResults();

  // Return failure count as program exit status (0 for success)
  return gFailures.empty() ? 0 : 1;
}