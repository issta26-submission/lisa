// Lightweight C++11 test harness for the focal C function:
// void sqlite3VdbeChangeP1(Vdbe *p, int addr, int val)
// This test does not depend on GTest and uses a minimal, self-contained
// mock environment to exercise the function's logic.
//
// Explanations:
// - Test 1 (Valid address): Verifies that with a valid non-negative address,
//   the function updates the targeted VdbeOp.p1 field correctly and that
//   the assertion predicate evaluates to true without triggering the test
//   assertion flag.
// - Test 2 (Invalid address): Uses a test-friendly assert replacement to
//   capture that addr >= 0 would fail, and also exercises behavior when
//   an invalid address is provided by exercising a clamp in sqlite3VdbeGetOp.
//   The test asserts that the fake assertion was triggered and that some
//   write to an Op occurred (to verify code execution path).

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <string>


// Global flag to capture whether the assertion predicate would have failed.
// This is used to emulate non-terminating assertions for coverage.
static bool g_assert_triggered = false;

// Minimal replacement for the standard C assert to avoid terminating tests.
// If the condition is false, we mark the assertion as triggered but continue.
#define assert(cond) do { if(!(cond)) { g_assert_triggered = true; } } while(0)

// Minimal dependency surface to mimic the needed parts of the real API.

struct VdbeOp {
  int p1;
};

struct Vdbe {
  VdbeOp* aOp; // array of operations
  int nOp;     // number of operations
};

// Minimal implementation of the function under test's dependency:
//
// In the real project, sqlite3VdbeGetOp(p, addr) would fetch the Op at a given
// address from the Vdbe's op-array. For unit testing, we provide a safe stub
// that clamps invalid addresses to valid indices to avoid crashes in tests.
VdbeOp* sqlite3VdbeGetOp(Vdbe *p, int addr) {
  if (p == nullptr || p->aOp == nullptr) return nullptr;
  if (addr < 0) {
    // Clamp to first operation for safety in test scenario.
    return &p->aOp[0];
  }
  if (addr >= p->nOp) {
    // Clamp to last operation for safety in test scenario.
    return &p->aOp[p->nOp - 1];
  }
  return &p->aOp[addr];
}

// Focal method under test (redeclared for the standalone test harness)
void sqlite3VdbeChangeP1(Vdbe *p, int addr, int val){
  assert( addr>=0 );
  sqlite3VdbeGetOp(p,addr)->p1 = val;
}

// Test 1: Valid address branch
// Purpose: Ensure that for a non-negative address, the p1 field of the
// corresponding VdbeOp is updated to the provided value.
bool testChangeP1_ValidAddr() {
  // Arrange
  Vdbe vb;
  vb.nOp = 3;
  VdbeOp ops[3] = { {0}, {0}, {0} };
  vb.aOp = ops;
  g_assert_triggered = false;

  // Act
  sqlite3VdbeChangeP1(&vb, 1, 123);

  // Assert
  bool passed = true;
  if (vb.aOp[1].p1 != 123) {
    std::cerr << "Test 1 Failed: Expected op[1].p1 to be 123, got "
              << vb.aOp[1].p1 << std::endl;
    passed = false;
  }
  if (g_assert_triggered) {
    std::cerr << "Test 1 Failed: Assertion should not have been triggered for valid addr." << std::endl;
    passed = false;
  }
  return passed;
}

// Test 2: Negative address branch (assert predicate false path)
// Note: We use the test-friendly assert replacement to capture that the
// assertion would have failed. Since our sqlite3VdbeGetOp clamps invalid
// addresses, we also observe a write to an Op to confirm path execution.
bool testChangeP1_InvalidAddr() {
  // Arrange
  Vdbe vb;
  vb.nOp = 3;
  VdbeOp ops[3] = { { 10 }, { 20 }, { 30 } };
  vb.aOp = ops;
  g_assert_triggered = false;

  // Act
  sqlite3VdbeChangeP1(&vb, -1, 999);

  // Assert
  bool passed = true;
  if (!g_assert_triggered) {
    std::cerr << "Test 2 Failed: Expected assertion to be triggered for invalid addr." << std::endl;
    passed = false;
  }
  // Since sqlite3VdbeGetOp clamps addr<0 to index 0 in this test harness,
  // op[0].p1 should have been updated to 999.
  if (vb.aOp[0].p1 != 999) {
    std::cerr << "Test 2 Failed: Expected op[0].p1 to be 999 due to clamp, got "
              << vb.aOp[0].p1 << std::endl;
    passed = false;
  }
  return passed;
}

int main() {
  int total = 0;
  int passed = 0;

  std::cout << "Running unit tests for sqlite3VdbeChangeP1 (standalone harness)..." << std::endl;

  total++;
  if (testChangeP1_ValidAddr()) {
    passed++;
    std::cout << "[PASS] testChangeP1_ValidAddr" << std::endl;
  } else {
    std::cout << "[FAIL] testChangeP1_ValidAddr" << std::endl;
  }

  total++;
  if (testChangeP1_InvalidAddr()) {
    passed++;
    std::cout << "[PASS] testChangeP1_InvalidAddr" << std::endl;
  } else {
    std::cout << "[FAIL] testChangeP1_InvalidAddr" << std::endl;
  }

  std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

  return (passed == total) ? 0 : 1;
}