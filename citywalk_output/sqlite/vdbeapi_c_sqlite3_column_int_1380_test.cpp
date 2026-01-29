// Unit test suite for the focal method: sqlite3_column_int
// This test is a self-contained C++11 harness that mirrors the essential behavior
// of sqlite3_column_int by providing minimal, controlled stand-ins for the
// surrounding SQLite-like environment. It does not rely on Google Test.
// Explanatory comments accompany each unit test to describe the intent and coverage.

#include <vector>
#include <opcodes.h>
#include <cstddef>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Domain-adapted minimal reproducer for the focal function's dependencies.
// This is intentionally lightweight and self-contained to allow focused unit tests.

// In the real project, sqlite3_column_int takes:
//   int sqlite3_column_int(sqlite3_stmt *pStmt, int i)
// and internally calls:
//   int val = sqlite3_value_int( columnMem(pStmt,i) );
//   columnMallocFailure(pStmt);
//   return val;
//
// For testing purposes we implement a small, self-contained replica of this logic
// with the following simplified types:

// Mimic of sqlite3_value (simplified)
struct sqlite3_value {
  int i; // value used by sqlite3_value_int in this test harness
};

// Simplified Mem type; layout matches sqlite3_value for our test purposes.
// In the real SQLite code Mem is richer, but the test only relies on 'i'.
struct Mem {
  int i;
};

// Forward declaration of the focal function (as implemented below in this test).
int sqlite3_column_int(struct sqlite3_stmt *pStmt, int i);

// Forward declaration of the "dependencies" used by sqlite3_column_int.
// We implement these inline in this test to avoid external linkage concerns.
static Mem* columnMem(struct sqlite3_stmt *pStmt, int i);
static void columnMallocFailure(struct sqlite3_stmt *pStmt);
static int sqlite3_value_int(struct sqlite3_value *pVal);

// Simple sqlite3_stmt replacement containing a vector of Mem to simulate columns.
struct sqlite3_stmt {
  std::vector<Mem> aMem; // per-column memory/value area
  // Additional fields from the real structure are unnecessary for this test
};

// Global counter to observe that columnMallocFailure() is invoked.
static int g_columnMallocFailure_calls = 0;

// Implementation of the dependencies (inline for test purposes)

// Returns a pointer to the i-th Mem in the statement, or nullptr if out of range.
static Mem* columnMem(struct sqlite3_stmt *pStmt, int i){
  if (pStmt == nullptr) return nullptr;
  if (i < 0 || i >= (int)pStmt->aMem.size()) return nullptr;
  return &pStmt->aMem[i];
}

// Records that a memory-allocation failure potential path was taken.
// In the real code, this would trigger memory-allocation related handling.
static void columnMallocFailure(struct sqlite3_stmt *pStmt){
  (void)pStmt; // unused in this minimal test harness
  g_columnMallocFailure_calls++;
}

// Simple translator for sqlite3_value_int within this test harness.
// Returns 0 if pVal is nullptr; otherwise returns the contained integer.
static int sqlite3_value_int(struct sqlite3_value *pVal){
  if (pVal == nullptr) return 0;
  return pVal->i;
}

// The focal method under test (reproduced in this isolated harness).
// int sqlite3_column_int(sqlite3_stmt *pStmt, int i){
//
//   int val = sqlite3_value_int( columnMem(pStmt,i) );
//   columnMallocFailure(pStmt);
//   return val;
// }
int sqlite3_column_int(struct sqlite3_stmt *pStmt, int i){
  int val = sqlite3_value_int( columnMem(pStmt,i) );
  columnMallocFailure(pStmt);
  return val;
}

// Simple non-terminating test harness (no GTest). Uses basic macros for assertions.

static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(actual, expected) \
  do { \
    ++g_tests_run; \
    if ((actual) != (expected)) { \
      std::cerr << "Test " << g_tests_run << " FAILED: Expected " << (expected) \
                << " but got " << (actual) << std::endl; \
      ++g_tests_failed; \
    } \
  } while (0)

