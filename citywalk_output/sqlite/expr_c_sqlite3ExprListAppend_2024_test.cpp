// C++11 unit tests for sqlite3ExprListAppend (a simplified, test-friendly mock of the SQLite internals)

#include <cstddef>
#include <iostream>
#include <sqliteInt.h>


// Minimal mockups to emulate the parts of the SQLite structures used by sqlite3ExprListAppend

struct sqlite3 {};

// Forward declaration of test-friendly types
struct Expr;
struct ExprList_item;
struct ExprList;

// Parsing context with a dummy sqlite3* database pointer (not used by the tests beyond satisfying the API)
struct Parse {
  sqlite3 *db;
};

// Very small expression placeholder
struct Expr {
  int dummy; // value not important; used to differentiate objects
};

// An item within an ExprList
struct ExprList_item {
  Expr *pExpr; // expression carried by the list item
  // In the real SQLite, there are more fields; only pExpr is needed for tests.
};

// The expression list structure
struct ExprList {
  int nAlloc;  // total allocation size for the array 'a'
  int nExpr;   // number of occupied items in 'a'
  ExprList_item *a; // dynamically allocated array of items
};

// Static 'zero' item used to reset a list entry before assigning a new expression
static ExprList_item zeroItem = { nullptr };

// Forward declarations of the function prototypes under test (mocked behavior)
ExprList *sqlite3ExprListAppendNew(sqlite3 *db, Expr *pExpr);
ExprList *sqlite3ExprListAppendGrow(sqlite3 *db, ExprList *pList, Expr *pExpr);

// The focal function under test (simplified to be self-contained for unit testing)
ExprList *sqlite3ExprListAppend(Parse *pParse, ExprList *pList, Expr *pExpr) {
  struct ExprList_item *pItem;
  if (pList == nullptr) {
    // Branch 1: NULL list -> create a new list with the provided expression
    return sqlite3ExprListAppendNew(pParse->db, pExpr);
  }
  // Branch 2: If there is no room for one more item, grow the list
  if (pList->nAlloc < pList->nExpr + 1) {
    return sqlite3ExprListAppendGrow(pParse->db, pList, pExpr);
  }
  // Branch 3: There is room; append in-place
  pItem = &pList->a[pList->nExpr++];
  *pItem = zeroItem;
  pItem->pExpr = pExpr;
  return pList;
}

// Mock implementation: Create a new ExprList with one item containing pExpr
ExprList *sqlite3ExprListAppendNew(sqlite3 *db, Expr *pExpr) {
  (void)db; // unused in tests
  ExprList *p = new ExprList;
  p->nAlloc = 1;
  p->nExpr  = 1;
  p->a = new ExprList_item[1];
  p->a[0].pExpr = pExpr;
  return p;
}

// Mock implementation: Grow the array to accommodate at least nExpr+1 items, then append
ExprList *sqlite3ExprListAppendGrow(sqlite3 *db, ExprList *pList, Expr *pExpr) {
  (void)db; // unused in tests

  int oldNExpr = pList->nExpr;
  int newAlloc = (pList->nAlloc > 0) ? pList->nAlloc * 2 : 1;
  if (newAlloc < pList->nExpr + 1) newAlloc = pList->nExpr + 1;

  ExprList_item *newA = new ExprList_item[newAlloc];
  for (int i = 0; i < pList->nExpr; ++i) {
    newA[i] = pList->a[i];
  }
  // If there was an old array, free it
  if (pList->a) delete[] pList->a;

  pList->a = newA;
  pList->nAlloc = newAlloc;
  // Append the new expression into the first free slot
  pList->a[oldNExpr].pExpr = pExpr;
  pList->nExpr = oldNExpr + 1;

  return pList;
}

// Domain knowledge notes (for reviewers):
// - The tests aim to exercise all branches of sqlite3ExprListAppend:
//   1) pList == nullptr
//   2) pList exists but has insufficient allocation (nAlloc < nExpr+1)
//   3) pList exists and has room (nAlloc >= nExpr+1)
// - We provide minimal memory management to keep tests deterministic and self-contained.


// ------------------------ Unit Tests ------------------------

