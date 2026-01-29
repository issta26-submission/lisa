/*
  C++11 Test Suite for sqlite3ExprSimplifiedAndOr (focal method)
  - This test harness provides a minimal, self-contained environment
    that mirrors the required Expr structure and operator semantics
    used by the focal method.
  - It implements a simplified version of the necessary parts of the
    SQLite expression system to exercise true/false branch handling.
  - No external test framework (GTest not used). Tests are run from main().
  - Memory management is handled carefully to avoid leaks/double frees
    by using a shared ownership deletion routine.
  - Each test case is documented with explanatory comments.
*/

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Domain-specific minimal mimic of the Expr structure and ops
// to exercise sqlite3ExprSimplifiedAndOr in a self-contained way.

struct Expr {
  int op;           // operation type: TK_AND, TK_OR, TK_CONST, or other
  Expr* pLeft;      // left subtree
  Expr* pRight;     // right subtree
  int iConst;       // constant value if op == TK_CONST (nonzero = true, zero = false)
  // test helper flag to avoid double-deletion in complex graphs
  unsigned char __visited;
};

// Operator codes (subset sufficient for tests)
enum {
  TK_AND  = 1,
  TK_OR   = 2,
  TK_CONST = 3,
  // other ops not used in tests can be represented by arbitrary integers
};

// Forward declaration of focal method (mirror of provided C function)
Expr *sqlite3ExprSimplifiedAndOr(Expr *pExpr);

// Lightweight helper predicates used by the focal method
static bool ExprAlwaysTrue(const Expr *p){
  // True when the expression is a constant nonzero
  return p != nullptr && p->op == TK_CONST && p->iConst != 0;
}
static bool ExprAlwaysFalse(const Expr *p){
  // True when the expression is a constant zero
  return p != nullptr && p->op == TK_CONST && p->iConst == 0;
}

// Factory helpers for test expression trees
static Expr* newConst(int value){
  Expr* e = new Expr{TK_CONST, nullptr, nullptr, value, 0};
  return e;
}
static Expr* mkAnd(Expr* a, Expr* b){
  Expr* e = new Expr{TK_AND, a, b, 0, 0};
  return e;
}
static Expr* mkOr(Expr* a, Expr* b){
  Expr* e = new Expr{TK_OR, a, b, 0, 0};
  return e;
}

// Recursive deletion with visited flag to handle shared subexpressions safely
static void freeTree(Expr* e){
  if(!e) return;
  if(e->__visited) return;
  e->__visited = 1;
  freeTree(e->pLeft);
  freeTree(e->pRight);
  delete e;
}

// Implementation of the focal method (as in expr.c)
Expr *sqlite3ExprSimplifiedAndOr(Expr *pExpr){
  // The provided code asserts non-null input; in tests we ensure non-null.
  if( pExpr->op==TK_AND || pExpr->op==TK_OR ){
    Expr *pRight = sqlite3ExprSimplifiedAndOr(pExpr->pRight);
    Expr *pLeft  = sqlite3ExprSimplifiedAndOr(pExpr->pLeft);
    if( ExprAlwaysTrue(pLeft) || ExprAlwaysFalse(pRight) ){
      pExpr = pExpr->op==TK_AND ? pRight : pLeft;
    }else if( ExprAlwaysTrue(pRight) || ExprAlwaysFalse(pLeft) ){
      pExpr = pExpr->op==TK_AND ? pLeft : pRight;
    }
  }
  return pExpr;
}

/***********************
  Test Harness
  - Each test creates expression trees, runs the focal method,
    and verifies the resulting structure matches expected outcomes.
  - Tests are designed to cover true/false branches of each predicate exhaustively.
  - main() runs all tests and reports summary.
***********************/

// Simple test harness results
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Utility: compare a resulting expression to an expected primitive CONST value
static bool expectConstValue(const Expr* e, int expected){
  return e != nullptr && e->op == TK_CONST && e->iConst == expected;
}

// Utility: check pointer identity (for non-const cases)
static bool expectSamePointer(const Expr* a, const Expr* b){
  return a == b;
}

// Test 1: AND(True, False) -> should simplify to False (CONST 0)
static bool test_case_and_true_false(){
  // Build: (TRUE AND FALSE)
  Expr* left  = newConst(1); // TRUE
  Expr* right = newConst(0); // FALSE
  Expr* root  = mkAnd(left, right);
  // Run focal method
  Expr* res = sqlite3ExprSimplifiedAndOr(root);
  bool ok = expectConstValue(res, 0);
  // Memory cleanup
  freeTree(root); // safe due to visited guards
  g_tests_run++;
  if(ok){
    g_tests_passed++;
  }else{
    g_tests_failed++;
    std::cerr << "[Test 1] Failed: (TRUE AND FALSE) did not simplify to CONST 0 as expected.\n";
  }
  return ok;
}

