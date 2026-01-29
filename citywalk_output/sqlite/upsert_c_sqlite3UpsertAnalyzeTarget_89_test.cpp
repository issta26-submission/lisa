// Unit test suite for sqlite3UpsertAnalyzeTarget
// This test suite is implemented in C++11 without Google Test.
// It provides minimal stubs and a self-contained copy of sqlite3UpsertAnalyzeTarget
// adapted for the test environment. The tests cover:
// - Rowid conflict target path (should succeed)
// - Conflict target matching a primary/unique index (should succeed)
// - Conflict target not matching any constraint (should return SQLITE_ERROR)

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <string>
#include <cstdlib>


// Domain: define minimal SQLite-like types and constants used by sqlite3UpsertAnalyzeTarget

// Simple status codes (subset)
#define SQLITE_OK 0
#define SQLITE_ERROR 1

// Token kinds and constants
const int TK_COLUMN = 1;
const int TK_COLLATE = 2;
const int XN_ROWID = -1;  // special column constant used to denote ROWID

// Forward declarations
struct Parse;
struct SrcList;
struct Expr;
struct ExprList;
struct Upsert;
struct Table;
struct Index;
struct NameContext;

// Lightweight structures to mimic SQLite internal types (minimal subset for tests)
struct Expr {
    int op;                 // operation type
    int iColumn;            // column index, or XN_ROWID
    Expr* pLeft;             // left subtree (used for sCol[0].pLeft)
    struct {
        const char* zToken; // token name (for COLLATE/literal stubs)
    } u;
    // For test simplicity, we keep a flat structure
};

struct ExprList {
    int nExpr;
    struct ExprListItem { Expr* pExpr; } *a; // dynamic array for test flexibility
};

struct SrcList {
    int nSrc;
    struct SrcListItem { Table* pTab; int iCursor; } *a; // single element is enough for tests
};

struct Upsert {
    ExprList* pUpsertTarget;
    Expr* pUpsertTargetWhere;
    Index* pUpsertIdx;
    Upsert* pNextUpsert;
};

// Table and Index stubs
struct Table {
    Index* pIndex;
    bool hasRowid;
};

struct Index {
    Index* pNext;
    int nKeyCol;
    int* aiColumn;          // column indices for key
    const char** azColl;     // column names
    ExprList* aColExpr;       // not used in tests (simplified)
    bool bHasExpr;            // not used in tests
    void* pPartIdxWhere;        // null in tests
    bool bUnique;
};

// Name context stub (unused in tests but required by function)
struct NameContext {
    Parse* pParse;
    SrcList* pSrcList;
};

// Parse stub
struct Parse { };

// Globals for test control
static int g_exprCompareReturn = 0; // controls sqlite3ExprCompare return value in tests

// Prototypes for helpers used by sqlite3UpsertAnalyzeTarget
static int sqlite3ResolveExprListNames(NameContext* pNC, ExprList* pTarget) { (void)pNC; (void)pTarget; return 0; }
static int sqlite3ResolveExprNames(NameContext* pNC, Expr* pExpr) { (void)pNC; (void)pExpr; return 0; }

static int sqlite3ExprCompare(Parse* pParse, Expr* pExpr1, Expr* pExpr2, int iCursor) {
    (void)pParse; (void)iCursor;
    // In tests, control the return value to drive branches
    return g_exprCompareReturn;
}

static void sqlite3ErrorMsg(Parse* pParse, const char* zFormat, ...) {
    // In tests, we don't need to store messages, but provide variadic interface
    (void)pParse;
    va_list ap;
    va_start(ap, zFormat);
    (void)va_arg(ap, void*); // avoid unused warning for variadic
    va_end(ap);
}

// Minimal snprintf wrapper to satisfy calls in the focal method
static int sqlite3_snprintf(int n, char* z, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vsnprintf(z, n, fmt, ap);
    va_end(ap);
    return ret;
}

