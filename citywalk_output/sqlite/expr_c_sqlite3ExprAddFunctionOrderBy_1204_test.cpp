// C++11 test suite for sqlite3ExprAddFunctionOrderBy
// This test harness provides lightweight mocks of the SQLite core
// to exercise the focal function's control flow without requiring
// the full SQLite build. It uses non-terminating expectations.

#include <vector>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain knowledge: provide minimal environment for the focal function
// to compile and run in isolation.

using std::cout;
using std::endl;
using std::vector;

/* Forward declarations of minimal SQLite-like types used by the focal method */
struct ExprList;
struct Expr;
struct Parse;
struct sqlite3;

/* Constants to mimic SQLite tokens/ops used by the focal function */
static const int TK_FUNCTION = 1;
static const int TK_ORDER = 2;

/* EP_FullSize flag placeholder used in the focal function */
static const int EP_FullSize = 1;

/* Token placeholder (not used in tests) */
struct Token { int dummy; };

/* Minimal sqlite3 structure with mallocFailed flag (for asserts in original code) */
struct sqlite3 {
    int mallocFailed;
};

/* Minimal Parse structure carrying a pointer to sqlite3 database */
struct Parse {
    sqlite3 *db;
};

/* Minimal ExprList with just the needed field nExpr */
struct ExprList {
    int nExpr;
};

/* Minimal Expr structure matching usage in focal function */
struct Expr {
    int op;                 // operator type (TK_FUNCTION or TK_ORDER)
    Expr *pLeft;              // left subexpression (used to attach ORDER BY as left child)
    struct {
        ExprList *pList;      // for x.pList
    } x;
    int isWindowFunc;         // flag to simulate IsWindowFunc(pExpr)
    int flags;                // to simulate EP_FullSize etc
};

/* ---------------- Mocked/Helper Infrastructure ---------------- */

// Allocation failure simulation for sqlite3ExprAlloc
static bool g_simulateAllocFailure = false;

// Track if a window-like aggregate error was invoked
static bool g_windowAggErrorCalled = false;

// Track if cleanup was registered via sqlite3ParserAddCleanup
struct CleanupEntry {
    void (*xCleanup)(sqlite3*, void*);
    void *pArg;
    Parse *pParse;
};
static vector<CleanupEntry> g_CleanupList;

// Track if sqlite3ExprListDelete was invoked (for test verification)
static bool g_exprListDeletedCalled = false;
static ExprList *g_lastDeletedExprList = nullptr;

// Track if sqlite3ExprOrderByAggregateError was invoked (via window func path)
static bool g_exprOrderByAggErrorCalled = false;

// Forward declarations of focal function (we implement a standalone version here)
static void sqlite3ExprOrderByAggregateError(Parse *pParse, const Expr *pExpr);
static void sqlite3ExprListDelete(sqlite3 *db, ExprList *pList);
static void sqlite3ParserAddCleanup(Parse *pParse,
                                  void (*xCleanup)(sqlite3*, void*),
                                  void *pArg);
static int ExprUseXList(const Expr *p);
static bool IsWindowFunc(const Expr *pExpr);
static Expr *sqlite3ExprAlloc(sqlite3 *db, int op, const Token *pToken, int dequote);
static void ExprSetProperty(Expr *p, int flags);

// The focal function under test (reproduced for testing in this harness)
static void sqlite3ExprAddFunctionOrderBy(
  Parse *pParse,        /* Parsing context */
  Expr *pExpr,          /* The function call to which ORDER BY is to be added */
  ExprList *pOrderBy    /* The ORDER BY clause to add */
){
/*
  The code is a near-verbatim reimplementation to be testable in this
  standalone harness environment.
*/
{
  Expr *pOB;
  sqlite3 *db = pParse->db;
  // NEVER(pOrderBy==0) is compiled as 0 in our harness for testability
  if( 0 ){
    // unreachable in harness (mimics NEVER)
    // nothing
  }
  if( pOrderBy==0 ){
    // If this branch were taken, it would assert; not taken in harness
  }
  if( pExpr==0 ){
    // Not exercised in harness
    sqlite3ExprListDelete(db, pOrderBy);
    return;
  }
  // In harness, ensure pExpr looks like a function with no left operand
  if( pExpr->op != TK_FUNCTION ){
    return;
  }
  if( pExpr->pLeft!=0 ){
    return;
  }
  if( ExprUseXList(pExpr)==0 ){
    return;
  }
  if( pExpr->x.pList==0 || (pExpr->x.pList->nExpr==0) ){
    sqlite3ParserAddCleanup(pParse,
        (void(*)(sqlite3*,void*))sqlite3ExprListDelete,
        pOrderBy);
    return;
  }
  if( IsWindowFunc(pExpr) ){
    sqlite3ExprOrderByAggregateError(pParse, pExpr);
    sqlite3ExprListDelete(db, pOrderBy);
    return;
  }
  pOB = sqlite3ExprAlloc(db, TK_ORDER, 0, 0);
  if( pOB==0 ){
    sqlite3ExprListDelete(db, pOrderBy);
    return;
  }
  pOB->x.pList = pOrderBy;
  if( !ExprUseXList(pOB) ){
    // Should not happen in harness
  }
  pExpr->pLeft = pOB;
  ExprSetProperty(pOB, EP_FullSize);
}
}

