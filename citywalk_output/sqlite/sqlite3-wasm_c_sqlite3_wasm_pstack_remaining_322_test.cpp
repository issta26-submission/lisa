/*
  Unit test suite for sqlite3_wasm_pstack_remaining and related pstack APIs.

  This test targets the focal method:
    sqlite3_wasm_pstack_remaining(void)

  It relies on the following public (exported) C interfaces provided by sqlite3-wasm.c:
    - void * sqlite3_wasm_pstack_ptr(void);
    - void sqlite3_wasm_pstack_restore(unsigned char * p);
    - void * sqlite3_wasm_pstack_alloc(int n);
    - int sqlite3_wasm_pstack_remaining(void);
    - int sqlite3_wasm_pstack_quota(void);

  Important: The internal PStack state is static and not directly accessible from tests.
  We exercise the API to move and restore the stack pointer, then verify the remaining
  bytes against the quota. We avoid terminating assertions; test failures are reported
  and the test sequence continues to improve coverage.

  Candidate Keywords (Step 1):
  - PStack, pBegin, pEnd, pPos
  - sqlite3_wasm_pstack_alloc, sqlite3_wasm_pstack_remaining, sqlite3_wasm_pstack_quota
  - Pointer arithmetic, alignment to 8 bytes
  - Boundary conditions (full/empty stack), restore functionality
  - Domain-specific behavior of WASM-backed stack management
*/

/* Step 2: Include necessary headers and declare C interfaces with C linkage */

#include <iostream>
#include <assert.h>
#include <string>
#include <emscripten/wasmfs.h>


// Declare the required C interfaces from the focal module.
// Use extern "C" to avoid C++ name mangling when linking against C code.
extern "C" {
  // Returns the current pPos pointer (end of the pstack by default if untouched)
  void * sqlite3_wasm_pstack_ptr(void);

  // Restore pPos to a previously captured position within [pBegin, pEnd]
  void sqlite3_wasm_pstack_restore(unsigned char * p);

  // Allocate n bytes on the pstack; returns new pPos or 0 on failure
  void * sqlite3_wasm_pstack_alloc(int n);

  // Returns the number of bytes currently available from pBegin to pPos
  int sqlite3_wasm_pstack_remaining(void);

  // Returns the total quota size of the pstack (pEnd - pBegin)
  int sqlite3_wasm_pstack_quota(void);
}

/* Step 3: Lightweight test harness (non-terminating assertions) */

// Global counters for test results
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Utility to mark a test as passed
static void mark_pass(const std::string& testName) {
  std::cout << "[PASS] " << testName << std::endl;
  ++g_tests_run;
  ++g_tests_passed;
}

// Utility to mark a test as failed
static void mark_fail(const std::string& testName, const std::string& reason) {
  std::cerr << "[FAIL] " << testName << ": " << reason << std::endl;
  ++g_tests_run;
  ++g_tests_failed;
}

// Assertion helpers (non-terminating)
#define TEST_EQ(expected, actual, testName) do { \
  ++g_tests_run; \
  if ((expected) != (actual)) { \
    mark_fail(testName, "expected " + std::to_string(expected) + \
              ", got " + std::to_string(actual)); \
  } else { \
    mark_pass(testName); \
  } \
} while(0)

#define TEST_TRUE(cond, testName) do { \
  ++g_tests_run; \
  if (!(cond)) { \
    mark_fail(testName, "condition is false"); \
  } else { \
    mark_pass(testName); \
  } \
} while(0)

/* Step 4: Test cases for sqlite3_wasm_pstack_remaining and related APIs

   Test philosophy:
   - Use the public API to manipulate the PStack without relying on internal state.
   - Validate that remaining equals quota when the pstack is pristine.
   - Validate after allocations that remaining decreases by the allocated (aligned) amount.
   - Validate restore functionality brings pPos back to a previously captured position.
   - Validate behavior when attempting to allocate more than available space (returns 0).
   - Each test is annotated with comments explaining its purpose.
*/

// Test 1: Initial state consistency: remaining should equal quota
static bool test_initial_remaining() {
  // Capture quota and remaining in the initial state
  int quota = sqlite3_wasm_pstack_quota();
  int rem = sqlite3_wasm_pstack_remaining();

  // Expect remaining == quota in the initial state (pPos at end)
  TEST_EQ(quota, rem, "Initial remaining equals quota");
  return g_tests_failed == 0;
}

