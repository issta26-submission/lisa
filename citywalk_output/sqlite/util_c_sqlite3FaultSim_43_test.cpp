// Test suite for the focal method: sqlite3FaultSim
// Purpose: Verify correct branching and interaction with sqlite3GlobalConfig.xTestCallback
// Notes:
// - We assume the project provides sqlite3.h and the internal sqlite3FaultSim implementation compiled in the test linkage.
// - We test both branches of the conditional: when the callback is NULL and when it is non-NULL.
// - The tests are written in C++11, using a lightweight, non-terminating assertion style (console output).
// - Static/internal helpers in the target file are not accessed directly; we interact only via the public-facing API.

#include <math.h>
#include <sqlite3.h>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// Acquire C linkage for the SQLite API
extern "C" {
}

// Extern declaration of the global configuration used by sqlite3FaultSim
// This mirrors the internal sqlite3GlobalConfig.xTestCallback field.
// The actual type sqlite3GlobalConfig is defined in the sqlite3.h header (typedefs struct ...).
extern sqlite3GlobalConfig sqlite3GlobalConfig;

// Global test state to validate callback behavior
static int g_lastInput = -1;
static int g_callCount = 0;

// A test callback to inject via sqlite3GlobalConfig.xTestCallback
// Records the input and counts invocations, returns a value derived from input.
static int s_testCallback(int i) {
  g_lastInput = i;
  g_callCount++;
  return i * 10; // deterministic return value for verification
}

/*
  Test 1: Null callback branch
  - Set sqlite3GlobalConfig.xTestCallback to NULL
  - Call sqlite3FaultSim with a sample input
  - Expect SQLITE_OK as return value
  - Rationale: exercises the false branch of "xCallback ? xCallback(iTest) : SQLITE_OK"
*/
static void test_null_callback_branch() {
  sqlite3GlobalConfig.xTestCallback = nullptr; // Explicitly disable callback
  int res = sqlite3FaultSim(5);
  bool pass = (res == SQLITE_OK);
  std::cout << "[TEST] Null callback branch: " << (pass ? "PASS" : "FAIL")
            << " (expected SQLITE_OK, got " << res << ")" << std::endl;
}

/*
  Test 2: Non-null callback branch
  - Assign a known callback (s_testCallback)
  - Call sqlite3FaultSim with a sample input
  - Verify:
      a) The returned value equals the callback's computation (i * 10)
      b) The callback was invoked exactly once
      c) The input passed to the callback matches the test input
  - This covers the true branch of the conditional
*/
static void test_non_null_callback_branch() {
  g_lastInput = -1;
  g_callCount = 0;
  sqlite3GlobalConfig.xTestCallback = s_testCallback;

  int input = 7;
  int expectedReturn = input * 10;
  int res = sqlite3FaultSim(input);

  bool pass = (res == expectedReturn) && (g_lastInput == input) && (g_callCount == 1);
  std::cout << "[TEST] Non-null callback branch: " << (pass ? "PASS" : "FAIL")
            << " (expected " << expectedReturn << ", got " << res
            << "; lastInput=" << g_lastInput << ", calls=" << g_callCount << ")" << std::endl;
}

/*
  Test 3: Multiple-calls with non-null callback
  - Reuse the same non-null callback
  - Invoke sqlite3FaultSim repeatedly with a sequence of inputs
  - Verify each call returns input * 10 and the callback is invoked exactly once per call
  - This reinforces correct per-call handling and that state resets between calls are not persisting in an unintended way
*/
static void test_multiple_non_null_calls() {
  sqlite3GlobalConfig.xTestCallback = s_testCallback;

  bool allPass = true;
  for (int i = 0; i < 5; ++i) {
    g_lastInput = -999;
    g_callCount = 0;

    int res = sqlite3FaultSim(i);
    int expected = i * 10;
    bool ok = (res == expected) && (g_lastInput == i) && (g_callCount == 1);

    if (!ok) {
      allPass = false;
    }
  }

  std::cout << "[TEST] Multiple non-null calls: " << (allPass ? "PASS" : "FAIL") << std::endl;
}

/*
  Main entry: run all unit tests
  - We avoid terminating on first failure to maximize coverage
*/
int main() {
  std::cout << "Starting sqlite3FaultSim unit tests (no GTest required)" << std::endl;

  test_null_callback_branch();
  test_non_null_callback_branch();
  test_multiple_non_null_calls();

  std::cout << "Unit tests complete." << std::endl;

  // Return 0 on success regardless of individual test outcomes (status is communicated via stdout)
  return 0;
}