/* ------------------- Real Implementations (Mocks) --------------- */

static void sqlite3ExprOrderByAggregateError(Parse *pParse, const Expr *p){
    (void)pParse;
    (void)p;
    g_exprOrderByAggErrorCalled = true;
}
static void sqlite3ExprListDelete(sqlite3 *db, ExprList *pList){
    // Mark as deleted; do not delete memory to keep explicit control in tests
    (void)db;
    g_exprListDeletedCalled = true;
    g_lastDeletedExprList = pList;
}
static void sqlite3ParserAddCleanup(Parse *pParse,
                                  void (*xCleanup)(sqlite3*, void*),
                                  void *pArg){
    (void)pParse;
    CleanupEntry e;
    e.xCleanup = xCleanup;
    e.pArg = pArg;
    e.pParse = pParse;
    g_CleanupList.push_back(e);
}
static int ExprUseXList(const Expr *p){
    (void)p;
    return 1;
}
static bool IsWindowFunc(const Expr *pExpr){
    if(!pExpr) return false;
    return pExpr->isWindowFunc != 0;
}
static Expr *sqlite3ExprAlloc(sqlite3 *db, int op, const Token *pToken, int dequote){
    (void)db; (void)pToken; (void)dequote;
    // Simulate allocation. Allow test to inject failure.
    if( g_simulateAllocFailure ) return nullptr;
    Expr *e = new Expr();
    e->op = op;
    e->pLeft = nullptr;
    e->x.pList = nullptr;
    e->isWindowFunc = 0;
    e->flags = 0;
    return e;
}
static void ExprSetProperty(Expr *p, int flags){
    if(p) p->flags |= flags;
}

/* ----------------------- Test Harness Macros ------------------- */

// Lightweight, non-terminating test assertions (like EXPECT_* in GTest)
static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_totalTests; \
    if(!(cond)) { \
        ++g_failedTests; \
        std::cerr << "EXPECT_TRUE failed at line " << __LINE__ << "\n"; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    ++g_totalTests; \
    if(!((a) == (b))) { \
        ++g_failedTests; \
        std::cerr << "EXPECT_EQ failed at line " << __LINE__ \
                  << ": " << (a) << " != " << (b) << "\n"; \
    } \
} while(0)

#define EXPECT_NEQ(a,b) do { \
    ++g_totalTests; \
    if((a) == (b)) { \
        ++g_failedTests; \
        std::cerr << "EXPECT_NEQ failed at line " << __LINE__ \
                  << ": " << (a) << " == " << (b) << "\n"; \
    } \
} while(0)

#define TEST(name) void name(); static int name##_registered = (name(),0); void name()

/* -------------------------- Tests ------------------------------ */

// Test 1: Basic path where a valid ORDER BY clause is attached to a function
TEST(test_AttachOrderBy_Merges correctly){
    // Prepare environment
    sqlite3 db; db.mallocFailed = 0;
    Parse pParse; pParse.db = &db;

    // Function expression: TK_FUNCTION with no left, and non-empty pList
    Expr *pExpr = new Expr();
    pExpr->op = TK_FUNCTION;
    pExpr->pLeft = nullptr;
    pExpr->isWindowFunc = 0;

    // pExpr's internal x.pList should exist to satisfy the assertion
    pExpr->x.pList = new ExprList();
    pExpr->x.pList->nExpr = 3;

    // OrderBy list
    ExprList *pOrderBy = new ExprList();
    pOrderBy->nExpr = 2;

    // Call method under test
    sqlite3ExprAddFunctionOrderBy(&pParse, pExpr, pOrderBy);

    // Assertions on expected state
    // pExpr should now have a left child which is an ORDER expression
    EXPECT_TRUE(pExpr->pLeft != nullptr);
    if(pExpr->pLeft){
        EXPECT_EQ(pExpr->pLeft->op, TK_ORDER);
        EXPECT_EQ(pExpr->pLeft->x.pList, pOrderBy);
        // EP_FullSize should be set on the ORDER expression
        EXPECT_TRUE((pExpr->pLeft->flags & EP_FullSize) != 0);
    }

    // Cleanup
    delete pExpr;
    delete pOrderBy;
    // No cleanup expected in this path
}

