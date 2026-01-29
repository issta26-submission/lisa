// Unit tests for lsmMutexTry in lsm_mutex.c
// Target: C++11, no Google Test, no external test framework.
// The tests create a fake lsm_env with a controllable xMutexTry function
// to verify that lsmMutexTry forwards the mutex pointer and propagates the return code.
//
// Notes:
// - We assume lsm_env.xMutexTry has signature: int (*)(lsm_mutex *)
// - We assign a C-linkage test function to simulate behavior.
// - We test both non-zero and zero return codes to cover branches.
// - No private members are accessed; tests rely only on the public interface defined in lsmInt.h.

#include <lsmInt.h>
#include <cstdio>
#include <cstring>


extern "C" {
}

// Global test state to validate callback behavior
static int g_returnCode = 0;
static lsm_mutex *g_lastMutexPtr = nullptr;

// C linkage fake implementation of xMutexTry to be assigned into lsm_env
extern "C" int fakeXMutexTry(lsm_mutex *pMutex) {
  // Record the exact mutex pointer passed to the underlying implementation
  g_lastMutexPtr = pMutex;
  // Return the controlled value to test propagation
  return g_returnCode;
}

// Simple non-terminating assertion macro: reports failure but continues
static int g_failures = 0;
#define ASSERT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    std::printf("ASSERT FAILED: %s (in %s:%d) - %s\n", #cond, __FILE__, __LINE__, (msg)); \
    ++g_failures; \
  } \
} while(0)

// Test 1: Forwarding of pMutex and propagation of non-zero return code
// - Sets up a tailored lsm_env with fakeXMutexTry returning a non-zero value.
// - Verifies that lsmMutexTry forwards the same mutex pointer and returns the same code.
static void test_lsmMutexTry_ForwardsPointerAndNonZeroReturn() {
  // Prepare environment
  g_lastMutexPtr = nullptr;
  g_returnCode = 42; // non-zero return code to simulate error
  lsm_env env;
  // Zero initialize to be explicit (matches typical C struct usage)
  std::memset(&env, 0, sizeof(env));
  // Attach fake function
  env.xMutexTry = &fakeXMutexTry;

  // Prepare a dummy mutex object
  lsm_mutex m;
  // No need to initialize fields of m for this test; the fakeXMutexTry doesn't inspect it
  int rc = lsmMutexTry(&env, &m);

  // Assertions: return code propagated, and same mutex pointer passed through
  ASSERT_TRUE(rc == 42, "lsmMutexTry should propagate non-zero return code from xMutexTry");
  ASSERT_TRUE(g_lastMutexPtr == &m, "lsmMutexTry should forward the same mutex pointer to xMutexTry");
}

// Test 2: Forwarding of pMutex and propagation of zero return code
// - Similar to Test 1 but with a zero return code to cover the true/false branch
static void test_lsmMutexTry_ForwardsPointerZeroReturn() {
  g_lastMutexPtr = nullptr;
  g_returnCode = 0; // zero return code to simulate success
  lsm_env env;
  std::memset(&env, 0, sizeof(env));
  env.xMutexTry = &fakeXMutexTry;

  lsm_mutex m;
  int rc = lsmMutexTry(&env, &m);

  // Assertions: return code propagated as zero, pointer forwarded
  ASSERT_TRUE(rc == 0, "lsmMutexTry should propagate zero return code from xMutexTry");
  ASSERT_TRUE(g_lastMutexPtr == &m, "lsmMutexTry should forward the same mutex pointer to xMutexTry");
}

// Simple test runner
int main() {
  // Run tests
  test_lsmMutexTry_ForwardsPointerAndNonZeroReturn();
  test_lsmMutexTry_ForwardsPointerZeroReturn();

  // Report summary
  if(g_failures == 0) {
    std::printf("ALL TESTS PASSED\n");
  } else {
    std::printf("TESTS COMPLETED with %d FAILURE(S)\n", g_failures);
  }

  // Return non-zero if any failure occurred to make CI easy to detect
  return g_failures ? 1 : 0;
}