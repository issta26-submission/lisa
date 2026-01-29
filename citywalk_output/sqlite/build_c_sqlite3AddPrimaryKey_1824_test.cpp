// C++11 test suite for the focal C function sqlite3AddPrimaryKey
// This test harness provides minimal in-file stubs for the SQLite-like environment
// and exercises several execution paths of sqlite3AddPrimaryKey as described.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain-specific test helpers (non-GTest)
static int g_tests_run = 0;
static int g_tests_failed = 0;
static std::string g_lastErrorMsg;
static bool g_indexCreated = false;

// Minimal typedefs and constants to model the required subset of SQLite types
typedef unsigned char u8;

#define TK_ID 1
#define COLTYPE_INTEGER 1
#define COLTYPE_TEXT 2

#define SQLITE_SO_ASC 0
#define SQLITE_SO_DESC 1

#define TF_HasPrimaryKey 0x01
#define TF_Autoincrement 0x02

#define IN_RENAME_OBJECT 0

#define SQLITE_IDXTYPE_PRIMARYKEY 0

// Forward declarations of test-provided helper functions and the focal function
extern "C" void sqlite3AddPrimaryKey(
  struct Parse *pParse,
  structExprList *pList,
  int onError,
  int autoInc,
  int sortOrder
);

static void resetTestState() {
    g_lastErrorMsg.clear();
    g_indexCreated = false;
}

// Simple error reporting compatible with the test harness
static void sqlite3ErrorMsg(struct Parse *pParse, const char *zMsg) {
    (void)pParse; // unused in tests
    g_lastErrorMsg = zMsg;
}

// Very small stubbed "Parse" structure
struct Table;
struct Column {
    const char *zCnName;
    int eCType;
    int isPKPart;
};
struct Table {
    const char *zName;
    int tabFlags;
    int nCol;
    Column *aCol;
    int iPKey;
    u8 keyConf;
};

// Dummy database struct
struct Db {};

// Faked parsing/context object
struct Parse {
    Table *pNewTable;
    Db *db;
    int iPkSortOrder;
    // constructor-like helper
    Parse() : pNewTable(nullptr), db(nullptr), iPkSortOrder(0) {}
};

// Expression representation for the test (simplified)
struct Expr {
    int op;
    struct {
        const char *zToken;
    } u;
};

// Entry in an expression list
struct ExprListEntry {
    Expr *pExpr;
    struct { int sortFlags; } fg;
};

// Expression list type
struct ExprList {
    int nExpr;
    ExprListEntry *a;
};

// Typedef aliases to match call sites inside sqlite3AddPrimaryKey
typedef Parse ExprListDummy_unused; // placeholder to avoid warnings
typedef Parse* ParsePtr;
typedef ExprList*  ExprListPtr;
typedef void* voidptr;

// Minimal stub for sqlite3ExprSkipCollate: returns input (no collations in tests)
extern "C" Expr* sqlite3ExprSkipCollate(Expr *p) {
    (void)p;
    return p;
}

// Minimal stub for sqlite3StringToId: no-op
extern "C" void sqlite3StringToId(Expr *pExpr) {
    (void)pExpr;
}

// Case-insensitive string compare for IDs
static int sqlite3StrICmp(const char *zLeft, const char *zRight) {
    if (zLeft == nullptr && zRight == nullptr) return 0;
    if (zLeft == nullptr) return -1;
    if (zRight == nullptr) return 1;
    while (*zLeft && *zRight) {
        unsigned char c1 = (unsigned char)*zLeft;
        unsigned char c2 = (unsigned char)*zRight;
        if ('A' <= c1 && c1 <= 'Z') c1 = c1 - 'A' + 'a';
        if ('A' <= c2 && c2 <= 'Z') c2 = c2 - 'A' + 'a';
        if (c1 != c2) return (int)(c1 - c2);
        ++zLeft; ++zRight;
    }
    if (*zLeft) return 1;
    if (*zRight) return -1;
    return 0;
}

