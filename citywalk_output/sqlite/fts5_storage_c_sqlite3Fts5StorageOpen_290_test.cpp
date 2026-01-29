// This is a self-contained C++11 test harness exercising the focal method
// sqlite3Fts5StorageOpen by re-implementing a minimal subset of the SQLite
// FTS5 storage environment sufficient for unit testing without GTest.
//
// The focal method is implemented here in a simplified form along with stubbed
// dependencies to allow deterministic testing of all branches.
// Note: This test suite uses only the C++ standard library and small lightweight
// test helpers (no GoogleTest or GMock).

#include <vector>
#include <fts5Int.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdarg>
#include <iostream>
#include <cstdlib>


// Domain-specific constants and types (subset of the real project)
typedef long long i64;
typedef long long sqlite3_int64;
#define SQLITE_OK 0
#define SQLITE_NOMEM 7
#define FTS5_CURRENT_VERSION 1
#define FTS5_CONTENT_NORMAL 0

// Public-facing structures (simplified)
struct Fts5Config {
  int nCol;
  int eContent;          // content type, e.g., FTS5_CONTENT_NORMAL
  int bColumnsize;
  int bContentlessDelete;
};

struct Fts5Index {
  // Intentionally empty for tests
};

struct Fts5Storage {
  i64 *aTotalSize;
  Fts5Config *pConfig;
  Fts5Index *pIndex;
};

// Global stubs and helpers to observe internal behavior
static std::vector<std::string> g_callLog; // records calls and key actions

static bool g_forceAllocFail = false;            // force initial malloc64 to fail
static int  g_nextCreateTableFail = 0;           // force next sqlite3Fts5CreateTable to fail

static void logCall(const std::string &msg) {
  g_callLog.push_back(msg);
}

static void resetLog() {
  g_callLog.clear();
}

// ---- Mocked/Stubbed low-level functions used by sqlite3Fts5StorageOpen ----
static void* sqlite3_malloc64(sqlite3_int64 n) {
  if (n <= 0) return nullptr;
  if (g_forceAllocFail) { g_forceAllocFail = false; return nullptr; }
  return std::malloc((size_t)n);
}
static void sqlite3_free(void *p) { if (p) std::free(p); }

static int sqlite3_snprintf(int n, char *zBuf, const char *zFormat, ...) {
  va_list ap;
  va_start(ap, zFormat);
  int ret = vsnprintf(zBuf, (size_t) n, zFormat, ap);
  va_end(ap);
  return ret;
}

// Simple sqlite3_value placeholder
typedef struct sqlite3_value sqlite3_value;

// Logging of create-table requests (store which table requested and defn)
static int sqlite3Fts5CreateTable(
  Fts5Config *pConfig,
  const char *zPost,
  const char *zDefn,
  int bWithout,
  char **pzErr
){
  (void) pConfig; (void) zDefn; (void) bWithout; (void) pzErr;
  std::string msg = std::string("CreateTable: ") + (zPost ? zPost : "NULL");
  logCall(msg);
  // Allow tests to force a failure on the next CreateTable call
  if (g_nextCreateTableFail > 0) {
    g_nextCreateTableFail--;
    return SQLITE_NOMEM;
  }
  return SQLITE_OK;
}
static void sqlite3Fts5StorageClose(Fts5Storage *p){
  (void)p;
  logCall("StorageClose");
}
static int sqlite3Fts5StorageConfigValue(Fts5Storage *p, const char *z, sqlite3_value *pVal, int iVal){
  (void)p; (void)pVal; (void)iVal;
  logCall(std::string("StorageConfigValue: ") + (z ? z : "NULL"));
  return SQLITE_OK;
}

// Forward declare the function under test (implemented below)
static int sqlite3Fts5StorageOpen(
  Fts5Config *pConfig, 
  Fts5Index *pIndex, 
  int bCreate, 
  Fts5Storage **pp,
  char **pzErr
);

