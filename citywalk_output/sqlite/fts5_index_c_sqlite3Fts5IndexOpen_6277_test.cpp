// Unit test suite for sqlite3Fts5IndexOpen
// Language: C++11
// Notes:
// - This test suite uses simple C mock functions to intercept interactions with
//   underlying storage/memory allocation helpers used by sqlite3Fts5IndexOpen.
// - It assumes that the production code is built together with this test and that
//   the function prototypes for sqlite3Fts5MallocZero, sqlite3Fts5Mprintf,
//   sqlite3Fts5CreateTable, sqlite3Fts5IndexReinit, sqlite3Fts5IndexClose
//   are resolved by these mocks (link-time substitution).
// - We rely on the public API surface shown in the focal method and its dependencies.

#include <fts5Int.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Forward declare production types to align with fts5Int.h if available.
extern "C" {
}

// Constants used by the production code (mirroring sqlite status codes)
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_ERROR
#define SQLITE_ERROR 1
#endif

// Global test harness state (mocks)
static int g_forceBadMalloc     = 0; // If set, sqlite3Fts5MallocZero returns NULL and non-OK rc
static int g_forceBadMprintf    = 0; // If set, sqlite3Fts5Mprintf returns NULL and non-OK rc

// Create-table mock behavior
static int g_createTableCall   = 0;
static int g_createTableReturn[4] = { SQLITE_OK, SQLITE_OK, SQLITE_OK, SQLITE_OK };
static int g_dataCreateTableCalled = 0;
static int g_idxCreateTableCalled  = 0;

// Index reinit/close mocks
static int g_reinitCalled       = 0;
static int g_indexCloseCalled   = 0;

// Storage for last created table (data vs idx)
static char g_lastCreatedTable[16] = {0};

// Last error string (if applicable)
static char g_lastError[256] = {0};

// Helper: reset all mock state between tests
static void mockResetState() {
  g_forceBadMalloc = 0;
  g_forceBadMprintf = 0;
  g_createTableCall = 0;
  for (int i = 0; i < 4; i++) g_createTableReturn[i] = SQLITE_OK;
  g_dataCreateTableCalled = 0;
  g_idxCreateTableCalled = 0;
  g_reinitCalled = 0;
  g_indexCloseCalled = 0;
  std::memset(g_lastCreatedTable, 0, sizeof(g_lastCreatedTable));
  std::memset(g_lastError, 0, sizeof(g_lastError));
}

// Mock: sqlite3Fts5MallocZero
extern "C" void* sqlite3Fts5MallocZero(int *pRc, int nByte) {
  if (g_forceBadMalloc) {
    if (pRc) *pRc = SQLITE_ERROR;
    return NULL;
  }
  if (pRc) *pRc = SQLITE_OK;
  void *p = std::calloc(1, nByte);
  return p;
}

// Mock: sqlite3Fts5Mprintf
extern "C" char* sqlite3Fts5Mprintf(int *pRc, const char *zFormat, ...) {
  if (g_forceBadMprintf) {
    if (pRc) *pRc = SQLITE_ERROR;
    return NULL;
  }
  char buffer[1024];
  va_list ap;
  va_start(ap, zFormat);
  int n = vsnprintf(buffer, sizeof(buffer), zFormat, ap);
  va_end(ap);
  if (n < 0) {
    if (pRc) *pRc = SQLITE_ERROR;
    return NULL;
  }
  char *out = (char*)std::malloc(n + 1);
  if (!out) {
    if (pRc) *pRc = SQLITE_ERROR;
    return NULL;
  }
  std::memcpy(out, buffer, n + 1);
  if (pRc) *pRc = SQLITE_OK;
  return out;
}

