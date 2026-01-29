// Lightweight C++11 unit tests for sqlite3Fts5StorageSize
// This test suite exercises the sqlite3Fts5StorageSize method from fts5_storage.c
// without relying on GoogleTest. It uses a minimal test harness with non-terminating
// assertions so all tests run even if some fail.

// Note: This test assumes the presence of the project's internal headers (e.g., fts5Int.h)
// and the SQLite compatibility layer (sqlite3.h). It uses the real types (Fts5Storage,
// Fts5Config, i64) as defined by the project to avoid mismatches with the production code.

#include <iostream>
#include <fts5Int.h>


// Bring in the project’s C declarations. The headers are C code; we wrap in extern "C".
extern "C" {
  // Include internal FTS5 types used by sqlite3Fts5StorageSize
  // The actual path may vary; adjust as needed for your build system.
  #include "fts5Int.h"

  // Declaration of the function under test (C linkage)
  int sqlite3Fts5StorageSize(Fts5Storage *p, int iCol, i64 *pnToken);

  // SQLITE constants are defined by SQLite; ensure you have sqlite3.h available.
  // If your test environment provides a different mechanism, adapt accordingly.
  #include "sqlite3.h"
}

// Simple non-terminating test harness
static int g_pass = 0;
static int g_fail = 0;

#define EXPECT_EQ(A, B) \
  do { \
    if ((A) == (B)) { ++g_pass; } \
    else { \
      ++g_fail; \
      std::cerr << "[FAIL] " #A " == " #B " expected: " << (B) << " got: " << (A) << "\n"; \
    } \
  } while(0)

#define EXPECT_TRUE(X) \
  do { \
    if (X) { ++g_pass; } \
    else { \
      ++g_fail; \
      std::cerr << "[FAIL] " #X " is true\n"; \
    } \
  } while(0)

#define EXPECT_NEQ(A, B) \
  do { \
    if ((A) != (B)) { ++g_pass; } \
    else { \
      ++g_fail; \
      std::cerr << "[FAIL] " #A " != " #B " expected (not equal), but both are " << (A) << "\n"; \
    } \
  } while(0)

// Helper to print a test header
static void printHeader(const char* name) {
  std::cout << "\n=== Test: " << name << " ===\n";
}

// Test 1: Negative iCol (-1) should trigger summation over all columns.
//         RC should be SQLITE_OK, and the token should be the sum of aTotalSize.
static void test_sqlite3Fts5StorageSize_allColumns() {
  printHeader("all columns (iCol = -1) sums total sizes");

  // Build a minimal Fts5Storage with 3 columns
  Fts5Config config;
  config.nCol = 3;

  // aTotalSize corresponding to each column
  i64 totals[3] = { 10, 20, 30 };  // sum = 60

  Fts5Storage storage;
  storage.pConfig = &config;
  storage.aTotalSize = totals;

  i64 token = 0;
  int rc = sqlite3Fts5StorageSize(&storage, -1, &token);

  // Expect: RC_OK and token equals 60
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(token, 60);
}

// Test 2: iCol within range should return the size of that single column.
//         We'll test a couple of representative indices to cover the path.
static void test_sqlite3Fts5StorageSize_singleColumn() {
  printHeader("single column (iCol in range) returns aTotalSize[iCol]");

  Fts5Config config;
  config.nCol = 3;

  i64 totals[3] = { 7, 11, 13 }; // values for each column

  Fts5Storage storage;
  storage.pConfig = &config;
  storage.aTotalSize = totals;

  i64 token = -1;
  int rc = sqlite3Fts5StorageSize(&storage, 0, &token);
  // Expect: RC_OK and token equals totals[0] = 7
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(token, 7);

  token = -1;
  rc = sqlite3Fts5StorageSize(&storage, 2, &token);
  // Expect: RC_OK and token equals totals[2] = 13
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(token, 13);
}

// Test 3: iCol out of range (iCol >= nCol) should yield SQLITE_RANGE
//         and token should be left in a known state (the code sets *pnToken to 0
//         when rc == SQLITE_OK, but when rc != SQLITE_OK the value may be unchanged;
//         the implementation sets rc to SQLITE_RANGE and does not alter token afterwards).
static void test_sqlite3Fts5StorageSize_outOfRange() {
  printHeader("out of range iCol (> nCol-1) returns SQLITE_RANGE");

  Fts5Config config;
  config.nCol = 3;

  i64 totals[3] = { 1, 2, 3 };

  Fts5Storage storage;
  storage.pConfig = &config;
  storage.aTotalSize = totals;

  i64 token = 999; // non-zero initial to observe any change
  int rc = sqlite3Fts5StorageSize(&storage, 3, &token);

  // We expect SQLITE_RANGE and token may remain unchanged or be set before; ensure RC is correct
  EXPECT_EQ(rc, SQLITE_RANGE);
  // Do not assert token value here as behavior after out-of-range path is not guaranteed to modify it
}

// Entry point
int main() {
  // Run tests
  test_sqlite3Fts5StorageSize_allColumns();
  test_sqlite3Fts5StorageSize_singleColumn();
  test_sqlite3Fts5StorageSize_outOfRange();

  // Summary
  std::cout << "\nTest Summary: " << g_pass << " passed, " << g_fail << " failed.\n";
  // Return non-zero if any test failed to indicate issues to a caller
  return (g_fail == 0) ? 0 : 1;
}