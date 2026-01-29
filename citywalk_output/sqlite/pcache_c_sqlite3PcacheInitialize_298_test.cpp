/*
  Unit test suite for the focal method: sqlite3PcacheInitialize (pcache.c)

  Step 1 (Program Understanding): The focal method checks the global PCache2
  initialization hook (sqlite3GlobalConfig.pcache2.xInit). If it is NULL (0),
  it calls sqlite3PCacheSetDefault() to install a built-in default page-cache,
  then asserts that xInit is no longer NULL. Finally, it invokes xInit with the
  argument sqlite3GlobalConfig.pcache2.pArg and returns the result.

  Step 2 (Test Case Generation): We exercise two code paths:
  - Branch 1: xInit != NULL initially -> should call the provided Init function
    and return its value without calling the default setter.
  - Branch 2: xInit == NULL initially -> should call sqlite3PCacheSetDefault(),
    then call the (now installed) Init function with the provided Arg and return
    its value. We shadow sqlite3PCacheSetDefault with a test double to control
    behavior deterministically.

  Step 3 (Test Case Refinement): Tests are implemented without GTest/GMock, using
  simple non-terminating assertions (they accumulate failures and run all tests).
  Static members in the focal file are mocked via symbol overriding (link-time
  shadowing) to keep tests self-contained and fully executable in C++11.

  Notes:
  - The test uses a minimal, proxy declaration of sqlite3GlobalConfig to access
    the necessary fields (pcache2.xInit and pcache2.pArg) without requiring the
    full SQLite internal headers.
  - We shadow sqlite3PCacheSetDefault() to simulate the default installation logic.
  - All tests run from main() and report a summary at the end.

  Limitations:
  - This test assumes the build environment links the test object before the
    pcache.c object (to allow symbol shadowing of sqlite3PCacheSetDefault).
  - We only exercise the sqlite3PcacheInitialize function and its direct
    interactions with sqlite3GlobalConfig and sqlite3PCacheSetDefault.

  Build notes:
  - Compile with: g++ -std=c++11 -O2 -Wall -Wextra -Werror test_pcache_initialize.cpp pcache.c -o test_pcache
  - Ensure the linker resolves the test's sqlite3PCacheSetDefault symbol by ordering object files appropriately.
*/

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Step 2/3: Provide a minimal, C-compatible declaration of the global config used by pcache.c.
// We declare an anonymous struct type with the needed member path:
// sqlite3GlobalConfig.pcache2.xInit and sqlite3GlobalConfig.pcache2.pArg.
extern "C" struct {
  struct {
    int (*xInit)(void*);
    void *pArg;
  } pcache2;
} sqlite3GlobalConfig;

// Forward declaration of the function under test (from pcache.c)
extern "C" int sqlite3PcacheInitialize(void);

// Backdoor: shadow sqlite3PCacheSetDefault to control behavior when xInit == 0
static int g_defaultCalled = 0;
static intptr_t g_defaultChosenArg = 0;
static intptr_t g_lastInitArg_OnNonZeroCase = -1;

// Test probe: record the arg when a non-zero Init function is used
static intptr_t g_lastInitArg = -1;

// Test Init functions (C linkage to be compatible with PCache API)
extern "C" int testInitNonZero(void *arg);
extern "C" int testInitFromDefault(void *arg);

// Implementation of testInitNonZero: used when xInit is non-zero initially
extern "C" int testInitNonZero(void *arg) {
  g_lastInitArg = reinterpret_castintptr_t(arg); // capture arg for assertions (cast helper below)
  // Return a known value to verify correct path execution
  return 42;
}

// Helper to cast void* to intptr_t (to avoid compiler warning in C++11 without using other headers)
static inline intptr_t reinterpret_castintptr_t(void* p) {
  return reinterpret_cast<intptr_t>(p);
}

// Correction: use a proper inline function to avoid misuse in testInitNonZero
extern "C" int testInitNonZero_fix(void *arg) {
  g_lastInitArg = reinterpret_cast<intptr_t>(arg);
  return 42;
}

// For clarity, rewire testInitNonZero to use the fix
#define testInitNonZero testInitNonZero_fix

// TestInitFromDefault: used after sqlite3PCacheSetDefault installs the default Init
extern "C" int testInitFromDefault(void *arg) {
  g_defaultChosenArg = reinterpret_castintptr_t(arg);
  return 77;
}

