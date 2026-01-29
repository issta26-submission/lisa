// Lightweight C++11 unit test harness for sqlite3ComputeGeneratedColumns
// NOTE: This is a self-contained test harness with minimal stubbed SQLite-like types.
// It is designed to exercise specific branches of sqlite3ComputeGeneratedColumns without
// requiring the full SQLite source tree or GTest. It uses a small in-file "framework"
// to report non-terminating test results.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cstdarg>
#include <iostream>


// Domain-specific non-terminating assertion helpers
static std::vector<std::string> g_testFailures;
#define CHECK(cond, msg) do { if(!(cond)) g_testFailures.push_back(msg); } while(0)
static void reportResults() {
    if(g_testFailures.empty()) {
        std::cout << "[PASS] All tests passed.\n";
    } else {
        std::cout << "[FAIL] " << g_testFailures.size() << " test(s) failed:\n";
        for(auto &m : g_testFailures) std::cout << "  - " << m << "\n";
    }
}

// Global knobs to simulate internal behavior
static bool g_forceNotAvail = false;            // Force NOTAVAIL from sqlite3WalkExpr
static std::string g_lastErrorMsg;              // Captured error message from sqlite3ErrorMsg

// SQLite-like constants and helpers (subset for the focal method)
#define TF_HasGenerated 1
#define TF_HasVirtual   2
#define TF_HasStored    4

#define COLFLAG_VIRTUAL 1
#define COLFLAG_STORED    2
#define COLFLAG_GENERATED 4
#define COLFLAG_NOTAVAIL  8
#define COLFLAG_BUSY      16

#define SQLITE_AFF_NONE 0

enum OpCode { OP_Affinity = 1, OP_TypeCheck = 2 };

// Minimal type stubs to support sqlite3ComputeGeneratedColumns
struct Expr {
    int dummy;
};

struct VdbeOp {
    int opcode;
    struct {
        char *z;
    } p4;
    int p4type;
    int p3;
};

struct Vdbe {
    VdbeOp *pLast;
};

// Forward declarations of internal helpers (stubs)
struct Table;
struct Column;
struct Parse;
struct Walker;

struct Parse {
    Vdbe *pVdbe;
    int iSelfTab;
};

struct Column {
    int colFlags;
    const char *zCnName;
};

struct Table {
    int tabFlags;
    int nCol;
    Column *aCol;
    const char *zName;
};

// Walker and callback placeholders
struct Walker {
    union {
        Table *pTab;
    } u;
    int (*xExprCallback)(Walker*, Expr*);
    int (*xSelectCallback)(Walker*, void*);
    int (*xSelectCallback2)(Walker*, void*);
    int eCode;
};

// Static function prototypes (supplied by the test harness)
static int exprColumnFlagUnion(Walker *pWalker, Expr *pExpr) { (void)pWalker; (void)pExpr; return 0; }
static int sqlite3TableAffinity(Vdbe *v, Table *pTab, int iReg) { (void)v; (void)pTab; (void)iReg; return 0; }
static VdbeOp* sqlite3VdbeGetLastOp(Vdbe *p) { return p ? p->pLast : nullptr; }
static Expr* sqlite3ColumnExpr(Table *pTab, Column *pCol) { (void)pTab; (void)pCol; static Expr dummy; return &dummy; }
static void sqlite3ExprCodeGeneratedColumn(Parse *pParse, Table *pTab, Column *pCol, int x) { (void)pParse; (void)pTab; (void)pCol; (void)x; }
static int sqlite3TableColumnToStorage(Table *pTab, int iCol) { (void)pTab; (void)iCol; return 0; }

static void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...) {
    (void)pParse;
    va_list ap;
    va_start(ap, zFormat);
    char buf[256];
    vsnprintf(buf, sizeof(buf), zFormat, ap);
    va_end(ap);
    g_lastErrorMsg = buf;
}

// Dummy walk function that uses g_forceNotAvail to signal NOTAVAIL
static void sqlite3WalkExpr(Walker *pWalker, Expr *pExpr) {
    (void)pExpr;
    // If global flag is set, pretend we couldn't compute dependencies
    pWalker->eCode = g_forceNotAvail ? COLFLAG_NOTAVAIL : 0;
}

static int sqlite3ExprCodeTarget(Parse *pParse, Table *pTab, Column *pCol, int x) {
    (void)pParse; (void)pTab; (void)pCol; (void)x;
    return 0;
}

