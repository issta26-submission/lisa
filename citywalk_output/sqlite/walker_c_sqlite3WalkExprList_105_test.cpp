// Lightweight C++11 unit test harness for sqlite3WalkExprList
// This test-suite is self-contained (no Google Test) and mocks only the
// minimal structures required to exercise sqlite3WalkExprList.
//
// Focus: validate correct propagation of WRC_Abort and WRC_Continue through
// sqlite3WalkExprList, under various scenarios (nullptr, empty list, single
// item, multiple items with/without abort conditions).

#include <stdlib.h>
#include <string.h>
#include <sqliteInt.h>
#include <iostream>


// Domain knowledge: emulate the C structures used by sqlite3WalkExprList
struct Walker {}; // opaque in production

struct Expr {
    int id; // simple discriminator used to control sqlite3WalkExpr behavior
};

struct ExprList_item {
    Expr *pExpr;
};

struct ExprList {
    int nExpr;
    ExprList_item *a;
};

// Directions (matching the SQLite-like API)
static int WRC_Abort = 1;
static int WRC_Continue = 0;

// Forward declarations (as in actual code dependencies)
int sqlite3WalkExpr(Walker *pWalker, Expr *pExpr);
int sqlite3WalkExprList(Walker *pWalker, ExprList *p);

// Global state used by the test to control behavior of sqlite3WalkExpr
static int abortedExprId = -1; // if set to an actual id, sqlite3WalkExpr aborts when encountering it

// Mock implementation of sqlite3WalkExpr used by sqlite3WalkExprList
// Returns non-zero (abort) if the expression's id matches abortedExprId.
int sqlite3WalkExpr(Walker *pWalker, Expr *pExpr) {
    (void)pWalker; // unused in this mock
    if (pExpr == nullptr) return 0;
    if (pExpr->id == abortedExprId) {
        return 1; // non-zero -> treated as abort
    }
    return 0; // continue
}

// Implementation under test: copy of provided focal method
int sqlite3WalkExprList(Walker *pWalker, ExprList *p){
  int i;
  struct ExprList_item *pItem;
  if( p ){
    for(i=p->nExpr, pItem=p->a; i>0; i--, pItem++){
      if( sqlite3WalkExpr(pWalker, pItem->pExpr) ) return WRC_Abort;
    }
  }
  return WRC_Continue;
}

// --------- Simple test harness (non-terminating, executes all tests) ---------

static int gTotalTests = 0;
static int gFailedTests = 0;

static void recordResult(const char* testName, bool passed) {
    gTotalTests++;
    if (!passed) {
        gFailedTests++;
        std::cerr << "[FAILED] " << testName << "\n";
    } else {
        std::cout << "[PASSED] " << testName << "\n";
    }
}

#define ASSERT_EQ(expected, actual, testName) \
    recordResult(testName, (expected) == (actual))

// Helpers to create/destroy expression lists conveniently
Expr* makeExpr(int id) {
    Expr* e = new Expr();
    e->id = id;
    return e;
}

ExprList* makeExprList(const Expr** exprs, int count) {
    ExprList* p = new ExprList();
    p->nExpr = count;
    p->a = new ExprList_item[count];
    for (int i = 0; i < count; ++i) {
        p->a[i].pExpr = const_cast<Expr*>(exprs[i]);
    }
    return p;
}

void freeExprList(ExprList* p) {
    if (!p) return;
    for (int i = 0; i < p->nExpr; ++i) {
        delete p->a[i].pExpr;
    }
    delete[] p->a;
    delete p;
}

// Test 1: pWalker != nullptr, p == nullptr -> should return WRC_Continue
void test_null_pointer_returns_continue() {
    Walker w;
    ExprList* p = nullptr;
    int res = sqlite3WalkExprList(&w, p);
    ASSERT_EQ(WRC_Continue, res, "null_pointer_returns_continue");
}

// Test 2: p != nullptr, but nExpr == 0 -> should return WRC_Continue
void test_empty_list_returns_continue() {
    Walker w;
    ExprList* p = new ExprList();
    p->nExpr = 0;
    p->a = nullptr;
    int res = sqlite3WalkExprList(&w, p);
    ASSERT_EQ(WRC_Continue, res, "empty_list_returns_continue");
    freeExprList(p); // safe even if empty
}

// Test 3: Single item, no abort (abortedExprId does not match any item)
void test_single_item_no_abort_returns_continue() {
    Walker w;
    abortedExprId = -1; // ensure no abort
    Expr* e = makeExpr(10);
    const Expr* items[1] = { e };
    ExprList* p = makeExprList(items, 1);
    int res = sqlite3WalkExprList(&w, p);
    ASSERT_EQ(WRC_Continue, res, "single_item_no_abort_returns_continue");
    freeExprList(p);
}

// Test 4: Multiple items, all non-aborting -> continue
void test_multiple_items_no_abort_returns_continue() {
    Walker w;
    abortedExprId = -1; // no aborts
    Expr* e1 = makeExpr(1);
    Expr* e2 = makeExpr(2);
    Expr* e3 = makeExpr(3);
    const Expr* items[3] = { e1, e2, e3 };
    ExprList* p = makeExprList(items, 3);
    int res = sqlite3WalkExprList(&w, p);
    ASSERT_EQ(WRC_Continue, res, "multiple_items_no_abort_returns_continue");
    freeExprList(p);
}

// Test 5: Abort occurs in middle -> return Abort
void test_abort_in_middle_returns_abort() {
    Walker w;
    abortedExprId = 2; // abort when encountering expression with id 2
    Expr* e1 = makeExpr(1);
    Expr* e2 = makeExpr(2); // this should trigger abort
    Expr* e3 = makeExpr(3);
    const Expr* items[3] = { e1, e2, e3 };
    ExprList* p = makeExprList(items, 3);
    int res = sqlite3WalkExprList(&w, p);
    ASSERT_EQ(WRC_Abort, res, "abort_in_middle_returns_abort");
    freeExprList(p);
}

// Entry point
int main() {
    std::cout << "Starting sqlite3WalkExprList unit tests...\n";

    test_null_pointer_returns_continue();
    test_empty_list_returns_continue();
    test_single_item_no_abort_returns_continue();
    test_multiple_items_no_abort_returns_continue();
    test_abort_in_middle_returns_abort();

    std::cout << "Tests completed: " << gTotalTests
              << " total, " << gFailedTests << " failed.\n";

    // Return non-zero if any test failed
    return (gFailedTests == 0) ? 0 : 1;
}