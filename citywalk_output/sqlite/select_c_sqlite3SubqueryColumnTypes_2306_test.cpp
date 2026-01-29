// Minimal C++11 test harness for sqlite3SubqueryColumnTypes (reproduced in tests)
// This file provides a self-contained test environment with lightweight stubs
// for SQLite internals to exercise the focal method without requiring a full SQLite build.
// It uses non-terminating, custom CHECK macros to maximize coverage.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Lightweight mimicry of SQLite types and constants

typedef long long i64;

// Affinity constants (subset for tests)
enum {
  SQLITE_AFF_NONE = 0,
  SQLITE_AFF_TEXT = 1,
  SQLITE_AFF_NUMERIC = 2,
  SQLITE_AFF_BLOB = 3,
  SQLITE_AFF_FLEXNUM = 4
};

#define TK_CAST 999

// Flags used by the test stubs
#define IN_RENAME_OBJECT 0
#define COLFLAG_NOINSERT 0x01
#define COLFLAG_HASTYPE 0x02
#define COLFLAG_HASCOLL 0x04

// Forward declarations for the focal method's dependencies (lightweight stubs)
struct sqlite3;
struct Parse;
struct Table;
struct Column;
struct Select;
struct Expr;
struct ExprList;
struct SrcList;
struct NameContext;
struct CollSeq;

typedef int ExprOp;

// Simple sqlite3 structure (stubbed)
struct sqlite3 {
  bool mallocFailed;
};

// Stubbed Parse, Table, Select, Expr, ExprList, SrcList, NameContext
struct Parse {
  sqlite3 *db;
  int nErr;
};

struct Column {
  int colFlags;
  char *zCnName;
  int affinity;
  // Intentionally small surface: only fields used by the focal method
  int szEst;
  // Other fields are omitted for brevity
  int tabFlags; // used as pTab->tabFlags in focal code
};

struct Table {
  int nCol;
  Column *aCol;
  int tabFlags;
  struct Index *pIndex;
  int szTabRow;
};

struct Expr {
  int op;
  int affinityOverride; // if non-zero, overrides expression affinity
  int dataType;          // used by sqlite3ExprDataType in tests
};

struct ExprList_item {
  Expr *pExpr;
};

struct ExprList {
  int nExpr;
  ExprList_item *a;
};

struct SrcList {
  // empty for tests
};

struct NameContext {
  SrcList *pSrcList;
};

// Collation sequence (unused in tests)
struct CollSeq {
  const char *zName;
};

// Helper: create a new tiny expression
Expr* newExpr(int op, int affinityOverride, int dataType) {
  Expr* e = new Expr();
  e->op = op;
  e->affinityOverride = affinityOverride;
  e->dataType = dataType;
  return e;
}

// Helper: create a new ExprList with given expressions
ExprList* newExprList(const std::vector<Expr*>& exprs) {
  ExprList* pEList = new ExprList();
  pEList->nExpr = (int)exprs.size();
  pEList->a = new ExprList_item[exprs.size()];
  for (size_t i = 0; i < exprs.size(); ++i) {
    pEList->a[i].pExpr = exprs[i];
  }
  return pEList;
}