// Helpers to simulate DB features
static int HasRowid(const Table* pTab) {
    return (pTab != nullptr) && pTab->hasRowid;
}
static int IsUniqueIndex(const Index* pIdx) {
    return (pIdx != nullptr) && pIdx->bUnique;
}
static int sqlite3ExprListBool(const ExprList* list) { (void)list; return 1; }

// The focal function under test (adapted to work with the simplified test framework)
extern "C" int sqlite3UpsertAnalyzeTarget(
  Parse *pParse,     /* The parsing context */
  SrcList *pTabList, /* Table into which we are inserting */
  Upsert *pUpsert    /* The ON CONFLICT clauses */
){
    Table *pTab;            /* That table into which we are inserting */
    int rc;                 /* Result code */
    int iCursor;            /* Cursor used by pTab */
    Index *pIdx;            /* One of the indexes of pTab */
    ExprList *pTarget;      /* The conflict-target clause */
    Expr *pTerm;            /* One term of the conflict-target clause */
    NameContext sNC;        /* Context for resolving symbolic names */
    Expr sCol[2];           /* Index column converted into an Expr */
    int nClause = 0;        /* Counter of ON CONFLICT clauses */
    // Basic sanity checks (as in original)
    if(!(pTabList)) return SQLITE_ERROR;
    if( pTabList->nSrc!=1 ) { /* keep safety in tests */ return SQLITE_ERROR; }
    if( pTabList->a[0].pTab==0 ) { return SQLITE_ERROR; }
    if( pUpsert==0 ) { return SQLITE_ERROR; }
    if( pUpsert->pUpsertTarget==0 ) { return SQLITE_ERROR; }

    // Resolve all symbolic names in the conflict-target clause (mocked)
    memset(&sNC, 0, sizeof(sNC));
    sNC.pParse = pParse;
    sNC.pSrcList = pTabList;
    for(; pUpsert && pUpsert->pUpsertTarget;
        pUpsert=pUpsert->pNextUpsert, nClause++){
        rc = sqlite3ResolveExprListNames(&sNC, pUpsert->pUpsertTarget);
        if( rc ) return rc;
        rc = sqlite3ResolveExprNames(&sNC, pUpsert->pUpsertTargetWhere);
        if( rc ) return rc;

        /* Check to see if the conflict target matches the rowid. */  
        pTab = pTabList->a[0].pTab;
        pTarget = pUpsert->pUpsertTarget;
        iCursor = pTabList->a[0].iCursor;
        if( HasRowid(pTab) 
         && pTarget->nExpr==1
         && (pTerm = pTarget->a[0].pExpr)->op==TK_COLUMN
         && pTerm->iColumn==XN_ROWID
        ){
          /* The conflict-target is the rowid of the primary table */
          assert( pUpsert->pUpsertIdx==0 );
          continue;
        }

        /* Initialize sCol[0..1] to be an expression parse tree for a
        ** single column of an index.  The sCol[0] node will be the TK_COLLATE
        ** operator and sCol[1] will be the TK_COLUMN operator.  Code below
        ** will populate the specific collation and column number values
        ** prior to comparing against the conflict-target expression.
        */
        memset(sCol, 0, sizeof(sCol));
        sCol[0].op = TK_COLLATE;
        sCol[0].pLeft = &sCol[1];
        sCol[1].op = TK_COLUMN;
        sCol[1].iTable = pTabList->a[0].iCursor; // simulate table cursor

        /* Check for matches against other indexes */
        for(pIdx=pTab->pIndex; pIdx; pIdx=pIdx->pNext){
            int ii, jj, nn;
            if( !IsUniqueIndex(pIdx) ) continue;
            if( pTarget->nExpr!=pIdx->nKeyCol ) continue;
            if( pIdx->pPartIdxWhere ){
                if( pUpsert->pUpsertTargetWhere==0 ) continue;
                if( sqlite3ExprCompare(pParse, pUpsert->pUpsertTargetWhere,
                                       pIdx->pPartIdxWhere, iCursor)!=0 ){
                    continue;
                }
            }
            nn = pIdx->nKeyCol;
            for(ii=0; ii<nn; ii++){
                Expr *pExpr;
                sCol[0].u.zToken = (char*)pIdx->azColl[ii];
                if( pIdx->aiColumn[ii]==XN_EXPR ){
                    assert( pIdx->aColExpr!=0 );
                    assert( pIdx->aColExpr->nExpr>ii );
                    assert( pIdx->bHasExpr );
                    pExpr = pIdx->aColExpr->a[ii].pExpr;
                    if( pExpr->op!=TK_COLLATE ){
                        sCol[0].pLeft = pExpr;
                        pExpr = &sCol[0];
                    }
                }else{
                    sCol[0].pLeft = &sCol[1];
                    sCol[1].iColumn = pIdx->aiColumn[ii];
                    pExpr = &sCol[0];
                }
                for(jj=0; jj<nn; jj++){
                    if( sqlite3ExprCompare(0,pTarget->a[jj].pExpr,pExpr,iCursor)<2 ){
                        break;  /* Column ii of the index matches column jj of target */
                    }
                }
                if( jj>=nn ){
                    /* The target contains no match for column jj of the index */
                    break;
                }
            }
            if( ii<nn ){
                /* Column ii of the index did not match any term of the conflict target.
                ** Continue the search with the next index. */
                continue;
            }
            pUpsert->pUpsertIdx = pIdx;
            break;
        }
        if( pUpsert->pUpsertIdx==0 ){
            char zWhich[16];
            if( nClause==0 && pUpsert->pNextUpsert==0 ){
                zWhich[0] = 0;
            }else{
                sqlite3_snprintf(sizeof(zWhich),zWhich,"%r ", nClause+1);
            }
            sqlite3ErrorMsg(pParse, "%sON CONFLICT clause does not match any "
                                    "PRIMARY KEY or UNIQUE constraint", zWhich);
            return SQLITE_ERROR;
        }
    }
    return SQLITE_OK;
}

