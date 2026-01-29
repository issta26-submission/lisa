// Unit test suite for the focal method testWriteDatasource in lsmtest3.c
// This test suite is crafted to be compiled with a C++11 compiler, without GTest.
// It uses a lightweight in-file test harness and no external testing framework.
// The tests rely on the presence of the following focal interfaces (as per <FOCAL_METHOD>):
//   void testWriteDatasource(TestDb *pDb, Datasource *pData, int i, int *pRc)
//   void testDatasourceEntry(Datasource *pData, int i, void **ppKey, int *pnKey, void **ppVal, int *pnVal)
//   void testWrite(TestDb *pDb, void *pKey, int nKey, void *pVal, int nVal, int *pRc)
// The test code provides lightweight C-callable mocks for testDatasourceEntry and testWrite
// to drive and observe testWriteDatasource behavior in isolation from the real database logic.
// It then exercises testWriteDatasource with valid and invalid indices to cover true/false branches.

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <lsmtest.h>


// Candidate Keywords extracted from Step 1 (for traceability in tests)
 // { Candidate Keywords: testWriteDatasource, TestDb, Datasource, testDatasourceEntry, testWrite,
 //   pData, i, pRc, pKey, pVal, nKey, nVal, pDb }

// Forward declarations of opaque types as seen by the focal code.
// We only need them as pointers here; their internals are not required for the test harness.
typedef struct TestDb TestDb;
typedef struct Datasource Datasource;

// Prototypes of the focal functions expected to exist (externally linked in lsmtest3.c).
extern "C" {
  void testDatasourceEntry(Datasource *pData, int i, void **ppKey, int *pnKey, void **ppVal, int *pnVal);
  void testWrite(TestDb *pDb, void *pKey, int nKey, void *pVal, int nVal, int *pRc);
}

// Declaration of the focal function under test.
// We'll declare it as extern to emphasize we are testing an existing symbol from lsmtest3.c.
extern "C" void testWriteDatasource(TestDb *pDb, Datasource *pData, int i, int *pRc);

// Lightweight, self-contained test harness (no external framework)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Simple non-terminating assertion macro (prints but does not abort)
#define TEST_ASSERT(cond, msg) do { \
  if (!(cond)) { \
    std::cerr << "TEST FAIL: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    ++g_tests_failed; \
  } else { \
    std::cout << "TEST PASS: " << (msg) << std::endl; \
  } \
  ++g_tests_run; \
} while(0)

// In-test helper structures to mock a tiny datasource
struct Entry {
  char *pKey;
  int nKey;
  char *pVal;
  int nVal;
};

static Entry g_entries[16];
static int g_entries_count = 0;

// State to observe which entry was "written" by the mock testWrite
static int g_last_written_index = -1;

// Helper to add a test entry (allocates simple buffers; lifetime managed by test harness)
static void add_entry(const char *k, const char *v) {
  if (g_entries_count >= 16) return;
  Entry &e = g_entries[g_entries_count];
  e.nKey = (int)std::strlen(k);
  e.nVal = (int)std::strlen(v);
  // Allocate exact bytes (no terminator requirement for raw key/value in tests)
  e.pKey = (char*)std::malloc(e.nKey);
  if (e.pKey) std::memcpy(e.pKey, k, e.nKey);
  e.pVal = (char*)std::malloc(e.nVal);
  if (e.pVal) std::memcpy(e.pVal, v, e.nVal);
  ++g_entries_count;
}

// Mock implementation of testDatasourceEntry to supply test data to testWriteDatasource
extern "C" void testDatasourceEntry(Datasource * /*pData*/, int i, void **ppKey, int *pnKey, void **ppVal, int *pnVal) {
  if (i >= 0 && i < g_entries_count) {
    *ppKey = (void*)g_entries[i].pKey;
    *pnKey = g_entries[i].nKey;
    *ppVal = (void*)g_entries[i].pVal;
    *pnVal = g_entries[i].nVal;
  } else {
    *ppKey = nullptr;
    *pnKey = 0;
    *ppVal = nullptr;
    *pnVal = 0;
  }
}

// Mock implementation of testWrite to observe how testWriteDatasource interacts with the inputs
extern "C" void testWrite(TestDb * /*pDb*/, void *pKey, int nKey, void */*pVal*/, int /*nVal*/, int *pRc) {
  // Look for a matching entry by key and key length
  int found = -1;
  for (int idx = 0; idx < g_entries_count; ++idx) {
    if (g_entries[idx].nKey == nKey && pKey != nullptr && g_entries[idx].pKey != nullptr) {
      if (std::memcmp(pKey, g_entries[idx].pKey, nKey) == 0) {
        found = idx;
        break;
      }
    }
  }
  if (found >= 0) {
    *pRc = 0;             // success
    g_last_written_index = found;
  } else {
    *pRc = -1;            // failure (no matching entry)
    g_last_written_index = -1;
  }
}

// Helper to reset global state before each test
static void reset_state() {
  // free any allocated keys/values from previous entries
  for (int i = 0; i < g_entries_count; ++i) {
    if (g_entries[i].pKey) { free(g_entries[i].pKey); g_entries[i].pKey = nullptr; }
    if (g_entries[i].pVal) { free(g_entries[i].pVal); g_entries[i].pVal = nullptr; }
  }
  g_entries_count = 0;
  g_last_written_index = -1;
}

// Test 1: Valid index 0 should succeed and write the first entry
static void testWriteDatasource_valid_index_zero() {
  reset_state();
  add_entry("A", "Alpha"); // i = 0
  // Prepare opaque pointers as expected by the focal function (we pass nullptrs where safe)
  TestDb *pDb = nullptr;
  Datasource *pData = nullptr;
  int rc = -999;

  // Call the focal function
  testWriteDatasource(pDb, pData, 0, &rc);

  // Assertions
  TEST_ASSERT(rc == 0, "testWriteDatasource should return 0 for valid index 0");
  TEST_ASSERT(g_last_written_index == 0, "testWrite should write the entry at index 0");
}

// Test 2: Valid index 1 should succeed and write the second entry
static void testWriteDatasource_valid_index_one() {
  reset_state();
  add_entry("A", "Alpha"); // i = 0
  add_entry("B", "Beta");  // i = 1
  TestDb *pDb = nullptr;
  Datasource *pData = nullptr;
  int rc = -999;

  testWriteDatasource(pDb, pData, 1, &rc);

  TEST_ASSERT(rc == 0, "testWriteDatasource should return 0 for valid index 1");
  TEST_ASSERT(g_last_written_index == 1, "testWrite should write the entry at index 1");
}

// Test 3: Invalid index should signal failure
static void testWriteDatasource_invalid_index() {
  reset_state();
  add_entry("X", "Xray"); // i = 0
  TestDb *pDb = nullptr;
  Datasource *pData = nullptr;
  int rc = -999;

  testWriteDatasource(pDb, pData, 2, &rc); // i=2 does not exist

  TEST_ASSERT(rc != 0, "testWriteDatasource should not succeed for invalid index");
  TEST_ASSERT(g_last_written_index == -1, "testWrite should not record a write for invalid index");
}

// Entry point that runs all tests
int main() {
  std::cout << "Starting test suite for testWriteDatasource (lsmtest3.c) using C++11 harness" << std::endl;

  // Run tests
  testWriteDatasource_valid_index_zero();
  testWriteDatasource_valid_index_one();
  testWriteDatasource_invalid_index();

  // Summary
  std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;

  // Cleanup any remaining allocations
  reset_state();

  return g_tests_failed == 0 ? 0 : 1;
}