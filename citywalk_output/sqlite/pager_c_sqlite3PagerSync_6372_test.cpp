// Unit tests for the focal method: sqlite3PagerSync
// This test suite provides a minimal in-memory environment to exercise the behavior
// of sqlite3PagerSync by mocking out the underlying OS/file operations.
// The tests are implemented in C++11 without using GTest (as requested).

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <wal.h>
#include <cassert>


// Domain-specific constants (mocked/stubbed for unit tests)
#define SQLITE_OK 0
#define SQLITE_NOTFOUND 1
#define SQLITE_ERROR 2
#define SQLITE_BUSY 3

// Fcntl/sync constants (stubbed)
#define SQLITE_FCNTL_SYNC 0

// MEMDB flag (static assertion in sqlite3PagerSync)
#define MEMDB 0

// ---------------------------------------------------------------------------
// Minimal dependencies and types required by sqlite3PagerSync (mocked)
// ---------------------------------------------------------------------------

// Pager structure (minimal subset used by sqlite3PagerSync)
struct Pager {
  int fd;
  int noSync;
  int syncFlags;
};

// Forward declarations of the mocked OS helpers (C linkage to mimic sqlite3 APIs)
extern "C" int sqlite3OsFileControl(int fd, int op, void* pArg);
extern "C" int sqlite3OsSync(int fd, int flags);

// Global state used by the mocks to control behavior and observe calls
static int g_fileControlReturn = SQLITE_OK;
static void* g_seenPArg = nullptr;
static int g_osSyncCalled = 0;
static int g_osSyncReturn = SQLITE_OK;

// zSuper tracking to verify the parameter passed through
static const char* g_expectedZSuper = nullptr;

// Helper to reset mocks between tests
static void resetMocks() {
  g_fileControlReturn = SQLITE_OK;
  g_seenPArg = nullptr;
  g_osSyncCalled = 0;
  g_osSyncReturn = SQLITE_OK;
  g_expectedZSuper = nullptr;
}

// Mock implementations of the OS-layer calls (appear as C functions to match the original code)
extern "C" int sqlite3OsFileControl(int fd, int op, void* pArg) {
  // Record the argument pointer to verify that zSuper is passed through correctly
  g_seenPArg = pArg;
  return g_fileControlReturn;
}

extern "C" int sqlite3OsSync(int fd, int flags) {
  ++g_osSyncCalled;
  return g_osSyncReturn;
}

// ---------------------------------------------------------------------------
// FOCAL_METHOD under test (reproduced in this test file for isolation)
// int sqlite3PagerSync(Pager *pPager, const char *zSuper){
//
//   int rc = SQLITE_OK;
//   void *pArg = (void*)zSuper;
//   rc = sqlite3OsFileControl(pPager->fd, SQLITE_FCNTL_SYNC, pArg);
//   if( rc==SQLITE_NOTFOUND ) rc = SQLITE_OK;
//   if( rc==SQLITE_OK && !pPager->noSync ){
//     assert( !MEMDB );
//     rc = sqlite3OsSync(pPager->fd, pPager->syncFlags);
//   }
//   return rc;
// }
// ---------------------------------------------------------------------------

int sqlite3PagerSync(Pager *pPager, const char *zSuper) {
  int rc = SQLITE_OK;
  void *pArg = (void*)zSuper;
  rc = sqlite3OsFileControl(pPager->fd, SQLITE_FCNTL_SYNC, pArg);
  if( rc==SQLITE_NOTFOUND ) rc = SQLITE_OK;
  if( rc==SQLITE_OK && !pPager->noSync ){
    assert( !MEMDB );
    rc = sqlite3OsSync(pPager->fd, pPager->syncFlags);
  }
  return rc;
}

// ---------------------------------------------------------------------------
// Lightweight test framework helpers (no external libs)
// ---------------------------------------------------------------------------

static void printHeader(const char* name) {
  std::cout << "=== " << name << " ===" << std::endl;
}

// Per-test pass/fail indicator
static bool g_currentTestPass = true;

// Simple assertion macro that reports failure and marks the current test as failed
#define TEST_ASSERT(cond, msg) \
  do { if(!(cond)) { \
    std::cerr << "Test " << __func__ << " failed: " << (msg) << std::endl; \
    g_currentTestPass = false; \
  } } while(0)


// ---------------------------------------------------------------------------
// Individual unit tests
// Each test follows:
// - Configure mocks
// - Call sqlite3PagerSync with a Pager instance and a zSuper
// - Assert on return code and on whether OsFileControl/OsSync were invoked
// - Verify that the zSuper parameter was passed through correctly
// - Print a short PASS/FAIL summary
// ---------------------------------------------------------------------------

// Test 1: When sqlite3OsFileControl returns SQLITE_NOTFOUND, the function should treat it as OK
// and, if permitted (noSync == 0), call sqlite3OsSync and return SQLITE_OK.
void test_NotFound_Is_Handled_As_OK_With_Sync()
{
  printHeader("Test 1: NOTFOUND -> OK and Sync Called");

  resetMocks();
  g_expectedZSuper = "Test1_Super";
  // Set mocks
  g_fileControlReturn = SQLITE_NOTFOUND;
  g_osSyncReturn = SQLITE_OK;

  Pager p;
  p.fd = 123;
  p.noSync = 0;
  p.syncFlags = 0;

  int rc = sqlite3PagerSync(&p, g_expectedZSuper);

  // Expectations
  TEST_ASSERT(rc == SQLITE_OK, "rc should be SQLITE_OK when NOTFOUND -> OK");
  TEST_ASSERT(g_osSyncCalled == 1, "OsSync should be called once after NOTFOUND");
  TEST_ASSERT(g_seenPArg == (void*)g_expectedZSuper, "zSuper should be passed to OsFileControl");

  if (g_currentTestPass) {
    std::cout << "PASS: " << __func__ << std::endl;
  } else {
    std::cout << "FAIL: " << __func__ << std::endl;
  }
}

