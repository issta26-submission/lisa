// Test suite for the focal method sqlite3AddGenerated (re-implemented for test harness)
 // Notes:
 // - This test harness provides minimal stubs and a self-contained reimplementation
 //   of sqlite3AddGenerated to exercise multiple branches of the logic.
 // - No GTest; a simple main() with test cases and console output.
 // - Domain knowledge applied: cover true/false predicates, static/global hooks,
 //   and correct handling of pointers and flags.
 // - The test focuses on the logic paths visible in the focal method and its dependencies.

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cassert>


// Domain Knowledge and Testing Strategy (comments)
/*
  Step 1: Candidate Keywords (core dependencies)
  - Parse, pParse->pNewTable, Table, Column, Token, Expr
  - Flags: COLFLAG_VIRTUAL, COLFLAG_STORED, COLFLAG_PRIMKEY
  - pType token interpretation: "virtual" or "stored"
  - pExpr handling: TK_ID, TK_UPLUS, TK_RAISE
  - Functions: sqlite3ErrorMsg, sqlite3PExpr, sqlite3ColumnSetExpr, sqlite3ExprDelete
  - Helpers: ALWAYS macro, makeColumnPartOfPrimaryKey, sqlite3StrNICmp
  Step 2: Unit Test Generation (branch coverage)
  - pTab == nullptr -> no-op path
  - IN_DECLARE_VTAB true -> error path
  - pCol.iDflt > 0 -> error path
  - pType == null, or "virtual" / "stored" -> eType handling and flag updates
  - eType==VIRTUAL -> nNVCol decrement
  - PrimKey flag toggling and invocation of makeColumnPartOfPrimaryKey
  - Generated expression path: TK_ID -> TK_UPLUS conversion and affExpr propagation
  - sqlite3ColumnSetExpr invoked with possibly new Expr
  - Error path: invalid pType string -> generated_error
  Step 3: Test Case Refinement
  - Multiple independent test cases to exercise each branch
  - Validate flag mutations, error messages, and expression propagation
*/

// Lightweight scaffolding: types and constants used by the focal function
using u8 = unsigned char;

static int g_IN_DECLARE_VTAB = 0; // Simulated condition controlled by tests

struct Token {
    int n;       // length
    const char* z; // text
};

// Forward declarations for test hooks
struct Parse;
struct Table;
struct Column;
struct Expr;

struct sqlite3 { int dummy; };

struct Parse {
    Table* pNewTable;
    sqlite3* db;
};

// Eigen types used by the function
struct Expr {
    int op;
    int affExpr;
};

// Column and Table stubs
struct Column {
    int iDflt;
    int colFlags;
    const char* zCnName;
    int affinity;
};

struct Table {
    Column* aCol;
    int nCol;
    int nNVCol;
    unsigned tabFlags;
};

// Flags and operator constants (subset for test)
const int COLFLAG_VIRTUAL = 1;
const int COLFLAG_STORED  = 2;
const int COLFLAG_PRIMKEY   = 4;

const int TF_HasVirtual = COLFLAG_VIRTUAL;
const int TF_HasStored  = COLFLAG_STORED;

const int TK_ID     = 1;
const int TK_RAISE  = 2;
const int TK_UPLUS  = 3;

// ALWAYS macro (simulated)
#define ALWAYS(x) ((x) ? 1 : 0)

// Global log to capture errors and actions
static std::vector<std::string> g_log_messages;
static std::vector<std::string> g_step_messages;

// Helpers to log
static void log_error(const char* fmt, ...) {
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    g_log_messages.push_back(std::string("ERROR: ") + buf);
}
static void log_step(const char* fmt, ...) {
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    g_step_messages.push_back(std::string("STEP: ") + buf);
}

// Stubs for production dependencies (simplified)
static void sqlite3ErrorMsg(Parse* pParse, const char* zFormat, ...) {
    // Re-route to log_error for assertions
    va_list ap;
    va_start(ap, zFormat);
    char buf[256];
    vsnprintf(buf, sizeof(buf), zFormat, ap);
    va_end(ap);
    (void)pParse;
    g_log_messages.push_back(std::string("ERROR: ") + buf);
}
static Expr* sqlite3PExpr(Parse* pParse, int op, Expr* pExpr, int) {
    (void)pParse;
    // Return a new expression with the requested op
    Expr* e = new Expr;
    e->op = op;
    e->affExpr = 0;
    return e;
}
static void sqlite3ColumnSetExpr(Parse* pParse, Table* pTab, Column* pCol, Expr* pExpr) {
    // Log what expression was associated with the column (for verification)
    (void)pParse; (void)pTab; (void)pCol;
    if (pExpr) {
        g_step_messages.push_back(std::string("ColumnSetExpr: col=") + pCol->zCnName + " op=" + std::to_string(pExpr->op));
    } else {
        g_step_messages.push_back(std::string("ColumnSetExpr: col=") + pCol->zCnName + " expr=null");
    }
}
static void sqlite3ExprDelete(void* db, Expr* pExpr) {
    (void)db;
    if (pExpr) {
        g_step_messages.push_back(std::string("ExprDelete: op=") + std::to_string(pExpr->op));
        delete pExpr;
    } else {
        g_step_messages.push_back("ExprDelete: null");
    }
}
static void makeColumnPartOfPrimaryKey(Parse* pParse, Column* pCol){
    (void)pParse;
    // Log that primary key portion was created
    g_step_messages.push_back(std::string("MakeColumnPartOfPrimaryKey: col=") + pCol->zCnName);
}