#define EXPECT_TRUE(cond) \
  do { \
    ++g_tests_run; \
    if (!(cond)) { \
      std::cerr << "Test " << g_tests_run << " FAILED: Condition is false" << std::endl; \
      ++g_tests_failed; \
    } \
  } while (0)

#define TEST(label) \
  std::cout << "Running test: " << label << std::endl;

// Helper to reset global test state between tests
static void reset_globals(){
  g_columnMallocFailure_calls = 0;
}

// Test 1: Normal path - value present at column 0; ensure correct value is returned
// and that columnMallocFailure is invoked exactly once.
static void test_basic_normal_path(){
  TEST("test_basic_normal_path: normal value retrieval and malloc failure hook");
  reset_globals();

  sqlite3_stmt stmt;
  // Prepare two columns: first holds value 42, second unused for this test
  stmt.aMem.resize(2);
  stmt.aMem[0].i = 42;
  stmt.aMem[1].i = 7; // not used in this test

  int result = sqlite3_column_int(&stmt, 0);

  EXPECT_EQ(result, 42);
  EXPECT_EQ(g_columnMallocFailure_calls, 1);
}

// Test 2: Out-of-bounds index - i is invalid; columnMem should return nullptr,
// sqlite3_value_int(nullptr) should yield 0, but the malloc-failure hook should still run.
static void test_out_of_bounds_index(){
  TEST("test_out_of_bounds_index: out-of-bounds index handling");
  reset_globals();

  sqlite3_stmt stmt;
  stmt.aMem.resize(2);
  stmt.aMem[0].i = 10;
  stmt.aMem[1].i = -5;

  int result = sqlite3_column_int(&stmt, 5); // out of range

  EXPECT_EQ(result, 0);
  EXPECT_EQ(g_columnMallocFailure_calls, 1);
}

// Test 3: Modify existing cell value and fetch again - verify new value is returned
// and that the malloc-failure hook is invoked again.
static void test_value_change(){
  TEST("test_value_change: dynamic value retrieval after modification");
  reset_globals();

  sqlite3_stmt stmt;
  stmt.aMem.resize(2);
  stmt.aMem[0].i = 123;
  stmt.aMem[1].i = -999;

  int first = sqlite3_column_int(&stmt, 0);
  // Change the underlying Mem value to a new one
  stmt.aMem[0].i = -77;

  int second = sqlite3_column_int(&stmt, 0);

  EXPECT_EQ(first, 123);
  EXPECT_EQ(second, -77);
  // Each call to sqlite3_column_int invokes columnMallocFailure, so total should be 2
  EXPECT_EQ(g_columnMallocFailure_calls, 2);
}

// Test 4: Null statement pointer - ensure we handle null pStmt gracefully
// (In this minimal harness, columnMem will return nullptr and sqlite3_value_int(nullptr) -> 0)
static void test_null_stmt_pointer(){
  TEST("test_null_stmt_pointer: handling null pStmt");
  reset_globals();

  int result = sqlite3_column_int(nullptr, 0);

  // If pStmt is null, columnMem cannot be used; sqlite3_value_int(nullptr) -> 0
  // columnMallocFailure should still be invoked (our test harness calls it regardless)
  EXPECT_EQ(result, 0);
  EXPECT_EQ(g_columnMallocFailure_calls, 1);
}

// Main entry point to run all tests
int main(){
  std::cout << "Starting sqlite3_column_int unit tests (focal method)..." << std::endl;

  test_basic_normal_path();
  test_out_of_bounds_index();
  test_value_change();
  test_null_stmt_pointer();

  if (g_tests_failed == 0){
    std::cout << "All tests PASSED (" << g_tests_run << " tests)." << std::endl;
    return 0;
  }else{
    std::cerr << g_tests_failed << " tests FAILED out of " << g_tests_run << " total." << std::endl;
    return 1;
  }
}