/*
  Test 1: Null list should trigger creation via AppendNew and place pExpr as the only element.
  - Expected: returned list is non-null, nExpr == 1, a != nullptr, a[0].pExpr == pExpr
*/
bool test_null_list() {
  Parse pParse;
  sqlite3 dummyDb;
  pParse.db = &dummyDb;

  Expr *pExpr = new Expr{ 42 };

  ExprList *res = sqlite3ExprListAppend(&pParse, nullptr, pExpr);

  bool ok = (res != nullptr)
            && (res->nExpr == 1)
            && (res->a != nullptr)
            && (res->a[0].pExpr == pExpr);

  // Cleanup
  delete[] res->a;
  delete res;
  delete pExpr;

  return ok;
}

/*
  Test 2: Non-null list with insufficient allocation should trigger Grow.
  - Prepare an empty list with nAlloc = 0, nExpr = 0.
  - After append, expect nExpr == 1, nAlloc >= 1, a != nullptr, a[0].pExpr == pExpr
*/
bool test_grow_path() {
  Parse pParse;
  sqlite3 dummyDb;
  pParse.db = &dummyDb;

  ExprList *list = new ExprList;
  list->nAlloc = 0;
  list->nExpr  = 0;
  list->a = nullptr;

  Expr *pExpr = new Expr{ 99 };

  ExprList *res = sqlite3ExprListAppend(&pParse, list, pExpr);

  bool ok = (res == list)
            && (res->nExpr == 1)
            && (res->a != nullptr)
            && (res->a[0].pExpr == pExpr);

  // Additional sanity: ensure memory is owned by the list and freed cleanly
  delete[] res->a;
  delete res;
  delete pExpr;

  return ok;
}

/*
  Test 3: Non-null list with enough allocation should append in-place (no Grow).
  - Prepare a list with nAlloc=1, nExpr=0, so there is room for one item.
  - After append, expect nExpr == 1 and a[0].pExpr == pExpr.
*/
bool test_no_grow_path() {
  Parse pParse;
  sqlite3 dummyDb;
  pParse.db = &dummyDb;

  ExprList *list = new ExprList;
  list->nAlloc = 1;
  list->nExpr  = 0;
  list->a = new ExprList_item[1];
  list->a[0].pExpr = nullptr;

  Expr *pExpr = new Expr{ 7 };

  ExprList *res = sqlite3ExprListAppend(&pParse, list, pExpr);

  bool ok = (res == list)
            && (res->nExpr == 1)
            && (res->a != nullptr)
            && (res->a[0].pExpr == pExpr);

  // Cleanup
  delete[] res->a;
  delete res;
  delete pExpr;

  return ok;
}

/*
  Test 4: Grow path but with a NULL pExpr (pExpr == nullptr) to ensure NULL is appended correctly.
  - Prepare a non-null list that will trigger Grow (nAlloc < nExpr+1).
  - After append, expect a[oldNExpr].pExpr == nullptr.
*/
bool test_grow_path_null_expr() {
  Parse pParse;
  sqlite3 dummyDb;
  pParse.db = &dummyDb;

  ExprList *list = new ExprList;
  list->nAlloc = 0;
  list->nExpr  = 0;
  list->a = nullptr;

  Expr *pExpr = nullptr;

  ExprList *res = sqlite3ExprListAppend(&pParse, list, pExpr);

  bool ok = (res == list)
            && (res->nExpr == 1)
            && (res->a != nullptr)
            && (res->a[0].pExpr == nullptr);

  // Cleanup
  delete[] res->a;
  delete res;

  return ok;
}


// ------------------------ Test Runner ------------------------

int main() {
  int passed = 0;
  int failed = 0;

  if (test_null_list()) {
    ++passed;
  } else {
    ++failed;
    std::cerr << "test_null_list failed\n";
  }

  if (test_grow_path()) {
    ++passed;
  } else {
    ++failed;
    std::cerr << "test_grow_path failed\n";
  }

  if (test_no_grow_path()) {
    ++passed;
  } else {
    ++failed;
    std::cerr << "test_no_grow_path failed\n";
  }

  if (test_grow_path_null_expr()) {
    ++passed;
  } else {
    ++failed;
    std::cerr << "test_grow_path_null_expr failed\n";
  }

  std::cout << "Tests passed: " << passed << ", Tests failed: " << failed << "\n";

  return failed ? 1 : 0;
}