// C++11 unit test suite for sqlite3ExprFunction (Expr *sqlite3ExprFunction(...))
// This test suite provides minimal stubs for SQLite internal structures to enable
// focused testing of the focal function's behavior without requiring the full SQLite build.
// The tests cover normal execution, allocation failure path, and error handling path
// for too many arguments, as described in the problem statement.

#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// ---------------------------
// Minimal internal stubs/types
// ---------------------------

// Token stub
struct Token {
    const char* z; // pointer into some buffer; used to compute offset in tests
};

// ExprList stub
struct ExprList {
    int nExpr;
};

// sqlite3 stub (only what's needed)
struct sqlite3 {
    int aLimit[10];
};

// Forward declaration of Parse and Expr used by sqlite3ExprFunction
struct Parse;

// Expr stub
struct Expr {
    int op;
    Token* pToken;
    int w_iOfst;      // mimic pNew->w.iOfst
    // mimic the 'x.pList' member inside Expr
    struct {
        ExprList* pList;
    } x;
    unsigned int flags;
    unsigned int Distinct;
};

// Parse stub
struct Parse {
    sqlite3* db;
    const char* zTail;
    int nested;
};

// Typedef/Constants (mirroring minimal needs)
#define TK_FUNCTION 1000

// Property flags (subset used by tests)
#define EP_HasFunc   0x01
#define EP_Distinct  0x02
#define EP_InnerON   0x04
#define EP_OuterON   0x08

// Distinct flags (subset)
#define SF_Distinct 1
#define SF_ALL      0

// Forward declarations of functions used by sqlite3ExprFunction
Expr* sqlite3ExprAlloc(sqlite3* db, int op, const Token* pToken, int dequote);
void sqlite3ExprListDelete(sqlite3* db, ExprList* pList);
void sqlite3ErrorMsg(Parse* pParse, const char* zFormat, const Token* pToken);
bool ExprHasProperty(Expr* p, int i)
{
    // In this simplified test harness, properties are stored in p->flags
    return (p ? (p->flags & i) != 0 : false);
}
void ExprSetProperty(Expr* p, int i)
{
    if (p) p->flags |= (unsigned int)i;
}
bool ExprUseXList(Expr* p)
{
    // Always true in this simplified environment
    return p != nullptr;
}
void sqlite3ExprSetHeightAndFlags(Parse* pParse, Expr* p)
{
    // No-op for test
}
extern "C" {
int sqlite3ExprCodeTarget(Parse*, Expr*, int);
}
static int pGlobDummy = 0; // dummy to avoid unused warnings

// Globals to observe/test behavior
static bool gForceExprAllocFail = false;
static int gExprListDeleteCount = 0;
static std::string gLastErrorMsg;

// Implementations of stubs

Expr* sqlite3ExprAlloc(sqlite3* db, int op, const Token* pToken, int dequote)
{
    (void)db; (void)dequote;
    if (gForceExprAllocFail) return nullptr;
    Expr* p = new Expr();
    p->op = op;
    p->pToken = const_cast<Token*>(pToken);
    p->w_iOfst = 0;
    p->x.pList = nullptr;
    p->flags = 0;
    p->Distinct = 0;
    return p;
}

void sqlite3ExprListDelete(sqlite3* db, ExprList* pList)
{
    (void)db;
    ++gExprListDeleteCount;
    delete pList;
}

void sqlite3ErrorMsg(Parse* pParse, const char* zFormat, const Token* pToken)
{
    (void)pParse;
    (void)pToken;
    // Record a readable error message (simplified)
    if (zFormat) {
        gLastErrorMsg = zFormat;
    } else {
        gLastErrorMsg.clear();
    }
}

// ---------------------------
// Focal method under test: sqlite3ExprFunction
// ---------------------------

