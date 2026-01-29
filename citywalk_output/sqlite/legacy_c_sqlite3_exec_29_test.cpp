// Test harness for the focal method sqlite3_exec (re-implemented in this file for unit testing
// under C++11 without GoogleTest). This test suite provides lightweight tests and explanatory
// comments for each case, focusing on coverage of true/false branches and key predicates.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cctype>
#include <map>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Domain knowledge: basic constants used by sqlite3_exec (subset sufficient for tests).
enum {
  SQLITE_OK = 0,
  SQLITE_ROW = 100,
  SQLITE_DONE = 101,
  SQLITE_ABORT = 999,
  SQLITE_NOMEM_BKPT = 1,
  SQLITE_MISUSE_BKPT = 2,
  SQLITE_NULL = 0,
  SQLITE_INTEGER = 1,
  SQLITE_TEXT = 3,
  // Convenience macro-like values used in tests
  SQLITE_ERROR = 1
};

// Forward declarations to mimic SQLite types
typedef int (*sqlite3_callback)(void*, int, char**, char**);

// Lightweight sqlite3 structure and related helpers (mocked/minimal)
struct sqlite3 {
  int mutex;        // not used in tests (no real threading in mock)
  int flags;        // used for SQLITE_NullCallback-like behavior (not used in tests)
  int errMask;      // not used directly in tests
  int misuse;        // if non-zero, sqlite3SafetyCheckOk will fail (simulate MISUSE)
  const char* last_err; // store last error string
};

// Fake sqlite3_stmt and Vdbe types (pointers used for stateful mock)
typedef void sqlite3_stmt;
typedef void Vdbe;

// Global state for prepared statements (simple mock)
struct PStmtState {
  bool prepared;
  int nCol;
  int currentRow;
  // Additional fields could be added for more fidelity
  std::string sql;
};

// Map to track statement state by the pointer value used in tests
static std::map<sqlite3_stmt*, PStmtState> g_stmtStates;
static int g_nextStmtId = 1; // not used functionally, but helps simulate distinct statements

// Utility: was_someone calling sqlite3SafetyCheckOk properly?
int sqlite3SafetyCheckOk(sqlite3* db) {
  if(!db) return 0;
  return (db->misuse == 0);
}

// Synchronization primitives (no real threading in tests)
void sqlite3_mutex_enter(sqlite3* db) { (void)db; }
void sqlite3_mutex_leave(sqlite3* db) { (void)db; }

// Error handling helpers
void sqlite3Error(sqlite3* db, int rc) {
  (void)rc;
  if(db) {
    // Store a readable message for tests
    switch(rc) {
      case SQLITE_ABORT: db->last_err = "ABORT"; break;
      case SQLITE_MISUSE_BKPT: db->last_err = "MISUSE"; break;
      case SQLITE_NOMEM_BKPT: db->last_err = "NOMEM"; break;
      default: db->last_err = "OK"; break;
    }
  }
}
const char* sqlite3_errmsg(sqlite3* db) {
  if(db && db->last_err) return db->last_err;
  return "OK";
}
char* sqlite3DbStrDup(sqlite3* db, const char* z) {
  (void)db;
  if(!z) return NULL;
  char* s = (char*)malloc(strlen(z) + 1);
  if(!s) return NULL;
  strcpy(s, z);
  return s;
}

// Memory management helpers (mock)
void* sqlite3DbMallocRaw(sqlite3* db, int n) {
  (void)db;
  if(n <= 0) return NULL;
  return malloc(n);
}
void sqlite3DbFree(sqlite3* db, void* p) {
  (void)db;
  if(p) free(p);
}

// Helpers for string/space utilities
int sqlite3Isspace(char c) {
  return std::isspace(static_cast<unsigned char>(c));
}

// Column helpers (mock)
int sqlite3VdbeFinalize(Vdbe* p) {
  (void)p;
  // In tests, no resource to finalize beyond removing state if present
  return SQLITE_OK;
}
int sqlite3ColumnCount(sqlite3_stmt* pStmt);
const char* sqlite3_column_name(sqlite3_stmt* pStmt, int i);
const char* sqlite3_column_text(sqlite3_stmt* pStmt, int i);
int sqlite3_column_type(sqlite3_stmt* pStmt, int i);

// The faux API exit (no real SQLite API semantics needed for tests)
int sqlite3ApiExit(sqlite3* db, int rc) { (void)db; return rc; }

// OOM fault (no-op in tests)
void sqlite3OomFault(sqlite3* db) {
  (void)db;
}

