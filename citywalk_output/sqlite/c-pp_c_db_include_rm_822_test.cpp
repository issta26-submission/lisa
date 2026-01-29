/*
  C++11 unit test suite for the focal method:
  void db_include_rm(const char * zKey)

  Notes:
  - This test suite is designed to be used in a C++11 environment without GoogleTest.
  - It relies on the project's c-pp.c (the focal file) being compiled in the same translation unit
    or being linked with this test harness. To maximize compatibility in a standalone fashion,
    the test harness includes c-pp.c directly via #include "c-pp.c".
  - We provide lightweight, C-compatible stubs for the database helper functions and the global
    state (g) used by db_include_rm. These stubs allow deterministic testing of:
      a) Prepared statement creation (db_prepare) when g.stmt.inclDel is NULL,
      b) Successful step (SQLITE_DONE) and binding behavior,
      c) Error handling path when rc != SQLITE_DONE,
      d) Reuse of an already-prepared statement (i.e., when g.stmt.inclDel is non-NULL).
  - We implement a tiny, self-contained test harness (no GTest) with simple test cases and
    an execution in main(), printing a summary of test results.
  - Static members and functions in the focal code are treated as part of the test environment.
    We access g via an extern "C" declaration that matches the expected structure used by db_include_rm.
*/

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <c-pp.c>
#include <cassert>


// 1) Bring in the focal method implementation and its dependencies.
//    We assume the project's build can compile c-pp.c in the same TU or link it alongside.
//    If your build system compiles c-pp.c separately, ensure it is linked with this test code.
extern "C" void db_include_rm(const char * zKey);

// Include the focal file. Depending on your build setup, you might instead compile c-pp.c separately.
// If including causes multiple-definition issues for stubs, you can adapt by compiling separately.

// 2) Provide minimal C-compatible stubs for the dependencies used by db_include_rm.
//    The real code relies on a global 'g' with a nested structure containing inclDel
//    (sqlite3_stmt*) and on a handful of helper functions (db_prepare, db_bind_text, db_step, etc.)
//    We implement lightweight, deterministic behaviors to test db_include_rm logic.

#ifndef SQLITE_DONE
#define SQLITE_DONE 101
#endif

// Forward declare sqlite3_stmt to avoid requiring the real sqlite3.h in this test harness.
// The focal code uses sqlite3_stmt*; we only need the pointer type here for the tests.
struct sqlite3_stmt;

// Access to the same global 'g' expected by db_include_rm.
// We declare a compatible external structure. The actual layout must match the production code.
// We provide a minimal layout sufficient for db_include_rm tests: g.stmt.inclDel.
extern "C" {
  struct GGlobalStmt {
    sqlite3_stmt *inclDel;
  };
  struct GGlobal {
    GGlobalStmt stmt;
  };
  // The actual global variable in c-pp.c is named 'g'. We declare it here as an extern so we can
  // manipulate its fields from tests.
  extern GGlobal g;
}

// 2.a) Test-state tracking variables and helper flags
static bool prepared_called = false;
static bool affirms_called = false;
static bool clear_bindings_called = false;
static bool reset_called = false;
static const char* last_bound_text = nullptr;
static const char* last_sql_used = nullptr;
static int last_rc_affirm = 0;

// 2.b) Stubbed sqlite3_bind_text
extern "C" void db_bind_text(sqlite3_stmt *pStmt, int col, const char * zStr){
  // For tests, capture the bound text (we assume a single key is bound for test clarity)
  (void)pStmt;
  (void)col;
  last_bound_text = zStr;
}

// 2.c) Stubbed db_prepare
//       It should allocate or assign a dummy statement object and set the pointer.
extern "C" void db_prepare(sqlite3_stmt **pStmt, const char * zSql, ...){
  prepared_called = true;
  // For tests, we simulate that a statement object was prepared by assigning a non-null pointer.
  // The actual type isn't important for the tests; we only need to verify pointer non-null and
  // that the sql string was utilized. We also record the SQL for potential verification.
  (void)zSql;
  *pStmt = reinterpret_cast<sqlite3_stmt*>(0xDEADBEEF); // dummy non-null pointer
  last_sql_used = zSql; // store the SQL string used for the prepared statement
}

