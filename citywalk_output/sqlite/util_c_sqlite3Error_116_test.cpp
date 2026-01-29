// Test suite for the focal method: sqlite3Error
// This test is self-contained and does not depend on external GTest.
// It exercises true/false branches of sqlite3Error by inspecting the sqlite3
// struct fields and a tracker for whether sqlite3ErrorFinish is invoked.

#include <math.h>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cassert>


// Minimal mock of the sqlite3 structure used by sqlite3Error.
// This is a focused subset sufficient for unit testing sqlite3Error behavior.
struct sqlite3 {
  int errCode;
  void *pErr;
  int errByteOffset;
};

// Tracker to verify if sqlite3ErrorFinish is invoked and with what parameters.
static int g_lastErrorFinishCalledWith = -1;
static sqlite3 *g_lastErrorFinishDb = nullptr;

// Mock implementation of the static function sqlite3ErrorFinish used by sqlite3Error.
// In the actual project this function is static and internal to the file.
// Here we provide a test-visible mock to observe invocation.
static void sqlite3ErrorFinish(sqlite3 *db, int err_code) {
  g_lastErrorFinishCalledWith = err_code;
  g_lastErrorFinishDb = db;
}

// The focal method under test (copied here to enable isolated unit testing).
// In the real project this would be defined in util.c.
// Behavior:
// - always set db->errCode = err_code
// - if (err_code != 0 || db->pErr != NULL) call sqlite3ErrorFinish(db, err_code)
// - else set db->errByteOffset = -1
void sqlite3Error(sqlite3 *db, int err_code){
  assert( db != 0 );
  db->errCode = err_code;
  if( err_code || db->pErr ){
    sqlite3ErrorFinish(db, err_code);
  }else{
    db->errByteOffset = -1;
  }
}

// Helper to reset the error finish tracker before each test.
static void resetFinishTracker() {
  g_lastErrorFinishCalledWith = -1;
  g_lastErrorFinishDb = nullptr;
}

// Test 1: Non-zero err_code with no pErr should call sqlite3ErrorFinish and set errCode.
// Expected: db->errCode == err_code; db->errByteOffset unchanged; sqlite3ErrorFinish invoked with err_code.
static void test_sqlite3Error_nonzero_errCode() {
  // Setup
  resetFinishTracker();
  sqlite3 db;
  db.errCode = 0;
  db.pErr = nullptr;
  db.errByteOffset = 123; // arbitrary initial value

  // Exercise
  sqlite3Error(&db, 5);

  // Verify
  bool ok = true;
  ok = ok && (db.errCode == 5);
  ok = ok && (db.errByteOffset == 123);                  // unchanged
  ok = ok && (g_lastErrorFinishCalledWith == 5);         // finish was invoked
  ok = ok && (g_lastErrorFinishDb == &db);

  if (ok) std::cout << "PASS: test_sqlite3Error_nonzero_errCode\n";
  else     std::cout << "FAIL: test_sqlite3Error_nonzero_errCode\n";
}

// Test 2: Zero err_code with non-null pErr should call sqlite3ErrorFinish and not touch errByteOffset.
// Expected: db->errCode == 0; db->errByteOffset unchanged; sqlite3ErrorFinish invoked with 0.
static void test_sqlite3Error_zero_errCode_with_pErr() {
  // Setup
  resetFinishTracker();
  sqlite3 db;
  db.errCode = -1;
  db.pErr = (void*)0x1; // non-null to trigger the if-condition
  db.errByteOffset = 777;

  // Exercise
  sqlite3Error(&db, 0);

  // Verify
  bool ok = true;
  ok = ok && (db.errCode == 0);
  ok = ok && (db.errByteOffset == 777);                   // unchanged
  ok = ok && (g_lastErrorFinishCalledWith == 0);          // finish was invoked
  ok = ok && (g_lastErrorFinishDb == &db);

  if (ok) std::cout << "PASS: test_sqlite3Error_zero_errCode_with_pErr\n";
  else     std::cout << "FAIL: test_sqlite3Error_zero_errCode_with_pErr\n";
}

// Test 3: Zero err_code with null pErr should set errByteOffset to -1 and not call sqlite3ErrorFinish.
// Expected: db->errCode == 0; db->errByteOffset == -1; sqlite3ErrorFinish not invoked.
static void test_sqlite3Error_zero_errCode_no_pErr() {
  // Setup
  resetFinishTracker();
  sqlite3 db;
  db.errCode = 42;
  db.pErr = nullptr;
  db.errByteOffset = 555;

  // Exercise
  sqlite3Error(&db, 0);

  // Verify
  bool ok = true;
  ok = ok && (db.errCode == 0);
  ok = ok && (db.errByteOffset == -1);
  ok = ok && (g_lastErrorFinishCalledWith == -1);
  ok = ok && (g_lastErrorFinishDb == nullptr);

  if (ok) std::cout << "PASS: test_sqlite3Error_zero_errCode_no_pErr\n";
  else     std::cout << "FAIL: test_sqlite3Error_zero_errCode_no_pErr\n";
}

int main() {
  // Run tests
  test_sqlite3Error_nonzero_errCode();
  test_sqlite3Error_zero_errCode_with_pErr();
  test_sqlite3Error_zero_errCode_no_pErr();

  // Simple summary
  // Note: Each test prints PASS/FAIL. In a more elaborate harness we'd count totals.
  return 0;
}