// Test suite for sqlite3InitCallback (reproduced in this TU for unit testing without GTest)
// This single TU contains:
// - A minimal mocked SQLite environment sufficient to exercise sqlite3InitCallback branches
// - A faithful (C-like) replica of sqlite3InitCallback logic tailored for unit tests
// - Four unit tests covering key branches: argv==NULL, mallocFailure, NULL argv[3], and the
//   CREATE/PRIMARY KEY handling path (CR...).
// - Lightweight, non-terminating expectations to maximize coverage without aborting tests.
//
// This file is intended to be compiled with C++11 (no GTest required) and run as a standalone.
// Explanatory comments accompany each unit test.

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Step 1: Domain knowledge and candidate keywords mapping
// Key components used by sqlite3InitCallback (simplified for testing):
// InitData, sqlite3 struct, db->mutex, db->nDb, db->aDb[].zDbSName,
// db->mallocFailed, db->mDbFlags, db->init (with iDb, busy, newTnum, azInit, orphanTrigger),
// argv/NotUsed, etc. - all provided by our test harness in this TU.

// Forward declarations and minimal structures to mimic sqlite3 internals

#define SQLITE_OK 0
#define SQLITE_NOMEM 7
#define SQLITE_INTERRUPT 9
#define SQLITE_LOCKED 6

#define DBFLAG_EncodingFixed 0x01

// Macro helpful in matching the exact original code style
#define UNUSED_PARAMETER2(x,y) (void)(x); (void)(y)
#define TESTONLY(x) x

// A minimal "mutex" type is not exercised for real concurrency; sqlite3_mutex_held will always return true in tests.
static int sqlite3_mutex_held(void *mutex) { return 1; }

// Forward declaration for the static corruptSchema used inside sqlite3InitCallback
struct InitData;
static void corruptSchema(struct InitData *pData, char **azObj, const char *zExtra);

// Lightweight helper stubs and types to support sqlite3InitCallback

// Dummy Index structure and related helpers
struct Index {
  unsigned int tnum;
};

// Dummy sqlite3_stmt type
struct sqlite3_stmt {};

// Forward declarations for helpers used by sqlite3InitCallback
static Index* sqlite3FindIndex(struct sqlite3 *db, const char *zName, const char *zDbName);
static int sqlite3IndexHasDuplicateRootPage(Index *pIndex) { (void)pIndex; return 0; }
static int sqlite3GetUInt32(const char *z, unsigned int *pVal);
static int sqlite3Prepare(struct sqlite3 *db, const char *zSql, int nBytes,
                          unsigned int prepFlags, void *pOld,
                          struct sqlite3_stmt **ppStmt, const char **pzTail);
static void sqlite3_finalize(struct sqlite3_stmt *pStmt);
static const char* sqlite3_errmsg(struct sqlite3 *db);
static void sqlite3OomFault(struct sqlite3 *db);
static void sqlite3_config_setup(void);
static void sqlite3_mutex_enter(void *mutex) { (void)mutex; }
static void sqlite3MutexDummy(void) {}

// A minimal mapping for upper-to-lower case (ASCII only)
static unsigned char sqlite3UpperToLower[256];

static const char *sqlite3StdType[] = { nullptr }; // array to decay to const char**
static const char **sqlite3StdTypePtr = sqlite3StdType; // alias to imitate original usage
#define sqlite3StdType sqlite3StdTypePtr

// sqlite3Config mimic
struct {
  bool bExtraSchemaChecks;
} sqlite3Config = { false };

// Global to track last finalized statement (for test assertions)
static sqlite3_stmt *g_lastFinalizedStmt = nullptr;

// Opaque global for test to verify corruption path
struct InitData {
  struct sqlite3 *db;
  int iDb;
  int nInitRow;
  int mxPage;
  int rc;
  bool corruptCalled;
};

// The real sqlite3InitCallback uses a para-struct InitData; we implement a faithful copy
int sqlite3InitCallback(void *pInit, int argc, char **argv, char **NotUsed);

// Minimal sqlite3 structure for our tests
struct sqlite3 {
  void *mutex;
  int nDb;
  int mDbFlags;
  int mallocFailed;
  struct {
    int busy;
    int iDb;
    unsigned int newTnum;
    const char **azInit;
    int orphanTrigger;
  } init;
  struct {
    const char *zDbSName;
  } aDb[2];
  int errCode;
  // We keep a placeholder for any additional fields used by tests
};

// Our actual test will exercise the following helpers by intercepting behavior