// 2.d) Stubbed db_step
static int step_rc = SQLITE_DONE;
extern "C" int db_step(sqlite3_stmt *pStmt){
  (void)pStmt;
  return step_rc;
}

// 2.e) Stubbed db_affirm_rc
extern "C" void db_affirm_rc(int rc, const char * zMsg){
  affirms_called = true;
  last_rc_affirm = rc;
  // We could log zMsg if desired; not strictly needed for assertions.
  (void)zMsg;
}

// 2.f) Stubbed g_debug
extern "C" void g_debug(int level, const char * fmt, ...){
  // No-op for testing; could be extended to capture log messages if needed.
  (void)level;
  (void)fmt;
}

// 2.g) Stubbed sqlite3_clear_bindings and sqlite3_reset
extern "C" int sqlite3_clear_bindings(sqlite3_stmt *pStmt){
  (void)pStmt;
  clear_bindings_called = true;
  return 0;
}
extern "C" int sqlite3_reset(sqlite3_stmt *pStmt){
  (void)pStmt;
  reset_called = true;
  return 0;
}

// 3) Minimal wrapper to initialize the test environment and the global g structure.
//    We provide a tiny mock of the expected g structure so that db_include_rm can operate on it.
extern "C" GGlobal g = { { nullptr } };

// 4) Test helpers and test cases

// Simple result container for test harness
struct TestResult {
  std::string name;
  bool passed;
  std::string message;
};

// Helper to log a test result
static void log_result(std::vector<TestResult>& results,
                       const std::string& name, bool passed, const std::string& message = "")
{
  results.push_back({name, passed, message});
}

// Test 1: When g.stmt.inclDel is NULL and db_step returns SQLITE_DONE, ensure normal deletion path
static void test_incl_rm_creates_prepare_and_steps_done(std::vector<TestResult>& results) {
  // Reset test state
  prepared_called = false;
  affirms_called = false;
  clear_bindings_called = false;
  reset_called = false;
  last_bound_text = nullptr;
  last_sql_used = nullptr;
  step_rc = SQLITE_DONE;

  // The initial state: no prepared statement
  g.stmt.inclDel = nullptr;

  // Call the function under test
  db_include_rm("path/to/file.txt");

  // Assertions
  bool ok = prepared_called && reset_called && clear_bindings_called && !affirms_called &&
            (g.stmt.inclDel != nullptr) &&
            (last_bound_text != nullptr) &&
            (strcmp(last_bound_text, "path/to/file.txt") == 0);

  if(!ok){
    log_result(results, "test_incl_rm_creates_prepare_and_steps_done",
               false,
               "Expected: prepare called, statement created, bind occurred with provided key, "
               "step returned SQLITE_DONE, no affirm_rc call, bindings reset.");
  } else {
    log_result(results, "test_incl_rm_creates_prepare_and_steps_done", true);
  }
}

// Test 2: When g.stmt.inclDel is NULL and db_step returns non-DONE, ensure error path is taken
static void test_incl_rm_error_path_calls_affirm_rc(std::vector<TestResult>& results) {
  // Reset test state
  prepared_called = false;
  affirms_called = false;
  clear_bindings_called = false;
  reset_called = false;
  last_bound_text = nullptr;
  last_sql_used = nullptr;
  step_rc = 123; // not SQLITE_DONE

  // Ensure statement is not prepared yet
  g.stmt.inclDel = nullptr;

  // Call the function under test
  db_include_rm("err_key");

  // Assertions
  bool ok = prepared_called && affirms_called && (g.stmt.inclDel != nullptr) &&
            clear_bindings_called && reset_called &&
            (last_bound_text != nullptr) &&
            (strcmp(last_bound_text, "err_key") == 0);

  if(!ok){
    log_result(results, "test_incl_rm_error_path_calls_affirm_rc",
               false,
               "Expected: prepare called, step returns non-DONE, db_affirm_rc invoked, "
               "bindings cleared and reset, key bound correctly.");
  } else {
    log_result(results, "test_incl_rm_error_path_calls_affirm_rc", true);
  }
}

