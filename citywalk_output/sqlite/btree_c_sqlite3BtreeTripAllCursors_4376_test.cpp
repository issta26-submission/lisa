// Test suite for sqlite3BtreeTripAllCursors (btree.c) using a minimal integration
// Demonstrates basic call paths and verifies edge-case handling without requiring
// full SQLite runtime initialization. This harness relies on the actual
// Btree/BtCursor structures and constants defined in the project's btreeInt.h.
// The tests are designed to be non-terminating (no aborts on failures) and report
// results via standard output.

#include <btreeInt.h>
#include <iostream>


// Include project-specific internal definitions to construct a minimal Btree/BtCursor
// and access constants like CURSOR_VALID, CURSOR_FAULT, BTCF_WriteFlag, etc.

// Declare the focal C function to be tested (C linkage)
extern "C" int sqlite3BtreeTripAllCursors(Btree *pBtree, int errCode, int writeOnly);

// A tiny, self-contained helper to print test results
static inline void log_test_result(const char* test_name, bool passed) {
  std::cout << "[TEST] " << test_name << " - " << (passed ? "PASSED" : "FAILED") << std::endl;
}

// Test 1: Call with a NULL Btree pointer should be safely handled and return SQLITE_OK (0)
bool test_null_btree_pointer() {
  // The focal function should simply return without doing work when pBtree is NULL.
  int rc = sqlite3BtreeTripAllCursors(nullptr, 0, 0);
  bool ok = (rc == SQLITE_OK);
  if(!ok) {
    std::cerr << "Error: sqlite3BtreeTripAllCursors(NULL,0,0) returned rc=" << rc << std::endl;
  }
  return ok;
}

// Test 2: Call with a Btree that has no cursors (pCursor == NULL) should be a no-op
bool test_btree_with_no_cursors() {
  // Build a minimal Btree structure with an empty cursor list.
  BtShared shared;
  shared.pCursor = nullptr;

  Btree btree;
  btree.pBt = &shared;

  int rc = sqlite3BtreeTripAllCursors(&btree, 0, 0);
  bool ok = (rc == SQLITE_OK);
  if(!ok) {
    std::cerr << "Error: sqlite3BtreeTripAllCursors(no cursors) returned rc=" << rc << std::endl;
  }
  return ok;
}

// Test 3: Btree with a single cursor; writeOnly = 0 should trigger the "clear cursor" path
// and update the cursor state to CURSOR_FAULT with skipNext set to errCode.
bool test_single_cursor_clear_path() {
  // Create a single BtCursor and chain it (pNext == NULL)
  BtCursor cursor;
  cursor.pNext = nullptr;
  cursor.curFlags = 0;                 // No BTCF_WriteFlag set (writeable bit not set)
  cursor.eState = CURSOR_VALID;        // Cursor initially valid
  cursor.skipNext = 0;                 // Will be overwritten by function

  // Compose BtShared and Btree with this one cursor
  BtShared shared;
  shared.pCursor = &cursor;

  Btree btree;
  btree.pBt = &shared;

  int errCode = 42;
  int rc = sqlite3BtreeTripAllCursors(&btree, errCode, 0); // writeOnly = 0 (non-writeOnly path)

  // After function:
  // - rc should remain SQLITE_OK (0)
  // - cursor.eState should be CURSOR_FAULT
  // - cursor.skipNext should be equal to errCode
  bool ok = (rc == SQLITE_OK) &&
            (cursor.eState == CURSOR_FAULT) &&
            (cursor.skipNext == errCode);

  if(!ok) {
    std::cerr << "Error: test_single_cursor_clear_path failed. rc=" << rc
              << " eState=" << cursor.eState
              << " skipNext=" << cursor.skipNext << std::endl;
  }
  return ok;
}

// Main test runner
int main() {
  int total = 0;
  int passed = 0;

  // Test 1
  {
    const char* test_name = "test_null_btree_pointer";
    bool ok = test_null_btree_pointer();
    log_test_result(test_name, ok);
    total++;
    if(ok) passed++;
  }

  // Test 2
  {
    const char* test_name = "test_btree_with_no_cursors";
    bool ok = test_btree_with_no_cursors();
    log_test_result(test_name, ok);
    total++;
    if(ok) passed++;
  }

  // Test 3
  {
    const char* test_name = "test_single_cursor_clear_path";
    bool ok = test_single_cursor_clear_path();
    log_test_result(test_name, ok);
    total++;
    if(ok) passed++;
  }

  std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

  return (passed == total) ? 0 : 1;
}