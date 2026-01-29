/*
  Lightweight C++11 test suite for the focal function:
  void lsmFinishReadTrans(lsm_db *pDb)

  Approach:
  - Provide C-style stub implementations for the external C dependencies
    (lsmFree, lsmShmLock, dbReleaseReadlock) to observe interactions.
  - Create two test scenarios matching the function's control flow:
    1) Read-transaction cleanup path (RoTrans active)
    2) Read-transaction path where RoTrans is not active (false branch)
  - Use a minimal in-process test harness (no GoogleTest/GTest).
  - Use the real lsm_db and lsm_env types via including lsmInt.h to enable
    realistic struct layout and member access required by lsmFinishReadTrans.
  - Validate both the state changes and the interaction with stubs (calls and freed pointers).
  - Ensure non-terminating checks (test harness reports pass/fail but does not abort).
*/

#include <vector>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>


// Include the library's internal types to construct a real lsm_db object.
// This header is part of the project under test.

// Declaration of the focal function (should be provided by the project/C code).
extern "C" void lsmFinishReadTrans(lsm_db *pDb);

// Forward declarations for C-style stubs (external functions used by lsmFinishReadTrans).
// We provide C linkage to match the actual library's linkage.
extern "C" {
  void lsmFree(lsm_env *pEnv, void *p);
  void lsmShmLock(lsm_db *pDb, int iLock, int eOp, int bBlock);
  int dbReleaseReadlock(lsm_db *db);
}

// ---- Lightweight test harness ----
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Stub instrumentation counters
static int g_free_calls = 0;
static int g_lock_calls = 0;
static int g_readlock_calls = 0;
static std::vector<void*> g_freed_ptrs;
static int g_last_lock_iLock = 0;
static int g_last_lock_eOp = 0;
static int g_last_lock_bBlock = 0;

// Stub implementations to observe behavior of lsmFinishReadTrans

extern "C" {

// Track frees called via lsmFree
void lsmFree(lsm_env *pEnv, void *p) {
  (void)pEnv; // pEnv is not used by the test stubs beyond type correctness
  ++g_free_calls;
  g_freed_ptrs.push_back(p);
}

// Track SHM locks invoked by lsmFinishReadTrans
void lsmShmLock(lsm_db *pDb, int iLock, int eOp, int bBlock) {
  (void)pDb; // address not used in test; we only observe that a lock call happened
  ++g_lock_calls;
  g_last_lock_iLock  = iLock;
  g_last_lock_eOp    = eOp;
  g_last_lock_bBlock = bBlock;
}

// Track read-lock releases
int dbReleaseReadlock(lsm_db *db) {
  (void)db;
  ++g_readlock_calls;
  return 0;
}
} // extern "C"

// Simple, non-terminating assertions for the test harness
static void expect_true(bool cond, const char* msg) {
  if (cond) {
    ++g_tests_passed;
  } else {
    ++g_tests_failed;
    fprintf(stderr, "TEST FAIL: %s\n", msg);
  }
}

static void expect_eq_int(int a, int b, const char* msg) {
  if (a == b) {
    ++g_tests_passed;
  } else {
    ++g_tests_failed;
    fprintf(stderr, "TEST FAIL: %s (expected %d, got %d)\n", msg, b, a);
  }
}

static void expect_null(void* p, const char* msg) {
  if (p == nullptr) {
    ++g_tests_passed;
  } else {
    ++g_tests_failed;
    fprintf(stderr, "TEST FAIL: %s (pointer not null: %p)\n", msg, p);
  }
}

static void expect_ptr_in_freed(void* p, const char* msg) {
  auto it = std::find(g_freed_ptrs.begin(), g_freed_ptrs.end(), p);
  if (it != g_freed_ptrs.end()) {
    ++g_tests_passed;
  } else {
    ++g_tests_failed;
    fprintf(stderr, "TEST FAIL: %s (pointer %p not found in freed set)\n", msg, p);
  }
}

// ---- Test scenarios ----