// Minimal VDBE-like environment for our simplified tests
int sqlite3ColumnCount(sqlite3_stmt* pStmt) {
  auto it = g_stmtStates.find(pStmt);
  if(it == g_stmtStates.end()) return 0;
  return it->second.nCol;
}
const char* sqlite3_column_name(sqlite3_stmt* pStmt, int i) {
  (void)i;
  // For our mock, return a fixed column name
  static const char* name = "col0";
  (void)pStmt;
  return name;
}
const char* sqlite3_column_text(sqlite3_stmt* pStmt, int i) {
  (void)i;
  // Return a stable test string
  return "row0";
}
int sqlite3_column_type(sqlite3_stmt* pStmt, int i) {
  (void)pStmt;
  (void)i;
  // Return TEXT type for test
  return SQLITE_TEXT;
}

// Prepare/Step/Finalize mocks
int sqlite3_prepare_v2(sqlite3* db, const char* zSql, int nByte, sqlite3_stmt** ppStmt, const char** pzLeftover) {
  (void)nByte;
  (void)db;
  const char* s = zSql ? zSql : "";
  // Trim spaces
  while(*s && sqlite3Isspace(*s)) s++;
  if(*s == '\0') {
    // whitespace only => simulate pStmt == 0 (whitespace/comment case)
    if(ppStmt) *ppStmt = NULL;
    if(pzLeftover) *pzLeftover = "";
    return SQLITE_OK;
  }
  // If zSql contains "ERROR", simulate a prepare error (rc != SQLITE_OK) but not a NULL pStmt
  if(std::strstr(s, "ERROR") != NULL) {
    if(ppStmt) *ppStmt = NULL;
    if(pzLeftover) *pzLeftover = s;
    return SQLITE_ERROR;
  }
  // Otherwise, create a dummy prepared statement
  sqlite3_stmt* stmt = (sqlite3_stmt*) (void*) (g_nextStmtId++);
  PStmtState st;
  st.prepared = true;
  st.nCol = 1;
  st.currentRow = 0;
  st.sql = s;
  g_stmtStates[stmt] = st;
  if(ppStmt) *ppStmt = stmt;
  if(pzLeftover) *pzLeftover = "";
  return SQLITE_OK;
}
int sqlite3_step(sqlite3_stmt* pStmt) {
  auto it = g_stmtStates.find(pStmt);
  if(it == g_stmtStates.end()) return SQLITE_DONE;
  PStmtState& st = it->second;
  if(st.currentRow == 0) {
    st.currentRow = 1;
    return SQLITE_ROW;
  } else {
    return SQLITE_DONE;
  }
}
int sqlite3VdbeFinalize(Vdbe* pStmt) {
  (void)pStmt;
  // In our mock, remove from map if exists
  auto it = g_stmtStates.begin();
  if(it != g_stmtStates.end()) {
    g_stmtStates.erase(it);
  }
  return SQLITE_OK;
}
int sqlite3Isspace(char c) { return std::isspace(static_cast<unsigned char>(c)); }