// Test 2: Single small allocation reduces remaining by aligned size
static bool test_single_allocation_reduces_remaining() {
  int quota = sqlite3_wasm_pstack_quota();
  // Capture current pPos to allow later restoration
  unsigned char * orig = (unsigned char *)sqlite3_wasm_pstack_ptr();

  // Allocate 128 bytes (8-byte aligned by allocator)
  void * p = sqlite3_wasm_pstack_alloc(128);
  TEST_TRUE(p != nullptr, "pstack_alloc(128) should succeed");

  int rem = sqlite3_wasm_pstack_remaining();
  TEST_EQ(quota - 128, rem, "Remaining after 128-byte allocation");

  // Restore to original position for test isolation
  sqlite3_wasm_pstack_restore(orig);
  int remAfterRestore = sqlite3_wasm_pstack_remaining();
  TEST_EQ(quota, remAfterRestore, "Remaining after restore to original position");
  return g_tests_failed == 0;
}

// Test 3: Multiple allocations and cumulative remaining
static bool test_multiple_allocations() {
  int quota = sqlite3_wasm_pstack_quota();

  // Start from current position
  unsigned char * orig1 = (unsigned char *)sqlite3_wasm_pstack_ptr();

  // First allocation: 1000 bytes (aligned to 8)
  void * a1 = sqlite3_wasm_pstack_alloc(1000);
  TEST_TRUE(a1 != nullptr, "pstack_alloc(1000) 1st alloc");

  // Second allocation: 1000 bytes
  void * a2 = sqlite3_wasm_pstack_alloc(1000);
  TEST_TRUE(a2 != nullptr, "pstack_alloc(1000) 2nd alloc");

  int rem = sqlite3_wasm_pstack_remaining();
  int expected = quota - 2000;
  TEST_EQ(expected, rem, "Remaining after two 1000-byte allocations");

  // Restore to original and verify
  sqlite3_wasm_pstack_restore(orig1);
  int remAfterRestore = sqlite3_wasm_pstack_remaining();
  TEST_EQ(quota, remAfterRestore, "Remaining after restore to original");

  return g_tests_failed == 0;
}

// Test 4: Over-allocation should fail (return 0 and not modify pPos)
static bool test_over_allocation_should_fail() {
  int quota = sqlite3_wasm_pstack_quota();

  // Start from pristine initial state
  unsigned char* orig = (unsigned char*)sqlite3_wasm_pstack_ptr();

  // Try to allocate well beyond the remaining space
  int tooBig = quota + 4096; // definitely too large
  void * p = sqlite3_wasm_pstack_alloc(tooBig);
  TEST_EQ((void*)nullptr, p, "pstack_alloc(tooBig) should return nullptr (0)");

  // Ensure pPos unchanged (remaining equals quota)
  int rem = sqlite3_wasm_pstack_remaining();
  TEST_EQ(quota, rem, "Remaining unchanged after failed allocation");

  // Restore to original
  sqlite3_wasm_pstack_restore(orig);
  return g_tests_failed == 0;
}

// Test 5: Restore using captured position returns to the exact previous state
static bool test_restore_to_original_position() {
  int quota = sqlite3_wasm_pstack_quota();

  unsigned char* orig = (unsigned char*)sqlite3_wasm_pstack_ptr();
  void* p1 = sqlite3_wasm_pstack_alloc(256);
  TEST_TRUE(p1 != nullptr, "pstack_alloc(256) for restore test");

  int remAfterAlloc = sqlite3_wasm_pstack_remaining();
  TEST_EQ(quota - 256, remAfterAlloc, "Remaining after 256-byte allocation");

  // Restore back to the captured original position
  sqlite3_wasm_pstack_restore(orig);

  int remAfterRestore = sqlite3_wasm_pstack_remaining();
  TEST_EQ(quota, remAfterRestore, "Remaining after restore to original position");

  return g_tests_failed == 0;
}

// Step 5: Run all tests and report results
int main() {
  std::cout << "Running sqlite3_wasm_pstack_remaining tests (C++11, non-GTest).\n";

  bool ok = true;
  ok &= test_initial_remaining();
  ok &= test_single_allocation_reduces_remaining();
  ok &= test_multiple_allocations();
  ok &= test_over_allocation_should_fail();
  ok &= test_restore_to_original_position();

  // Summary
  std::cout << "\nTest Summary: " << g_tests_passed << " passed, "
            << g_tests_failed << " failed, "
            << g_tests_run << " total.\n";

  // Return non-zero if any test failed (so CI can detect failure)
  return (g_tests_failed == 0) ? 0 : 1;
}