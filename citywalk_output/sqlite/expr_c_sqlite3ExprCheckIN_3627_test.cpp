// Lightweight C++11 test harness for sqlite3ExprCheckIN (focal method) with minimal stubs.
// This test is self-contained and does not rely on GoogleTest. It uses non-terminating
// assertions and prints a summary at the end.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Domain-specific minimal type stubs to build the test harness around sqlite3ExprCheckIN.
// We only implement what sqlite3ExprCheckIN uses directly.

struct ExprList {
  int nExpr;
};

struct Select {
  ExprList *pEList;
};

// Expr layout assumed by the focal method:
//
// typedef struct Expr {
//   Expr *pLeft;
//   struct { Select *pSelect; } x;
// } Expr;
//
// The test harness mirrors this layout.
struct Expr {
  Expr *pLeft;
  struct {
    Select *pSelect;
  } x;
};

struct Db {
  bool mallocFailed;
};

struct Parse {
  Db *db;
};

// Global test context to control behavior of dependent functions used by sqlite3ExprCheckIN.
struct TestContext {
  int vecValue;            // value returned by sqlite3ExprVectorSize(pLeft)
  bool useXSelect;          // value returned by ExprUseXSelect(pIn)
  int subselectCalls;        // how many times sqlite3SubselectError() called
  int subselectActual;       // last actual nExpr passed to sqlite3SubselectError
  int subselectVector;       // last nVector passed to sqlite3SubselectError
  int vectorMsgCalls;        // how many times sqlite3VectorErrorMsg() called
} gCtx;

// Dependency stubs (C linkage to resemble production code linkage)
extern "C" int sqlite3ExprVectorSize(Expr *pExpr) {
  (void)pExpr; // unused in stub; return controlled value
  return gCtx.vecValue;
}

extern "C" int ExprUseXSelect(Expr *pExpr) {
  (void)pExpr;
  return gCtx.useXSelect ? 1 : 0;
}

extern "C" void sqlite3SubselectError(Parse *pParse, int nExpr, int nVector) {
  (void)pParse;
  gCtx.subselectCalls++;
  gCtx.subselectActual = nExpr;
  gCtx.subselectVector = nVector;
}

extern "C" void sqlite3VectorErrorMsg(Parse *pParse, Expr *pLeft) {
  (void)pParse;
  (void)pLeft;
  gCtx.vectorMsgCalls++;
}

// Focal method under test (reproduced here for the standalone test harness).
// The actual production implementation would reside in expr.c.
extern "C" int sqlite3ExprCheckIN(Parse *pParse, Expr *pIn) {
  int nVector = sqlite3ExprVectorSize(pIn->pLeft);
  if( ExprUseXSelect(pIn) && !pParse->db->mallocFailed ){
    if( nVector!=pIn->x.pSelect->pEList->nExpr ){
      sqlite3SubselectError(pParse, pIn->x.pSelect->pEList->nExpr, nVector);
      return 1;
    }
  }else if( nVector!=1 ){
    sqlite3VectorErrorMsg(pParse, pIn->pLeft);
    return 1;
  }
  return 0;
}

// Simple non-terminating assertion helper for tests.
static int g_failures = 0;
#define EXPECT_EQ(actual, expected, msg)                             \
  do {                                                                \
    if ((actual) != (expected)) {                                   \
      std::cerr << "TEST FAIL: " << (msg)                          \
                << " | Expected: " << (expected)                    \
                << ", Actual: " << (actual) << std::endl;         \
      ++g_failures;                                                  \
    }                                                               \
  } while (0)

// Utility to reset the test context for each test case.
static void reset_ctx() {
  gCtx.vecValue = 0;
  gCtx.useXSelect = false;
  gCtx.subselectCalls = 0;
  gCtx.subselectActual = -1;
  gCtx.subselectVector = -1;
  gCtx.vectorMsgCalls = 0;
}