// Test 2: When the function list is zero-expr, a cleanup should be registered
TEST(test_AttachOrderBy_OnZeroArg aggregates_register_cleanup){
    sqlite3 db; db.mallocFailed = 0;
    Parse pParse; pParse.db = &db;

    Expr *pExpr = new Expr();
    pExpr->op = TK_FUNCTION;
    pExpr->pLeft = nullptr;
    pExpr->isWindowFunc = 0;
    // pList non-null to satisfy first checks, but nExpr==0 to trigger cleanup branch
    pExpr->x.pList = new ExprList();
    pExpr->x.pList->nExpr = 0;

    ExprList *pOrderBy = new ExprList();
    pOrderBy->nExpr = 1;

    // Call
    sqlite3ExprAddFunctionOrderBy(&pParse, pExpr, pOrderBy);

    // Expect a cleanup registration
    EXPECT_TRUE(!g_CleanupList.empty());
    if(!g_CleanupList.empty()){
        EXPECT_TRUE(g_CleanupList[0].xCleanup == (void(*)(sqlite3*,void*))sqlite3ExprListDelete);
        EXPECT_TRUE(g_CleanupList[0].pArg == pOrderBy);
    }

    // Cleanup
    delete pExpr;
    delete pOrderBy;
}

// Test 3: Window function path should trigger aggregate error and delete ORDER BY
TEST(test_WindowFunc_Path_triggers_agg_error_and_deletes_orderby){
    sqlite3 db; db.mallocFailed = 0;
    Parse pParse; pParse.db = &db;

    Expr *pExpr = new Expr();
    pExpr->op = TK_FUNCTION;
    pExpr->pLeft = nullptr;
    pExpr->isWindowFunc = 1; // trigger window function path

    ExprList *pOrderBy = new ExprList();
    pOrderBy->nExpr = 2;

    sqlite3ExprAddFunctionOrderBy(&pParse, pExpr, pOrderBy);

    // Expect window-aggregate error path
    EXPECT_TRUE(g_exprOrderByAggErrorCalled);
    // ORDER BY should have been deleted
    EXPECT_TRUE(g_exprListDeletedCalled);
    if(g_exprListDeletedCalled){
        EXPECT_EQ(g_lastDeletedExprList, pOrderBy);
    }

    delete pExpr;
}

// Test 4: Allocation failure during ORDER BY allocation should delete the list
TEST(test_AllocFailure_path.deletes_list){
    sqlite3 db; db.mallocFailed = 0;
    Parse pParse; pParse.db = &db;

    // Force allocation to fail
    g_simulateAllocFailure = true;

    Expr *pExpr = new Expr();
    pExpr->op = TK_FUNCTION;
    pExpr->pLeft = nullptr;
    pExpr->isWindowFunc = 0;
    pExpr->x.pList = new ExprList();
    pExpr->x.pList->nExpr = 2;

    ExprList *pOrderBy = new ExprList();
    pOrderBy->nExpr = 2;

    sqlite3ExprAddFunctionOrderBy(&pParse, pExpr, pOrderBy);

    // Expect deletion of the ORDER BY list on allocation failure
    EXPECT_TRUE(g_exprListDeletedCalled);
    if(g_exprListDeletedCalled){
        EXPECT_EQ(g_lastDeletedExprList, pOrderBy);
    }

    // Reset flag for other tests
    g_simulateAllocFailure = false;

    delete pExpr;
}

// Very lightweight test runner
int main() {
    // Run tests
    test_AttachOrderBy_Merges correctly();
    test_AttachOrderBy_OnZeroArg aggregates_register_cleanup();
    test_WindowFunc_Path_triggers_agg_error_and_deletes_orderby();
    test_AllocFailure_path.deletes_list();

    cout << "Tests run: " << g_totalTests << ", Passed: "
         << (g_totalTests - g_failedTests) << ", Failed: "
         << g_failedTests << endl;

    // Cleanup any remaining allocations (best-effort)
    // In a full test suite, we'd track allocated objects and delete them here.

    return g_failedTests ? 1 : 0;
}