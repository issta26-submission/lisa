// Unit test suite for the focal method lsmMutexDel
// The goal is to validate the behavior of lsmMutexDel(lsm_env *pEnv, lsm_mutex *pMutex):
// - If pMutex is non-null, it should call pEnv->xMutexDel(pMutex)
// - If pMutex is null, it should not call xMutexDel
//
// Notes:
// - The test environment provides a minimal stand-in for the real C/C library types
// - We avoid using any external test framework (no GTest); a lightweight, self-contained harness is used
// - Assertions are non-terminating: tests log failures and continue
// - Static/class dependencies from the provided snippet are reflected in this test via
//   simple struct definitions to emulate the actual behavior

#include <lsmInt.h>
#include <iostream>


// Domain knowledge adherence: proper C++11 usage, simple test harness, and explicit non-terminating checks

// -------------------------
// Minimal type stubs for the unit under test
// -------------------------

// Opaque mutex type (stub)
struct lsm_mutex { };

// Environment structure expected by lsmMutexDel
// Contains a function pointer xMutexDel which is invoked when deleting a mutex
struct lsm_env {
  void (*xMutexDel)(lsm_mutex*);
};

// Focal function under test, implemented exactly as described in the prompt
// void lsmMutexDel(lsm_env *pEnv, lsm_mutex *pMutex){
 // if( pMutex ) pEnv->xMutexDel(pMutex);
// }
void lsmMutexDel(lsm_env *pEnv, lsm_mutex *pMutex){
  if( pMutex ) pEnv->xMutexDel(pMutex);
}

// -------------------------
// Test harness state: capture calls to xMutexDel via different envs
// -------------------------

// Env1 will use DelFromEnv1 to handle deletions
static lsm_mutex* g_lastDeletedEnv1 = nullptr;
static int g_deleteCountEnv1 = 0;
static void DelFromEnv1(lsm_mutex* m) {
  g_lastDeletedEnv1 = m;
  ++g_deleteCountEnv1;
}

// Env2 will use DelFromEnv2 to handle deletions, to verify independence of environments
static lsm_mutex* g_lastDeletedEnv2 = nullptr;
static int g_deleteCountEnv2 = 0;
static void DelFromEnv2(lsm_mutex* m) {
  g_lastDeletedEnv2 = m;
  ++g_deleteCountEnv2;
}

// -------------------------
// Helper assertion: non-terminating, logs failures but returns a boolean outcome
// -------------------------

static bool assert_true(bool cond, const char* msg) {
  if(!cond) {
    std::cerr << "Assertion failed: " << msg << std::endl;
    return false;
  }
  return true;
}

// -------------------------
// Test 1: Null mutex should not trigger deletion callback
// - Purpose: verify the false-branch behavior of the conditional
// -------------------------

static bool test_LsmMutexDel_NullMutex_DoesNotCall() {
  // Reset global state
  g_lastDeletedEnv1 = nullptr;
  g_deleteCountEnv1 = 0;

  // Prepare environment with a valid xMutexDel
  lsm_env env;
  env.xMutexDel = DelFromEnv1;

  // Call with null mutex; should NOT call xMutexDel
  lsmMutexDel(&env, nullptr);

  bool ok = assert_true(g_lastDeletedEnv1 == nullptr,
                        "xMutexDel should not be called when pMutex is null") &&
            assert_true(g_deleteCountEnv1 == 0,
                        "Deletion count should remain 0 after null mutex");

  if(!ok) {
    std::cerr << "Test 1 (Null mutex) FAILED\n";
  } else {
    std::cout << "Test 1 (Null mutex) PASSED\n";
  }
  return ok;
}

// -------------------------
// Test 2: Non-null mutex should trigger callback exactly once and with correct pointer
// - Purpose: verify the true-branch behavior of the conditional
// -------------------------

static bool test_LsmMutexDel_NonNullMutex_CallsEnv1Once() {
  // Reset global state
  g_lastDeletedEnv1 = nullptr;
  g_deleteCountEnv1 = 0;

  // Prepare environment with a valid xMutexDel
  lsm_env env;
  env.xMutexDel = DelFromEnv1;

  // Prepare a mutex instance
  lsm_mutex m;

  // Call under test
  lsmMutexDel(&env, &m);

  bool ok = assert_true(g_lastDeletedEnv1 == &m,
                        "xMutexDel should be called with the exact mutex pointer") &&
            assert_true(g_deleteCountEnv1 == 1,
                        "xMutexDel should be invoked exactly once for non-null mutex");

  if(!ok) {
    std::cerr << "Test 2 (Non-null mutex) FAILED\n";
  } else {
    std::cout << "Test 2 (Non-null mutex) PASSED\n";
  }
  return ok;
}

// -------------------------
// Test 3: Multiple calls across two separate environments confirm independence
// - Purpose: ensure each environment's callback is invoked correctly and independently
// -------------------------

static bool test_LsmMutexDel_MultipleCalls_WithTwoEnvs() {
  // Reset global state for both environments
  g_lastDeletedEnv1 = nullptr;
  g_deleteCountEnv1 = 0;
  g_lastDeletedEnv2 = nullptr;
  g_deleteCountEnv2 = 0;

  // Prepare two separate environments with their own callbacks
  lsm_env env1;
  env1.xMutexDel = DelFromEnv1;

  lsm_env env2;
  env2.xMutexDel = DelFromEnv2;

  // Two distinct mutex instances
  lsm_mutex m1;
  lsm_mutex m2;

  // First call on env1 with m1
  lsmMutexDel(&env1, &m1);
  bool ok1 = (g_lastDeletedEnv1 == &m1) && (g_deleteCountEnv1 == 1);

  // Second call on env2 with m2
  lsmMutexDel(&env2, &m2);
  bool ok2 = (g_lastDeletedEnv2 == &m2) && (g_deleteCountEnv2 == 1);

  bool ok = ok1 && ok2;
  if(!ok) {
    std::cerr << "Test 3 (Multiple envs) FAILED: "
              << "Env1 last=" << g_lastDeletedEnv1 << ", count=" << g_deleteCountEnv1
              << "; Env2 last=" << g_lastDeletedEnv2 << ", count=" << g_deleteCountEnv2
              << std::endl;
  } else {
    std::cout << "Test 3 (Multiple envs) PASSED\n";
  }
  return ok;
}

// -------------------------
// Main: Run all tests and report overall result
// -------------------------

int main() {
  std::cout << "Starting unit tests for lsmMutexDel...\n";

  bool t1 = test_LsmMutexDel_NullMutex_DoesNotCall();
  bool t2 = test_LsmMutexDel_NonNullMutex_CallsEnv1Once();
  bool t3 = test_LsmMutexDel_MultipleCalls_WithTwoEnvs();

  bool allPassed = t1 && t2 && t3;

  if(allPassed) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << "SOME TESTS FAILED" << std::endl;
    return 1;
  }
}

// End of test suite
// Candidate Keywords identified from the focal method and dependencies:
// - lsm_mutexDel, lsm_env, xMutexDel, pMutex, non-null branch, null branch
// - Dependency interactions: pEnv->xMutexDel(pMutex), proper pointer forwarding
// - Class/Module concepts: encapsulation via function pointer callbacks, environment-driven behavior
// - Domain considerations: simple function pointer-based callback invocation, safe-guarded NULL check