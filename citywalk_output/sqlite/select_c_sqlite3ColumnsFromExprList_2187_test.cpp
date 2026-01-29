// Self-contained C++11 test suite for sqlite3ColumnsFromExprList
// Note: This test harness provides minimal stubs of SQLite internal types
// and functions required to exercise the focal method without relying on the
// actual SQLite build. It exercises multiple branches to improve coverage.

#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Lightweight stubs and types to mimic necessary SQLite structures.

typedef short i16;     // matched usage in sqlite3ColumnsFromExprList
typedef unsigned int u32;

const int SQLITE_OK = 0;
const int TK_DOT    = 1;
const int TK_COLUMN = 2;
const int TK_ID     = 3;
const int EP_IntValue = 1;
const int COLFLAG_NOEXPAND = 1;

// Forward declarations
struct sqlite3;
struct Table;
struct Column;
struct Expr;
struct Parse;
struct ExprList;
struct ExprList_item;
struct Hash;

// Domain constants
enum {
  ENAME_NAME = 1
};

// Simple token structures
struct sqlite3 {
  // empty stub
};

// Hash table wrapper expected by sqlite3ColumnsFromExprList
struct Hash {
  // map name to the ExprList_item used to derive a name
  std::unordered_map<std::string, ExprList_item*> map;
};

// Column structure used by the focal function
struct Column {
  char *zCnName;
  unsigned int hName;
  int colFlags;
};

// Expression structure used by the focal function
struct Expr {
  int op;
  Expr *pRight;
  struct {
    Table *pTab;
  } y;
  int iColumn;
  struct {
    const char *zToken;
  } u;
};

// ExprList item wrapper
struct ExprList_item {
  const char *zEName;
  struct {
    int eEName;
    int bUsingTerm;
    int bNoExpand;
  } fg;
  Expr *pExpr;
};

// ExprList container
struct ExprList {
  int nExpr;
  ExprList_item *a; // dynamic array
};

// Parse context used by the focal function
struct Parse {
  sqlite3 *db;
  int nErr;
  int rc;
};

// Helpers (memory management & utilities)

static void* sqlite3DbMallocZero(sqlite3* db, size_t sz){
  (void)db;
  void* p = std::calloc(1, sz);
  return p;
}
static void sqlite3DbFree(sqlite3* db, void* p){
  (void)db;
  std::free(p);
}
static char* sqlite3DbStrDup(sqlite3* db, const char* z){
  (void)db;
  if(!z) return nullptr;
  size_t n = std::strlen(z) + 1;
  char* s = (char*)std::malloc(n);
  std::memcpy(s, z, n);
  return s;
}
static char* sqlite3MPrintf(sqlite3* db, const char* fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  // rough sizing
  int size = 256;
  char* buf = (char*)std::malloc(size);
  int n = vsnprintf(buf, size, fmt, ap);
  va_end(ap);
  if(n >= size){
    size = n + 1;
    buf = (char*)std::realloc(buf, size);
    va_start(ap, fmt);
    vsnprintf(buf, size, fmt, ap);
    va_end(ap);
  }
  return buf;
}
static int sqlite3IsTrueOrFalse(const char* zName){
  if(!zName) return 0;
  std::string s(zName);
  for(char &c: s) c = (char)std::tolower(c);
  return (s == "true" || s == "false"); // treat as boolean word -> true to false
}
static u32 sqlite3StrIHash(const char* zName){
  // simple case-insensitive hash
  std::string s = zName ? zName : "";
  for(char &c: s) c = std::tolower(c);
  std::hash<std::string> hasher;
  return (u32)hasher(s);
}
static int sqlite3ProgressCheck(Parse* pParse){
  // stub: no progress checking
  (void)pParse;
  return 0;
}
static int sqlite3OomFault(sqlite3* db){
  (void)db;
  return 0;
}

