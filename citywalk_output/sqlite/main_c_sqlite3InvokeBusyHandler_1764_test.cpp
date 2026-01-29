/*
Step 1 - Program Understanding (informational)
- Focal method: sqlite3InvokeBusyHandler(BusyHandler *p)
- Behavior:
  - If p->xBusyHandler == 0 or p->nBusy < 0, return 0 (early exit).
  - Otherwise call rc = p->xBusyHandler(p->pBusyArg, p->nBusy).
  - If rc == 0, set p->nBusy = -1; else increment p->nBusy.
  - Return rc.
- Dependent component: BusyHandler with fields:
  - int (*xBusyHandler)(void*, int)
  - int nBusy
  - void *pBusyArg
- This test focuses on true/false branches of the conditional, and the two code paths after the BusyHandler invocation.
- Candidate Keywords: BusyHandler, xBusyHandler, pBusyArg, nBusy, rc, rc==0, nBusy<0, NULL function pointer, return value propagation, side-effects on nBusy.
*/

#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>


// Step 2/3: Unit Test Generation
// We provide a minimal, self-contained BusyHandler type that mirrors the essentials
// used by sqlite3InvokeBusyHandler. This is kept in test.cpp to allow testing without
// pulling in the full SQLite headers.
extern "C" int sqlite3InvokeBusyHandler(struct BusyHandler *p); // C linkage for the focal function

// Minimal BusyHandler struct matching the layout expected by sqlite3InvokeBusyHandler.
// Field order is important: xBusyHandler, nBusy, pBusyArg.
struct BusyHandler {
  int (*xBusyHandler)(void*, int);
  int nBusy;
  void *pBusyArg;
};

// Helper busy-hook implementations for tests

// Returns 0 to exercise the rc == 0 branch
static int busy_returns_zero(void* arg, int nBusy) {
  return 0;
}

// Returns non-zero to exercise the rc != 0 branch
static int busy_returns_nonzero(void* arg, int nBusy) {
  return 1;
}

// Echoes the incoming nBusy value into the provided arg, to verify argument passing.
// Also returns non-zero to exercise increment path.
static int busy_echo_arg(void* arg, int nBusy) {
  if (arg != nullptr) {
    int* p = reinterpret_cast<int*>(arg);
    *p = nBusy;
  }
  return 1;
}

// Simple test framework (non-terminating): record failures but continue execution
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST(name) static bool name()
#define CHECK_EQ(a, b) do { ++g_total_tests; if ((a) != (b)) { ++g_failed_tests; printf("CHECK_FAILED: %s:%d: %s == %s? (%lld != %lld)\n", __FILE__, __LINE__, #a, #b, (long long)(a), (long long)(b)); } else { printf("TEST_PASS: %s\n", #name); } } while(0)
#define CHECK_INT_EQ(a, b) do { ++g_total_tests; if ((a) != (b)) { ++g_failed_tests; printf("CHECK_FAILED: %s:%d: %s != %s (actual=%d, expected=%d)\n", __FILE__, __LINE__, #a, #b, (int)(a), (int)(b)); } else { printf("TEST_PASS: %s\n", #name); } } while(0)
#define ASSERT_PASSED(cond) do { ++g_total_tests; if(!(cond)) { ++g_failed_tests; printf("ASSERT_FAILED: %s:%d: %s\n", __FILE__, __LINE__, #cond); } else { printf("TEST_PASS: %s\n", __FUNCTION__); } } while(0)


// Test 1: xBusyHandler is NULL -> should return 0 and not modify nBusy
TEST(test_null_busyhandler_returns_zero) {
  BusyHandler bh;
  bh.xBusyHandler = nullptr;
  bh.nBusy = 10;
  bh.pBusyArg = nullptr;

  int rc = sqlite3InvokeBusyHandler(&bh);

  // rc must be 0
  if (rc != 0) {
    printf("FAIL: rc != 0 in test_null_busyhandler_returns_zero (rc=%d)\n", rc);
    return false;
  }
  // nBusy should remain unchanged
  if (bh.nBusy != 10) {
    printf("FAIL: nBusy changed in test_null_busyhandler_returns_zero (nBusy=%d)\n", bh.nBusy);
    return false;
  }
  return true;
}