// The focal method reproduced for testing with our stubs
void sqlite3ComputeGeneratedColumns(
  Parse *pParse,    /* Parsing context */
  int iRegStore,    /* Register holding the first column */
  Table *pTab       /* The table */
){
  int i;
  Walker w;
  Column *pRedo;
  int eProgress;
  VdbeOp *pOp;
  // Ensure the table has generated columns
  if(!(pTab->tabFlags & TF_HasGenerated)) return;
  // Debug/test hooks
  // testcase in real SQLite is a macro; we ignore

  sqlite3TableAffinity(pParse->pVdbe, pTab, iRegStore);
  if( (pTab->tabFlags & TF_HasStored)!=0 ){
    pOp = sqlite3VdbeGetLastOp(pParse->pVdbe);
    if( pOp->opcode==OP_Affinity ){
      int ii, jj;
      char *zP4 = pOp->p4.z;
      // Defensive checks
      if(zP4==nullptr) {
        zP4 = 0;
      } else {
        // Apply NONE affinity to non-virtual stored columns
        for(ii=jj=0; zP4[jj]; ii++){
          if( pTab->aCol[ii].colFlags & COLFLAG_VIRTUAL ){
            continue;
          }
          if( pTab->aCol[ii].colFlags & COLFLAG_STORED ){
            zP4[jj] = SQLITE_AFF_NONE;
          }
          jj++;
        }
      }
    }else if( pOp->opcode==OP_TypeCheck ){
      /* If an OP_TypeCheck was generated because the table is STRICT,
      ** then set the P3 operand to indicate that generated columns should
      ** not be checked */
      pOp->p3 = 1;
    }
  }
  /* Two-pass algorithm for generated columns */
  for(i=0; i<pTab->nCol; i++){
    if( pTab->aCol[i].colFlags & COLFLAG_GENERATED ){
      // Mark as not available initially
      pTab->aCol[i].colFlags |= COLFLAG_NOTAVAIL;
    }
  }
  w.u.pTab = pTab;
  w.xExprCallback = exprColumnFlagUnion;
  w.xSelectCallback = 0;
  w.xSelectCallback2 = 0;
  pParse->iSelfTab = -iRegStore;
  do{
    eProgress = 0;
    pRedo = 0;
    for(i=0; i<pTab->nCol; i++){
      Column *pCol = pTab->aCol + i;
      if( (pCol->colFlags & COLFLAG_NOTAVAIL)!=0 ){
        int x;
        pCol->colFlags |= COLFLAG_BUSY;
        w.eCode = 0;
        sqlite3WalkExpr(&w, sqlite3ColumnExpr(pTab, pCol));
        pCol->colFlags &= ~COLFLAG_BUSY;
        if( w.eCode & COLFLAG_NOTAVAIL ){
          pRedo = pCol;
          continue;
        }
        eProgress = 1;
        // Column is now available; generate code for it
        if(!(pCol->colFlags & COLFLAG_GENERATED)) {
          // Shouldn't happen in our test harness, but guard anyway
          continue;
        }
        x = sqlite3TableColumnToStorage(pTab, i) + iRegStore;
        sqlite3ExprCodeGeneratedColumn(pParse, pTab, pCol, x);
        pCol->colFlags &= ~COLFLAG_NOTAVAIL;
      }
    }
  }while( pRedo && eProgress );
  if( pRedo ){
    sqlite3ErrorMsg(pParse, "generated column loop on \"%s\"", pRedo->zCnName);
  }
  pParse->iSelfTab = 0;
}

// Helper to create and manage test objects
static Table* makeTable(int nCol, int tabFlags, const char* name) {
    Table* t = new Table;
    t->tabFlags = tabFlags;
    t->nCol = nCol;
    t->zName = name;
    t->aCol = new Column[nCol];
    for(int i=0; i<nCol; ++i) {
        t->aCol[i].colFlags = 0;
        t->aCol[i].zCnName = nullptr;
    }
    return t;
}

static void freeTable(Table* t) {
    if(!t) return;
    delete[] t->aCol;
    delete t;
}

static void setColName(Table* t, int idx, const char* name) {
    if(idx>=0 && idx < t->nCol) t->aCol[idx].zCnName = name;
}

