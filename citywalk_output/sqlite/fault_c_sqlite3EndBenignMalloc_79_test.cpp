/* 
   File: sqliteInt.h (test stub)
   Purpose: Minimal stub to simulate the environment used by sqlite3EndBenignMalloc
   in fault.c. This stub provides:
     - A WsdHooks structure with a function pointer xBenignEnd
     - A global instance wsdHooks
     - A no-op macro wsdHooksInit to simplify testing and allow controlled behaviour
   Notes:
     - The real project may define a more complex TLS/initialization mechanism.
     - For tests we force wsdHooksInit to be a no-op so we can observe true/false branches.
*/ 

#ifndef SQLITE_INT_H
#define SQLITE_INT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void (*xBenignEnd)(void);
} WsdHooks;

extern WsdHooks wsdHooks;

/* In production, wsdHooksInit may perform TLS-related initialization.
   For unit tests, we keep it as a no-op to observe direct effect of
   wsdHooks.xBenignEnd being NULL or non-NULL. */
#define wsdHooksInit do { } while(0)

#ifdef __cplusplus
}
#endif

#endif // SQLITE_INT_H

/* 
   File: test_fault_end_benign_malloc.cpp
   Purpose: Provide a lightweight, non-terminating test suite (no GTest)
            for the focal method sqlite3EndBenignMalloc located in fault.c.

   Approach:
     - Create a minimal environment by defining the required type and
       a global instance (wsdHooks) consistent with the stub header.
     - Override wsdHooksInit to no-op (already done by the stub macro).
     - Test two branches of the conditional:
         1) Non-null xBenignEnd should invoke the callback exactly once.
         2) Null xBenignEnd should not invoke any callback.
     - Use simple, non-terminating assertions and print PASS/FAIL per test.
*/

#include <iostream>
#include <cstdio>
#include <sqliteInt.h>


// Include the test stub to match the real code's environment

// Provide the actual definition for the external global variable so linking succeeds.
WsdHooks wsdHooks = { nullptr };

// Declaration of the focal function (present in fault.c)
extern "C" void sqlite3EndBenignMalloc(void);

// Simple non-terminating test framework
static int g_test_failures = 0;

#define ASSERT_EQ(expected, actual, msg)                                  \
  do {                                                                     \
    if ((expected) != (actual)) {                                        \
      std::cerr << "TEST FAIL: " << (msg)                               \
                << " | Expected: " << (expected)                           \
                << ", Actual: " << (actual) << std::endl;               \
      ++g_test_failures;                                                 \
    } else {                                                             \
      std::cout << "TEST PASS: " << (msg) << std::endl;                  \
    }                                                                    \
  } while(0)

static int g_callback_count = 0;

// Callback used to verify invocation of xBenignEnd
static void benigEndCallback(void) {
  ++g_callback_count;
}

// Test 1: When xBenignEnd is non-null, the callback should be invoked exactly once.
void test_endBenignMalloc_invokes_callback_when_non_null() {
  g_callback_count = 0;
  wsdHooks.xBenignEnd = &benigEndCallback;

  sqlite3EndBenignMalloc();

  ASSERT_EQ(1, g_callback_count, "sqlite3EndBenignMalloc should invoke non-null xBenignEnd exactly once");
}

// Test 2: When xBenignEnd is NULL, the callback should not be invoked.
void test_endBenignMalloc_does_not_invoke_when_null() {
  g_callback_count = 0;
  wsdHooks.xBenignEnd = NULL;

  sqlite3EndBenignMalloc();

  ASSERT_EQ(0, g_callback_count, "sqlite3EndBenignMalloc should not invoke when xBenignEnd is NULL");
}

// A small runner that executes all tests in this file
void run_all_tests() {
  std::cout << "Starting sqlite3EndBenignMalloc tests..." << std::endl;
  test_endBenignMalloc_invokes_callback_when_non_null();
  test_endBenignMalloc_does_not_invoke_when_null();
  std::cout << "Finished tests. Failures: " << g_test_failures << std::endl;
}

int main() {
  run_all_tests();
  // Non-terminating assertions are used; return non-zero if any test failed.
  return g_test_failures != 0 ? 1 : 0;
}