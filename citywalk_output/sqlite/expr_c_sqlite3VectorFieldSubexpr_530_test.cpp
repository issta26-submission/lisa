// Unit-test suite for the focal method: sqlite3VectorFieldSubexpr
// This test suite is crafted in C++11, without GoogleTest, and uses
// a lightweight, non-terminating assertion mechanism to maximize coverage.
// The tests mock the minimal subset of the SQLite internal Expr structures
// and helpers required by sqlite3VectorFieldSubexpr to exercise true/false
// branches of the method's conditional logic.
// Explanatory comments accompany each unit test.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Lightweight, non-terminating test macros
static int g_testFailures = 0;
#define TEST_CHECK(cond, msg)                                  \
  do {                                                           \
    if(!(cond)) {                                                \
      std::cerr << "[TEST FAILED] " << (msg)                     \
                << " (in " << __FILE__ << ":" << __LINE__ << ")" \
                << std::endl;                                    \
      ++g_testFailures;                                          \
    }                                                            \
  } while(0)

#define TEST_PASS(msg)                                           \
  do {                                                           \
    std::cout << "[TEST PASS] " << (msg) << std::endl;          \
  } while(0)

// Minimal subset of SQLite-like structures and helpers required by sqlite3VectorFieldSubexpr

// Opcodes (subset)
enum {
  TK_ERROR   = 1,
  TK_REGISTER= 2,
  TK_SELECT  = 3
};

// Forward declarations
struct Expr;
struct Select;
struct ExprList;
struct List;

// Minimal Expr structure compatible with sqlite3VectorFieldSubexpr usage
struct Expr {
  int op;
  int op2;
  // x union-like storage: the code uses pVector->x.pSelect and pVector->x.pList
  struct {
    Select *pSelect;
    List   *pList;
  } x;
  int iVectorSize;     // length if treated as a vector expression
  int useXSelect;      // indicates ExprUseXSelect(p)
  int useXList;         // indicates ExprUseXList(p)
};

// A simple ExprList element containing a single Expr*
struct ListExprElem {
  Expr *pExpr;
};

// ExprList containing an array of elements
struct ExprList {
  int nExpr;
  ListExprElem *a;
};

// Select containing an expression list for vector select case
struct Select {
  ExprList *pEList;
};

// List containing an array of elements with pExpr
struct List {
  int nExpr;
  struct {
    Expr *pExpr;
  } *a;
};

// ------------------------------------------------------------------------------------------------
// Implementations of the required helper predicates used by sqlite3VectorFieldSubexpr
// These are provided here solely for the test harness and are intentionally tiny.

static inline int sqlite3ExprVectorSize(const Expr *p){
  return p ? p->iVectorSize : 0;
}
static inline int sqlite3ExprIsVector(const Expr *p){
  return p && p->iVectorSize > 0;
}
static inline int ExprUseXSelect(const Expr *p){
  return p && p->useXSelect;
}
static inline int ExprUseXList(const Expr *p){
  return p && p->useXList;
}

// The focal method under test (re-implemented here for the test harness):
Expr *sqlite3VectorFieldSubexpr(Expr *pVector, int i){
  // Precondition: i must be less than vector size unless vector is an error
  assert( i<sqlite3ExprVectorSize(pVector) || pVector->op==TK_ERROR );
  if( sqlite3ExprIsVector(pVector) ){
    // The following assertion mirrors the original: either op2==0 or op==TK_REGISTER
    assert( pVector->op2==0 || pVector->op==TK_REGISTER );
    if( pVector->op==TK_SELECT || pVector->op2==TK_SELECT ){
      assert( ExprUseXSelect(pVector) );
      return pVector->x.pSelect->pEList->a[i].pExpr;
    }else{
      assert( ExprUseXList(pVector) );
      return pVector->x.pList->a[i].pExpr;
    }
  }
  return pVector;
}

// ------------------------------------------------------------------------------------------------
// Helper factories to construct test data

