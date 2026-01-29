// Test suite for the focal method: sqlite3WalCallback
// This test targets the C function declared in wal.c which operates on a Wal object.
// The tests are written in C++11 (no GoogleTest), using a lightweight, non-terminating assertion style.

#include <Windows.h>
#include <string>
#include <iostream>
#include <wal.h>
#include <cstdint>


// Import necessary dependencies for the Wal type and the function under test.
// We rely on the project-provided wal.h to define the Wal structure and its fields,
// including iCallback. The function sqlite3WalCallback uses Wal *pWal and manipulates iCallback.
extern "C" {
  // Forward declare the function under test with C linkage to ensure correct linkage.
  // The actual definition resides in wal.c.
  typedef struct Wal Wal;
  int sqlite3WalCallback(Wal *pWal);
}

// Lightweight test framework (non-terminating assertions)
static int g_test_failures = 0;
static int g_test_count = 0;

#define LOG_FAIL(msg) do { \
  std::cerr << "FAIL: " << __func__ << ": " << msg << " (file " << __FILE__ << ", line " << __LINE__ << ")\n"; \
} while(0)

#define EXPECT_EQ(expected, actual) do { \
  ++g_test_count; \
  if( (expected) != (actual) ) { \
    LOG_FAIL("Expected " #expected " == " #actual " but got " + std::to_string(actual) + " (actual)"); \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_TRUE(condition) do { \
  ++g_test_count; \
  if(!(condition)) { \
    LOG_FAIL("Expected condition to be true: " #condition); \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_FALSE(condition) do { \
  ++g_test_count; \
  if( (condition) ) { \
    LOG_FAIL("Expected condition to be false: " #condition); \
    ++g_test_failures; \
  } \
} while(0)

// Test 1: Null pointer input should yield 0 and not crash
void test_sqlite3WalCallback_null_ptr() {
  // When pWal is NULL, the function should return 0
  int ret = sqlite3WalCallback(nullptr);
  EXPECT_EQ(0, ret);
}

// Test 2: Non-null Wal with a non-zero iCallback should return that value and clear it
void test_sqlite3WalCallback_read_and_clear() {
  Wal w;
  // Initialize iCallback to a non-zero sentinel value
  w.iCallback = 1234u;
  int ret = sqlite3WalCallback(&w);
  EXPECT_EQ(1234, ret);   // Return the original iCallback value
  EXPECT_EQ(0, w.iCallback); // After callback, iCallback should be reset to 0
}

// Test 3: Non-null Wal with iCallback already zero should return 0 and leave iCallback at 0
void test_sqlite3WalCallback_zero() {
  Wal w;
  w.iCallback = 0;
  int ret = sqlite3WalCallback(&w);
  EXPECT_EQ(0, ret);
  EXPECT_EQ(0, w.iCallback);
}

// Test 4: Multiple calls on the same Wal should clear after the first call and return 0 on subsequent calls
void test_sqlite3WalCallback_multiple_calls() {
  Wal w;
  w.iCallback = 5u;
  int r1 = sqlite3WalCallback(&w);
  int r2 = sqlite3WalCallback(&w);
  EXPECT_EQ(5, r1);
  EXPECT_EQ(0, r2);
  EXPECT_EQ(0, w.iCallback);
}

// Simple test runner
int main() {
  std::cout << "Starting test suite for sqlite3WalCallback...\n";

  test_sqlite3WalCallback_null_ptr();
  test_sqlite3WalCallback_read_and_clear();
  test_sqlite3WalCallback_zero();
  test_sqlite3WalCallback_multiple_calls();

  std::cout << "Tests completed. Ran: " << g_test_count << ", Failures: " << g_test_failures << "\n";

  if (g_test_failures > 0) {
    std::cerr << "Some tests FAILED. Review the failure messages above.\n";
  } else {
    std::cout << "All tests PASSED.\n";
  }

  // Return non-zero if any test failed to integrate with common CI expectations
  return g_test_failures;
}