// Test Case 1: ExprUseXSelect(pIn) is true, mallocFailed is false, nVector != pEList->nExpr
// Expect: sqlite3SubselectError called and sqlite3ExprCheckIN returns 1.
static void test_case_1() {
  reset_ctx();
  gCtx.vecValue = 3;      // nVector
  gCtx.useXSelect = true; // ExprUseXSelect(pIn)
  gCtx.vectorMsgCalls = 0;
  gCtx.subselectCalls = 0;

  Db db; db.mallocFailed = false;
  Parse pParse; pParse.db = &db;

  // Build pIn with pLeft (nullptr is fine for test as vecSize is stubbed)
  Expr in;
  in.pLeft = nullptr;

  // pIn.x.pSelect->pEList->nExpr = 5
  ExprList el; el.nExpr = 5;
  Select s; s.pEList = &el;
  in.x.pSelect = &s;

  int rc = sqlite3ExprCheckIN(&pParse, &in);

  EXPECT_EQ(rc, 1, "Case1: return code should be 1");
  EXPECT_EQ(gCtx.subselectCalls, 1, "Case1: subselect should be called once");
  EXPECT_EQ(gCtx.subselectActual, 5, "Case1: actual nExpr passed to subselect");
  EXPECT_EQ(gCtx.subselectVector, 3, "Case1: nVector passed to subselect");
}

// Test Case 2: ExprUseXSelect(pIn) is true, mallocFailed is false, nVector == pEList->nExpr
// Expect: no error, returns 0.
static void test_case_2() {
  reset_ctx();
  gCtx.vecValue = 5;      // nVector matches pEList->nExpr
  gCtx.useXSelect = true; // ExprUseXSelect(pIn)
  gCtx.subselectCalls = 0;

  Db db; db.mallocFailed = false;
  Parse pParse; pParse.db = &db;

  Expr in;
  in.pLeft = nullptr;

  ExprList el; el.nExpr = 5;
  Select s; s.pEList = &el;
  in.x.pSelect = &s;

  int rc = sqlite3ExprCheckIN(&pParse, &in);

  EXPECT_EQ(rc, 0, "Case2: return code should be 0 when vectors match");
  EXPECT_EQ(gCtx.subselectCalls, 0, "Case2: subselect should not be called");
}

// Test Case 3: ExprUseXSelect(pIn) is false, mallocFailed is irrelevant, nVector != 1
// Expect: sqlite3VectorErrorMsg called and returns 1.
static void test_case_3() {
  reset_ctx();
  gCtx.vecValue = 2;      // nVector != 1
  gCtx.useXSelect = false; // ExprUseXSelect(pIn) false

  Db db; db.mallocFailed = false;
  Parse pParse; pParse.db = &db;

  Expr in;
  in.pLeft = nullptr;

  // pIn.x.pSelect is not used in this path, but set up to be consistent
  ExprList el; el.nExpr = 7;
  Select s; s.pEList = &el;
  in.x.pSelect = &s;

  int rc = sqlite3ExprCheckIN(&pParse, &in);

  EXPECT_EQ(rc, 1, "Case3: return code should be 1 due to vector error path");
  EXPECT_EQ(gCtx.vectorMsgCalls, 1, "Case3: vector error message should be invoked");
}

// Test Case 4: ExprUseXSelect(pIn) is false, nVector == 1
// Expect: no error, returns 0.
static void test_case_4() {
  reset_ctx();
  gCtx.vecValue = 1;      // nVector == 1
  gCtx.useXSelect = false; // ExprUseXSelect(pIn) false

  Db db; db.mallocFailed = false;
  Parse pParse; pParse.db = &db;

  Expr in;
  in.pLeft = nullptr;

  // For completeness, provide pIn.x.pSelect though not used
  ExprList el; el.nExpr = 3;
  Select s; s.pEList = &el;
  in.x.pSelect = &s;

  int rc = sqlite3ExprCheckIN(&pParse, &in);

  EXPECT_EQ(rc, 0, "Case4: no error when nVector equals 1 and ExprUseXSelect is false");
}

// Main test runner
int main() {
  // Run all test cases
  test_case_1();
  test_case_2();
  test_case_3();
  test_case_4();

  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cout << g_failures << " TEST(S) FAILED\n";
  }
  return g_failures;
}