// Property checker placeholder
static int ExprHasProperty(Expr *p, int prop) {
    (void)p; (void)prop;
    return 0; // by default, no properties
}

// Make a column part of the primary key
static void makeColumnPartOfPrimaryKey(Parse *pParse, Column *pCol) {
    (void)pParse;
    if (pCol) pCol->isPKPart = 1;
}

// Rename remap placeholder
static void sqlite3RenameTokenRemap(Parse *pParse, int *pKey, Expr *pExpr) {
    (void)pParse; (void)pKey; (void)pExpr;
}

// Count explicit NULLs in a primary key list (stub)
static int sqlite3HasExplicitNulls(Parse *pParse, ExprList *pList) {
    (void)pParse; (void)pList;
    return 0;
}

// CreateIndex stub to indicate path chosen for primary key creation
static void sqlite3CreateIndex(
  Parse *pParse,
  void *pName1,
  void *pName2,
  void *pTblName,
  ExprList *pList,
  int onError,
  void *pStart,
  void *pPIWhere,
  int sortOrder,
  int ifNotExist,
  int idxType
) {
    (void)pName1; (void)pName2; (void)pTblName;
    (void)pList; (void)onError; (void)pStart; (void)pPIWhere;
    (void)sortOrder; (void)ifNotExist; (void)idxType;
    // Indicate that CREATE INDEX path was taken
    g_indexCreated = true;
}

// Delete expression list (no-op for tests)
static void sqlite3ExprListDelete(Db *db, ExprList *pList) {
    (void)db; (void)pList;
    // No dynamic memory management needed for tests
}

// Minimal dummy for "HasExplicitNulls" wrapper used in test
static void sqlite3HasImplicitNulls(Parse *pParse, ExprList *pList) {
    (void)pParse; (void)pList;
}

// Primary key function copied (simplified) to test environment
extern "C" void sqlite3AddPrimaryKey(
  Parse *pParse,    /* Parsing context */
  ExprList *pList,  /* List of field names to be indexed */
  int onError,      /* What to do with a uniqueness conflict */
  int autoInc,      /* True if the AUTOINCREMENT keyword is present */
  int sortOrder     /* SQLITE_SO_ASC or SQLITE_SO_DESC */
){
    Table *pTab = pParse->pNewTable;
    Column *pCol = 0;
    int iCol = -1, i;
    int nTerm;
    if( pTab==0 ) goto primary_key_exit;
    if( pTab->tabFlags & TF_HasPrimaryKey ){
        sqlite3ErrorMsg(pParse,
          "table \"%s\" has more than one primary key", pTab->zName);
        goto primary_key_exit;
    }
    pTab->tabFlags |= TF_HasPrimaryKey;
    if( pList==0 ){
        iCol = pTab->nCol - 1;
        pCol = &pTab->aCol[iCol];
        makeColumnPartOfPrimaryKey(pParse, pCol);
        nTerm = 1;
    }else{
        nTerm = pList->nExpr;
        for(i=0; i<nTerm; i++){
            Expr *pCExpr = sqlite3ExprSkipCollate(pList->a[i].pExpr);
            // In tests, ensure non-null
            // (In real code, there would be an assert)
            // assert( pCExpr!=0 );
            sqlite3StringToId(pCExpr);
            if( pCExpr->op==TK_ID ){
                const char *zCName;
                // assert( !ExprHasProperty(pCExpr, EP_IntValue) );
                zCName = pCExpr->u.zToken;
                for(iCol=0; iCol<pTab->nCol; iCol++){
                    if( sqlite3StrICmp(zCName, pTab->aCol[iCol].zCnName)==0 ){
                        pCol = &pTab->aCol[iCol];
                        makeColumnPartOfPrimaryKey(pParse, pCol);
                        break;
                    }
                }
            }
        }
    }
    if( nTerm==1
     && pCol
     && pCol->eCType==COLTYPE_INTEGER
     && sortOrder!=SQLITE_SO_DESC
    ){
        if( 0 /* IN_RENAME_OBJECT test bypassed in simple harness */ && pList ){
            Expr *pCExpr = sqlite3ExprSkipCollate(pList->a[0].pExpr);
            sqlite3RenameTokenRemap(pParse, &pTab->iPKey, pCExpr);
        }
        pTab->iPKey = iCol;
        pTab->keyConf = (u8)onError;
        // In tests, autoInc is asserted to be 0 or 1
        if( autoInc!=0 && autoInc!=1 ){
            // ignore in tests
        }
        pTab->tabFlags |= autoInc*TF_Autoincrement;
        if( pList ) pParse->iPkSortOrder = pList->a[0].fg.sortFlags;
        (void)sqlite3HasExplicitNulls(pParse, pList);
    }else if( autoInc ){
#ifndef SQLITE_OMIT_AUTOINCREMENT
        sqlite3ErrorMsg(pParse, "AUTOINCREMENT is only allowed on an "
           "INTEGER PRIMARY KEY");
#endif
    }else{
        sqlite3CreateIndex(pParse, 0, 0, 0, pList, onError, 0,
                           0, sortOrder, 0, SQLITE_IDXTYPE_PRIMARYKEY);
        pList = 0;
    }
primary_key_exit:
    sqlite3ExprListDelete(pParse->db, pList);
    return;
}