Expr* sqlite3ExprFunction(
  Parse *pParse,        /* Parsing context */
  ExprList *pList,      /* Argument list */
  const Token *pToken,  /* Name of the function */
  int eDistinct         /* SF_Distinct or SF_ALL or 0 */
){
{
  Expr *pNew;
  sqlite3 *db = pParse->db;
  // In real code, assert(pToken); here we mimic a runtime assertion by simple check
  if (pToken == nullptr) return nullptr;
  pNew = sqlite3ExprAlloc(db, TK_FUNCTION, pToken, 1);
  if( pNew==nullptr ){
    sqlite3ExprListDelete(db, pList); /* Avoid memory leak when malloc fails */
    return 0;
  }
  // Original code asserts no Inner/Outer ON; we simulate always false
  // pNew->w.iOfst = (int)(pToken->z - pParse->zTail);
  pNew->w_iOfst = (int)(pToken->z - pParse->zTail);
  if( pList
   && pList->nExpr > pParse->db->aLimit[0] // SQLITE_LIMIT_FUNCTION_ARG assumed at index 0
   && !pParse->nested
  ){
    sqlite3ErrorMsg(pParse, "too many arguments on function %T", pToken);
  }
  pNew->x.pList = pList;
  ExprSetProperty(pNew, EP_HasFunc);
  assert( ExprUseXList(pNew) );
  sqlite3ExprSetHeightAndFlags(pParse, pNew);
  if( eDistinct==SF_Distinct ) ExprSetProperty(pNew, EP_Distinct);
  return pNew;
}
}

// ---------------------------
// Test framework (minimal)
// ---------------------------

static int gTestsRun = 0;
static int gFailures = 0;

void failTest(const std::string& name, const std::string& note)
{
    ++gFailures;
    std::cerr << "[FAIL] " << name << ": " << note << "\n";
}

void test_normal_path()
{
    // Reset state
    gForceExprAllocFail = false;
    gExprListDeleteCount = 0;
    gLastErrorMsg.clear();

    // Setup minimal objects
    sqlite3 db;
    db.aLimit[0] = 10; // allow up to 10 arguments
    Parse pParse;
    pParse.db = &db;
    char tailBuf[128];
    pParse.zTail = tailBuf;
    pParse.nested = 0;

    char tokenBuf[32];
    // Place token Z pointer at offset 4 from zTail
    Token pToken;
    pToken.z = tailBuf + 4;

    // Prepare argument list with 3 expressions (<= limit)
    ExprList* pList = new ExprList();
    pList->nExpr = 3;

    // Call function with SF_Distinct
    Expr* pRes = sqlite3ExprFunction(&pParse, pList, &pToken, SF_Distinct);

    gTestsRun++;

    if (pRes == nullptr) {
        failTest("test_normal_path", "Expected non-null result but got null");
        return;
    }
    // Check basic fields
    if (pRes->op != TK_FUNCTION) {
        failTest("test_normal_path", "Expression op is not TK_FUNCTION");
        return;
    }
    if (pRes->x.pList != pList) {
        failTest("test_normal_path", "pList not attached to function expression via x.pList");
        return;
    }
    if (pRes->w_iOfst != 4) {
        failTest("test_normal_path", "Incorrect w_iOfst offset");
        return;
    }
    if ((pRes->flags & EP_HasFunc) == 0) {
        failTest("test_normal_path", "EP_HasFunc flag not set on function expression");
        return;
    }
    if ((pRes->Distinct & EP_Distinct) == 0) {
        // Distinct flag is set by OR-ing EP_Distinct into properties, we emulate with Distinct field
        // In our simplified model, check Distinct value (non-zero means distinct)
        // We used pRes->Distinct as a separate value; set to 1 when SF_Distinct is passed.
        // If not, declare failure.
        failTest("test_normal_path", "EP_Distinct not set on function expression despite SF_Distinct");
        return;
    }

    // Cleanup
    delete pList;
    delete pRes;
}