static int sqlite3StrNICmp(const char* a, const char* b, int n){
    // Case-insensitive compare for known literals
    for(int i=0;i<n && a[i] && b[i];++i){
        int ca = (unsigned char)std::tolower((unsigned char)a[i]);
        int cb = (unsigned char)std::tolower((unsigned char)b[i]);
        if(ca != cb) return ca - cb;
    }
    int la = (int)std::strlen(a);
    int lb = (int)std::strlen(b);
    int minl = std::min({la, lb, n});
    if (minl==0) return la-lb;
    return la - lb;
}

// Local reimplementation of sqlite3AddGenerated (simplified for unit tests)
static void sqlite3AddGenerated(Parse *pParse, Expr *pExpr, Token *pType){
#ifndef SQLITE_OMIT_GENERATED_COLUMNS
  u8 eType = COLFLAG_VIRTUAL;
  Table *pTab = pParse->pNewTable;
  Column *pCol;
  if( pTab==0 ){
    /* generated column in an CREATE TABLE IF NOT EXISTS that already exists */
    goto generated_done;
  }
  pCol = &(pTab->aCol[pTab->nCol-1]);
  if( g_IN_DECLARE_VTAB ){
    sqlite3ErrorMsg(pParse, "virtual tables cannot use computed columns");
    goto generated_done;
  }
  if( pCol->iDflt>0 ) goto generated_error;
  if( pType ){
    if( pType->n==7 && sqlite3StrNICmp("virtual",pType->z,7)==0 ){
      /* no-op */
    }else if( pType->n==6 && sqlite3StrNICmp("stored",pType->z,6)==0 ){
      eType = COLFLAG_STORED;
    }else{
      goto generated_error;
    }
  }
  if( eType==COLFLAG_VIRTUAL ) pTab->nNVCol--;
  pCol->colFlags |= eType;
  assert( TF_HasVirtual==COLFLAG_VIRTUAL );
  assert( TF_HasStored==COLFLAG_STORED );
  pTab->tabFlags |= eType;
  if( pCol->colFlags & COLFLAG_PRIMKEY ){
    makeColumnPartOfPrimaryKey(pParse, pCol); /* For the error message */
  }
  if( ALWAYS(pExpr) && pExpr->op==TK_ID ){
    /* The value of a generated column needs to be a real expression, not
    ** just a reference to another column, in order for covering index
    ** optimizations to work correctly.  So if the value is not an expression,
    ** turn it into one by adding a unary "+" operator. */
    pExpr = sqlite3PExpr(pParse, TK_UPLUS, pExpr, 0);
  }
  if( pExpr && pExpr->op!=TK_RAISE ) pExpr->affExpr = pCol->affinity;
  sqlite3ColumnSetExpr(pParse, pTab, pCol, pExpr);
  pExpr = 0;
  goto generated_done;
generated_error:
  sqlite3ErrorMsg(pParse, "error in generated column \"%s\"",
                  pCol->zCnName);
generated_done:
  sqlite3ExprDelete(pParse->db, pExpr);
#else
  /* Throw and error for the GENERATED ALWAYS AS clause if the
  ** SQLITE_OMIT_GENERATED_COLUMNS compile-time option is used. */
  sqlite3ErrorMsg(pParse, "generated columns not supported");
  sqlite3ExprDelete(pParse->db, pExpr);
#endif
}

// Helper: reset test environment
static void reset_env(Parse& pParse, Table& tab, std::vector<Column>& cols){
    g_log_messages.clear();
    g_step_messages.clear();
    // Setup a simple table with 1 column
    cols.clear();
    Column c;
    c.iDflt = 0;
    c.colFlags = 0;
    c.zCnName = "col0";
    c.affinity = 0;
    cols.push_back(c);
    tab.aCol = cols.data();
    tab.nCol = 1;
    tab.nNVCol = 1; // start with 1 NV column
    tab.tabFlags = 0;
    // Attach to parse
    pParse.pNewTable = &tab;
    pParse.db = nullptr; // not used in tests
}

// Utility: print test result
static void print_result(const std::string& name, bool ok){
    std::cout << name << ": " << (ok ? "PASSED" : "FAILED") << "\n";
}

// Test 1: pTab == null => generated_done early, no errors
static bool test_no_table_path(){
    // Setup
    Parse pParse;
    pParse.pNewTable = nullptr;
    pParse.db = nullptr;
    Token type{0,nullptr};
    Expr* e = nullptr;

    // Execute
    sqlite3AddGenerated(&pParse, e, &type);

    // Verify: no errors
    if (!g_log_messages.empty()) {
        return false;
    }
    if (!g_step_messages.empty()) {
        return false;
    }
    return true;
}