// Forward declarations of the focal function and several support functions
// The test harness provides lightweight implementations of these to compile the focal function.
extern "C" {

// Minimal helper functions used inside sqlite3SubqueryColumnTypes

// Affinity for an expression
int sqlite3ExprAffinity(Expr *p) {
  if (p == nullptr) return SQLITE_AFF_NONE;
  if (p->affinityOverride != 0) return p->affinityOverride;
  // If the expression has an explicit dataType of textual, numeric, etc., we could map it.
  // For tests, rely on explicit affinityOverride only.
  return SQLITE_AFF_NONE;
}

// Data type inferred from an expression
int sqlite3ExprDataType(Expr *p) {
  if (!p) return 0;
  return p->dataType;
}

// Column type inference helper (stubbed, returns 0 to exercise default path)
const char* columnType(NameContext *pNC, Expr *p, int a, int b, int c) {
  (void)pNC; (void)p; (void)a; (void)b; (void)c;
  return nullptr;
}

// Convert a textual type name to an affinity
int sqlite3AffinityType(const char *zType, int dummy) {
  if (zType == nullptr) return 0;
  if (strcmp(zType, "NUM") == 0) return SQLITE_AFF_NUMERIC;
  if (strcmp(zType, "TEXT") == 0) return SQLITE_AFF_TEXT;
  return 0;
}

// Standard type affinities used to map back to type names
static const int SQLITE_N_STDTYPE = 5;
static const int sqlite3StdTypeAffinity[SQLITE_N_STDTYPE] = {
  0, // index 0 unused
  SQLITE_AFF_TEXT,     // 1
  SQLITE_AFF_NUMERIC,  // 2
  SQLITE_AFF_BLOB,     // 3
  SQLITE_AFF_TEXT      // 4
};
static const char *sqlite3StdType[SQLITE_N_STDTYPE] = {
  "",
  "TEXT",
  "NUM",
  "BLOB",
  "TEXT"
};

// Collation sequence fetcher (stubbed: returns NULL)
CollSeq* sqlite3ExprCollSeq(Parse*, Expr*) {
  return nullptr;
}

// Apply a column's collation (no-op in tests)
void sqlite3ColumnSetColl(sqlite3 *db, Column *pCol, const char *zColl){
  (void)db; (void)pCol; (void)zColl;
}

// String length helper (30-char max; simple strlen)
i64 sqlite3Strlen30(const char *z){
  if(!z) return 0;
  return (i64)strlen(z);
}

// Memory reallocation helper similar to sqlite3DbReallocOrFree
char* sqlite3DbReallocOrFree(sqlite3 *db, char *pOld, size_t newSize){
  if (newSize == 0) {
    free(pOld);
    return nullptr;
  }
  char *pNew = (char*)malloc(newSize);
  if (pNew == nullptr) return nullptr;
  if (pOld) {
    size_t oldLen = strlen(pOld);
    size_t toCopy = oldLen < newSize-1 ? oldLen : newSize-1;
    memcpy(pNew, pOld, toCopy);
    pNew[toCopy] = '\0';
    free(pOld);
  } else {
    pNew[0] = '\0';
  }
  return pNew;
}

// Expression data type accessor for Next subquery loop (stubbed to return a representative type)
int sqlite3ExprDataTypeForNext(Expr *p){
  (void)p;
  return 0;
}
}

// The focal method to test (copied and adapted for test environment)
void sqlite3SubqueryColumnTypes(
  Parse *pParse,      /* Parsing contexts */
  Table *pTab,        /* Add column type information to this table */
  Select *pSelect,    /* SELECT used to determine types and collations */
  char aff            /* Default affinity. */
){
  sqlite3 *db = pParse->db;
  struct Column *pCol;
  CollSeq *pColl;
  int i,j;
  Expr *p;
  struct ExprList_item *a;
  NameContext sNC;
  // Basic safety checks (simulated asserts; assume they pass in tests)
  // NOTE: In our small harness we avoid actual runtime asserts to keep tests non-terminating.
  // The real code uses assert(); we rely on properly constructed test data.
  // if( pSelect!=0 );
  // if( (pSelect->selFlags & SF_Resolved)!=0 );
  // if( pTab->nCol==pSelect->pEList->nExpr || pParse->nErr>0 );
  // if( aff==SQLITE_AFF_NONE || aff==SQLITE_AFF_BLOB );
  if( db->mallocFailed ) return;
  // Simplified: ignore IN_RENAME_OBJECT
  while( pSelect->pPrior ) pSelect = pSelect->pPrior;
  a = pSelect->pEList->a;
  memset(&sNC, 0, sizeof(sNC));
  sNC.pSrcList = pSelect->pSrc;
  for(i=0, pCol=pTab->aCol; i<pTab->nCol; i++, pCol++){
    const char *zType;
    i64 n;
    pTab->tabFlags |= (pCol->colFlags & COLFLAG_NOINSERT);
    p = a[i].pExpr;
    /* pCol->szEst = ... // Column size est for SELECT tables never used */
    pCol->affinity = 0; // simplified; use default path
    pCol->affinity = sqlite3ExprAffinity(p);
    if( pCol->affinity<=SQLITE_AFF_NONE ){
      pCol->affinity = aff;
    }
    if( pCol->affinity>=SQLITE_AFF_TEXT && pSelect->pNext ){
      int m = 0;
      Select *pS2;
      for(m=0, pS2=pSelect->pNext; pS2; pS2=pS2->pNext){
        m |= sqlite3ExprDataType(pS2->pEList->a[i].pExpr);
      }
      if( pCol->affinity==SQLITE_AFF_TEXT && (m&0x01)!=0 ){
        pCol->affinity = SQLITE_AFF_BLOB;
      }else
      if( pCol->affinity>=SQLITE_AFF_NUMERIC && (m&0x02)!=0 ){
        pCol->affinity = SQLITE_AFF_BLOB;
      }
      if( pCol->affinity>=SQLITE_AFF_NUMERIC && p->op==TK_CAST ){
        pCol->affinity = SQLITE_AFF_FLEXNUM;
      }
    }
    zType = columnType(&sNC, p, 0, 0, 0);
    if( zType==0 || pCol->affinity!=sqlite3AffinityType(zType, 0) ){
      if( pCol->affinity==SQLITE_AFF_NUMERIC
       || pCol->affinity==SQLITE_AFF_FLEXNUM
      ){
        zType = "NUM";
      }else{
        zType = 0;
        for(j=1; j<SQLITE_N_STDTYPE; j++){
          if( sqlite3StdTypeAffinity[j]==pCol->affinity ){
            zType = sqlite3StdType[j];
            break;
          }
        }
      }
    }
    if( zType ){
      i64 m = sqlite3Strlen30(zType);
      n = sqlite3Strlen30(pCol->zCnName);
      pCol->zCnName = sqlite3DbReallocOrFree(db, pCol->zCnName, n+m+2);
      pCol->colFlags &= ~(COLFLAG_HASTYPE|COLFLAG_HASCOLL);
      if( pCol->zCnName ){
        memcpy(&pCol->zCnName[n+1], zType, m+1);
        pCol->colFlags |= COLFLAG_HASTYPE;
      }
    }
    pColl = sqlite3ExprCollSeq(pParse, p);
    if( pColl ){
      // In tests, this is usually NULL
      // sqlite3ColumnSetColl(db, pCol, pColl->zName);
    }
  }
  pTab->szTabRow = 1; /* Any non-zero value works */
}

