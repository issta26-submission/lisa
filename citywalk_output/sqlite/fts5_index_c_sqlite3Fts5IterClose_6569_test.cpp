/*
  Unit test suite for the focal method:
  sqlite3Fts5IterClose(Fts5IndexIter *pIndexIter)

  Notes:
  - This file provides a minimal, self-contained C++11 test harness that
    reproduces the essential types and behavior needed by sqlite3Fts5IterClose.
  - The real project contains many static helpers; here we stub only what is
    required to exercise the focal method's control flow and side effects.
  - Tests are written as simple C++ functions and invoked from main() without
    any external testing framework (as GTest is not allowed per requirements).
  - Tests use non-terminating assertions style (print results rather than abort)
    and ensure static-like behavior via global flags.
*/

#include <cstddef>
#include <iostream>
#include <fts5Int.h>


// Minimal reproductions of the dependencies used by sqlite3Fts5IterClose

// Forward declaration of the index type (opaque in the real project)
struct Fts5Index {};

// Iterate structure used by the focal function
struct Fts5Iter {
  Fts5Index *pIndex;
};

// The focal function's parameter type (mirrors the real API)
struct Fts5IndexIter {
  Fts5Index *pIndex;
};

// Global test hooks to observe side effects of the focal function
static int g_fts5MultiIterFree_called = 0;
static Fts5Iter *g_lastIterPassed = nullptr;

static int g_sqlite3Fts5IndexCloseReader_called = 0;
static Fts5Index *g_lastIndexPassed = nullptr;

// Stubbed dependencies invoked by sqlite3Fts5IterClose
static void fts5MultiIterFree(Fts5Iter *pIter){
  g_fts5MultiIterFree_called++;
  g_lastIterPassed = pIter;
}

static void sqlite3Fts5IndexCloseReader(Fts5Index *p){
  g_sqlite3Fts5IndexCloseReader_called++;
  g_lastIndexPassed = p;
}

// The focal method under test (reproduced for unit testing)
void sqlite3Fts5IterClose(Fts5IndexIter *pIndexIter){
  if( pIndexIter ){
    Fts5Iter *pIter = (Fts5Iter*)pIndexIter;
    Fts5Index *pIndex = pIter->pIndex;
    fts5MultiIterFree(pIter);
    sqlite3Fts5IndexCloseReader(pIndex);
  }
}

// Helper utilities for tests
static void reset_globals(){
  g_fts5MultiIterFree_called = 0;
  g_lastIterPassed = nullptr;
  g_sqlite3Fts5IndexCloseReader_called = 0;
  g_lastIndexPassed = nullptr;
}

// Simple, non-terminating assertion helper
static bool assert_eq_int(int a, int b, const char* msg){
  if(a == b){
    return true;
  }else{
    std::cerr << "[ASSERT FAIL] " << msg << " | expected: " << b << ", got: " << a << "\n";
    return false;
  }
}

static bool assert_ptr_eq(const void* a, const void* b, const char* msg){
  if(a == b){
    return true;
  }else{
    std::cerr << "[ASSERT FAIL] " << msg << " | pointers differ\n";
    return false;
  }
}

// Test 1: When pIndexIter is null, no side effects should occur
static bool test_null_pointer_no_actions(){
  // Setup
  reset_globals();

  // Execute
  sqlite3Fts5IterClose(nullptr);

  // Verify
  bool ok = true;
  ok &= assert_eq_int(g_fts5MultiIterFree_called, 0, "fts5MultiIterFree should not be called");
  ok &= assert_eq_int(g_sqlite3Fts5IndexCloseReader_called, 0, "sqlite3Fts5IndexCloseReader should not be called");
  ok &= assert_eq_int(g_lastIterPassed == nullptr ? 1 : 0, 1, "last iter should remain null"); // simple check to ensure no stale state
  ok &= assert_eq_int(g_lastIndexPassed == nullptr ? 1 : 0, 1, "last index should remain null");
  if(ok) std::cout << "[OK] test_null_pointer_no_actions passed\n";
  return ok;
}

// Test 2: Non-null pIndexIter with a valid pIndex should invoke both side effects
static bool test_non_null_pointer_calls_both(){
  // Setup
  reset_globals();
  Fts5Index index;
  Fts5IndexIter iter;
  iter.pIndex = &index;

  // Execute
  sqlite3Fts5IterClose(&iter);

  // Verify
  bool ok = true;
  ok &= assert_eq_int(g_fts5MultiIterFree_called, 1, "fts5MultiIterFree should be called exactly once");
  ok &= assert_ptr_eq((void*)g_lastIterPassed, (void*)&iter, "fts5MultiIterFree should receive the original iter cast-pointer");
  ok &= assert_eq_int(g_sqlite3Fts5IndexCloseReader_called, 1, "sqlite3Fts5IndexCloseReader should be called exactly once");
  ok &= assert_ptr_eq((void*)g_lastIndexPassed, (void*)&index, "sqlite3Fts5IndexCloseReader should receive the original index");
  if(ok) std::cout << "[OK] test_non_null_pointer_calls_both passed\n";
  return ok;
}

// Test 3: Non-null pIndexIter with null pIndex should still call the iterator free and pass null to closeReader
static bool test_non_null_pointer_with_null_index(){
  // Setup
  reset_globals();
  Fts5IndexIter iter;
  iter.pIndex = nullptr; // simulate pIndexIter's inner index being null

  // Execute
  sqlite3Fts5IterClose(&iter);

  // Verify
  bool ok = true;
  ok &= assert_eq_int(g_fts5MultiIterFree_called, 1, "fts5MultiIterFree should be called once even if pIndex is null");
  ok &= assert_ptr_eq((void*)g_lastIterPassed, (void*)&iter, "fts5MultiIterFree should receive the original iter cast-pointer");
  ok &= assert_eq_int(g_sqlite3Fts5IndexCloseReader_called, 1, "sqlite3Fts5IndexCloseReader should be called even if index is null");
  ok &= (g_lastIndexPassed == nullptr);
  if(ok) std::cout << "[OK] test_non_null_pointer_with_null_index passed\n";
  return ok;
}

// Entry point to run tests
int main(){
  bool all_ok = true;

  std::cout << "Running test suite for sqlite3Fts5IterClose (C++11, no GTest).\n";

  all_ok &= test_null_pointer_no_actions();
  all_ok &= test_non_null_pointer_calls_both();
  all_ok &= test_non_null_pointer_with_null_index();

  if(all_ok){
    std::cout << "All tests passed.\n";
    return 0;
  }else{
    std::cout << "Some tests failed.\n";
    return 1;
  }
}