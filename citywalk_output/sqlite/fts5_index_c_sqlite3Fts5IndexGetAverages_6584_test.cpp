// Unit tests for sqlite3Fts5IndexGetAverages using a lightweight C++ test harness
// This test suite is designed to run without GoogleTest and uses non-terminating assertions.
// It mocks minimal environment to exercise the function under test in a controlled way.

// Assumptions:
// - The project headers (e.g., fts5Int.h) are available in the include path.
// - sqlite3Fts5IndexGetAverages is accessible from C/C++ (extern "C").
// - For testing purposes, we provide lightweight mock implementations for a subset
//   of dependent functions that sqlite3Fts5IndexGetAverages relies on.
// - Static functions inside the original fts5_index.c are not overridden here; tests
//   target the observable behavior of sqlite3Fts5IndexGetAverages given controlled inputs.

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <fts5Int.h>


// Lightweight macro shims for test-only constants
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_ERROR
#define SQLITE_ERROR 1
#endif

// Forward declare types and functions from the project.
// These headers must be on the include path when compiling tests.
extern "C" {
  // The actual project header defines i64, Fts5Index, Fts5Config, Fts5Data, etc.
  #include "fts5Int.h"
}

// If not provided by the headers, ensure basic types exist for test compilation.
#ifndef FTS5_AVERAGES_ROWID
#define FTS5_AVERAGES_ROWID 0
#endif

// We rely on the function under test having C linkage.
extern "C" int sqlite3Fts5IndexGetAverages(Fts5Index *p, i64 *pnRow, i64 *anSize);

// Mocked or test-provided dependencies (provided with C linkage)
typedef unsigned char u8;
typedef unsigned long long u64;

// Global state used by mocks to drive test scenarios
static i64 testNN;                 // Number of per-column varints in the averages data
static unsigned char testBytes[256]; // Raw varint bytes representing values after the first one
static Fts5Data testData;           // Mocked Fts5Data structure used by fts5DataRead

// Mock implementation of fts5DataRead used by sqlite3Fts5IndexGetAverages
extern "C" Fts5Data* fts5DataRead(Fts5Index *p, i64 iRowid) {
  // Return a pointer to our testData, populated by the test harness
  testData.nn = (int)testNN;
  testData.p = testBytes;
  return &testData;
}

// Mock implementation of fts5DataRelease (no-op for tests)
extern "C" void fts5DataRelease(Fts5Data *pData) {
  // No resources to release in the mock
  (void)pData;
}

// Mock implementation of fts5GetVarint
// Simple 1-byte varint reader for test purposes: reads a single byte as value
extern "C" int fts5GetVarint(const u8 *p, u64 *pVal) {
  if(p == nullptr || pVal == nullptr) return 0;
  *pVal = (u64)(*p);
  return 1; // bytes consumed
}

// Mock implementation of fts5IndexReturn
// Returns the rc field of the index (simulating the real function's behavior)
extern "C" int fts5IndexReturn(Fts5Index *p) {
  if(p == nullptr) return SQLITE_ERROR;
  return p->rc;
}

// ------------------- Test Harness -------------------

static int g_failures = 0;

// Simple non-terminating assertions
#define TEST_ASSERT_TRUE(cond, msg) \
  do { if(!(cond)) { ++g_failures; printf("FAIL: %s\n", msg); } else { printf("PASS: %s\n", msg); } } while(0)

#define TEST_ASSERT_EQ(a, b, msg) \
  do { if((a) != (b)) { ++g_failures; printf("FAIL: %s (expected %lld, got %lld)\n", msg, (long long)(b), (long long)(a)); } else { printf("PASS: %s\n", msg); } } while(0)

#define TEST_CASE(name) void name()

static void run_all_tests();

// Test 1: Basic success path - p->rc == SQLITE_OK and averages contain nn values for nCol columns
TEST_CASE(test_get_averages_basic_success) {
  // Arrange
  Fts5Index idx;
  Fts5Config cfg;
  // Initialize structures (values are representative for the test)
  memset(&idx, 0, sizeof(Fts5Index));
  memset(&cfg, 0, sizeof(Fts5Config));

  // We pretend there are 2 columns
  cfg.nCol = 2;
  idx.pConfig = &cfg;
  idx.rc = SQLITE_OK;

  // Prepare mock data: pnRow = 5, anSize[0] = 7, anSize[1] = 11
  testNN = 2;                // two per-column values
  testBytes[0] = 5;          // pnRow = 5
  testBytes[1] = 7;          // anSize[0] = 7
  testBytes[2] = 11;         // anSize[1] = 11

  i64 row = 0;
  i64 sizes[4] = {0}; // enough space for 4 columns; we will use first 2

  // Act
  int rc = sqlite3Fts5IndexGetAverages(&idx, &row, sizes);

  // Assert
  TEST_ASSERT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK in success path");
  TEST_ASSERT_EQ(row, 5, "pnRow should be read as 5");
  TEST_ASSERT_EQ(sizes[0], 7, "anSize[0] should be 7");
  TEST_ASSERT_EQ(sizes[1], 11, "anSize[1] should be 11");
}

