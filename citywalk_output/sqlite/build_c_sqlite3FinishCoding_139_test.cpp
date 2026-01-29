// Unit test suite for sqlite3FinishCoding (Step 2 & 3: comprehensive tests with
// coverage-oriented branches). This test suite is written in C++11 without
// using GTest. It relies on the project's internal SQLite headers to access
// the Parse, Vdbe and related internal structures. The tests are designed to be
// executed from main() and use simple assertions with clear explanations.
//
// Notes:
// - This test assumes the build environment provides the internal SQLite headers
//   (e.g., sqliteInt.h, parse.h) and compiles alongside the SQLite sources.
// - The tests aim to exercise key early-branch behavior of sqlite3FinishCoding:
//   a) When there is no active VDBE and the database is busy, finish with
//      SQLITE_DONE (early return).
//   b) When there is an error and a prior memory allocation failure, finish
//      with SQLITE_NOMEM.
// - We avoid terminating assertions (use normal asserts) to maximize code coverage
//   execution paths in the tested function.
//

#include <parse.h>
#include <vdbe.h>
#include <cstdio>
#include <sqlite3.h>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>
#include <cassert>


// Include internal SQLite headers to access the internal data structures.
// These headers are typically available when compiling SQLite from source.
// If testing in an environment where these headers are located in a different
// path, adjust include paths accordingly.
extern "C" {
}

// The test uses SQLite's public constants for return codes.


// Lightweight assertion helper that prints a message on failure and aborts.
#define TEST_ASSERT(cond, msg)                                 \
  do {                                                         \
    if(!(cond)) {                                              \
      fprintf(stderr, "TEST FAILED: %s\n", (msg));            \
      exit(EXIT_FAILURE);                                    \
    }                                                          \
  } while(0)


// Helper to create a minimal sqlite3 object and wire it to a Parse.
// We purposefully avoid driving the full parser state; we only establish
// the fields that sqlite3FinishCoding inspects for the early branches.
// This lightweight setup is designed to trigger specific branches in
// sqlite3FinishCoding without requiring a full SQL compilation pipeline.
static void setupMinimalDbAndParseForTest(Parse *pParse, sqlite3 *db)
{
  // Clear structures
  memset(pParse, 0, sizeof(Parse));
  memset(db, 0, sizeof(sqlite3));

  // Wire relationships that sqlite3FinishCoding checks
  pParse->db = db;
  db->pParse = pParse;

  // Basic invariant expected by sqlite3FinishCoding
  pParse->pToplevel = 0;
  db->init.busy = 0;
  db->mallocFailed = 0;
  // After this point, the specific scenario will override fields as needed by the test.
}


// Test 1: When there is no active VDBE (pParse->pVdbe == 0) and the DB is busy,
// sqlite3FinishCoding should set rc to SQLITE_DONE and return early.
static void test_finishCoding_noVdbe_busyTerminatesWithDone()
{
  Parse pParse;
  sqlite3 db;
  setupMinimalDbAndParseForTest(&pParse, &db);

  // Scenario: pParse->nested is false, no error; nested is checked first.
  pParse.nested = 0;
  pParse.nErr = 0;

  // Ensure VDBE is not yet created (v==0)
  pParse.pVdbe = 0;

  // Simulate a busy database: db->init.busy = 1
  db.init.busy = 1;
  // Ensure a cross-check that the internal assertion does not get tripped
  // by setting pParse->pToplevel to 0 (already 0).

  // Call the function under test
  sqlite3FinishCoding(&pParse);

  // Expected: rc == SQLITE_DONE
  TEST_ASSERT(pParse.rc == SQLITE_DONE, "Expected SQLITE_DONE when v==0 and db->init.busy==1");
}

// Test 2: When there is an error in the parse (nErr > 0) and a memory
// allocation failure has occurred (db->mallocFailed == 1), rc should be SQLITE_NOMEM.
static void test_finishCoding_nErrWithMemFailureSetsNOMEM()
{
  Parse pParse;
  sqlite3 db;
  setupMinimalDbAndParseForTest(&pParse, &db);

  // Scenario: error present
  pParse.nErr = 1;
  pParse.nNested = 0; // ensure no confusion with nested state
  pParse.pToplevel = 0;

  // Simulate memory failure in db
  db.init.busy = 0;
  db.mallocFailed = 1;
  pParse.nTableLock = 0;
  pParse.pVdbe = 0;

  // Ensure the test does not crash
  sqlite3FinishCoding(&pParse);

  // Expected: rc == SQLITE_NOMEM
  TEST_ASSERT(pParse.rc == SQLITE_NOMEM, "Expected SQLITE_NOMEM when nErr>0 and mallocFailed==1");
}


// Test 3: When pParse->bReturning is set with a non-empty Returning list (nRetCol > 0),
// the code path should execute the returning-rows logic (OP_FkCheck, OP_Rewind, etc.)
// This test verifies that the code path is entered without crashing and the test
// completes with rc set (SQLITE_DONE under normal completion).
static void test_finishCoding_withReturningBranch_entersReturningCode()
{
  Parse pParse;
  sqlite3 db;
  setupMinimalDbAndParseForTest(&pParse, &db);

  // Prepare a minimal Returning object
  Returning ret;
  memset(&ret, 0, sizeof(Returning));
  ret.nRetCol = 1;
  ret.iRetCur = 1;
  ret.iRetReg = 0;

  // Link pParse to a Returning structure
  pParse.bReturning = 1;
  pParse.u1.pReturning = &ret;

  // Provide enough state to allow sqlite3FinishCoding to reach the Returning branch.
  // Note: This test focuses on entering the Returning code path, not on the exact
  // VM op generation. We avoid system-wide side effects by ensuring no errors are set.
  pParse.nErr = 0;
  pParse.pToplevel = 0;

  // We still need a VDBE object since the function later asserts v != 0 for the Returning path.
  // Create a tiny dummy Vdbe object via the library's allocator if available.
  // If not available in the test environment, skip this test gracefully.
  // We attempt to get a Vdbe via sqlite3GetVdbe; if it fails, we skip.
  pParse.pVdbe = sqlite3GetVdbe(&pParse);
  if(pParse.pVdbe == 0) {
    // If we cannot create a Vdbe, skip the test gracefully
    fprintf(stderr, "Skipping test: unable to create Vdbe in test environment.\n");
    return;
  }

  // Ensure db->init.busy is not interfering
  db.init.busy = 0;
  db.mallocFailed = 0;

  // Call the function under test
  sqlite3FinishCoding(&pParse);

  // If the code path executed fully, rc should be SQLITE_DONE
  // Depending on the exact environment, the test may require additional setup.
  if(pParse.rc != SQLITE_DONE) {
    fprintf(stderr, "Warning: Returning branch did not finish with SQLITE_DONE as expected in this environment. rc=%d\n", pParse.rc);
  }
  TEST_ASSERT(pParse.rc == SQLITE_DONE, "Expected SQLITE_DONE after completing Returning branch execution");
}


// Main test runner
int main(void)
{
  // Run tests in a deterministic order with explanations in comments.
  test_finishCoding_noVdbe_busyTerminatesWithDone();
  test_finishCoding_nErrWithMemFailureSetsNOMEM();
  test_finishCoding_withReturningBranch_entersReturningCode();

  printf("All tests completed. If any assertion failed, the program would have aborted.\n");
  return 0;
}