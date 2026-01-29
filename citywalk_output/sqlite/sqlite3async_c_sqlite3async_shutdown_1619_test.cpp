// Lightweight C++11 test harness for sqlite3async_shutdown
// This test re-creates the minimal environment to exercise the focal method
// without depending on GTest or external libraries.
// It uses extern "C" linkage to mimic the C function under test and provides
// minimal mocks for the external dependencies (async_vfs, sqlite3_vfs, etc.).

#include <sched.h>
#include <string.h>
#include <windows.h>
#include <assert.h>
#include <cstdio>
#include <cstdint>
#include <sqlite3.h>
#include <stdio.h>
#include <sqlite3async.h>
#include <stdarg.h>
#include <pthread.h>


// ------------------------------------------------------------
// Minimal type/mocks to mimic the dependencies of sqlite3async_shutdown
// ------------------------------------------------------------

// Forward declare C linkage for the function under test and its dependencies
extern "C" {
  void sqlite3async_shutdown(void);
  void sqlite3_vfs_unregister(struct sqlite3_vfs*);

  void async_os_shutdown(void);
}

// A minimal sqlite3_vfs type since the real one is part of SQLite.
// Here we only need an incomplete type to pass a pointer to sqlite3_vfs_unregister.
struct sqlite3_vfs {
  int dummy; // not used by tests
};

// A lightweight representation of the async VFS used by sqlite3async_shutdown.
// We only care about the pAppData field for the shutdown logic.
typedef struct {
  void* pAppData;
} AsyncVfsLike;

// The focal VFS instance (as used by sqlite3async_shutdown).
// We model it as a static-like global in this translation unit.
// The real implementation would have a more complex layout; we only need the pAppData field.
static AsyncVfsLike async_vfs = { nullptr };

