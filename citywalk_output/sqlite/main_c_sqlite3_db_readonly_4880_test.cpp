// Self-contained C++11 test suite for the focal C function sqlite3_db_readonly
// This file provides minimal stub implementations for the dependent SQLite
// components so the focal function can be exercised in isolation without GTest.

// The tests use non-terminating EXPECT_* style assertions to maximize coverage
// without halting on first failure, and they are invoked from main() as required
// when GTest is not allowed.

#include <cstdio>
#include <sqliteInt.h>


// Simple non-terminating test assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ_INT(actual, expected) \
  do { \
    ++g_total_tests; \
    if ((actual) != (expected)) { \
      fprintf(stderr, "Test assertion failed: %s:%d: EXPECT_EQ_INT(%d, %d) but got %d\n", __FILE__, __LINE__, (int)(expected), (int)(expected), (int)(actual)); \
      ++g_failed_tests; \
    } \
  } while (0)


// Step 1 & 2: Provide necessary dependencies and the focal function in C linkage
extern "C" {

// Enable the API armor path in the focal function for testing the negative path
#define SQLITE_ENABLE_API_ARMOR 1
#ifndef SQLITE_MISUSE_BKPT
#define SQLITE_MISUSE_BKPT 0
#endif

// Forward declaration of sqlite3 struct (opaque in the real project)
typedef struct sqlite3 sqlite3;

// Minimal Btree structure used by the focal function
typedef struct Btree {
  int isReadonly;
} Btree;

// Static test-friendly Btree instances to drive test scenarios
static Btree g_ro_btree  = {1}; // readonly
static Btree g_rw_btree  = {0}; // not readonly

// Stub: sqlite3DbNameToBtree(db, zDbName)
static Btree *sqlite3DbNameToBtree(sqlite3 *db, const char *zDbName) {
  if (zDbName == nullptr) return nullptr;
  // Map specific db names to test Btrees
  if (strcmp(zDbName, "ro") == 0)  return &g_ro_btree;
  if (strcmp(zDbName, "rw") == 0)  return &g_rw_btree;
  // "null" or unknown names yield a null pointer to exercise -1 path
  if (strcmp(zDbName, "null") == 0) return nullptr;
  return nullptr;
}

// Stub: sqlite3BtreeIsReadonly(Btree*)
static int sqlite3BtreeIsReadonly(Btree *p) {
  return (p != nullptr) ? p->isReadonly : -1;
}

// Static safety check used by the API armor path
static int sqlite3SafetyCheckOk(sqlite3 *db) {
  return db != nullptr;
}

// The focal method under test (copied verbatim from the provided snippet)
int sqlite3_db_readonly(sqlite3 *db, const char *zDbName) {
  Btree *pBt;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ){
    (void)SQLITE_MISUSE_BKPT;
    return -1;
  }
#endif
  pBt = sqlite3DbNameToBtree(db, zDbName);
  return pBt ? sqlite3BtreeIsReadonly(pBt) : -1;
}

// End of focal function and dependencies
} // extern "C"


// Helper wrappers for tests (useful to ensure we always have a non-null db pointer)
static sqlite3 *makeDummyDb() {
  // A minimal dummy sqlite3 object; only its non-null identity matters for tests
  static sqlite3 dummy;
  return &dummy;
}


// Step 3: Individual test cases

// Test 1: NULL db pointer should trigger API armor path and return -1
static void test_null_db_should_return_-1_due_to_safety_check() {
  // NULL db triggers safety check fail, even with zDbName provided
  sqlite3 *db = nullptr;
  int rc = sqlite3_db_readonly(db, "ro");
  EXPECT_EQ_INT(rc, -1);
}

// Test 2: NULL db pointer with a different zDbName should also return -1
static void test_null_db_with_other_dbname_should_return_-1() {
  sqlite3 *db = nullptr;
  int rc = sqlite3_db_readonly(db, "rw");
  EXPECT_EQ_INT(rc, -1);
}

// Test 3: Valid db, zDbName maps to readonly Btree => should return 1 (true)
static void test_valid_db_readonly_true() {
  sqlite3 *db = makeDummyDb();
  int rc = sqlite3_db_readonly(db, "ro");
  EXPECT_EQ_INT(rc, 1);
}

// Test 4: Valid db, zDbName maps to writable Btree => should return 0 (false)
static void test_valid_db_readonly_false() {
  sqlite3 *db = makeDummyDb();
  int rc = sqlite3_db_readonly(db, "rw");
  EXPECT_EQ_INT(rc, 0);
}

// Test 5: Valid db, unknown zDbName should map to null and return -1
static void test_valid_db_unknown_dbname_should_return_-1() {
  sqlite3 *db = makeDummyDb();
  int rc = sqlite3_db_readonly(db, "unknown");
  EXPECT_EQ_INT(rc, -1);
}

// Test 6: Valid db, explicit "null" zDbName should return -1 (null Btree)
static void test_valid_db_null_zdbname_should_return_-1() {
  sqlite3 *db = makeDummyDb();
  int rc = sqlite3_db_readonly(db, "null");
  EXPECT_EQ_INT(rc, -1);
}


// Step 4: Run all tests and summarize

static void run_all_tests() {
  test_null_db_should_return_-1_due_to_safety_check();
  test_null_db_with_other_dbname_should_return_-1();
  test_valid_db_readonly_true();
  test_valid_db_readonly_false();
  test_valid_db_unknown_dbname_should_return_-1();
  test_valid_db_null_zdbname_should_return_-1();

  // Summary
  printf("Total tests executed: %d\n", g_total_tests);
  printf("Total tests failed:   %d\n", g_failed_tests);
  if (g_failed_tests == 0) {
    printf("All tests PASSED.\n");
  } else {
    printf("Some tests FAILED. Please review the logs above.\n");
  }
}

// Main entry point for the test executable
int main() {
  run_all_tests();
  // Return non-zero if any test failed
  return (g_failed_tests == 0) ? 0 : 1;
}