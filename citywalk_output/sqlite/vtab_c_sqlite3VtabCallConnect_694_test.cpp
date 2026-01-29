// Test suite for the focal method sqlite3VtabCallConnect (from vtab.c)
// This test suite is self-contained, uses a lightweight custom testing framework,
// and does not rely on GoogleTest or any external testing library.
// It exercises true/false branches of the method under different mocked dependencies.

#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cstdarg>
#include <cstdlib>
#include <utility>
#include <cassert>


// Domain constants (subset of SQLite)
static const int SQLITE_OK = 0;
static const int SQLITE_ERROR = 1;

// Forward declarations to mirror the focal method dependencies
struct sqlite3;
struct sqlite3_module;
struct sqlite3_vtab;
struct VTable;
struct Parse;
struct Table;

// Lightweight VTable placeholder
struct VTable {};

// Minimal representation of a Module with a pointer to sqlite3_module
struct sqlite3_module {
  int (*xConnect)(sqlite3*, void*, int, const char* const*, sqlite3_vtab**, char**);
};

// Module wrapper used by tests
struct Module {
  sqlite3_module* pModule;
};

// Hash-like storage of modules inside sqlite3 (very small test helper)
struct sqlite3_hash {
  std::vector<std::pair<std::string, Module*>> aModule;
};

// Core sqlite3-like database object used in tests
struct sqlite3 {
  sqlite3_hash aModule;
};

// Parse structure used by the focal function
struct Parse {
  sqlite3* db;       // database handle
  int rc;              // return code to propagate errors
  char* lastError;     // captured error string for assertions
};

// Table and virtual table plumbing used by sqlite3VtabCallConnect
struct Table {
  // emulate the union: pTab->u.vtab.azArg[0]
  struct {
    const char* azArg[1];
  } vtab;
  bool isVirtual;
  bool existingVTab; // indicates whether sqlite3GetVTable should return non-null
};

// Helper to expose virtual-table state (mimics original IsVirtual macro)
static bool IsVirtual(const Table* pTab) {
  return pTab ? pTab->isVirtual : false;
}

// Emulated VTable access function used by sqlite3VtabCallConnect
static VTable* sqlite3GetVTable(sqlite3* db, Table* pTab) {
  (void)db; // unused in tests
  if (!pTab) return nullptr;
  if (pTab->existingVTab) return (VTable*)1; // non-null sentinel
  return nullptr;
}

// Module hash lookup (very small, linear search)
static Module* sqlite3HashFind(sqlite3_hash* pHash, const char* zMod) {
  if(!pHash || !zMod) return nullptr;
  for (auto &entry : pHash->aModule) {
    if (entry.first && zMod && std::strcmp(entry.first.c_str(), zMod) == 0) {
      return entry.second;
    }
  }
  return nullptr;
}

// Insert/module registration helper for tests
static void sqlite3HashInsert(sqlite3_hash* pHash, const char* zName, Module* pMod) {
  if(!pHash || !zName || !pMod) return;
  pHash->aModule.push_back({std::string(zName), pMod});
}

// Error reporting utilities
static void sqlite3ErrorMsg(Parse* pParse, const char* zFormat, ...) {
  if (!pParse) return;
  va_list ap;
  va_start(ap, zFormat);
  // Collect formatted message into a buffer
  char buf[1024];
  vsnprintf(buf, sizeof(buf), zFormat, ap);
  va_end(ap);
  // Free previous error if any
  if (pParse->lastError) {
    free(pParse->lastError);
    pParse->lastError = nullptr;
  }
  // Duplicate into parse structure
  pParse->lastError = strdup(buf);
}

static void sqlite3DbFree(sqlite3* db, void* p) {
  (void)db;
  free(p);
}

// Static forward declaration of focal helper used in sqlite3VtabCallConnect
static int vtabCallConstructor(
  sqlite3 *db,
  Table *pTab,
  Module *pMod,
  int (*xConstruct)(sqlite3*,void*,int,const char*const*,sqlite3_vtab**,char**),
  char **pzErr
) {
  // In tests, if a module exists, invoke its xConnect wrapper
  if (!pMod || !pMod->pModule) return SQLITE_OK;
  if (xConstruct) {
    return xConstruct(db, (void*)pTab, 0, nullptr, nullptr, pzErr);
  }
  return SQLITE_OK;
}

// Focal method under test: sqlite3VtabCallConnect
static int sqlite3VtabCallConnect(Parse *pParse, Table *pTab) {
  sqlite3 *db = pParse->db;
  const char *zMod;
  Module *pMod;
  int rc;
  // Basic sanity checks
  if (!pTab) return SQLITE_ERROR;
  // In real code this asserts; keep behavior identical for test
  // assert( pTab );
  // assert( IsVirtual(pTab) );

  // If the virtual table already has an associated VTable, return OK
  if( sqlite3GetVTable(db, pTab) ){
    return SQLITE_OK;
  }

  // Locate the required virtual table module
  zMod = pTab->vtab.azArg[0];
  pMod = (Module*)sqlite3HashFind(&db->aModule, zMod);
  if( !pMod ){
    const char *zModule = pTab->vtab.azArg[0];
    sqlite3ErrorMsg(pParse, "no such module: %s", zModule);
    rc = SQLITE_ERROR;
  }else{
    char *zErr = 0;
    rc = vtabCallConstructor(db, pTab, pMod, pMod->pModule->xConnect, &zErr);
    if( rc!=SQLITE_OK ){
      sqlite3ErrorMsg(pParse, "%s", zErr);
      pParse->rc = rc;
    }
    sqlite3DbFree(db, zErr);
  }
  return rc;
}

