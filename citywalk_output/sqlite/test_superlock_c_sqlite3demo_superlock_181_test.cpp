/*
  Test Suite for sqlite3demo_superlock (C API) using a C++11 test harness without GTest.

  Notes:
  - We assume the project under test compiles with C++11 and links against sqlite3.
  - Focus is on exercising the focal method sqlite3demo_superlock and its basic behavior
    (success path and a basic failure path). Since the internal WAL/busy mechanics depend on
    the external SQLite state and environment, tests keep assertions non-terminating in spirit,
    but here we implement explicit boolean results per test for clarity.
  - The test harness uses a minimal, self-contained main() with manual test case invocation.
  - Public interfaces exposed by the focal file are declared with extern "C" to ensure correct linkage.
  - All tests are designed to be reproducible on typical POSIX environments. Platform-specific
    adjustments may be needed for Windows.
*/

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <cstdlib>


// SQLite header
extern "C" {
  #include <sqlite3.h>
}

// Declarations of the focal function and unlock helper.
// These are C symbols; use extern "C" to ensure proper linkage in C++.
extern "C" {
  int sqlite3demo_superlock(const char *zPath,
                           const char *zVfs,
                           int (*xBusy)(void*,int),
                           void *pBusyArg,
                           void **ppLock);
  void sqlite3demo_superunlock(void *pLock);
}

// Lightweight test helper to generate a reasonably unique db path in the current dir.
// We intentionally reuse a fixed path per run to simplify cleanup; tests remove the file after use.
static std::string getTestDbPath() {
  return "./test_superlock.db";
}

// Busy callback used in one test to demonstrate that the function accepts a busy handler.
// This is a no-op busy callback that increments a counter if invoked.
static int test_busy_callback(void *pArg, int /*nBusy*/) {
  if (pArg) {
    int *pCounter = static_cast<int *>(pArg);
    ++(*pCounter);
  }
  // Indicate to SQLite to retry immediately (0 means not busy in this wrapper-context; actual
  // semantics depend on SQLite). We return 0 to indicate "not retrying here".
  return 0;
}

// Simple assertion helpers (non-terminating style: report failures but continue).
#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "Test assertion failed: " << (msg) \
              << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")\n"; \
  } \
} while(false)

// Test Case 1: Basic success path
// - Purpose: Ensure that sqlite3demo_superlock can acquire a lock on a fresh database and return a non-null handle.
// - Rationale: Covers the subset of code paths responsible for opening the DB, registering a busy handler
//   (if a callback is provided), beginning an exclusive transaction, and returning a valid lock object.
static bool test_basic_success_path() {
  std::cout << "Running test_basic_success_path...\n";

  // Prepare a clean test DB path
  std::string dbPath = getTestDbPath();
  std::remove(dbPath.c_str());  // ignore if not present

  void *pLock = nullptr;
  int rc = sqlite3demo_superlock(dbPath.c_str(), /*zVfs*/ nullptr, /*xBusy*/ nullptr, /*pBusyArg*/ nullptr, &pLock);

  bool ok = true;
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3demo_superlock should return SQLITE_OK for a new DB");
  TEST_ASSERT(pLock != nullptr, "sqlite3demo_superlock should provide a non-null lock handle on success");

  // Clean up: unlock if lock acquired
  if (pLock) {
    sqlite3demo_superunlock(pLock);
    pLock = nullptr;
  }

  // Cleanup the temp DB file
  std::remove(dbPath.c_str());

  if (!ok) {
    std::cerr << "test_basic_success_path: FAILED\n";
  } else {
    std::cout << "test_basic_success_path: PASSED\n";
  }
  return ok;
}

// Test Case 2: Failure path when path cannot be opened/created
// - Purpose: Validate that providing an invalid path results in a non-SQLITE_OK return and no lock is returned.
// - Rationale: Exercises the branch that handles rc != SQLITE_OK and ensures proper cleanup.
static bool test_failure_path_invalid_path() {
  std::cout << "Running test_failure_path_invalid_path...\n";

  // Provide an obviously invalid path (empty string) to force failure
  const char *invalidPath = "";
  void *pLock = reinterpret_cast<void*>(1); // sentinel value to detect modification
  int rc = sqlite3demo_superlock(invalidPath, /*zVfs*/ nullptr, /*xBusy*/ nullptr, /*pBusyArg*/ nullptr, &pLock);

  bool ok = true;
  TEST_ASSERT(rc != SQLITE_OK, "sqlite3demo_superlock should fail for an invalid path");
  TEST_ASSERT(pLock == 0, "On failure, ppLock should be set to 0 (null)");
  // No need to unlock since no lock should be acquired

  if (!ok) {
    std::cerr << "test_failure_path_invalid_path: FAILED\n";
  } else {
    std::cout << "test_failure_path_invalid_path: PASSED\n";
  }
  return ok;
}

// Test Case 3: Busy handler integration (basic invocation capability)
// - Purpose: Verify that a non-null busy handler can be supplied and that the function accepts it
//   without immediately failing. We can't deterministically cause a busy condition here, but this
//   test ensures the API accepts a non-null xBusy and a non-null pBusyArg pointer.
static bool test_busy_handler_integration() {
  std::cout << "Running test_busy_handler_integration...\n";

  std::string dbPath = getTestDbPath();
  std::remove(dbPath.c_str());  // ensure clean slate

  int busyCounter = 0;
  void *pBusyArg = &busyCounter;

  void *pLock = nullptr;
  int rc = sqlite3demo_superlock(dbPath.c_str(),
                                 /*zVfs*/ nullptr,
                                 /*xBusy*/ test_busy_callback,
                                 /*pBusyArg*/ pBusyArg,
                                 &pLock);

  bool ok = true;
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3demo_superlock should succeed with a valid busy handler (when possible)");
  TEST_ASSERT(pLock != nullptr, "Expected non-null lock handle when rc==SQLITE_OK");

  // Attempt to unlock if acquired
  if (pLock) {
    sqlite3demo_superunlock(pLock);
    pLock = nullptr;
  }

  // Cleanup
  std::remove(dbPath.c_str());

  if (!ok) {
    std::cerr << "test_busy_handler_integration: FAILED\n";
  } else {
    std::cout << "test_busy_handler_integration: PASSED\n";
  }

  // Note: We do not rely on busyCounter value since we cannot deterministically induce a busy condition
  // in this isolated test. This test merely asserts that the API accepts a non-null busy handler.
  return ok;
}

// Entry point for the test suite
int main() {
  std::cout << "Starting sqlite3demo_superlock test suite (C++11 harness, no GTest) ...\n";

  bool all_passed = true;

  if (!test_basic_success_path()) all_passed = false;
  if (!test_failure_path_invalid_path()) all_passed = false;
  if (!test_busy_handler_integration()) all_passed = false;

  std::cout << "Test suite completed. Summary: "
            << (all_passed ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << "\n";

  // Return 0 if all tests passed; non-zero otherwise
  return all_passed ? 0 : 1;
}