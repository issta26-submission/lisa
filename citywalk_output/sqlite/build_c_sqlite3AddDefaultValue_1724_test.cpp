// Minimal, self-contained C++11 test suite for the focal SQLite-like function
// sqlite3AddDefaultValue, re-implemented in this test to allow isolated testing
// without bringing in the entire SQLite runtime.
// The tests cover key branches: no table, non-constant default, generated column,
// and a valid default value path.

// Note: This test suite is designed to be compiled with a C++11 compiler.
// It uses simple runtime assertions and prints results to stdout.

#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <string>
#include <iostream>
#include <cassert>


// Domain-oriented keywords (for test traceability)
 // Candidate Keywords: sqlite3ExprIsConstantOrFunction, COLFLAG_GENERATED, COLFLAG_VIRTUAL,
 // COLFLAG_STORED, TK_SPAN, EP_Skip, EXPRDUP_REDUCE, sqlite3ErrorMsg, sqlite3ColumnSetExpr

// Static utilities and stubs to simulate minimal SQLite-like behavior

// Forward declarations for test-scoped structures
struct Expr;
struct Table;
struct Column;
struct Parse;

// Simple error capture (non-terminating, to continue test execution)
static std::string g_lastError;

// Test hook macro (no-op, to mirror "testcase" calls in production)
#define testcase(x) ((void)0)

// Flags and constants (subset)
enum {
    COLFLAG_GENERATED = 0x01,
    COLFLAG_VIRTUAL   = 0x02,
    COLFLAG_STORED      = 0x04
};

static const int TK_SPAN = 1;
static const int EP_Skip = 1;
static const int EXPRDUP_REDUCE = 1;

// Expr structure (minimal)
struct Expr {
    int op;
    union {
        char *zToken; // token span
    } u;
    Expr *pLeft;
    int flags;
    bool isConstant; // helper for test: is constant
    bool isFunction; // helper for test: is function
    Expr() : op(0), pLeft(nullptr), flags(0), isConstant(false), isFunction(false) {
        u.zToken = nullptr;
    }
};

// Column structure (minimal)
struct Column {
    const char *zCnName; // column name
    int colFlags;
    Expr *pDfltExpr; // default expression (set by sqlite3ColumnSetExpr in production)
    Column() : zCnName(nullptr), colFlags(0), pDfltExpr(nullptr) {}
};

// Table structure (minimal)
struct Table {
    Column *aCol;
    int nCol;
};

// Init/DB structures (minimal)
struct InitInfo { bool busy; int iDb; InitInfo() : busy(false), iDb(0) {} };
struct sqlite3 { InitInfo init; };

// Parse context (minimal)
struct Parse {
    sqlite3 *db;
    Table *pNewTable;
};

// Helpers to simulate SQLite internal API used by sqlite3AddDefaultValue

// Allocate and copy a string token
char* sqlite3DbSpanDup(sqlite3* db, const char* zStart, const char* zEnd) {
    (void)db; // unused in this minimal mock
    size_t len = static_cast<size_t>(zEnd - zStart);
    char* s = new char[len + 1];
    if (len > 0) {
        std::memcpy(s, zStart, len);
    }
    s[len] = '\0';
    return s;
}

// Duplicate expression (deep-ish, shallow copy of essential fields)
Expr* sqlite3ExprDup(sqlite3* db, Expr* p, int flags) {
    (void)db; (void)flags;
    if (!p) return nullptr;
    Expr* q = new Expr(*p);
    if (p->u.zToken) {
        q->u.zToken = sqlite3DbSpanDup(db, p->u.zToken, p->u.zToken + std::strlen(p->u.zToken));
    }
    // Note: In production, a deep copy of subtrees would be needed; for tests this suffices.
    return q;
}

// Free token string (and simulate memory management)
void sqlite3DbFree(sqlite3* db, char* z) {
    (void)db;
    delete[] z;
}

// Error reporting replacement (non-terminating)
void sqlite3ErrorMsg(Parse* pParse, const char* zFormat, ...) {
    (void)pParse;
    char buffer[512];
    va_list ap;
    va_start(ap, zFormat);
    vsnprintf(buffer, sizeof(buffer), zFormat, ap);
    va_end(ap);
    g_lastError = std::string(buffer);
}

// Column set expression (store for test inspection)
void sqlite3ColumnSetExpr(Parse* pParse, Table* pTab, Column* pCol, Expr* pExpr) {
    (void)pParse; (void)pTab;
    if (pCol) {
        pCol->pDfltExpr = pExpr;
    }
}

