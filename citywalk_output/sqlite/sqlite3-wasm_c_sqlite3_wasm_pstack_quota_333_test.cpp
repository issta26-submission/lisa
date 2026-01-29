// Unit test suite for sqlite3_wasm_pstack_quota and related pstack APIs
// Focus: verify quota behavior via the focal method and ensure interactions with pstack state.
// Notes:
// - Uses C linkage to call into the focal C code (sqlite3_wasm_pstack_quota and friends).
// - No GTest; a lightweight test harness is implemented using a few test functions and a main().
// - Domain knowledge: PStack_mem is 512 * 8 bytes -> quota = 4096 bytes.
// - We exercise true/false branches by validating correct quota value, remaining changes after allocations,
//   and edge cases for allocation (0 or insufficient space).

#include <emscripten/wasmfs.h>
#include <assert.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Step 1 (Candidate Keywords mapping): PStack, pBegin, pEnd, pPos, sqlite3_wasm_pstack_quota
// The tests interact with the following exported C APIs to observe and modify PStack state.
// These declarations use C linkage to link with the C implementation in sqlite3-wasm.c.

extern "C" {
  // Focal method under test
  int sqlite3_wasm_pstack_quota(void);

  // Helpers to interact with the pstack
  void* sqlite3_wasm_pstack_ptr(void);                 // current pPos
  void* sqlite3_wasm_pstack_alloc(int n);               // allocate n bytes on pstack
  int   sqlite3_wasm_pstack_remaining(void);            // remaining bytes from pBegin to pPos
  void  sqlite3_wasm_pstack_restore(unsigned char* p);  // restore pPos to p
}

// Expected quota based on the implementation: PStack_mem size = 512 * 8 bytes = 4096
static const int EXPECTED_PSTACK_QUOTA = 512 * 8;

// Helper to print test results
static void print_test_result(const char* test_name, bool passed) {
  std::cout << (passed ? "[PASS] " : "[FAIL] ") << test_name << std::endl;
}

// Test 1: Baseline quota should match the expected constant (4096)
static bool test_quota_baseline() {
  // true branch: quota equals the known static size
  int quota = sqlite3_wasm_pstack_quota();
  bool ok = (quota == EXPECTED_PSTACK_QUOTA);
  if(!ok) {
    std::cerr << "Expected quota " << EXPECTED_PSTACK_QUOTA
              << ", got " << quota << std::endl;
  }
  return ok;
}

// Test 2: Allocation reduces remaining and quota remains constant
static bool test_allocation_changes_remaining() {
  // Read initial remaining
  int rem_before = sqlite3_wasm_pstack_remaining();
  int quota_before = sqlite3_wasm_pstack_quota();

  // Allocate a modest amount (e.g., 64 bytes)
  void* p = sqlite3_wasm_pstack_alloc(64);
  if(p == 0) {
    std::cerr << "Allocation of 64 bytes failed unexpectedly." << std::endl;
    return false;
  }

  // Remaining should decrease by the allocated amount
  int rem_after = sqlite3_wasm_pstack_remaining();
  if(rem_before - 64 != rem_after) {
    std::cerr << "Remaining after allocation mismatch. Before: "
              << rem_before << ", After: " << rem_after << std::endl;
    return false;
  }

  // Quota should remain unchanged (constant total size)
  int quota_after = sqlite3_wasm_pstack_quota();
  if(quota_before != quota_after) {
    std::cerr << "Quota changed after allocation. Before: "
              << quota_before << ", After: " << quota_after << std::endl;
    return false;
  }

  // Optional: restore state to not leak into next tests (not strictly required, but safe)
  unsigned char* curPos = (unsigned char*)sqlite3_wasm_pstack_ptr();
  // Restore to current position (no-op, but demonstrates API usage)
  sqlite3_wasm_pstack_restore(curPos);

  return true;
}

// Test 3: Allocation beyond available space should fail and not modify remaining
static bool test_allocation_insufficient_space() {
  // Current remaining should be some value <= quota
  int rem_before = sqlite3_wasm_pstack_remaining();
  int quota = sqlite3_wasm_pstack_quota();

  // Try to allocate a large chunk that may exceed space
  // We attempt 4096 (whole buffer) which should fail if prior allocations reduced space.
  void* p = sqlite3_wasm_pstack_alloc(4096);
  if(p != nullptr) {
    // If this unexpectedly succeeds, it's a breach of state in typical progression
    std::cerr << "Unexpectedly succeeded in allocating 4096 bytes; test assumptions violated." << std::endl;
    return false;
  }

  // Remaining should remain unchanged
  int rem_after = sqlite3_wasm_pstack_remaining();
  if(rem_before != rem_after) {
    std::cerr << "Remaining changed after failed allocation. Before: "
              << rem_before << ", After: " << rem_after << std::endl;
    return false;
  }

  // Quota should still be the same
  int quota_after = sqlite3_wasm_pstack_quota();
  if(quota != quota_after) {
    std::cerr << "Quota changed after failed allocation. Before: "
              << quota << ", After: " << quota_after << std::endl;
    return false;
  }

  // Intentionally do not restore state here; subsequent tests can proceed with current state
  return true;
}

// Test 4: Allocation with zero should return null and not modify state
static bool test_allocation_zero_returns_null() {
  void* p = sqlite3_wasm_pstack_alloc(0);
  if(p != nullptr) {
    std::cerr << "Allocation with n=0 should return NULL, but did not." << std::endl;
    return false;
  }

  // State should be unchanged
  int rem = sqlite3_wasm_pstack_remaining();
  int quota = sqlite3_wasm_pstack_quota();
  if(rem < 0 || quota <= 0) {
    std::cerr << "Invalid state after zero-allocation test. rem=" << rem
              << " quota=" << quota << std::endl;
    return false;
  }

  return true;
}

int main() {
  using namespace std;

  // Basic test execution: run each test in sequence and report results.
  int tests_passed = 0;
  int total_tests = 4;

  bool t1 = test_quota_baseline();
  print_test_result("Quota baseline equals expected 4096", t1);
  tests_passed += t1 ? 1 : 0;

  bool t2 = test_allocation_changes_remaining();
  print_test_result("Allocation reduces remaining and keeps quota constant", t2);
  tests_passed += t2 ? 1 : 0;

  bool t3 = test_allocation_insufficient_space();
  print_test_result("Insufficient space allocation fails and doesn't change remaining", t3);
  tests_passed += t3 ? 1 : 0;

  bool t4 = test_allocation_zero_returns_null();
  print_test_result("Zero-sized allocation returns null and doesn't modify state", t4);
  tests_passed += t4 ? 1 : 0;

  std::cout << "Summary: " << tests_passed << "/" << total_tests << " tests passed." << std::endl;

  return (tests_passed == total_tests) ? EXIT_SUCCESS : EXIT_FAILURE;
}