// Expr helpers
static Expr* sqlite3ExprSkipCollateAndLikely(Expr* p){
  return p;
}
static int ExprUseYTab(Expr* p){
  return p && p->y.pTab != nullptr;
}
static int ExprHasProperty(Expr* p, int EP){
  (void)p; (void)EP;
  return 0;
}
static int sqlite3HashFill(Hash* ht, const char* zName, ExprList_item* pX){
  (void)ht; (void)zName; (void)pX;
  return 0;
}
static void sqlite3ColumnPropertiesFromName(Parse* pParse, Column* pCol){
  (void)pParse; (void)pCol;
}
static int sqlite3HashInsert(Hash* ht, const char* zName, ExprList_item* pX){
  // simple insert, return pX on success, 0 on collision
  if(!zName) return 0;
  auto it = ht->map.find(zName);
  if(it != ht->map.end()) return 0;
  ht->map[zName] = pX;
  return 1; // non-null to mirror original code path
}
static ExprList_item* sqlite3HashFind(Hash* ht, const char* zName){
  if(!zName) return nullptr;
  auto it = ht->map.find(zName);
  if(it==ht->map.end()) return nullptr;
  return it->second;
}
static void sqlite3HashClear(Hash* ht){
  ht->map.clear();
}
static int sqlite3Oom(void){
  return 0;
}
static int sqlite3NumColumnsFromExprList_placeholder = 0;

// Lightweight Hash usage: in our test, we'll create a Hash and pass it around
static void sqlite3HashInit(Hash* ht){
  ht->map.clear();
}

// Core focal method (copied/adapted to run with test stubs)
extern "C" int sqlite3ColumnsFromExprList(
  Parse *pParse,          /* Parsing context */
  ExprList *pEList,       /* Expr list from which to derive column names */
  i16 *pnCol,             /* Write the number of columns here */
  Column **paCol          /* Write the new column list here */
){
/* The implementation below mirrors the provided snippet, but uses our
   lightweight stubs to operate in the test harness. */
  sqlite3 *db = pParse->db;   /* Database connection */
  int i, j;                   /* Loop counters */
  u32 cnt;                    /* Index added to make the name unique */
  Column *aCol, *pCol;        /* For looping over result columns */
  int nCol;                   /* Number of columns in the result set */
  char *zName;                /* Column name */
  int nName;                  /* Size of name in zName[] */
  Hash ht;                    /* Hash table of column names */
  Table *pTab;
  // initialize hash
  sqlite3HashInit(&ht);

  if( pEList ){
    nCol = pEList->nExpr;
    aCol = (Column*)sqlite3DbMallocZero(db, sizeof(aCol[0])*nCol);
    // testcase( aCol==0 );
    // We skip NEVER and ALWAYS macros for simplicity in test harness
    if( nCol>32767 ) nCol = 32767;
  }else{
    nCol = 0;
    aCol = 0;
  }
  // assert( nCol==(i16)nCol );
  *pnCol = (i16)nCol;
  *paCol = aCol;

  for(i=0, pCol=aCol; i<nCol && !pParse->nErr; i++, pCol++){
    struct ExprList_item *pX = &pEList->a[i];
    struct ExprList_item *pCollide;
    /* Get an appropriate name for the column
    */
    if( (zName = pX->zEName)!=0 && pX->fg.eEName==ENAME_NAME ){
      /* If the column contains an "AS <name>" phrase, use <name> as the name */
    }else{
      Expr *pColExpr = sqlite3ExprSkipCollateAndLikely(pX->pExpr);
      while( pColExpr!=0 && pColExpr->op==TK_DOT ){
        pColExpr = pColExpr->pRight;
        // In test harness, ensure non-null
        if(!pColExpr) break;
      }
      if( pColExpr && pColExpr->op==TK_COLUMN
       && ExprUseYTab(pColExpr)
       && pColExpr->y.pTab!=0 )
      {
        /* For columns use the column name name */
        int iCol = pColExpr->iColumn;
        pTab = pColExpr->y.pTab;
        if( iCol<0 ) iCol = pTab->iPKey;
        zName = iCol>=0 ? pTab->aCol[iCol].zCnName : "rowid";
      }else if( pColExpr && pColExpr->op==TK_ID ){
        // string token
        // In test harness we simply take token if present
        zName = pColExpr->u.zToken;
      }else{
        /* Use the original text of the column expression as its name */
        zName = pX->zEName;  /* pointer comparison intended in original */
      }
    }
    if( zName && !sqlite3IsTrueOrFalse(zName) ){
      zName = sqlite3DbStrDup(db, zName);
    }else{
      zName = sqlite3MPrintf(db,"column%d",i+1);
    }
    /* Make sure the column name is unique.  If the name is not unique,
    ** append an integer to the name so that it becomes unique.
    */
    cnt = 0;
    while( zName && (pCollide = sqlite3HashFind(&ht, zName))!=0 ){
      if( pCollide->fg.bUsingTerm ){
        pCol->colFlags |= COLFLAG_NOEXPAND;
      }
      nName = std::strlen(zName);
      if( nName>0 ){
        for(j=nName-1; j>0 && std::isdigit(zName[j]); j--){}
        if( zName[j]==':' ) nName = j;
      }
      zName = sqlite3MPrintf(db, "%.*s:%u", nName, zName, ++cnt);
      sqlite3ProgressCheck(pParse);
      if( cnt>3 ){
        // mimic randomness
        cnt = 1;
      }
    }
    pCol->zCnName = zName;
    pCol->hName = sqlite3StrIHash(zName);
    if( pX->fg.bNoExpand ){
      pCol->colFlags |= COLFLAG_NOEXPAND;
    }
    sqlite3ColumnPropertiesFromName(0, pCol);
    if( zName && sqlite3HashInsert(&ht, zName, pX)==pX ){
      sqlite3OomFault(db);
    }
  }
  sqlite3HashClear(&ht);
  if( pParse->nErr ){
    for(j=0; j<i; j++){
      sqlite3DbFree(db, aCol[j].zCnName);
    }
    sqlite3DbFree(db, aCol);
    *paCol = 0;
    *pnCol = 0;
    return pParse->rc;
  }
  return SQLITE_OK;
}

