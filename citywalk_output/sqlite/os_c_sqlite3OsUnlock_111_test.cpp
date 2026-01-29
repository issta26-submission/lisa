// C++11 unit tests for the focal method sqlite3OsUnlock in os.c
// Notes:
// - This test suite uses a minimal, self-contained mock of sqlite3_file and its
//   pMethods.xUnlock to verify sqlite3OsUnlock behavior without requiring the
//   full SQLite test infrastructure.
// - Tests are designed to be executable without Google Test (GTest).
// - The test suite attempts to cover:
//    1) True branches of the predicate (lockType is SQLITE_LOCK_NONE or SQLITE_LOCK_SHARED).
//    2) The false branch of the predicate by triggering the assertion (when possible).
//       This is done via a SIGABRT catch mechanism to recover from the abort and still
//       report coverage. If the environment disables asserts (NDEBUG), this test will
//       report as skipped/fail accordingly.
// - All tests operate through extern "C" linkage to the actual sqlite3OsUnlock function
//   and are designed to work with a project that compiles os.c alongside this test.

#include <setjmp.h>
#include <signal.h>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Domain-provided constants (matching expected usage in sqlite3OsUnlock)
#define SQLITE_LOCK_NONE 0
#define SQLITE_LOCK_SHARED 1

// Forward declare the C structures and function to link with sqlite3OsUnlock.
extern "C" {

  // Minimal C-compatible representations required by sqlite3OsUnlock
  struct sqlite3_file;
  struct sqlite3_io_methods {
    int (*xUnlock)(struct sqlite3_file*, int);
  };
  struct sqlite3_file {
    struct sqlite3_io_methods *pMethods;
  };

  // Focal C function under test (located in os.c)
  int sqlite3OsUnlock(struct sqlite3_file *id, int lockType);
}

// Lightweight test-state globals used by the mock implementation
namespace TestNS {
  // The value that the mock xUnlock should return
  int g_returnValueForUnlock = 0;

  // The last call capture for verification
  struct sqlite3_file g_file_under_test;
  struct sqlite3_io_methods g_methods;

  // Globals to capture the arguments passed to the mock xUnlock
  struct sqlite3_file *g_last_id_ptr = nullptr;
  int g_last_lockType = -1;

  // Simple mock: record inputs and return a preset value
  int mock_xUnlock(struct sqlite3_file *id, int lockType) {
    g_last_id_ptr = id;
    g_last_lockType = lockType;
    return g_returnValueForUnlock;
  }

  // For abort-path test (signal handling)
  jmp_buf g_env;
  struct sigaction g_old_action;
  void sigabrt_handler(int /*sig*/) {
    // Jump back to test harness if an abort occurs
    longjmp(g_env, 1);
  }
}

// Convenience macros to log test results without terminating on failure
static int g_pass = 0;
static int g_fail = 0;

#define EXPECT_EQ(a, b) do { \
  if ((a) != (b)) { \
    std::cerr << "EXPECT_EQ failed: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")" << std::endl; \
    ++g_fail; \
  } else { \
    ++g_pass; \
  } \
} while (0)

#define EXPECT_TRUE(cond) do { \
  if (!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " << #cond << std::endl; \
    ++g_fail; \
  } else { \
    ++g_pass; \
  } \
} while (0)

#define EXPECT_FALSE(cond) do { \
  if ((cond)) { \
    std::cerr << "EXPECT_FALSE failed: " << #cond << std::endl; \
    ++g_fail; \
  } else { \
    ++g_pass; \
  } \
} while (0)


// Test 1: Validate sqlite3OsUnlock with SQLITE_LOCK_NONE
// - Verifies that xUnlock is called with the provided id and lockType
void test_sqlite3OsUnlock_with_none() {
  // Prepare mock
  TestNS::g_returnValueForUnlock = 12345;
  TestNS::g_last_id_ptr = nullptr;
  TestNS::g_last_lockType = -1;

  TestNS::g_methods.xUnlock = TestNS::mock_xUnlock;
  TestNS::g_file_under_test.pMethods = &TestNS::g_methods;

  // Call the focal method
  int ret = sqlite3OsUnlock(&TestNS::g_file_under_test, SQLITE_LOCK_NONE);

  // Assertions
  EXPECT_EQ(ret, 12345);
  EXPECT_EQ(TestNS::g_last_id_ptr, &TestNS::g_file_under_test);
  EXPECT_EQ(TestNS::g_last_lockType, SQLITE_LOCK_NONE);
}

