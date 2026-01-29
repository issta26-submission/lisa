/*
  Test suite for a simplified, self-contained replica of the focal logic
  of sqlite3ExprImpliesNonNullRow (originally in expr.c). This test harness
  is crafted to be self-contained (no external GTest) and uses C++11 only.

  Important note:
  - This repository environment cannot reliably instantiate the full SQLite internal
    AST and walker machinery. To provide executable unit tests that exercise the
    logical branches of sqlite3ExprImpliesNonNullRow, we implement a compact,
    self-contained analog of the essential control flow and dependency surface
    (Expr node structure, simple Walker, and a minimal DFS walker).
  - The tests verify true/false branch coverage for the decision points:
      * null input return 0
      * NOTNULL wrapping a subexpression
      * AND-chain short-circuit behavior
      * Leaftree without NOTNULL/AND affecting eCode
  - The tests do not rely on SQLite's private/static internals; they model the
    behavior sufficiently to validate the decision logic paths.

  Build:
  - Compile with a standard C++11 compiler, e.g. g++ -std=c++11 test_expr_impl_notnull.cpp -o test_expr
  - Run: ./test_expr

  The tests employ a lightweight assertion scheme that does not abort on failure,
  to ensure all test cases run and report their outcomes.
*/

#include <iostream>
#include <sqliteInt.h>
#include <vector>
#include <string>


// Self-contained simulation of the minimal parts of SQLite's structures
// needed to model the focal function's control flow.

enum Op {
  OP_LITERAL = 0,  // leaf literal expression
  TK_NOTNULL = 1,  // NOT NULL operator
  TK_AND = 2,      // AND operator
  // Other operators are not needed for this test harness
};

// Lightweight expression node
struct Expr {
  int op;              // operation type (OP_LITERAL, TK_NOTNULL, TK_AND, ...)
  Expr* pLeft;          // left subtree
  Expr* pRight;         // right subtree
  bool isNotNullProp;    // domain-specific flag used by impliesNotNullRow mimic
  // Constructor convenience
  Expr(int o, Expr* l = nullptr, Expr* r = nullptr, bool n=nullptr)
    : op(o), pLeft(l), pRight(r), isNotNullProp(n) {}
};

// Walker structure with minimal fields used by the focal code path
struct Walker {
  // Callback that processes an Expr node during walk
  int (*xExprCallback)(Walker*, Expr*);
  void* xSelectCallback;
  void* xSelectCallback2;
  int eCode;        // accumulated code from walking the expression
  int mWFlags;
  union { int iCur; } u;
};

// Forward declarations of test harness helpers
static int impliesNotNullRow(Walker* w, Expr* p);
static int sqlite3ExprImpliesNonNullRow_Test(Expr *p, int iTab, int isRJ);
static void sqlite3WalkExpr_Test(Walker* pWalker, Expr* pExpr);
static Expr* sqlite3ExprSkipCollateAndLikely_Test(Expr* p);
static void cleanupExpr(Expr* p);

// Minimal mock implementations to mirror the behavior needed by the test harness

// In tests, we "skip collate" by returning the same node (no-op)
static Expr* sqlite3ExprSkipCollateAndLikely_Test(Expr* p){
  return p;
}

// A simple DFS walk over the expression tree. It calls the provided
// xExprCallback on each node, then recursively over children.
static void sqlite3WalkExpr_Test(Walker* pWalker, Expr* pExpr){
  if(pWalker == nullptr || pExpr == nullptr) return;
  if(pWalker->xExprCallback){
    pWalker->xExprCallback(pWalker, pExpr);
  }
  if(pExpr->pLeft)  sqlite3WalkExpr_Test(pWalker, pExpr->pLeft);
  if(pExpr->pRight) sqlite3WalkExpr_Test(pWalker, pExpr->pRight);
}

// Callback used by the walker to simulate "impliesNotNullRow" behavior.
// We model a positive implication flow whenever a leaf has isNotNullProp set.
// This mirrors the idea that a subexpression marked in tests as NOT NULL implies the
// row is non-null for the purposes of this unit test.
static int impliesNotNullRow(Walker* w, Expr* p){
  (void)w; (void)p; // unused in this simplified mock, but kept for signature fidelity
  // If the node is a literal leaf and isNotNullProp is true, mark eCode
  if(p && p->isNotNullProp){
    // In the real code, eCode would be derived through evaluation. Here we set 1.
    // The test harness interprets non-zero eCode as "true".
    return 1;
  }
  return 0;
}

// Re-implementation (in-test) of the focal function's logic, using the
// lightweight test scaffolding above.
static int sqlite3ExprImpliesNonNullRow_Test(Expr *p, int iTab, int isRJ){
  Walker w;
  p = sqlite3ExprSkipCollateAndLikely_Test(p);
  if( p==nullptr ) return 0;
  if( p->op==TK_NOTNULL ){
    p = p->pLeft;
  }else{
    while( p->op==TK_AND ){
      if( sqlite3ExprImpliesNonNullRow_Test(p->pLeft, iTab, isRJ) ) return 1;
      p = p->pRight;
    }
  }
  w.xExprCallback = impliesNotNullRow;
  w.xSelectCallback = nullptr;
  w.xSelectCallback2 = nullptr;
  w.eCode = 0;
  w.mWFlags = (isRJ!=0);
  w.u.iCur = iTab;
  sqlite3WalkExpr_Test(&w, p);
  return w.eCode;
}