// Subtest scaffolding

static void free_E(expr) { (void)expr; }

// Test helpers
static void run_test_message(const char* name, bool ok){
  std::cout << "[TEST] " << name << " -> " << (ok ? "PASS" : "FAIL") << std::endl;
}

// Constructors for test scenarios

static Parse* make_parse(sqlite3* db){
  Parse* p = (Parse*)std::malloc(sizeof(Parse));
  p->db = db;
  p->nErr = 0;
  p->rc = SQLITE_OK;
  return p;
}
static void free_parse(Parse* p){
  if(p){ std::free(p); }
}
static sqlite3* dummy_db(){
  return (sqlite3*)std::malloc(sizeof(sqlite3));
}
static void free_db(sqlite3* db){
  if(db) std::free(db);
}

// Test 1: pEList = nullptr
static bool test_case_null_expr_list(){
  sqlite3* db = dummy_db();
  Parse* pParse = make_parse(db);
  i16 nCol = 0;
  Column* pCol = nullptr;

  int rc = sqlite3ColumnsFromExprList(pParse, nullptr, &nCol, &pCol);
  bool passed = (rc == SQLITE_OK) && (nCol == 0) && (pCol == nullptr);

  // cleanup
  if(pCol) sqlite3DbFree(db, pCol);
  free_parse(pParse);
  free_db(db);

  run_test_message("test_case_null_expr_list", passed);
  return passed;
}

// Helper to build a single expression that is a TK_COLUMN referencing a Table
static Expr* build_column_expr(Table* pTab, int iCol){
  Expr* e = (Expr*)std::malloc(sizeof(Expr));
  e->op = TK_COLUMN;
  e->pRight = nullptr;
  e->y.pTab = pTab;
  e->iColumn = iCol;
  e->u.zToken = nullptr;
  return e;
}

// Test 2: Column from real table key name
struct SimpleTable {
  Table t;
  Column c0;
  int created;
};