// Test 2: When sqlite3OsFileControl returns SQLITE_OK and noSync is false, OsSync is invoked
void test_OkAndSyncInvoked()
{
  printHeader("Test 2: OK -> Sync Called");
  resetMocks();
  g_expectedZSuper = "Test2_Super";

  g_fileControlReturn = SQLITE_OK;
  g_osSyncReturn = SQLITE_OK;

  Pager p;
  p.fd = 101;
  p.noSync = 0;
  p.syncFlags = 0;

  int rc = sqlite3PagerSync(&p, g_expectedZSuper);

  TEST_ASSERT(rc == SQLITE_OK, "rc should be SQLITE_OK when OsFileControl returns OK and Sync succeeds");
  TEST_ASSERT(g_osSyncCalled == 1, "OsSync should be called once");
  TEST_ASSERT(g_seenPArg == (void*)g_expectedZSuper, "zSuper should be passed to OsFileControl");

  if (g_currentTestPass) {
    std::cout << "PASS: " << __func__ << std::endl;
  } else {
    std::cout << "FAIL: " << __func__ << std::endl;
  }
}

// Test 3: When OsSync returns an error, that error should propagate to the caller
void test_OsSync_Returns_Error_Propagated()
{
  printHeader("Test 3: Ok -> OsSync returns error");
  resetMocks();
  g_expectedZSuper = "Test3_Super";

  g_fileControlReturn = SQLITE_OK;
  g_osSyncReturn = SQLITE_BUSY; // simulate error

  Pager p;
  p.fd = 222;
  p.noSync = 0;
  p.syncFlags = 0;

  int rc = sqlite3PagerSync(&p, g_expectedZSuper);

  TEST_ASSERT(rc == SQLITE_BUSY, "rc should propagate SQLITE_BUSY from OsSync");
  TEST_ASSERT(g_osSyncCalled == 1, "OsSync should be called once on success path");
  TEST_ASSERT(g_seenPArg == (void*)g_expectedZSuper, "zSuper should be passed to OsFileControl");

  if (g_currentTestPass) {
    std::cout << "PASS: " << __func__ << std::endl;
  } else {
    std::cout << "FAIL: " << __func__ << std::endl;
  }
}

// Test 4: When noSync is true, OsSync should not be called, even if OsFileControl returns OK
void test_NoSync_Flag_Prevents_OsSync()
{
  printHeader("Test 4: noSync prevents OsSync call");
  resetMocks();
  g_expectedZSuper = "Test4_Super";

  g_fileControlReturn = SQLITE_OK;
  g_osSyncReturn = SQLITE_OK;

  Pager p;
  p.fd = 333;
  p.noSync = 1; // do not perform sync
  p.syncFlags = 0;

  int rc = sqlite3PagerSync(&p, g_expectedZSuper);

  TEST_ASSERT(rc == SQLITE_OK, "rc should be SQLITE_OK when not syncing (noSync=1) despite OK OsFileControl");
  TEST_ASSERT(g_osSyncCalled == 0, "OsSync should NOT be called when noSync is true");
  TEST_ASSERT(g_seenPArg == (void*)g_expectedZSuper, "zSuper should be passed to OsFileControl");

  if (g_currentTestPass) {
    std::cout << "PASS: " << __func__ << std::endl;
  } else {
    std::cout << "FAIL: " << __func__ << std::endl;
  }
}

// Test 5: When OsFileControl returns an error (not NOTFOUND/OK), OsSync should not be called
void test_OsFileControl_Returns_Error_No_Sync()
{
  printHeader("Test 5: OsFileControl returns error; no Sync attempted");
  resetMocks();
  g_expectedZSuper = "Test5_Super";

  g_fileControlReturn = SQLITE_ERROR; // not NOTFOUND, not OK
  g_osSyncReturn = SQLITE_OK;

  Pager p;
  p.fd = 444;
  p.noSync = 0;
  p.syncFlags = 0;

  int rc = sqlite3PagerSync(&p, g_expectedZSuper);

  TEST_ASSERT(rc == SQLITE_ERROR, "rc should be SQLITE_ERROR when OsFileControl returns error");
  TEST_ASSERT(g_osSyncCalled == 0, "OsSync should NOT be called when OsFileControl fails");
  TEST_ASSERT(g_seenPArg == (void*)g_expectedZSuper, "zSuper should be passed to OsFileControl");

  if (g_currentTestPass) {
    std::cout << "PASS: " << __func__ << std::endl;
  } else {
    std::cout << "FAIL: " << __func__ << std::endl;
  }
}

// ---------------------------------------------------------------------------
// Main entry: run all tests
// ---------------------------------------------------------------------------

int main() {
  // Run tests
  test_NotFound_Is_Handled_As_OK_With_Sync();
  test_OkAndSyncInvoked();
  test_OsSync_Returns_Error_Propagated();
  test_NoSync_Flag_Prevents_OsSync();
  test_OsFileControl_Returns_Error_No_Sync();

  // Summary (simple pass/fail based on per-test output)
  // Note: Individual tests print PASS/FAIL lines as they complete.
  std::cout << "All tests executed." << std::endl;
  return 0;
}