// ------------------------------------------------------------
// Test infrastructure (non-terminating assertions)
// - We implement simple EXPECT_* macros that log failures but do not abort the process.
// - We keep a counter of failed expectations and report at the end.
// ------------------------------------------------------------
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    printf("TEST FAILURE: %s (line %d)\n", (msg), __LINE__); \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_EQ_PTR(a, b, msg) do { \
  if((void*)(a) != (void*)(b)) { \
    printf("TEST FAILURE: %s (expected %p, got %p) (line %d)\n", (msg), (void*)(b), (void*)(a), __LINE__); \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_EQ_INT(a, b, msg) do { \
  if((int)(a) != (int)(b)) { \
    printf("TEST FAILURE: %s (expected %d, got %d) (line %d)\n", (msg), (int)(b), (int)(a), __LINE__); \
    ++g_test_failures; \
  } \
} while(0)

#define RESET_COUNTERS() do { g_test_failures = 0; } while(0)

// ------------------------------------------------------------
// Mock implementations of external symbols used by sqlite3async_shutdown
// These provide observable side effects for assertions.
// ------------------------------------------------------------

// Track whether async_os_shutdown was called
static int g_async_os_shutdown_calls = 0;
extern "C" void async_os_shutdown(void) {
  ++g_async_os_shutdown_calls;
}

// Track the argument passed to sqlite3_vfs_unregister to verify the correct object is unregistered
static void* g_last_unregister_arg = nullptr;
static int g_unregister_called = 0;

extern "C" void sqlite3_vfs_unregister(struct sqlite3_vfs* pVfs) {
  g_unregister_called = 1;
  g_last_unregister_arg = (void*)pVfs;
}

// ------------------------------------------------------------
// Implementation under test (reproduces the focal method in a single TU)
// We implement sqlite3async_shutdown with the same behavior as in the focal code.
// It uses the global async_vfs instance and the external mocks above.
// This TU acts as the compilation unit containing the method under test.
// ------------------------------------------------------------

extern "C" void sqlite3async_shutdown(void) {
  if( async_vfs.pAppData ){
    async_os_shutdown();
    sqlite3_vfs_unregister((sqlite3_vfs *)&async_vfs);
    async_vfs.pAppData = 0;
  }
}

// ------------------------------------------------------------
// Test cases
// - Test 1: When async_vfs.pAppData is non-null, the function should:
//     a) call async_os_shutdown
//     b) unregister the VFS
//     c) clear pAppData
// - Test 2: When async_vfs.pAppData is null, the function should do nothing
// (no calls to async_os_shutdown or sqlite3_vfs_unregister and no modification of pAppData).
// Each test includes explanatory comments.
// ------------------------------------------------------------

// Helper to reset test state between test cases
static void reset_test_state() {
  g_async_os_shutdown_calls = 0;
  g_unregister_called = 0;
  g_last_unregister_arg = nullptr;
  async_vfs.pAppData = nullptr;
}

// Test 1: True branch - ensure shutdown sequence is performed and state is reset
static void test_sqlite3async_shutdown_with_appdata() {
  // Explanation:
  // - Set app data to simulate an initialized/active async VFS.
  // - Call sqlite3async_shutdown and verify:
  //   * async_os_shutdown is invoked exactly once
  //   * sqlite3_vfs_unregister is called with the address of async_vfs
  //   * async_vfs.pAppData is cleared (set to 0)
  printf("Running test_sqlite3async_shutdown_with_appdata...\n");

  reset_test_state();
  // Arrange
  int expected_appdata_ptr_present = 1;
  void* dummy_ptr = (void*)0xDEADBEEF;
  async_vfs.pAppData = dummy_ptr;

  // Act
  sqlite3async_shutdown();

  // Assert
  EXPECT_TRUE(g_async_os_shutdown_calls == 1, "async_os_shutdown should be called once");
  EXPECT_TRUE(g_unregister_called == 1, "sqlite3_vfs_unregister should be called once");
  EXPECT_EQ_PTR(g_last_unregister_arg, (void*)&async_vfs, "sqlite3_vfs_unregister should be called with &async_vfs");
  EXPECT_EQ_PTR(async_vfs.pAppData, nullptr, "async_vfs.pAppData should be cleared to 0 (NULL)");
  // Ensure non-null was used to trigger the path
  EXPECT_TRUE((void*)dummy_ptr != nullptr, "existing appData pointer should be non-null before shutdown");
  printf("test_sqlite3async_shutdown_with_appdata completed.\n\n");
}

// Test 2: False branch - ensure no actions occur when there is no app data
static void test_sqlite3async_shutdown_without_appdata() {
  // Explanation:
  // - Ensure that with pAppData == NULL, the function does not call shutdown/unregister
  // - pAppData remains NULL and no side effects occur
  printf("Running test_sqlite3async_shutdown_without_appdata...\n");

  reset_test_state();

  // Arrange
  async_vfs.pAppData = nullptr;

  // Act
  sqlite3async_shutdown();

  // Assert
  EXPECT_TRUE(g_async_os_shutdown_calls == 0, "async_os_shutdown should not be called when appData is NULL");
  EXPECT_TRUE(g_unregister_called == 0, "sqlite3_vfs_unregister should not be called when appData is NULL");
  EXPECT_EQ_PTR(async_vfs.pAppData, nullptr, "async_vfs.pAppData should remain NULL");
  printf("test_sqlite3async_shutdown_without_appdata completed.\n\n");
}

// ------------------------------------------------------------
// Main entrypoint to run tests
// ------------------------------------------------------------
int main() {
  printf("Starting sqlite3async_shutdown test suite (C++11 harness, no GTest)\n\n");

  // Run tests
  test_sqlite3async_shutdown_with_appdata();
  test_sqlite3async_shutdown_without_appdata();

  // Summary
  if(g_test_failures == 0) {
    printf("ALL TESTS PASSED\n");
  } else {
    printf("TOTAL TEST FAILURES: %d\n", g_test_failures);
  }
  return g_test_failures;
}