// Mock: sqlite3Fts5CreateTable
extern "C" int sqlite3Fts5CreateTable(Fts5Config *pConfig, const char *zTable, const char *zSql, int bIfNotExists, char **pzErr) {
  // Track calls for coverage logic
  if (zTable && std::strcmp(zTable, "data") == 0) {
    g_dataCreateTableCalled++;
  } else if (zTable && std::strcmp(zTable, "idx") == 0) {
    g_idxCreateTableCalled++;
  }

  // Sequenced return values to simulate success/failure paths
  int ret = SQLITE_OK;
  if (g_createTableCall < 4) {
    ret = g_createTableReturn[g_createTableCall];
    g_createTableCall++;
  } else {
    ret = SQLITE_OK;
  }

  // Record last created table for diagnostics
  if (zTable) {
    std::strncpy(g_lastCreatedTable, zTable, sizeof(g_lastCreatedTable) - 1);
    g_lastCreatedTable[sizeof(g_lastCreatedTable) - 1] = '\0';
  } else {
    std::strcpy(g_lastCreatedTable, "");
  }

  return ret;
}

// Mock: sqlite3Fts5IndexReinit
extern "C" int sqlite3Fts5IndexReinit(Fts5Index *p) {
  g_reinitCalled = 1;
  return SQLITE_OK;
}

// Mock: sqlite3Fts5IndexClose
extern "C" void sqlite3Fts5IndexClose(Fts5Index *p) {
  g_indexCloseCalled = 1;
  // Free the object if allocated
  if (p) {
    // Best-effort clean up; the real implementation may do more,
    // but for unit tests we can simply free the memory block if present.
    std::free(p);
  }
}

// Simple test assertion helpers
static bool assertTrue(bool cond, const char *msg) {
  if (!cond) {
    std::cerr << "ASSERT FAILED: " << msg << std::endl;
    return false;
  }
  return true;
}

// Test 1: Open without creating on a clean config
static bool test_OpenWithoutCreate() {
  mockResetState();

  Fts5Config cfg;
  std::memset(&cfg, 0, sizeof(cfg));
  // Some headers define zName as char*; ensure a valid string
  cfg.zName = "testdb";

  Fts5Index *pOut = nullptr;
  char *pzErr = nullptr;

  int rc = sqlite3Fts5IndexOpen(&cfg, 0, &pOut, &pzErr);

  bool ok = true;
  ok &= assertTrue(rc == SQLITE_OK, "Open without create should return SQLITE_OK");
  ok &= assertTrue(pOut != nullptr, "Returned pointer should be non-null");
  if (pOut) {
    ok &= assertTrue(pOut->pConfig == &cfg, "pConfig should be preserved inside new index");
    ok &= assertTrue(pOut->nWorkUnit == FTS5_WORK_UNIT, "nWorkUnit should be initialized to FTS5_WORK_UNIT");
    // zDataTbl should be "testdb_data" due to formatting
    ok &= assertTrue(pOut->zDataTbl != nullptr, "zDataTbl should be non-null when formatting succeeds");
    if (pOut->zDataTbl) {
      ok &= assertTrue(std::strcmp(pOut->zDataTbl, "testdb_data") == 0, "zDataTbl should be 'testdb_data'");
    }
  }

  // Verify that no create table calls happened
  ok &= assertTrue(g_dataCreateTableCalled == 0, "Data table should not be created when bCreate == 0");
  ok &= assertTrue(g_idxCreateTableCalled == 0, "Idx table should not be created when bCreate == 0");
  ok &= assertTrue(g_reinitCalled == 0, "IndexReinit should not be called when bCreate == 0");

  // Cleanup
  if (pOut) sqlite3Fts5IndexClose(pOut);
  // pOut should be null after close, but our close call will free; ensure no crash
  return ok;
}