// Test 3: When g.stmt.inclDel is already prepared (non-NULL), ensure no re-preparation occurs
static void test_incl_rm_reuse_prepared_no_reprepare(std::vector<TestResult>& results) {
  // Reset test state
  prepared_called = false;
  affirms_called = false;
  clear_bindings_called = false;
  reset_called = false;
  last_bound_text = nullptr;
  last_sql_used = nullptr;
  step_rc = SQLITE_DONE;

  // Simulate an already-prepared statement
  g.stmt.inclDel = reinterpret_cast<sqlite3_stmt*>(0x1234);

  // Call the function under test
  db_include_rm("existing_key");

  // Assertions: should not call db_prepare, but should still perform step, bind, and cleanup
  bool ok = !prepared_called && reset_called && clear_bindings_called &&
            (last_bound_text != nullptr) && (strcmp(last_bound_text, "existing_key") == 0);

  if(!ok){
    log_result(results, "test_incl_rm_reuse_prepared_no_reprepare",
               false,
               "Expected: no prepare call when inclDel already set, but bindings and cleanup occur and key bound.");
  } else {
    log_result(results, "test_incl_rm_reuse_prepared_no_reprepare", true);
  }
}

// 5) Main test runner
int main() {
  std::vector<TestResult> results;

  // Run tests
  test_incl_rm_creates_prepare_and_steps_done(results);
  test_incl_rm_error_path_calls_affirm_rc(results);
  test_incl_rm_reuse_prepared_no_reprepare(results);

  // Report results
  int passed = 0;
  for (auto &r : results) {
    if (r.passed) {
      std::cout << "[PASS] " << r.name << std::endl;
      ++passed;
    } else {
      std::cout << "[FAIL] " << r.name << " - " << r.message << std::endl;
    }
  }

  std::cout << "Tests passed: " << passed << "/" << results.size() << std::endl;

  // Return 0 if all tests passed, non-zero otherwise
  return (passed == static_cast<int>(results.size())) ? 0 : 1;
} 

/*
  Explanation of testing approach and coverage:

  - Test 1 covers the true-branch of the key predicate:
    if (!g.stmt.inclDel) { db_prepare(...); }
    and the typical success path (SQLITE_DONE from db_step) including
    binding, cleanup, and the debug path.

  - Test 2 covers the false-branch of the SQLITE_DONE predicate:
    rc != SQLITE_DONE leads to db_affirm_rc with the given message.
    We assert that the error-reporting path is invoked, and that
    the key is bound and a prepared statement exists.

  - Test 3 covers the scenario where a statement is already prepared before
    calling db_include_rm. It asserts that db_prepare is not invoked again
    (no re-preparation) and that normal cleanup happens with the new key bound.

  These tests exercise:
  - Binding behavior (db_bind_text)
  - Preparation path (db_prepare)
  - Step result handling (db_step via rc)
  - Error reporting path (db_affirm_rc)
  - Cleanup (sqlite3_clear_bindings, sqlite3_reset)
  - Debug-path invocation (g_debug)

  Important notes and caveats:
  - The test harness relies on the project allowing the c-pp.c file to be included
    directly for testing in the same translation unit. If your build system
    compiles c-pp.c separately, you should adapt by linking this test against
    the compiled object and providing the appropriate stubs in a separate
    compilation unit (instead of including c-pp.c here).
  - The test uses a minimal, deterministic set of stubs to avoid requiring a full
    SQLite runtime during unit testing. In environments where you have a real
    SQLite library linked, the stubs should be replaced by the actual implementations.
*/