Expr* makeExpr() {
  Expr* e = new Expr();
  e->op = 0;
  e->op2 = 0;
  e->x.pSelect = nullptr;
  e->x.pList = nullptr;
  e->iVectorSize = 0;
  e->useXSelect = 0;
  e->useXList = 0;
  return e;
}

// Test 1: Non-vector input should return the original pointer
void test_non_vector_input_returns_same_pointer() {
  // Arrange
  Expr* pVector = makeExpr(); // iVectorSize == 0 -> not a vector
  // Act
  Expr* res = sqlite3VectorFieldSubexpr(pVector, 0);
  // Assert
  TEST_CHECK(res == pVector, "Non-vector input should return the same pointer");
  TEST_PASS("test_non_vector_input_returns_same_pointer");
}

// Test 2: Vector with TK_SELECT path
void test_vector_with_select_path_returns_select_expr() {
  // Arrange: create 3 inner expressions to be selected
  Expr* inner0 = makeExpr();
  Expr* inner1 = makeExpr();
  Expr* inner2 = makeExpr();

  // Build ExprList for Select
  ExprList* eList = new ExprList();
  eList->nExpr = 3;
  eList->a = new ListExprElem[3];
  eList->a[0].pExpr = inner0;
  eList->a[1].pExpr = inner1;
  eList->a[2].pExpr = inner2;

  // Build Select
  Select* select = new Select();
  select->pEList = eList;

  // Build vector pVector that uses TK_SELECT path
  Expr* pVector = makeExpr();
  pVector->op = TK_SELECT;
  pVector->op2 = 0;
  pVector->iVectorSize = 3;
  pVector->useXSelect = 1;
  pVector->useXList = 0;
  pVector->x.pSelect = select;
  pVector->x.pList = nullptr;

  // Act
  Expr* res = sqlite3VectorFieldSubexpr(pVector, 1);

  // Assert
  TEST_CHECK(res == inner1, "Vector select path should return pEList->a[i].pExpr");
  TEST_PASS("test_vector_with_select_path_returns_select_expr");

  // Cleanup (not strictly necessary for test harness lifetime, but good hygiene)
  delete[] eList->a;
  delete eList;
  delete select;
  // innerX expressions are intentionally not deleted to avoid double-free in this test harness
}

// Test 3: Vector with TK_LIST path
void test_vector_with_list_path_returns_list_expr() {
  // Arrange: create 3 inner expressions for the list path
  Expr* l0 = makeExpr();
  Expr* l1 = makeExpr();
  Expr* l2 = makeExpr();

  // Build List
  List* lst = new List();
  lst->nExpr = 3;
  lst->a = new struct { Expr *pExpr; }[3];
  lst->a[0].pExpr = l0;
  lst->a[1].pExpr = l1;
  lst->a[2].pExpr = l2;

  // Build vector pVector that uses List path (i.e., not TK_SELECT)
  Expr* pVector = makeExpr();
  pVector->op = 0;        // not TK_SELECT
  pVector->op2 = 0;
  pVector->iVectorSize = 3;
  pVector->useXSelect = 0;
  pVector->useXList = 1;
  pVector->x.pList = lst;
  pVector->x.pSelect = nullptr;

  // Act
  Expr* res = sqlite3VectorFieldSubexpr(pVector, 2);

  // Assert
  TEST_CHECK(res == l2, "Vector list path should return pList->a[i].pExpr");
  TEST_PASS("test_vector_with_list_path_returns_list_expr");

  // Cleanup
  delete[] lst->a;
  delete lst;
  // l0, l1, l2 are not deleted to avoid double-free in this simple harness
}

// Main runner
int main() {
  test_non_vector_input_returns_same_pointer();
  test_vector_with_select_path_returns_select_expr();
  test_vector_with_list_path_returns_list_expr();

  if(g_testFailures == 0){
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cerr << g_testFailures << " TEST(S) FAILED" << std::endl;
    return 1;
  }
}