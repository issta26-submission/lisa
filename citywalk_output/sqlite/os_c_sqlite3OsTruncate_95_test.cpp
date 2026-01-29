// This test suite targets the focal method:
// int sqlite3OsTruncate(sqlite3_file *id, i64 size){
//   return id->pMethods->xTruncate(id, size);
// }
// The tests are written in C++11 (no GoogleTest) and use a small custom
// harness with non-terminating assertions to maximize coverage.

#include <iostream>
#include <limits>
#include <sqliteInt.h>


// Domain knowledge notes (for maintainers):
// - Candidate Keywords: sqlite3_file, sqlite3_io_methods (pMethods), xTruncate, id, size, sqlite3OsTruncate
// - The focal method delegates to the xTruncate function pointer contained in id->pMethods.
// - We'll simulate a minimal environment (C-like structs) to validate correct delegation
//   and argument propagation without relying on the real SQLite sources.

using i64 = long long;

// Minimal stand-in structures to emulate the necessary parts of SQLite's IO layer.
struct sqlite3_file; // forward declaration

typedef int (*sqlite3TruncateFn)(sqlite3_file *id, i64 size);

struct sqlite3_io_methods {
  sqlite3TruncateFn xTruncate;
  // Other function pointers omitted (not needed for this test)
};

struct sqlite3_file {
  sqlite3_io_methods *pMethods;
};

// The function under test (reproduced for the test harness).
extern "C" int sqlite3OsTruncate(sqlite3_file *id, i64 size){
  return id->pMethods->xTruncate(id, size);
}

// Global test harness state to capture xTruncate invocations.
static sqlite3_file g_file1;
static sqlite3_file g_file2;
static sqlite3_io_methods g_methods;

static sqlite3_file *g_lastIdPtr = nullptr;
static i64 g_lastSizeVal = 0;

// First fake truncate function: returns a fixed value and records inputs.
static int fake_truncate(sqlite3_file *id, i64 size){
  g_lastIdPtr = id;
  g_lastSizeVal = size;
  return 42;
}

// Alternate fake truncate function: returns a different fixed value.
static int fake_truncate_alt(sqlite3_file *id, i64 size){
  g_lastIdPtr = id;
  g_lastSizeVal = size;
  return -7;
}

// Simple non-terminating test assertion framework (log-only).
static int g_failures = 0;

#define EXPECT_EQ(a,b,msg) do { auto _a = (a); auto _b = (b); if (!(_a == _b)) { std::cerr << "FAIL: " << (msg) << " | expected: " << _b << " actual: " << _a << "\n"; ++g_failures; } } while(0)
#define EXPECT_PTR_EQ(a,b,msg) do { auto * _pa = (a); auto * _pb = (b); if (_pa != _pb) { std::cerr << "FAIL: " << (msg) << " | expected pointer: " << _pb << " actual pointer: " << _pa << "\n"; ++g_failures; } } while(0)

// Reset the capture state before each test.
static void reset_capture() {
  g_lastIdPtr = nullptr;
  g_lastSizeVal = std::numeric_limits<i64>::min();
}

// Test 1: Basic delegation - ensure xTruncate is called with correct id and size,
// and the return value from sqlite3OsTruncate matches the xTruncate return.
static void test_sqlite3OsTruncate_basic_delegation() {
  // Arrange
  reset_capture();
  g_methods.xTruncate = fake_truncate;
  g_file1.pMethods = &g_methods;

  // Act
  int ret = sqlite3OsTruncate(&g_file1, 1234);

  // Assert
  EXPECT_EQ(ret, 42, "sqlite3OsTruncate should return the value from xTruncate (42)");
  EXPECT_PTR_EQ(g_lastIdPtr, &g_file1, "sqlite3OsTruncate should pass the correct id to xTruncate");
  EXPECT_EQ(g_lastSizeVal, 1234, "sqlite3OsTruncate should pass the correct size to xTruncate");
}

// Test 2: Zero size propagation - ensure size 0 is correctly propagated.
static void test_sqlite3OsTruncate_zero_size() {
  // Arrange
  reset_capture();
  g_methods.xTruncate = fake_truncate;
  g_file1.pMethods = &g_methods;

  // Act
  int ret = sqlite3OsTruncate(&g_file1, 0);

  // Assert
  EXPECT_EQ(ret, 42, "sqlite3OsTruncate should return 42 for this test path");
  EXPECT_PTR_EQ(g_lastIdPtr, &g_file1, "id should remain the same for zero-size test");
  EXPECT_EQ(g_lastSizeVal, 0, "size should be 0 for zero-size test");
}

// Test 3: Multiple files and alternate return - ensure correct file is passed and
// that a different xTruncate implementation can be swapped in.
static void test_sqlite3OsTruncate_multiple_files_and_alt() {
  // Arrange
  reset_capture();
  g_methods.xTruncate = fake_truncate;
  g_file1.pMethods = &g_methods;
  g_file2.pMethods = &g_methods;

  // Act on first file with original function
  int ret1 = sqlite3OsTruncate(&g_file1, 555);

  // Assert for first file
  EXPECT_EQ(ret1, 42, "First file should return 42 with original xTruncate");
  EXPECT_PTR_EQ(g_lastIdPtr, &g_file1, "First file id passed should be &g_file1");
  EXPECT_EQ(g_lastSizeVal, 555, "First file size should be 555");

  // Swap in alternate implementation
  g_methods.xTruncate = fake_truncate_alt;

  // Act on second file with alternate function
  int ret2 = sqlite3OsTruncate(&g_file2, -999);

  // Assert for second file
  EXPECT_EQ(ret2, -7, "Second file should return -7 with alternate xTruncate");
  EXPECT_PTR_EQ(g_lastIdPtr, &g_file2, "Second file id passed should be &g_file2");
  EXPECT_EQ(g_lastSizeVal, -999, "Second file size should be -999");
}

// Entry point for the tests
int main() {
  std::cout << "sqlite3OsTruncate unit tests (C++11, non-GTest) started\n";

  // Prepare a common methods structure
  g_methods.xTruncate = nullptr; // initially not set

  // Run tests
  test_sqlite3OsTruncate_basic_delegation();
  test_sqlite3OsTruncate_zero_size();
  test_sqlite3OsTruncate_multiple_files_and_alt();

  // Summary
  if (g_failures == 0) {
    std::cout << "All tests passed.\n";
  } else {
    std::cerr << g_failures << " test(s) failed.\n";
  }

  return g_failures;
}