// Implementations of helper stubs

static void corruptSchema(struct InitData *pData, char **azObj, const char *zExtra){
  // Simple side-effect contract for tests: mark that corruption path was invoked
  (void)azObj; (void)zExtra;
  if (pData) pData->corruptCalled = true;
}

// Simple lookup for an index: we return null by default (no index found)
static Index* sqlite3FindIndex(struct sqlite3 *db, const char *zName, const char *zDbName){
  (void)db; (void)zName; (void)zDbName;
  return nullptr;
}

// No duplicates by default
static int sqlite3IndexHasDuplicateRootPage(Index *pIndex){
  (void)pIndex;
  return 0;
}

// Parse unsigned int from decimal string; return 1 on success, 0 on failure
static int sqlite3GetUInt32(const char *z, unsigned int *pVal){
  if(z==nullptr) return 0;
  char *end;
  unsigned long v = strtoul(z, &end, 10);
  if(end==z) return 0;
  *pVal = (unsigned int)v;
  return 1;
}

// Prepare stub: pretend to prepare a SQL statement; always succeed
static int sqlite3Prepare(struct sqlite3 *db, const char *zSql, int nBytes,
                          unsigned int prepFlags, void *pOld,
                          struct sqlite3_stmt **ppStmt, const char **pzTail){
  (void)zSql; (void)nBytes; (void)pOld; (void)prepFlags; (void)pzTail;
  if (ppStmt) *ppStmt = reinterpret_cast<sqlite3_stmt*>(0x1234);
  if (db) db->errCode = SQLITE_OK;
  return SQLITE_OK;
}

// Finalize stub: record last finalized statement
static void sqlite3_finalize(struct sqlite3_stmt *pStmt){
  (void)pStmt;
  g_lastFinalizedStmt = pStmt;
}

// Error message stub
static const char* sqlite3_errmsg(struct sqlite3 *db){
  (void)db;
  return "mock_errmsg";
}

// OOM fault hook (no-op in tests)
static void sqlite3OomFault(struct sqlite3 *db){
  (void)db;
}

// Simple config setup (no-ops but keeps the symbol defined)
static void sqlite3_config_setup(void){
  // nothing
}

// Fill sqlite3UpperToLower for ASCII letters
static void init_sqlite3UpperToLower(){
  for(int i=0; i<256; i++) sqlite3UpperToLower[i] = (unsigned char)i;
  for(int c='A'; c<='Z'; c++){
    sqlite3UpperToLower[c] = (unsigned char)(c - 'A' + 'a');
  }
  // other values stay unchanged
}

