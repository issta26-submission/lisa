// Unit test suite for the focal method sqlite3VdbeHandleMovedCursor
// This test is crafted to run in a C++11 environment without GTest.
// It uses minimal stubs and a small in-process test harness to exercise the
// core logic and branches of the function described in <FOCAL_METHOD>.

#include <cstdio>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Domain-specific constants and types (minimal stubs to enable compilation)
static const int CURTYPE_BTREE = 1;
static const int CACHE_STALE  = 2;  // Value chosen for testing; mirrors SQLite's semantics in this mock

// Forward declaration of the testable focal function and its dependencies
// We implement a self-contained version here to enable isolated unit testing
// without requiring a full SQLite build.
struct VdbeCursor {
  int eCurType;
  struct {
    void *pCursor;
  } uc;
  int cacheStatus;
  int nullRow;
};

// Mocked dependency behavior controlled by test cases
static int g_BtreeCursorHasMoved = 1;
static int g_BtreeCursorRestore_isDifferentRow = 0;
static int g_BtreeCursorRestore_rc = 0;

// Mocked sqlite3BtreeCursorHasMoved(void*)
extern "C" int sqlite3BtreeCursorHasMoved(void *pCursor){
  (void)pCursor; // parameter unused in mock
  // Return value controlled by test setup (global)
  return g_BtreeCursorHasMoved;
}

// Mocked sqlite3BtreeCursorRestore(void*, int*)
extern "C" int sqlite3BtreeCursorRestore(void *pCursor, int *pIsDifferentRow){
  (void)pCursor; // parameter unused in mock
  if (pIsDifferentRow) *pIsDifferentRow = g_BtreeCursorRestore_isDifferentRow;
  return g_BtreeCursorRestore_rc;
}

// The focal method under test, implemented with C linkage to mimic the original file's linkage
// This mirrors the logic exactly as provided in <FOCAL_METHOD>.
#ifndef SQLITE_NOINLINE
#define SQLITE_NOINLINE
#endif
extern "C" int SQLITE_NOINLINE sqlite3VdbeHandleMovedCursor(VdbeCursor *p){
  int isDifferentRow, rc;
  assert( p->eCurType==CURTYPE_BTREE );
  assert( p->uc.pCursor!=0 );
  assert( sqlite3BtreeCursorHasMoved(p->uc.pCursor) );
  rc = sqlite3BtreeCursorRestore(p->uc.pCursor, &isDifferentRow);
  p->cacheStatus = CACHE_STALE;
  if( isDifferentRow ) p->nullRow = 1;
  return rc;
}

// Lightweight test assertion helpers (non-terminating)
static int g_failures = 0;
#define EXPECT_EQ(a,b) do { \
  if ((a) != (b)) { \
    fprintf(stderr, "EXPECT_EQ failed: %s (%ld) != %s (%ld) at %s:%d\n", #a, static_cast<long>(a), #b, static_cast<long>(b), __FILE__, __LINE__); \
    ++g_failures; \
  } \
} while(0)

#define TEST_COMMENT(desc) fprintf(stdout, "Test: %s\n", desc)


// Test case 1: isDifferentRow = 0 should not set nullRow; rc should be as returned by Restore
// Preconditions are satisfied: CURTYPE_BTREE, non-null pCursor, and HasMoved returns true.
void test_handleMovedCursor_isDifferentRow_false() {
  TEST_COMMENT("Case 1: isDifferentRow == 0 (no nullRow set, rc returned as Restore)")

  // Configure mock behaviors
  g_BtreeCursorHasMoved = 1;               // has moved (true)
  g_BtreeCursorRestore_isDifferentRow = 0; // isDifferentRow false
  g_BtreeCursorRestore_rc = 7;             // rc to be returned by Restore

  // Prepare VdbeCursor
  VdbeCursor p;
  p.eCurType = CURTYPE_BTREE;
  p.uc.pCursor = reinterpret_cast<void*>(1); // non-null cursor
  p.cacheStatus = 0;
  p.nullRow = 0;

  int rc = sqlite3VdbeHandleMovedCursor(&p);

  // Validate expectations
  EXPECT_EQ(rc, 7);
  EXPECT_EQ(p.nullRow, 0);
  EXPECT_EQ(p.cacheStatus, CACHE_STALE);
}

// Test case 2: isDifferentRow = 1 should set nullRow to 1; rc should be restored value
void test_handleMovedCursor_isDifferentRow_true() {
  TEST_COMMENT("Case 2: isDifferentRow == 1 (expect nullRow to be set, rc from Restore)")

  // Configure mock behaviors
  g_BtreeCursorHasMoved = 1;                 // has moved (true)
  g_BtreeCursorRestore_isDifferentRow = 1;   // isDifferentRow true
  g_BtreeCursorRestore_rc = -3;               // rc to be returned by Restore

  // Prepare VdbeCursor
  VdbeCursor p;
  p.eCurType = CURTYPE_BTREE;
  p.uc.pCursor = reinterpret_cast<void*>(2); // non-null cursor
  p.cacheStatus = 0;
  p.nullRow = 0;

  int rc = sqlite3VdbeHandleMovedCursor(&p);

  // Validate expectations
  EXPECT_EQ(rc, -3);
  EXPECT_EQ(p.nullRow, 1);
  EXPECT_EQ(p.cacheStatus, CACHE_STALE);
}

// Entry point: run all tests and report summary
int main() {
  fprintf(stdout, "Running sqlite3VdbeHandleMovedCursor unit tests (mocked environment)\n");

  test_handleMovedCursor_isDifferentRow_false();
  test_handleMovedCursor_isDifferentRow_true();

  if (g_failures == 0) {
    fprintf(stdout, "ALL TESTS PASSED\n");
  } else {
    fprintf(stderr, "SOME TESTS FAILED: %d failure(s)\n", g_failures);
  }

  return g_failures;
}