// Rename unmap stub (no-op)
void sqlite3RenameExprUnmap(Parse* pParse, Expr* pExpr) {
    (void)pParse; (void)pExpr;
}

// Delete expression (no-op and safe)
void sqlite3ExprDelete(sqlite3* db, Expr* pExpr) {
    (void)db;
    // In real SQLite this would recursively free; here we assume test controls lifetime.
    // To avoid double free in tests, we simply leak intentionally or handle with flag if needed.
    // We'll implement a safe delete for allocated tokens and the node itself.
    if (!pExpr) return;
    if (pExpr->u.zToken) {
        delete[] pExpr->u.zToken;
    }
    delete pExpr;
}

// Client-facing: the focal method, re-implemented for test isolation
void sqlite3AddDefaultValue(
    Parse *pParse,           /* Parsing context */
    Expr *pExpr,             /* The parsed expression of the default value */
    const char *zStart,      /* Start of the default value text */
    const char *zEnd         /* First character past end of default value text */
){
    // Local declarations same shape as production
    Table *p;
    Column *pCol;
    sqlite3 *db = pParse->db;
    p = pParse->pNewTable;
    if( p!=nullptr ){
        int isInit = db->init.busy && db->init.iDb!=1;
        pCol = &(p->aCol[p->nCol-1]);
        // Use a simplified predicate mirroring production:
        if( pExpr == nullptr || ! (pExpr->isConstant || pExpr->isFunction) ){
            sqlite3ErrorMsg(pParse, "default value of column [%s] is not constant",
                pCol->zCnName);
#ifndef SQLITE_OMIT_GENERATED_COLUMNS
        }else if( pCol->colFlags & COLFLAG_GENERATED ){
            testcase( pCol->colFlags & COLFLAG_VIRTUAL );
            testcase( pCol->colFlags & COLFLAG_STORED );
            sqlite3ErrorMsg(pParse, "cannot use DEFAULT on a generated column");
#endif
        }else{
            // Copy of pExpr with span token
            Expr x, *pDfltExpr;
            std::memset(&x, 0, sizeof(x));
            x.op = TK_SPAN;
            x.u.zToken = sqlite3DbSpanDup(db, zStart, zEnd);
            x.pLeft = pExpr;
            x.flags = EP_Skip;
            pDfltExpr = sqlite3ExprDup(db, &x, EXPRDUP_REDUCE);
            sqlite3DbFree(db, x.u.zToken);
            sqlite3ColumnSetExpr(pParse, p, pCol, pDfltExpr);
        }
    }
    // IN_RENAME_OBJECT macro
#define IN_RENAME_OBJECT 0
#if IN_RENAME_OBJECT
    sqlite3RenameExprUnmap(pParse, pExpr);
#endif
    sqlite3ExprDelete(db, pExpr);
}

// Utility to reset global error for each test
void resetError() {
    g_lastError.clear();
}

// Test environment builders

Parse* buildParse(sqlite3* db, Table* tbl) {
    Parse* p = new Parse();
    p->db = db;
    p->pNewTable = tbl;
    return p;
}

Table* buildTable(int nCol) {
    Table* t = new Table();
    t->nCol = nCol;
    t->aCol = new Column[nCol];
    return t;
}

void deleteTable(Table* t) {
    if (!t) return;
    for (int i = 0; i < t->nCol; ++i) {
        // free any allocated default expr in tests
        if (t->aCol[i].pDfltExpr) {
            // If this were heap-allocated in tests, delete it
            // We rely on test harness lifecycle management
        }
    }
    delete[] t->aCol;
    delete t;
}

// Test 1: No table present; function should do nothing and not error
void test_no_table_case() {
    resetError();

    sqlite3 db;
    db.init.busy = false;
    db.init.iDb = 0;

    Parse parse;
    parse.db = &db;
    parse.pNewTable = nullptr; // no table

    // dummy expression
    Expr* pExpr = new Expr();
    pExpr->isConstant = true;

    // call
    sqlite3AddDefaultValue(&parse, pExpr, "default_text_start", "default_text_end");

    // expectations
    bool ok = g_lastError.empty();
    if (ok) {
        std::cout << "[OK] test_no_table_case: no error when pNewTable is nullptr\n";
    } else {
        std::cout << "[FAIL] test_no_table_case: unexpected error: " << g_lastError << "\n";
    }

    // cleanup
    delete pExpr;
}