// The actual focal function under test (reconstructed here for unit testing)
int sqlite3_exec(
  sqlite3 *db,                /* The database on which the SQL executes */
  const char *zSql,           /* The SQL to be executed */
  sqlite3_callback xCallback, /* Invoke this callback routine */
  void *pArg,                 /* First argument to xCallback() */
  char **pzErrMsg             /* Write error messages here */
){
  int rc = SQLITE_OK;         /* Return code */
  const char *zLeftover;      /* Tail of unprocessed SQL */
  sqlite3_stmt *pStmt = 0;    /* The current SQL statement */
  char **azCols = 0;          /* Names of result columns */
  int callbackIsInit;         /* True if callback data is initialized */
  if( !sqlite3SafetyCheckOk(db) ) return SQLITE_MISUSE_BKPT;
  if( zSql==0 ) zSql = "";
  sqlite3_mutex_enter(db);
  sqlite3Error(db, SQLITE_OK);
  while( rc==SQLITE_OK && zSql[0] ){
    int nCol = 0;
    char **azVals = 0;
    pStmt = 0;
    rc = sqlite3_prepare_v2(db, zSql, -1, &pStmt, &zLeftover);
    assert( rc==SQLITE_OK || pStmt==0 );
    if( rc!=SQLITE_OK ){
      continue;
    }
    if( !pStmt ){
      /* this happens for a comment or white-space */
      zSql = zLeftover;
      continue;
    }
    callbackIsInit = 0;
    while( 1 ){
      int i;
      rc = sqlite3_step(pStmt);
      /* Invoke the callback function if required */
      if( xCallback && (SQLITE_ROW==rc || 
          (SQLITE_DONE==rc && !callbackIsInit
                           && db->flags&0) ) ){
        if( !callbackIsInit ){
          nCol = sqlite3_column_count(pStmt);
          azCols = sqlite3DbMallocRaw(db, (2*nCol+1)*sizeof(const char*));
          if( azCols==0 ){
            goto exec_out;
          }
          for(i=0; i<nCol; i++){
            azCols[i] = (char *)sqlite3_column_name(pStmt, i);
            /* sqlite3VdbeSetColName() installs column names as UTF8
            ** strings so there is no way for sqlite3_column_name() to fail. */
            // In mock, ensure non-null
            assert( azCols[i]!=0 );
          }
          callbackIsInit = 1;
        }
        if( rc==SQLITE_ROW ){
          azVals = &azCols[nCol];
          for(i=0; i<nCol; i++){
            azVals[i] = (char *)sqlite3_column_text(pStmt, i);
            if( !azVals[i] && sqlite3_column_type(pStmt, i)!=SQLITE_NULL ){
              sqlite3OomFault(db);
              goto exec_out;
            }
          }
          azVals[i] = 0;
        }
        if( xCallback(pArg, nCol, azVals, azCols) ){
          /* EVIDENCE-OF: R-38229-40159 If the callback function to
          ** sqlite3_exec() returns non-zero, then sqlite3_exec() will
          ** return SQLITE_ABORT. */
          rc = SQLITE_ABORT;
          sqlite3VdbeFinalize((Vdbe *)pStmt);
          pStmt = 0;
          sqlite3Error(db, SQLITE_ABORT);
          goto exec_out;
        }
      }
      if( rc!=SQLITE_ROW ){
        rc = sqlite3VdbeFinalize((Vdbe *)pStmt);
        pStmt = 0;
        zSql = zLeftover;
        while( sqlite3Isspace(zSql[0]) ) zSql++;
        break;
      }
    }
    sqlite3DbFree(db, azCols);
    azCols = 0;
  }
exec_out:
  if( pStmt ) sqlite3VdbeFinalize((Vdbe *)pStmt);
  sqlite3DbFree(db, azCols);
  rc = sqlite3ApiExit(db, rc);
  if( rc!=SQLITE_OK && pzErrMsg ){
    *pzErrMsg = sqlite3DbStrDup(0, sqlite3_errmsg(db));
    if( *pzErrMsg==0 ){
      rc = SQLITE_NOMEM_BKPT;
      sqlite3Error(db, SQLITE_NOMEM);
    }
  }else if( pzErrMsg ){
    *pzErrMsg = 0;
  }
  assert( (rc&db->errMask)==rc );
  sqlite3_mutex_leave(db);
  return rc;
}

// Lightweight test harness (non-GTest) with non-terminating expectations
static int g_totalPass = 0;
static int g_totalFail = 0;

#define LOG(msg) do { std::cerr << msg << std::endl; } while(0)
#define EXPECT_TRUE(cond) do { \
  if(!(cond)){ \
    LOG("EXPECT_TRUE FAILED: " #cond " at line " << __LINE__); \
    g_totalFail++; \
  } else { g_totalPass++; } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if((a)!=(b)){ \
    LOG("EXPECT_EQ FAILED: " << (a) << " != " << (b) << " at line " << __LINE__); \
    g_totalFail++; \
  } else { g_totalPass++; } \
} while(0)