// Test 2: Validate sqlite3OsUnlock with SQLITE_LOCK_SHARED
// - Verifies that xUnlock is called with the provided id and lockType
void test_sqlite3OsUnlock_with_shared() {
  // Prepare mock
  TestNS::g_returnValueForUnlock = 67890;
  TestNS::g_last_id_ptr = nullptr;
  TestNS::g_last_lockType = -1;

  TestNS::g_methods.xUnlock = TestNS::mock_xUnlock;
  TestNS::g_file_under_test.pMethods = &TestNS::g_methods;

  // Call the focal method
  int ret = sqlite3OsUnlock(&TestNS::g_file_under_test, SQLITE_LOCK_SHARED);

  // Assertions
  EXPECT_EQ(ret, 67890);
  EXPECT_EQ(TestNS::g_last_id_ptr, &TestNS::g_file_under_test);
  EXPECT_EQ(TestNS::g_last_lockType, SQLITE_LOCK_SHARED);
}

// Test 3: Abort-path coverage for the assertion in sqlite3OsUnlock
// - Attempts to trigger the assertion by passing an invalid lockType.
// - Uses a signal handler with setjmp/longjmp to recover if the runtime assertion aborts the program.
// - If the environment defines NDEBUG and disables assertions, this test will report a skip/fail since
//   the abort path cannot be exercised.
void test_sqlite3OsUnlock_assert_path() {
  // Install a signal handler to catch abort (SIGABRT) from failed assert
  bool did_recover = false;
  if (setjmp(TestNS::g_env) == 0) {
    // Set handler
    struct sigaction act;
    std::memset(&act, 0, sizeof(act));
    act.sa_handler = TestNS::sigabrt_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(SIGABRT, &act, &TestNS::g_old_action) != 0) {
      // If we cannot install signal handler, skip gracefully
      std::cerr << "SKIP: Unable to install SIGABRT handler for abort-path test." << std::endl;
      return;
    }

    // Prepare file context (xUnlock may be unused if assert triggers)
    TestNS::g_returnValueForUnlock = 0;
    TestNS::g_methods.xUnlock = TestNS::mock_xUnlock;
    TestNS::g_file_under_test.pMethods = &TestNS::g_methods;

    // Trigger abort by passing an invalid lockType
    // This is expected to abort via the assertion in sqlite3OsUnlock
    // If the environment has NDEBUG, this will not abort.
    sqlite3OsUnlock(&TestNS::g_file_under_test, -1);

    // If we reach here, no abort occurred; record as a test failure for abort-path coverage
    EXPECT_TRUE(false && "Abort did not occur as expected (NDEBUG might be defined).");
  } else {
    // We jumped back due to abort via longjmp from the signal handler
    did_recover = true;
    EXPECT_TRUE(true); // Abort path was exercised
  }

  // Restore previous SIGABRT handler
  sigaction(SIGABRT, &TestNS::g_old_action, nullptr);

  // If abort was not observed, mark as failure
  if (!did_recover) {
    std::cerr << "Abort-path coverage not exercised (environment may compile without asserts)." << std::endl;
  }
}

// Entry point for running tests
int main() {
  std::cout << "Starting sqlite3OsUnlock unit test suite (C++11)..." << std::endl;

  // Initialize global test structures
  TestNS::g_file_under_test.pMethods = nullptr;
  TestNS::g_methods.xUnlock = nullptr;
  // Run tests
  test_sqlite3OsUnlock_with_none();
  test_sqlite3OsUnlock_with_shared();
  test_sqlite3OsUnlock_assert_path();

  // Report summary
  std::cout << "Tests run: " << (g_pass + g_fail) << ", Passed: " << g_pass << ", Failed: " << g_fail << std::endl;
  if (g_fail > 0) {
    return 1;
  }
  return 0;
}