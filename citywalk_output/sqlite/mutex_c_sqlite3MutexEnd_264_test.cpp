// Lightweight C++11 unit test suite for sqlite3MutexEnd (focal method)
// This test harness avoids GTest and uses a small custom EXPECT-like assertion system.
// It mocks the necessary SQLite global configuration to trigger both branches of sqlite3MutexEnd.

#include <iostream>
#include <sqliteInt.h>
#include <string>


// Ensure linkage with the focal C-style function under test
// The actual sqlite3MutexEnd is assumed to be linked into the test binary from the codebase under test.
extern "C" int sqlite3MutexEnd(void);

// Define SQLITE_OK to align with the focal method's expectations
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif

// Lightweight, non-terminating assertion macros
static int g_totalAssertions = 0;
static int g_failedAssertions = 0;

#define EXPECT_EQ(expected, actual) \
  do { \
    ++g_totalAssertions; \
    if ((expected) != (actual)) { \
      std::cerr << "EXPECT_EQ failed: expected " << (expected) \
                << " but got " << (actual) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
      ++g_failedAssertions; \
    } \
  } while(0)

#define EXPECT_TRUE(cond) \
  do { \
    ++g_totalAssertions; \
    if (!(cond)) { \
      std::cerr << "EXPECT_TRUE failed: condition is false (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
      ++g_failedAssertions; \
    } \
  } while(0)

#define EXPECT_FALSE(cond) \
  do { \
    ++g_totalAssertions; \
    if (cond) { \
      std::cerr << "EXPECT_FALSE failed: condition is true (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
      ++g_failedAssertions; \
    } \
  } while(0)

// ---------------------------------------------------------------------------
// Mocking the necessary SQLite global configuration to drive sqlite3MutexEnd
// Note: We only declare what's required by sqlite3MutexEnd: sqlite3GlobalConfig.mutex.xMutexEnd
// The actual type/layout in the real SQLite code may differ; this is a pragmatic mock
// to allow testing of the focal logic in isolation.
// ---------------------------------------------------------------------------

extern "C" {

// Define a minimal layout that matches the usage inside sqlite3MutexEnd():
// sqlite3GlobalConfig.mutex.xMutexEnd -> function pointer returning int
typedef int (*xMutexEndFn)(void);

// A compact, test-focused replica of the portion of SQLite's global config
typedef struct {
  struct {
    xMutexEndFn xMutexEnd;
  } mutex;
} sqlite3GlobalConfigType;

// Declare the global symbol that sqlite3MutexEnd will reference.
// In the real project, sqlite3GlobalConfig is defined in the library.
// Here, we provide a definition so tests can modify it at runtime.
sqlite3GlobalConfigType sqlite3GlobalConfig;

// Optional: provide forward declarations for test-only end-functions
int endFuncReturns7(void);
int endFuncIncrementAndReturn42(void);

} // extern "C"

// ---------------------------------------------------------------------------
// Test-specific end functions (C linkage)
// ---------------------------------------------------------------------------

// Case 1: end function returns 7 (to test propagation through sqlite3MutexEnd)
extern "C" int endFuncReturns7(void) {
  return 7;
}

// Case 2: end function increments a counter and returns 42 (to verify invocation)
static int s_incrementCounter = 0;
extern "C" int endFuncIncrementAndReturn42(void) {
  ++s_incrementCounter;
  return 42;
}

// Helper: reset the shared counter for deterministic tests
static void resetCounter() {
  s_incrementCounter = 0;
}

// ---------------------------------------------------------------------------
// Individual test cases (explanatory comments before each test)
// Each test exercises a small, isolated aspect of sqlite3MutexEnd's logic.
// The tests are designed to be non-terminating; they rely on EXPECT_* assertions.
// ---------------------------------------------------------------------------

// Test 1: When xMutexEnd is NULL, sqlite3MutexEnd should return SQLITE_OK and not call any function.
static void test_mutexEnd_whenXMutexEndIsNull() {
  // Setup: ensure the function pointer is NULL
  sqlite3GlobalConfig.mutex.xMutexEnd = nullptr;

  // Exercise
  int rc = sqlite3MutexEnd();

  // Verify: rc must remain SQLITE_OK
  EXPECT_EQ(SQLITE_OK, rc);

  // Explanation:
  // This validates the path where no mutex end function is registered.
  // The function should not attempt to call a NULL pointer and should return SQLITE_OK.
}

// Test 2: When xMutexEnd points to a function that returns 7, sqlite3MutexEnd should propagate that value.
static void test_mutexEnd_propagatesEndValue7() {
  // Setup: point to a function that returns 7
  sqlite3GlobalConfig.mutex.xMutexEnd = endFuncReturns7;

  // Exercise
  int rc = sqlite3MutexEnd();

  // Verify: rc must equal the return value of endFuncReturns7 (i.e., 7)
  EXPECT_EQ(7, rc);
}

// Test 3: When xMutexEnd is invoked, it should be called exactly once per sqlite3MutexEnd call.
// We verify both that the function is invoked and that the return value is propagated.
static void test_mutexEnd_invokesEndFunctionOnceAndPropagates42() {
  // Setup: use a function that increments a counter and returns 42
  resetCounter();
  sqlite3GlobalConfig.mutex.xMutexEnd = endFuncIncrementAndReturn42;

  // Exercise
  int rc = sqlite3MutexEnd();

  // Verify: the end function should be called exactly once and return 42
  EXPECT_EQ(1, s_incrementCounter); // counter should have incremented once
  EXPECT_EQ(42, rc);                  // return value should be 42
}

// Test 4: Sanity check to ensure multiple sequential calls reflect independent invocations
static void test_mutexEnd_multipleSequentialCalls() {
  // Setup: function that increments counter and returns 100
  // We'll implement a quick local function-like behavior by reusing endFuncIncrementAndReturn42,
  // but instrumented so we can distinguish different calls.
  // For simplicity, reuse endFuncIncrementAndReturn42 and reset counter in between.

  // Use the same function
  sqlite3GlobalConfig.mutex.xMutexEnd = endFuncIncrementAndReturn42;

  // First call
  resetCounter();
  int rc1 = sqlite3MutexEnd();
  int callsAfterFirst = s_incrementCounter;

  // Second call
  int rc2 = sqlite3MutexEnd();
  int callsAfterSecond = s_incrementCounter;

  // Verify: counter has incremented at least once, and both return values are as expected
  EXPECT_EQ(1, callsAfterFirst);   // first increment happened once
  EXPECT_EQ(2, callsAfterSecond);  // second increment happened again (total 2)
  EXPECT_EQ(42, rc1);
  EXPECT_EQ(42, rc2);
}

// ---------------------------------------------------------------------------
// Test harness and main
// ---------------------------------------------------------------------------

static void run_all_tests() {
  test_mutexEnd_whenXMutexEndIsNull();
  test_mutexEnd_propagatesEndValue7();
  test_mutexEnd_invokesEndFunctionOnceAndPropagates42();
  test_mutexEnd_multipleSequentialCalls();
}

int main() {
  // Initialize the mocked global config (layout must match test definitions)
  sqlite3GlobalConfig.mutex.xMutexEnd = nullptr;

  // Run tests
  run_all_tests();

  // Summary
  std::cout << "Tests executed: " << g_totalAssertions << std::endl;
  std::cout << "Assertions passed: " << (g_totalAssertions - g_failedAssertions) << std::endl;
  std::cout << "Assertions failed: " << g_failedAssertions << std::endl;

  return (g_failedAssertions == 0) ? 0 : 1;
}