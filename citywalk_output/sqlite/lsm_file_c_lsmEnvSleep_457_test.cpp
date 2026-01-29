// Test suite for the focal method lsmEnvSleep in lsm_file.c
// Purpose: Verify that lsmEnvSleep delegates the call to pEnv->xSleep(pEnv, nUs)
// without altering parameters, i.e., that the function pointer is invoked with
// the same environment pointer and the exact microseconds value.
//
// Notes:
// - This test uses the project's lsm_env type as defined in lsmInt.h.
// - It avoids GTest and uses a lightweight, non-terminating assertion style.
// - The tests are written in C++11 and link against the C implementation of lsmEnvSleep
//   (i.e., lsm_file.c must be compiled and linked into the test binary).
// - All explanatory comments are provided for clarity per the instructions.

#include <sys/types.h>
#include <lsmInt.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>


// Bring in the project-specific type definitions.
// This header should expose the lsm_env struct, including the xSleep function pointer:
// void (*xSleep)(lsm_env*, int);

extern "C" {
  // Declaration of the focal function under test.
  // The function is defined in lsm_file.c (C)
  void lsmEnvSleep(lsm_env *pEnv, int nUs);
}

// -----------------------------------------------------------------------------
// Test 1: Basic delegation test
// - Purpose: Ensure that lsmEnvSleep calls the xSleep callback exactly once
//   with the provided environment pointer and the nUs value.
// - Mechanism: Provide a mock xSleep that records the arguments it received.
// -----------------------------------------------------------------------------

static lsm_env *g_last_env_basic = nullptr;
static int g_last_nUs_basic = 0;

// Mock implementation of xSleep to capture call details.
static void mock_xSleep_basic(lsm_env *pEnv, int nUs) {
  g_last_env_basic = pEnv;
  g_last_nUs_basic = nUs;
}

bool test_lsmEnvSleep_basic() {
  // Arrange
  lsm_env env;
  env.xSleep = mock_xSleep_basic;

  g_last_env_basic = nullptr;
  g_last_nUs_basic = -1;

  // Act
  lsmEnvSleep(&env, 12345);

  // Assert (non-terminating, prints result)
  bool passed = (g_last_env_basic == &env) && (g_last_nUs_basic == 12345);
  if (passed) {
    std::cout << "[PASS] test_lsmEnvSleep_basic: lsmEnvSleep delegates to xSleep with correct args.\n";
  } else {
    std::cerr << "[FAIL] test_lsmEnvSleep_basic: Delegation or argument passing failed.\n";
  }
  return passed;
}


// -----------------------------------------------------------------------------
// Test 2: Multi-environment delegation test
// - Purpose: Ensure that lsmEnvSleep works correctly when two different environments
//   provide distinct xSleep callbacks. Each callback should receive its own env pointer
//   and the proper nUs value when invoked.
// - Mechanism: Create two separate env objects with two separate mocks, call
//   lsmEnvSleep on each, and verify correct coupling.
// -----------------------------------------------------------------------------

static lsm_env *g_last_env_A = nullptr;
static int g_last_nUs_A = 0;

static lsm_env *g_last_env_B = nullptr;
static int g_last_nUs_B = 0;

static void mock_xSleep_A(lsm_env *pEnv, int nUs) {
  g_last_env_A = pEnv;
  g_last_nUs_A = nUs;
}

static void mock_xSleep_B(lsm_env *pEnv, int nUs) {
  g_last_env_B = pEnv;
  g_last_nUs_B = nUs;
}

bool test_lsmEnvSleep_multiple_envs() {
  // Arrange
  lsm_env envA;
  envA.xSleep = mock_xSleep_A;

  lsm_env envB;
  envB.xSleep = mock_xSleep_B;

  g_last_env_A = nullptr;
  g_last_nUs_A = -1;

  g_last_env_B = nullptr;
  g_last_nUs_B = -1;

  // Act: Call for envA
  lsmEnvSleep(&envA, 7);
  bool passA = (g_last_env_A == &envA) && (g_last_nUs_A == 7);

  // Act: Call for envB
  lsmEnvSleep(&envB, 13);
  bool passB = (g_last_env_B == &envB) && (g_last_nUs_B == 13);

  // Assert
  if (passA && passB) {
    std::cout << "[PASS] test_lsmEnvSleep_multiple_envs: Correct delegation for multiple envs.\n";
  } else {
    if (!passA)
      std::cerr << "[FAIL] test_lsmEnvSleep_multiple_envs: envA delegation failed.\n";
    if (!passB)
      std::cerr << "[FAIL] test_lsmEnvSleep_multiple_envs: envB delegation failed.\n";
  }

  return passA && passB;
}


// -----------------------------------------------------------------------------
// Main: Run all tests
// -----------------------------------------------------------------------------

int main() {
  int total = 0;
  int failures = 0;

  auto run = [&](bool passed) {
    total++;
    if (!passed) failures++;
  };

  bool t1 = test_lsmEnvSleep_basic();
  run(t1);

  bool t2 = test_lsmEnvSleep_multiple_envs();
  run(t2);

  // Summary
  if (failures == 0) {
    std::cout << "[SUMMARY] All tests passed (" << total << " tests).\n";
  } else {
    std::cerr << "[SUMMARY] Some tests failed: " << failures << " of " << total << ".\n";
  }

  return failures;
}