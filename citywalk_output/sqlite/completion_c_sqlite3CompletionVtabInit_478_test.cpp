// Unit test suite for sqlite3CompletionVtabInit (from completion.c)
// Target: C++11, no Google Test, plain main-based test runner.
// This test stubs the SQLite API symbol sqlite3_create_module to verify:
// - The function calls into sqlite3_create_module when the SQLITE_OMIT_VIRTUALTABLE is not defined.
// - The correct module name ("completion") is passed.
// - The return code from sqlite3CompletionVtabInit propagates from sqlite3_create_module.
// Notes:
// - We rely on the actual completion.c being compiled/linked in the build so that
//   sqlite3CompletionVtabInit is resolved.
// - We provide a minimal forward declaration for sqlite3 and sqlite3_module to avoid
//   depending on the real SQLite headers in this isolated test environment.
// - We do not attempt to exercise the true macro-based branch (SQLITE_OMIT_VIRTUALTABLE)
//   at compile-time here; instead, we validate the default path where sqlite3_create_module is invoked.

#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3ext.h>
#include <cstring>


// Forward declarations to minimize dependency on sqlite3 headers.
// We only need pointer types; the internals are unused in this test.
struct sqlite3;
struct sqlite3_module;

// Declaration of the focal function under test (from completion.c)
extern "C" int sqlite3CompletionVtabInit(struct sqlite3* db);

// Mock/Stub for sqlite3_create_module to capture calls and control behavior.
// The real function has the signature:
// int sqlite3_create_module(sqlite3*, const char *zName, const struct sqlite3_module*, void*)
// We mirror that signature here to ensure correct linkage.
extern "C" {

// Global control variables for the mock
static const char* sqlite3_last_module_name = nullptr;
static const struct sqlite3_module* sqlite3_last_module_ptr = nullptr;
static bool sqlite3_create_module_was_called = false;
static int sqlite3_create_module_return_value = 0;

// A simple struct declaration to satisfy the expected type in the 3rd parameter.
// We do not touch its contents; the test only checks non-null pointer behavior.
struct sqlite3_module;

// The mock implementation
int sqlite3_create_module(struct sqlite3* db,
                          const char* zName,
                          const struct sqlite3_module* pModule,
                          void* pAux)
{
  sqlite3_create_module_was_called = true;
  sqlite3_last_module_name = zName;
  sqlite3_last_module_ptr = pModule;
  // Return the controlled value to simulate success/failure of module creation.
  return sqlite3_create_module_return_value;
}
} // extern "C"

// Helper test utilities
static void reset_mock_state() {
  sqlite3_last_module_name = nullptr;
  sqlite3_last_module_ptr = nullptr;
  sqlite3_create_module_was_called = false;
  sqlite3_create_module_return_value = 0;
}

// Test 1: Verify that sqlite3CompletionVtabInit calls sqlite3_create_module
// with the correct module name "completion" and propagates SQLITE_OK on success.
int test_sqlite3CompletionVtabInit_calls_create_module_true() {
  reset_mock_state();
  sqlite3_create_module_return_value = 0; // Simulate success
  // Call the focal function with a null db pointer (the mock does not dereference it)
  int rc = sqlite3CompletionVtabInit(nullptr);

  bool test_passed = true;
  if (rc != 0) {
    test_passed = false;
  }
  if (!sqlite3_create_module_was_called) {
    test_passed = false;
  }
  if (sqlite3_last_module_name == nullptr || std::strcmp(sqlite3_last_module_name, "completion") != 0) {
    test_passed = false;
  }
  if (sqlite3_last_module_ptr == nullptr) {
    test_passed = false;
  }

  if (test_passed) {
    std::printf("Test1 PASSED: sqlite3CompletionVtabInit invoked sqlite3_create_module with 'completion' and returned OK.\n");
    return 0;
  } else {
    std::printf("Test1 FAILED: sqlite3CompletionVtabInit did not behave as expected.\n");
    std::printf("  rc=%d, called=%d, last_name=%s, last_ptr=%p\n",
                rc,
                sqlite3_create_module_was_called ? 1 : 0,
                sqlite3_last_module_name ? sqlite3_last_module_name : "(null)",
                (void*)sqlite3_last_module_ptr);
    return 1;
  }
}

// Test 2: Ensure that a non-zero return from sqlite3_create_module propagates through
// sqlite3CompletionVtabInit (i.e., the RC is forwarded to the caller).
int test_sqlite3CompletionVtabInit_propagates_error() {
  reset_mock_state();
  sqlite3_create_module_return_value = -1; // Simulate failure in module creation
  int rc = sqlite3CompletionVtabInit(nullptr);

  bool test_passed = true;
  if (rc != -1) {
    test_passed = false;
  }
  if (!sqlite3_create_module_was_called) {
    test_passed = false;
  }
  if (sqlite3_last_module_name == nullptr || std::strcmp(sqlite3_last_module_name, "completion") != 0) {
    test_passed = false;
  }
  if (sqlite3_last_module_ptr == nullptr) {
    test_passed = false;
  }

  if (test_passed) {
    std::printf("Test2 PASSED: Error from sqlite3_create_module is propagated by sqlite3CompletionVtabInit.\n");
    return 0;
  } else {
    std::printf("Test2 FAILED: sqlite3CompletionVtabInit did not propagate error as expected.\n");
    std::printf("  rc=%d, called=%d, last_name=%s, last_ptr=%p\n",
                rc,
                sqlite3_create_module_was_called ? 1 : 0,
                sqlite3_last_module_name ? sqlite3_last_module_name : "(null)",
                (void*)sqlite3_last_module_ptr);
    return 1;
  }
}

// Entry point: run tests in a straightforward sequence.
// As required by the domain knowledge, we avoid any heavy testing frameworks and rely on standard I/O.
int main() {
  int failures = 0;

  std::printf("Starting sqlite3CompletionVtabInit unit tests (without GTest).\n");

  failures += test_sqlite3CompletionVtabInit_calls_create_module_true();
  failures += test_sqlite3CompletionVtabInit_propagates_error();

  if (failures == 0) {
    std::printf("All tests PASSED.\n");
  } else {
    std::printf("Total failures: %d\n", failures);
  }

  return failures;
}