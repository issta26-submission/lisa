// C++11 test harness for the focal C function: db_define_add
// This test suite mocks the minimal SQLite interaction (db_prepare, db_bind_text, db_step, sqlite3_reset, db_affirm_rc, g_debug)
// and validates the control flow of db_define_add under different scenarios without using GTest.
// The test code provides its own implementation of the dependencies to enable standalone compilation.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>
#include <cstdarg>
#include <stdarg.h>
#include <cstring>
#include <string>
#include <iostream>
#include <cassert>


// Lightweight mock types and constants to mirror the focal dependencies
typedef struct sqlite3_stmt sqlite3_stmt; // opaque placeholder for sqlite3_stmt

// Minimal sqlite3-style RC constants (mirroring sqlite3.h values sufficiently for tests)
enum { SQLITE_OK = 0, SQLITE_ERROR = 1, SQLITE_DONE = 101 };

// Global mock environment to simulate the original program state
struct Stmt {
  sqlite3_stmt *defIns;
};

struct Global {
  Stmt stmt;
};

// Global instance used by the focal method
Global g;

// Mock state tracking for tests
static bool db_prepare_called;
static bool sqlite3_reset_called;
static bool db_affirm_rc_called;
static int  last_rc_from_db_step;
static std::string bound_key_copy;      // last key bound via db_bind_text
static std::string last_debug_message;    // captured debug output
static bool debug_called;
static int  next_db_step_rc;             // controls return code of db_step in tests

// Forward declaration of the focal function under test
extern "C" void db_define_add(const char * zKey);

// Mocked dependency implementations (minimal behavior for testing)
extern "C" {

  // Simulate preparing a statement
  void db_prepare(sqlite3_stmt **pStmt, const char * zSql, ...)
  {
    (void)zSql; // not used in test
    db_prepare_called = true;
    // Allocate a tiny dummy statement object and attach to global state
    *pStmt = new sqlite3_stmt();
    g.stmt.defIns = *pStmt;
  }

  // Simulate binding text to a prepared statement (only capture the bound key)
  void db_bind_text(sqlite3_stmt * /*pStmt*/, int col, const char * zStr)
  {
    (void)col; // we only test binding at column 1
    bound_key_copy = (zStr != nullptr) ? zStr : "";
  }

  // Simulate stepping the prepared statement and return a predefined RC
  int db_step(sqlite3_stmt * /*pStmt*/)
  {
    last_rc_from_db_step = next_db_step_rc;
    return next_db_step_rc;
  }

  // Simulate resetting a prepared statement
  void sqlite3_reset(sqlite3_stmt * /*pStmt*/)
  {
    sqlite3_reset_called = true;
  }

  // Simulate assertion/notification of an RC issue
  void db_affirm_rc(int rc, const char * zMsg)
  {
    (void)zMsg; // message is not evaluated in tests; we track RC
    db_affirm_rc_called = true;
    last_rc_from_db_step = rc;
  }

  // Capture debug output
  void g_debug(int level, const char * fmt, ...)
  {
    (void)level;
    va_list ap;
    va_start(ap, fmt);
    char buffer[512];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);
    last_debug_message = buffer;
    debug_called = true;
  }
}

// Implementation of the focal function under test
extern "C" void db_define_add(const char * zKey){
  int rc;
  if(!g.stmt.defIns){
    db_prepare(&g.stmt.defIns,
               "INSERT OR REPLACE INTO def(k) VALUES(?)");
  }
  db_bind_text(g.stmt.defIns, 1, zKey);
  rc = db_step(g.stmt.defIns);
  if(SQLITE_DONE != rc){
    db_affirm_rc(rc, "Stepping INSERT on def");
  }
  g_debug(2,("define: %s\n",zKey));
  sqlite3_reset(g.stmt.defIns);
}

// Helper to reset the test environment
static void reset_test_env()
{
  g.stmt.defIns = nullptr;
  db_prepare_called = false;
  sqlite3_reset_called = false;
  db_affirm_rc_called = false;
  last_rc_from_db_step = SQLITE_OK;
  bound_key_copy.clear();
  last_debug_message.clear();
  debug_called = false;
  next_db_step_rc = SQLITE_DONE; // default to success
  // Ensure memory is cleaned between tests (avoid leaks in long runs)
  // Note: We allocate a dummy sqlite3_stmt in db_prepare; we'll delete it explicitly in tests.
}