void test_allocation_failure_path()
{
    // Reset
    gForceExprAllocFail = true;
    gExprListDeleteCount = 0;
    gLastErrorMsg.clear();

    sqlite3 db;
    db.aLimit[0] = 10;
    Parse pParse;
    pParse.db = &db;
    char tailBuf[128];
    pParse.zTail = tailBuf;
    pParse.nested = 0;

    Token pToken;
    pToken.z = tailBuf + 2;

    ExprList* pList = new ExprList();
    pList->nExpr = 1;

    Expr* pRes = sqlite3ExprFunction(&pParse, pList, &pToken, SF_ALL);

    gTestsRun++;

    if (pRes != nullptr) {
        failTest("test_allocation_failure_path", "Expected null due to allocation failure, got non-null");
        delete pRes;
        delete pList;
        return;
    }
    // Verify that pList was deleted to prevent leak
    if (gExprListDeleteCount != 1) {
        failTest("test_allocation_failure_path", "ExprListDelete not invoked on allocation failure");
        delete pList; // safe guard
        return;
    }

    // Reset for subsequent tests
    gForceExprAllocFail = false;
}

void test_too_many_arguments_path()
{
    // Reset
    gForceExprAllocFail = false;
    gExprListDeleteCount = 0;
    gLastErrorMsg.clear();

    sqlite3 db;
    db.aLimit[0] = 2; // limit for function arguments
    Parse pParse;
    pParse.db = &db;
    char tailBuf[128];
    pParse.zTail = tailBuf;
    pParse.nested = 0;

    Token pToken;
    pToken.z = tailBuf + 5;

    ExprList* pList = new ExprList();
    pList->nExpr = 3; // > limit

    Expr* pRes = sqlite3ExprFunction(&pParse, pList, &pToken, SF_ALL);

    gTestsRun++;

    if (pRes == nullptr) {
        failTest("test_too_many_arguments_path", "Expected non-null result when allowed to create function");
        delete pList;
        return;
    }

    // Expect an error message to be produced
    if (gLastErrorMsg.empty() || gLastErrorMsg.find("too many arguments on function") == std::string::npos) {
        failTest("test_too_many_arguments_path", "Expected error message about too many arguments");
        delete pRes;
        delete pList;
        return;
    }

    delete pRes;
    delete pList;
}

void test_too_many_arguments_with_nested_true_path()
{
    // When nested is true, the arg-count check should not trigger error
    gForceExprAllocFail = false;
    gExprListDeleteCount = 0;
    gLastErrorMsg.clear();

    sqlite3 db;
    db.aLimit[0] = 2; // limit
    Parse pParse;
    pParse.db = &db;
    pParse.nested = 1; // nested true disables the "too many arguments" error branch

    char tailBuf[128];
    pParse.zTail = tailBuf;

    Token pToken;
    pToken.z = tailBuf + 1;

    ExprList* pList = new ExprList();
    pList->nExpr = 3; // greater than limit but should not error due to nested

    Expr* pRes = sqlite3ExprFunction(&pParse, pList, &pToken, SF_Distinct);

    gTestsRun++;

    if (pRes == nullptr) {
        failTest("test_too_many_arguments_with_nested_true_path", "Expected non-null result when nested is true");
        delete pList;
        return;
    }

    // Should not have produced an error message
    if (!gLastErrorMsg.empty()) {
        failTest("test_too_many_arguments_with_nested_true_path", "Unexpected error message when nested is true");
        delete pRes;
        delete pList;
        return;
    }

    delete pRes;
    delete pList;
}

// ---------------------------
// Main: run tests
// ---------------------------

int main()
{
    std::cout << "Running tests for sqlite3ExprFunction (simplified harness)..." << std::endl;

    test_normal_path();
    test_allocation_failure_path();
    test_too_many_arguments_path();
    test_too_many_arguments_with_nested_true_path();

    std::cout << "Tests run: " << gTestsRun << ", Failures: " << gFailures << std::endl;
    if (gFailures > 0) {
        return 1;
    }
    return 0;
}