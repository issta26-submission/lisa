// C++11 unit test harness for the focal function testDelete from lsmtest_main.c
// This test focuses on the control flow of the predicate "if (*pRc==0)" and
// verifies that testDelete calls tdb_delete exactly when *pRc is 0.
// It uses a lightweight fake TestDb structure and a mock tdb_delete to observe
// call counts without requiring the full database implementation.
//
// Notes:
// - We rely on weak symbol override for tdb_delete so that, in environments where
//   the real tdb_delete is present, it will take precedence. If not, our mock
//   implementation will be used.
// - We avoid triggering non-zero rc paths since testSetError(rc) would invoke
//   test_failed() (which asserts in the original file). Therefore, the tests
//   cover the true/false branches of the if predicate and the side-effects
//   on the call count and *pRc when rc==0.

#include <lsmtest.h>
#include <cstdio>
#include <sys/time.h>
#include <sqlite3.h>
#include <sys/resource.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Forward declaration of the focal function from the C file.
// The symbol is C-linkage in the project; we declare it with extern "C".
extern "C" void testDelete(struct TestDb *pDb, void *pKey, int nKey, int *pRc);

// Lightweight TestDb structure used by the test.
// This mirrors the minimal interface the mock tdb_delete relies on.
struct TestDb {
  int returnCodeForDelete; // RC that the mock tdb_delete should return
  int deleteCalled;        // Tracks how many times tdb_delete is invoked for this DB
  int dummy;                // Placeholder to mimic potential real members
};

// Global counter to observe how many times the mock tdb_delete is invoked
static int g_delete_call_count = 0;

// Mock/tweakable tdb_delete function.
// Declared as weak so that it can be overridden by a real implementation if present.
// If overridden by a real implementation, this weak definition will be ignored.
extern "C" int tdb_delete(TestDb* pDb, void* pKey, int nKey) __attribute__((weak));
extern "C" int tdb_delete(TestDb* pDb, void* pKey, int nKey)
{
  // Increment global counter and per-DB counter to observe behavior.
  ++g_delete_call_count;
  if (pDb) {
    pDb->deleteCalled += 1;
  }
  // Return the RC specified by the fake DB; if none set, default to 0.
  return (pDb ? pDb->returnCodeForDelete : 0);
}

// Helper: wrapper to invoke the focal function in a simple test harness
static void call_testDelete(TestDb *pDb, int valueKey, int *pRc)
{
  // Use a pointer to the key value as the key; length is passed as nKey.
  testDelete(pDb, &valueKey, sizeof(valueKey), pRc);
}

// Test 1: When *pRc == 0, testDelete should call tdb_delete exactly once
// and propagate the rc (via *pRc) returned by tdb_delete.
static bool test_delete_calls_when_rc_zero()
{
  g_delete_call_count = 0;
  TestDb db;
  db.returnCodeForDelete = 0; // mock returns success
  db.deleteCalled = 0;
  int rc = 0;
  int keyVal = 0x1234;
  call_testDelete(&db, keyVal, &rc);

  bool ok = true;
  if (g_delete_call_count != 1) {
    std::cerr << "Test 1 FAILED: tdb_delete should be called exactly once when *pRc==0\n";
    ok = false;
  }
  if (db.deleteCalled != 1) {
    std::cerr << "Test 1 FAILED: db.deleteCalled should be incremented exactly once\n";
    ok = false;
  }
  if (rc != 0) {
    std::cerr << "Test 1 FAILED: *pRc should be updated to rc (0) when tdb_delete returns 0\n";
    ok = false;
  }
  if (!ok) std::cerr << "Test 1: FAILED\n";
  return ok;
}

// Test 2: When *pRc != 0, testDelete should NOT call tdb_delete
static bool test_delete_not_called_when_rc_nonzero()
{
  g_delete_call_count = 0;
  TestDb db;
  db.returnCodeForDelete = 0; // would be used if called
  db.deleteCalled = 0;
  int rc = 42; // non-zero initial RC
  int keyVal = 0xABCD;
  call_testDelete(&db, keyVal, &rc);

  bool ok = true;
  if (g_delete_call_count != 0) {
    std::cerr << "Test 2 FAILED: tdb_delete should not be called when *pRc!=0\n";
    ok = false;
  }
  if (db.deleteCalled != 0) {
    std::cerr << "Test 2 FAILED: db.deleteCalled should remain 0 when not called\n";
    ok = false;
  }
  if (rc != 42) {
    std::cerr << "Test 2 FAILED: *pRc should remain unchanged when not calling tdb_delete\n";
    ok = false;
  }
  if (!ok) std::cerr << "Test 2: FAILED\n";
  return ok;
}

int main() {
  // Basic, explicit tests for the focal function's control-flow branches.
  std::cout << "Starting unit tests for testDelete (focal method).\n";

  bool pass1 = test_delete_calls_when_rc_zero();
  bool pass2 = test_delete_not_called_when_rc_nonzero();

  if (pass1 && pass2) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cerr << "Some tests FAILED.\n";
    return 1;
  }
}