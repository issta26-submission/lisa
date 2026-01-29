// Lightweight C++11 unit tests for sqlite3_wasm_pstack_alloc
// Follows instructions to avoid GTest and use domain-appropriate checks.
// Tests run in a single process and rely on the public exported stack API.

#include <emscripten/wasmfs.h>
#include <assert.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Step 1: Import necessary C API from the focal module
extern "C" {
  // Focal method under test
  void * sqlite3_wasm_pstack_alloc(int n);

  // Additional public utilities to observe internal state (no direct access to internals)
  unsigned char * sqlite3_wasm_pstack_ptr(void);
  void sqlite3_wasm_pstack_restore(unsigned char * p);
  int sqlite3_wasm_pstack_remaining(void);
  int sqlite3_wasm_pstack_quota(void);
}

// Simple non-terminating test harness (no GTest). Reports per-test status.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_START(name) \
  do { std::cout << "RUN: " << name << std::endl; } while(0)

#define CHECK(cond, msg) \
  do { \
    ++g_total_tests; \
    if(cond) { \
      std::cout << "  PASS: " << msg << std::endl; \
    } else { \
      ++g_failed_tests; \
      std::cerr << "  FAIL: " << msg << std::endl; \
    } \
  } while(0)

int main() {
  // Note: We intentionally do not reset the PStack between tests to exercise
  // the monotonic consumption behavior of sqlite3_wasm_pstack_alloc.

  // Step 2: Candidate test cases for sqlite3_wasm_pstack_alloc

  // Test 1: n <= 0 should return NULL
  TEST_START("pstack_alloc_negative_and_zero_returns_null");
  void *p_zero = sqlite3_wasm_pstack_alloc(0);
  CHECK(p_zero == nullptr, "n=0 should return NULL");
  void *p_negative = sqlite3_wasm_pstack_alloc(-5);
  CHECK(p_negative == nullptr, "n=-5 should return NULL");

  // Test 2: Alignment behavior and pointer advancement
  TEST_START("pstack_alloc_alignment_and_ptr_relation");
  int rem_before = sqlite3_wasm_pstack_remaining();
  void *p1 = sqlite3_wasm_pstack_alloc(9); // 9 aligns to 16
  void *p1_ptr = sqlite3_wasm_pstack_ptr();
  int rem_after = sqlite3_wasm_pstack_remaining();

  bool p1_nonnull = (p1 != nullptr);
  bool p1_aligned = p1_nonnull && (reinterpret_cast<uintptr_t>(p1) % 8 == 0);
  bool p1_matches_ptr = p1_ptr == p1;
  bool rem_decreased_expected = rem_after == rem_before - 16;

  CHECK(p1_nonnull && p1_aligned, "alloc(9) should allocate 16 bytes and be 8-byte aligned");
  CHECK(p1_matches_ptr, "Returned pointer should equal sqlite3_wasm_pstack_ptr()");
  CHECK(rem_decreased_expected, "Remaining space should decrease by 16 after alloc(9)");

  // Test 3: Exhaustive allocation in 8-byte chunks until space is depleted
  TEST_START("pstack_alloc_exhaust_8_byte_chunks");
  int allocated8 = 0;
  while (true) {
    void *p = sqlite3_wasm_pstack_alloc(8);
    if (p == nullptr) break;
    ++allocated8;
  }
  int rem_final = sqlite3_wasm_pstack_remaining();

  // The initial state before this loop was: 16 (from n=9) + 8 (from Test 1) = 24 bytes used
  // Total capacity = 4096 bytes. Remaining should be 4096 - 24 = 4072.
  // 4072 / 8 = 509 full 8-byte allocations should be possible.
  bool allocated8_expected = (allocated8 == 509);
  CHECK(allocated8_expected, "Should allocate exactly 509 blocks of 8 bytes after prior allocations");
  CHECK(rem_final == 0, "After exhausting memory, remaining should be 0");

  // Test 4: Allocation after exhaustion should fail (return NULL)
  TEST_START("pstack_alloc_after_exhaustion_should_fail");
  void *p_after = sqlite3_wasm_pstack_alloc(8);
  CHECK(p_after == nullptr, "alloc after exhaustion should return NULL");

  // Final report
  std::cout << "----------------------------------------" << std::endl;
  std::cout << "Total tests run: " << g_total_tests
            << ", Failures: " << g_failed_tests << std::endl;

  // Non-zero exit code on failure to integrate with CI pipelines
  return g_failed_tests;
}