// Lightweight test scaffolding and test runners

// Global test status
static int g_failures = 0;

// Simple check macro that doesn't abort on failure
#define CHECK(cond, msg) do { if(!(cond)) { std::cerr << "[FAIL] " << (msg) << std::endl; ++g_failures; } } while(0)

// Build a minimal environment for a single-column table and a simple select
struct TestEnv {
  Parse pParse;
  Table pTab;
  Select pSelect;
  // Convenience allocated objects
  Column col;
  Expr exprMain;
  ExprList eList;
  SrcList src;
  // Next/Prior chain
  Select nextSubq;
  Select* pNextSubq;

  TestEnv() {
    // Initialize db
    static sqlite3 db_instance;
    db_instance.mallocFailed = false;
    pParse.db = &db_instance;
    pParse.nErr = 0;

    // Table and column
    pTab.nCol = 1;
    pTab.aCol = &col;
    pTab.tabFlags = 0;
    pTab.pIndex = nullptr;
    pTab.szTabRow = 0;
    col.colFlags = 0;
    col.zCnName = (char*)"col0";
    col.affinity = SQLITE_AFF_NONE;
    col.szEst = 0;
    // Expr for main select
    exprMain = *newExpr(0, 0, 0);
    // pEList for main select
    eList.nExpr = 1;
    eList.a = new ExprList_item[1];
    eList.a[0].pExpr = &exprMain;
    // pSrc
    // (empty for tests)
    // pSelect
    pSelect.selFlags = 1; // SF_Resolved (we simulate by 1)
    pSelect.pNext = nullptr;
    pSelect.pPrior = nullptr;
    pSelect.pEList = &eList;
    pSelect.pSrc = &src;
    // Next subquery (for advanced tests)
    nextSubq.selFlags = 1;
    nextSubq.pNext = nullptr;
    nextSubq.pPrior = nullptr;
    nextSubq.pEList = &eList; // reuse expression list for ease
    nextSubq.pSrc = &src;
    pNextSubq = nullptr;
  }

  // Helper to set main select's first expression to a given affinity/dataType
  void setMainExpr(int op, int affinityOverride, int dataType) {
    exprMain.op = op;
    exprMain.affinityOverride = affinityOverride;
    exprMain.dataType = dataType;
  }

  // Cleanup (not strictly required for short-lived tests)
  ~TestEnv() {
    delete[] eList.a;
  }
};

