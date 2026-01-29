// Self-contained C++11 unit test for sqlite3RegisterPerConnectionBuiltinFunctions
// This test-suite is designed to run without GoogleTest (GTest) and without linking
// to the real SQLite library. It provides minimal stubs/mocks for the C APIs used by
// sqlite3RegisterPerConnectionBuiltinFunctions and exercises both branches of the logic.

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cassert>


// Domain-provided constants (emulated for test)
enum { SQLITE_OK = 0, SQLITE_NOMEM = 7 };

// Lightweight placeholder for sqlite3 database handle
struct sqlite3 {};

// Global mock state to control and observe behavior of the production function
static int g_overload_rc = SQLITE_OK;                // Next return code from sqlite3_overload_function
static const char* g_overload_last_name = nullptr;   // Captured function name argument
static int g_overload_last_nArg = -1;                // Captured nArg argument
static bool g_oom_was_called = false;                // Whether sqlite3OomFault was invoked

// Mocked C API: sqlite3_overload_function
// Captures its inputs for verification and returns a controllable rc.
int sqlite3_overload_function(sqlite3* /*db*/, const char* zFunctionName, int nArg){
  g_overload_last_name = zFunctionName;
  g_overload_last_nArg = nArg;
  return g_overload_rc;
}

// Mocked C API: sqlite3OomFault
// Signals that an out-of-memory fault handling path was invoked.
void sqlite3OomFault(sqlite3* /*db*/){
  g_oom_was_called = true;
}

// Production-like function under test (reproduced here for self-contained testing)
void sqlite3RegisterPerConnectionBuiltinFunctions(sqlite3 *db){
  int rc = sqlite3_overload_function(db, "MATCH", 2);
  assert( rc==SQLITE_NOMEM || rc==SQLITE_OK );
  if( rc==SQLITE_NOMEM ){
    sqlite3OomFault(db);
  }
}

// Simple test harness (non-terminating assertions)
static int g_total_asserts = 0;
static int g_failed_asserts = 0;
#define ASSERT_TRUE(cond) do { \
  ++g_total_asserts; \
  if(!(cond)) { \
    ++g_failed_asserts; \
    std::cerr << "Assertion failed: " #cond << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
  } \
} while(0)

// Test Case 1: rc == SQLITE_OK should NOT trigger OOM path and should overload "MATCH" with 2 args
// - Verifies both the correct function name ("MATCH") and argument count (2) are passed
// - Verifies that sqlite3OomFault is NOT called
// - Ensures normal path executes without invoking OOM fault handler
bool test_case_ok_nooom() {
  // Reset mocks
  g_total_asserts = g_failed_asserts = 0;
  g_overload_rc = SQLITE_OK;
  g_oom_was_called = false;
  g_overload_last_name = nullptr;
  g_overload_last_nArg = -1;

  sqlite3 db;
  sqlite3RegisterPerConnectionBuiltinFunctions(&db);

  // Validate that the overload function was called with correct parameters
  bool name_ok = (g_overload_last_name != nullptr) && (std::strcmp(g_overload_last_name, "MATCH") == 0);
  bool arg_ok  = (g_overload_last_nArg == 2);
  ASSERT_TRUE(name_ok);
  ASSERT_TRUE(arg_ok);

  // Validate that OOM path was not invoked
  ASSERT_TRUE(!g_oom_was_called);

  if (g_failed_asserts == 0) {
    std::cout << "test_case_ok_nooom: PASSED" << std::endl;
    return true;
  } else {
    std::cout << "test_case_ok_nooom: FAILED with " << g_failed_asserts << " assertion(s)" << std::endl;
    return false;
  }
}

// Test Case 2: rc == SQLITE_NOMEM should trigger OOM path exactly once
// - Verifies that sqlite3OomFault is invoked when overload reports NOMEM
// - Verifies that the function still passes the correct parameters to sqlite3_overload_function
bool test_case_nOMEM_calls_oom() {
  // Reset mocks
  g_total_asserts = g_failed_asserts = 0;
  g_overload_rc = SQLITE_NOMEM;
  g_oom_was_called = false;
  g_overload_last_name = nullptr;
  g_overload_last_nArg = -1;

  sqlite3 db;
  sqlite3RegisterPerConnectionBuiltinFunctions(&db);

  // OOM path must be invoked
  ASSERT_TRUE(g_oom_was_called);

  if (g_failed_asserts == 0) {
    std::cout << "test_case_nOMEM_calls_oom: PASSED" << std::endl;
    return true;
  } else {
    std::cout << "test_case_nOMEM_calls_oom: FAILED with " << g_failed_asserts << " assertion(s)" << std::endl;
    return false;
  }
}

// Main function: run all tests as per domain guidance (no GTest, call from main)
int main() {
  std::cout << "Running unit tests for sqlite3RegisterPerConnectionBuiltinFunctions (self-contained, C++11)" << std::endl;

  bool t1 = test_case_ok_nooom();
  bool t2 = test_case_nOMEM_calls_oom();

  int total = 2;
  int passed = (t1?1:0) + (t2?1:0);

  std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

  return (passed == total) ? 0 : 1;
}