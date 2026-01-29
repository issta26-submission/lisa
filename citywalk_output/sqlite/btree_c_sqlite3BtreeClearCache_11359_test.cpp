// C++11 test harness for sqlite3BtreeClearCache
// This harness relies on the project's headers (e.g., btreeInt.h) providing
// the real Btree, BtShared, Pager types and sqlite3BtreeClearCache function.
// It overrides sqlite3PagerClearCache via a C linkage function to observe
// whether the cache clear path is invoked.
// The tests cover true and false branches of the inTransaction check.

#include <btreeInt.h>
#include <iostream>
#include <cstdint>


// If needed, wrap C headers to ensure correct linkage when compiling as C++
extern "C" {
  // Include the actual project header(s) that declare Btree/BtShared/Pager and the
  // function under test. This is the intended integration point for the real code.
  // The exact header path may vary per project layout.
  // For example:
  // #include "btreeInt.h"
  // If your environment requires a specific include order, adjust accordingly.
  // We rely on the real definitions to ensure layout-compatible test objects.

  // Forward declarations for types used by sqlite3BtreeClearCache
  // The following line uses the project-provided definitions from headers.
  // We assume Pager, Btree, BtShared, TRANS_NONE, etc., are defined there.

  // Mock declaration for the function we override to observe behavior.
  // The actual function in the project will be linked against, but our test
  // provides a definition here to intercept calls.
  // We declare with C linkage to match the project's expectations.
  void sqlite3PagerClearCache(Pager *pPager);
  // The focal function is defined in the project's C source (btree.c).
  void sqlite3BtreeClearCache(Btree *p);
}

// If the project headers are not automatically included by the build system,
// you may need to explicitly include them here, e.g.:
// extern "C" {
// #include "btreeInt.h"  // defines Btree, BtShared, Pager, TRANS_NONE, etc.
// }

///////////////////////////
// Global test state
///////////////////////////

static int g_pagerClearCache_calls = 0; // counts how many times sqlite3PagerClearCache is invoked

///////////////////////////
// Mocked dependency: sqlite3PagerClearCache
///////////////////////////

// Override the C function used by sqlite3BtreeClearCache to observe its invocation.
// This mock is intentionally minimal; it does not perform any real pager cache work.
extern "C" void sqlite3PagerClearCache(Pager *pPager) {
  (void)pPager; // suppress unused parameter warning if not used
  ++g_pagerClearCache_calls;
}

///////////////////////////
// Lightweight test harness
///////////////////////////

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion macro: logs failures but continues executing other checks
#define CHECK(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "TEST FAILURE: " << msg << std::endl; \
    ++g_failed_tests; \
  } \
} while(0)

#define TEST_PASSED() do { \
  ++g_total_tests; \
} while(0)

// Test 1: When inTransaction == TRANS_NONE, sqlite3PagerClearCache should be invoked
bool test_ClearCache_WhenInTransactionIsNone() {
  // Reset mock state
  g_pagerClearCache_calls = 0;

  // Build minimal object graph expected by sqlite3BtreeClearCache
  // Btree is defined in the project headers; BtShared is its internal payload.
  // We initialize only the fields that are read by sqlite3BtreeClearCache:
  // pBt->inTransaction and pBt->pPager.

  // Note: The actual types come from the project's headers. Here we instantiate
  // them using their public layout to ensure compatibility with the function under test.

  BtShared btShared;           // internal shared state used by Btree
  btShared.inTransaction = TRANS_NONE; // ensure "true" branch
  btShared.pPager = nullptr;           // pointer value is not used by the mock

  Btree btree;                   // focal object
  btree.pBt = &btShared;         // link to BtShared

  // Invoke the function under test
  sqlite3BtreeClearCache(&btree);

  // Verify: the mocked sqlite3PagerClearCache should have been called exactly once
  bool ok = (g_pagerClearCache_calls == 1);
  return ok;
}

// Test 2: When inTransaction != TRANS_NONE, sqlite3PagerClearCache should NOT be invoked
bool test_ClearCache_WhenInTransactionNotNone() {
  // Reset mock state
  g_pagerClearCache_calls = 0;

  BtShared btShared;
  btShared.inTransaction = 1; // not TRANS_NONE
  btShared.pPager = nullptr;

  Btree btree;
  btree.pBt = &btShared;

  sqlite3BtreeClearCache(&btree);

  // Verify: the mocked sqlite3PagerClearCache should NOT have been called
  bool ok = (g_pagerClearCache_calls == 0);
  return ok;
}

// Entry point for running tests
int main() {
  std::cout << "Running unit tests for sqlite3BtreeClearCache (C function under test) using C++11 harness" << std::endl;

  // Run Test 1
  bool t1 = test_ClearCache_WhenInTransactionIsNone();
  if(t1) {
    std::cout << "PASS: test_ClearCache_WhenInTransactionIsNone" << std::endl;
  } else {
    std::cout << "FAIL: test_ClearCache_WhenInTransactionIsNone" << std::endl;
  }
  g_total_tests++;

  // Run Test 2
  bool t2 = test_ClearCache_WhenInTransactionNotNone();
  if(t2) {
    std::cout << "PASS: test_ClearCache_WhenInTransactionNotNone" << std::endl;
  } else {
    std::cout << "FAIL: test_ClearCache_WhenInTransactionNotNone" << std::endl;
  }
  g_total_tests++;

  // Summary
  std::cout << "Test Summary: " << (g_total_tests - g_failed_tests) << " passed, "
            << g_failed_tests << " failed, out of " << g_total_tests << " tests." << std::endl;

  return g_failed_tests == 0 ? 0 : 1;
}