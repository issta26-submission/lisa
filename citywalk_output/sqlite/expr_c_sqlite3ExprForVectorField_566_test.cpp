// Test suite for the focal method sqlite3ExprForVectorField
// This test harness provides minimal in-memory scaffolding to exercise the
// logic of sqlite3ExprForVectorField as described in the provided FOCAL_METHOD.
// The aim is to validate both true/false branches of the dependent predicates
// and ensure proper interactions with dependent helpers without relying on GTest.

// This file is self-contained C++11 code and does not require the full SQLite
// source tree. It mocks only the necessary pieces to execute sqlite3ExprForVectorField.

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain knowledge notes (comments for maintainers):
// - We implement a tiny subset of SQLite's Expr structure and related helpers.
// - We expose a global IN_RENAME_OBJECT flag to test both branches inside the
//   TK_VECTOR path (rename-object scenario vs normal).
// - The test covers:
//   1) Path when pVector->op == TK_SELECT
//   2) Path when pVector->op == TK_VECTOR and IN_RENAME_OBJECT == 0 (dup inner)
//   3) Path when pVector->op == TK_VECTOR and IN_RENAME_OBJECT == 1 (rename and return inner)

//////////////////////////////////////////////////////////////////
// Minimal scaffolding to simulate the required SQLite-like types //
//////////////////////////////////////////////////////////////////

// Opcodes (subset)
enum SqliteOp {
  TK_SELECT = 1,
  TK_VECTOR = 2,
  TK_SELECT_COLUMN = 3
};

// Property flags
enum ExprProperty {
  EP_FullSize = 1 << 0
};

// Forward declarations
struct sqlite3;
struct Parse;
struct Expr;
struct ExprList;
struct ExprListElem;

// Lightweight memory space similar to SQLite's
struct sqlite3 {
  int dummy;
};

// Expr list element (inside the vector and expressions)
struct ExprListElem {
  Expr *pExpr;
};

// Expr list
struct ExprList {
  ExprListElem *a; // array of elements
  int nExpr;       // number of elements
};

// Expression (simplified)
struct Expr {
  int op;
  int iTable;
  int iColumn;
  Expr *pLeft;
  Expr *pRight;
  int flags;              // EP_... bit flags
  struct {
    ExprList *pList;       // only used when op == TK_VECTOR
  } x;
};

// Parsing context
struct Parse {
  sqlite3 *db;
};

// Helper: global flag to simulate IN_RENAME_OBJECT behavior
static int IN_RENAME_OBJECT = 0;

// ----------------------------
// Helpers to mimic SQLite API
// ----------------------------

// Create a new Expr with given op; basic fields are initialized.
static Expr* sqlite3ExprNew(int op) {
  Expr* e = new Expr();
  e->op = op;
  e->iTable = 0;
  e->iColumn = 0;
  e->pLeft = nullptr;
  e->pRight = nullptr;
  e->flags = 0;
  e->x.pList = nullptr;
  return e;
}

// Create a Parse object with a dummy sqlite3 db
static Parse* sqlite3ParseNew(sqlite3* db) {
  Parse* p = new Parse();
  p->db = db;
  return p;
}

// Allocate a simple expression "copy"
static Expr* sqlite3ExprDup(sqlite3* db, const Expr* p, int dupFlags);

// Simple PExpr constructor used by sqlite3ExprForVectorField
static Expr* sqlite3PExpr(Parse *pParse, int op, Expr *pLeft, Expr *pRight) {
  // In real SQLite this would allocate and initialize an expression node.
  // Here we construct a minimal one sufficient for tests.
  Expr* e = new Expr();
  e->op = op;
  e->pLeft = pLeft;
  e->pRight = pRight;
  e->iTable = 0;
  e->iColumn = 0;
  e->flags = 0;
  e->x.pList = nullptr;
  return e;
}

// Set a property flag on an expression
static void ExprSetProperty(Expr *p, int mask) {
  if (p) p->flags |= mask;
}

// Predicate helpers (simplified)
static int ExprUseXSelect(const Expr *p) {
  return p ? p->op == TK_SELECT : 0;
}
static int ExprUseXList(const Expr *p) {
  return p ? p->op == TK_VECTOR : 0;
}

// Copy an expression deeply (simplified)
static Expr* sqlite3ExprDup(sqlite3* db, const Expr* p, int dupFlags) {
  if (p == nullptr) return nullptr;
  Expr* q = new Expr();
  q->op = p->op;
  q->iTable = p->iTable;
  q->iColumn = p->iColumn;
  q->flags = p->flags;
  q->pLeft = nullptr;
  q->pRight = nullptr;

  // Deep copy pLeft/pRight if present
  if (p->pLeft) q->pLeft = sqlite3ExprDup(db, p->pLeft, dupFlags);
  if (p->pRight) q->pRight = sqlite3ExprDup(db, p->pRight, dupFlags);

  // Copy vector payload if present
  if (p->x.pList) {
    q->x.pList = new ExprList();
    q->x.pList->nExpr = p->x.pList->nExpr;
    if (p->x.pList->a) {
      q->x.pList->a = new ExprListElem[p->x.pList->nExpr];
      for (int i = 0; i < p->x.pList->nExpr; ++i) {
        q->x.pList->a[i].pExpr = sqlite3ExprDup(db, p->x.pList->a[i].pExpr, dupFlags);
      }
    } else {
      q->x.pList->a = nullptr;
    }
  } else {
    q->x.pList = nullptr;
  }
  return q;
}

