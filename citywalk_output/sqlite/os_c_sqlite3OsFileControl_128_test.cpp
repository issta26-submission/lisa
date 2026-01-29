// Test suite for sqlite3OsFileControl
// Purpose: verify core behavior of sqlite3OsFileControl when integrated with a mock VFS file
//          - Branch: id->pMethods == 0 should return SQLITE_NOTFOUND
//          - Branch: id->pMethods != 0 should forward to xFileControl and return its result
// Assumptions:
// - The environment provides the SQLite internal/core headers (e.g., sqlite3.h) that define
//   sqlite3_file, sqlite3_io_methods, SQLITE_NOTFOUND, and related types/constants.
// - The focal function sqlite3OsFileControl is linked from the project under test (os.c).
// - This test uses a minimal mock of sqlite3_file and sqlite3_io_methods sufficient to exercise
//   the function's behavior, without needing to reimplement the entire SQLite I/O layer.
// - No GTest is used per requirements; a lightweight main-based test harness is provided.

#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Attempt to use real SQLite types. If the environment provides them, great.
// If not, the test will fail to compile; this file is intended to be used where the
// SQLite core headers are available.

// Forward declaration of the focal function under test.
// It uses C linkage; in C++ this must be declared with extern "C" to avoid name mangling.
extern "C" int sqlite3OsFileControl(sqlite3_file *id, int op, void *pArg);

// Global test state to verify that the mock xFileControl was invoked correctly.
static sqlite3_file *g_last_id = nullptr;
static int g_last_op = 0;
static void *g_last_pArg = nullptr;

// Minimal mock of the xFileControl callback.
// The real SQLite signature is: int xFileControl(sqlite3_file*, int, void*);
static int MockXFileControl(sqlite3_file *id, int op, void *pArg) {
  // Record invocation details for verification in tests.
  g_last_id = id;
  g_last_op = op;
  g_last_pArg = pArg;

  // Return a distinctive value to verify propagation from sqlite3OsFileControl.
  return 0xABCD1234;
}

// Utility to reset the global test state between tests.
static void reset_test_state() {
  g_last_id = nullptr;
  g_last_op = 0;
  g_last_pArg = nullptr;
}

// Test 1: When id->pMethods is null, sqlite3OsFileControl should return SQLITE_NOTFOUND
// This validates the NULL-vtable guard path in the focal method.
static bool test_sqlite3OsFileControl_notfound_when_null_methods() {
  // Prepare a sqlite3_file with no methods
  sqlite3_file f;
  std::memset(&f, 0, sizeof(f));  // zeroes fields, including pMethods
  f.pMethods = nullptr;           // explicitly ensure no methods

  // Call the focal function with an arbitrary operation and argument
  int result = sqlite3OsFileControl(&f, 0x01 /* arbitrary op */, (void*)0xDEADBEEF);

  // Verify that the result equals SQLITE_NOTFOUND as defined by SQLite headers
  bool ok = (result == SQLITE_NOTFOUND);

  // Explanation comment:
  // - Ensures the early exit path when no IO methods are installed on the file handle.
  // - We do not compare to a magic value here; we rely on the documented API contract
  //   that SQLITE_NOTFOUND is returned in this scenario.
  if(!ok) {
    std::cerr << "[Test 1] Expected SQLITE_NOTFOUND when pMethods is NULL, got: " << result << "\n";
  }

  reset_test_state();
  return ok;
}

// Test 2: When id->pMethods is non-null, sqlite3OsFileControl should forward to xFileControl
// and return whatever xFileControl returns. It should also pass through the same id, op, and pArg.
static bool test_sqlite3OsFileControl_forward_to_xFileControl() {
  // Prepare a sqlite3_file with a mock methods table
  sqlite3_file f;
  std::memset(&f, 0, sizeof(f));  // zero the file structure

  // SQLite's sqlite3_io_methods typically has several fields; for the purposes of this test,
  // we only need to set the xFileControl callback. The rest can be left zero/NULL as long as
  // they are not dereferenced by sqlite3OsFileControl (which only uses xFileControl here).
  sqlite3_io_methods methods;
  std::memset(&methods, 0, sizeof(methods));
  methods.xFileControl = MockXFileControl;

  f.pMethods = &methods;  // attach the mock methods

  // Prepare test inputs
  int test_op = 0x1234;
  void *test_pArg = (void*)0xDEADC0DE;

  // Call the focal function
  int result = sqlite3OsFileControl(&f, test_op, test_pArg);

  // Assertions:
  // - The return value should match what MockXFileControl returns
  // - The recorded id, op, and pArg should reflect the call
  bool ok_return = (result == 0xABCD1234);
  bool ok_id = (g_last_id == &f);
  bool ok_op = (g_last_op == test_op);
  bool ok_arg = (g_last_pArg == test_pArg);

  if(!ok_return) {
    std::cerr << "[Test 2] Return value from sqlite3OsFileControl did not propagate from xFileControl. "
              << "Expected 0xABCD1234, got " << std::hex << result << std::dec << "\n";
  }
  if(!ok_id) {
    std::cerr << "[Test 2] xFileControl received incorrect sqlite3_file pointer (id). Expected &f.\n";
  }
  if(!ok_op) {
    std::cerr << "[Test 2] xFileControl received incorrect op. Expected " << test_op
              << " got " << g_last_op << "\n";
  }
  if(!ok_arg) {
    std::cerr << "[Test 2] xFileControl received incorrect pArg. Expected " << test_pArg
              << " got " << g_last_pArg << "\n";
  }

  reset_test_state();
  return ok_return && ok_id && ok_op && ok_arg;
}

// Entry point for the test suite.
// This harness does not rely on any testing framework; it reports failures and continues
// executing subsequent tests (non-terminating assertions style).
int main() {
  int failures = 0;

  // Informational header
  std::cout << "sqlite3OsFileControl unit tests (C++11, no GTest) begin\n";

  if(!test_sqlite3OsFileControl_notfound_when_null_methods()) {
    ++failures;
  } else {
    std::cout << "[Test 1] Passed: returns SQLITE_NOTFOUND when pMethods == nullptr\n";
  }

  if(!test_sqlite3OsFileControl_forward_to_xFileControl()) {
    ++failures;
  } else {
    std::cout << "[Test 2] Passed: forwards to xFileControl and propagates return value\n";
  }

  // Summary
  if(failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cout << failures << " TEST(S) FAILED\n";
  }

  return failures;
}