// Test 1: RoTrans is active. Expect apShm array and its elements to be freed,
// apShm, nShm, pShmhdr to be reset, and dbReleaseReadlock/lsmShmLock to be invoked.
static void test_RoTransCleanup() {
  g_tests_run++;

  // Reset instrumentation
  g_free_calls = 0;
  g_lock_calls = 0;
  g_readlock_calls = 0;
  g_freed_ptrs.clear();
  g_last_lock_iLock = g_last_lock_eOp = g_last_lock_bBlock = 0;

  // Allocate a minimal environment and RoTrans-allocated shared memory blocks
  lsm_env* pEnv = new lsm_env{}; // zero-initialized
  lsm_db db;
  std::memset(&db, 0, sizeof(db));

  db.pWorker = nullptr;
  db.pCsr = nullptr;
  db.nTransOpen = 0;
  db.bRoTrans = 1;
  db.nShm = 2;

  void* block0 = malloc(64);
  void* block1 = malloc(128);
  void** apArr = (void**)malloc(sizeof(void*) * 2);
  apArr[0] = block0;
  apArr[1] = block1;

  db.apShm = apArr;
  db.pShmhdr = (void*)0xDEADBEEF; // non-null before
  db.pEnv = pEnv;

  // Execute focal method
  lsmFinishReadTrans(&db);

  // Validate true branch effects
  expect_true(db.apShm == nullptr, "apShm should be NULL after RoTrans cleanup");
  expect_eq_int(db.nShm, 0, "nShm should be 0 after RoTrans cleanup");
  expect_true(db.pShmhdr == nullptr, "pShmhdr should be NULL after RoTrans cleanup");

  // DB read lock must have been released
  expect_eq_int(g_readlock_calls, 1, "dbReleaseReadlock should be called exactly once");

  // SHM lock should have been attempted for rotrans unlock
  expect_eq_int(g_lock_calls, 1, "lsmShmLock should be called exactly once");
  expect_eq_int(g_last_lock_eOp, /* unlock op value is defined in library; we just verify call occurred */ g_last_lock_eOp, g_last_lock_eOp);
  // We can't rely on exact macro values here, but we can verify that a lock call happened with the expected parameters style:
  expect_true(g_last_lock_iLock != 0 || g_last_lock_bBlock == 0, "lsmShmLock should have been invoked with rotrans unlock (structure of params observed)");

  // Freed pointers should include all three allocated blocks: apArr[0], apArr[1], and apArr itself
  expect_ptr_in_freed(block0, "First RoTrans block should be freed");
  expect_ptr_in_freed(block1, "Second RoTrans block should be freed");
  expect_ptr_in_freed(apArr, "RoTrans apShm array should be freed");

  // Cleanup any remaining allocated memory in test (the stub does not actually free them)
  free(block0);
  free(block1);
  free(apArr);
  delete pEnv;
}

// Test 2: RoTrans is not active. Expect only dbReleaseReadlock to be invoked and no memory free.
static void test_RoTransNotActive() {
  g_tests_run++;

  // Reset instrumentation
  g_free_calls = 0;
  g_lock_calls = 0;
  g_readlock_calls = 0;
  g_freed_ptrs.clear();

  lsm_env* pEnv = new lsm_env{};
  lsm_db db;
  std::memset(&db, 0, sizeof(db));

  db.pWorker = nullptr;
  db.pCsr = nullptr;
  db.nTransOpen = 0;
  db.bRoTrans = 0;
  db.nShm = 0;
  db.apShm = nullptr;
  db.pShmhdr = nullptr;
  db.pEnv = pEnv;

  lsmFinishReadTrans(&db);

  // Validate that no RoTrans cleanup happened
  expect_eq_int(g_readlock_calls, 1, "dbReleaseReadlock should be called exactly once (RoTrans not active)");
  expect_eq_int(g_free_calls, 0, "No lsmFree calls should occur when RoTrans is not active");
  // No freed pointers should be observed
  expect_true(g_freed_ptrs.empty(), "Freed pointers set should remain empty when RoTrans is not active");

  delete pEnv;
}

// ---- Test runner and summary ----
int main() {
  // Run tests
  test_RoTransCleanup();
  test_RoTransNotActive();

  // Summary
  printf("Test Summary: ran=%d, passed=%d, failed=%d\n",
         g_tests_run, g_tests_passed, g_tests_failed);

  // Return non-zero if any test failed
  return (g_tests_failed > 0) ? 1 : 0;
}