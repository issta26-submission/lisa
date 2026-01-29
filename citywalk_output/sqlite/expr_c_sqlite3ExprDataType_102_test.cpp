/*
  C++11 test harness for the focal SQLite function:
  int sqlite3ExprDataType(const Expr *pExpr)

  Notes:
  - This test harness provides lightweight, self-contained stubs
    for the Expr and ExprList structures that mirror the minimal
    layout required by sqlite3ExprDataType.
  - It also provides a small, controllable sqlite3ExprAffinity stub so
    we can drive the TK_COLUMN/affinity branches and test multiple
    outcomes without requiring the full SQLite runtime.
  - We exercise true/false branches of key predicates, including:
    - TK_STRING, TK_BLOB, TK_CONCAT, TK_VARIABLE/TK_FUNCTION/TK_AGG_FUNCTION
    - TK_COLUMN family with numeric/text/other affinities
    - TK_COLLATE and TK_UPLUS chaining to reach inner expressions
    - TK_NULL end condition
    - TK_CASE with an ExprList containing inner expressions
    - An unknown op (default 0x01)
  - Static linkage: sqlite3ExprAffinity and sqlite3ExprDataType are assumed
    to be provided by the linked library; we override sqlite3ExprAffinity to
    enable deterministic test control.
  - No GTest; a small in-house test harness prints PASS/FAIL per test.
*/

#include <iostream>
#include <sqliteInt.h>
#include <vector>


// Forward declare the focal function from the library (C linkage)
extern "C" int sqlite3ExprDataType(const struct Expr *pExpr);

// Lightweight stand-ins for the library's expressions and lists.
// We mirror only the pieces needed by sqlite3ExprDataType.

enum {
  TK_COLLATE = 1,
  TK_IF_NULL_ROW = 2,
  TK_UPLUS = 3,
  TK_NULL = 4,
  TK_STRING = 5,
  TK_BLOB = 6,
  TK_CONCAT = 7,
  TK_VARIABLE = 8,
  TK_AGG_FUNCTION = 9,
  TK_FUNCTION = 10,
  TK_COLUMN = 11,
  TK_AGG_COLUMN = 12,
  TK_SELECT = 13,
  TK_CAST = 14,
  TK_SELECT_COLUMN = 15,
  TK_VECTOR = 16,
  TK_CASE = 17
};

// Affinity constants (subset sufficient for tests)
enum {
  SQLITE_AFF_TEXT = 1,
  SQLITE_AFF_NUMERIC = 2
};

// Forward declare; the test harness defines a minimal Expr layout.
struct ExprList;

// Minimal Expr layout compatible with sqlite3ExprDataType usage
struct Expr {
  int op;
  Expr *pLeft;
  struct {
    ExprList *pList;
  } x;
  int affinityOverride; // for test control of sqlite3ExprAffinity
};

// ExprList with a dynamic array-like structure.
// We purposely keep it simple: nExpr and an array-like member 'a' of items
// where each item has a member pExpr.
struct ExprList {
  int nExpr;
  struct ExprListItem {
    Expr *pExpr;
  } *a;
};

// Helper to create simple Expr nodes with standardized initialization
static Expr* makeExpr(int op, Expr* left = nullptr) {
  Expr* e = new Expr;
  e->op = op;
  e->pLeft = left;
  e->x.pList = nullptr;
  e->affinityOverride = -1; // -1 means "no override"
  return e;
}

// Stub for sqlite3ExprAffinity used by sqlite3ExprDataType's TK_COLUMN family.
// We expose it with C linkage so the linker resolves it when sqlite3ExprDataType calls it.
extern "C" int sqlite3ExprAffinity(const Expr *pExpr) {
  if (!pExpr) return 0;
  if (pExpr->affinityOverride != -1) return pExpr->affinityOverride;

  switch (pExpr->op) {
    // These op kinds should exercise the numeric affinity path
    case TK_COLUMN:
    case TK_AGG_COLUMN:
    case TK_SELECT:
    case TK_CAST:
    case TK_SELECT_COLUMN:
    case TK_VECTOR:
      return SQLITE_AFF_NUMERIC;
    // Default: treat as no specific affinity (return 0 -> 0x07 in type func)
    default:
      return 0;
  }
  return 0;
}

// Simple test harness utilities
static void testLog(const std::string &name, const std::string &msg) {
  std::cout << "[TEST] " << name << ": " << msg << std::endl;
}
static void assertEq(const std::string &name, int actual, int expected) {
  if (actual == expected) {
    testLog(name, "PASS");
  } else {
    std::cout << "[FAIL] " << name
              << " - expected: 0x" << std::hex << (unsigned)expected
              << ", got: 0x" << std::hex << (unsigned)actual
              << std::dec << std::endl;
  }
}

// Test 1: TK_STRING should return 0x02
static void test_string_returns_0x02() {
  Expr *e = makeExpr(TK_STRING);
  int v = sqlite3ExprDataType(e);
  assertEq("TK_STRING should yield 0x02", v, 0x02);
  delete e;
}

// Test 2: TK_BLOB should return 0x04
static void test_blob_returns_0x04() {
  Expr *e = makeExpr(TK_BLOB);
  int v = sqlite3ExprDataType(e);
  assertEq("TK_BLOB should yield 0x04", v, 0x04);
  delete e;
}

// Test 3: TK_CONCAT should return 0x06
static void test_concat_returns_0x06() {
  Expr *e = makeExpr(TK_CONCAT);
  int v = sqlite3ExprDataType(e);
  assertEq("TK_CONCAT should yield 0x06", v, 0x06);
  delete e;
}

