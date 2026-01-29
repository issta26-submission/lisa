// C++11 Unit Test Suite for sqlite3RecordErrorByteOffset
// This test suite is designed to be compiled in a single translation unit
// by including the focal C source file (printf.c) to obtain access to
// internal SQLite structures and the function under test.
// It uses a lightweight, non-terminating test harness (no GTest) as requested.

#include <printf.c>
#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


// Include the focal implementation so that the test harness and the function
// share the same internal type definitions (sqlite3, Parse, etc.)
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT(cond, msg)                          \
  do {                                                  \
    ++g_total_tests;                                    \
    if(!(cond)) {                                       \
      g_failed_tests++;                                 \
      printf("TEST FAIL: %s\n", msg);                   \
    } else {                                            \
      printf("TEST PASS: %s\n", msg);                   \
    }                                                   \
  } while(0)

//
// Helpers to isolate tests
//

// Helper to reset a sqlite3-like db object to a clean state
static void resetDb(sqlite3 *db) {
  // We blindly zero the whole object to reset internal fields used by tests.
  // The actual size is known to the translation unit after including printf.c.
  memset(db, 0, sizeof(*db));
}

//
// Test 1: Normal path where all preconditions hold and z points inside zTail.
// Expect: db->errByteOffset is set to the offset (3 in this test).
//
static void test_NormalPath() {
  // Prepare tail text and Parse structure
  char zTail[] = "0123456789";
  Parse pParse;
  pParse.zTail = zTail;

  // Prepare db with required initial state
  sqlite3 db;
  resetDb(&db);
  db.errByteOffset = -2;
  db.pParse = &pParse;

  // z pointing to a position inside zTail (offset 3)
  const char *z = &zTail[3];

  // Call the focal method
  sqlite3RecordErrorByteOffset(&db, z);

  // Verify: offset should be set to 3
  TEST_ASSERT(db.errByteOffset == 3, "NormalPath: errByteOffset should be 3 when z points inside zTail at offset 3");
}

//
// Test 2: Early return when db->errByteOffset != -2.
// Expect: db->errByteOffset remains unchanged (e.g., 5).
//
static void test_ErrByteOffset_NotMinus2() {
  char zTail[] = "abcdefghij";
  Parse pParse;
  pParse.zTail = zTail;

  sqlite3 db;
  resetDb(&db);
  db.errByteOffset = 5; // not equal to -2
  db.pParse = &pParse;

  // z inside tail
  const char *z = &zTail[2];

  sqlite3RecordErrorByteOffset(&db, z);

  // Verify: value unchanged
  TEST_ASSERT(db.errByteOffset == 5, "ErrByteOffset_NotMinus2: should not modify when errByteOffset != -2");
}

//
// Test 3: Early return when pParse is NULL.
// Expect: db->errByteOffset remains unchanged (-2).
//
static void test_PParseNull() {
  char zTail[] = "xyzxyz";
  Parse pParse;
  pParse.zTail = zTail;

  sqlite3 db;
  resetDb(&db);
  db.errByteOffset = -2;
  db.pParse = &pParse;

  // Now simulate NULL pParse
  db.pParse = NULL;

  const char *z = &zTail[1];

  sqlite3RecordErrorByteOffset(&db, z);

  // Verify: value unchanged
  TEST_ASSERT(db.errByteOffset == -2, "PParseNull: should not modify when pParse is NULL");
}

//
// Test 4: Early return when zTail is NULL.
// Expect: db->errByteOffset remains unchanged (-2).
//
static void test_ZTailNull() {
  // zTail is NULL
  Parse pParse;
  // No zTail assigned
  pParse.zTail = NULL;

  sqlite3 db;
  resetDb(&db);
  db.errByteOffset = -2;
  db.pParse = &pParse;

  const char *z = "dummy"; // z must be non-null per contract of the function

  sqlite3RecordErrorByteOffset(&db, z);

  // Verify: value unchanged
  TEST_ASSERT(db.errByteOffset == -2, "ZTailNull: should not modify when zTail is NULL");
}

//
// Main test runner
//
int main() {
  printf("Starting sqlite3RecordErrorByteOffset unit tests (C++11 harness).\n");

  test_NormalPath();
  test_ErrByteOffset_NotMinus2();
  test_PParseNull();
  test_ZTailNull();

  printf("Tests completed. Total: %d, Passed: %d, Failed: %d\n",
         g_total_tests, g_total_tests - g_failed_tests, g_failed_tests);

  // Return non-zero if any test failed
  return g_failed_tests ? 1 : 0;
}