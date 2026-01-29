// Unit test suite for the focal method: sqlite3_bind_int
// This test suite is written for C++11, without using GTest.
// It mocks the dependent sqlite3_bind_int64 behavior to verify
// that sqlite3_bind_int forwards correctly and casts values as intended.

// Test goals aligned with Step 2/Step 3 guidance:
// - Validate that sqlite3_bind_int simply forwards to sqlite3_bind_int64
// - Validate correct casting from int to i64 (signed 64-bit)
// - Validate that the correct parameters are passed (statement pointer, index, value)
// - Validate behavior when the statement pointer is null
// - Use non-terminating assertions so all tests run and report all failures

#include <opcodes.h>
#include <iomanip>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Domain knowledge and compatibility notes:
// - We implement a minimal i64 alias to simulate sqlite3_int64 used by the code under test.
// - We define a minimal sqlite3_stmt structure to represent statement handles.
// - We provide a concrete implementation of sqlite3_bind_int64 to capture inputs.
// - We provide sqlite3_bind_int (the focal method) exactly as in the source snippet.

typedef long long i64; // Simulated sqlite3_int64

// Minimal statement type to satisfy the function signatures.
struct sqlite3_stmt {};

// Global test harness state to capture calls to sqlite3_bind_int64
static sqlite3_stmt *g_last_stmt = nullptr;
static int g_last_i = 0;
static i64 g_last_value = 0;
static int g_bind_int64_calls = 0;

// Mock implementation of sqlite3_bind_int64 to observe behavior.
// It records the arguments and returns a sentinel value.
int sqlite3_bind_int64(sqlite3_stmt *p, int i, i64 iValue) {
  g_last_stmt = p;
  g_last_i = i;
  g_last_value = iValue;
  ++g_bind_int64_calls;
  // Return a sentinel value to verify propagation from sqlite3_bind_int
  return 123;
}

// The focal method under test (as provided):
int sqlite3_bind_int(sqlite3_stmt *p, int i, int iValue){
  return sqlite3_bind_int64(p, i, (i64)iValue);
}

// The actual sqlite3_bind_int is a very thin wrapper around sqlite3_bind_int64.
// For completeness and to enable self-contained tests, we reproduce the exact logic above here.
// (In the real project, this would be pulled from the source file under test.)

// Simple non-terminating test harness utilities
static int total_tests = 0;
static int failed_tests = 0;

#define EXPECT_EQ(expected, actual, msg) do { \
  total_tests++; \
  if ((expected) != (actual)) { \
    std::cerr << "[TEST FAIL] " << msg << " | expected: " << (expected) \
              << ", actual: " << (actual) << std::endl; \
    ++failed_tests; \
  } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
  total_tests++; \
  if (!(cond)) { \
    std::cerr << "[TEST FAIL] " << msg << std::endl; \
    ++failed_tests; \
  } \
} while(0)

// Test 1: Ensure that sqlite3_bind_int forwards to sqlite3_bind_int64
// with the same statement, index, and a correct cast from int to i64.
// Also verify that the return value from sqlite3_bind_int64 propagates.
void test_sqlite3_bind_int_forwards_and_casts() {
  // Reset harness state
  g_last_stmt = nullptr;
  g_last_i = 0;
  g_last_value = 0;
  g_bind_int64_calls = 0;

  sqlite3_stmt s;

  int ret = sqlite3_bind_int(&s, 2, 7);

  // The wrapper should return the sentinel value from sqlite3_bind_int64
  EXPECT_EQ(123, ret, "sqlite3_bind_int should propagate return value from sqlite3_bind_int64");

  // The statement pointer should be forwarded correctly
  EXPECT_EQ(&s, g_last_stmt, "sqlite3_bind_int should forward statement pointer");

  // The index and value should be forwarded with proper casting to i64
  EXPECT_EQ(2, g_last_i, "sqlite3_bind_int should forward the parameter index");
  EXPECT_EQ((i64)7, g_last_value, "sqlite3_bind_int should cast int to i64 correctly");

  // Ensure sqlite3_bind_int64 was invoked exactly once for this call
  EXPECT_EQ(1, g_bind_int64_calls, "sqlite3_bind_int64 should be called exactly once per sqlite3_bind_int call");
}

// Test 2: Validate casting for negative values and standard integers
void test_sqlite3_bind_int_negative_and_standard_values() {
  // Reset harness state
  g_last_stmt = nullptr;
  g_last_i = 0;
  g_last_value = 0;
  g_bind_int64_calls = 0;

  sqlite3_stmt s;

  // Negative value test
  int ret_neg = sqlite3_bind_int(&s, 0, -1);
  EXPECT_EQ(123, ret_neg, "Binding a negative int should propagate return value");

  EXPECT_EQ(&s, g_last_stmt, "Statement pointer should be forwarded for negative value test");
  EXPECT_EQ(0, g_last_i, "Index should be forwarded for negative value test");
  EXPECT_EQ((i64)(-1), g_last_value, "Negative int should cast to i64 correctly");
  EXPECT_EQ(1, g_bind_int64_calls, "sqlite3_bind_int64 should be called exactly once for negative value test");

  // Reset for standard value test
  g_last_stmt = nullptr;
  g_last_i = 0;
  g_last_value = 0;
  g_bind_int64_calls = 0;

  // Standard positive value
  int ret_pos = sqlite3_bind_int(&s, 5, 2147483647); // INT_MAX
  EXPECT_EQ(123, ret_pos, "Binding INT_MAX should propagate return value");
  EXPECT_EQ(&s, g_last_stmt, "Statement pointer should be forwarded for INT_MAX test");
  EXPECT_EQ(5, g_last_i, "Index should be forwarded for INT_MAX test");
  EXPECT_EQ((i64)2147483647, g_last_value, "INT_MAX should cast to i64 correctly");
  EXPECT_EQ(1, g_bind_int64_calls, "sqlite3_bind64 should be called once for INT_MAX test");
}

// Test 3: Validate behavior when the statement pointer is null
void test_sqlite3_bind_int_null_statement() {
  // Reset harness state
  g_last_stmt = nullptr;
  g_last_i = 0;
  g_last_value = 0;
  g_bind_int64_calls = 0;

  // If p is null, the wrapper still forwards the call (as per its simple forwarding logic)
  int ret = sqlite3_bind_int(nullptr, 0, 42);

  EXPECT_EQ(123, ret, "sqlite3_bind_int should propagate return value even when statement pointer is null");
  EXPECT_EQ(nullptr, g_last_stmt, "Statement pointer should be null when passed as null");
  EXPECT_EQ(0, g_last_i, "Index should be forwarded correctly for null statement");
  EXPECT_EQ((i64)42, g_last_value, "Value should cast to i64 correctly even when statement is null");
  EXPECT_EQ(1, g_bind_int64_calls, "sqlite3_bind_int64 should be called even if statement is null");
}

// Helper to run all tests and report summary
void run_all_tests() {
  std::cout << "Running sqlite3_bind_int unit tests (C++11, no GTest)..." << std::endl;

  test_sqlite3_bind_int_forwards_and_casts();
  test_sqlite3_bind_int_negative_and_standard_values();
  test_sqlite3_bind_int_null_statement();

  std::cout << "Tests run: " << total_tests << ", Failures: " << failed_tests << std::endl;
  if (failed_tests > 0) {
    std::cout << "Some tests failed. See details above for diagnostics." << std::endl;
  } else {
    std::cout << "All tests passed." << std::endl;
  }
}

// Main entry point: run tests
int main() {
  run_all_tests();
  // Non-zero exit on any failure to aid integration with build systems
  return failed_tests != 0 ? 1 : 0;
}