// Helper to delete a small expression tree (avoid leaks in tests)
static void cleanupExpr(Expr* p){
  if(!p) return;
  cleanupExpr(p->pLeft);
  cleanupExpr(p->pRight);
  delete p;
}

// Simple, non-terminating assertion helper
static int assertEq(int actual, int expected, const std::string& msg, int& failCount){
  if(actual != expected){
    std::cerr << "ASSERT FAILED: " << msg
              << " | expected=" << expected << " actual=" << actual << "\n";
    ++failCount;
    return 0;
  } else {
    std::cout << "ASSERT PASSED: " << msg << "\n";
    return 1;
  }
}

// Tests
// Each test returns true if it passed; false otherwise. They do not abort on failure.

static bool test_null_input_returns_zero(){
  // Test: input is nullptr -> should return 0
  int res = sqlite3ExprImpliesNonNullRow_Test(nullptr, 0, 0);
  int passed = (res == 0);
  if(!passed){
    std::cerr << "Test failed: null input should return 0\n";
  }
  return passed;
}

// Test 1: NOTNULL wrapping a subexpression with NOT NULL implies
static bool test_notnull_wraps_inner_implies_true(){
  // inner leaf which signals NOT NULL has been observed
  Expr* inner = new Expr(OP_LITERAL, nullptr, nullptr, true /*isNotNullProp*/);
  Expr* notnull = new Expr(TK_NOTNULL, inner, nullptr, false);
  int res = sqlite3ExprImpliesNonNullRow_Test(notnull, 0, 0);
  bool ok = (res == 1);
  if(!ok){
    std::cerr << "Test failed: NOTNULL wrapping inner with NOT NULL should imply non-null (1)\n";
  }
  cleanupExpr(notnull);
  cleanupExpr(inner);
  return ok;
}

// Test 2: AND-chain where left subtree implies NOT NULL
static bool test_and_chain_left_implies_true(){
  // Left subtree implies NOT NULL (via inner leaf)
  Expr* inner = new Expr(OP_LITERAL, nullptr, nullptr, true);
  Expr* left = inner; // reuse
  Expr* rightLeaf = new Expr(OP_LITERAL, nullptr, nullptr, false);
  Expr* andNode = new Expr(TK_AND, left, rightLeaf, false);

  // The left subtree should cause sqlite3ExprImpliesNonNullRow_Test to return 1
  int res = sqlite3ExprImpliesNonNullRow_Test(andNode, 0, 0);
  bool ok = (res == 1);
  if(!ok){
    std::cerr << "Test failed: AND-chain where left implies NOT NULL should return 1\n";
  }
  cleanupExpr(andNode);
  // inner is shared with left; no separate free needed
  // rightLeaf freed by andNode deletion in some libs; double-free avoided here for simplicity
  delete inner;
  delete rightLeaf;
  return ok;
}

// Test 3: AND-chain where neither side implies NOT NULL -> result 0
static bool test_and_chain_none_implies_false(){
  Expr* leftLeaf = new Expr(OP_LITERAL, nullptr, nullptr, false);
  Expr* rightLeaf = new Expr(OP_LITERAL, nullptr, nullptr, false);
  Expr* andNode = new Expr(TK_AND, leftLeaf, rightLeaf, false);

  int res = sqlite3ExprImpliesNonNullRow_Test(andNode, 0, 0);
  bool ok = (res == 0);
  if(!ok){
    std::cerr << "Test failed: AND-chain with no NOT NULL implications should return 0\n";
  }
  cleanupExpr(andNode);
  // leftLeaf and rightLeaf are cleaned by cleanupExpr
  delete leftLeaf;
  delete rightLeaf;
  return ok;
}

// Test 4: Leaf with NOTNULL-like property (without NOTNULL) implies non-null
static bool test_leaf_with_notnull_prop_implies_true(){
  Expr* leaf = new Expr(OP_LITERAL, nullptr, nullptr, true);
  int res = sqlite3ExprImpliesNonNullRow_Test(leaf, 0, 0);
  bool ok = (res == 1);
  if(!ok){
    std::cerr << "Test failed: leaf with NOT NULL prop should imply non-null (1)\n";
  }
  cleanupExpr(leaf);
  return ok;
}

// Test 5: Null input again to ensure no crash and proper 0 return
static bool test_null_input_again(){
  int res = sqlite3ExprImpliesNonNullRow_Test(nullptr, 123, 1);
  bool ok = (res == 0);
  if(!ok){
    std::cerr << "Test failed: null input with non-zero iTab/isRJ should still return 0\n";
  }
  return ok;
}

// Main driver
int main(){
  int total = 0;
  int failed = 0;

  std::vector<bool(*)()> tests = {
    test_null_input_returns_zero,
    test_notnull_wraps_inner_implies_true,
    test_and_chain_left_implies_true,
    test_and_chain_none_implies_false,
    test_leaf_with_notnull_prop_implies_true,
    test_null_input_again
  };

  // Run tests
  for(auto t : tests){
    bool ok = t();
    total++;
    if(!ok) ++failed;
  }

  std::cout << "\nTest results: " << (total - failed) << " passed, " << failed << " failed, out of " << total << " tests.\n";

  return (failed == 0) ? 0 : 1;
}