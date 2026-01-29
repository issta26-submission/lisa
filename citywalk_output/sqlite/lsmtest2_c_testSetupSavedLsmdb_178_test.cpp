// This test suite targets the focal method testSetupSavedLsmdb from lsmtest2.c.
// It uses lightweight, in-process C-style stubs to exercise different branches
// of the function without requiring the full project I/O or external dependencies.
// The tests are written in C++11 but avoid any external testing framework (no GTest).

#include <iostream>
#include <cstdio>
#include <lsmtest.h>


// Domain types used by the focal method (mirrored here for test stubs)
typedef struct Datasource { int dummy; } Datasource;
typedef struct TestDb { int opened; int written; } TestDb;

// Lightweight, non-terminating assertion macro
static int gTestsFailed = 0;
#define EXPECT_TRUE(cond, msg) \
  do { if(!(cond)) { std::cerr << "[TEST FAILED] " << msg << "\n"; ++gTestsFailed; } } while(0)

// Mocked global state to drive the stubbed dependencies
static bool g_openShouldSucceed = true;
static int g_testWriteRc = 0;
static bool g_savedDbCalled = false;
static bool g_dbClosed = false;

// Forward declaration of the focal method under test (as provided in lsmtest2.c)
extern "C" void testSetupSavedLsmdb(const char *zCfg,
                                   const char *zFile,
                                   Datasource *pData,
                                   int nRow,
                                   int *pRc);

// C linkage for the stubs so that the focal method can call them
extern "C" {
  // Stubs for the dependencies used by testSetupSavedLsmdb

  // Open a TestDb; succeed or fail based on g_openShouldSucceed
  int tdb_lsm_open(const char *zCfg, const char *zFile, int flag, TestDb **ppDb) {
    if (g_openShouldSucceed) {
      *ppDb = new TestDb{1, 0};
      g_dbClosed = false;
      return 0;
    } else {
      *ppDb = nullptr;
      return -1;
    }
  }

  // Write a datasource range; the outcome is controlled by g_testWriteRc
  void testWriteDatasourceRange(TestDb *pDb, Datasource *pData, int iFrom, int nRow, int *pRc) {
    if (pDb && pDb->opened) {
      pDb->written = 1;
      *pRc = g_testWriteRc;
    } else {
      *pRc = -1;
    }
  }

  // Close and delete the TestDb
  void testClose(TestDb **ppDb) {
    g_dbClosed = true;
    if (ppDb && *ppDb) {
      delete *ppDb;
      *ppDb = nullptr;
    }
  }

  // Save the DB (no real IO), used to verify conditional invocation
  void testSaveDb(const char *zFile, const char *mode) {
    g_savedDbCalled = true;
  }

  // Other static helpers in the original file (not used directly in tests)
  int testCksumDatabase(TestDb *pDb, char *zOut){ (void)pDb; (void)zOut; return 0; }
  int testCountDatabase(TestDb *pDb){ (void)pDb; return 0; }
  void testCaseStart(int *pRc, const char *zFmt, ...) { (void)pRc; (void)zFmt; }
  int testCaseBegin(int *pRc, const char *zPattern, const char *zFmt, ...) { (void)pRc; (void)zPattern; (void)zFmt; return 0; }
  void testCaseFinish(int rc) { (void)rc; }
  void testCaseSkip() { }
  void do_crash_test(const char *zPattern, int *pRc) { (void)zPattern; (void)pRc; }
}

// Helper to reset mocks between tests (except for total failures counter)
static void resetMocks() {
  g_openShouldSucceed = true;
  g_testWriteRc = 0;
  g_savedDbCalled = false;
  g_dbClosed = false;
}

// Test 1: True path where open succeeds and write range succeeds -> close called and saveDb invoked
static void test_all_paths_success() {
  resetMocks();
  g_openShouldSucceed = true;
  g_testWriteRc = 0;
  g_savedDbCalled = false;
  g_dbClosed = false;

  Datasource ds{0};
  int rc = 0;
  testSetupSavedLsmdb("cfg", "file", &ds, 10, &rc);

  EXPECT_TRUE(rc == 0, "test_all_paths_success: rc should be 0 on success path");
  EXPECT_TRUE(g_dbClosed, "test_all_paths_success: database should be closed after operation");
  EXPECT_TRUE(g_savedDbCalled, "test_all_paths_success: testSaveDb should be called on success");
}

// Test 2: Open fails -> no writes, no close, no save
static void test_open_failure_no_calls() {
  resetMocks();
  g_openShouldSucceed = false;
  g_testWriteRc = 0;
  g_savedDbCalled = false;
  g_dbClosed = false;

  Datasource ds{0};
  int rc = 0;
  testSetupSavedLsmdb("cfg", "file", &ds, 5, &rc);

  EXPECT_TRUE(rc != 0, "test_open_failure_no_calls: rc should be non-zero when open fails");
  EXPECT_TRUE(!g_dbClosed, "test_open_failure_no_calls: db should not be closed on open failure");
  EXPECT_TRUE(!g_savedDbCalled, "test_open_failure_no_calls: saveDb should not be called when open fails");
}

// Test 3: Open succeeds but write fails -> close should happen, save should not be called
static void test_write_failure_no_save() {
  resetMocks();
  g_openShouldSucceed = true;
  g_testWriteRc = -7; // simulate a write error
  g_savedDbCalled = false;
  g_dbClosed = false;

  Datasource ds{0};
  int rc = 0;
  testSetupSavedLsmdb("cfg", "file", &ds, 8, &rc);

  EXPECT_TRUE(rc == -7, "test_write_failure_no_save: rc should propagate write error");
  EXPECT_TRUE(g_dbClosed, "test_write_failure_no_save: db should be closed on write error");
  EXPECT_TRUE(!g_savedDbCalled, "test_write_failure_no_save: saveDb should not be called when write fails");
}

// Test 4: pRc non-zero initially -> function should skip any operation
static void test_skip_when_pRc_nonzero() {
  resetMocks();
  int pRc = 1; // non-zero initial value
  Datasource ds{0};
  testSetupSavedLsmdb("cfg", "file", &ds, 3, &pRc);
  EXPECT_TRUE(pRc == 1, "test_skip_when_pRc_nonzero: pRc should remain unchanged when non-zero");
  EXPECT_TRUE(!g_dbClosed, "test_skip_when_pRc_nonzero: no db close should occur when skipped");
  EXPECT_TRUE(!g_savedDbCalled, "test_skip_when_pRc_nonzero: saveDb should not be called when skipped");
}

// Main entry: run all tests and report results
int main() {
  test_all_paths_success();
  test_open_failure_no_calls();
  test_write_failure_no_save();
  test_skip_when_pRc_nonzero();

  if (gTestsFailed == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cerr << gTestsFailed << " test(s) failed.\n";
    return 1;
  }
}