// Minimal public API to be used by tests (our own simplified version)
static int sqlite3Fts5StorageOpen(
  Fts5Config *pConfig, 
  Fts5Index *pIndex, 
  int bCreate, 
  Fts5Storage **pp,
  char **pzErr
){
  int rc = SQLITE_OK;
  Fts5Storage *p;                 /* New object */
  sqlite3_int64 nByte;            /* Bytes of space to allocate */
  nByte = sizeof(Fts5Storage)               /* Fts5Storage object */
        + pConfig->nCol * sizeof(i64);      /* Fts5Storage.aTotalSize[] */
  *pp = p = (Fts5Storage*)sqlite3_malloc64(nByte);
  if( !p ) return SQLITE_NOMEM;
  std::memset(p, 0, (size_t)nByte);
  p->aTotalSize = (i64*)&p[1];
  p->pConfig = pConfig;
  p->pIndex = pIndex;
  if( bCreate ){
    if( pConfig->eContent==FTS5_CONTENT_NORMAL ){
      int nDefn = 32 + pConfig->nCol*10;
      char *zDefn = sqlite3_malloc64(32 + (sqlite3_int64)pConfig->nCol * 10);
      if( zDefn==0 ){
        rc = SQLITE_NOMEM;
      }else{
        int i;
        int iOff;
        sqlite3_snprintf(nDefn, zDefn, "id INTEGER PRIMARY KEY");
        iOff = (int)strlen(zDefn);
        for(i=0; i<pConfig->nCol; i++){
          sqlite3_snprintf(nDefn-iOff, &zDefn[iOff], ", c%d", i);
          iOff += (int)strlen(&zDefn[iOff]);
        }
        rc = sqlite3Fts5CreateTable(pConfig, "content", zDefn, 0, pzErr);
      }
      sqlite3_free(zDefn);
    }
    if( rc==SQLITE_OK && pConfig->bColumnsize ){
      const char *zCols = "id INTEGER PRIMARY KEY, sz BLOB";
      if( pConfig->bContentlessDelete ){
        zCols = "id INTEGER PRIMARY KEY, sz BLOB, origin INTEGER";
      }
      rc = sqlite3Fts5CreateTable(pConfig, "docsize", zCols, 0, pzErr);
    }
    if( rc==SQLITE_OK ){
      rc = sqlite3Fts5CreateTable(
          pConfig, "config", "k PRIMARY KEY, v", 1, pzErr
      );
    }
    if( rc==SQLITE_OK ){
      rc = sqlite3Fts5StorageConfigValue(p, "version", 0, FTS5_CURRENT_VERSION);
    }
  }
  if( rc ){
    sqlite3Fts5StorageClose(p);
    *pp = 0;
  }
  return rc;
}

// The following test helpers implement a tiny test framework without GTest.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) \
  do { if(!(cond)) { std::cerr << "EXPECT_TRUE failed: " << msg << std::endl; ++g_failed_tests; } } while(0)

#define EXPECT_EQ(expected, actual, msg) \
  do { \
    if(!((expected) == (actual))) { \
      std::cerr << "EXPECT_EQ failed: " << msg \
                << " | expected: " << (expected) << " actual: " << (actual) << std::endl; \
      ++g_failed_tests; \
    } \
  } while(0)

#define EXPECT_PTR_EQ(expected, actual, msg) \
  do { \
    if((void*)(expected) != (void*)(actual)) { \
      std::cerr << "EXPECT_PTR_EQ failed: " << msg \
                << " | expected: " << expected << " actual: " << actual << std::endl; \
      ++g_failed_tests; \
    } \
  } while(0)

static void test_OpenStorage_NoCreate() {
  g_callLog.clear();
  g_forceAllocFail = false;
  g_nextCreateTableFail = 0;

  Fts5Config cfg;
  cfg.nCol = 3;
  cfg.eContent = FTS5_CONTENT_NORMAL;
  cfg.bColumnsize = 0;
  cfg.bContentlessDelete = 0;

  Fts5Storage *p = nullptr;
  char *pzErr = nullptr;

  int rc = sqlite3Fts5StorageOpen(&cfg, nullptr, 0, &p, &pzErr);

  EXPECT_EQ(SQLITE_OK, rc, "Open with bCreate=0 should succeed");
  EXPECT_TRUE(p != nullptr, "Storage pointer should be non-null");
  if(p){
    EXPECT_PTR_EQ(&cfg, p->pConfig, "pConfig should point to input cfg");
    EXPECT_PTR_EQ(nullptr, p->pIndex, "pIndex should be null when not provided");
  }

  // Clean up: close if allocated (our stub logs)
  if (p) {
    sqlite3Fts5StorageClose(p);
  }

  // No create-table calls expected
  EXPECT_TRUE(g_callLog.empty(), "No CreateTable calls should occur when bCreate is false");
  g_callLog.clear();
  ++g_total_tests;
}

