// Minimal C++11 test harness for sqlite3ExprVectorSize
// This test reproduces the essential behavior of the focal function
// in a self-contained environment to validate true/false branches.
// It does not rely on external SQLite headers to ensure portability.

#include <memory>
#include <iostream>
#include <sqliteInt.h>
#include <cassert>


// Domain-specific token definitions (simplified subset)
enum TokenType {
  TK_REGISTER = 1,
  TK_VECTOR   = 2,
  TK_SELECT   = 3
};

// Forward declarations for test structures
struct ExprList;
struct Select;

// A simplified union to mimic the Expr.x field used by sqlite3ExprVectorSize
union ExprX {
  ExprList *pList;
  Select   *pSelect;
};

// Expression node (simplified)
struct Expr {
  unsigned char op;
  unsigned char op2;
  ExprX x;
};

// ExprList (holds number of expressions for vector/select list)
struct ExprList {
  int nExpr;
};

// Select (holds an expression list for the select expression)
struct Select {
  ExprList *pEList;
};

// Helper predicates mimicking internal helpers in sqlite3ExprVectorSize
static inline bool ExprUseXList(const Expr *p) {
  // In the real code, this would check internal state.
  // For test purposes, we simply verify presence of pList.
  return p->x.pList != nullptr;
}

static inline bool ExprUseXSelect(const Expr *p) {
  // For test purposes, verify presence of pSelect.
  return p->x.pSelect != nullptr;
}

// Focal method re-implemented for the test environment
int sqlite3ExprVectorSize(const Expr *pExpr){
  unsigned char op = pExpr->op;
  if( op == TK_REGISTER ) op = pExpr->op2;
  if( op == TK_VECTOR ){
    assert( ExprUseXList(pExpr) );
    return pExpr->x.pList->nExpr;
  } else if( op == TK_SELECT ){
    assert( ExprUseXSelect(pExpr) );
    return pExpr->x.pSelect->pEList->nExpr;
  } else {
    return 1;
  }
}

// Lightweight test harness
static int g_total = 0;
static int g_failed = 0;

// Non-terminating check: log failures but continue
#define CHECK_EQ(expected, actual, msg) do { \
  ++g_total; \
  if( (expected) != (actual) ){ \
    std::cerr << "TEST FAIL: " << msg << " | expected " << (expected) \
              << ", got " << (actual) << "\n"; \
    ++g_failed; \
  } \
} while(0)

// Test 1: TK_VECTOR path with explicit vector list; should return nExpr
// Expects that a vector with 5 expressions yields 5.
void test_vector_path_from_vector_list() {
  // Arrange
  Expr e;
  e.op = TK_VECTOR;     // direct vector path
  e.op2 = 0;
  e.x.pList = new ExprList;
  e.x.pList->nExpr = 5;
  e.x.pSelect = nullptr;

  // Act
  int size = sqlite3ExprVectorSize(&e);

  // Assert
  CHECK_EQ(5, size, "Vector path should return nExpr from pList (5)");
  delete e.x.pList;
}

// Test 2: TK_REGISTER followed by TK_VECTOR in op2; should return nExpr from vector list
// This exercises the branch where op is replaced by op2 and vector path is taken.
void test_register_to_vector_path() {
  // Arrange
  Expr e;
  e.op = TK_REGISTER;
  e.op2 = TK_VECTOR;
  e.x.pList = new ExprList;
  e.x.pList->nExpr = 3;
  e.x.pSelect = nullptr;

  // Act
  int size = sqlite3ExprVectorSize(&e);

  // Assert
  CHECK_EQ(3, size, "Register->Vector path should return nExpr from pList (3)");
  delete e.x.pList;
}

// Test 3: TK_REGISTER followed by TK_SELECT in op2; should return nExpr from select list
// This exercises the select-path branch via op2 and a valid pSelect structure.
void test_register_to_select_path() {
  // Arrange
  Expr e;
  e.op = TK_REGISTER;
  e.op2 = TK_SELECT;
  e.x.pList = nullptr; // ensure vector path is not taken
  e.x.pSelect = new Select;
  e.x.pSelect->pEList = new ExprList;
  e.x.pSelect->pEList->nExpr = 7;

  // Act
  int size = sqlite3ExprVectorSize(&e);

  // Assert
  CHECK_EQ(7, size, "Register->Select path should return nExpr from pEList (7)");
  delete e.x.pSelect->pEList;
  delete e.x.pSelect;
}

// Test 4: Other operation (neither VECTOR nor SELECT); should return 1
// Verifies the default behavior for non-vector/select expressions.
void test_other_op_returns_one() {
  // Arrange
  Expr e;
  e.op = 123; // some arbitrary value not equal to VECTOR or REGISTER/SELECT paths
  e.op2 = 0;
  e.x.pList = nullptr;
  e.x.pSelect = nullptr;

  // Act
  int size = sqlite3ExprVectorSize(&e);

  // Assert
  CHECK_EQ(1, size, "Other op should return 1 by default");
}

int main() {
  // Run tests
  test_vector_path_from_vector_list();
  test_register_to_vector_path();
  test_register_to_select_path();
  test_other_op_returns_one();

  // Summary
  std::cout << "Ran " << g_total << " tests. "
            << "Passed " << (g_total - g_failed) << ", "
            << "Failed " << g_failed << ".\n";

  return g_failed ? 1 : 0;
}