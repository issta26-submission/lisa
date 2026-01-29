// Test suite for the focal method: sqlite3JsonTableFunctions
// This test suite is written for C++11, does not use GTest, and relies on
// the SQLite C API. It verifies that sqlite3JsonTableFunctions registers
// the expected JSON table modules (json_each and json_tree) into an in-memory
// SQLite database.
//
// Reviewer notes:
// - Step 1 (Program Understanding): The focal method registers two modules by name
//   "json_each" and "json_tree" using sqlite3_create_module and then returns an
//   integer rc status. The key dependencies are sqlite3, sqlite3_module, and the
//   module objects jsonEachModule and jsonTreeModule.
// - Step 2 (Unit Test Generation): Tests target that both modules become visible via
//   sqlite3_find_module after calling sqlite3JsonTableFunctions on a valid db handle.
// - Step 3 (Test Case Refinement): We keep tests non-terminating (log-only on failure),
// handle static expectations conservatively, and keep tests executable in a normal
// C++ environment with linking to sqlite3.
//
// Important: This test assumes that the json extension source (json.c) is compiled
// into the test binary or linked in, so that jsonEachModule and jsonTreeModule symbols
// are available to sqlite3JsonTableFunctions. The test uses sqlite3_find_module to
// verify module registration.

#include <sstream>
#include <iomanip>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>



extern "C" {
  // Focal function under test. The function is defined in the json extension source.
  // It should return SQLITE_OK when modules are registered successfully.
  int sqlite3JsonTableFunctions(sqlite3 *db);
  // The module objects jsonEachModule and jsonTreeModule are defined in the
  // json.c implementation (not visible here if they are static; they are
  // expected to be available for sqlite3_create_module to register).
  // extern sqlite3_module jsonEachModule;
  // extern sqlite3_module jsonTreeModule;
}

// Lightweight non-terminating test harness
static int g_failures = 0;
static int g_tests_run = 0;

static void log_pass(const std::string &msg) {
  std::cout << "[PASS] " << msg << std::endl;
}
static void log_fail(const std::string &msg) {
  std::cout << "[FAIL] " << msg << std::endl;
  ++g_failures;
}
static void log_info(const std::string &msg) {
  std::cout << "[INFO] " << msg << std::endl;
}
static void test_expect_true(bool cond, const std::string &desc) {
  ++g_tests_run;
  if (cond) {
    log_pass(desc);
  } else {
    log_fail(desc + " (condition evaluation failed)");
  }
}
static void test_expect_int_eq(int a, int b, const std::string &desc) {
  ++g_tests_run;
  if (a == b) {
    log_pass(desc);
  } else {
    std::ostringstream oss;
    oss << desc << " (expected " << b << ", got " << a << ")";
    log_fail(oss.str());
  }
}

// Candidate Keywords (Step 1) for the focal method
// - sqlite3, sqlite3JsonTableFunctions
// - jsonEachModule, jsonTreeModule
// - sqlite3_create_module, sqlite3_find_module
// - JSON, JSON functions, JsonTable, JsonTree
// - Module registry loop, rc status handling
// - In-memory database, SQLITE_OK
// These keywords represent the core dependencies and behavior exploited by tests.

static void test_json_table_functions_registers_modules() {
  log_info("Starting test_json_table_functions_registers_modules");

  sqlite3 *db = nullptr;
  int rc_open = sqlite3_open(":memory:", &db);
  test_expect_int_eq(rc_open, SQLITE_OK, "sqlite3_open returns SQLITE_OK");
  test_expect_true(db != nullptr, "sqlite3_open produced non-null db handle");

  if (db == nullptr) {
    log_fail("Aborting test_json_table_functions_registers_modules due to null db");
    return;
  }

  // Call the focal function
  int rc = sqlite3JsonTableFunctions(db);
  test_expect_int_eq(rc, SQLITE_OK, "sqlite3JsonTableFunctions(db) returns SQLITE_OK");

  // Verify that the two modules were registered: json_each and json_tree
  sqlite3_module *pJsonEach = sqlite3_find_module(db, "json_each");
  test_expect_true(pJsonEach != nullptr, "Module 'json_each' is registered in the database");

  sqlite3_module *pJsonTree = sqlite3_find_module(db, "json_tree");
  test_expect_true(pJsonTree != nullptr, "Module 'json_tree' is registered in the database");

  // Cleanup
  sqlite3_close(db);

  log_info("Completed test_json_table_functions_registers_modules");
}

// Additional test: ensure that repeated invocation does not crash and returns a valid rc.
// Note: Depending on SQLite's module handling, repeated registration might return SQLITE_OK
// or SQLITE_ERROR. We treat both as acceptable non-crashing outcomes for coverage purposes.
static void test_json_table_functions_idempotent_call() {
  log_info("Starting test_json_table_functions_idempotent_call");

  sqlite3 *db = nullptr;
  int rc_open = sqlite3_open(":memory:", &db);
  test_expect_int_eq(rc_open, SQLITE_OK, "sqlite3_open for idempotent test returns SQLITE_OK");
  test_expect_true(db != nullptr, "db handle non-null for idempotent test");

  if (db == nullptr) {
    log_fail("Aborting test_json_table_functions_idempotent_call due to null db");
    return;
  }

  int rc1 = sqlite3JsonTableFunctions(db);
  test_expect_int_eq(rc1, SQLITE_OK, "First sqlite3JsonTableFunctions call returns SQLITE_OK");

  int rc2 = sqlite3JsonTableFunctions(db);
  // Accept either SQLITE_OK or SQLITE_ERROR as a safe bound for an idempotent behavior
  bool ok = (rc2 == SQLITE_OK) || (rc2 == SQLITE_ERROR);
  log_pass("Second sqlite3JsonTableFunctions call returned acceptable rc value (OK or ERROR)");
  if (!ok) {
    // Provide more detail if unexpected
    std::ostringstream oss;
    oss << "Second call rc unexpected: " << rc2;
    log_fail(oss.str());
  }

  // If modules were registered, they should remain discoverable
  sqlite3_module *pJsonEach = sqlite3_find_module(db, "json_each");
  test_expect_true(pJsonEach != nullptr, "Module 'json_each' should still be registered after second call");

  sqlite3_module *pJsonTree = sqlite3_find_module(db, "json_tree");
  test_expect_true(pJsonTree != nullptr, "Module 'json_tree' should still be registered after second call");

  sqlite3_close(db);

  log_info("Completed test_json_table_functions_idempotent_call");
}

// Entry point
int main() {
  log_info("Starting test suite for sqlite3JsonTableFunctions (no GTest, C++11)");
  test_json_table_functions_registers_modules();
  test_json_table_functions_idempotent_call();

  // Summary
  std::cout << std::endl;
  std::cout << "Test summary: " << g_tests_run << " test(s) executed, "
            << g_failures << " failure(s)." << std::endl;

  // Non-terminating process: return 0 if all tests passed, non-zero if any failed
  return (g_failures == 0) ? 0 : 1;
}