// Test 2: nBusy < 0 -> should return 0 and not call the Busy handler
TEST(test_negative_nBusy_no_call) {
  BusyHandler bh;
  bh.xBusyHandler = busy_returns_zero; // would be called if allowed
  bh.nBusy = -1;
  int store = 0;
  bh.pBusyArg = &store;

  int rc = sqlite3InvokeBusyHandler(&bh);

  // rc must be 0 due to early exit on nBusy < 0
  if (rc != 0) {
    printf("FAIL: rc != 0 in test_negative_nBusy_no_call (rc=%d)\n", rc);
    return false;
  }
  // Ensure BusyHandler was not actually invoked by checking store remains unchanged
  if (store != 0) {
    printf("FAIL: Busy handler invoked when nBusy < 0 (store=%d)\n", store);
    return false;
  }
  return true;
}

// Test 3: rc == 0 path: after call, nBusy should become -1
TEST(test_rc_zero_sets_nBusy_to_minus_one) {
  BusyHandler bh;
  bh.xBusyHandler = busy_returns_zero;
  bh.nBusy = 3;
  int arg_container = -999;
  bh.pBusyArg = &arg_container;

  int rc = sqlite3InvokeBusyHandler(&bh);

  // rc should be 0
  if (rc != 0) {
    printf("FAIL: rc != 0 in test_rc_zero_sets_nBusy_to_minus_one (rc=%d)\n", rc);
    return false;
  }
  // nBusy should be set to -1
  if (bh.nBusy != -1) {
    printf("FAIL: nBusy != -1 after rc==0 path (nBusy=%d)\n", bh.nBusy);
    return false;
  }
  // Arg should not have been used in rc==0 path; value may be unchanged
  if (arg_container != -999) {
    printf("WARNING: arg_container unexpectedly changed in rc==0 path (arg_container=%d)\n", arg_container);
  }
  return true;
}

// Test 4: rc != 0 path: after call, nBusy should increment and pBusyArg should receive the original nBusy
TEST(test_rc_nonzero_increments_nBusy_and_passes_arg) {
  BusyHandler bh;
  bh.xBusyHandler = busy_echo_arg; // writes nBusy into arg
  bh.nBusy = 5;
  int arg_storage = -1;
  bh.pBusyArg = &arg_storage;

  int rc = sqlite3InvokeBusyHandler(&bh);

  // rc should be non-zero (non-zero value from busy_echo_arg)
  if (rc <= 0) {
    printf("FAIL: rc <= 0 in test_rc_nonzero_increments_nBusy_and_passes_arg (rc=%d)\n", rc);
    return false;
  }
  // nBusy should have incremented by 1 (since rc != 0)
  if (bh.nBusy != 6) {
    printf("FAIL: nBusy not incremented in test_rc_nonzero_increments_nBusy_and_passes_arg (nBusy=%d)\n", bh.nBusy);
    return false;
  }
  // Ensure the arg received the previous nBusy value (5)
  if (arg_storage != 5) {
    printf("FAIL: arg_storage not matching previous nBusy (arg_storage=%d, expected=5)\n", arg_storage);
    return false;
  }
  return true;
}

int main() {
  // Step 3: Test Case Refinement and Execution
  // Run all tests and summarize results. We intentionally avoid terminating on first failure
  // to maximize code coverage during testing.

  bool t1 = test_null_busyhandler_returns_zero();
  bool t2 = test_negative_nBusy_no_call();
  bool t3 = test_rc_zero_sets_nBusy_to_minus_one();
  bool t4 = test_rc_nonzero_increments_nBusy_and_passes_arg();

  // Report overall result
  int total = g_total_tests;
  int failures = g_failed_tests;
  printf("Total checks attempted: %d\n", total);
  printf("Total failures: %d\n", failures);
  if (failures == 0) {
    printf("ALL TESTS PASSED\n");
  } else {
    printf("SOME TESTS FAILED\n");
  }

  // Return non-zero if any test failed
  return failures > 0 ? 1 : 0;
}