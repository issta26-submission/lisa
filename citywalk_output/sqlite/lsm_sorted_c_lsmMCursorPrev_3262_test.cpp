// Test suite for the focal method lsmMCursorPrev
// This file provides a lightweight, framework-agnostic unit test
// without using Google Test. It relies on the project-provided
// headers to supply the real types and constants (MultiCursor,
// CURSOR_PREV_OK, LSM_MISUSE_BKPT, etc.).
// The tests cover true/false branches of the condition predicate.

#include <iostream>
#include <lsm_sorted.h>


// Include project headers that declare the necessary types, constants,
// and the function under test. This allows the test to use the real
// definitions (e.g., MultiCursor, CURSOR_PREV_OK, LSM_MISUSE_BKPT).

// Simple test harness helpers
static bool test_passed = true;
static void report(const char* name, bool ok) {
  if (ok) {
    std::cout << "[PASS] " << name << std::endl;
  } else {
    std::cerr << "[FAIL] " << name << std::endl;
    test_passed = false;
  }
}

// Test 1: Misuse branch when CURSOR_PREV_OK is not set in pCsr->flags
// Expected behavior: lsmMCursorPrev should return LSM_MISUSE_BKPT.
static bool test_lsmMCursorPrev_misuse_branch() {
  // Prepare a MultiCursor instance with no CURSOR_PREV_OK bit set.
  // We rely on the real constants/macros provided by the project headers.
  MultiCursor csr;
  csr.flags = 0; // Force the predicate (pCsr->flags & CURSOR_PREV_OK) == 0

  int rc = lsmMCursorPrev(&csr);

  // Expect the exact misuse return value defined by the codebase.
  if (rc != LSM_MISUSE_BKPT) {
    std::cerr << "lsmMCursorPrev mis-use branch returned " << rc
              << " (expected LSM_MISUSE_BKPT)"
              << std::endl;
    report("test_lsmMCursorPrev_misuse_branch", false);
    return false;
  }

  report("test_lsmMCursorPrev_misuse_branch", true);
  return true;
}

// Test 2: Prev-OK branch when CURSOR_PREV_OK is set in pCsr->flags
// Expected behavior: lsmMCursorPrev should delegate to multiCursorAdvance(pCsr, 1)
// and return whatever that call returns. We cannot directly observe the internal
// call, but we can assert that the function does not take the mis-use path.
static bool test_lsmMCursorPrev_prevOk_branch() {
  MultiCursor csr;
  csr.flags = CURSOR_PREV_OK; // Set the bit so the predicate passes

  int rc = lsmMCursorPrev(&csr);

  // The exact value depends on multiCursorAdvance's behavior, which is
  // implementation-specific. We only verify that misuse is not taken.
  if (rc == LSM_MISUSE_BKPT) {
    std::cerr << "lsmMCursorPrev prevOk branch unexpectedly returned mis-use value."
              << std::endl;
    report("test_lsmMCursorPrev_prevOk_branch", false);
    return false;
  }

  report("test_lsmMCursorPrev_prevOk_branch", true);
  return true;
}

int main() {
  // Run tests
  test_passed = true;

  bool ok1 = test_lsmMCursorPrev_misuse_branch();
  bool ok2 = test_lsmMCursorPrev_prevOk_branch();

  // Aggregate results
  if (ok1 && ok2) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cerr << "SOME TESTS FAILED" << std::endl;
    return 1;
  }
}