// --- Test helpers: mocked module xConnect implementations ---

// xConnect that succeeds
static int test_xconnect_ok(sqlite3* db, void* pAux, int argc,
                          const char* const* argv, sqlite3_vtab** ppVTab, char** pzErr) {
  (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVTab;
  if (pzErr) *pzErr = nullptr;
  return SQLITE_OK;
}

// xConnect that fails and returns an error message
static int test_xconnect_error(sqlite3* db, void* pAux, int argc,
                             const char* const* argv, sqlite3_vtab** ppVTab, char** pzErr) {
  (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVTab;
  if (pzErr) *pzErr = strdup("connect failed");
  return SQLITE_ERROR;
}

// --- Lightweight test framework (non-terminating expectations) ---

static int tests_run = 0;
static int tests_failed = 0;
#define EXPECT_TRUE(cond) \
  do { \
    ++tests_run; \
    if(!(cond)) { \
      fprintf(stderr, "EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
      ++tests_failed; \
    } \
  } while(0)

#define EXPECT_FALSE(cond) EXPECT_TRUE(!(cond))
#define EXPECT_EQ(a,b) \
  do { ++tests_run; if(!((a)==(b))){ fprintf(stderr,"EXPECT_EQ failed: %s == %s (actual: %ld != %ld) at %s:%d\n", #a, #b, (long)(a), (long)(b), __FILE__, __LINE__); ++tests_failed; } } while(0)

#define TEST_FUNC(name) void name()

// --- Test cases ---

// Test 1: When a VTable already exists, sqlite3VtabCallConnect should return SQLITE_OK
TEST_FUNC(test_existing_vtab_returns_ok) {
  // Setup
  sqlite3 db;
  db.aModule.aModule.clear(); // ensure empty

  Parse pParse;
  pParse.db = &db;
  pParse.rc = 0;
  pParse.lastError = nullptr;

  Table tab;
  tab.isVirtual = true;
  tab.existingVTab = true;
  tab.vtab.azArg[0] = "any_module";

  // Execute
  int rc = sqlite3VtabCallConnect(&pParse, &tab);

  // Verify
  EXPECT_EQ(rc, SQLITE_OK);
  // Ensure no error message was produced
  EXPECT_TRUE(pParse.lastError == nullptr);
}

// Test 2: Module exists and xConnect returns SQLITE_OK
TEST_FUNC(test_module_found_connect_ok) {
  // Setup
  static Module mOk;
  static sqlite3_module modOk;
  modOk.xConnect = test_xconnect_ok;
  mOk.pModule = &modOk;

  static sqlite3 db;
  db.aModule.aModule.clear();
  sqlite3HashInsert(&db.aModule, "modOk", &mOk);

  Parse pParse;
  pParse.db = &db;
  pParse.rc = 0;
  pParse.lastError = nullptr;

  Table tab;
  tab.isVirtual = true;
  tab.existingVTab = false; // ensure we go through module path
  tab.vtab.azArg[0] = "modOk";

  // Execute
  int rc = sqlite3VtabCallConnect(&pParse, &tab);

  // Verify
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_TRUE(pParse.lastError == nullptr);
}

// Test 3: Module exists but xConnect returns error and sets zErr
TEST_FUNC(test_module_connect_error_propagates_error) {
  // Setup
  static Module mError;
  static sqlite3_module modError;
  modError.xConnect = test_xconnect_error;
  mError.pModule = &modError;

  static sqlite3 db;
  db.aModule.aModule.clear();
  sqlite3HashInsert(&db.aModule, "modError", &mError);

  Parse pParse;
  pParse.db = &db;
  pParse.rc = 0;
  pParse.lastError = nullptr;

  Table tab;
  tab.isVirtual = true;
  tab.existingVTab = false;
  tab.vtab.azArg[0] = "modError";

  // Execute
  int rc = sqlite3VtabCallConnect(&pParse, &tab);

  // Verify
  EXPECT_EQ(rc, SQLITE_ERROR);
  EXPECT_TRUE(pParse.lastError != nullptr);
  EXPECT_TRUE(std::strcmp(pParse.lastError, "connect failed") == 0);
}

// Test 4: No such module -> error path with specific message
TEST_FUNC(test_no_such_module_errors) {
  // Setup
  static sqlite3 db;
  db.aModule.aModule.clear();

  Parse pParse;
  pParse.db = &db;
  pParse.rc = 0;
  pParse.lastError = nullptr;

  Table tab;
  tab.isVirtual = true;
  tab.existingVTab = false;
  tab.vtab.azArg[0] = "missingModule";

  // Execute
  int rc = sqlite3VtabCallConnect(&pParse, &tab);

  // Verify
  EXPECT_EQ(rc, SQLITE_ERROR);
  EXPECT_TRUE(pParse.lastError != nullptr);
  // expect formatted error
  EXPECT_TRUE(std::strcmp(pParse.lastError, "no such module: missingModule") == 0);
}

// Helper to run all tests
static void run_all_tests() {
  test_existing_vtab_returns_ok();
  test_module_found_connect_ok();
  test_module_connect_error_propagates_error();
  test_no_such_module_errors();
}

// Entry point
int main() {
  run_all_tests();
  fprintf(stdout, "Tests run: %d, Failures: %d\n", tests_run, tests_failed);
  return (tests_failed ? 1 : 0);
}