// Shadow of sqlite3PCacheSetDefault to simulate default installer
extern "C" void sqlite3PCacheSetDefault(void) {
  g_defaultCalled = 1;
  // Install our test default Init function and a known Arg
  sqlite3GlobalConfig.pcache2.xInit = testInitFromDefault;
  sqlite3GlobalConfig.pcache2.pArg = (void*)0x12345678;
  g_defaultChosenArg = 0; // reset, will be set by testInitFromDefault when invoked
}

// Utility: reset global test state between tests
static void resetTestState() {
  g_defaultCalled = 0;
  g_defaultChosenArg = 0;
  g_lastInitArg = -1;
  // Leave sqlite3GlobalConfig as-is; individual tests will set xInit/pArg as needed
}

// True/False assertion helper (non-terminating): prints message on failure
static bool assert_true(bool cond, const char* msg) {
  if(!cond) {
    std::cerr << "Assertion failed: " << (msg ? msg : "unknown") << std::endl;
    return false;
  }
  return true;
}

// Test 1: xInit != 0 initially -> sqlite3PcacheInitialize should call the provided xInit
static bool test_nonzero_branch_behavior() {
  resetTestState();

  // Arrange: xInit is non-null; point to our testInitNonZero function
  sqlite3GlobalConfig.pcache2.xInit = testInitNonZero;
  sqlite3GlobalConfig.pcache2.pArg = (void*)0xDEAD;

  // Act
  int rv = sqlite3PcacheInitialize();

  // Assert: should call testInitNonZero and return its value; default setter should not be invoked
  bool ok = true;
  ok &= assert_true(rv == 42, "Return value should be 42 when xInit is non-null");
  ok &= assert_true(g_lastInitArg == reinterpret_castintptr_t((void*)0xDEAD),
                    "Init function should receive the provided Arg (0xDEAD)");
  ok &= assert_true(g_defaultCalled == 0, "sqlite3PCacheSetDefault should not be called when xInit is non-null");

  if(!ok) {
    std::cerr << "[test_nonzero_branch] rv=" << rv
              << ", lastArg=" << g_lastInitArg
              << ", defaultCalled=" << g_defaultCalled << std::endl;
  }
  return ok;
}

// Test 2: xInit == 0 initially -> sqlite3PcacheInitialize should call sqlite3PCacheSetDefault,
// then call the installed default Init with the Arg and return its value
static bool test_zero_branch_uses_default() {
  resetTestState();

  // Arrange: xInit is NULL; Arg value is irrelevant until after default installs its own
  sqlite3GlobalConfig.pcache2.xInit = nullptr;
  sqlite3GlobalConfig.pcache2.pArg = nullptr;

  // Act
  int rv = sqlite3PcacheInitialize();

  // Assert: default installer should have been invoked, a new Init should have been used,
  // and the Return value should be from testInitFromDefault (77)
  bool ok = true;
  ok &= assert_true(rv == 77, "Return value should be 77 after default Init is installed");
  ok &= assert_true(g_defaultCalled != 0, "sqlite3PCacheSetDefault should have been called when xInit is NULL");
  ok &= assert_true(g_defaultChosenArg == reinterpret_castintptr_t((void*)0x12345678),
                    "Default Init should have been invoked with Arg 0x12345678");

  if(!ok) {
    std::cerr << "[test_zero_branch_uses_default] rv=" << rv
              << ", defaultCalled=" << g_defaultCalled
              << ", chosenArg=" << g_defaultChosenArg
              << std::endl;
  }
  return ok;
}

// Step 3: Test Case Refinement via main()
int main() {
  int tests_run = 0;
  int tests_failed = 0;

  // Run test 1
  std::cout << "Running test_nonzero_branch_behavior..." << std::endl;
  if(test_nonzero_branch_behavior()) {
    std::cout << "  PASSED" << std::endl;
  } else {
    std::cout << "  FAILED" << std::endl;
    ++tests_failed;
  }
  ++tests_run;

  // Run test 2
  std::cout << "Running test_zero_branch_uses_default..." << std::endl;
  if(test_zero_branch_uses_default()) {
    std::cout << "  PASSED" << std::endl;
  } else {
    std::cout << "  FAILED" << std::endl;
    ++tests_failed;
  }
  ++tests_run;

  // Summary
  std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << std::endl;
  return (tests_failed == 0) ? 0 : 1;
}