// Public alias used by the focal method
static Expr* sqlite3ExprDup_public(sqlite3* db, const Expr* p, int dupFlags) {
  return sqlite3ExprDup(db, p, dupFlags);
}

// Create a vector node with given inner expressions (pExprs[0..n-1])
static Expr* makeVectorWithExprs(Expr** pExprs, int nExpr) {
  Expr* vec = sqlite3ExprNew(TK_VECTOR);
  vec->x.pList = new ExprList();
  vec->x.pList->nExpr = nExpr;
  vec->x.pList->a = new ExprListElem[nExpr];
  for (int i = 0; i < nExpr; ++i) {
    vec->x.pList->a[i].pExpr = pExprs[i];
  }
  return vec;
}

// ----------------------------
// Focal method under test (copied from prompt)
// ----------------------------
static Expr* sqlite3ExprForVectorField(
  Parse *pParse,       /* Parsing context */
  Expr *pVector,       /* The vector.  List of expressions or a sub-SELECT */
  int iField,          /* Which column of the vector to return */
  int nField           /* Total number of columns in the vector */
){
  Expr *pRet;
  if( pVector->op==TK_SELECT ){
    // In tests, ExprUseXSelect is satisfied for TK_SELECT
    assert( ExprUseXSelect(pVector) );
    /* The TK_SELECT_COLUMN Expr node:
    **
    ** pLeft:           pVector containing TK_SELECT.  Not deleted.
    ** pRight:          not used.  But recursively deleted.
    ** iColumn:         Index of a column in pVector
    ** iTable:          0 or the number of columns on the LHS of an assignment
    ** pLeft->iTable:   First in an array of register holding result, or 0
    **                  if the result is not yet computed.
    **
    ** sqlite3ExprDelete() specifically skips the recursive delete of
    ** pLeft on TK_SELECT_COLUMN nodes.  But pRight is followed, so pVector
    ** can be attached to pRight to cause this node to take ownership of
    ** pVector.  Typically there will be multiple TK_SELECT_COLUMN nodes
    ** with the same pLeft pointer to the pVector, but only one of them
    ** will own the pVector.
    */
    pRet = sqlite3PExpr(pParse, TK_SELECT_COLUMN, 0, 0);
    if( pRet ){
      ExprSetProperty(pRet, EP_FullSize);
      pRet->iTable = nField;
      pRet->iColumn = iField;
      pRet->pLeft = pVector;
    }
  }else{
    if( pVector->op==TK_VECTOR ){
      Expr **ppVector;
      assert( ExprUseXList(pVector) );
      ppVector = &pVector->x.pList->a[iField].pExpr;
      pVector = *ppVector;
      if( IN_RENAME_OBJECT ){
        /* This must be a vector UPDATE inside a trigger */
        *ppVector = 0;
        return pVector;
      }
    }
    pRet = sqlite3ExprDup(pParse->db, pVector, 0);
  }
  return pRet;
}

// ----------------------------
// Test framework (very lightweight)
// ----------------------------

// Simple test result container
struct TestResult {
  std::string name;
  bool passed;
  std::string message;
};

// Global test results
static std::vector<TestResult> g_tests;

// Non-terminating assertion helpers
#define TST_ASSERT(cond, name, msg) \
  do { if (!(cond)) { \
    g_tests.push_back({name, false, msg}); \
  } else { \
    g_tests.push_back({name, true, ""}); \
  } \
} while(0)

// Utility: pretty print result summary
static void PrintSummary() {
  int total = (int)g_tests.size();
  int passed = 0;
  for (const auto& t : g_tests) if (t.passed) ++passed;
  std::cerr << "Tests run: " << total << ", Passed: " << passed << ", Failed: " << (total - passed) << "\n";
  for (const auto& t : g_tests) {
    if (!t.passed) {
      std::cerr << "  [FAIL] " << t.name << ": " << t.message << "\n";
    }
  }
}

// Helpers to build expressions used in tests
static Expr* NewLeafExpr(int op) { return sqlite3ExprNew(op); }

