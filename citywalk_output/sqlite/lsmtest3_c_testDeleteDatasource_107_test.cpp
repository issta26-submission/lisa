// Candidate Keywords extracted from the focal method and its class dependencies:
// - testDeleteDatasource, testDatasourceEntry, testDelete
// - pDb (TestDb*), pData (Datasource*), i (int), pRc (int*)
// - pKey, nKey (internal keys used to delete)
// - TestDb, Datasource, CksumDb (as used in accompanying test scaffolding)
// - testWriteDatasource, testDelete (lower-level operations invoked by the focal method)
// This test suite provides a lightweight, non-terminating assertion framework and exercises
// the focal function testDeleteDatasource in a C/C++ integrated environment.

#include <iostream>
#include <lsmtest.h>
#include <vector>
#include <string>


// Minimal non-terminating test framework
static int g_totalFailures = 0;

#define EXPECT_TRUE(cond) \
  do { \
    if(!(cond)) { \
      ++g_totalFailures; \
      std::cerr << "[FAILED] " << __FUNCTION__ << ": Expected true but was false: " #cond << "\n"; \
    } \
  } while(0)

#define EXPECT_FALSE(cond) \
  do { \
    if((cond)) { \
      ++g_totalFailures; \
      std::cerr << "[FAILED] " << __FUNCTION__ << ": Expected false but was true: " #cond << "\n"; \
    } \
  } while(0)

#define EXPECT_EQ(a,b) \
  do { \
    if(!((a) == (b))) { \
      ++g_totalFailures; \
      std::cerr << "[FAILED] " << __FUNCTION__ << ": Expected " #a " == " #b ", but got " << (a) << " != " << (b) << "\n"; \
    } \
  } while(0)

using namespace std;

// Forward declarations for the C API used by the focal method.
// These declarations assume integration with the existing C/C project under test.
// The actual definitions are expected to be provided by the project (lsmtest.h / linked library).
extern "C" {

  // Opaque types from the project
  typedef struct TestDb TestDb;
  typedef struct Datasource Datasource;
  typedef struct CksumDb CksumDb;

  // Factory/utility functions provided by the test framework (assumed)
  // Create and destroy a test database
  TestDb* lsmTestDbNew();
  void lsmTestDbFree(TestDb *pDb);

  // Create and destroy a datasource associated with a test db
  Datasource* lsmTestDatasourceNew(TestDb *pDb);
  void lsmTestDatasourceFree(Datasource *pData);

  // The functions from the class dependencies (simplified signatures)
  // See <FOCAL_CLASS_DEP> declarations in the prompt
  CksumDb *testCksumArrayNew(Datasource *pData, int nFirst, int nLast, int nStep);
  char *testCksumArrayGet(CksumDb *p, int nRow);
  void testCksumArrayFree(CksumDb *p);
  void testWriteDatasource(TestDb *pDb, Datasource *pData, int i, int *pRc);
  void testDeleteDatasource(TestDb *pDb, Datasource *pData, int i, int *pRc);
  void testDatasourceEntry(Datasource *pData, int i, void **pKey, int *pKeyLen, int, int);

  // The focal function under test (redeclared for clarity)
  void testDeleteDatasource(TestDb *pDb, Datasource *pData, int i, int *pRc);
}

// Lightweight wrapper to run a test and report result count
static void runTest(const string &name, const function<void()> &fn) {
  cout << "[RUN] " << name << "\n";
  g_totalFailures = 0;
  fn();
  if (g_totalFailures == 0) {
    cout << "[PASS] " << name << "\n";
  } else {
    cout << "[FAIL] " << name << ": " << g_totalFailures << " failure(s)\n";
  }
}

// Test: basic delete path for an existing datasource entry
static void test_delete_datasource_basic_path() {
  // Create a test DB and a datasource
  TestDb *pDb = lsmTestDbNew();
  Datasource *pData = lsmTestDatasourceNew(pDb);

  // Ensure we can write an entry first (positive path)
  int rcWrite = 0;
  testWriteDatasource(pDb, pData, 0, &rcWrite);
  EXPECT_TRUE(rcWrite == 0);

  // Now delete the previously written entry using the focal method
  int rcDelete = 0;
  testDeleteDatasource(pDb, pData, 0, &rcDelete);

  // We expect a successful delete (rc presumably 0 on success)
  EXPECT_TRUE(rcDelete == 0);

  // Cleanup
  lsmTestDatasourceFree(pData);
  lsmTestDbFree(pDb);
}

// Test: attempt to delete a non-existent entry (false branch coverage)
static void test_delete_datasource_nonexistent_entry() {
  // Create a test DB and a datasource
  TestDb *pDb = lsmTestDbNew();
  Datasource *pData = lsmTestDatasourceNew(pDb);

  // Attempt to delete an entry that was never written
  int rcDelete = 0;
  testDeleteDatasource(pDb, pData, 9999, &rcDelete);

  // Expect a non-zero return code (failure path)
  EXPECT_TRUE(rcDelete != 0);

  // Cleanup
  lsmTestDatasourceFree(pData);
  lsmTestDbFree(pDb);
}

// Test: multiple consecutive deletes to exercise potential internal state transitions
static void test_delete_datasource_multiple() {
  TestDb *pDb = lsmTestDbNew();
  Datasource *pData = lsmTestDatasourceNew(pDb);

  // Write several entries
  const int N = 3;
  for (int i = 0; i < N; ++i) {
    int rcWrite = 0;
    testWriteDatasource(pDb, pData, i, &rcWrite);
    EXPECT_TRUE(rcWrite == 0);
  }

  // Delete them one by one via the focal method
  for (int i = 0; i < N; ++i) {
    int rcDel = 0;
    testDeleteDatasource(pDb, pData, i, &rcDel);
    EXPECT_TRUE(rcDel == 0);
  }

  // Final attempt to delete already-deleted entry to trigger false path
  int rcDelExtra = 0;
  testDeleteDatasource(pDb, pData, 0, &rcDelExtra);
  EXPECT_TRUE(rcDelExtra != 0);

  // Cleanup
  lsmTestDatasourceFree(pData);
  lsmTestDbFree(pDb);
}

// entry point
int main() {
  vector<pair<string, function<void()>>> tests = {
    {"test_delete_datasource_basic_path", test_delete_datasource_basic_path},
    {"test_delete_datasource_nonexistent_entry", test_delete_datasource_nonexistent_entry},
    {"test_delete_datasource_multiple", test_delete_datasource_multiple}
  };

  cout << "Starting test suite for testDeleteDatasource (C/C++ integrated tests)\n";

  int totalFailures = 0;
  for (auto &t : tests) {
    g_totalFailures = 0;
    cout << "Running: " << t.first << "\n";
    t.second(); // run the test
    totalFailures += g_totalFailures;
  }

  if (totalFailures == 0) {
    cout << "ALL TESTS PASSED\n";
  } else {
    cout << "TOTAL FAILURES: " << totalFailures << "\n";
  }

  return totalFailures;
}