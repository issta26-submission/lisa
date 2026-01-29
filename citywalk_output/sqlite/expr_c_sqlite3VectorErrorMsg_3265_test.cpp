/*
  Unit test suite for the focal method:
  void sqlite3VectorErrorMsg(Parse *pParse, Expr *pExpr)

  Summary:
  - Validates that when ExprUseXSelect(pExpr) is true, sqlite3SubselectError is invoked
    with the correct nExpr value and error code.
  - Validates that when ExprUseXSelect(pExpr) is false, sqlite3ErrorMsg is invoked with
    the exact message "row value misused".
  - This test is self-contained (no GTest), uses minimal stubs for dependencies, and
    exercises the two branches of the focal method.

  Notes:
  - The test harness defines minimal representations for the types Parse, Expr, Select,
    and ExprList to mirror the usage in the focal method.
  - Static helpers (ExprUseXSelect, sqlite3SubselectError, sqlite3ErrorMsg) are implemented
    to drive and capture behavior for assertions.
  - The preprocessor branch SQLITE_OMIT_SUBQUERY is assumed not defined in these tests to
    exercise the true-branch path. The code is written to be easily adaptable if that macro
    is defined (the test harness itself is self-contained and does not depend on external
    SQLite sources).
  - All assertions are non-terminating (they print failures but continue), per domain knowledge
    guidance.

  Key Candidate Keywords:
  - ExprUseXSelect, pExpr->x.pSelect->pEList->nExpr, sqlite3SubselectError, sqlite3ErrorMsg
  - Parse, Expr, Select, ExprList structures and their minimal fields
  - "row value misused" message, branch coverage, non-terminating EXPECT-like assertions
*/

#include <iostream>
#include <sqliteInt.h>
#include <cassert>
#include <string>


// ------------------------
// Minimal type stubs (test harness)
// ------------------------

struct ExprList {
  int nExpr;
};

struct Select {
  ExprList *pEList;
};

struct Expr {
  // Mimic the nested structure used by the focal method:
  struct {
    Select *pSelect;
  } x;
  int useXSelect; // drives ExprUseXSelect(pExpr) in tests
};

// Opaque parse object used by the focal method
struct Parse {};

// ------------------------
// Global capture state for assertions
// ------------------------

static bool g_subselectCalled = false;
static int  g_subselectNExpr = 0;
static int  g_subselectErr   = 0;

static bool g_errorMsgCalled = false;
static std::string g_errorMsgStr;

// ------------------------
// Helpers to mimic dependencies used by the focal method
// These are kept intentionally simple to drive test scenarios deterministically.
// ------------------------

bool ExprUseXSelect(const Expr *pExpr) {
  // Drive path via a simple field; if not set, behave deterministically.
  return (pExpr != nullptr) && (pExpr->useXSelect != 0);
}

// Stub for sqlite3SubselectError used when ExprUseXSelect(pExpr) is true
void sqlite3SubselectError(Parse *pParse, int nExpr, int err) {
  (void)pParse; (void)err; // suppress unused in release
  g_subselectCalled = true;
  g_subselectNExpr = nExpr;
  g_subselectErr   = err;
}

// Stub for sqlite3ErrorMsg used when error needs to be reported
void sqlite3ErrorMsg(Parse *pParse, const char *z) {
  (void)pParse; // suppress unused in release
  g_errorMsgCalled = true;
  g_errorMsgStr = z ? std::string(z) : std::string();
}

// ------------------------
// The focal method under test (reproduced here for a self-contained harness)
// void sqlite3VectorErrorMsg(Parse *pParse, Expr *pExpr)
// ------------------------

void sqlite3VectorErrorMsg(Parse *pParse, Expr *pExpr){
#ifndef SQLITE_OMIT_SUBQUERY
  if( ExprUseXSelect(pExpr) ){
    sqlite3SubselectError(pParse, pExpr->x.pSelect->pEList->nExpr, 1);
  }else
#endif
  {
    sqlite3ErrorMsg(pParse, "row value misused");
  }
}

