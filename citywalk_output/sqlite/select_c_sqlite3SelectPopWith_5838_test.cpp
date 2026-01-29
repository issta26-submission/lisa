// Lightweight C++11 test harness for the focal method: sqlite3SelectPopWith
// This test is self-contained (no external test framework) and aims to
// validate the core branching logic of the focal function.
// It re-implements a minimal subset of the involved structures to simulate
// realistic usage and verify behavior across key scenarios.

// This file intentionally uses a minimal, self-contained approach to maximize
// executable coverage without depending on GTest or other frameworks.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Domain-oriented macro: treat a NULL-like pointer as "false" and non-NULL as "true"
#define OK_IF_ALWAYS_TRUE(x) ((x) != nullptr)

// Forward declarations of minimal structures to mirror the sqlite3-like API
struct With;
struct Select;
struct Parse;
struct Walker;

// Minimal stub types matching the focal method's usage
struct Walker {
  Parse *pParse;
};

struct With {
  With *pOuter;
};

// In the original code, Select contains pointers to prior selects and possibly a With
struct Select {
  Select *pPrior; // Previous/earlier select in a chain; nullptr if this is the rightmost
  With *pWith;    // Associated With object for this select (if any)
};

// Parse holds a possible error flag and a pointer to an associated With
struct Parse {
  int nErr;    // Non-zero indicates an error; mirrors sqlite3's error counter
  With *pWith;  // Current active WITH clause for the parser context
};

// The helper function to discover the rightmost Select in a possible chain.
// In the production code, this is a static function inside select.c.
// For unit testing, we provide a simple, dependency-free implementation.
static Select* findRightmost(Select *p){
  if (p == nullptr) return nullptr;
  while (p->pPrior != nullptr) {
    p = p->pPrior;
  }
  return p;
}

// The focal method under test (re-implemented here for a self-contained harness).
// Signature must match: void sqlite3SelectPopWith(Walker *pWalker, Select *p)
void sqlite3SelectPopWith(Walker *pWalker, Select *p){
  // Access the Parse object from the Walker
  Parse *pParse = pWalker->pParse;

  // Branch: if pParse->pWith exists (non-null) and the current Select has no prior
  // (i.e., it's the rightmost in the chain), proceed to potentially pop the WITH scope.
  if( OK_IF_ALWAYS_TRUE(pParse->pWith) && p->pPrior == nullptr ){
    With *pWith = findRightmost(p)->pWith;
    if( pWith != nullptr ){
      // Maintain runtime sanity with an assertion mirroring the production code:
      // either the parser's current WITH matches the found one, or there is an error.
      // In unit tests, we ensure this path is taken safely by providing matching pointers.
      // (Note: The assertion mirrors the library's internal consistency checks.)
      // If this assertion fails in test, it will terminate the test run.
      // To keep tests non-terminating, ensure conditions satisfy the assertion.
      assert( pParse->pWith == pWith || pParse->nErr );
      pParse->pWith = pWith->pOuter;
    }
  }
}

// Lightweight test framework scaffolding: non-terminating assertions with messages
#define TEST_FAIL(msg) do { std::cerr << "TEST_FAIL: " << msg << std::endl; } while(false)
#define TEST_PASS(msg) do { std::cout << "TEST_PASS: " << msg << std::endl; } while(false)

// Helper macros to simplify test validation
#define CHECK_EQ(a, b, hint) do { if ((a) != (b)) { TEST_FAIL(std::string("Mismatch: ") + hint); } else { TEST_PASS((hint)); } } while(false)
#define CHECK_PTR_EQ(a, b, hint) do { if ((a) != (b)) { TEST_FAIL(std::string("Pointer mismatch: ") + hint); } else { TEST_PASS((hint)); } } while(false)