// Test 2: AND(False, True) -> should simplify to False (left)
static bool test_case_and_false_true(){
  // Build: (FALSE AND TRUE)
  Expr* left  = newConst(0); // FALSE
  Expr* right = newConst(1); // TRUE
  Expr* root  = mkAnd(left, right);
  Expr* res = sqlite3ExprSimplifiedAndOr(root);
  bool ok = expectConstValue(res, 0) && expectSamePointer(res, left);
  freeTree(root);
  g_tests_run++;
  if(ok) g_tests_passed++;
  else {
    g_tests_failed++;
    std::cerr << "[Test 2] Failed: (FALSE AND TRUE) did not simplify to left CONST 0 as expected.\n";
  }
  return ok;
}

// Test 3: OR(True, False) -> should simplify to True (left)
static bool test_case_or_true_false(){
  // Build: (TRUE OR FALSE)
  Expr* left  = newConst(1); // TRUE
  Expr* right = newConst(0); // FALSE
  Expr* root  = mkOr(left, right);
  Expr* res = sqlite3ExprSimplifiedAndOr(root);
  bool ok = expectConstValue(res, 1) && expectSamePointer(res, left);
  freeTree(root);
  g_tests_run++;
  if(ok) g_tests_passed++;
  else {
    g_tests_failed++;
    std::cerr << "[Test 3] Failed: (TRUE OR FALSE) did not simplify to left CONST 1 as expected.\n";
  }
  return ok;
}

// Test 4: OR(False, True) -> should simplify to True (right)
static bool test_case_or_false_true(){
  // Build: (FALSE OR TRUE)
  Expr* left  = newConst(0); // FALSE
  Expr* right = newConst(1); // TRUE
  Expr* root  = mkOr(left, right);
  Expr* res = sqlite3ExprSimplifiedAndOr(root);
  bool ok = expectConstValue(res, 1) && expectSamePointer(res, right);
  freeTree(root);
  g_tests_run++;
  if(ok) g_tests_passed++;
  else {
    g_tests_failed++;
    std::cerr << "[Test 4] Failed: (FALSE OR TRUE) did not simplify to right CONST 1 as expected.\n";
  }
  return ok;
}

// Test 5: Nested: (TRUE AND FALSE) OR FALSE -> simplifies to FALSE
static bool test_case_nested_and_or_simplification(){
  // Build inner: (TRUE AND FALSE) => FALSE
  Expr* a = newConst(1); // TRUE
  Expr* b = newConst(0); // FALSE
  Expr* inner = mkAnd(a, b);
  // Outer: (inner) OR FALSE
  Expr* c = newConst(0); // FALSE
  Expr* root = mkOr(inner, c);

  Expr* res = sqlite3ExprSimplifiedAndOr(root);
  // Expected: FALSE (CONST 0)
  bool ok = expectConstValue(res, 0);
  freeTree(root);
  g_tests_run++;
  if(ok) g_tests_passed++;
  else {
    g_tests_failed++;
    std::cerr << "[Test 5] Failed: Nested simplification did not yield CONST 0 as expected.\n";
  }
  return ok;
}

// Test 6: No simplification (root is a non-constant composite with no true/false constants)
static bool test_case_no_simplification(){
  // Build: (X AND Y) where X and Y are non-const leaves (op != CONST)
  Expr* X = new Expr{999, nullptr, nullptr, 0, 0}; // non-const leaf
  Expr* Y = new Expr{999, nullptr, nullptr, 0, 0}; // non-const leaf
  Expr* root = mkAnd(X, Y);
  Expr* res = sqlite3ExprSimplifiedAndOr(root);
  // Expect the same root pointer (no simplification)
  bool ok = expectSamePointer(res, root);
  freeTree(root);
  g_tests_run++;
  if(ok) g_tests_passed++;
  else {
    g_tests_failed++;
    std::cerr << "[Test 6] Failed: No simplification case did not preserve root pointer.\n";
  }
  return ok;
}

// Entry point
int main() {
  // Run all test cases
  bool all_ok = true;
  all_ok &= test_case_and_true_false();
  all_ok &= test_case_and_false_true();
  all_ok &= test_case_or_true_false();
  all_ok &= test_case_or_false_true();
  all_ok &= test_case_nested_and_or_simplification();
  all_ok &= test_case_no_simplification();

  std::cout << "sqlite3ExprSimplifiedAndOr test results: "
            << g_tests_passed << " / " << g_tests_run << " tests passed.\n";

  if(g_tests_failed != 0){
    std::cerr << g_tests_failed << " test(s) FAILED.\n";
  }

  // Return non-zero if any test failed to help integration with build scripts
  return g_tests_failed ? 1 : 0;
}