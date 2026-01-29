// This test suite targets the focal method:
// i16 sqlite3TableColumnToIndex(Index *pIdx, i16 iCol)
// It expects an Index-like structure with fields:
//   - int nColumn
//   - i16 *aiColumn  (i16 is a 16-bit integer type alias used by the SQLite codebase)
//
// The tests are written in C++11 (no Google Test or external mocking framework).
// We rely on linking against the actual implementation of sqlite3TableColumnToIndex
// (assumed to be provided in the build.c / sqlite3 source in the test environment).
//
// Design notes (Domain Knowledge considerations):
// - Use non-terminating assertions: tests report PASS/FAIL but continue execution.
// - Do not rely on private/internal helpers of the focal module; only use the exported function.
// - Static functions in the source file are not directly testable; we focus on the public behavior.
// - Tests are written in plain C++ with minimal standard library usage to maximize portability.

#include <iostream>
#include <sqliteInt.h>


// Domain-specific type alias used by the focal method.
// We mirror the minimal required type to compile with the production signature.
typedef short i16;

// Minimal reproduction of the Index type as expected by the focal function.
// This layout must match the production layout for correct access of nColumn and aiColumn.
typedef struct {
  int nColumn;
  i16 *aiColumn;
  // Other members exist in the full SQLite implementation, but are not required
  // for the sqlite3TableColumnToIndex behavior tested here.
} Index;

// Declaration of the focal function with C linkage.
// We assume the production code provides this symbol (from build.c / sqlite3.c).
extern "C" i16 sqlite3TableColumnToIndex(Index *pIdx, i16 iCol);

// Simple non-terminating test harness (collects and reports results).
static int g_failures = 0;
#define TEST_EQ(expected, actual, descr) do {                                 \
  if ((expected) != (actual)) {                                                \
    std::cerr << "FAIL: " << descr                                               \
              << " | expected: " << static_cast<long long>(expected)            \
              << "  actual: " << static_cast<long long>(actual) << std::endl; \
    ++g_failures;                                                               \
  } else {                                                                       \
    std::cout << "PASS: " << descr                                               \
              << std::endl;                                                    \
  }                                                                              \
} while(0)

// Test 1: Empty index should always yield -1 for any iCol.
static void test_empty_index() {
  // Scenario: zero columns, no valid aiColumn entries.
  Index idx;
  idx.nColumn = 0;
  idx.aiColumn = nullptr;

  i16 res = sqlite3TableColumnToIndex(&idx, (i16)0);
  TEST_EQ((i16)-1, res, "sqlite3TableColumnToIndex returns -1 for empty index");
}

// Test 2: Basic positive lookups and a negative (not found) case.
static void test_basic_find() {
  // Scenario:
  // aiColumn = [0, 2, 4]
  // nColumn = 3
  // Expected: iCol 0 -> 0, 2 -> 1, 4 -> 2, 3 (not present) -> -1
  i16 arr[3] = {0, 2, 4};
  Index idx;
  idx.nColumn = 3;
  idx.aiColumn = arr;

  TEST_EQ((i16)0, sqlite3TableColumnToIndex(&idx, (i16)0),
          "first element 0 should map to index 0");
  TEST_EQ((i16)1, sqlite3TableColumnToIndex(&idx, (i16)2),
          "second element 2 should map to index 1");
  TEST_EQ((i16)2, sqlite3TableColumnToIndex(&idx, (i16)4),
          "third element 4 should map to index 2");
  TEST_EQ((i16)-1, sqlite3TableColumnToIndex(&idx, (i16)3),
          "absent value 3 should return -1");
}

// Test 3: Duplicates in the column array should return the first occurrence.
static void test_duplicates() {
  // Scenario: aiColumn = [5, 5, 7], nColumn = 3
  // For iCol = 5, should return index 0 (first occurrence)
  i16 arr[3] = {5, 5, 7};
  Index idx;
  idx.nColumn = 3;
  idx.aiColumn = arr;

  TEST_EQ((i16)0, sqlite3TableColumnToIndex(&idx, (i16)5),
          "duplicate value should return first index (0)");
}

// Test 4: Negative values and edge cases in iCol.
static void test_negative_values() {
  // Scenario: aiColumn = [-1, 1, -2], nColumn = 3
  // Map -1 -> 0, -2 -> 2, and a value not present (e.g., 0) should be -1
  i16 arr[3] = { (i16)-1, (i16)1, (i16)-2 };
  Index idx;
  idx.nColumn = 3;
  idx.aiColumn = arr;

  TEST_EQ((i16)0, sqlite3TableColumnToIndex(&idx, (i16)-1),
          "negative -1 should map to index 0");
  TEST_EQ((i16)2, sqlite3TableColumnToIndex(&idx, (i16)-2),
          "negative -2 should map to index 2");
  TEST_EQ((i16)-1, sqlite3TableColumnToIndex(&idx, (i16)0),
          "absent value 0 should return -1");
}

// Runner
int main() {
  std::cout << "Starting tests for sqlite3TableColumnToIndex..." << std::endl;

  test_empty_index();
  test_basic_find();
  test_duplicates();
  test_negative_values();

  if (g_failures == 0) {
    std::cout << "All tests passed." << std::endl;
  } else {
    std::cout << g_failures << " test(s) failed." << std::endl;
  }

  return g_failures; // non-zero indicates failure to the runner
}