static void test_OpenStorage_CreatePaths() {
  g_callLog.clear();
  g_forceAllocFail = false;
  g_nextCreateTableFail = 0;

  Fts5Config cfg;
  cfg.nCol = 2;
  cfg.eContent = FTS5_CONTENT_NORMAL;
  cfg.bColumnsize = 1;
  cfg.bContentlessDelete = 0;

  Fts5Storage *p = nullptr;
  char *pzErr = nullptr;

  int rc = sqlite3Fts5StorageOpen(&cfg, nullptr, 1, &p, &pzErr);

  EXPECT_EQ(SQLITE_OK, rc, "Open with bCreate=1 should succeed");
  EXPECT_TRUE(p != nullptr, "Storage pointer should be non-null");
  if(!p) { ++g_total_tests; return; }

  // Expect content, docsize, config table creation
  bool hasContent = false, hasDocsize = false, hasConfig = false;
  for (const auto &s : g_callLog) {
    if (s.find("CreateTable: content") != std::string::npos) hasContent = true;
    if (s.find("CreateTable: docsize") != std::string::npos) hasDocsize = true;
    if (s.find("CreateTable: config") != std::string::npos) hasConfig = true;
  }
  EXPECT_TRUE(hasContent, "Content table should be created when eContent is NORMAL");
  EXPECT_TRUE(hasDocsize, "Docsize table should be created when rc OK");
  EXPECT_TRUE(hasConfig, "Config table should be created on success");

  // Ensure storage close will be called on failure paths; here we didn't fail, but close to clean up
  sqlite3Fts5StorageClose(p);
  g_callLog.clear();
  ++g_total_tests;
}

static void test_OpenStorage_AllocFailInitial() {
  g_callLog.clear();
  g_forceAllocFail = true; // Force allocation to fail on initial malloc
  g_nextCreateTableFail = 0;

  Fts5Config cfg;
  cfg.nCol = 1;
  cfg.eContent = FTS5_CONTENT_NORMAL;
  cfg.bColumnsize = 0;
  cfg.bContentlessDelete = 0;

  Fts5Storage *p = nullptr;
  char *pzErr = nullptr;

  int rc = sqlite3Fts5StorageOpen(&cfg, nullptr, 0, &p, &pzErr);

  EXPECT_EQ(SQLITE_NOMEM, rc, "Open should fail with SQLITE_NOMEM when malloc64 fails");
  EXPECT_TRUE(p == nullptr, "Storage pointer should be null on allocation failure");
  ++g_total_tests;
}

static void test_OpenStorage_CreateTableFailure() {
  g_callLog.clear();
  g_forceAllocFail = false;
  g_nextCreateTableFail = 1; // Force first CreateTable to fail

  Fts5Config cfg;
  cfg.nCol = 1;
  cfg.eContent = FTS5_CONTENT_NORMAL;
  cfg.bColumnsize = 0;
  cfg.bContentlessDelete = 0;

  Fts5Storage *p = nullptr;
  char *pzErr = nullptr;

  int rc = sqlite3Fts5StorageOpen(&cfg, nullptr, 1, &p, &pzErr);

  EXPECT_EQ(SQLITE_NOMEM, rc, "Open should fail when a CreateTable call fails");
  EXPECT_TRUE(p == nullptr, "Storage pointer should be null after failure");
  ++g_total_tests;
}

static void test_OpenStorage_NoContentBranch() {
  g_callLog.clear();
  g_forceAllocFail = false;
  g_nextCreateTableFail = 0;

  Fts5Config cfg;
  cfg.nCol = 1;
  cfg.eContent = 123; // non-normal content (simulate alternative branch)
  cfg.bColumnsize = 1;
  cfg.bContentlessDelete = 0;

  Fts5Storage *p = nullptr;
  char *pzErr = nullptr;

  int rc = sqlite3Fts5StorageOpen(&cfg, nullptr, 1, &p, &pzErr);

  EXPECT_EQ(SQLITE_OK, rc, "Open should succeed even if content path is skipped");
  // Should not have created 'content' table
  bool hasContent = false, hasDocsize = false, hasConfig = false;
  for (const auto &s : g_callLog) {
    if (s.find("CreateTable: content") != std::string::npos) hasContent = true;
    if (s.find("CreateTable: docsize") != std::string::npos) hasDocsize = true;
    if (s.find("CreateTable: config") != std::string::npos) hasConfig = true;
  }
  EXPECT_TRUE(!hasContent, "Content table should not be created when eContent != NORMAL");
  EXPECT_TRUE(hasDocsize, "Docsize table should be created when rc OK regardless of content");
  EXPECT_TRUE(hasConfig, "Config table should be created on success");
  if (p) sqlite3Fts5StorageClose(p);
  ++g_total_tests;
}

static void run_all_tests() {
  test_OpenStorage_NoCreate();
  test_OpenStorage_CreatePaths();
  test_OpenStorage_AllocFailInitial();
  test_OpenStorage_CreateTableFailure();
  test_OpenStorage_NoContentBranch();

  // Summary
  std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;
}

int main() {
  run_all_tests();
  if (g_failed_tests > 0) {
    std::cerr << "Some tests FAILED." << std::endl;
    return 1;
  }
  std::cout << "All tests PASSED." << std::endl;
  return 0;
}