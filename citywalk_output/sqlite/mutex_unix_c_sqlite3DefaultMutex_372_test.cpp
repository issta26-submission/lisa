// Unit test suite for sqlite3DefaultMutex in mutex_unix.c
// Target: C++11, no GTest, a lightweight test harness with non-terminating checks.
// This test focuses on the focal function and its dependencies as described.
// Notes:
// - The test is designed to be compiled in two configurations:
//   1) SQLITE_DEBUG defined (to exercise xHeld and xNotheld paths).
//   2) SQLITE_DEBUG not defined (to exercise the 0,0 placeholders).
// - To cover both branches, compile this same file twice with/without -DSQLITE_DEBUG.
// - We do not call through the function pointers (they may be dummy in this test build);
//   instead we validate that the pointers exist and have expected non-null values
//   where applicable, and that the static nature of the returned struct is preserved.

#include <cstddef>
#include <iostream>
#include <sqliteInt.h>
#include <pthread.h>


// Include SQLite internal type definitions to inspect sqlite3_mutex_methods.
// This header is part of the SQLite source (path may vary in your project).

// Declare the focal C API with C linkage for the C++ test code.
extern "C" {
  // The actual function signature in mutex_unix.c
  sqlite3_mutex_methods const *sqlite3DefaultMutex(void);
}

// Lightweight test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion helper: logs failure but continues
#define CHECK(cond, msg) do { \
  ++g_total_tests; \
  if (!(cond)) { \
    ++g_failed_tests; \
    std::cerr << "[TEST FAILED] " << (msg) \
              << " (line " << __LINE__ << ")" << std::endl; \
  } \
} while (0)

// Test 1: Basic properties of sqlite3DefaultMutex
// - Returns a non-null pointer
// - Returns the same pointer on subsequent calls (static object semantics)
// - Exposes function pointers for core mutex operations (non-nullized)
// - Verifies presence of xHeld/xNotheld in debug builds and null placeholders otherwise
static void test_sqlite3DefaultMutex_basic() {
  // Obtain the mutex method table from the focal function
  const sqlite3_mutex_methods *m1 = sqlite3DefaultMutex();
  const sqlite3_mutex_methods *m2 = sqlite3DefaultMutex();

  // Basic sanity checks
  CHECK(m1 != nullptr, "sqlite3DefaultMutex() should not return null");
  CHECK(m1 == m2, "sqlite3DefaultMutex() should return the same pointer on subsequent calls");

  // Core function pointers should be non-null
  // (these are the addresses of the static functions defined in mutex_unix.c when built with SQLITE_MUTEX_PTHREADS)
  CHECK(m1->xInit != nullptr, "xInit should be non-null");
  CHECK(m1->xEnd != nullptr, "xEnd should be non-null");
  CHECK(m1->xAlloc != nullptr, "xAlloc should be non-null");
  CHECK(m1->xFree != nullptr, "xFree should be non-null");
  CHECK(m1->xEnter != nullptr, "xEnter should be non-null");
  CHECK(m1->xTry != nullptr, "xTry should be non-null");
  CHECK(m1->xLeave != nullptr, "xLeave should be non-null");

  // Conditional checks based on whether SQLITE_DEBUG is defined
#ifdef SQLITE_DEBUG
  // In debug mode, the last two pointers should be non-null
  CHECK(m1->xHeld != nullptr, "xHeld should be non-null when SQLITE_DEBUG is defined");
  CHECK(m1->xNotheld != nullptr, "xNotheld should be non-null when SQLITE_DEBUG is defined");
#else
  // In non-debug mode, the last two pointers should be null (0)
  CHECK(m1->xHeld == nullptr, "xHeld should be null when SQLITE_DEBUG is not defined");
  CHECK(m1->xNotheld == nullptr, "xNotheld should be null when SQLITE_DEBUG is not defined");
#endif
}

// Entry point
int main() {
  // Run tests
  test_sqlite3DefaultMutex_basic();

  // Summary
  std::cout << "Total tests executed: " << g_total_tests << std::endl;
  std::cout << "Total tests failed: "   << g_failed_tests << std::endl;

  // Non-terminating test run: report status suitable for CI or manual inspection
  if (g_failed_tests != 0) {
    std::cerr << "One or more tests FAILED." << std::endl;
  } else {
    std::cout << "All tests PASSED." << std::endl;
  }

  // Return non-zero if any test failed
  return (g_failed_tests == 0) ? 0 : 1;
}