// Test 2: Open with create path and successful table creation
static bool test_OpenWithCreate_Success() {
  mockResetState();
  // Two calls to sqlite3Fts5CreateTable for "data" and "idx"
  g_createTableReturn[0] = SQLITE_OK;
  g_createTableReturn[1] = SQLITE_OK;
  g_createTableCall = 0;

  Fts5Config cfg;
  std::memset(&cfg, 0, sizeof(cfg));
  cfg.zName = "ftstest";

  Fts5Index *pOut = nullptr;
  char *pzErr = nullptr;

  int rc = sqlite3Fts5IndexOpen(&cfg, 1, &pOut, &pzErr);

  bool ok = true;
  ok &= assertTrue(rc == SQLITE_OK, "Open with create path should return SQLITE_OK when all operations succeed");
  ok &= assertTrue(pOut != nullptr, "Returned pointer should be non-null on success");
  if (pOut) {
    ok &= assertTrue(pOut->pConfig == &cfg, "pConfig should be preserved inside new index");
    ok &= assertTrue(pOut->zDataTbl != nullptr, "zDataTbl should be initialized when bCreate is true");
    if (pOut->zDataTbl) {
      ok &= assertTrue(std::strcmp(pOut->zDataTbl, "ftstest_data") == 0, "zDataTbl should be 'ftstest_data'");
    }
  }

  // Verify create table calls count
  ok &= assertTrue(g_dataCreateTableCalled == 1, "Data table create should be called once");
  ok &= assertTrue(g_idxCreateTableCalled == 1, "Idx table create should be called once");
  ok &= assertTrue(g_reinitCalled == 1, "IndexReinit should be called after creation");
  ok &= assertTrue(g_indexCloseCalled == 0, "IndexClose should not be called on success");

  // Cleanup
  if (pOut) sqlite3Fts5IndexClose(pOut);
  return ok;
}

// Test 3: Open with create path and first CreateTable fails
static bool test_OpenWithCreate_FailureFirstTable() {
  mockResetState();
  // First call returns error
  g_createTableReturn[0] = SQLITE_ERROR;
  g_createTableReturn[1] = SQLITE_OK;
  g_createTableCall = 0;

  Fts5Config cfg;
  std::memset(&cfg, 0, sizeof(cfg));
  cfg.zName = "faildb";

  Fts5Index *pOut = nullptr;
  char *pzErr = nullptr;

  int rc = sqlite3Fts5IndexOpen(&cfg, 1, &pOut, &pzErr);

  bool ok = true;
  ok &= assertTrue(rc != SQLITE_OK, "Open should fail when first CreateTable fails");
  ok &= assertTrue(pOut == nullptr || pOut == 0, "On failure, *pp should be reset to 0/null");
  ok &= assertTrue(g_indexCloseCalled == 1, "IndexClose should be called to clean up on failure");

  // Cleanup (in case pOut non-null due to partial allocation)
  if (pOut) sqlite3Fts5IndexClose(pOut);
  return ok;
}

// Test 4: Malloc failure path
static bool test_OpenMallocFailure() {
  mockResetState();
  g_forceBadMalloc = 1; // Force non-OK rc from allocation

  Fts5Config cfg;
  std::memset(&cfg, 0, sizeof(cfg));
  cfg.zName = "nomem";

  Fts5Index *pOut = nullptr;
  char *pzErr = nullptr;

  int rc = sqlite3Fts5IndexOpen(&cfg, 0, &pOut, &pzErr);

  bool ok = true;
  ok &= assertTrue(rc != SQLITE_OK, "Open should fail when allocation fails");
  ok &= assertTrue(pOut == nullptr || pOut == 0, "On allocation failure, *pp should be 0/NULL");
  ok &= assertTrue(g_indexCloseCalled == 1, "IndexClose should be called to clean up on failure (NULL pointer safe)");

  // Cleanup
  if (pOut) sqlite3Fts5IndexClose(pOut);
  return ok;
}

int main() {
  bool all = true;

  std::cout << "Running sqlite3Fts5IndexOpen unit tests (C/C++11, no GTest)\n";

  all &= test_OpenWithoutCreate();
  std::cout << "Test 1: OpenWithoutCreate " << (all ? "PASSED" : "FAILED") << "\n";

  all = true;
  all &= test_OpenWithCreate_Success();
  std::cout << "Test 2: OpenWithCreate_Success " << (all ? "PASSED" : "FAILED") << "\n";

  all = true;
  all &= test_OpenWithCreate_FailureFirstTable();
  std::cout << "Test 3: OpenWithCreate_FailureFirstTable " << (all ? "PASSED" : "FAILED") << "\n";

  all = true;
  all &= test_OpenMallocFailure();
  std::cout << "Test 4: OpenMallocFailure " << (all ? "PASSED" : "FAILED") << "\n";

  std::cout << "Unit tests completed.\n";
  return 0;
}