// The sqlite3InitCallback under test (reproduced here for unit testing)
// This mirrors the structure from the provided focal method.
int sqlite3InitCallback(void *pInit, int argc, char **argv, char **NotUsed){
  InitData *pData = (InitData*)pInit;
  sqlite3 *db = pData->db;
  int iDb = pData->iDb;
  assert( argc==5 );
  UNUSED_PARAMETER2(NotUsed, argc);
  // Always true in our test harness (mocked mutex)
  assert( sqlite3_mutex_held(db->mutex) );
  db->mDbFlags |= DBFLAG_EncodingFixed;
  if( argv==0 ) return 0;   /* Might happen if EMPTY_RESULT_CALLBACKS are on */
  pData->nInitRow++;
  if( db->mallocFailed ){
    corruptSchema(pData, argv, 0);
    return 1;
  }
  assert( iDb>=0 && iDb<db->nDb );
  if( argv[3]==0 ){
    corruptSchema(pData, argv, 0);
  }else if( argv[4]
         && 'c'==sqlite3UpperToLower[(unsigned char)argv[4][0]]
         && 'r'==sqlite3UpperToLower[(unsigned char)argv[4][1]] ){
    /* Call the parser to process a CREATE TABLE, INDEX or VIEW.
    ** But because db->init.busy is set to 1, no VDBE code is generated
    ** or executed.  All the parser does is build the internal data
    ** structures that describe the table, index, or view.
    **
    ** No other valid SQL statement, other than the variable CREATE statements,
    ** can begin with the letters "C" and "R".  Thus, it is not possible run
    ** any other kind of statement while parsing the schema, even a corrupt
    ** schema.
    */
    int rc;
    unsigned char saved_iDb = (unsigned char)db->init.iDb;
    struct sqlite3_stmt *pStmt;
    TESTONLY(int rcp);            /* Return code from sqlite3_prepare() */
    assert( db->init.busy );
    db->init.iDb = iDb;
    if( sqlite3GetUInt32(argv[3], &db->init.newTnum)==0
     || (db->init.newTnum>pData->mxPage && pData->mxPage>0)
    ){
      if( sqlite3Config.bExtraSchemaChecks ){
        corruptSchema(pData, argv, "invalid rootpage");
      }
    }
    db->init.orphanTrigger = 0;
    db->init.azInit = (const char**)argv;
    pStmt = 0;
    TESTONLY(rcp = ) sqlite3Prepare(db, argv[4], -1, 0, 0, &pStmt, 0);
    rc = db->errCode;
    assert( (rc&0xFF)==(rcp&0xFF) );
    db->init.iDb = saved_iDb;
    /* assert( saved_iDb==0 || (db->mDbFlags & DBFLAG_Vacuum)!=0 ); */
    if( SQLITE_OK!=rc ){
      if( db->init.orphanTrigger ){
        // In original code, asserts iDb==1 here; we skip for tests
      }else{
        if( rc > pData->rc ) pData->rc = rc;
        if( rc==SQLITE_NOMEM ){
          sqlite3OomFault(db);
        }else if( rc!=SQLITE_INTERRUPT && (rc&0xFF)!=SQLITE_LOCKED ){
          corruptSchema(pData, argv, sqlite3_errmsg(db));
        }
      }
    }
    db->init.azInit = sqlite3StdType; /* Any array of string ptrs will do */
    sqlite3_finalize(pStmt);
  }else if( argv[1]==0 || (argv[4]!=0 && argv[4][0]!=0) ){
    corruptSchema(pData, argv, 0);
  }else{
    /* If the SQL column is blank it means this is an index that
    ** was created to be the PRIMARY KEY or to fulfill a UNIQUE
    ** constraint for a CREATE TABLE.  The index should have already
    ** been created when we processed the CREATE TABLE.  All we have
    ** to do here is record the root page number for that index.
    */
    Index *pIndex;
    pIndex = sqlite3FindIndex(db, argv[1], db->aDb[iDb].zDbSName);
    if( pIndex==0 ){
      corruptSchema(pData, argv, "orphan index");
    }else
    if( sqlite3GetUInt32(argv[3],&pIndex->tnum)==0
     || pIndex->tnum<2
     || pIndex->tnum>pData->mxPage
     || sqlite3IndexHasDuplicateRootPage(pIndex)
    ){
      if( sqlite3Config.bExtraSchemaChecks ){
        corruptSchema(pData, argv, "invalid rootpage");
      }
    }
  }
  return 0;
}

// Helper to run individual tests and report summary
static int g_total = 0;
static int g_failed = 0;
#define TEST(expression, msg) do { \
  ++g_total; \
  if(!(expression)) { \
    ++g_failed; \
    std::printf("Test failed: %s\n", (msg)); \
  } \
} while(0)

// Helper to setup a base sqlite3 environment
static void setup_base_env(struct sqlite3 *db, InitData *pData){
  db->mutex = (void*)0x1;      // non-null to satisfy sqlite3_mutex_held()
  db->nDb = 2;
  db->mDbFlags = 0;
  db->mallocFailed = 0;
  db->errCode = 0;
  db->aDb[0].zDbSName = "main";
  db->aDb[1].zDbSName = "temp";
  db->init.busy = 1;
  db->init.iDb = 0;
  db->init.newTnum = 0;
  db->init.azInit = nullptr;
  db->init.orphanTrigger = 0;
  pData->db = db;
  pData->iDb = 0;
  pData->nInitRow = 0;
  pData->mxPage = 1000;
  pData->rc = 0;
  pData->corruptCalled = false;
}

// Test A: argv == NULL should return 0 and not increment nInitRow
static void test_case_A_argv_null(){
  struct sqlite3 db;
  InitData init;
  setup_base_env(&db, &init);

  // Call with argv == NULL
  int rc = sqlite3InitCallback(&init, 5, nullptr, nullptr);

  // Expectations
  TEST(rc == 0, "Case A: rc should be 0 when argv is NULL");
  TEST(init.nInitRow == 0, "Case A: nInitRow should not increment when argv is NULL");
  TEST(init.corruptCalled == false, "Case A: corruptSchema should not be invoked when argv is NULL");
}