// Test 2: When p->rc is not SQLITE_OK, function should return the error and zero outputs
TEST_CASE(test_get_averages_rc_not_ok) {
  // Arrange
  Fts5Index idx;
  Fts5Config cfg;
  memset(&idx, 0, sizeof(Fts5Index));
  memset(&cfg, 0, sizeof(Fts5Config));
  cfg.nCol = 2;
  idx.pConfig = &cfg;
  idx.rc = SQLITE_ERROR; // not OK

  testNN = 2;
  testBytes[0] = 9; // pnRow would be 9 if read, but rc will short-circuit
  testBytes[1] = 1;
  testBytes[2] = 2;

  i64 row = 0;
  i64 sizes[2];
  memset(sizes, 0, sizeof(sizes));

  // Act
  int rc = sqlite3Fts5IndexGetAverages(&idx, &row, sizes);

  // Assert: outputs should remain zero, only rc indicates error
  TEST_ASSERT_EQ(rc, SQLITE_ERROR, "rc should propagate SQLITE_ERROR when p->rc != SQLITE_OK");
  TEST_ASSERT_EQ(row, 0, "pnRow should remain 0 when rc != OK");
  TEST_ASSERT_TRUE(sizes[0] == 0 && sizes[1] == 0, "anSize values should remain zero when rc != OK");
}

// Test 3: When averages row data indicates nn == 0, sizes should remain zero
TEST_CASE(test_get_averages_nn_zero) {
  // Arrange
  Fts5Index idx;
  Fts5Config cfg;
  memset(&idx, 0, sizeof(Fts5Index));
  memset(&cfg, 0, sizeof(Fts5Config));
  cfg.nCol = 3;
  idx.pConfig = &cfg;
  idx.rc = SQLITE_OK;

  testNN = 0; // no per-column averages
  testBytes[0] = 4; // pnRow would be read if executed
  // No per-column values follow

  i64 row = 0;
  i64 sizes[4];
  memset(sizes, 0, sizeof(sizes));

  // Act
  int rc = sqlite3Fts5IndexGetAverages(&idx, &row, sizes);

  // Assert
  TEST_ASSERT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK when averages present but nn == 0");
  TEST_ASSERT_EQ(row, 4, "pnRow should be read as 4");
  for (int i=0; i<3; ++i) {
    TEST_ASSERT_EQ(sizes[i], 0, "anSize[i] should be 0 when nn == 0");
  }
}

// Test 4: When nCol > nn, function should read up to nn and ignore the rest
TEST_CASE(test_get_averages_nn_less_than_ncol) {
  // Arrange
  Fts5Index idx;
  Fts5Config cfg;
  memset(&idx, 0, sizeof(Fts5Index));
  memset(&cfg, 0, sizeof(Fts5Config));
  cfg.nCol = 3; // 3 columns, but we'll provide only 2 values after pnRow
  idx.pConfig = &cfg;
  idx.rc = SQLITE_OK;

  testNN = 2;
  testBytes[0] = 12; // pnRow
  testBytes[1] = 21; // anSize[0]
  testBytes[2] = 22; // anSize[1]
  // No third per-column value present

  i64 row = 0;
  i64 sizes[3];
  memset(sizes, 0, sizeof(sizes));

  // Act
  int rc = sqlite3Fts5IndexGetAverages(&idx, &row, sizes);

  // Assert: sizes[2] should remain 0 since nn == 2
  TEST_ASSERT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK when partial data is available");
  TEST_ASSERT_EQ(row, 12, "pnRow should be read as 12");
  TEST_ASSERT_EQ(sizes[0], 21, "anSize[0] should be 21");
  TEST_ASSERT_EQ(sizes[1], 22, "anSize[1] should be 22");
  TEST_ASSERT_EQ(sizes[2], 0, "anSize[2] should remain 0 when not provided (nn < nCol)");
}

// ------------------- Test Runner -------------------
static void run_all_tests() {
  printf("Running sqlite3Fts5IndexGetAverages unit tests...\n");
  test_get_averages_basic_success();
  test_get_averages_rc_not_ok();
  test_get_averages_nn_zero();
  test_get_averages_nn_less_than_ncol();
  if (g_failures == 0) {
    printf("ALL TESTS PASSED\n");
  } else {
    printf("TOTAL FAILURES: %d\n", g_failures);
  }
}

// Main entry point
int main(void) {
  run_all_tests();
  return (g_failures == 0) ? 0 : 1;
}