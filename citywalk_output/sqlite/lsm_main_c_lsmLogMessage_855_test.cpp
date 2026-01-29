/***************************************************************************************
 * Unit test suite for the focal method lsmLogMessage located in lsm_main.c
 * This test suite is implemented in C++11 (no GTest) and uses a lightweight
 * in-house EXPECT-style non-terminating assertions to maximize coverage.
 *
 * Key dependencies and context (taken from the focal code and class dependencies):
 * - lsm_db: core database object with fields xLog, pLogCtx, pEnv
 * - xLog: callback function invoked when logging is enabled
 * - LsmString: intermediate string builder used to compose log messages
 * - lsmStringInit, lsmStringVAppendf, lsmStringClear: helpers for string formation
 * - lsmLogMessage: the method under test
 *
 * Candidate Keywords (Step 1): lsm_db, xLog, pLogCtx, pEnv, LsmString, lsmStringInit,
 * lsmStringVAppendf, lsmStringClear, lsmLogMessage
 *
 * Domain knowledge (Step 3) used:
 * - Use of C varargs forwarding through lsmLogMessage
 * - Correct handling when xLog is NULL (no operation)
 * - Non-terminating assertions to maximize code coverage
 * - Interoperability with C code: include C headers via extern "C"
 * - Access to static-like behavior via the public API only (no private members)
 *
 * Notes:
 * - This test does not rely on GTest. It uses a minimal custom EXPECT_TRUE macro.
 * - The tests rely on the actual lsmString implementation present in the linked
 *   project to build the log string. To validate content, the mock xLog captures
 *   the string passed to it and stores a copy for assertion.
 ****************************************************************************************/

#include <lsmInt.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdarg>
#include <iostream>


// Step 1: Candidate Keywords are annotated above and will be used to guide test design.

// Include the C headers of the module under test to obtain correct types.
// The headers are expected to provide: lsm_env, lsm_db, LsmString, and prototypes
// for lsmStringInit, lsmStringVAppendf, lsmStringClear, and lsmLogMessage.
extern "C" {
}

// Ensure we have a known symbol for lsmLogMessage with C linkage
extern "C" {
void lsmLogMessage(lsm_db *pDb, int rc, const char *zFormat, ...);
}

// Domain knowledge: lightweight test harness with non-terminating assertions
static int g_failures = 0;
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    ++g_failures; \
    fprintf(stderr, "EXPECT_TRUE failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
  } \
} while(0)

// Global test state to capture logs produced by lsmLogMessage
static int g_call_count = 0;
static int g_last_rc = 0;
static std::string g_last_log_str;

// Mock of the xLog callback to capture its arguments for verification
static void mock_xLog(void *pCtx, int rc, const char *z) {
  (void)pCtx; // unused in tests, but kept for signature fidelity
  ++g_call_count;
  g_last_rc = rc;
  if(z) g_last_log_str = z;
  else g_last_log_str.clear();
}

// Test 1: Ensure no operation occurs when xLog is NULL
static void test_lsmLogMessage_noLogCallback() {
  // Reset test state
  g_call_count = 0;
  g_last_rc = 0;
  g_last_log_str.clear();

  // Prepare a db object with no logging callback
  lsm_db db;
  std::memset(&db, 0, sizeof(db));
  db.xLog = nullptr; // No logging callback
  db.pEnv = nullptr;
  db.pLogCtx = nullptr;

  // Call the function under test
  lsmLogMessage(&db, 5, "format %d", 1);

  // Verify: no log callback should be invoked
  EXPECT_TRUE(g_call_count == 0);
  // Additional sanity checks
  EXPECT_TRUE(g_last_rc == 0);
  EXPECT_TRUE(g_last_log_str.empty());
}

// Test 2: Ensure proper behavior when xLog is provided
static void test_lsmLogMessage_withCallback() {
  // Reset test state
  g_call_count = 0;
  g_last_rc = 0;
  g_last_log_str.clear();

  // Prepare a db object with a real logging callback
  lsm_db db;
  std::memset(&db, 0, sizeof(db));
  db.xLog = mock_xLog;
  db.pEnv = nullptr;
  db.pLogCtx = nullptr;

  // Call the function under test with a formatted message
  lsmLogMessage(&db, 7, "hello %s number %d", "world", 9);

  // Verify: xLog should be invoked exactly once with the correct rc and formatted string
  EXPECT_TRUE(g_call_count == 1);
  EXPECT_TRUE(g_last_rc == 7);
  EXPECT_TRUE(!g_last_log_str.empty());
  // Check that the produced string includes the expected substrings
  EXPECT_TRUE(g_last_log_str.find("hello") != std::string::npos);
  EXPECT_TRUE(g_last_log_str.find("world") != std::string::npos);
  EXPECT_TRUE(g_last_log_str.find("9") != std::string::npos);
}

// A simple test runner
static void runAllTests() {
  printf("Running lsmLogMessage test suite...\n");
  test_lsmLogMessage_noLogCallback();
  test_lsmLogMessage_withCallback();

  if(g_failures == 0) {
    printf("All tests passed.\n");
  } else {
    printf("Tests finished with %d failure(s).\n", g_failures);
  }
}

int main() {
  runAllTests();
  return g_failures; // Non-zero exit code if any test failed
}