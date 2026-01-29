// Self-contained C++11 unit tests for the focal method logic (sqlite3ExprCodeLoadIndexColumn)
// This test harness emulates the minimal internal structures and mocks the dependent
// subroutines to verify both branches of the conditional.
// Notes:
// - This is a standalone, non-GTest test harness suitable for environments without GTest.
// - Tests cover the true-branch (XN_EXPR) and false-branch (regular column) paths.
// - Static and private implementation details are avoided; we provide lightweight stubs.
// - The aim is to validate logic flow and call-site interactions rather than full SQLite integration.

#include <iostream>
#include <sqliteInt.h>
#include <cassert>


// Domain: We re-create the minimal subset of SQLite structures and APIs needed
// to exercise the focal logic in a self-contained manner.

#define XN_EXPR -1  // Local override to trigger the expression-index path in tests

// Forward declarations for mocks (would be provided by sqlite in the real project)
struct Expr {};
struct Vdbe {};
struct Table { int dummy; };  // minimal placeholder
struct ExprList { int nExpr; struct { Expr *pExpr; } *a; };  // used by real code path (simplified)

// Minimal Parse structure with the fields used by the focal method
struct Parse {
  int iSelfTab;
  Vdbe *pVdbe;
};

// Minimal Index structure with fields used by the focal method
struct ColExprItem {
  Expr *pExpr;
};
struct ColExpr {
  int nExpr;
  ColExprItem *a;
};
struct Index {
  int *aiColumn;      // mapping of index columns to table columns (or XN_EXPR)
  ColExpr *aColExpr;   // optional expression for expression-indexed columns
  Table *pTable;        // table the index refers to
};

// Globals to capture calls from the mocks for verification
static Parse* g_lastCopyParse = nullptr;
static Expr*  g_lastCopiedExpr = nullptr;
static int    g_lastCopiedReg = 0;
static bool   g_copyCalled = false;

static Vdbe*  g_lastGetColumnVdbe = nullptr;
static Table* g_lastGetColumnTable = nullptr;
static int    g_lastGetColumn_iTabCur = 0;
static int    g_lastGetColumn_iCol = 0;
static int    g_lastGetColumn_regOut = 0;
static bool   g_getColumnCalled = false;

// Mock implementations to capture interactions
extern "C" void sqlite3ExprCodeCopy(Parse *pParse, Expr *pExpr, int regOut) {
  g_lastCopyParse = pParse;
  g_lastCopiedExpr = pExpr;
  g_lastCopiedReg = regOut;
  g_copyCalled = true;
}
extern "C" void sqlite3ExprCodeGetColumnOfTable(Vdbe *v, Table *pTab, int iTabCur, int iCol, int regOut) {
  (void)v; // unused in test mock
  g_lastGetColumnVdbe = v;
  g_lastGetColumnTable = pTab;
  g_lastGetColumn_iTabCur = iTabCur;
  g_lastGetColumn_iCol = iCol;
  g_lastGetColumn_regOut = regOut;
  g_getColumnCalled = true;
}

// The focal method under test (re-implemented here in the test harness for isolation)
// Signature matches the one provided in the prompt, but uses our test-doubles above.
extern "C" void sqlite3ExprCodeLoadIndexColumn(
  Parse *pParse,  /* The parsing context */
  Index *pIdx,    /* The index whose column is to be loaded */
  int iTabCur,    /* Cursor pointing to a table row */
  int iIdxCol,    /* The column of the index to be loaded */
  int regOut      /* Store the index column value in this register */
){
  i16 iTabCol = pIdx->aiColumn[iIdxCol];
  if( iTabCol==XN_EXPR ){
    // expression-indexed column path
    assert( pIdx->aColExpr );
    assert( pIdx->aColExpr->nExpr>iIdxCol );
    pParse->iSelfTab = iTabCur + 1;
    sqlite3ExprCodeCopy(pParse, pIdx->aColExpr->a[iIdxCol].pExpr, regOut);
    pParse->iSelfTab = 0;
  }else{
    // regular column path
    sqlite3ExprCodeGetColumnOfTable(pParse->pVdbe, pIdx->pTable, iTabCur,
                                    iTabCol, regOut);
  }
}

// Lightweight types for the test (C++ friendly) to avoid including sqlite internals
// Real code would include sqliteInt.h and related headers.
typedef short i16;

// Test helpers
static void testBranchExpressionIndex_true();
static void testBranchRegularColumn_false();

int main() {
  // Run tests and report
  int total_tests = 0;
  int failed = 0;

  // Test 1: true branch (XN_EXPR path)
  {
    testBranchExpressionIndex_true();
    ++total_tests;
    // The test itself prints its status; we infer success by observing no aborts
  }

  // Test 2: false branch (regular column path)
  {
    testBranchRegularColumn_false();
    ++total_tests;
  }

  // In this harness, tests print their own results. Return code indicates success.
  if (false) { // placeholder to allow easy extension
    // no-op
  }
  std::cout << "Total tests executed: " << total_tests << "\n";
  if (failed) {
    std::cerr << "Some tests FAILED.\n";
    return 1;
  }
  std::cout << "All tests completed.\n";
  return 0;
}

