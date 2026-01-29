// C++11 test suite for lsmMalloc in lsm_mem.c
// Focus: verify that lsmMalloc forwards its arguments to the environment's xMalloc callback
// and returns exactly what xMalloc returns. Also cover the nullptr return path.
// Note: This test suite avoids terminating assertions and uses a lightweight, self-contained runner.

#include <cstdlib>
#include <lsmInt.h>
#include <cstdio>
#include <cstddef>


// Bring in C declarations from the project.
// The focal method lsmMalloc is declared/defined in C sources; ensure correct linkage.
extern "C" {
}

// Global test state used by the fake xMalloc implementations to verify call details.
static lsm_env* g_LastEnv = nullptr;
static size_t   g_LastN   = 0;
static void*    g_LastReturn = nullptr;

// Fake xMalloc implementations to control/test behavior

// Normal allocator: records inputs and returns a freshly allocated block
static void* TestMalloc(lsm_env* pEnv, size_t N) {
  g_LastEnv = pEnv;
  g_LastN = N;
  g_LastReturn = std::malloc(N);
  return g_LastReturn;
}

// Nullable allocator: records inputs and returns nullptr to simulate allocation failure
static void* TestMallocNull(lsm_env* pEnv, size_t N) {
  g_LastEnv = pEnv;
  g_LastN = N;
  g_LastReturn = nullptr;
  return nullptr;
}

/*
  Test 1: lsmMalloc forwards parameters to xMalloc and returns its result
  - Set up an lsm_env with xMalloc = TestMalloc
  - Call lsmMalloc(&env, 128)
  - Validate: g_LastEnv is &env, g_LastN is 128, and the returned pointer equals g_LastReturn
  - Cleanup allocated memory
*/
static bool test_lsmMalloc_forwards_params() {
  printf("Test 1: lsmMalloc forwards parameters to xMalloc and returns its result\n");

  // Reset global trackers
  g_LastEnv = nullptr;
  g_LastN = 0;
  g_LastReturn = nullptr;

  // Prepare environment with our fake allocator
  static lsm_env env;
  env.xMalloc = TestMalloc;

  // Call the focal method
  void* result = lsmMalloc(&env, 128);

  // Validate forwarding behavior
  bool ok = true;
  if (g_LastEnv != &env) {
    printf("  FAIL: xMalloc received wrong env: got %p, want %p\n", (void*)g_LastEnv, (void*)&env);
    ok = false;
  }
  if (g_LastN != 128) {
    printf("  FAIL: xMalloc received wrong N: got %zu, want 128\n", g_LastN);
    ok = false;
  }
  if (result != g_LastReturn) {
    printf("  FAIL: return value from lsmMalloc does not match xMalloc return\n");
    ok = false;
  }

  // Clean up any allocated memory produced by TestMalloc
  if (g_LastReturn) {
    std::free(g_LastReturn);
  }

  return ok;
}

/*
  Test 2: lsmMalloc propagates nullptr when xMalloc returns nullptr
  - Set up an lsm_env with xMalloc = TestMallocNull
  - Call lsmMalloc(&env, 64)
  - Validate: result is nullptr and parameters were forwarded correctly
*/
static bool test_lsmMalloc_propagates_null_return() {
  printf("Test 2: lsmMalloc propagates nullptr when xMalloc returns nullptr\n");

  // Reset global trackers
  g_LastEnv = nullptr;
  g_LastN = 0;
  g_LastReturn = nullptr;

  // Prepare environment with a null-return allocator
  static lsm_env env;
  env.xMalloc = TestMallocNull;

  // Call the focal method
  void* result = lsmMalloc(&env, 64);

  // Validate
  bool ok = true;
  if (result != nullptr) {
    printf("  FAIL: Expected nullptr return from lsmMalloc, got %p\n", result);
    ok = false;
  }
  if (g_LastEnv != &env) {
    printf("  FAIL: xMalloc received wrong env: got %p, want %p\n", (void*)g_LastEnv, (void*)&env);
    ok = false;
  }
  if (g_LastN != 64) {
    printf("  FAIL: xMalloc received wrong N: got %zu, want 64\n", g_LastN);
    ok = false;
  }

  return ok;
}

/*
  Test 3: lsmMalloc handles a zero-size allocation correctly (forwarding zero is allowed by malloc)
  - Reuse TestMalloc to verify forwarding for N = 0
  - Ensure we can clean up if a non-null pointer is returned
*/
static bool test_lsmMalloc_zero_size_forwarding() {
  printf("Test 3: lsmMalloc forwards zero-size allocations correctly\n");

  // Reset global trackers
  g_LastEnv = nullptr;
  g_LastN = 0;
  g_LastReturn = nullptr;

  static lsm_env env;
  env.xMalloc = TestMalloc;

  void* result = lsmMalloc(&env, 0);

  bool ok = true;
  if (g_LastEnv != &env) {
    printf("  FAIL: xMalloc received wrong env for N=0: got %p, want %p\n", (void*)g_LastEnv, (void*)&env);
    ok = false;
  }
  if (g_LastN != 0) {
    printf("  FAIL: xMalloc received wrong N for N=0: got %zu, want 0\n", g_LastN);
    ok = false;
  }
  if (result != g_LastReturn) {
    printf("  FAIL: return value for N=0 does not match xMalloc return\n");
    ok = false;
  }

  if (g_LastReturn) {
    std::free(g_LastReturn);
  }

  return ok;
}

/*
  Lightweight test harness:
  - Runs all tests and reports overall success/failure.
  - Does not terminate the test program on a failing assertion.
*/
int main() {
  int total = 0;
  int passed = 0;

  // Run tests
  if (test_lsmMalloc_forwards_params()) passed++; total++;
  if (test_lsmMalloc_propagates_null_return()) passed++; total++;
  if (test_lsmMalloc_zero_size_forwarding()) passed++; total++;

  printf("Summary: %d/%d tests passed.\n", passed, total);
  if (passed != total) {
    printf("Some tests failed. See above output for details.\n");
    return 1;
  }
  printf("All tests passed.\n");
  return 0;
}