// ------------------------
// Lightweight test framework (non-terminating assertions)
// ------------------------

static int g_totalAssertions = 0;
static int g_failedAssertions = 0;

#define EXPECT_TRUE(cond) \
  do { \
    ++g_totalAssertions; \
    if(!(cond)) { \
      ++g_failedAssertions; \
      std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                << "  Condition: " #cond "\n"; \
    } \
  } while(0)

#define EXPECT_EQ(a, b) \
  do { \
    ++g_totalAssertions; \
    if(!((a) == (b))) { \
      ++g_failedAssertions; \
      std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                << "  Expected " << (a) << " == " << (b) << "\n"; \
    } \
  } while(0)

#define RESET_STATE() \
  do { \
    g_subselectCalled = false; \
    g_subselectNExpr = 0; \
    g_subselectErr   = 0; \
    g_errorMsgCalled = false; \
    g_errorMsgStr.clear(); \
  } while(0)

// ------------------------
// Test cases
// ------------------------

/*
  Test Case 1: Subquery path taken (ExprUseXSelect returns true)
  - pExpr.x.pSelect->pEList->nExpr = 5
  - Expect sqlite3SubselectError to be called with nExpr=5 and err=1
  - Expect sqlite3ErrorMsg not to be called
*/
void test_vectorErrorMsg_subselect_true() {
  Parse parseObj;
  // Build expression tree: pExpr->x.pSelect->pEList->nExpr == 5
  ExprList elist;
  elist.nExpr = 5;

  Select selectObj;
  selectObj.pEList = &elist;

  Expr exprObj;
  exprObj.x.pSelect = &selectObj;
  exprObj.useXSelect = 1; // trigger the true-branch

  // Reset and invoke
  RESET_STATE();
  sqlite3VectorErrorMsg(&parseObj, &exprObj);

  // Assertions
  EXPECT_TRUE(g_subselectCalled);                // subselect path taken
  EXPECT_EQ(g_subselectNExpr, 5);                // nExpr propagated
  EXPECT_EQ(g_subselectErr, 1);                  // error code passed
  EXPECT_TRUE(!g_errorMsgCalled);                // no generic error message should be printed
}

/*
  Test Case 2: Non-subquery path (ExprUseXSelect returns false)
  - pExpr.useXSelect = 0
  - Expect sqlite3ErrorMsg to be called with "row value misused"
  - Expect sqlite3SubselectError not to be called
*/
void test_vectorErrorMsg_subquery_false() {
  Parse parseObj;
  // Build expression tree: pExpr->x.pSelect->pEList is irrelevant since we won't take true-branch
  ExprList elist;
  elist.nExpr = 42; // arbitrary

  Select selectObj;
  selectObj.pEList = &elist;

  Expr exprObj;
  exprObj.x.pSelect = &selectObj;
  exprObj.useXSelect = 0; // trigger the false-branch (else)

  // Reset and invoke
  RESET_STATE();
  sqlite3VectorErrorMsg(&parseObj, &exprObj);

  // Assertions
  EXPECT_TRUE(!g_subselectCalled);                 // subselect path not taken
  EXPECT_TRUE(g_errorMsgCalled);                   // generic error message used
  EXPECT_EQ(g_errorMsgStr, "row value misused");  // exact message content
}

// ------------------------
// Main: run tests and report results
// ------------------------

int main() {
  std::cout << "Running sqlite3VectorErrorMsg unit tests (self-contained harness)\n";

  test_vectorErrorMsg_subselect_true();
  test_vectorErrorMsg_subquery_false();

  int total = g_totalAssertions;
  int failed = g_failedAssertions;

  std::cout << "Tests run: " << total << ", Failures: " << failed << "\n";

  // If any test failed, return non-zero to indicate failure
  return (failed == 0) ? 0 : 1;
}