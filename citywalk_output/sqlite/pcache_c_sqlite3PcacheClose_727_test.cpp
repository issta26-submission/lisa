// Minimal test harness for sqlite3PcacheClose (pcache.c) using C++11, without GTest.
// The test includes a lightweight sqliteInt.h substitute to enable compiling pcache.c
// in a single translation unit. It then exercises the true branch (non-null pCache) and
// the false branch (pCache->pCache == 0) using a fork to safely trigger the assert.

#include <sys/types.h>
#include <cstdio>
#include <sys/wait.h>
#include <cstring>
#include <sqliteInt.h>
#include <unistd.h>
#include <pcache.c>
#include <iostream>
#include <cstdlib>
#include <cassert>


// ------------------------------------------------------------------------------------
// Minimal sqliteInt.h substitute (declares only what's needed by pcache.c in this test).
// ------------------------------------------------------------------------------------

#ifndef SQLITEINT_H
#define SQLITEINT_H

// Lightweight typedefs to satisfy pcache.c needs
typedef unsigned char u8;
typedef unsigned int Pgno;
typedef struct PgHdr PgHdr; // forward declaration for test scaffolding
typedef struct sqlite3_pcache_page sqlite3_pcache_page;

// Define a minimal PCache structure compatible with pcache.c access
typedef struct PCache PCache;
struct PCache {
  void *pCache;
};

// Forward declare for xDestroy hook
typedef struct {
  void (*xDestroy)(void*);
} sqlite3_pcache_methods2;

// Global config structure with only the pcache2 hook used by sqlite3PcacheClose
typedef struct {
  sqlite3_pcache_methods2 pcache2;
} sqlite3GlobalConfigStruct;

// Extern global config used by sqlite3PcacheClose
extern sqlite3GlobalConfigStruct sqlite3GlobalConfig;

// Variadic tracing function used by pcache.c
#ifdef __cplusplus
extern "C" {
#endif
void pcacheTrace(const char* fmt, ...);
#ifdef __cplusplus
}
#endif

#define SQLITE_NOINLINE

#endif // SQLITEINT_H

// Provide a concrete instance of sqlite3GlobalConfig with a test xDestroy hook.
// testDestroy is declared below (forward).
static void testDestroy(void* p);

// Forward declare to initialize hook structures before testDestroy definition
static sqlite3_pcache_methods2 test_pcache_methods2 = { testDestroy };
static sqlite3GlobalConfigStruct sqlite3GlobalConfig = { test_pcache_methods2 };


// ------------------------------------------------------------------------------------
// C linkage bridge for pcache.c inclusion
// ------------------------------------------------------------------------------------
static int gTraceCallCount = 0;

// Implement a test version of pcacheTrace to count invocations.
#ifdef __cplusplus
extern "C" {
#endif
void pcacheTrace(const char* fmt, ...) {
  (void)fmt;
  ++gTraceCallCount;
}
#ifdef __cplusplus
}
#endif

// Forward declare and define destruction hook (to verify that xDestroy is called with correct pointer)
static void testDestroy(void* p) {
  // Nothing here; test will capture the argument via a dedicated global
  // The real capture is in the test harness below; this function is required by the
  // sqlite3GlobalConfig.pcache2.xDestroy hook.
  // We'll actually capture the pointer in the test via a separate global variable.
  // The function body is kept empty to minimize side effects.
  (void)p;
}

// Globals to capture destroy argument for verification
static void* gLastDestroyedPointer = nullptr;
static int gDestroyCallCount = 0;

// Override the xDestroy to capture its input
static void testDestroyCapture(void* p) {
  gLastDestroyedPointer = p;
  ++gDestroyCallCount;
}

// Rewind the test hook setup to use the capture function
static sqlite3_pcache_methods2 test_pcache_methods2_capture = { testDestroyCapture };
static sqlite3GlobalConfigStruct sqlite3GlobalConfig_capture = { test_pcache_methods2_capture };

// Ensure the actual sqlite3GlobalConfig symbol used by pcache.c points to the capture setup.
// We'll swap the global config at runtime in main() before tests.
// Note: The symbol sqlite3GlobalConfig must be the one referenced by pcache.c;
// We provide both a declaration and a definition; at runtime we'll rebind as needed.


