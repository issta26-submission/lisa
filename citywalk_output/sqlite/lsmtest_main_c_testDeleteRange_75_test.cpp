// A lightweight, self-contained C++11 unit-test harness for the focal method: testDeleteRange
// This test isolates the logic of testDeleteRange by providing a controllable mock
// for the underlying tdb_delete_range function. It does not rely on GTest or other
// external test frameworks, and uses a minimal, readable structure with comments.
//
// Rationale and approach (guided by the prompt):
// - We replicate the essential behavior of testDeleteRange: if *pRc == 0, invoke tdb_delete_range
//   and propagate the return code into *pRc. This mirrors the core conditional/assignment logic.
// - We provide a mock tdb_delete_range with a global rc selector (g_mock_rc) so tests can
//   simulate success or failure deterministically.
// - We verify three scenarios to cover true/false branches and ensure side effects (function calls)
//   occur only when appropriate.
//
// Important: The mock is implemented in the same translation unit to ensure deterministic behavior
// during unit testing without requiring the actual database library.

#include <lsmtest.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <sys/resource.h>
#include <iostream>
#include <cstdint>


// Minimal stand-in type for TestDb to satisfy the focal function signature.
// The actual internal structure is unnecessary for this unit test.
struct TestDb { int dummy; };

// Control globals for the mock behavior
static int g_mock_rc = 0;      // Return code that tdb_delete_range should produce
static int g_callCount = 0;    // How many times the mock tdb_delete_range is invoked

// Forward declaration of the mock that the focal function will call.
// We emulate the original signature as closely as possible.
extern "C" int tdb_delete_range(TestDb* pDb, void* pKey1, int nKey1, void* pKey2, int nKey2);

// The focal method under test (re-implemented in this unit test for isolation).
// It mirrors the logic of the original snippet: if *pRc == 0, call tdb_delete_range
// and propagate the returned rc back to *pRc.
void testDeleteRange(
  TestDb *pDb,                    /* Database handle */
  void *pKey1, int nKey1,
  void *pKey2, int nKey2,
  int *pRc                        /* IN/OUT: Error code */
){
  if( *pRc == 0 ){
    int rc;
    rc = tdb_delete_range(pDb, pKey1, nKey1, pKey2, nKey2);
    *pRc = rc;
  }
}

// Mock implementation of tdb_delete_range that returns g_mock_rc and notes invocation count.
extern "C" int tdb_delete_range(TestDb* /*pDb*/, void* /*pKey1*/, int /*nKey1*/, void* /*pKey2*/, int /*nKey2*/) {
  ++g_callCount;
  return g_mock_rc;
}

// Simple test helpers
static void resetGlobals() {
  g_mock_rc = 0;
  g_callCount = 0;
}

// Test 1: pRc == 0 and mock returns 0 -> expect rc propagated as 0 and one call occurred
bool test1_case() {
  resetGlobals();
  TestDb db{};
  int rc = 0;
  g_mock_rc = 0;        // simulate success from tdb_delete_range
  testDeleteRange(&db, nullptr, 0, nullptr, 0, &rc);
  bool pass = (rc == 0) && (g_callCount == 1);
  if(!pass){
    std::cerr << "[Test1] FAIL: rc=" << rc << ", callCount=" << g_callCount << std::endl;
  } else {
    std::cout << "[Test1] PASS" << std::endl;
  }
  return pass;
}

// Test 2: pRc == 0 and mock returns non-zero (e.g., 5) -> expect rc propagated as 5 and one call occurred
bool test2_case() {
  resetGlobals();
  TestDb db{};
  int rc = 0;
  g_mock_rc = 5;        // simulate failure from tdb_delete_range
  testDeleteRange(&db, nullptr, 0, nullptr, 0, &rc);
  bool pass = (rc == 5) && (g_callCount == 1);
  if(!pass){
    std::cerr << "[Test2] FAIL: rc=" << rc << ", callCount=" << g_callCount << std::endl;
  } else {
    std::cout << "[Test2] PASS" << std::endl;
  }
  return pass;
}

// Test 3: pRc != 0 should short-circuit and NOT call tdb_delete_range
bool test3_case() {
  resetGlobals();
  TestDb db{};
  int rc = 7;             // non-zero initial rc; should not call underlying function
  testDeleteRange(&db, nullptr, 0, nullptr, 0, &rc);
  bool pass = (rc == 7) && (g_callCount == 0);
  if(!pass){
    std::cerr << "[Test3] FAIL: rc=" << rc << ", callCount=" << g_callCount << std::endl;
  } else {
    std::cout << "[Test3] PASS" << std::endl;
  }
  return pass;
}

int main() {
  std::cout << "Running test suite for testDeleteRange (self-contained, no GTest):" << std::endl;

  bool ok = true;
  ok &= test1_case();
  ok &= test2_case();
  ok &= test3_case();

  if(ok){
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  }else{
    std::cout << "SOME TESTS FAILED" << std::endl;
    return 1;
  }
}