// C++11 test suite for the focal method testMallocOom
// This test exercises the wrapper function testMallocOom(lsm_env*, int, int, void(*)(void*), void*)
// by validating that the underlying tmMallocOom state is updated correctly.
// The tests are designed to be non-terminating on failures (log-only) to maximize coverage.

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <lsm.h>
#include <cstdint>


// The focal function is defined in lsmtest_mem.c with C linkage.
// We declare it here so the C++ test can invoke it.
extern "C" void testMallocOom(
  lsm_env *pEnv, 
  int nCountdown, 
  int bPersist,
  void (*xHook)(void *),
  void *pHookCtx
);

// Simple no-op hook used for testing the hook-pointer storage
static void testHookNoOp(void* /*ctx*/) {
  // Intentionally empty
}

// Minimal test harness to capture test results without terminating on failure
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define CHECK(cond, msg) do { \
  ++g_total_tests; \
  if(!(cond)) { \
    std::cerr << "FAIL: " << msg << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")" << std::endl; \
    ++g_failed_tests; \
  } \
} while(0)

// Test 1: Basic state initialization and storage of parameters
// - Set nCountdown, bPersist, xHook, pHookCtx
// - After wrapper call, tmNCountdown should reflect the internal countdown logic (decremented if >0)
// - xHook and pHookCtx should be stored as provided
// - bEnable should be 1 (enabled) after tmMallocOom
// - pFirst should remain untouched (nullptr) as no allocation occurs yet
static void testMallocOom_BasicInitialization() {
  lsm_env env = {0};
  TmGlobal tm = {0};
  env.pMemCtx = &tm;

  int hookCtxValue = 0;
  testMallocOom(&env, 3, 1, &testHookNoOp, &hookCtxValue);

  // The internal countdown should be decremented by 1 when > 0
  CHECK(tm.nCountdown == 2, "After testMallocOom(3,1,...), nCountdown should be 2 (decrement when >0)");

  // Persist flag should be stored
  CHECK(tm.bPersist == 1, "After testMallocOom, bPersist should be stored as 1");

  // Hook pointer should be stored
  CHECK(tm.xHook == &testHookNoOp, "After testMallocOom, xHook should point to provided hook");

  // Hook context should be stored
  CHECK(tm.pHookCtx == &hookCtxValue, "After testMallocOom, pHookCtx should point to hookCtxValue");

  // Enable flag should be set to 1
  CHECK(tm.bEnable == 1, "After testMallocOom, bEnable should be 1 (enabled)");
  
  // First block pointer should not have been touched (no allocation yet)
  CHECK(tm.pFirst == nullptr, "After testMallocOom, pFirst should remain nullptr (no allocation)");
}

// Test 2: Second invocation should update state accordingly
// - Use different countdown and persistence flags
// - xHook and pHookCtx can be NULL to verify they can be cleared
static void testMallocOom_SecondInvocation() {
  lsm_env env = {0};
  TmGlobal tm = {0};
  env.pMemCtx = &tm;

  // First call to establish initial state
  testMallocOom(&env, 3, 1, &testHookNoOp, nullptr);

  // Second call with new parameters
  testMallocOom(&env, 5, 0, nullptr, nullptr);

  // Countdown should be decremented again (from 5 to 4)
  CHECK(tm.nCountdown == 4, "After second testMallocOom(5,0,...), nCountdown should be 4 (decrement from 5)");

  // Persist flag should reflect the new value
  CHECK(tm.bPersist == 0, "After second testMallocOom, bPersist should be 0");

  // Hook and hook context should be reset to NULL
  CHECK(tm.xHook == nullptr, "After second testMallocOom, xHook should be NULL");
  CHECK(tm.pHookCtx == nullptr, "After second testMallocOom, pHookCtx should be NULL");

  // Enable flag should remain 1
  CHECK(tm.bEnable == 1, "After second testMallocOom, bEnable should remain 1");
}

static void run_all_tests() {
  testMallocOom_BasicInitialization();
  testMallocOom_SecondInvocation();

  std::cout << "Test Summary: total=" << g_total_tests
            << ", failed=" << g_failed_tests << std::endl;
  if (g_failed_tests == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
  } else {
    std::cerr << "SOME TESTS FAILED" << std::endl;
  }
}

int main() {
  run_all_tests();
  return g_failed_tests;
}