#define EXPECT_STR_EQ(a,b) do { \
  if(((a)==NULL && (b)!=NULL) || ((a)!=NULL && (b)==NULL) || ((a)&&(b) && std::strcmp((a),(b)) != 0)){ \
    LOG("EXPECT_STR_EQ FAILED: \"" << (a?a:"<NULL>\") << "\" != \"" << (b?b:"<NULL>\") << "\" at line " << __LINE__); \
    g_totalFail++; \
  } else { g_totalPass++; } \
} while(0)


// Test 1: Basic successful execution with one row and normal callback
int test_basic_success() {
  g_stmtStates.clear();
  sqlite3 db;
  db.misuse = 0;
  db.flags = 0;
  db.errMask = 0;
  db.last_err = nullptr;

  std::vector<std::string> rows;
  auto callback = [](void* arg, int nCol, char** azVals, char** azCols) -> int {
    std::vector<std::string>* out = static_cast<std::vector<std::string>*>(arg);
    if(nCol > 0 && azVals && azVals[0]) {
      out->push_back(std::string(azVals[0]));
    }
    // Do not abort
    return 0;
  };

  const char* sql = "SELECT 1";
  char* errorMsg = nullptr;

  int rc = sqlite3_exec(&db, sql, callback, &rows, &errorMsg);

  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(rows.size(), 1);
  if(rows.size() == 1) {
    EXPECT_EQ(rows[0], "row0");
  }

  // No error message expected
  EXPECT_TRUE(errorMsg == nullptr);

  // Cleanup errorMsg if allocated
  if(errorMsg) free(errorMsg);

  return (g_totalFail == 0) ? 0 : -1;
}

// Test 2: Callback aborts execution (non-zero return from callback)
int test_abort_on_callback() {
  g_stmtStates.clear();
  sqlite3 db;
  db.misuse = 0;
  db.flags = 0;
  db.errMask = 0;
  db.last_err = nullptr;

  std::vector<std::string> rows;
  bool firstRow = true;
  auto callback = [&rows, &firstRow](void* arg, int nCol, char** azVals, char** azCols) -> int {
    (void)arg; (void)azCols;
    if(firstRow) {
      // First row: push value and then abort on second call
      if(nCol > 0 && azVals && azVals[0]) rows.push_back(std::string(azVals[0]));
      firstRow = false;
      return 1; // abort on first invocation
    }
    return 0;
  };

  // Ensure the mock returns a row on first step
  const char* sql = "SELECT 1";
  char* errorMsg = nullptr;

  int rc = sqlite3_exec(&db, sql, callback, &rows, &errorMsg);

  EXPECT_EQ(rc, SQLITE_ABORT);
  // When abort occurs, an error message should be recorded
  EXPECT_TRUE(errorMsg != nullptr);
  if(errorMsg) {
    EXPECT_STR_EQ(errorMsg, "ABORT");
    free(errorMsg);
  }

  return (g_totalFail == 0) ? 0 : -1;
}

// Test 3: Whitespace-only SQL should trigger pStmt == 0 branch
int test_whitespace_pstmt_branch() {
  g_stmtStates.clear();
  sqlite3 db;
  db.misuse = 0;
  db.flags = 0;
  db.errMask = 0;
  db.last_err = nullptr;

  std::vector<std::string> rows;
  auto callback = [](void* arg, int nCol, char** azVals, char** azCols) -> int {
    (void)arg; (void)nCol; (void)azVals; (void)azCols;
    // Should not be called for whitespace-only input
    return 0;
  };

  const char* sql = "   "; // whitespace only
  char* errorMsg = nullptr;

  int rc = sqlite3_exec(&db, sql, callback, &rows, &errorMsg);

  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_TRUE(rows.empty());
  EXPECT_TRUE(errorMsg == nullptr);

  return (g_totalFail == 0) ? 0 : -1;
}

// Test 4: Safety check fails (misuse flagged)
int test_safety_check_missuse() {
  g_stmtStates.clear();
  sqlite3 db;
  db.misuse = 1; // trigger MISUSE
  db.flags = 0;
  db.errMask = 0;
  db.last_err = nullptr;

  std::vector<std::string> rows;
  auto callback = [](void* arg, int nCol, char** azVals, char** azCols) -> int {
    (void)arg; (void)nCol; (void)azVals; (void)azCols;
    return 0;
  };

  const char* sql = "SELECT 1";
  char* errorMsg = nullptr;

  int rc = sqlite3_exec(&db, sql, callback, &rows, &errorMsg);

  EXPECT_EQ(rc, SQLITE_MISUSE_BKPT);
  // When misuse, an error message may or may not be set depending on impl; our mock uses MISUSE mapping
  if(errorMsg) {
    // If an error message existed, verify it's a plausible text
    EXPECT_TRUE(std::string(errorMsg).size() > 0);
    free(errorMsg);
  }

  return (g_totalFail == 0) ? 0 : -1;
}

// Entry point for running all tests (no GTest, plain main)
int main() {
  LOG("Running sqlite3_exec test suite (mocked environment)");
  int r1 = test_basic_success();
  int r2 = test_abort_on_callback();
  int r3 = test_whitespace_pstmt_branch();
  int r4 = test_safety_check_missuse();

  if(g_totalFail == 0) {
    LOG("All tests PASSED");
  } else {
    LOG("Tests FAILED: " << g_totalFail << " failures, " << g_totalPass << " passes");
  }

  // Return non-zero if any test failed
  int overall = (g_totalFail == 0) ? 0 : -1;
  return overall;
}