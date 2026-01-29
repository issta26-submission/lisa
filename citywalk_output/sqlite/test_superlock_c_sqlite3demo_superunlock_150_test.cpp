// Comprehensive C++11 test suite for sqlite3demo_superunlock
// This test harness mocks the minimal SQLite interactions to exercise
// the focal function in environments without a full SQLite runtime.
// It does not rely on GTest; tests are executed from main() with
// non-terminating assertions (printed outputs).

#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3.h>
#include <cstring>
#include <iostream>


// Forward declare the focal function (defined in test_superlock.c)
extern "C" void sqlite3demo_superunlock(void *pLock);

// Include SQLite types and constants for pointer types and flags.
// We rely on the header to define the necessary types and constants.

// Candidate structure to mirror the expected client-side lock object.
// We assume the layout: sqlite3* db; int bWal;  (matches common usage pattern)
struct Superlock {
  sqlite3 *db;
  int bWal;
};

// Global state to drive and observe behavior of the stubs.
enum CaseType { CASE_WAL_OK, CASE_WAL_FALSE, CASE_FILE_FAIL };

// Drive the stubs' behavior
static CaseType g_case = CASE_WAL_OK;

// Fake "file pointer" infrastructure
struct FakeSqlite3File {
  sqlite3_io_methods *pMethods;
};
static FakeSqlite3File g_fd;

// A global io_methods instance that routes xShmLock to our test hook
static sqlite3_io_methods g_io_methods;

// Captured xShmLock calls
struct XShmLockCall {
  int idx;
  int nByte;
  int flags;
};
static std::vector<XShmLockCall> g_lock_calls;

// Observation of close/free behavior
static bool g_close_called = false;
static sqlite3 *g_closed_db = nullptr;
static bool g_free_called = false;
static void *g_freed_ptr = nullptr;

// Forward declare stubbed SQLite functions with C linkage
extern "C" {

// sqlite3_file_control stub: optionally fail based on test case
int sqlite3_file_control(sqlite3 *db, const char *zDbName, int op, void **ppFile) {
  // If test requests a failure, simulate non-OK rc
  if (g_case == CASE_FILE_FAIL) {
    return SQLITE_ERROR;
  }
  // Otherwise, supply a fake file pointer to be used by sqlite3demo_superunlock
  *ppFile = (void *)&g_fd;
  return SQLITE_OK;
}

// sqlite3_close stub: record invocation and argument
void sqlite3_close(sqlite3 *db) {
  g_close_called = true;
  g_closed_db = db;
}

// sqlite3_free stub: record invocation and argument
void sqlite3_free(void *p) {
  g_free_called = true;
  g_freed_ptr = p;
}

} // extern "C"


// Helper: reset all observers and state before each test
static void reset_state() {
  g_case = CASE_WAL_OK;
  g_lock_calls.clear();
  g_close_called = false;
  g_closed_db = nullptr;
  g_free_called = false;
  g_freed_ptr = nullptr;
  // Prepare the fake fd's method pointer
  memset(&g_fd, 0, sizeof(g_fd));
  g_io_methods = sqlite3_io_methods{}; // zero-initialize
  // Attach our xShmLock hook
  // (We declare the function below; it captures calls into g_lock_calls)
}

// Hook: captures xShmLock invocations
static int test_xShmLock(sqlite3_file *fd, int idx, int nByte, int flags) {
  (void)fd; // not strictly needed for test verification
  g_lock_calls.push_back({idx, nByte, flags});
  return SQLITE_OK;
}

// Prepare the fake IO methods object
static void prepare_io_methods() {
  // Ensure the xShmLock hook is set
  g_io_methods.xShmLock = test_xShmLock;
  // Attach io_methods to the fake fd
  g_fd.pMethods = &g_io_methods;
}

// Simple non-terminating assertion helper
static void expect(bool condition, const char* message) {
  if (!condition) {
    std::cerr << "EXPECTATION FAILED: " << message << std::endl;
  }
}

