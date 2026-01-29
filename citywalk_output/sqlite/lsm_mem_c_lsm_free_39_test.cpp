// Minimal unit-test suite for the focal method lsm_free in lsm_mem.c
// This test suite verifies that lsm_free correctly forwards the environment
// parameter to lsmFree, and uses the default environment when a NULL pEnv is passed.
// The test suite is self-contained and does not rely on Google Test.
// It uses lightweight, non-terminating assertions to maximize execution coverage.

#include <cstddef>
#include <lsmInt.h>
#include <iostream>


// Include the project's dependencies to get the correct type definitions.
// lsmInt.h is expected to define lsm_env and declare lsm_default_env and lsmFree.

// Bring the C symbols into C++ linkage for proper symbol resolution.
extern "C" {
  // Declaration of the function under test (C linkage)
  void lsm_free(lsm_env *pEnv, void *p);

  // Prototypes that lsm_free will call (C linkage). We provide our own mocks below.
  void lsmFree(lsm_env*, void*);
  lsm_env *lsm_default_env();
}

// Global state to capture interactions performed by the mocked lsmFree.
static lsm_env *g_last_free_env = nullptr;
static void *g_last_free_ptr = nullptr;

// Mock: capture the arguments passed to lsmFree when lsm_free is invoked.
extern "C" void lsmFree(lsm_env* env, void* p) {
  g_last_free_env = env;
  g_last_free_ptr = p;
}

// Mock: provide a stable default environment instance for tests.
// This mirrors the typical project behavior where lsm_default_env() returns a singleton.
extern "C" lsm_env *lsm_default_env() {
  static lsm_env def;
  return &def;
}

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT(cond, msg) \
  do { \
    if(!(cond)) { \
      ++g_tests_failed; \
      std::cerr << "[FAIL] " << __func__ << ": " << msg << "\n"; \
    } \
  } while(false)

#define TEST_BEGIN(test_name) \
  void test_##test_name()

#define TEST_END(test_name) \
  /* no-op */

// Test 1: Non-null environment should be forwarded to lsmFree
// This ensures that lsm_free uses the provided environment without modification.
TEST_BEGIN(lsm_free_passes_through_non_null_env) {
  ++g_tests_run;
  // Reset captured state
  g_last_free_env = nullptr;
  g_last_free_ptr = nullptr;

  lsm_env local_env;                 // Explicit non-null environment
  void *dummy_ptr = (void*)0x1234;     // Non-null pointer payload

  // Call the function under test
  lsm_free(&local_env, dummy_ptr);

  // Assertions: environment should be the same as provided; pointer passes through
  EXPECT(g_last_free_env == &local_env, "Expected non-null env to be forwarded to lsmFree");
  EXPECT(g_last_free_ptr == dummy_ptr, "Expected payload pointer to be forwarded unchanged");
}
TEST_END(lsm_free_passes_through_non_null_env)

// Test 2: Null environment should use the default environment
// This checks the conditional operator pEnv ? pEnv : lsm_default_env() behavior.
TEST_BEGIN(lsm_free_uses_default_env_when_null) {
  ++g_tests_run;
  g_last_free_env = nullptr;
  g_last_free_ptr = nullptr;

  void *payload = (void*)0xDEAD;

  lsm_free(nullptr, payload);

  // Assertions: environment should be the default_env instance and payload should be forwarded
  EXPECT(g_last_free_env != nullptr, "Default environment should be provided when pEnv is NULL");
  EXPECT(g_last_free_env == lsm_default_env(), "Expected default environment instance to be used");
  EXPECT(g_last_free_ptr == payload, "Expected payload pointer to be forwarded unchanged");
}
TEST_END(lsm_free_uses_default_env_when_null)

// Test 3: Null environment and null payload should still route through correctly
TEST_BEGIN(lsm_free_handles_null_ptr_and_null_env) {
  ++g_tests_run;
  g_last_free_env = nullptr;
  g_last_free_ptr = nullptr;

  lsm_free(nullptr, nullptr);

  // Assertions: both env and pointer behavior should be correct (pointer remains NULL)
  EXPECT(g_last_free_ptr == nullptr, "NULL payload should be forwarded as NULL");
  EXPECT(g_last_free_env == lsm_default_env(), "Default environment should be used when pEnv is NULL");
}
TEST_END(lsm_free_handles_null_ptr_and_null_env)

// Entry point: run all tests and report results.
// This avoids reliance on an external test framework.
int main() {
  test_lsm_free_passes_through_non_null_env();
  test_lsm_free_uses_default_env_when_null();
  test_lsm_free_handles_null_ptr_and_null_env();

  std::cout << "Tests run: " << g_tests_run << "\n";
  std::cout << "Tests passed: " << (g_tests_run - g_tests_failed) << "\n";
  std::cout << "Tests failed: " << g_tests_failed << "\n";

  // Return non-zero if any test failed to aid automation.
  return g_tests_failed ? 1 : 0;
}