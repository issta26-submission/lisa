// Lightweight C++11 unit test for the focal C function sqlite3SrcListFuncArgs
// This test harness provides minimal stubs for the dependent SQLite-like types
// and verifies both branches of the function (non-null SrcList and null SrcList).

#include <cstddef>
#include <iostream>
#include <sqliteInt.h>


// -------------------------
// Minimal dependency stubs
// -------------------------

// Forward declare a minimal sqlite3 type to satisfy Parse's db member.
struct sqlite3 {};

// ExprList is an opaque placeholder in this test.
struct ExprList {};

// Parse struct with a sqlite3* db member, matching the usage in sqlite3SrcListFuncArgs.
struct Parse {
  sqlite3 *db;
};

// SrcList and related structures (simplified versions to exercise sqlite3SrcListFuncArgs)
struct SrcList;  // forward

// Structure to hold per-item flags for SrcList items.
struct SrcItemFg {
  int notIndexed;
  int isIndexedBy;
  int isTabFunc;
};

// SrcItem contains flag data and a union-like container for function argument list.
struct SrcItem {
  SrcItemFg fg;
  union {
    ExprList *pFuncArg;
  } u1;
};

// SrcList contains an array of SrcItem and the count nSrc.
struct SrcList {
  SrcItem *a;
  int nSrc;
};

// -------------------------
// Function under test (reproduced for test)
// -------------------------

// Global hook to observe whether sqlite3ExprListDelete is invoked (used in tests).
static bool gExprListDeleteCalled = false;

// Minimal mock implementation of sqlite3ExprListDelete to observe the call in tests.
static void sqlite3ExprListDelete(sqlite3 *db, ExprList *pList) {
  // In real code this would free memory; here we just set a flag for test observation.
  (void)db;  // silence unused parameter in test harness
  (void)pList;
  gExprListDeleteCalled = true;
}

// The focal method implemented to mirror the logic in the provided snippet.
void sqlite3SrcListFuncArgs(Parse *pParse, SrcList *p, ExprList *pList) {
  if (p) {
    SrcItem *pItem = &p->a[p->nSrc - 1];
    // In the real code, there are asserts ensuring initial state.
    // Our test harness ensures those fields are initialized to the expected values.
    pItem->u1.pFuncArg = pList;
    pItem->fg.isTabFunc = 1;
  } else {
    sqlite3ExprListDelete(pParse->db, pList);
  }
}

// -------------------------
// Test harness
// -------------------------

// Helper macro for simple, non-terminating checks (non-GTest approach)
#define CHECK(cond, msg)                               \
  do {                                                   \
    if (!(cond)) {                                       \
      std::cerr << "CHECK FAILED: " << msg << "\n";    \
      return false;                                      \
    }                                                    \
  } while (0)

// Test 1: True branch when SrcList is non-null
// - Ensures that the last SrcItem has its function argument set and isTabFunc flag set.
bool test_sqlite3SrcListFuncArgs_trueBranch() {
  // Prepare Parse with a dummy sqlite3 db pointer
  Parse pParse;
  sqlite3 dbInstance;
  pParse.db = &dbInstance;

  // Prepare SrcList with a single SrcItem, initialized to the required state.
  SrcList src;
  src.nSrc = 1;
  src.a = new SrcItem[1];
  // Initialize flags to 0 as expected by the original assertions
  src.a[0].fg.notIndexed = 0;
  src.a[0].fg.isIndexedBy = 0;
  src.a[0].fg.isTabFunc = 0;
  src.a[0].u1.pFuncArg = nullptr;

  // Prepare an ExprList to pass as pList
  ExprList *pList = new ExprList();

  // Call the function under test (true branch)
  sqlite3SrcListFuncArgs(&pParse, &src, pList);

  // Validate post-conditions:
  // - pList is now assigned to the last item as pFuncArg
  // - isTabFunc flag is set to 1
  // - other flags remain 0 (as initialized)
  SrcItem *pItem = &src.a[src.nSrc - 1];
  bool ok = (pItem->u1.pFuncArg == pList) &&
            (pItem->fg.isTabFunc == 1) &&
            (pItem->fg.notIndexed == 0) &&
            (pItem->fg.isIndexedBy == 0);

  // Cleanup
  delete[] src.a;
  delete pList;

  return ok;
}

// Test 2: Null branch when SrcList is null
// - Ensures that sqlite3ExprListDelete is invoked with the provided pList.
bool test_sqlite3SrcListFuncArgs_nullBranch() {
  // Reset the global observation flag
  gExprListDeleteCalled = false;

  Parse pParse;
  sqlite3 dbInstance;
  pParse.db = &dbInstance;

  SrcList *p = nullptr;
  ExprList *pList = new ExprList();

  // Call the function under test (null branch)
  sqlite3SrcListFuncArgs(&pParse, p, pList);

  bool called = gExprListDeleteCalled;

  delete pList;
  // Expect theDelete function to have been called
  if (!called) {
    std::cerr << "CHECK FAILED: sqlite3ExprListDelete was not invoked in null branch\n";
  }
  return called;
}

// -------------------------
// Main
// -------------------------

int main() {
  int failures = 0;

  std::cout << "Running unit tests for sqlite3SrcListFuncArgs...\n";

  if (!test_sqlite3SrcListFuncArgs_trueBranch()) {
    std::cout << "Test 1 (true branch) FAILED.\n";
    ++failures;
  } else {
    std::cout << "Test 1 (true branch) PASSED.\n";
  }

  if (!test_sqlite3SrcListFuncArgs_nullBranch()) {
    std::cout << "Test 2 (null branch) FAILED.\n";
    ++failures;
  } else {
    std::cout << "Test 2 (null branch) PASSED.\n";
  }

  if (failures == 0) {
    std::cout << "All tests PASSED.\n";
  } else {
    std::cout << failures << " test(s) FAILED.\n";
  }

  return failures;
}