// Test 1: Wal enabled, file control returns OK -> two xShmLock calls, then close/free
static void test_wal_ok_performs_shmlock_and_cleanup() {
  reset_state();
  prepare_io_methods();

  // Build a valid-ish Superlock object
  sqlite3* dummy_db = reinterpret_cast<sqlite3*>(0xDEADBEEF);
  Superlock sl;
  sl.db = dummy_db;
  sl.bWal = 1; // true

  // Drive the test
  sqlite3demo_superunlock((void *)&sl);

  // Assertions
  // Expect exactly two xShmLock invocations with expected indices and sizes
  expect(g_lock_calls.size() == 2, "Two xShmLock calls expected when WAL is enabled and file-pointer is obtainable");
  if (g_lock_calls.size() >= 1) {
    expect(g_lock_calls[0].idx == 2, "First xShmLock call should be for index 2");
    expect(g_lock_calls[0].nByte == 1, "First xShmLock call should lock 1 byte");
    int expectedFlags = SQLITE_SHM_UNLOCK | SQLITE_SHM_EXCLUSIVE;
    expect(g_lock_calls[0].flags == expectedFlags, "First xShmLock flags should be correct");
  }
  if (g_lock_calls.size() >= 2) {
    expect(g_lock_calls[1].idx == 3, "Second xShmLock call should be for index 3");
    expect(g_lock_calls[1].nByte == (SQLITE_SHM_NLOCK - 3), "Second xShmLock call should lock SQLITE_SHM_NLOCK-3 bytes");
    int expectedFlags = SQLITE_SHM_UNLOCK | SQLITE_SHM_EXCLUSIVE;
    expect(g_lock_calls[1].flags == expectedFlags, "Second xShmLock flags should be correct");
  }
  // Always expect cleanup
  expect(g_close_called, "sqlite3_close should be called");
  expect(g_closed_db == dummy_db, "sqlite3_close should receive the original db pointer");
  expect(g_free_called, "sqlite3_free should be called");
  expect(g_freed_ptr == &sl, "sqlite3_free should receive the original pLock pointer");
  std::cout << "Test WAL_OK path completed.\n";
}

// Test 2: Wal disabled -> no xShmLock calls, but still close/free
static void test_wal_false_skips_shmlock_and_cleans_up() {
  reset_state();
  prepare_io_methods();

  sqlite3* dummy_db = reinterpret_cast<sqlite3*>(0xBAADF00D);
  Superlock sl;
  sl.db = dummy_db;
  sl.bWal = 0; // false

  sqlite3demo_superunlock((void *)&sl);

  // Assertions
  expect(g_lock_calls.empty(), "No xShmLock calls should occur when WAL flag is false");
  expect(g_close_called, "sqlite3_close should still be called");
  expect(g_closed_db == dummy_db, "sqlite3_close should receive the original db pointer");
  expect(g_free_called, "sqlite3_free should be called");
  expect(g_freed_ptr == &sl, "sqlite3_free should receive the original pLock pointer");
  std::cout << "Test WAL_FALSE path completed.\n";
}

// Test 3: Wal enabled but file control reports error -> no shmlock, still close/free
static void test_file_control_error_during_pointer_retrieval() {
  reset_state();
  prepare_io_methods();

  // Force file_control to fail
  g_case = CASE_FILE_FAIL;

  sqlite3* dummy_db = reinterpret_cast<sqlite3*>(0xFEEDFACE);
  Superlock sl;
  sl.db = dummy_db;
  sl.bWal = 1; // true

  sqlite3demo_superunlock((void *)&sl);

  // Assertions
  expect(g_lock_calls.empty(), "No xShmLock calls should occur when file_control fails");
  expect(g_close_called, "sqlite3_close should still be called even if unlocking fails");
  expect(g_closed_db == dummy_db, "sqlite3_close should receive the original db pointer");
  expect(g_free_called, "sqlite3_free should be called");
  expect(g_freed_ptr == &sl, "sqlite3_free should receive the original pLock pointer");

  std::cout << "Test FILE_CONTROL_FAIL path completed.\n";
}

int main() {
  std::cout << "Starting sqlite3demo_superunlock test suite (C++11, no GTest)...\n";

  test_wal_ok_performs_shmlock_and_cleanup();
  test_wal_false_skips_shmlock_and_cleans_up();
  test_file_control_error_during_pointer_retrieval();

  std::cout << "Test suite completed.\n";
  return 0;
}