// Test 1: Basic path where no special affinities or next-subquery branches are taken.
// - nCol = 1
// - pEList[0].pExpr affinity = NONE
// - aff = SQLITE_AFF_NONE
// Expect: no changes to affinity, szTabRow becomes 1.
void test_basic_path_case() {
  TestEnv env;
  // No special next subquery
  env.pSelect.selFlags = 1; // SF_Resolved
  // Initial column state
  env.col.zCnName = (char*)"col0";
  env.col.affinity = SQLITE_AFF_NONE;
  env.col.colFlags = 0;

  sqlite3SubqueryColumnTypes(env.pParse, &env.pTab, &env.pSelect, SQLITE_AFF_NONE);

  // Assertions
  CHECK(env.pTab.szTabRow == 1, "szTabRow should be set to 1");
  CHECK(env.col.affinity == SQLITE_AFF_NONE, "Column affinity should remain NONE");
  CHECK(env.pTab.nCol == 1, "Tab should have exactly 1 column");
}

// Test 2: Text affinity on first column with a non-empty next-subquery having a first expr dataType that denotes TEXT.
// - pCol[0].affinity = SQLITE_AFF_TEXT
// - pSelect->pNext exists with pEList->a[0].pExpr.dataType has a bit 0x01 set (TEXT-like)
// Expect: pCol[0].affinity becomes SQLITE_AFF_BLOB
void test_text_affinity_with_next_branch() {
  TestEnv env;
  // Prepare environment: one column, main expr affinity TEXT
  env.col.zCnName = (char*)"col0";
  env.col.affinity = SQLITE_AFF_TEXT;
  env.pSelect.selFlags = 1;
  // Create next-subquery with one expression whose dataType has bit 0x01 set
  Expr* nextExpr = newExpr(0, 0, 1); // data type bit 0x01
  ExprList* nextEList = newExprList({ nextExpr });
  Select* nextSel = &env.nextSubq;
  nextSel->selFlags = 1;
  nextSel->pEList = nextEList;
  nextSel->pSrc = nullptr;
  nextSel->pNext = nullptr;
  // Link into main select
  env.pSelect.pNext = nextSel;
  // The main select's own expression (pExpr) is used for the "affinity" logic
  env.exprMain = *newExpr(0, 0, 0);

  sqlite3SubqueryColumnTypes(env.pParse, &env.pTab, &env.pSelect, SQLITE_AFF_TEXT);

  // Expect: affinity upgraded to BLOB due to (m&0x01)!=0
  CHECK(env.col.affinity == SQLITE_AFF_BLOB, "Text affinity with next subquery should convert to BLOB");
  // Cleanup
  delete nextExpr;
  delete nextEList;
}

// Test 3: Numeric affinity with a CAST operation in the main expression and a non-empty next-subquery
// - pCol[0].affinity = SQLITE_AFF_NUMERIC
// - pExpr (main) op = TK_CAST
// - next-subquery's first expr dataType has bit 0x02 set (numeric)
 // Expect: affinity ends up as SQLITE_AFF_FLEXNUM
void test_numeric_affinity_cast_branch() {
  TestEnv env;
  // Setup: numeric affinity on first column
  env.col.zCnName = (char*)"col0";
  env.col.affinity = SQLITE_AFF_NUMERIC;
  // Main expression is a CAST operation
  env.setMainExpr(TK_CAST, 0, 0);

  // Create next-subquery with dataType having bit 0x02 set
  Expr* nextExpr = newExpr(0, 0, 2); // data type 2 enables 0x02
  ExprList* nextEList = newExprList({ nextExpr });
  Select* nextSel = &env.nextSubq;
  nextSel->selFlags = 1;
  nextSel->pEList = nextEList;
  nextSel->pSrc = nullptr;
  nextSel->pNext = nullptr;
  env.pSelect.pNext = nextSel;

  sqlite3SubqueryColumnTypes(env.pParse, &env.pTab, &env.pSelect, SQLITE_AFF_NUMERIC);

  // Expect: final affinity FLEXNUM due to CAST after numeric and 0x02 bit
  CHECK(env.col.affinity == SQLITE_AFF_FLEXNUM, "Numeric affinity with CAST should result in FLEXNUM");

  // Cleanup
  delete nextExpr;
  delete nextEList;
}

// Runner
int main() {
  std::cout << "Running sqlite3SubqueryColumnTypes unit tests (mocked environment)" << std::endl;

  test_basic_path_case();
  test_text_affinity_with_next_branch();
  test_numeric_affinity_cast_branch();

  if (g_failures == 0) {
    std::cout << "[OK] All tests passed." << std::endl;
    return 0;
  } else {
    std::cout << "[WARN] " << g_failures << " test(s) failed." << std::endl;
    return 1;
  }
}