// ------------------------------------------------------------------------------------
// Include the actual pcache.c source inside this translation unit, ensuring C linkage.
// We wrap the include in extern "C" to maintain C linkage for symbols like sqlite3PcacheClose.
// ------------------------------------------------------------------------------------
extern "C" {
}

// ----------------------------
// Lightweight test harness
// ----------------------------

// Simple non-terminating test assertion helper (logs failures but continues)
static int gTestFailures = 0;
static void expect_true(bool cond, const char* msg) {
  if (!cond) {
    std::cerr << "TEST FAILURE: " << msg << std::endl;
    ++gTestFailures;
  }
}

// Test 1: True branch of sqlite3PcacheClose
// - Sets up a PCache with a non-null pCache.
// - Verifies that xDestroy is called with the exact pCache pointer.
// - Verifies that pcacheTrace was invoked (via a counter).
static bool test_sqlite3PcacheClose_trueBranch() {
  // Reset trackers
  gTraceCallCount = 0;
  gDestroyCallCount = 0;
  gLastDestroyedPointer = nullptr;

  // Use a dummy object as the pCache payload
  int dummy;
  PCache pc;
  pc.pCache = &dummy;

  // Install capture destroy hook
  sqlite3GlobalConfig_capture.pcache2.xDestroy = testDestroyCapture;

  // Call the function under test
  sqlite3PcacheClose(&pc);

  // Validate that the destroy hook was called exactly once with the correct argument
  bool okDestroyPointer =
    (gLastDestroyedPointer == pc.pCache) && (gDestroyCallCount == 1);

  // Validate that the trace function was invoked at least once
  bool okTrace =
    (gTraceCallCount >= 1);

  expect_true(okDestroyPointer, "Destroy should be called with the exact pCache pointer.");
  expect_true(okTrace, "pcacheTrace should be invoked on CLOSE.");

  return (gTestFailures == 0);
}

// Test 2: False branch of sqlite3PcacheClose
// - Spawns a child process to deliberately trigger the assert (pCache->pCache == 0).
// - The child exiting with SIGABRT indicates the assertion fired as expected.
static bool test_sqlite3PcacheClose_falseBranch() {
#ifdef __unix__
  pid_t pid = fork();
  if (pid < 0) {
    std::cerr << "Fork failed; skipping false branch test." << std::endl;
    return false;
  } else if (pid == 0) {
    // Child: invoke CLOSE with non-null pCache but pCache==nullptr to trigger assert
    PCache pc;
    pc.pCache = nullptr;
    // This should trigger an assertion and abort the child process
    sqlite3PcacheClose(&pc);
    // If somehow it returns, exit cleanly
    _exit(0);
  } else {
    int status = 0;
    waitpid(pid, &status, 0);
    // Child should be terminated by SIGABRT due to assertion failure
    bool childTerminatedByAbort = WIFSIGNALED(status) && (WTERMSIG(status) == SIGABRT);
    if (!childTerminatedByAbort) {
      std::cerr << "False branch test did not abort as expected." << std::endl;
    }
    return childTerminatedByAbort;
  }
#else
  // Non-POSIX environments: skip this test
  std::cout << "False branch test skipped (requires fork)." << std::endl;
  return true;
#endif
}

// ----------------------------
// Main: run tests
// ----------------------------
int main() {
  // Ensure the capture translation unit uses the capture destroy hook before tests
  // We rebind the sqlite3GlobalConfig pointer to the capture version to intercept xDestroy.
  // Since sqlite3GlobalConfig is declared in the test header, we can assign to it directly.
  // The pcache.c compilation during this translation unit uses sqlite3GlobalConfig
  // to resolve xDestroy; by reassigning its pcache2.xDestroy, we capture the calls.

  // Bind to capture-based config for tests
  // Replace the previous banner with capture hook
  sqlite3GlobalConfig = sqlite3GlobalConfig_capture;

  // Run Test 1
  bool t1 = test_sqlite3PcacheClose_trueBranch();
  // Run Test 2 (may be skipped on non-POSIX)
  bool t2 = test_sqlite3PcacheClose_falseBranch();

  int totalFailures = gTestFailures;
  if (!t1) ++totalFailures;
  if (!t2) ++totalFailures;

  if (totalFailures == 0) {
    std::cout << "All tests passed." << std::endl;
  } else {
    std::cout << "Tests failed: " << totalFailures << std::endl;
  }

  return totalFailures ? 1 : 0;
}