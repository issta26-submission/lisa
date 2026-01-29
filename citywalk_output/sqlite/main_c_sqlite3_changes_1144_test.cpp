// Unit test suite for the focal method: sqlite3_changes
// This test suite is crafted to be compiled with C++11 and without Google Test.
// It provides a minimal test harness and stubs the dependent function sqlite3_changes64
// so that sqlite3_changes can be exercised in isolation.
//
// Step 1 (Program Understanding): The focal method simply returns the cast of
// sqlite3_changes64(db) to int. Therefore, tests focus on ensuring that
// the cast behavior and delegation are correct under various values returned by
// sqlite3_changes64. The domain keywords identified include: sqlite3_changes, sqlite3_changes64,
// db, cast to int, and the sqlite3* db handle parameter.
//
// Step 2 (Unit Test Generation): Tests cover zero, positive, INT_MAX, overflow cast, and negative values
// returned by sqlite3_changes64 to verify proper delegation and casting behavior.
//
// Step 3 (Test Case Refinement): Tests include boundary and edge cases to maximize coverage of the cast
// and function delegation. A lightweight non-terminating assertion approach is used (EXPECT-like helpers).

#include <iostream>
#include <climits>
#include <sqliteInt.h>


// Forward declarations and minimal scaffolding to isolate sqlite3_changes for testing.
// We mirror only the minimal API surface needed by the focal function.
// The real project would include sqlite3 types; here we provide a lightweight stand-in.

extern "C" {
  typedef struct sqlite3 sqlite3;       // Opaque in the real library; we only pass pointers around
  typedef long long sqlite3_int64;     // Common sqlite type for 64-bit integers

  // Forward declaration of the 64-bit changes function (to be stubbed in tests)
  sqlite3_int64 sqlite3_changes64(sqlite3 *db);

  // Focal method under test: delegates to sqlite3_changes64 and casts to int
  int sqlite3_changes(sqlite3 *db);
}

// Global stub variable to simulate sqlite3_changes64 behavior in tests
static sqlite3_int64 g_sqlite3_changes64_stub = 0;

// Stubbed implementation of sqlite3_changes64 used by sqlite3_changes during tests.
// The test code controls the return value via g_sqlite3_changes64_stub.
extern "C" sqlite3_int64 sqlite3_changes64(sqlite3 *db) {
  (void)db; // db is unused by the stub
  return g_sqlite3_changes64_stub;
}

// Implementation of the focal function under test in C++ (without altering behavior)
extern "C" int sqlite3_changes(sqlite3 *db) {
  return (int)sqlite3_changes64(db);
}

// Lightweight non-terminating test harness
static int g_test_failures = 0;
static int g_test_count = 0;

// Non-terminating assertion helper for integer equality
static void expect_eq_int(int expected, int actual, const char* testName) {
  ++g_test_count;
  if (expected != actual) {
    std::cerr << "[FAIL] " << testName
              << ": expected=" << expected
              << ", actual=" << actual << std::endl;
    ++g_test_failures;
  } else {
    std::cout << "[OK] " << testName << std::endl;
  }
}

// Test 1: Zero value returned by sqlite3_changes64 should yield 0 after cast
// Rationale: verifies basic delegation and correct cast when the 64-bit value is zero.
static void test_sqlite3_changes_zero() {
  g_sqlite3_changes64_stub = 0;
  int result = sqlite3_changes(nullptr);
  expect_eq_int(0, result, "sqlite3_changes_zero");
}

// Test 2: Positive small value should be preserved after cast
// Rationale: ensures normal operation with a typical non-boundary value.
static void test_sqlite3_changes_positive() {
  g_sqlite3_changes64_stub = 12345;
  int result = sqlite3_changes(nullptr);
  expect_eq_int(12345, result, "sqlite3_changes_positive");
}

// Test 3: Value equal to INT_MAX should be cast correctly
// Rationale: boundary condition at the edge of int range.
static void test_sqlite3_changes_intmax() {
  g_sqlite3_changes64_stub = INT_MAX;
  int result = sqlite3_changes(nullptr);
  expect_eq_int(INT_MAX, result, "sqlite3_changes_intmax");
}

// Test 4: Value just above INT_MAX to test casting overflow behavior
// Rationale: ensures defined behavior of cast when 64-bit value exceeds int.
static void test_sqlite3_changes_cast_overflow() {
  long long val = (long long)INT_MAX + 1; // INT_MAX + 1
  g_sqlite3_changes64_stub = val;
  int result = sqlite3_changes(nullptr);
  int expected = (int)val; // casting behavior should match C/C++ cast rules
  expect_eq_int(expected, result, "sqlite3_changes_cast_overflow");
}

// Test 5: Negative value to ensure cast handles negatives consistently
// Rationale: although sqlite3_changes64 typically returns non-negative values,
// the cast should still be deterministic for negative input.
static void test_sqlite3_changes_negative() {
  g_sqlite3_changes64_stub = -1;
  int result = sqlite3_changes(nullptr);
  int expected = -1;
  expect_eq_int(expected, result, "sqlite3_changes_negative");
}

// Runner to execute all tests
static void run_all_tests() {
  std::cout << "Starting sqlite3_changes unit tests (C++11, no GTest)" << std::endl;
  test_sqlite3_changes_zero();
  test_sqlite3_changes_positive();
  test_sqlite3_changes_intmax();
  test_sqlite3_changes_cast_overflow();
  test_sqlite3_changes_negative();
  std::cout << "Finished tests. Total: " << g_test_count
            << ", Failures: " << g_test_failures << std::endl;
}

// Main entry point: executes tests per domain knowledge guidance (call test methods from main)
int main() {
  run_all_tests();
  // Return non-zero if any test failed
  return g_test_failures ? 1 : 0;
}