// Additional test helpers for dynamic test input construction
static Expr* NewExpr(int op, int iColumn) {
    Expr* e = (Expr*)malloc(sizeof(Expr));
    e->op = op;
    e->iColumn = iColumn;
    e->pLeft = nullptr;
    e->u.zToken = nullptr;
    return e;
}

static ExprList* NewExprList(int nExpr, Expr* first) {
    ExprList* el = (ExprList*)malloc(sizeof(ExprList));
    el->nExpr = nExpr;
    el->a = nullptr;
    if(nExpr>0) {
        el->a = new ExprList::ExprListItem[nExpr];
        for(int i=0; i<nExpr; ++i) el->a[i].pExpr = nullptr;
        el->a[0].pExpr = first;
    }
    return el;
}

// Test 1: Rowid path should succeed (rc == SQLITE_OK)
void test_RowidConflict_OK() {
    // Set up test objects
    // Upsert with a conflict-target of ROWID
    Expr* pExprRowid = NewExpr(TK_COLUMN, XN_ROWID);
    ExprList* pTarget = NewExprList(1, pExprRowid);

    Upsert up;
    up.pUpsertTarget = pTarget;
    up.pUpsertTargetWhere = nullptr;
    up.pUpsertIdx = nullptr;
    up.pNextUpsert = nullptr;

    Table tab;
    tab.pIndex = nullptr;
    tab.hasRowid = true; // triggers ROWID path
    Upsert* pUpsert = &up;

    SrcList tabList;
    tabList.nSrc = 1;
    tabList.a = new SrcList::SrcListItem[1];
    tabList.a[0].pTab = &tab;
    tabList.a[0].iCursor = 0;

    Parse parse;
    int rc = sqlite3UpsertAnalyzeTarget(&parse, &tabList, pUpsert);

    printf("Test RowidConflict_OK: rc=%d (expected %d)\n", rc, SQLITE_OK);

    // Cleanup
    free(pExprRowid);
    // pTarget owned by upsert on heap in a real scenario; here freed by allocator
    delete[] tabList.a;
}