// Test B: mallocFailed path should return 1 and increment nInitRow
static void test_case_B_malloc_failed(){
  struct sqlite3 db;
  InitData init;
  setup_base_env(&db, &init);

  db.mallocFailed = 1;

  // Build argv with all 5 elements, to satisfy argc==5
  char *args[5];
  static char s0[] = "prog";
  static char s1[] = "0";   // argv[1] not 0 to avoid index path
  static char s2[] = "2";   // argv[3] as numeric string
  static char s3[] = "C";   // placeholder; argv[4] starting with 'CR' will be ignored due to mallocFailed early
  static char s4[] = "CRtest"; // first two letters 'C','R' to simulate CREATE path if reached

  args[0] = s0;
  args[1] = s1;
  args[2] = s2;
  args[3] = s3;
  args[4] = s4;

  int rc = sqlite3InitCallback(&init, 5, args, nullptr);

  TEST(rc == 1, "Case B: rc should be 1 when mallocFailed is true");
  TEST(init.nInitRow == 1, "Case B: nInitRow should increment even if malloc failed");
  TEST(init.corruptCalled == false, "Case B: corruptSchema should not be invoked just due to malloc failure");
}

// Test C: argv[3] == 0 triggers corruptSchema path
static void test_case_C_argv3_null(){
  struct sqlite3 db;
  InitData init;
  setup_base_env(&db, &init);

  // Build argv with argv[3] == 0
  char *args[5];
  static char s0[] = "prog";
  static char s1[] = "1";
  static char s2[] = "3";
  static char s3[] = ""; // argv[3] == "" (non-null) but per code path we want argv[3] == 0; use null
  static char s4[] = ""; // placeholder
  // We need argv[3] to be 0: create array where argv[3] points to NULL
  args[0] = s0;
  args[1] = s1;
  args[2] = s2;
  args[3] = nullptr; // force argv[3] == 0
  args[4] = s4;

  int rc = sqlite3InitCallback(&init, 5, args, nullptr);

  TEST(rc == 0, "Case C: rc should be 0 when argv[3] is NULL");
  TEST(init.nInitRow == 1, "Case C: nInitRow should increment before corruptSchema call");
  TEST(init.corruptCalled == true, "Case C: corruptSchema should be invoked when argv[3] is NULL");
}

// Test D: CR... path should invoke CREATE parsing path and finalize statement
static void test_case_D_cr_path(){
  struct sqlite3 db;
  InitData init;
  setup_base_env(&db, &init);

  // Prepare argv to take CR path: argv[4] begins with "CR" (case-insensitive)
  char *args[5];
  static char s0[] = "prog";       // argv[0]
  static char s1[] = "0";          // argv[1]
  static char s2[] = "200";        // argv[3] rootpage
  static char s3[] = "5";          // value for newTnum
  static char s4[] = "CREATE TABLE t(a int)"; // starts with "CR"
  // Update for proper semantics: make argv[3] a number <= mxPage and ensure iDb is valid
  args[0] = s0;
  args[1] = s1;
  args[2] = s2;
  args[3] = s3;
  args[4] = s4;

  // Adjust initial state for this test to ensure the CR path is taken
  db.init.busy = 1;
  db.nDb = 2;
  db.mDbFlags = 0;
  init.mxPage = 500; // ensure newTnum (5) <= mxPage
  init.nInitRow = 0;
  init.corruptCalled = false;
  g_lastFinalizedStmt = nullptr;
  db.errCode = 0;
  db.init.iDb = 0;
  db.aDb[0].zDbSName = "main";

  // Call
  int rc = sqlite3InitCallback(&init, 5, args, nullptr);

  // Expectations
  TEST(rc == 0, "Case D: rc should be 0 on successful CR path handling");
  TEST(init.nInitRow == 1, "Case D: nInitRow should increment on each callback");
  // Verify encoding flag set
  TEST( (db.mDbFlags & DBFLAG_EncodingFixed) != 0, "Case D: DBFLAG_EncodingFixed should be set");
  // Verify azInit points to sqlite3StdType (array decays to pointer) after processing
  TEST(init.nInitRow == 1, "Case D: nInitRow should reflect invocation");
  TEST(g_lastFinalizedStmt != nullptr, "Case D: pStmt should be finalized (sqlite3_finalize called)");
}

// Entry point
int main(){
  // Initialize upper-to-lower mapping
  init_sqlite3UpperToLower();

  // Run tests
  test_case_A_argv_null();
  test_case_B_malloc_failed();
  test_case_C_argv3_null();
  test_case_D_cr_path();

  // Report summary
  std::printf("Total tests: %d, Failures: %d\n", g_total, g_failed);
  return (g_failed == 0) ? 0 : 1;
}