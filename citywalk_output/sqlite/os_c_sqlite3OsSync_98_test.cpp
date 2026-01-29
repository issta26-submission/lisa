/************* Unit Test Suite for sqlite3OsSync *************
 This test suite is crafted to validate the focal method:
   int sqlite3OsSync(sqlite3_file *id, int flags)

 Key aspects captured from the focal class/dependency surface:
 - sqlite3_file struct with a member: pMethods pointing to sqlite3_io_methods
 - sqlite3_io_methods struct with a function pointer: xSync(sqlite3_file*, int)
 - SQLITE_OK (success code)
 - DO_OS_MALLOC_TEST macro invocation inside sqlite3OsSync
 - Behavior:
     - If flags is zero: return SQLITE_OK and do not call xSync
     - If flags is non-zero: call id->pMethods->xSync(id, flags) and return its result

 The test does not depend on the real SQLite library and uses minimal stubs to reproduce the behavior.
**********************************************************************/

#include <iostream>
#include <sqliteInt.h>


// Domain knowledge helpers and simple stubbed environment to mimic sqlite3OsSync
#define SQLITE_OK 0
#define DO_OS_MALLOC_TEST(p) do { (void)(p); } while(0) // stub: no-op for test

// Focal types (minimal stand-ins for test environment)
struct sqlite3_file;
struct sqlite3_io_methods {
  int (*xSync)(sqlite3_file*, int);
};
struct sqlite3_file {
  sqlite3_io_methods *pMethods;
};

// Focal method under test (reproduced in test environment)
int sqlite3OsSync(sqlite3_file *id, int flags){
  DO_OS_MALLOC_TEST(id);
  return flags ? id->pMethods->xSync(id, flags) : SQLITE_OK;
}

// Domain-specific test scaffolding
// Global hooks to verify that xSync would be invoked and with which parameters
static int g_sync_calls = 0;
static int g_sync_lastFlags = 0;
static int g_sync_returnValue = 0;

// Mock implementation of xSync used by the tests
static int mock_xSync(sqlite3_file* /*id*/, int flags) {
  ++g_sync_calls;
  g_sync_lastFlags = flags;
  return g_sync_returnValue;
}

// Lightweight assertion utility
#define ASSERT_TRUE(cond, msg) \
  do { if (!(cond)) { std::cerr << "ASSERTION FAILED: " << (msg) << "\n"; return false; } } while(0)


// Test 1: When flags == 0, sqlite3OsSync should return SQLITE_OK and NOT call xSync
bool test_sqlite3OsSync_flags_zero_no_call() {
  // Arrange
  sqlite3_io_methods methods;
  methods.xSync = &mock_xSync;
  sqlite3_file id;
  id.pMethods = &methods;

  g_sync_calls = 0;
  g_sync_lastFlags = -1;
  g_sync_returnValue = 999; // value would be ignored if not called

  // Act
  int result = sqlite3OsSync(&id, 0);

  // Assert
  ASSERT_TRUE(result == SQLITE_OK, "Expected SQLITE_OK when flags == 0");
  ASSERT_TRUE(g_sync_calls == 0, "xSync should not be called when flags == 0");
  return true;
}


// Test 2: When flags != 0, sqlite3OsSync should call xSync exactly once and return its value
bool test_sqlite3OsSync_flags_nonzero_calls_and_returns_value() {
  // Arrange
  sqlite3_io_methods methods;
  methods.xSync = &mock_xSync;
  sqlite3_file id;
  id.pMethods = &methods;

  g_sync_calls = 0;
  g_sync_lastFlags = -1;
  g_sync_returnValue = 7;

  // Act
  int result = sqlite3OsSync(&id, 2);

  // Assert
  ASSERT_TRUE(g_sync_calls == 1, "xSync should be called exactly once when flags != 0");
  ASSERT_TRUE(g_sync_lastFlags == 2, "xSync should receive the correct flags value");
  ASSERT_TRUE(result == 7, "sqlite3OsSync should return the value provided by xSync");
  return true;
}


// Test runner
int main() {
  bool all_passed = true;

  std::cout << "Running sqlite3OsSync test: flags == 0 (no call)..." << std::endl;
  all_passed &= test_sqlite3OsSync_flags_zero_no_call();
  std::cout << "Test 1 " << (all_passed ? "PASSED" : "FAILED") << std::endl;

  std::cout << "Running sqlite3OsSync test: flags != 0 (call and return)..." << std::endl;
  all_passed &= test_sqlite3OsSync_flags_nonzero_calls_and_returns_value();
  std::cout << "Test 2 " << (all_passed ? "PASSED" : "FAILED") << std::endl;

  if (all_passed) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cerr << "SOME TESTS FAILED" << std::endl;
    return 1;
  }
}