// Test 1: AFFINITY path when there are stored columns
static void test_affinity_stored_branch() {
    // Prepare parse and Vdbe with a last op OP_Affinity
    Parse parse;
    Vdbe vdbe;
    VdbeOp op;
    op.opcode = OP_Affinity;
    // zP4 must be a null-terminated string; length >= number of non-virtual stored columns
    const int nCol = 3;
    char zP4buf[4] = { 'A', 'B', 0, 0 }; // initial content; last byte is terminator
    op.p4.z = zP4buf;
    op.p4type = 1; // P4_DYNAMIC
    op.p3 = 0;
    vdbe.pLast = &op;
    parse.pVdbe = &vdbe;

    // Build table with 3 columns: first two stored, third non-stored
    Table* pTab = makeTable(nCol, TF_HasGenerated | TF_HasStored, "t_affinity");
    pTab->aCol[0].colFlags = COLFLAG_STORED;
    pTab->aCol[1].colFlags = COLFLAG_STORED;
    pTab->aCol[2].colFlags = 0; // not stored, not virtual
    setColName(pTab, 0, "c0");
    setColName(pTab, 1, "c1");
    setColName(pTab, 2, "c2");

    // iRegStore points to first register holding first column
    int iRegStore = 0;

    // Call focal method
    g_forceNotAvail = false;
    g_lastErrorMsg.clear();
    sqlite3ComputeGeneratedColumns(&parse, iRegStore, pTab);

    // Expect stored columns' zP4 values to be set to NONE
    bool ok0 = (op.p4.z[0] == SQLITE_AFF_NONE);
    bool ok1 = (op.p4.z[1] == SQLITE_AFF_NONE);
    CHECK(ok0, "Affinity not NONE for first stored column");
    CHECK(ok1, "Affinity not NONE for second stored column");

    freeTable(pTab);
    // No explicit assertion framework; failures added to g_testFailures
    if(!g_testFailures.empty()) {
        // Do nothing; tests collected
    }
}

// Test 2: OP_TypeCheck branch sets pOp->p3 = 1
static void test_typecheck_branch() {
    Parse parse;
    Vdbe vdbe;
    VdbeOp op;
    op.opcode = OP_TypeCheck;
    op.p4.z = nullptr;
    op.p4type = 0;
    op.p3 = 0;
    vdbe.pLast = &op;
    parse.pVdbe = &vdbe;

    // Table with at least one stored/generated column
    Table* pTab = makeTable(2, TF_HasGenerated | TF_HasStored, "t_typecheck");
    pTab->aCol[0].colFlags = COLFLAG_GENERATED; // generated column
    pTab->aCol[1].colFlags = 0;
    setColName(pTab, 0, "gc0");
    setColName(pTab, 1, "col1");

    int iRegStore = 0;

    g_forceNotAvail = false;
    g_lastErrorMsg.clear();
    sqlite3ComputeGeneratedColumns(&parse, iRegStore, pTab);

    bool ok = (op.p3 == 1);
    CHECK(ok, "OP_TypeCheck did not set p3=1 as expected");
    freeTable(pTab);
}

// Test 3: NOTAVAIL path triggers error message "generated column loop on"
static void test_notavail_error_path() {
    Parse parse;
    Vdbe vdbe;
    VdbeOp op;
    op.opcode = OP_Affinity;
    char zP4buf[3] = { 'X', 'Y', 0 };
    op.p4.z = zP4buf;
    op.p4type = 1;
    op.p3 = 0;
    vdbe.pLast = &op;
    parse.pVdbe = &vdbe;

    // Table with one generated column; set NOTAVAIL path
    Table* pTab = makeTable(2, TF_HasGenerated | TF_HasStored, "t_notavail");
    pTab->aCol[0].colFlags = COLFLAG_GENERATED; // generated column
    pTab->aCol[1].colFlags = 0;
    setColName(pTab, 0, "gen0");
    setColName(pTab, 1, "col1");

    int iRegStore = 0;

    g_forceNotAvail = true; // Force NOTAVAIL through walker
    g_lastErrorMsg.clear();

    sqlite3ComputeGeneratedColumns(&parse, iRegStore, pTab);

    // Expect an error message to have been generated
    CHECK(!g_lastErrorMsg.empty(), "Expected error message for generated column loop");
    if(!g_lastErrorMsg.empty()) {
        // Basic sanity: the message should mention 'generated column loop'
        CHECK(g_lastErrorMsg.find("generated column loop") != std::string::npos,
              "Error message does not contain expected text");
    }

    freeTable(pTab);
}

int main() {
    // Run all tests
    test_affinity_stored_branch();
    test_typecheck_branch();
    test_notavail_error_path();

    // Post-test reporting
    reportResults();
    return g_testFailures.empty() ? 0 : 1;
}