// Test 4: TK_VARIABLE (and similar) should return 0x07
static void test_variable_function_or_aggr_returns_0x07() {
  Expr *e = makeExpr(TK_VARIABLE);
  int v = sqlite3ExprDataType(e);
  assertEq("TK_VARIABLE should yield 0x07", v, 0x07);
  delete e;
}

// Test 5-7: TK_COLUMN with various affinities
static void test_column_affinity_numeric_returns_0x05() {
  Expr *e = makeExpr(TK_COLUMN);
  e->affinityOverride = -1; // ensure default behavior
  int v = sqlite3ExprDataType(e);
  assertEq("TK_COLUMN with numeric affinity should yield 0x05", v, 0x05);
  delete e;
}

static void test_column_affinity_text_returns_0x06() {
  Expr *e = makeExpr(TK_COLUMN);
  e->affinityOverride = SQLITE_AFF_TEXT; // force TEXT affinity
  int v = sqlite3ExprDataType(e);
  assertEq("TK_COLUMN with TEXT affinity should yield 0x06", v, 0x06);
  delete e;
}

static void test_column_affinity_other_returns_0x07() {
  Expr *e = makeExpr(TK_COLUMN);
  e->affinityOverride = 0; // not numeric, not text
  int v = sqlite3ExprDataType(e);
  assertEq("TK_COLUMN with other affinity should yield 0x07", v, 0x07);
  delete e;
}

// Test 8: TK_NULL should end with 0x00
static void test_null_returns_0x00() {
  Expr *e = makeExpr(TK_NULL);
  int v = sqlite3ExprDataType(e);
  assertEq("TK_NULL should yield 0x00", v, 0x00);
  delete e;
}

// Test 9: TK_COLLATE wrapping around another expression
static void test_collate_wraps_and_propagates_inner() {
  Expr *inner = makeExpr(TK_STRING);
  Expr *e = makeExpr(TK_COLLATE, inner);
  int v = sqlite3ExprDataType(e);
  assertEq("TK_COLLATE wrapping STRING should yield 0x02", v, 0x02);
  delete inner;
  delete e;
}

// Test 10: TK_CASE with a simple two-expression case list
static void test_case_expression_combines_inner_types() {
  // Build inner expressions: [1] -> STRING (0x02), [2] -> BLOB (0x04)
  Expr *caseInner1 = makeExpr(TK_STRING);
  Expr *caseInner2 = makeExpr(TK_BLOB);

  ExprList *plist = new ExprList;
  plist->nExpr = 3; // as per sqlite3ExprDataType a[1] and a[2] are used
  plist->a = new ExprList::ExprListItem[3];
  plist->a[0].pExpr = nullptr;
  plist->a[1].pExpr = caseInner1;
  plist->a[2].pExpr = caseInner2;

  Expr *caseExpr = makeExpr(TK_CASE);
  caseExpr->x.pList = plist;

  int v = sqlite3ExprDataType(caseExpr);
  // 0x02 | 0x04 => 0x06
  assertEq("TK_CASE with STRING and BLOB should yield 0x06", v, 0x06);

  delete caseInner1;
  delete caseInner2;
  delete plist->a;
  delete plist;
  delete caseExpr;
}

// Test 11: Unknown op should return 0x01
static void test_unknown_op_returns_0x01() {
  Expr *e = makeExpr(999);
  int v = sqlite3ExprDataType(e);
  assertEq("Unknown op should yield 0x01", v, 0x01);
  delete e;
}

// Test 12: TK_UPLUS wrapping around STRING yields same as inner
static void test_upplus_wraps_to_string() {
  Expr *inner = makeExpr(TK_STRING);
  Expr *outer = makeExpr(TK_UPLUS, inner);
  int v = sqlite3ExprDataType(outer);
  assertEq("TK_UPLUS wrapping STRING should yield 0x02", v, 0x02);
  delete inner;
  delete outer;
}

// Test 13: TK_CASE with odd nExpr triggers last element case path
static void test_case_odd_nexpr_considers_last() {
  // Build inner: only one element (nExpr must be >0 per asserts)
  Expr *innerLast = makeExpr(TK_STRING);

  ExprList *plist = new ExprList;
  plist->nExpr = 1; // odd
  plist->a = new ExprList::ExprListItem[1];
  plist->a[0].pExpr = innerLast; // index 0 is not used in loop, but used for last element

  Expr *caseExpr = makeExpr(TK_CASE);
  caseExpr->x.pList = plist;

  int v = sqlite3ExprDataType(caseExpr);
  // Since there is no a[1] or a[2], the loop won't access, but the code asserts nExpr > 0 and then
  // would return the combined value of innerLast; in absence of a[1], the loop won't modify res, and since
  // the last element exists due to nExpr%2 (1%2=1), it will access a[pList->nExpr-1] => a[0], which is innerLast (STRING)
  // So expectation is 0x02
  assertEq("TK_CASE with single STRING last element yields 0x02", v, 0x02);

  delete innerLast;
  delete plist->a;
  delete plist;
  delete caseExpr;
}

// Entry point
int main() {
  std::cout << "Starting sqlite3ExprDataType unit tests (C++11 harness)\n";

  test_string_returns_0x02();
  test_blob_returns_0x04();
  test_concat_returns_0x06();
  test_variable_function_or_aggr_returns_0x07();
  test_column_affinity_numeric_returns_0x05();
  test_column_affinity_text_returns_0x06();
  test_column_affinity_other_returns_0x07();
  test_null_returns_0x00();
  test_collate_wraps_and_propagates_inner();
  test_case_expression_combines_inner_types();
  test_unknown_op_returns_0x01();
  test_upplus_wraps_to_string();
  test_case_odd_nexpr_considers_last();

  std::cout << "sqlite3ExprDataType unit tests completed.\n";
  return 0;
}