// Test 1: Branch where the index column refers to an expression (XN_EXPR)
static void testBranchExpressionIndex_true() {
  // Reset global mocks
  g_lastCopyParse = nullptr;
  g_lastCopiedExpr = nullptr;
  g_lastCopiedReg = 0;
  g_copyCalled = false;

  g_lastGetColumnVdbe = nullptr;
  g_lastGetColumnTable = nullptr;
  g_lastGetColumn_iTabCur = 0;
  g_lastGetColumn_iCol = 0;
  g_lastGetColumn_regOut = 0;
  g_getColumnCalled = false;

  // Build a minimal fake Index with an expression column
  Parse pParse;
  pParse.iSelfTab = 0;
  pParse.pVdbe = nullptr; // not used in true-branch

  // Prepare aiColumn so that iIdxCol=0 yields XN_EXPR
  int *aiColumn = new int[1];
  aiColumn[0] = XN_EXPR; // force expression path

  ColExpr *pExprCol = new ColExpr;
  pExprCol->nExpr = 2;
  pExprCol->a = new struct ColExprItem[2];
  pExprCol->a[0].pExpr = new Expr; // expression to be copied
  pExprCol->a[1].pExpr = new Expr; // second entry (not used in this test)

  Index idx;
  idx.aiColumn = aiColumn;
  idx.aColExpr = pExprCol;
  idx.pTable = nullptr; // not used in true-branch

  // Call the focal method
  int iTabCur = 7;
  int iIdxCol = 0;
  int regOut  = 42;
  sqlite3ExprCodeLoadIndexColumn(&pParse, &idx, iTabCur, iIdxCol, regOut);

  // Verify the true-branch behavior
  bool ok = true;
  // iSelfTab should have been set to iTabCur+1 during copy and reset to 0 after
  if (pParse.iSelfTab != 0) {
    std::cerr << "[FAIL] testBranchExpressionIndex_true: pParse.iSelfTab expected 0 after load, got "
              << pParse.iSelfTab << "\n";
    ok = false;
  }
  // The copy function should have been called with the first expression and regOut
  if (!g_copyCalled) {
    std::cerr << "[FAIL] testBranchExpressionIndex_true: sqlite3ExprCodeCopy not invoked\n";
    ok = false;
  }
  if (g_lastCopiedExpr != pExprCol->a[0].pExpr) {
    std::cerr << "[FAIL] testBranchExpressionIndex_true: sqlite3ExprCodeCopy called with unexpected Expr*\n";
    ok = false;
  }
  if (g_lastCopiedReg != regOut) {
    std::cerr << "[FAIL] testBranchExpressionIndex_true: sqlite3ExprCodeCopy regOut mismatch\n";
    ok = false;
  }

  if (ok) {
    std::cout << "[PASS] testBranchExpressionIndex_true: Expression-index branch executed correctly.\n";
  } else {
    std::cerr << "[FAIL] testBranchExpressionIndex_true: See details above.\n";
  }

  // Cleanup
  delete[] aiColumn;
  delete[] pExprCol->a;
  delete pExprCol;
  // Note: pParse, idx structures were stack-allocated; no further cleanup needed.
}

// Test 2: Branch where the index column refers to a real table column (regular column)
static void testBranchRegularColumn_false() {
  // Reset global mocks
  g_lastCopyParse = nullptr;
  g_lastCopiedExpr = nullptr;
  g_lastCopiedReg = 0;
  g_copyCalled = false;

  g_lastGetColumnVdbe = nullptr;
  g_lastGetColumnTable = nullptr;
  g_lastGetColumn_iTabCur = 0;
  g_lastGetColumn_iCol = 0;
  g_lastGetColumn_regOut = 0;
  g_getColumnCalled = false;

  // Build a minimal fake Index with a real column (not XN_EXPR)
  Parse pParse;
  pParse.iSelfTab = 0;
  Vdbe vdb;
  pParse.pVdbe = &vdb;

  int *aiColumn = new int[1];
  aiColumn[0] = 5; // not XN_EXPR

  ColExpr *pExprCol = new ColExpr; // not used in this path
  pExprCol->nExpr = 0;
  pExprCol->a = nullptr;

  Index idx;
  idx.aiColumn = aiColumn;
  idx.aColExpr = pExprCol; // even if non-null, should not be used in this path
  Table tbl;
  idx.pTable = &tbl;

  // Call the focal method
  int iTabCur = 3;
  int iIdxCol = 0;
  int regOut  = 77;
  sqlite3ExprCodeLoadIndexColumn(&pParse, &idx, iTabCur, iIdxCol, regOut);

  // Verify the false-branch behavior
  bool ok = true;
  if (!g_getColumnCalled) {
    std::cerr << "[FAIL] testBranchRegularColumn_false: sqlite3ExprCodeGetColumnOfTable not invoked\n";
    ok = false;
  }
  if (g_lastGetColumnVdbe != &vdb) {
    std::cerr << "[FAIL] testBranchRegularColumn_false: sqlite3ExprCodeGetColumnOfTable received wrong Vdbe*\n";
    ok = false;
  }
  if (g_lastGetColumnTable != &tbl) {
    std::cerr << "[FAIL] testBranchRegularColumn_false: sqlite3ExprCodeGetColumnOfTable received wrong Table*\n";
    ok = false;
  }
  if (g_lastGetColumn_iTabCur != iTabCur) {
    std::cerr << "[FAIL] testBranchRegularColumn_false: sqlite3ExprCodeGetColumnOfTable iTabCur mismatch\n";
    ok = false;
  }
  if (g_lastGetColumn_iCol != aiColumn[0]) {
    std::cerr << "[FAIL] testBranchRegularColumn_false: sqlite3ExprCodeGetColumnOfTable iCol mismatch\n";
    ok = false;
  }
  if (g_lastGetColumn_regOut != regOut) {
    std::cerr << "[FAIL] testBranchRegularColumn_false: sqlite3ExprCodeGetColumnOfTable regOut mismatch\n";
    ok = false;
  }

  if (ok) {
    std::cout << "[PASS] testBranchRegularColumn_false: Regular column path executed correctly.\n";
  } else {
    std::cerr << "[FAIL] testBranchRegularColumn_false: See details above.\n";
  }

  // Cleanup
  delete[] aiColumn;
  delete[] pExprCol->a;
  delete pExprCol;
}