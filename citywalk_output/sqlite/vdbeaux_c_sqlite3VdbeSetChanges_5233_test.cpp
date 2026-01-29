// Minimal C++11 test suite for sqlite3VdbeSetChanges (vdbeaux.c) without GTest.
// The tests use a small in-process harness and a mock for sqlite3_mutex_held.
// Explanations are provided as comments above each test.

// Domain knowledge constraints satisfied:
// - Only standard library used (POSIX fork for an assertion-path test on Unix-like systems).
// - Static helpers are kept internal to the test file.
// - No access to private/internal SQLite APIs is required here; we mock only what is necessary.
// - Tests are executable from main() and do not rely on GTest.
// - We treat i64 as a 64-bit integer alias (long long) to align with typical SQLite typedefs.

#include <sys/types.h>
#include <cstdio>
#include <sys/wait.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>


// Lightweight test harness
int g_tests_run = 0;
int g_tests_failed = 0;

// Simple EXPECT macros that do not terminate the test (non-terminating assertions)
#define EXPECT_TRUE(cond) \
  do { \
    if(!(cond)) { \
      std::cerr << "Expectation failed: " #cond " at " __FILE__ ":" << __LINE__ << std::endl; \
      ++g_tests_failed; \
    } \
  } while(0)

#define EXPECT_EQ(a,b) \
  do { \
    if((a) != (b)) { \
      std::cerr << "Expectation failed: (" #a ")=" << (a) << " != (" #b ")=" << (b) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
      ++g_tests_failed; \
    } \
  } while(0)

#define TEST_BEGIN(name) \
  static void name(); \
  static void name##_runner() { name(); } \
  /* declare for potential future registries */ \
  static int name##_registered = (0, (int)0);

#define TEST_END(name) 

// Domain-specific types and mocks to mimic the essential parts of the focal function's environment

// Assume 64-bit integer type used by sqlite3VdbeSetChanges
typedef long long i64;

// Mocked mutex structure (minimal, only to exist in memory for the test)
struct sqlite3_mutex { int dummy; };

// Focal sqlite3 structure used by sqlite3VdbeSetChanges
// We only define the fields required by the test harness and the focal function
struct sqlite3 {
  sqlite3_mutex *mutex; // mutex handle
  i64 nChange;          // last change delta
  i64 nTotalChange;     // cumulative change count
};

// Forward declarations of the focal function and its needed helpers
// We provide a minimal, mockable interface to allow linking with the real implementation in vdbeaux.c
extern "C" {

// i64 alias for the function parameter
typedef long long i64;

// Function under test (provided in vdbeaux.c in the actual project)
void sqlite3VdbeSetChanges(sqlite3 *db, i64 nChange);

// Mutex helper used by the function; in the real project this is sqlite3_mutex_held
int sqlite3_mutex_held(struct sqlite3_mutex *p);
}

// Global flag controlled by tests to simulate mutex ownership status
static int g_mutexHeld_flag = 0;

// Mock implementation to intercept the assertion in the focal method.
// In the real code, sqlite3_mutex_held(db->mutex) must be true; we simulate that here.
extern "C" int sqlite3_mutex_held(struct sqlite3_mutex *p) {
  (void)p; // unused in the mock
  return g_mutexHeld_flag;
}

// Helper to set mutex ownership from tests
static void set_mutex_held(bool held) {
  g_mutexHeld_flag = held ? 1 : 0;
}

// -----------------------------------------------------------------------------
// Test 1: Basic behavior when the mutex is held
// Verify that sqlite3VdbeSetChanges updates both db.nChange and db.nTotalChange
// -----------------------------------------------------------------------------
static void test_basic_behavior_when_mutex_held() {
  ++g_tests_run;
  // Setup: simulate a database with a valid mutex and zero changes
  sqlite3 db;
  sqlite3_mutex m;
  m.dummy = 0;
  db.mutex = &m;
  db.nChange = 0;
  db.nTotalChange = 0;

  // Simulate that the caller holds the mutex
  set_mutex_held(true);

  // First change: add 5
  sqlite3VdbeSetChanges(&db, 5);
  EXPECT_EQ(db.nChange, 5);
  EXPECT_EQ(db.nTotalChange, 5);

  // Second change: add 3
  sqlite3VdbeSetChanges(&db, 3);
  EXPECT_EQ(db.nChange, 3);
  EXPECT_EQ(db.nTotalChange, 8);
}

// -----------------------------------------------------------------------------
// Test 2: More change values (positive, then negative) to ensure accumulation
// -----------------------------------------------------------------------------
static void test_accumulation_with_positive_and_negative_changes() {
  ++g_tests_run;
  sqlite3 db;
  sqlite3_mutex m;
  m.dummy = 0;
  db.mutex = &m;
  db.nChange = 0;
  db.nTotalChange = 0;

  set_mutex_held(true);

  // Positive changes
  sqlite3VdbeSetChanges(&db, 10);
  EXPECT_EQ(db.nChange, 10);
  EXPECT_EQ(db.nTotalChange, 10);

  // Negative change (e.g., rollback or correction)
  sqlite3VdbeSetChanges(&db, -4);
  EXPECT_EQ(db.nChange, -4);
  EXPECT_EQ(db.nTotalChange, 6);
}

// -----------------------------------------------------------------------------
// Test 3: False branch for the mutex predicate (assert would fire)
// We attempt to exercise the assertion by running the call in a separate
// process where the assertion would normally abort the process.
// This test is POSIX-specific due to fork/wait semantics.
// If the child is terminated by SIGABRT, the test passes for this branch.
// -----------------------------------------------------------------------------
#ifndef _WIN32
static void test_assert_triggers_on_mutex_not_held() {
  ++g_tests_run;

  // Prepare a minimal db and mutex
  sqlite3 db;
  sqlite3_mutex m;
  m.dummy = 0;
  db.mutex = &m;
  db.nChange = 0;
  db.nTotalChange = 0;

  // Do not hold the mutex to trigger the assertion inside sqlite3VdbeSetChanges.
  set_mutex_held(false);

  pid_t pid = fork();
  if (pid == 0) {
    // Child process: should hit the assertion and abort
    // If the assertion does not abort (due to undefined behavior or test harness quirks),
    // exit with non-zero to indicate failure to terminate as expected.
    sqlite3VdbeSetChanges(&db, 1);
    _exit(74); // unexpected path if no abort occurred
  } else if (pid > 0) {
    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT) {
      // Expected: child aborted due to assertion
      // Treat as pass for the false-path scenario
    } else {
      // If we reach here, the assertion did not abort as expected
      std::cerr << "Expected child to abort due to assertion (SIGABRT). Status: " << status << std::endl;
      ++g_tests_failed;
    }
  } else {
    // Fork failed; report test as failed to run
    std::cerr << "Fork failed for test_assert_triggers_on_mutex_not_held." << std::endl;
    ++g_tests_failed;
  }
}
#endif // _WIN32

// -----------------------------------------------------------------------------
// Main: Run all tests and report a summary
// -----------------------------------------------------------------------------
int main() {
  // Run tests
  test_basic_behavior_when_mutex_held();
#ifndef _WIN32
  test_assert_triggers_on_mutex_not_held();
#endif
  test_accumulation_with_positive_and_negative_changes();

  // Summary
  std::cout << "Tests run: " << g_tests_run
            << ", Failures: " << g_tests_failed << std::endl;

  // Return non-zero if any test failed
  return (g_tests_failed != 0) ? 1 : 0;
}