// Test 1: TK_SELECT path should create a TK_SELECT_COLUMN node with correct fields
static void test_select_path_sets_fields() {
  // Setup parse context
  sqlite3 db;
  Parse* pParse = sqlite3ParseNew(&db);

  // pVector: TK_SELECT
  Expr* pVector = NewLeafExpr(TK_SELECT); // acts as a TK_SELECT vector
  // nField to put into iTable
  const int iField = 5;
  const int nField = 10;

  // Call focal method
  Expr* pRet = sqlite3ExprForVectorField(pParse, pVector, iField, nField);

  // Assertions
  TST_ASSERT(pRet != nullptr, "test_select_path_sets_fields: pRet != nullptr", "Returned null in select path");
  if (pRet) {
    // op should be TK_SELECT_COLUMN
    TST_ASSERT(pRet->op == TK_SELECT_COLUMN, "test_select_path_sets_fields: op TK_SELECT_COLUMN",
               "Returned node is not TK_SELECT_COLUMN");
    // FullSize flag set
    TST_ASSERT((pRet->flags & EP_FullSize) != 0, "test_select_path_sets_fields: EP_FullSize set",
               "EP_FullSize flag not set on pRet");
    // iTable and iColumn set accordingly
    TST_ASSERT(pRet->iTable == nField, "test_select_path_sets_fields: iTable",
               "pRet->iTable incorrect");
    TST_ASSERT(pRet->iColumn == iField, "test_select_path_sets_fields: iColumn",
               "pRet->iColumn incorrect");
    // pLeft should point to original pVector
    TST_ASSERT(pRet->pLeft == pVector, "test_select_path_sets_fields: pLeft",
               "pRet->pLeft does not reference pVector");
  }

  // Cleanup not necessary for test purposes (short-lived)
  (void)pParse;
}

// Test 2: TK_VECTOR path with IN_RENAME_OBJECT == 0 should dup the inner expression
static void test_vector_path_dup_inner_when_not_renaming() {
  // Prepare inner expression
  Expr* inner = NewLeafExpr(42);

  // Create a vector with two elements, inner at index 1
  // For our test, we only need index 1 to exist
  Expr* e0 = NewLeafExpr(11);
  Expr* e1 = inner;
  Expr* vector = makeVectorWithExprs((Expr*[]){e0, e1}, 2);

  // Setup parse and pVector
  sqlite3 db;
  Parse* pParse = sqlite3ParseNew(&db);

  // Ensure IN_RENAME_OBJECT is false
  IN_RENAME_OBJECT = 0;

  // iField = 1 (second element), nField arbitrary (not used in dup path)
  Expr* pRet = sqlite3ExprForVectorField(pParse, vector, 1, 2);

  // Assertions
  TST_ASSERT(pRet != nullptr, "test_vector_path_dup_inner_when_not_renaming: pRet != nullptr",
             "Returned null in vector path when not renaming");
  if (pRet) {
    // pRet should be a dup of inner (same op, but different address)
    TST_ASSERT(pRet != inner, "test_vector_path_dup_inner_when_not_renaming: pRet != inner",
               "Returned expression should be a copy, not the same pointer");
    TST_ASSERT(pRet->op == inner->op, "test_vector_path_dup_inner_when_not_renaming: inner op preserved",
               "Duplicated expression op mismatch");
  }

  // Ensure no mutation of original vector's inner element
  TST_ASSERT(vector->x.pList->a[1].pExpr == inner, "test_vector_path_dup_inner_when_not_renaming: original inner intact",
             "Original inner expression was changed unexpectedly");

  (void)pParse;
}

// Test 3: TK_VECTOR path with IN_RENAME_OBJECT == 1 should rename and return inner expression
static void test_vector_path_rename_behaviour() {
  // Prepare inner expression
  Expr* inner = NewLeafExpr(77);

  // Create a vector with two elements, inner at index 1
  Expr* e0 = NewLeafExpr(21);
  Expr* e1 = inner;
  Expr* vector = makeVectorWithExprs((Expr*[]){e0, e1}, 2);

  // Setup parse and pVector
  sqlite3 db;
  Parse* pParse = sqlite3ParseNew(&db);

  // Ensure IN_RENAME_OBJECT is true
  IN_RENAME_OBJECT = 1;

  // iField = 1 to pick inner
  Expr* pRet = sqlite3ExprForVectorField(pParse, vector, 1, 2);

  // Assertions
  TST_ASSERT(pRet == inner, "test_vector_path_rename_behaviour: returns inner when renaming",
             "Expected the inner expression to be returned directly when IN_RENAME_OBJECT is true");

  // The corresponding slot should be nulled in the vector
  TST_ASSERT(vector->x.pList->a[1].pExpr == nullptr, "test_vector_path_rename_behaviour: inner slot nulled",
             "Inner slot should be nulled after ownership transfer during rename");

  (void)pParse;
}

// ----------------------------
// Main function to run tests
// ----------------------------
int main() {
  // Run all tests
  test_select_path_sets_fields();
  test_vector_path_dup_inner_when_not_renaming();
  test_vector_path_rename_behaviour();

  // Print summary
  PrintSummary();

  // Exit code: 0 if all tests passed
  int total = (int)g_tests.size();
  int passed = 0;
  for (const auto& t : g_tests) if (t.passed) ++passed;
  if (passed != total) {
    std::cerr << "Some tests failed. Exiting with non-zero status.\n";
    return 1;
  }
  return 0;
}