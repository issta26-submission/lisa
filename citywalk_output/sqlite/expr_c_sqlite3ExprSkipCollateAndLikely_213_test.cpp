// Minimal in-repo unit tests for the focal method: sqlite3ExprSkipCollateAndLikely
// This test suite provides a self-contained C++11 harness that mocks only the
// tiny subset of the sqlite3 Expr structures and constants needed to exercise
// the control flow of the focal function.
// The real project would compile this against sqlite3; here we replicate just
// enough to validate the logic paths.

#include <vector>
#include <iostream>
#include <sqliteInt.h>
#include <cassert>


// Domain-specific constants (subset sufficient for tests)
enum OpCode {
  TK_FUNCTION = 1,
  TK_COLLATE  = 2,
  // other ops are not needed for these tests
};

// Property flags
enum ExprProperty {
  EP_Skip     = 1 << 0,
  EP_Unlikely = 1 << 1
};

// Forward declaration
struct ExprList;

// Lightweight expression node used for tests
struct Expr {
  int op;                    // operation type (TK_FUNCTION, TK_COLLATE, etc.)
  Expr *pLeft;                // left sub-expression (used for TK_COLLATE path)
  struct {
    ExprList *pList;          // for EP_Unlikely path: list of arguments
  } x;
  unsigned int flags;          // property flags (EP_Skip, EP_Unlikely)
  bool hasXList;               // indicates that x.pList is valid
  Expr() : op(0), pLeft(nullptr), flags(0), hasXList(false) { x.pList = nullptr; }
};

// List element used inside ExprList to mimic pList->a[0].pExpr
struct ListElem { Expr *pExpr; };

// Expression list (subset: only a single element is required by tests)
struct ExprList {
  int nExpr;
  ListElem *a;
  ExprList(int n = 0) : nExpr(n), a(nullptr) {}
};

// Helper constructors to simplify test setup
static Expr* NewExpr(int op, unsigned int flags = 0, Expr* pLeft = nullptr) {
  Expr* e = new Expr();
  e->op = op;
  e->flags = flags;
  e->pLeft = pLeft;
  e->hasXList = false;
  e->x.pList = nullptr;
  return e;
}

static ExprList* NewExprList(int n) {
  ExprList* L = new ExprList(n);
  L->a = new ListElem[n];
  // Initialize pointers to nullptr
  for (int i = 0; i < n; ++i) L->a[i].pExpr = nullptr;
  return L;
}

// Minimal reproduction of the focal function (self-contained for tests)
static Expr *sqlite3ExprSkipCollateAndLikely(Expr *pExpr){
  while( pExpr && (pExpr->flags & (EP_Skip|EP_Unlikely)) ){
    if( pExpr->flags & EP_Unlikely ){
      // In real code: assert( ExprUseXList(pExpr) );
      // and: assert( pExpr->x.pList->nExpr>0 );
      // and: assert( pExpr->op==TK_FUNCTION );
      // Move to the first argument expression
      assert(pExpr->hasXList);
      assert(pExpr->x.pList != nullptr);
      assert(pExpr->x.pList->nExpr > 0);
      assert(pExpr->op == TK_FUNCTION);
      pExpr = pExpr->x.pList->a[0].pExpr;
    }else{
      // EP_Skip path for collate
      assert(pExpr->op == TK_COLLATE);
      pExpr = pExpr->pLeft;
    }
  }  
  return pExpr;
}

// Test harness

// Test 1: When there is no EP_Skip/EP_Unlikely, the expression is returned unchanged.
static bool Test_NoSkipReturnsSame() {
  // Arrange
  Expr *p = NewExpr(999); // some arbitrary op, no flags
  // Act
  Expr *r = sqlite3ExprSkipCollateAndLikely(p);
  // Assert
  bool ok = (r == p);
  if (!ok) {
    std::cerr << "[Test_NoSkipReturnsSame] Failed: returned pointer differs from input.\n";
  }
  // Cleanup
  delete p;
  return ok;
}

// Test 2: EP_Skip without EP_Unlikely path should traverse via TK_COLLATE to left.
static bool Test_CollateTraversal() {
  // Arrange
  // inner expression that should be returned
  Expr *pInner = NewExpr(0, 0, nullptr);
  // left expression with TK_COLLATE and EP_Skip
  Expr *pLeft = NewExpr(TK_COLLATE, EP_Skip, pInner);
  pLeft->pLeft = pInner; // set the pLeft for the collate path
  // Act
  Expr *r = sqlite3ExprSkipCollateAndLikely(pLeft);
  // Assert
  bool ok = (r == pInner);
  if (!ok) {
    std::cerr << "[Test_CollateTraversal] Failed: expected to skip collate and return left's child.\n";
  }
  // Cleanup
  delete pInner;
  delete pLeft;
  return ok;
}

// Test 3: EP_Skip | EP_Unlikely path should dereference first argument and return it.
static bool Test_UnlikelyDereference() {
  // Arrange
  // inner target expression to be returned after dereference
  Expr *pInner = NewExpr(0, 0, nullptr);

  // Build list with one element: a[0].pExpr = pInner
  ExprList *pList = NewExprList(1);
  pList->a[0].pExpr = pInner;

  // Build function-like expression with EP_Unlikely and EP_Skip, and attach the list
  Expr *pFn = NewExpr(TK_FUNCTION, EP_Skip | EP_Unlikely, nullptr);
  pFn->hasXList = true;
  pFn->x.pList = pList;

  // Act
  Expr *r = sqlite3ExprSkipCollateAndLikely(pFn);

  // Assert
  bool ok = (r == pInner);
  if (!ok) {
    std::cerr << "[Test_UnlikelyDereference] Failed: expected to dereference first arg to pInner.\n";
  }

  // Cleanup
  delete pInner;
  delete[] pList->a;
  delete pList;
  delete pFn;
  return ok;
}

// Test 4: When the chain leads to null (EP_Skip with pLeft = nullptr), function returns nullptr.
static bool Test_NullLeftChain_ReturnsNull() {
  // Arrange
  Expr *pLeft = nullptr;
  Expr *pExpr = NewExpr(TK_COLLATE, EP_Skip, pLeft);
  pExpr->pLeft = pLeft;

  // Act
  Expr *r = sqlite3ExprSkipCollateAndLikely(pExpr);

  // Assert
  bool ok = (r == nullptr);
  if (!ok) {
    std::cerr << "[Test_NullLeftChain_ReturnsNull] Failed: expected null result when left is null.\n";
  }

  // Cleanup
  delete pExpr;
  return ok;
}

int main() {
  std::vector<bool(*)()> tests = {
    Test_NoSkipReturnsSame,
    Test_CollateTraversal,
    Test_UnlikelyDereference,
    Test_NullLeftChain_ReturnsNull
  };

  const char* testNames[] = {
    "Test_NoSkipReturnsSame",
    "Test_CollateTraversal",
    "Test_UnlikelyDereference",
    "Test_NullLeftChain_ReturnsNull"
  };

  int passed = 0;
  int total = static_cast<int>(tests.size());

  std::cout << "Running " << total << " unit tests for sqlite3ExprSkipCollateAndLikely (self-contained mock).\n";

  for (size_t i = 0; i < tests.size(); ++i) {
    bool result = tests[i]();
    if (result) {
      std::cout << "[PASS] " << testNames[i] << "\n";
      ++passed;
    } else {
      std::cout << "[FAIL] " << testNames[i] << "\n";
    }
  }

  std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

  return (passed == total) ? 0 : 1;
}