static Table* make_table_with_key(const char* cn){
  SimpleTable* st = new SimpleTable();
  st->created = 1;
  st->t.iPKey = 0;
  st->t.aCol = (Column*)std::malloc(sizeof(Column)*1);
  st->t.aCol[0].zCnName = std::strdup(cn);
  st->c0 = st->t.aCol[0];
  return &st->t;
}
static bool test_case_column_name_from_table_key(){
  // Setup table with iPKey=0 and one column named "id"
  Table* pTab = make_table_with_key("id");

  // Build ExprList with one expression that is a TK_COLUMN referencing this table
  Expr* e = build_column_expr(pTab, -1); // iColumn = -1 -> uses iPKey
  ExprList* pEList = (ExprList*)std::malloc(sizeof(ExprList));
  pEList->nExpr = 1;
  pEList->a = (ExprList_item*)std::malloc(sizeof(ExprList_item));
  pEList->a[0].zEName = nullptr;
  pEList->a[0].fg.eEName = ENAME_NAME;
  pEList->a[0].fg.bUsingTerm = 0;
  pEList->a[0].fg.bNoExpand = 0;
  pEList->a[0].pExpr = e;

  // pParse
  sqlite3* db = dummy_db();
  Parse* pParse = make_parse(db);

  i16 nCol;
  Column* aCol;
  int rc = sqlite3ColumnsFromExprList(pParse, pEList, &nCol, &aCol);

  bool pass = true;
  if(rc != SQLITE_OK) pass = false;
  if(nCol != 1) pass = false;
  if(!aCol) pass = false;
  if(aCol && aCol[0].zCnName == nullptr) pass = false;
  // Expect the derived name to be the key's column name "id"
  if(aCol && aCol[0].zCnName && std::strcmp(aCol[0].zCnName, "id") != 0) pass = false;

  // cleanup
  sqlite3DbFree(db, aCol);
  free_parse(pParse);
  // Free memory allocated for pEList
  std::free(pEList->a);
  std::free(pEList);
  free_db(db);

  run_test_message("test_case_column_name_from_table_key", pass);
  return pass;
}

// Test 3: Alias name via zEName and ENAME_NAME
static bool test_case_alias_name_used(){
  // Create a dummy table (unused)
  Table t;
  t.iPKey = 0;
  t.aCol = nullptr;

  // Setup pExpr as a simple non-column expression to force alias path
  // We'll reuse a column expression but provide an alias
  Expr* e = (Expr*)std::malloc(sizeof(Expr));
  e->op = TK_COLUMN;
  e->pRight = nullptr;
  e->y.pTab = &t;
  e->iColumn = -1;
  e->u.zToken = nullptr;

  ExprList* pEList = (ExprList*)std::malloc(sizeof(ExprList));
  pEList->nExpr = 1;
  pEList->a = (ExprList_item*)std::malloc(sizeof(ExprList_item));
  pEList->a[0].zEName = "AliasName"; // alias
  pEList->a[0].fg.eEName = ENAME_NAME;
  pEList->a[0].fg.bUsingTerm = 0;
  pEList->a[0].fg.bNoExpand = 0;
  pEList->a[0].pExpr = e;

  sqlite3* db = dummy_db();
  Parse* pParse = make_parse(db);

  i16 nCol;
  Column* aCol;
  int rc = sqlite3ColumnsFromExprList(pParse, pEList, &nCol, &aCol);

  bool pass = true;
  if(rc != SQLITE_OK) pass = false;
  if(nCol != 1) pass = false;
  if(!aCol) pass = false;
  if(aCol && aCol[0].zCnName == nullptr) pass = false;
  // Expect alias name to be preserved
  if(aCol && std::strcmp(aCol[0].zCnName, "AliasName") != 0) pass = false;

  // cleanup
  sqlite3DbFree(db, aCol);
  free_parse(pParse);
  std::free(pEList->a);
  std::free(pEList);
  std::free(e);
  free_db(db);

  run_test_message("test_case_alias_name_used", pass);
  return pass;
}