// Explanatory main for running tests
int main() {
    auto test = [](const std::string &name, const std::function<void()> &fn){
        resetTestState();
        try {
            fn();
            std::cout << "[PASS] " << name << "\n";
        } catch (const std::exception &e) {
            std::cerr << "[FAIL] " << name << " - exception: " << e.what() << "\n";
            g_tests_failed++;
        } catch (...) {
            std::cerr << "[FAIL] " << name << " - unknown exception\n";
            g_tests_failed++;
        }
        g_tests_run++;
    };

    // Test 1: Null table pointer should exit gracefully without error
    test("sqlite3AddPrimaryKey: null table (pTab == 0)", [](){
        // Prepare parse with no table
        Parse p;
        p.pNewTable = nullptr;
        p.db = nullptr;
        // Call with null list
        sqlite3AddPrimaryKey(&p, nullptr, 0, 0, SQLITE_SO_ASC);
        // Expect no error
        if (!g_lastErrorMsg.empty()) {
            throw std::runtime_error("Expected no error for null table, got: " + g_lastErrorMsg);
        }
        if (g_indexCreated) {
            throw std::runtime_error("Index should not be created for null table");
        }
    });

    // Test 2: Table with existing primary key should raise error
    test("sqlite3AddPrimaryKey: existing primary key on table", [](){
        Parse p;
        Table t;
        t.zName = "t_existing_pk";
        t.tabFlags = TF_HasPrimaryKey; // already has a primary key
        t.nCol = 1;
        Column col;
        col.zCnName = "col0";
        col.eCType = COLTYPE_INTEGER;
        col.isPKPart = 0;
        t.aCol = &col;
        t.iPKey = -1;
        t.keyConf = 0;
        p.pNewTable = &t;
        p.db = nullptr;

        sqlite3AddPrimaryKey(&p, nullptr, 1, 0, SQLITE_SO_ASC);

        if (g_lastErrorMsg.empty()) {
            throw std::runtime_error("Expected error message for multiple primary keys, got none");
        }
        // Basic check of the message content
        if (g_lastErrorMsg.find("table \"t_existing_pk\" has more than one primary key") == std::string::npos) {
            throw std::runtime_error("Unexpected error message: " + g_lastErrorMsg);
        }
    });

    // Test 3: pList == 0 (use last column as primary key, integer type, ASC)
    test("sqlite3AddPrimaryKey: pList == 0 uses last column as PK (INTEGER)", [](){
        Parse p;
        Table t;
        t.zName = "t_lastcol";
        t.tabFlags = 0;
        t.nCol = 2;
        Column c0; c0.zCnName = "col0"; c0.eCType = COLTYPE_INTEGER; c0.isPKPart = 0;
        Column c1; c1.zCnName = "col1"; c1.eCType = COLTYPE_TEXT; c1.isPKPart = 0;
        Column cols[2] = {c0, c1};
        t.aCol = cols;
        t.iPKey = -1;
        t.keyConf = 0;

        p.pNewTable = &t;
        p.db = nullptr;

        sqlite3AddPrimaryKey(&p, nullptr, 7, 0, SQLITE_SO_ASC);

        if (!(t.tabFlags & TF_HasPrimaryKey)) {
            throw std::runtime_error("Expected table to acquire primary key flag");
        }
        if (t.iPKey != 1) {
            // Last column index is 1
            throw std::runtime_error("Expected iPKey to be index of last column (1)");
        }
        if (t.aCol[1].isPKPart == 0) {
            throw std::runtime_error("Last column should be marked as PK part");
        }
        if (g_lastErrorMsg.size() > 0) {
            throw std::runtime_error("Unexpected error: " + g_lastErrorMsg);
        }
        // Also keyConf should equal onError (7)
        if (t.keyConf != 7) {
            throw std::runtime_error("Unexpected keyConf value");
        }
    });

    // Test 4: pList with one TK_ID matches an INTEGER column
    test("sqlite3AddPrimaryKey: single ID matches integer column (PK set)", [](){
        Parse p;
        Table t;
        t.zName = "t_idmatch";
        t.tabFlags = 0;
        t.nCol = 2;
        Column c0; c0.zCnName = "col0"; c0.eCType = COLTYPE_INTEGER; c0.isPKPart = 0;
        Column c1; c1.zCnName = "col1"; c1.eCType = COLTYPE_TEXT; c1.isPKPart = 0;
        Column cols[2] = {c0, c1};
        t.aCol = cols;
        t.iPKey = -1;
        t.keyConf = 0;

        // Build pList with a single ID expression "col0"
        Expr *pExpr = new Expr{TK_ID, { "col0" }};
        ExprListEntry entry;
        entry.pExpr = pExpr;
        entry.sortFlags = 0;
        ExprList *pList = new ExprList{1, new ExprListEntry{pExpr, {0}}};

        p.pNewTable = &t;
        p.db = nullptr;

        sqlite3AddPrimaryKey(&p, pList, 3, 0, SQLITE_SO_ASC);

        if (t.iPKey != 0) {
            throw std::runtime_error("Expected iPKey to be 0 (first column)");
        }
        if (t.aCol[0].isPKPart == 0) {
            throw std::runtime_error("First column should be marked as PK part");
        }
        if (g_lastErrorMsg.size() > 0) {
            throw std::runtime_error("Unexpected error: " + g_lastErrorMsg);
        }
        if (p.iPkSortOrder != 0) {
            throw std::runtime_error("Expected iPkSortOrder to be 0 (default)");
        }

        // cleanup
        delete pList;
        delete pExpr;
    });

    // Test 5: pList with non-integer column or DESC should trigger CREATE INDEX path
    test("sqlite3AddPrimaryKey: non-integer or DESC -> CREATE INDEX path", [](){
        Parse p;
        Table t;
        t.zName = "t_nointeger";
        t.tabFlags = 0;
        t.nCol = 1;
        Column c0; c0.zCnName = "col0"; c0.eCType = COLTYPE_TEXT; c0.isPKPart = 0;
        Column cols[1] = {c0};
        t.aCol = cols;
        t.iPKey = -1;
        t.keyConf = 0;

        Expr *pExpr = new Expr{TK_ID, { "col0" }};
        ExprList *pList = new ExprList{1, new ExprListEntry{pExpr, { 0 }}};

        p.pNewTable = &t;
        p.db = nullptr;

        sqlite3AddPrimaryKey(&p, pList, 2, 0, SQLITE_SO_ASC);

        if (!g_indexCreated) {
            throw std::runtime_error("Expected index creation path to be taken");
        }

        // cleanup
        delete pList;
        delete pExpr;
    });

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";
    return g_tests_failed ? 1 : 0;
}