// Test 2: Non-constant default value triggers error
void test_non_constant_default() {
    resetError();

    // Setup db and parse
    sqlite3 db;
    db.init.busy = false;
    db.init.iDb = 0;

    // Table with one column
    Table* tbl = buildTable(1);
    tbl->aCol[0].zCnName = "colA";
    tbl->aCol[0].colFlags = 0;

    Parse parse;
    parse.db = &db;
    parse.pNewTable = tbl;

    // pExpr is not constant or function
    Expr* pExpr = new Expr();
    pExpr->isConstant = false;
    pExpr->isFunction = false;

    sqlite3AddDefaultValue(&parse, pExpr, "start", "end");

    // Expect an error about non-constant default value
    std::string expected = "default value of column [colA] is not constant";
    if (g_lastError == expected) {
        std::cout << "[OK] test_non_constant_default: correctly reported non-constant default: " << g_lastError << "\n";
    } else {
        std::cout << "[FAIL] test_non_constant_default: expected error '" << expected
                  << "' but got '" << g_lastError << "'\n";
    }

    // cleanup
    delete pExpr;
    deleteTable(tbl);
}

// Test 3: Generated column should emit error about DEFAULT on generated column
void test_generated_column_error() {
    resetError();

    // Setup db and parse
    sqlite3 db;
    db.init.busy = false;
    db.init.iDb = 0;

    // Table with one generated column
    Table* tbl = buildTable(1);
    tbl->aCol[0].zCnName = "gen_col";
    tbl->aCol[0].colFlags = COLFLAG_GENERATED; // mark as generated

    Parse parse;
    parse.db = &db;
    parse.pNewTable = tbl;

    // pExpr that is a constant; the branch will still trigger generated
    Expr* pExpr = new Expr();
    pExpr->isConstant = true;

    sqlite3AddDefaultValue(&parse, pExpr, "start", "end");

    // Expect generated column error
    std::string expected = "cannot use DEFAULT on a generated column";
    if (g_lastError == expected) {
        std::cout << "[OK] test_generated_column_error: correctly reported generated column usage: " << g_lastError << "\n";
    } else {
        std::cout << "[FAIL] test_generated_column_error: expected '" << expected
                  << "' but got '" << g_lastError << "'\n";
    }

    // cleanup
    delete pExpr;
    deleteTable(tbl);
}

// Test 4: Valid constant default should attach a derived expression to the column
void test_valid_default_sets_expr() {
    resetError();

    // Setup db and parse
    sqlite3 db;
    db.init.busy = false;
    db.init.iDb = 0;

    // Table with one normal column
    Table* tbl = buildTable(1);
    tbl->aCol[0].zCnName = "colB";
    tbl->aCol[0].colFlags = 0; // not generated

    Parse parse;
    parse.db = &db;
    parse.pNewTable = tbl;

    // pExpr that is a constant
    Expr* pExpr = new Expr();
    pExpr->isConstant = true;
    pExpr->isFunction = false;

    sqlite3AddDefaultValue(&parse, pExpr, "token_start_", "token_end_");

    // After successful path, the column should have a non-null pDfltExpr
    bool hasExpr = (tbl->aCol[0].pDfltExpr != nullptr);
    bool leftMatchesOriginal = (hasExpr && tbl->aCol[0].pDfltExpr->pLeft == pExpr);

    if (hasExpr && leftMatchesOriginal) {
        std::cout << "[OK] test_valid_default_sets_expr: default expression attached to column as expected\n";
    } else {
        std::cout << "[FAIL] test_valid_default_sets_expr: expected default expression to be attached. hasExpr=" 
                  << hasExpr << ", leftMatches=" << leftMatchesOriginal << "\n";
    }

    // Cleanup
    // Note: We avoid deleting pExpr here as sqlite3AddDefaultValue retains a pointer to it in pDfltExpr->pLeft
    // The test harness will clean up pExpr at end of program.
    // We manually delete the created pDfltExpr to prevent leaks
    if (tbl->aCol[0].pDfltExpr) {
        if (tbl->aCol[0].pDfltExpr->u.zToken) {
            delete[] tbl->aCol[0].pDfltExpr->u.zToken;
        }
        delete tbl->aCol[0].pDfltExpr;
        tbl->aCol[0].pDfltExpr = nullptr;
    }
    deleteTable(tbl);
}

int main() {
    // Run each test with simple textual output
    std::cout << "Starting sqlite3AddDefaultValue unit tests (self-contained):

" << std::endl;

    test_no_table_case();
    test_non_constant_default();
    test_generated_column_error();
    test_valid_default_sets_expr();

    // Summary
    if (g_lastError.empty()) {
        // If no error occurred in any test, print summary (not precise aggregation here)
        std::cout << "\nAll tests executed. Summary: see individual test outputs above." << std::endl;
    } else {
        std::cout << "\nSome tests produced errors. Last error captured: " << g_lastError << std::endl;
    }

    return 0;
}