// Test Case 1: When g.stmt.defIns is NULL, db_define_add should prepare, bind, step, and reset on success
static void test_case_prepare_and_success_path()
{
  reset_test_env();
  // Ensure initial state: defIns is null
  g.stmt.defIns = nullptr;

  // Set next rc to SQLITE_DONE to simulate a successful INSERT/REPLACE
  next_db_step_rc = SQLITE_DONE;

  const char * testKey = "unit_test_key_A";

  // Execute
  db_define_add(testKey);

  // Validations
  assert(db_prepare_called && "db_define_add should call db_prepare when defIns is NULL");
  assert(g.stmt.defIns != nullptr && "defIns should be created by db_prepare");
  assert(bound_key_copy == testKey && "db_bind_text should be called with the provided key");
  assert(next_db_step_rc == SQLITE_DONE && "db_step should return SQLITE_DONE for success case");
  assert(!db_affirm_rc_called && "db_affirm_rc should not be called on SQLITE_DONE");
  // Verify debug output contains the key
  assert(debug_called && "g_debug should be invoked");
  assert(last_debug_message.find("define: unit_test_key_A") != std::string::npos);

  // Cleanup
  if (g.stmt.defIns) {
    delete g.stmt.defIns;
    g.stmt.defIns = nullptr;
  }

  // Additional sanity: ensure reset was invoked
  assert(sqlite3_reset_called && "sqlite3_reset should be invoked on the prepared stmt");
}

// Test Case 2: When db_step returns an error, db_define_add should call db_affirm_rc
static void test_case_error_from_step()
{
  reset_test_env();
  g.stmt.defIns = nullptr;
  // Configure to return an error from db_step
  next_db_step_rc = SQLITE_ERROR;

  const char * testKey = "unit_test_key_B";

  db_define_add(testKey);

  // Validations
  // We still expect prepare and bind to occur
  assert(db_prepare_called && "db_define_add should call db_prepare when defIns is NULL");
  assert(g.stmt.defIns != nullptr);

  // Check that the error path was triggered
  assert(db_affirm_rc_called && "db_affirm_rc should be called when rc != SQLITE_DONE");
  assert(last_rc_from_db_step == SQLITE_ERROR);

  // Debug output should still be produced
  assert(debug_called && "g_debug should be invoked even on error path");
  assert(last_debug_message.find("define: unit_test_key_B") != std::string::npos);

  // Cleanup
  if (g.stmt.defIns) {
    delete g.stmt.defIns;
    g.stmt.defIns = nullptr;
  }

  // Ensure reset was invoked
  assert(sqlite3_reset_called && "sqlite3_reset should be invoked on the prepared stmt");
}

// Test Case 3: When defIns is already initialized, db_define_add should NOT call db_prepare again
static void test_case_no_reprepare_when_already_initialized()
{
  reset_test_env();
  // Pre-create a dummy statement to simulate an already prepared statement
  g.stmt.defIns = new sqlite3_stmt();
  db_prepare_called = false; // reset flag since we expect no new preparation

  // Set next rc to SQLITE_DONE to simulate successful step
  next_db_step_rc = SQLITE_DONE;

  const char * testKey = "unit_test_key_C";

  db_define_add(testKey);

  // Validations
  // There should be no new call to db_prepare
  assert(!db_prepare_called && "db_define_add should NOT call db_prepare when defIns already exists");
  // Ensure binding occurred with the new key
  assert(bound_key_copy == testKey);

  // Ensure reset was called
  assert(sqlite3_reset_called && "sqlite3_reset should be invoked on the prepared stmt");

  // Cleanup
  if (g.stmt.defIns) {
    delete g.stmt.defIns;
    g.stmt.defIns = nullptr;
  }
}

// Entry point for running all tests
int main(void)
{
  std::cout << "Starting unit tests for db_define_add (C focal method) with mocked environment...\n";

  test_case_prepare_and_success_path();
  test_case_error_from_step();
  test_case_no_reprepare_when_already_initialized();

  std::cout << "All tests completed successfully.\n";
  return 0;
}