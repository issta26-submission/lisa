/*
  Unit test suite for the focal method:
  Bitmask sqlite3WhereGetMask(WhereMaskSet *pMaskSet, int iCursor)

  This standalone test file provides a minimal, self-contained environment
  to exercise the function without requiring the full SQLite codebase or
  external testing frameworks (GTest, etc.).

  Notes:
  - We define minimal substitutes for the dependent types used by the focal
    method (Bitmask, MASKBIT, WhereMaskSet) to enable compilation.
  - Tests are designed to cover:
      * First-element match (ix[0] == iCursor) -> returns 1
      * Second-or-later element match (ix[i] == iCursor) -> returns MASKBIT(i)
      * No match -> returns 0
  - All tests use non-terminating, custom expectations to maximize code coverage
    without aborting on failures.
  - Static/internal helpers used by the real project are not relied upon; this
    file is self-contained.

  Candidate Keywords (for test design): Bitmask, MASKBIT, WhereMaskSet, n, ix[],
  iCursor, early-return, loop-based search, boundary condition, assertions,
  non-terminating test assertions.
*/

#include <whereInt.h>
#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Minimal stand-in types and macros to compile the focal method in isolation
using Bitmask = unsigned int;
#define MASKBIT(i) (((Bitmask)1) << (i))

// Minimal dependency structure similar to the real code's usage
struct WhereMaskSet {
  int n;        // number of active entries
  int ix[8];    // indices of masks; ix[0] is checked specially
};

// The focal method under test (copied/adapted for standalone compilation)
Bitmask sqlite3WhereGetMask(WhereMaskSet *pMaskSet, int iCursor){
  int i;
  // Bounds and invariants mimicked as in the original code
  assert( pMaskSet->n <= (int)sizeof(Bitmask)*8 );
  assert( pMaskSet->n > 0 || pMaskSet->ix[0] < 0 );
  assert( iCursor >= -1 );
  if( pMaskSet->ix[0] == iCursor ){
    return 1;
  }
  for(i = 1; i < pMaskSet->n; i++){
    if( pMaskSet->ix[i] == iCursor ){
      return MASKBIT(i);
    }
  }
  return 0;
}

// Lightweight, non-terminating test harness (GTest-like semantics without frameworks)
static int g_failures = 0;

static void test_pass(const char* testName){
  printf("PASS: %s\n", testName);
}

static void test_fail(const char* testName, const char* detail){
  printf("FAIL: %s - %s\n", testName, detail);
  ++g_failures;
}

// Assertion-like helpers that do not abort on failure
static void expect_equal(Bitmask actual, Bitmask expected, const char* testName, const char* detail){
  if(actual != expected){
    char buf[128];
    std::snprintf(buf, sizeof(buf), "expected %u, got %u", (unsigned)expected, (unsigned)actual);
    test_fail(testName, detail ? detail : buf);
  } else {
    test_pass(testName);
  }
}

// Test Suite

// Test 1: First element matches iCursor (ix[0] == iCursor) -> return 1
// This exercises the initial short-circuit path.
static void test_sqlite3WhereGetMask_firstElementMatch(){
  WhereMaskSet s;
  s.n = 3;
  s.ix[0] = 5;  // iCursor we will search for
  s.ix[1] = 9;
  s.ix[2] = 3;

  Bitmask res = sqlite3WhereGetMask(&s, 5);

  expect_equal(res, 1, "test_sqlite3WhereGetMask_firstElementMatch",
               "ix[0] equals iCursor should return 1");
}

// Test 2: Second element matches iCursor (ix[1] == iCursor) -> return MASKBIT(1)
// This exercises the loop path (i from 1 up to n-1).
static void test_sqlite3WhereGetMask_secondElementMatch(){
  WhereMaskSet s;
  s.n = 3;
  s.ix[0] = 0;
  s.ix[1] = 7;   // iCursor at index 1
  s.ix[2] = 2;

  Bitmask res = sqlite3WhereGetMask(&s, 7);

  expect_equal(res, MASKBIT(1), "test_sqlite3WhereGetMask_secondElementMatch",
               "ix[1] equals iCursor should return MASKBIT(1)");
}

// Test 3: No element matches iCursor -> return 0
// This covers the default return path after the loop.
static void test_sqlite3WhereGetMask_noMatchReturnsZero(){
  WhereMaskSet s;
  s.n = 2;
  s.ix[0] = 0;
  s.ix[1] = 2;

  Bitmask res = sqlite3WhereGetMask(&s, 1);

  expect_equal(res, 0, "test_sqlite3WhereGetMask_noMatchReturnsZero",
               "no ix[i] equals iCursor should return 0");
}

// Test 4: Single-element case where ix[0] == iCursor (n == 1)
// Ensures the boundary condition where the search space has only one entry.
static void test_sqlite3WhereGetMask_singleElementFirstMatch(){
  WhereMaskSet s;
  s.n = 1;
  s.ix[0] = -1;  // iCursor to match
  Bitmask res = sqlite3WhereGetMask(&s, -1);

  expect_equal(res, 1, "test_sqlite3WhereGetMask_singleElementFirstMatch",
               "n==1 and ix[0]==iCursor should return 1");
}

// Main entry to run tests
int main(){
  // Run tests in a deterministic order
  test_sqlite3WhereGetMask_firstElementMatch();
  test_sqlite3WhereGetMask_secondElementMatch();
  test_sqlite3WhereGetMask_noMatchReturnsZero();
  test_sqlite3WhereGetMask_singleElementFirstMatch();

  if(g_failures){
    printf("Total failures: %d\n", g_failures);
    return 1;
  } else {
    printf("All tests passed\n");
    return 0;
  }
}