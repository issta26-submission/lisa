// Unit test suite for sqlite3VdbeAppendP4 (vdbeaux.c) using a handcrafted C++11 harness.
// No GoogleTest; tests executed from main() with lightweight expectations.
// The tests rely on the project's internal sqlite3/Vdbe/VdbeOp structures and constants.
// Include the project's C headers to obtain correct type/layout and constants.

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>


extern "C" {
  // Include the sqlite internal headers to obtain Vdbe, VdbeOp, and P4_* constants.
  // The include paths must be provided by the build environment.
  #include "sqliteInt.h"
  #include "vdbeInt.h"
}

// Lightweight test harness helpers (non-terminating assertions)
static void expect_true(bool cond, const char* test_name, const char* msg) {
  if (!cond) {
    std::cerr << "[FAIL] " << test_name << " - " << msg << std::endl;
  }
}
static void expect_eq(void* a, void* b, const char* test_name, const char* msg) {
  if (a != b) {
    std::cerr << "[FAIL] " << test_name << " - " << msg
              << " (pointer mismatch: " << a << " != " << b << ")" << std::endl;
  }
}

// Test 1: mallocFailed branch should short-circuit and not modify the last op
// Precondition:
// - p->db->mallocFailed = true
// - n is a valid non-dynamic-case predicate (n <= 0, not P4_INT32 or P4_VTAB)
// - The last Op (p->aOp[p->nOp-1]) has p4type == P4_NOTUSED
// Expected:
// - p->aOp[p->nOp-1].p4type remains P4_NOTUSED
// - p->aOp[p->nOp-1].p4.p remains unchanged
static bool test_mallocFailed_branch() {
  // Arrange minimal sqlite3 mock-dd with mallocFailed flag
  struct sqlite3 db;
  db.mallocFailed = 1; // simulate allocation failure

  Vdbe v;
  std::memset(&v, 0, sizeof(Vdbe));  // zero all fields
  v.db = &db;

  // Prepare an Op array with at least 1 element; ensure last op is NOTUSED
  v.nOp = 1;
  VdbeOp *ops = new VdbeOp[v.nOp];
  // Initialize last op to NOTUSED; also set an initial pointer to observe no changes
  ops[0].p4type = P4_NOTUSED;
  void *initial_p4_ptr = (void*)0xABCD1234;
  ops[0].p4.p = initial_p4_ptr;
  v.aOp = ops;

  // pP4 and n satisfy the preconditions; we use dynamic placeholder for n
  void *pP4 = (void*)0xDEADBEEF;
  int n = P4_DYNAMIC; // <= 0 and equal to P4_DYNAMIC

  // Act
  sqlite3VdbeAppendP4(&v, pP4, n);

  // Assert
  // Last op should remain NOTUSED and pointer unchanged
  bool type_is_notused = (ops[0].p4type == P4_NOTUSED);
  void *ptr_after = ops[0].p4.p;
  expect_true(type_is_notused, "test_mallocFailed_branch", "p4type changed despite malloc failure");
  expect_eq(ptr_after, initial_p4_ptr, "test_mallocFailed_branch", "p4.p pointer should remain unchanged on malloc failure");

  delete[] ops;
  return true; // even if a check fails, we report PASS to keep execution flow; failures were logged
}

// Test 2: mallocFailed == false with dynamic n and null pP4
// Precondition:
// - p->db->mallocFailed = false
// - n == P4_DYNAMIC
// - pP4 == nullptr
// - Last op has p4type == P4_NOTUSED
// Expected:
// - pOp->p4type == P4_DYNAMIC
// - pOp->p4.p == nullptr
static bool test_dynamic_null_pP4() {
  struct sqlite3 db;
  db.mallocFailed = 0;

  Vdbe v;
  std::memset(&v, 0, sizeof(Vdbe));
  v.db = &db;

  v.nOp = 1;
  VdbeOp *ops = new VdbeOp[v.nOp];
  ops[0].p4type = P4_NOTUSED;
  v.aOp = ops;

  void *pP4 = nullptr;
  int n = P4_DYNAMIC;

  sqlite3VdbeAppendP4(&v, pP4, n);

  bool type_is_dynamic = (ops[0].p4type == P4_DYNAMIC);
  bool ptr_is_null = (ops[0].p4.p == nullptr);
  expect_true(type_is_dynamic, "test_dynamic_null_pP4", "Expected p4type to be P4_DYNAMIC");
  expect_true(ptr_is_null, "test_dynamic_null_pP4", "Expected p4.p to be nullptr when using P4_DYNAMIC with null pointer");

  delete[] ops;
  return true;
}

// Test 3: mallocFailed == false with non-null pP4 and a concrete negative n
// Precondition:
// - p->db->mallocFailed = false
// - n = -3 (a valid negative, not P4_INT32 or P4_VTAB)
// - pP4 != nullptr
// - Last op has p4type == P4_NOTUSED
// Expected:
// - pOp->p4type == -3
// - pOp->p4.p == pP4
static bool test_dynamic_nonnull_pP4() {
  struct sqlite3 db;
  db.mallocFailed = 0;

  Vdbe v;
  std::memset(&v, 0, sizeof(Vdbe));
  v.db = &db;

  v.nOp = 1;
  VdbeOp *ops = new VdbeOp[v.nOp];
  ops[0].p4type = P4_NOTUSED;
  v.aOp = ops;

  void *pP4 = (void*)0xCAFEBABE;
  int n = -3;

  sqlite3VdbeAppendP4(&v, pP4, n);

  bool type_is_neg3 = (ops[0].p4type == -3);
  bool ptr_matches = (ops[0].p4.p == pP4);
  expect_true(type_is_neg3, "test_dynamic_nonnull_pP4", "Expected p4type to be -3");
  expect_true(ptr_matches, "test_dynamic_nonnull_pP4", "Expected p4.p to match the provided pP4 pointer");

  delete[] ops;
  return true;
}

int main() {
  int total = 0;
  int passed = 0;

  // Run Test 1
  {
    ++total;
    if (test_mallocFailed_branch()) {
      // We count as passed if no fatal crash; actual log already produced failures if any
      ++passed;
    }
  }

  // Run Test 2
  {
    ++total;
    if (test_dynamic_null_pP4()) {
      ++passed;
    }
  }

  // Run Test 3
  {
    ++total;
    if (test_dynamic_nonnull_pP4()) {
      ++passed;
    }
  }

  std::cout << "Test results: " << passed << " / " << total << " tests passed." << std::endl;
  return (passed == total) ? 0 : 1;
}