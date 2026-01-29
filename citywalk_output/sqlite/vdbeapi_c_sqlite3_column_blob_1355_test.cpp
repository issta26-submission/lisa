/*
  Unit Test Suite for sqlite3_column_blob (vdbeapi.c)
  - Target: validate behavior of sqlite3_column_blob using real SQLite integration
  - Approach: implement a lightweight, non-terminating test harness in C++
  - Tests cover: non-null blob, zeroblob, and NULL cases
  - Notes:
    - This test uses the actual SQLite API via sqlite3.h and links against the sqlite3 library.
    - The tests rely on in-memory database ":memory:".
    - Observations are asserted using non-terminating EXPECT_* macros to maximize coverage.
  - Domain knowledge references:
    - Uses real sqlite3_stmt, sqlite3_bind_blob, sqlite3_bind_zeroblob, sqlite3_bind_null
    - Validates pointer returns and content via sqlite3_column_blob and sqlite3_column_bytes
  - Candidate Keywords derived from the focal method and its dependencies:
    - sqlite3_column_blob, sqlite3_value_blob, columnMem, columnMallocFailure, zeroblob, blob, pStmt, i
    - Observed behaviors: pointer return, memory expansion on zeroblob, NULL handling
*/

#include <cstdio>
#include <opcodes.h>
#include <sqlite3.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <cstdlib>



// Lightweight test harness (non-terminating assertions)
static int g_totalAssertions = 0;
static int g_failedAssertions = 0;

#define EXPECT_TRUE(cond) \
  do { \
    ++g_totalAssertions; \
    if(!(cond)) { \
      fprintf(stderr, "EXPECT_TRUE failed: %s\n", #cond); \
      ++g_failedAssertions; \
    } \
  } while(0)

#define EXPECT_EQ(a, b) \
  do { \
    ++g_totalAssertions; \
    if(!((a) == (b))) { \
      fprintf(stderr, "EXPECT_EQ failed: %s == %s, actual (%lld) vs (%lld)\n", #a, #b, (long long)(a), (long long)(b)); \
      ++g_failedAssertions; \
    } \
  } while(0)

#define EXPECT_NEQ(a, b) \
  do { \
    ++g_totalAssertions; \
    if((a) == (b)) { \
      fprintf(stderr, "EXPECT_NEQ failed: %s != %s, both are (%lld)\n", #a, #b, (long long)(a)); \
      ++g_failedAssertions; \
    } \
  } while(0)

#define EXPECT_PTR_NOT_NULL(p) \
  do { \
    ++g_totalAssertions; \
    if((p) == nullptr) { \
      fprintf(stderr, "EXPECT_PTR_NOT_NULL failed: %s is NULL\n", #p); \
      ++g_failedAssertions; \
    } \
  } while(0)

static void run_all_tests();

// Test 1: Bind a real blob and verify sqlite3_column_blob returns correct pointer and content
static void test_blob_retrieval_basic() {
  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr);
  if(db == nullptr) return;

  sqlite3_stmt *pStmt = nullptr;
  rc = sqlite3_prepare_v2(db, "SELECT ?", -1, &pStmt, nullptr);
  EXPECT_TRUE(rc == SQLITE_OK);
  EXPECT_TRUE(pStmt != nullptr);
  if(pStmt == nullptr) {
    sqlite3_close(db);
    return;
  }

  // Bind a concrete blob
  static const unsigned char blob[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  rc = sqlite3_bind_blob(pStmt, 1, blob, sizeof(blob), SQLITE_STATIC);
  EXPECT_TRUE(rc == SQLITE_OK);

  rc = sqlite3_step(pStmt);
  EXPECT_TRUE(rc == SQLITE_ROW);

  const void *p = sqlite3_column_blob(pStmt, 0);
  EXPECT_PTR_NOT_NULL(p);

  int n = sqlite3_column_bytes(pStmt, 0);
  EXPECT_EQ(n, (int)sizeof(blob));

  if(p != nullptr) {
    EXPECT_EQ(memcmp(p, blob, sizeof(blob)), 0);
  }

  rc = sqlite3_finalize(pStmt);
  EXPECT_TRUE(rc == SQLITE_OK);
  sqlite3_close(db);
}

// Test 2: Bind a zeroblob and verify blob length and content (zeros)
static void test_zeroblob_retrieval() {
  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr);
  if(db == nullptr) return;

  sqlite3_stmt *pStmt = nullptr;
  rc = sqlite3_prepare_v2(db, "SELECT ?", -1, &pStmt, nullptr);
  EXPECT_TRUE(rc == SQLITE_OK);
  EXPECT_TRUE(pStmt != nullptr);
  if(pStmt == nullptr) {
    sqlite3_close(db);
    return;
  }

  // Bind a zeroblob of length 5
  rc = sqlite3_bind_zeroblob(pStmt, 1, 5);
  EXPECT_TRUE(rc == SQLITE_OK);

  rc = sqlite3_step(pStmt);
  EXPECT_TRUE(rc == SQLITE_ROW);

  const void *p = sqlite3_column_blob(pStmt, 0);
  EXPECT_PTR_NOT_NULL(p);

  int n = sqlite3_column_bytes(pStmt, 0);
  EXPECT_EQ(n, 5);

  // Since it's a zeroblob, contents should be zeros
  if(p != nullptr) {
    const unsigned char *data = reinterpret_cast<const unsigned char*>(p);
    for(int i = 0; i < 5; ++i) {
      EXPECT_EQ((int)data[i], 0);
    }
  }

  rc = sqlite3_finalize(pStmt);
  EXPECT_TRUE(rc == SQLITE_OK);
  sqlite3_close(db);
}

// Test 3: Bind NULL and verify sqlite3_column_blob returns NULL pointer
static void test_null_blob_behavior() {
  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr);
  if(db == nullptr) return;

  sqlite3_stmt *pStmt = nullptr;
  rc = sqlite3_prepare_v2(db, "SELECT ?", -1, &pStmt, nullptr);
  EXPECT_TRUE(rc == SQLITE_OK);
  EXPECT_TRUE(pStmt != nullptr);
  if(pStmt == nullptr) {
    sqlite3_close(db);
    return;
  }

  // Bind NULL
  rc = sqlite3_bind_null(pStmt, 1);
  EXPECT_TRUE(rc == SQLITE_OK);

  rc = sqlite3_step(pStmt);
  EXPECT_TRUE(rc == SQLITE_ROW);

  const void *p = sqlite3_column_blob(pStmt, 0);
  // For NULL, blob pointer should be NULL
  EXPECT_TRUE(p == nullptr);

  rc = sqlite3_finalize(pStmt);
  EXPECT_TRUE(rc == SQLITE_OK);
  sqlite3_close(db);
}

// Run all tests and report a final summary
static void run_all_tests() {
  printf("Starting sqlite3_column_blob unit tests using real SQLite API...\n");

  test_blob_retrieval_basic();
  test_zeroblob_retrieval();
  test_null_blob_behavior();

  printf("Test Summary: total assertions = %d, failures = %d\n",
         g_totalAssertions, g_failedAssertions);

  if(g_failedAssertions > 0) {
    fprintf(stderr, "Some tests FAILED. See details above.\n");
  } else {
    printf("All tests PASSED.\n");
  }
}

int main() {
  run_all_tests();
  // Return non-zero if any assertion failed
  return (g_failedAssertions > 0) ? 1 : 0;
}