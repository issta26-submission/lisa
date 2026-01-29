// Lightweight C++11 test suite for sqlite3_wasm_pstack_ptr and related stack controls.
// This test exercises the static per-process stack (PStack) in sqlite3-wasm.c
// by invoking the C-callable API functions exposed there.
// Notes:
// - We assume sqlite3-wasm.c is compiled and linked with this test.
// - We deliberately avoid terminating assertions; instead we collect and report failures.
// - We rely on C linkage for the functions, so wrap in extern "C" when declaring.

// Compile note (example):
// g++ -std=c++11 -O2 -Wall test_pstack.cpp sqlite3-wasm.c -o test_pstack

#include <emscripten/wasmfs.h>
#include <assert.h>
#include <cinttypes>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Declarations of the C APIs (extern "C" for C linkage)
extern "C" {
  void * sqlite3_wasm_pstack_ptr(void);
  void * sqlite3_wasm_pstack_alloc(int n);
  void sqlite3_wasm_pstack_restore(unsigned char * p);
  int sqlite3_wasm_pstack_remaining(void);
  int sqlite3_wasm_pstack_quota(void);
}

// Simple non-terminating test harness
static int g_tests = 0;
static int g_failures = 0;

#define TEST(cond, msg) do {                               \
    ++g_tests;                                             \
    if(!(cond)) {                                           \
      ++g_failures;                                        \
      std::cerr << "Test FAILED: " << msg << "\n";        \
    } else {                                               \
      std::cout << "Test PASSED: " << msg << "\n";        \
    }                                                        \
  } while(0)

int main() {
  // The underlying PStack_mem is 512 * 8 = 4096 bytes.
  // PStack.pBegin points to the start; PStack.pEnd points just past the end;
  // PStack.pPos starts pointing at the end (i.e., stack is empty).

  // Test 1: Initial remaining should be full capacity (4096 bytes).
  // This confirms that on startup the pstack is completely unused.
  int rem0 = sqlite3_wasm_pstack_remaining();
  TEST(rem0 == 4096, "Initial remaining == 4096 (full stack) named capacity");

  // Snapshot the initial pointer (should be end of PStack_mem)
  void * pEndInitial = sqlite3_wasm_pstack_ptr();

  // Test 2: Allocate 128 bytes and verify pointer moves back by 128
  void * pBeforeAlloc128 = sqlite3_wasm_pstack_ptr();
  void * pAlloc128 = sqlite3_wasm_pstack_alloc(128);
  TEST(pAlloc128 != nullptr, "Alloc(128) should succeed (non-null pointer)");
  void * pAfterAlloc128 = sqlite3_wasm_pstack_ptr();
  intptr_t diff128 = (intptr_t)pBeforeAlloc128 - (intptr_t)pAfterAlloc128;
  TEST(diff128 == 128, "Pointer moved back by 128 after alloc(128)");
  int rem1 = sqlite3_wasm_pstack_remaining();
  TEST(rem1 == 4096 - 128, "Remaining after alloc(128) == 3968");

  // Test 3: Allocate the remaining 3968 bytes to reach pBegin
  // This should succeed and place pPos at the beginning of PStack_mem.
  void * pBeforeAllocRest = sqlite3_wasm_pstack_ptr();
  void * pAllocRest = sqlite3_wasm_pstack_alloc(3968);
  TEST(pAllocRest != nullptr, "Alloc(3968) should succeed to reach pBegin");
  void * pAfterAllocRest = sqlite3_wasm_pstack_ptr();
  TEST(pAfterAllocRest == (void*)((uintptr_t)pBeforeAllocRest - 3968),
       "Pointer should now be at pBegin after allocating remaining space");
  int remFinal = sqlite3_wasm_pstack_remaining();
  TEST(remFinal == 0, "Remaining after consuming all space should be 0");

  // Test 4: Allocation should fail when no space left
  void * pAllocFail = sqlite3_wasm_pstack_alloc(8);
  TEST(pAllocFail == nullptr, "Alloc(8) should fail when no space left");

  // Test 5: Quota should reflect total capacity (4096)
  int quota = sqlite3_wasm_pstack_quota();
  TEST(quota == 4096, "Quota() should report total capacity 4096");

  // Test 6: Restore to current pointer should be a no-op (no crash, pointer unchanged)
  void * pCurrent = sqlite3_wasm_pstack_ptr();
  sqlite3_wasm_pstack_restore((unsigned char*)pCurrent);
  void * pAfterRestoreCurrent = sqlite3_wasm_pstack_ptr();
  TEST(pAfterRestoreCurrent == pCurrent,
       "Restore to current pointer should be a no-op and keep pointer unchanged");

  // Summary
  std::cout << "Summary: total tests = " << g_tests
            << ", failures = " << g_failures << std::endl;

  // Return non-zero if any test failed
  return (g_failures ? 1 : 0);
}