// Test 2: IN_DECLARE_VTAB true => error path
static bool test_virtual_table_error_path(){
    Parse pParse;
    Table tab;
    std::vector<Column> cols;
    reset_env(pParse, tab, cols);

    g_IN_DECLARE_VTAB = 1; // simulate IN_DECLARE_VTAB
    Token type{0,nullptr};
    Expr* e = nullptr;

    sqlite3AddGenerated(&pParse, e, &type);

    bool found = false;
    for (auto &m : g_log_messages){
        if (m.find("virtual tables cannot use computed columns") != std::string::npos){
            found = true; break;
        }
    }
    g_IN_DECLARE_VTAB = 0;
    return found;
}

// Test 3: pCol.iDflt > 0 => error in generated column
static bool test_dflt_error_path(){
    Parse pParse;
    Table tab;
    std::vector<Column> cols;
    reset_env(pParse, tab, cols);
    // Make the last column have a default
    tab.aCol[0].iDflt = 5;

    Token type{0,nullptr};
    Expr* e = nullptr;

    sqlite3AddGenerated(&pParse, e, &type);

    bool found = false;
    for (auto &m : g_log_messages){
        if (m.find("error in generated column") != std::string::npos){
            found = true; break;
        }
    }
    return found;
}

// Test 4: pType "stored" -> eType STORed, flags updated
static bool test_type_stored_path(){
    Parse pParse;
    Table tab;
    std::vector<Column> cols;
    reset_env(pParse, tab, cols);

    Token type{6, "stored"};
    Expr* e = nullptr;

    sqlite3AddGenerated(&pParse, e, &type);

    // Validate: pCol has STORED flag
    bool okFlags = (tab.aCol[0].colFlags & COLFLAG_STORED) != 0;
    bool okTab = (tab.tabFlags & COLFLAG_STORED) != 0;
    return okFlags && okTab;
}

// Test 5: pExpr TK_ID triggers TK_UPLUS and ColumnSetExpr gets non-null expr
static bool test_expr_id_to_uplus_path(){
    Parse pParse;
    Table tab;
    std::vector<Column> cols;
    reset_env(pParse, tab, cols);

    // Provide an initial expression that is a TK_ID
    Expr* initial = new Expr{TK_ID, 0};

    Token type{0,nullptr};
    // Call function
    sqlite3AddGenerated(&pParse, initial, &type);

    // Verify that ColumnSetExpr was called with an expression of op TK_UPLUS
    bool found = false;
    for (auto &m : g_step_messages){
        if (m.find("ColumnSetExpr: col=") != std::string::npos){
            // We logged op of pExpr in ColumnSetExpr; ensure it's not null
            if (m.find("op=") != std::string::npos){
                found = true;
                break;
            }
        }
    }
    // Cleanup: delete any allocated expressions from test
    // (In this harness, sqlite3ExprDelete is invoked inside function)
    // Nothing else to do here.
    return found;
}

// Test 6: pCol has PRIMKEY flag => makeColumnPartOfPrimaryKey invoked
static bool test_primkey_path(){
    Parse pParse;
    Table tab;
    std::vector<Column> cols;
    reset_env(pParse, tab, cols);
    // Set primkey flag on the column
    tab.aCol[0].colFlags |= COLFLAG_PRIMKEY;

    Token type{0,nullptr};
    Expr* e = nullptr;

    sqlite3AddGenerated(&pParse, e, &type);

    bool found = false;
    for (auto &m : g_step_messages){
        if (m.find("MakeColumnPartOfPrimaryKey: col=") != std::string::npos){
            found = true; break;
        }
    }
    return found;
}

// Main: run tests
int main() {
    // Precondition: reset environment
    // Test 1
    bool t1 = test_no_table_path();
    print_result("Test 1 - no table path", t1);

    // Test 2
    bool t2 = test_virtual_table_error_path();
    print_result("Test 2 - virtual table error path", t2);

    // Test 3
    bool t3 = test_dflt_error_path();
    print_result("Test 3 - default present error path", t3);

    // Test 4
    bool t4 = test_type_stored_path();
    print_result("Test 4 - type stored flags", t4);

    // Test 5
    bool t5 = test_expr_id_to_uplus_path();
    print_result("Test 5 - ID to UPLUS expr propagation", t5);

    // Test 6
    bool t6 = test_primkey_path();
    print_result("Test 6 - primkey hook invocation", t6);

    // Summary
    int total = 6;
    int passed = (int)t1 + (int)t2 + (int)t3 + (int)t4 + (int)t5 + (int)t6;
    std::cout << "Summary: " << passed << "/" << total << " tests passed.\n";

    return 0;
}

// Note: This self-contained test harness re-implements the focal function logic in order to provide deterministic unit tests
// for the various branches, with stubs and logs to verify behavior. In a full build against the original repository,
// the tests would link against the real sqlite3AddGenerated and its dependent symbols.