/*
  Unit tests for the focal C function:
    void tdb_lsm_config_work_hook(
      TestDb *pDb, 
      void (*xWork)(lsm_db *, void *), 
      void *pWorkCtx
    );
  Goal: verify true/false branches of the internal predicate using C++11 (no GTest).
  Notes:
  - The test harness relies on the C code in the repository. We include the necessary
    headers via extern "C" to ensure proper linkage with the C symbols.
  - We simulate a valid LsmDb instance by constructing an LsmDb object whose first
    member is TestDb (as in the source: "struct LsmDb { TestDb base; ... }"). The
    helper function tdb_lsm(TestDb*) is expected to return a non-null lsm_db* when
    the object corresponds to a real LsmDb (i.e., pDb->db is non-null).
  - We verify both branches:
    1) When tdb_lsm(pDb) yields non-null, xWork and pWorkCtx are assigned.
    2) When tdb_lsm(pDb) yields null, xWork and pWorkCtx are left untouched.
  - Static helpers from the C code are kept simple; tests do not rely on static state
    beyond what the test constructs.
*/

#include <stdlib.h>
#include <lsmtest.h>
#include <string.h>
#include <lsmtest_tdb.h>
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <lsm.h>
#include <pthread.h>
#include <zlib.h>


// Bring C symbols into the C++ test by using C linkage for the included headers.
extern "C" {
}

/* Candidate Keywords (Step 1)
   - LsmDb: concrete implementation of the LSM DB wrapper around TestDb
   - TestDb: test harness base class (as defined in lsmtest_tdb.h)
   - tdb_lsm: helper that returns an lsm_db* given a TestDb*
   - tdb_lsm_config_work_hook: the method under test
   - xWork: user-supplied work callback (signature matches void (*)(lsm_db *, void*))
   - pWorkCtx: user context passed to the work callback
   - Branch: true when tdb_lsm(pDb) is non-null, assign fields
   - Branch: false when tdb_lsm(pDb) is null, do nothing
*/

static int g_fail = 0; // count failing tests (non-terminating, per domain guidance)

// Dummy work callbacks to test assignment and pointer equality
static void fakeWork(lsm_db *db, void *ctx) {
  // no-op for test purpose
  (void)db; (void)ctx;
}

static void otherWork(lsm_db *db, void *ctx) {
  // alternate function to distinguish from fakeWork
  (void)db; (void)ctx;
}

// Utility macro: non-terminating assertion
#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "[TEST FAIL] " << msg << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    ++g_fail; \
  } else { \
    std::cout << "[TEST PASS] " << msg << std::endl; \
  } \
} while(0)

/*
  Test 1: Branch true
  - Construct an LsmDb object with a non-null db pointer.
  - Call tdb_lsm_config_work_hook with a known fakeWork callback and a context.
  - Expect xWork and pWorkCtx to be updated to the supplied values.
  - This validates the "if (tdb_lsm(pDb))" true branch.
*/
static void test_branch_true() {
  // Prepare a LsmDb instance that should trigger the true branch.
  LsmDb lsm;
  std::memset(&lsm, 0, sizeof(lsm));

  // Simulate a valid LSM DB by giving a non-null db pointer
  lsm.db = reinterpret_cast<lsm_db*>(0x1); // non-null sentinel

  ++g_fail; // optimize: reset via explicit pass/fail counters later; will re-set on success
  // Ensure initial state
  lsm.xWork = nullptr;
  lsm.pWorkCtx = nullptr;

  // Execute the function under test
  tdb_lsm_config_work_hook((TestDb*)&lsm, fakeWork, reinterpret_cast<void*>(0xDEADBEEF));

  // Validate: the function should have assigned the callbacks
  bool ok = (lsm.xWork == fakeWork) && (lsm.pWorkCtx == reinterpret_cast<void*>(0xDEADBEEF));
  // Report result (non-terminating)
  TEST_ASSERT(ok, "Branch true assigns xWork and pWorkCtx correctly");
}

/*
  Test 2: Branch false
  - Construct an LsmDb object with a null db pointer to simulate the false condition.
  - Initialize xWork/pWorkCtx to non-null values to ensure they are not overwritten.
  - Call tdb_lsm_config_work_hook; the fields should remain unchanged.
  - This validates the "else" path (no changes) when tdb_lsm(pDb) is false.
*/
static void test_branch_false() {
  LsmDb lsm;
  std::memset(&lsm, 0, sizeof(lsm));

  // Simulate an invalid/absent LSM DB by giving a null db pointer
  lsm.db = nullptr;

  // Pre-set some non-null values to verify they are not overwritten
  lsm.xWork = otherWork;
  lsm.pWorkCtx = reinterpret_cast<void*>(0xABCD);

  // Execute the function under test
  tdb_lsm_config_work_hook((TestDb*)&lsm, fakeWork, reinterpret_cast<void*>(0x1234));

  // Validate: the fields should remain unchanged
  bool ok = (lsm.xWork == otherWork) && (lsm.pWorkCtx == reinterpret_cast<void*>(0xABCD));
  TEST_ASSERT(ok, "Branch false leaves xWork and pWorkCtx unchanged");
}

/*
  Main: execute tests and report summary.
  - The tests are designed to be executable under C++11 without a testing framework.
  - We rely on the non-terminating TEST_ASSERT to gather results and print them.
*/
int main() {
  std::cout << "Running tdb_lsm_config_work_hook unit tests (C++11, no GTest)" << std::endl;

  // Reset global failure counter
  g_fail = 0;

  test_branch_true();
  test_branch_false();

  if(g_fail == 0) {
    std::cout << "[OVERALL PASS] All tests passed." << std::endl;
  } else {
    std::cout << "[OVERALL FAIL] " << g_fail << " test(s) failed." << std::endl;
  }

  // Return nonzero if any test failed
  return g_fail;
}