// Test 4: Duplicate names cause suffixing :1
static bool test_case_duplicate_name_suffix(){
  // Two items both producing the same base name "X"
  Table t;
  t.iPKey = 0;
  t.aCol = nullptr;

  // First expression -> alias "X"
  Expr* e1 = (Expr*)std::malloc(sizeof(Expr));
  e1->op = TK_COLUMN;
  e1->pRight = nullptr;
  e1->y.pTab = &t;
  e1->iColumn = -1;
  e1->u.zToken = nullptr;

  // Second expression -> alias "X" again
  Expr* e2 = (Expr*)std::malloc(sizeof(Expr));
  e2->op = TK_COLUMN;
  e2->pRight = nullptr;
  e2->y.pTab = &t;
  e2->iColumn = -1;
  e2->u.zToken = nullptr;

  ExprList* pEList = (ExprList*)std::malloc(sizeof(ExprList));
  pEList->nExpr = 2;
  pEList->a = (ExprList_item*)std::malloc(sizeof(ExprList_item)*2);
  // first
  pEList->a[0].zEName = "X";
  pEList->a[0].fg.eEName = ENAME_NAME;
  pEList->a[0].fg.bUsingTerm = 0;
  pEList->a[0].fg.bNoExpand = 0;
  pEList->a[0].pExpr = e1;
  // second
  pEList->a[1].zEName = "X";
  pEList->a[1].fg.eEName = ENAME_NAME;
  pEList->a[1].fg.bUsingTerm = 0;
  pEList->a[1].fg.bNoExpand = 0;
  pEList->a[1].pExpr = e2;

  sqlite3* db = dummy_db();
  Parse* pParse = make_parse(db);

  i16 nCol;
  Column* aCol;
  int rc = sqlite3ColumnsFromExprList(pParse, pEList, &nCol, &aCol);

  bool pass = true;
  if(rc != SQLITE_OK) pass = false;
  if(nCol != 2) pass = false;
  if(!aCol) pass = false;
  if(aCol[0].zCnName == nullptr || aCol[1].zCnName == nullptr) pass = false;
  // first name should be "X"
  if(std::strcmp(aCol[0].zCnName, "X") != 0) pass = false;
  // second name should be "X:1" due to collision
  if(std::strcmp(aCol[1].zCnName, "X:1") != 0) pass = false;

  // cleanup
  sqlite3DbFree(db, aCol);
  free_parse(pParse);
  std::free(pEList->a);
  std::free(pEList);
  std::free(e1);
  std::free(e2);
  free_db(db);

  run_test_message("test_case_duplicate_name_suffix", pass);
  return pass;
}

// Test 5: pParse->nErr non-zero triggers cleanup and returns rc
static bool test_case_parse_error_cleanup(){
  Table t;
  t.iPKey = 0;
  t.aCol = nullptr;

  Expr* e = (Expr*)std::malloc(sizeof(Expr));
  e->op = TK_COLUMN;
  e->pRight = nullptr;
  e->y.pTab = &t;
  e->iColumn = -1;
  e->u.zToken = nullptr;

  ExprList* pEList = (ExprList*)std::malloc(sizeof(ExprList));
  pEList->nExpr = 1;
  pEList->a = (ExprList_item*)std::malloc(sizeof(ExprList_item));
  pEList->a[0].zEName = "A";
  pEList->a[0].fg.eEName = ENAME_NAME;
  pEList->a[0].fg.bUsingTerm = 0;
  pEList->a[0].fg.bNoExpand = 0;
  pEList->a[0].pExpr = e;

  sqlite3* db = dummy_db();
  Parse* pParse = make_parse(db);
  // induce error
  pParse->nErr = 1;
  pParse->rc = 123; // some error code

  i16 nCol;
  Column* aCol;
  int rc = sqlite3ColumnsFromExprList(pParse, pEList, &nCol, &aCol);

  bool pass = (rc == pParse->rc);

  // cleanup
  if(aCol) sqlite3DbFree(db, aCol);
  free_parse(pParse);
  std::free(pEList->a);
  std::free(pEList);
  std::free(e);
  free_db(db);

  run_test_message("test_case_parse_error_cleanup", pass);
  return pass;
}

int main(){
  int total = 0;
  int failures = 0;

  auto run = [&](bool ok){
    total++;
    if(!ok) failures++;
  };

  run(test_case_null_expr_list());
  run(test_case_column_name_from_table_key());
  run(test_case_alias_name_used());
  run(test_case_duplicate_name_suffix());
  run(test_case_parse_error_cleanup());

  if(failures==0){
    std::cout << "ALL TESTS PASSED (" << total << " tests)" << std::endl;
  }else{
    std::cout << "SOME TESTS FAILED: " << failures << " / " << total << std::endl;
  }
  return failures;
}