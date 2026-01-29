// Test suite for sqlite3WalDbsize(Wal *pWal)
// Purpose: Verify correct behavior of sqlite3WalDbsize in different scenarios.
// This test is written for a C++11 environment without GoogleTest.
// It uses a lightweight, non-terminating assertion approach and prints
// results to stdout for each test case.

#include <Windows.h>
#include <cstdio>
#include <wal.h>


// Include the WAL-related definitions and ensure C linkage for C APIs.
extern "C" {
Pgno sqlite3WalDbsize(Wal *pWal);
}

static bool test_Dbsize_NonNull_ReadLockNonNegative() {
  // True branch: pWal != NULL and readLock >= 0
  Wal w;
  w.readLock = 2;          // Non-negative read lock
  w.hdr.nPage = 123;         // Known page count to be returned

  Pgno result = sqlite3WalDbsize(&w);
  if (result != 123) {
    printf("Test 1 FAILED: Expected 123, got %u\n", (unsigned)result);
    return false;
  }
  return true;
}

static bool test_Dbsize_NullPointer_ReturnsZero() {
  // Null pointer should yield 0
  Pgno result = sqlite3WalDbsize(nullptr);
  if (result != 0) {
    printf("Test 2 FAILED: Expected 0 for null Wal, got %u\n", (unsigned)result);
    return false;
  }
  return true;
}

static bool test_Dbsize_ReadLockNegative_ReturnsZero() {
  // Without a valid read lock (negative), the function should return 0
  Wal w;
  w.readLock = -1;           // Negative read lock to trigger false branch
  w.hdr.nPage = 77;           // Value should be ignored

  Pgno result = sqlite3WalDbsize(&w);
  if (result != 0) {
    printf("Test 3 FAILED: Expected 0 when readLock < 0, got %u\n", (unsigned)result);
    return false;
  }
  return true;
}

int main() {
  int total = 0;
  int passed = 0;

  printf("Starting sqlite3WalDbsize tests...\n");

  // Run Test 1
  ++total;
  if (test_Dbsize_NonNull_ReadLockNonNegative()) {
    printf("Test 1 PASSED: Non-null Wal with readLock >= 0 returns hdr.nPage.\n");
    ++passed;
  }

  // Run Test 2
  ++total;
  if (test_Dbsize_NullPointer_ReturnsZero()) {
    printf("Test 2 PASSED: Null Wal returns 0.\n");
    ++passed;
  }

  // Run Test 3
  ++total;
  if (test_Dbsize_ReadLockNegative_ReturnsZero()) {
    printf("Test 3 PASSED: Negative readLock returns 0.\n");
    ++passed;
  }

  printf("sqlite3WalDbsize tests completed: %d/%d passed.\n", passed, total);

  // Return non-zero if any test failed to aid quick detection in shells
  return (passed == total) ? 0 : 1;
}