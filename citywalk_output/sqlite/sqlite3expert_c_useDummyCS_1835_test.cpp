/*
  Unit test suite for the focal method:
    void useDummyCS(void *up1, sqlite3 *db, int etr, const char *zName)

  Objective:
  - Verify that useDummyCS delegates correctly to sqlite3_create_collation_v2 with
    the expected parameters.
  - Ensure the function passes the exact dummy comparator (dummyCompare) and
    preserves the passed sqlite3* pointer, etr value, and zName string.
  - Use a lightweight, non-terminating test framework (no GTest) and run tests
    from main() to maximize coverage.

  Notes:
  - We provide a test-specific override of sqlite3_create_collation_v2 to capture
    the arguments passed by useDummyCS. This overrides the SQLite library symbol
    for the duration of the test translation unit.
  - The dummyCompare function is declared in sqlite3expert.h as a non-static
    function; thus we can compare the function pointer against dummyCompare.
*/

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <sqlite3expert.h>


// Candidate Keywords extracted from the focal method:
// - useDummyCS
// - sqlite3_create_collation_v2
// - dummyCompare
// - sqlite3 (db handle, in-memory DB usage for testing)
// - zName (name of the collation)
// - etr (text encoding/comparison type parameter)

// Global state to capture the mocked sqlite3_create_collation_v2 invocation
static sqlite3* g_mock_db = nullptr;
static const char* g_mock_zName = nullptr;
static int g_mock_etr = 0;
static void* g_mock_pArg = nullptr;
static int (*g_mock_xCmp)(void*, int, const void*, int, const void*) = nullptr;
static void (*g_mock_pDestroy)(void*) = nullptr;
static bool g_mock_called = false;

/*
  Mock override of sqlite3_create_collation_v2.
  This must be declared with C linkage to replace the library symbol during linking.
*/
extern "C" int sqlite3_create_collation_v2(
    sqlite3* db, 
    const char* zName, 
    int eTextRep, 
    void* pArg,
    int (*xCmp)(void*, int, const void*, int, const void*),
    void (*pDestroy)(void*)
) {
  g_mock_called = true;
  g_mock_db = db;
  g_mock_zName = zName;
  g_mock_etr = eTextRep;
  g_mock_pArg = pArg;
  g_mock_xCmp = xCmp;
  g_mock_pDestroy = pDestroy;
  // Return 0 to mimic successful registration (as the real API would)
  return 0;
}

// Lightweight test harness
static int totalTests = 0;
static int failedTests = 0;

#define CHECK(cond, msg) do { \
  ++totalTests; \
  if (cond) { \
    printf("[OK] %s\n", msg); \
  } else { \
    printf("[FAIL] %s\n", msg); \
    ++failedTests; \
  } \
} while(0)

// Test 1: Basic invocation
void test_useDummyCS_basic() {
  // Reset mock state
  g_mock_called = false;
  g_mock_db = nullptr;
  g_mock_zName = nullptr;
  g_mock_etr = 0;
  g_mock_pArg = nullptr;
  g_mock_xCmp = nullptr;
  g_mock_pDestroy = nullptr;

  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  CHECK(rc == SQLITE_OK, "sqlite3_open should return SQLITE_OK for in-memory DB");

  // Call the focal method
  void* exampleUp = (void*)0x1234; // non-null up1 to ensure it's ignored
  const char* testName = "test_collation_basic";
  useDummyCS(exampleUp, db, 42, testName);

  // Assertions about the mocked call
  CHECK(g_mock_called, "sqlite3_create_collation_v2 should be called by useDummyCS");
  CHECK(g_mock_db == db, "sqlite3_create_collation_v2 should receive the same sqlite3*");
  CHECK(g_mock_zName != nullptr && strcmp(g_mock_zName, testName) == 0,
        "sqlite3_create_collation_v2 should receive the correct zName");
  CHECK(g_mock_etr == 42, "sqlite3_create_collation_v2 should receive the correct etr");
  CHECK(g_mock_pArg == nullptr, "sqlite3_create_collation_v2 should receive nullptr for pArg (0 in call)");
  CHECK(g_mock_pDestroy == nullptr, "sqlite3_create_collation_v2 should receive nullptr for pDestroy (0 in call)");
  CHECK(g_mock_xCmp == dummyCompare, "sqlite3_create_collation_v2 should receive dummyCompare as xCmp");
  sqlite3_close(db);
}

// Test 2: Null zName scenario
void test_useDummyCS_nullName() {
  // Reset mock state
  g_mock_called = false;
  g_mock_db = nullptr;
  g_mock_zName = nullptr;
  g_mock_etr = 0;
  g_mock_pArg = nullptr;
  g_mock_xCmp = nullptr;
  g_mock_pDestroy = nullptr;

  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  CHECK(rc == SQLITE_OK, "sqlite3_open should succeed for second in-memory DB");

  // Call with NULL zName to ensure graceful handling (as a pass-through to the mock)
  void* someUp = nullptr;
  useDummyCS(someUp, db, 7, nullptr);

  // Assertions about the mocked call
  CHECK(g_mock_called, "sqlite3_create_collation_v2 should be called even when zName is NULL");
  CHECK(g_mock_db == db, "sqlite3_create_collation_v2 should receive the same sqlite3* (NULL zName case)");
  CHECK(g_mock_zName == nullptr, "sqlite3_create_collation_v2 should receive nullptr for zName");
  CHECK(g_mock_etr == 7, "sqlite3_create_collation_v2 should receive the correct etr");
  CHECK(g_mock_pArg == nullptr, "pArg should remain nullptr as in the call");
  CHECK(g_mock_pDestroy == nullptr, "pDestroy should remain nullptr as in the call");
  CHECK(g_mock_xCmp == dummyCompare, "xCmp should still be set to dummyCompare even in NULL name case");
  sqlite3_close(db);
}

// Main: Run tests
int main() {
  printf("Starting tests for useDummyCS and related dependencies...\n");
  test_useDummyCS_basic();
  test_useDummyCS_nullName();

  printf("Tests completed. Total: %d, Failed: %d\n", totalTests, failedTests);
  // Non-terminating: do not exit with failure if some tests fail
  return (failedTests == 0) ? 0 : 1;
}