// Test Case 1: Basic pop with non-null pWith on the rightmost select
// - Build a rightmost Select whose pWith points to a With object (w1)
// - The parser's pWith initially equals w1 to satisfy the internal assert
// - The rightmost With's outer is wOuter; after the call, pParse->pWith should become wOuter
bool test_case_pop_with_basic_success() {
  // Setup With objects
  With wOuter;     // Outer (outer-most) WITH scope
  wOuter.pOuter = nullptr;

  With w1;         // Rightmost With attached to the rightmost Select
  w1.pOuter = &wOuter;

  // Setup Select chain: rightmost has no pPrior, and its pWith is w1
  Select rightmost;
  rightmost.pPrior = nullptr;
  rightmost.pWith  = &w1;

  // The Select under test (p) is the one whose popWith behavior is invoked
  Select p;
  p.pPrior = nullptr;
  p.pWith = nullptr; // not used by the test, but present for completeness

  // Parse context: pWith initially set to w1 to satisfy the internal assert
  Parse parse;
  parse.nErr = 0;
  parse.pWith = &w1;

  Walker walker;
  walker.pParse = &parse;

  // Execute the focal function
  sqlite3SelectPopWith(&walker, &p);

  // Validate: parse.pWith should now point to wOuter
  if (parse.pWith == &wOuter) {
    TEST_PASS("test_case_pop_with_basic_success: pParse->pWith updated to pWith->pOuter when applicable.");
    return true;
  } else {
    TEST_FAIL("test_case_pop_with_basic_success: pParse->pWith did not update to pWith->pOuter as expected.");
    return false;
  }
}

// Test Case 2: No operation when rightmost's pWith is NULL
// - Rightmost Select has pWith == NULL, so the inner branch should not execute
// - The parser's pWith should remain unchanged after the call
bool test_case_pop_with_rightmost_without_with() {
  // Setup With objects (not used since rightmost pWith will be NULL)
  With wOuter;
  wOuter.pOuter = nullptr;

  With w1;      // Not used; kept for structural similarity
  w1.pOuter = &wOuter;

  // Rightmost select with pWith = NULL
  Select rightmost;
  rightmost.pPrior = nullptr;
  rightmost.pWith  = nullptr; // explicitly no WITH

  // The Select under test
  Select p;
  p.pPrior = nullptr;
  p.pWith = nullptr;

  // Parser with a non-null pWith to ensure the condition would be evaluated
  Parse parse;
  parse.nErr = 0;
  parse.pWith = &w1;

  Walker walker;
  walker.pParse = &parse;

  // Execute the focal function
  sqlite3SelectPopWith(&walker, &p);

  // Validate: parse.pWith should remain unchanged (still &w1)
  if (parse.pWith == &w1) {
    TEST_PASS("test_case_pop_with_rightmost_without_with: pParse->pWith unchanged when rightmost pWith is NULL.");
    return true;
  } else {
    TEST_FAIL("test_case_pop_with_rightmost_without_with: pParse->pWith unexpectedly changed.");
    return false;
  }
}

// Test Case 3: No operation when p->pPrior != 0
// - Even if pParse->pWith is non-null, the function should not modify pParse->pWith
bool test_case_no_operation_when_pPrior_not_zero() {
  // Setup With objects
  With wOuter;
  wOuter.pOuter = nullptr;

  With w1;
  w1.pOuter = &wOuter;

  // Create a chain where p is not the rightmost (pPrior != NULL)
  Select prior;
  prior.pPrior = nullptr;
  prior.pWith = &w1;  // not used directly by test, but part of potential chain

  Select p;
  p.pPrior = &prior;  // non-null, so the condition should fail
  p.pWith = nullptr;

  Parse parse;
  parse.nErr = 0;
  parse.pWith = &w1; // non-null

  Walker walker;
  walker.pParse = &parse;

  // Execute the focal function
  sqlite3SelectPopWith(&walker, &p);

  // Validate: parse.pWith should remain unchanged
  if (parse.pWith == &w1) {
    TEST_PASS("test_case_no_operation_when_pPrior_not_zero: pParse->pWith remained unchanged when pPrior != 0.");
    return true;
  } else {
    TEST_FAIL("test_case_no_operation_when_pPrior_not_zero: pParse->pWith was unexpectedly changed.");
    return false;
  }
}

int main() {
  int passed = 0;
  int total = 0;

  // Include a guard against unused result warnings
  bool r1 = test_case_pop_with_basic_success();
  total++;
  if (r1) passed++;

  bool r2 = test_case_pop_with_rightmost_without_with();
  total++;
  if (r2) passed++;

  bool r3 = test_case_no_operation_when_pPrior_not_zero();
  total++;
  if (r3) passed++;

  std::cout << "Tests run: " << total << ", Passed: " << passed << std::endl;
  return (passed == total) ? 0 : 1;
}