// Test 2: Conflict target matches a unique index -> should succeed
void test_IndexMatch_OK() {
    // Create a target with 1 expression
    Expr* pExprTarget = NewExpr(TK_COLUMN, 0);
    ExprList* pTarget = NewExprList(1, pExprTarget);

    // Create an index: UNIQUE, nKeyCol=1, aiColumn[0]=0, azColl[0]="col0"
    int* aiColumn = new int[1];
    aiColumn[0] = 0;
    const char* coll0 = "col0";
    const char** azColl = new const char*[1];
    azColl[0] = coll0;

    Index idx;
    idx.pNext = nullptr;
    idx.nKeyCol = 1;
    idx.aiColumn = aiColumn;
    idx.azColl = azColl;
    idx.aColExpr = nullptr;
    idx.bHasExpr = false;
    idx.pPartIdxWhere = nullptr;
    idx.bUnique = true;
    idx.pNext = nullptr;

    Table tab;
    tab.pIndex = &idx;
    tab.hasRowid = false;

    Upsert up;
    up.pUpsertTarget = pTarget;
    up.pUpsertTargetWhere = nullptr;
    up.pUpsertIdx = nullptr;
    up.pNextUpsert = nullptr;

    Upsert* pUpsert = &up;
    SrcList tabList;
    tabList.nSrc = 1;
    tabList.a = new SrcList::SrcListItem[1];
    tabList.a[0].pTab = &tab;
    tabList.a[0].iCursor = 0;

    Parse parse;
    int rc = sqlite3UpsertAnalyzeTarget(&parse, &tabList, pUpsert);

    printf("Test IndexMatch_OK: rc=%d (expected %d)\n", rc, SQLITE_OK);

    // Cleanup
    delete[] aiColumn;
    delete[] azColl;
    delete[] tabList.a;
    free(pExprTarget);
}

// Test 3: Conflict target does not match any constraint -> should return SQLITE_ERROR
void test_NoMatchConstraint_ERROR() {
    // Target with 1 expression
    Expr* pExprTarget = NewExpr(TK_COLUMN, 0);
    ExprList* pTarget = NewExprList(1, pExprTarget);

    // Create a non-matching index: nKeyCol=2 (target has nExpr=1)
    int* aiColumn = new int[2];
    aiColumn[0] = 0;
    aiColumn[1] = 1;
    const char* coll0 = "col0";
    const char** azColl = new const char*[2];
    azColl[0] = coll0;
    azColl[1] = "col1";

    Index idx;
    idx.pNext = nullptr;
    idx.nKeyCol = 2;
    idx.aiColumn = aiColumn;
    idx.azColl = azColl;
    idx.aColExpr = nullptr;
    idx.bHasExpr = false;
    idx.pPartIdxWhere = nullptr;
    idx.bUnique = true; // could be true or false; not relevant since nKeyCol != nExpr
    idx.pNext = nullptr;

    Table tab;
    tab.pIndex = &idx;
    tab.hasRowid = false;

    Upsert up;
    up.pUpsertTarget = pTarget;
    up.pUpsertTargetWhere = nullptr;
    up.pUpsertIdx = nullptr;
    up.pNextUpsert = nullptr;

    Upsert* pUpsert = &up;

    SrcList tabList;
    tabList.nSrc = 1;
    tabList.a = new SrcList::SrcListItem[1];
    tabList.a[0].pTab = &tab;
    tabList.a[0].iCursor = 0;

    Parse parse;
    int rc = sqlite3UpsertAnalyzeTarget(&parse, &tabList, pUpsert);

    printf("Test NoMatchConstraint_ERROR: rc=%d (expected %d)\n", rc, SQLITE_ERROR);

    // Cleanup
    delete[] aiColumn;
    delete[] azColl;
    delete[] tabList.a;
    free(pExprTarget);
}

// Entry point to run tests
int main() {
    test_RowidConflict_OK();
    test_IndexMatch_OK();
    test_NoMatchConstraint_ERROR();
    return 0;
}