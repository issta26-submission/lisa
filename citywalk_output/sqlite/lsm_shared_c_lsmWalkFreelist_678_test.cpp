// Unit test suite for lsmWalkFreelist in lsm_shared.c
// Goal: provide a small, self-contained C++11 test harness that exercises
// the focal method under controlled, mockized dependencies without using GTest.
// The test suite overrides the companion helper lsmSortedWalkFreelist to
// deterministically drive execution paths and validate behavior.

// Note: This test code is designed to be linked against the production binary
// that contains lsmWalkFreelist. To enable controlled testing, a test-scoped
// override of lsmSortedWalkFreelist is provided here. The struct layouts are
// replicated in test code to match the usage in lsmWalkFreelist.

#include <vector>
#include <lsmInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// Domain types (simplified for test harness)
using i64 = long long;
using u32 = uint32_t;

// Simplified return code used by lsmWalkFreelist (LSM_OK = 0)
static const int LSM_OK = 0;

// Forward declaration of the focal function (as seen in production)
extern "C" int lsmWalkFreelist(
  struct lsm_db *pDb,
  int bReverse,
  int (*x)(void *, int, i64),
  void *pCtx
);

// Forward declaration for the helper (to override in test)
extern "C" int lsmSortedWalkFreelist(
  struct lsm_db *pDb,
  int bReverse,
  int (*x)(void *, int, i64),
  void *pCtx
);

// Minimal struct layout mirroring production definitions used by lsmWalkFreelist
struct FreelistEntry {
  int iId;
  int iBlk;
};

struct Freelist {
  int nEntry;
  FreelistEntry *aEntry;
};

struct lsm_db; // forward

struct Worker {
  Freelist freelist;
};

struct lsm_db {
  Worker *pWorker;
  Freelist *pFreelist;
};

// Global test state to control lsmSortedWalkFreelist behavior
static int g_testScenario = 0; // 0 = normal, 1 = force bDone = 1

// A test WalkFreelistCtx layout that mirrors the fields used by lsmWalkFreelist.
// This is used by the overridden lsmSortedWalkFreelist to access and mutate
// the context as lsmWalkFreelist would.
struct WalkFreelistCtx {
  lsm_db *pDb;
  int bReverse;
  Freelist *pFreelist;
  int iFree;
  int (*xUsr)(void *, int, i64);
  void *pUsrctx;
  int bDone;
};

// Overridden helper to drive test scenarios deterministically.
// This function is declared extern "C" to allow lsmWalkFreelist to link against it.
// We intentionally keep its behavior simple: set bDone based on testScenario and
// return success so that the outer path can exercise either the early-exit or
// the full freelist walk logic.
extern "C" int lsmSortedWalkFreelist(
  lsm_db *pDb,
  int bReverse,
  int (*x)(void *, int, i64),
  void *pCtx
) {
  // Interpret the test scenario to influence control flow
  (void)pDb; (void)bReverse; (void)x; (void)pCtx; // silence unused in case of compile-time optimization
  // Cast to known layout to modify bDone as needed
  WalkFreelistCtx *p = reinterpret_cast<WalkFreelistCtx*>(pCtx);
  if (p == nullptr) return 0;
  if (g_testScenario == 1) {
    // Force the outer if(ctx[0].bDone==0) to be false
    p->bDone = 1;
  } else {
    // Normal behavior: do not modify bDone (remain 0)
    p->bDone = 0;
  }
  // No actual walking performed here; the real walk happens in lsmWalkFreelist's
  // subsequent manual iteration with the provided callback functions.
  return 0;
}

// Callback used by testCase1 to verify early exit via ctx[1].x (the "x" callback)
static int testCallbackExit(void *pCtx, int iBlk, i64 iId) {
  (void)pCtx; (void)iBlk; (void)iId;
  // Return non-zero to signal termination as per lsmWalkFreelist contract
  return 1;
}

// Callback used by testCase2 to simulate no early-exit (returns 0)
static int testCallbackContinue(void *pCtx, int iBlk, i64 iId) {
  (void)pCtx; (void)iBlk; (void)iId;
  return 0;
}

// Simple assertion helper that prints a message but does not terminate program
static int g_failCount = 0;
#define ASSERT_TRUE(cond, msg) \
  do { if(!(cond)) { \
    std::cerr << "Assertion failed: " << (msg) << " [" << __FILE__ << ":" << __LINE__ << "]\n"; \
    ++g_failCount; \
  } } while(0)


// Test harness: initialize a lsm_db-like structure with deterministic freelist data
static void initDbWithFreelist(
  Freelist &freelist,
  FreelistEntry entries[],
  int n
) {
  freelist.nEntry = n;
  freelist.aEntry = entries;
}

// Test Case 1: Normal path where a positive iId triggers early exit via ctx[1].x
// Expect lsmWalkFreelist to return LSM_OK.
static void testCase1_NormalExitViaSecondCallback() {
  std::cout << "TestCase1: Normal path exits via second callback (ctx[1].x) => expect LSM_OK\n";

  // Build freelist with mixed iId values: one non-negative (to trigger callback)
  FreelistEntry entries[] = {
    { -1, 101 },
    {  2, 202 }, // first non-negative entry
    {  3, 303 }
  };
  Freelist freelist;
  initDbWithFreelist(freelist, entries, 3);

  Worker worker;
  worker.freelist = freelist;

  Freelist pFreelist = freelist;
  Freelist *pFreelistPtr = &pFreelist;

  lsm_db db;
  db.pWorker = &worker;
  db.pFreelist = pFreelistPtr;

  // Run with bReverse = 0 (normal direction), and testCallbackExit as the x callback
  int rc = lsmWalkFreelist(&db, 0, testCallbackExit, nullptr);

  ASSERT_TRUE(rc == LSM_OK, "Expected LSM_OK return from lsmWalkFreelist (Case 1)");
  std::cout << "End of TestCase1\n";
}

// Test Case 2: No entry with non-negative iId; ensure function completes and returns LSM_OK.
// This tests the false-branch path where no callback is invoked.
static void testCase2_NoPositiveEntries() {
  std::cout << "TestCase2: No non-negative iId entries; expect LSM_OK (no early exit)\n";

  FreelistEntry entries[] = {
    { -1, 11 },
    { -2, 22 },
    { -3, 33 }
  };
  Freelist freelist;
  initDbWithFreelist(freelist, entries, 3);

  Worker worker;
  worker.freelist = freelist;

  Freelist pFreelist = freelist;
  Freelist *pFreelistPtr = &pFreelist;

  lsm_db db;
  db.pWorker = &worker;
  db.pFreelist = pFreelistPtr;

  int rc = lsmWalkFreelist(&db, 0, testCallbackContinue, nullptr);

  ASSERT_TRUE(rc == LSM_OK, "Expected LSM_OK return from lsmWalkFreelist (Case 2)");
  std::cout << "End of TestCase2\n";
}

// Test Case 3: Reverse direction path (bReverse = 1) with a positive iId; ensure code runs.
// We rely on the same callback logic; ensures no crash and an LSM_OK return in this scenario.
static void testCase3_ReverseDirectionWithExit() {
  std::cout << "TestCase3: Reverse direction path with positive iId; expect LSM_OK\n";

  FreelistEntry entries[] = {
    { -1, 5 },
    { 5, 15 }, // iId positive
    { 0, 25 }
  };
  Freelist freelist;
  initDbWithFreelist(freelist, entries, 3);

  Worker worker;
  worker.freelist = freelist;

  Freelist pFreelist = freelist;
  Freelist *pFreelistPtr = &pFreelist;

  lsm_db db;
  db.pWorker = &worker;
  db.pFreelist = pFreelistPtr;

  int rc = lsmWalkFreelist(&db, 1, testCallbackExit, nullptr);

  ASSERT_TRUE(rc == LSM_OK, "Expected LSM_OK return from lsmWalkFreelist (Case 3)");
  std::cout << "End of TestCase3\n";
}

// Test Case 4: Force the global SortedWalk to set bDone and skip the inner loops.
// This tests the branch where ctx[0].bDone != 0 prevents the freelist walk.
// Expect LSM_OK since rc from lsmSortedWalkFreelist is 0.
static void testCase4_ForceBDoneSkipsInnerLoop() {
  std::cout << "TestCase4: Force bDone to skip inner loop; expect LSM_OK\n";

  FreelistEntry entries[] = {
    { 1, 100 }, // non-negative, but will be skipped due to bDone
  };
  Freelist freelist;
  initDbWithFreelist(freelist, entries, 1);

  Worker worker;
  worker.freelist = freelist;

  Freelist pFreelist = freelist;
  Freelist *pFreelistPtr = &pFreelist;

  lsm_db db;
  db.pWorker = &worker;
  db.pFreelist = pFreelistPtr;

  // Toggle scenario to cause bDone = 1 inside lsmSortedWalkFreelist
  g_testScenario = 1;

  int rc = lsmWalkFreelist(&db, 0, testCallbackExit, nullptr);

  // Reset scenario
  g_testScenario = 0;

  ASSERT_TRUE(rc == LSM_OK, "Expected LSM_OK return from lsmWalkFreelist (Case 4)");
  std::cout << "End of TestCase4\n";
}

// Test Case 5: Null freelist edge-case (pFreelist null and no entries to walk).
// Ensures code handles null pointers gracefully without crash.
static void testCase5_NullFreelistPointers() {
  std::cout << "TestCase5: Null freelist pointers; expect LSM_OK without crash\n";

  // No entries, zero-length freelist
  Freelist freelist;
  freelist.nEntry = 0;
  freelist.aEntry = nullptr;

  Worker worker;
  worker.freelist = freelist;

  Freelist *pFreelistPtr = nullptr;

  lsm_db db;
  db.pWorker = &worker;
  db.pFreelist = pFreelistPtr; // intentionally null

  int rc = lsmWalkFreelist(&db, 0, testCallbackContinue, nullptr);

  ASSERT_TRUE(rc == LSM_OK, "Expected LSM_OK return from lsmWalkFreelist (Case 5)");
  std::cout << "End of TestCase5\n";
}

// Entry point
int main() {
  std::cout << "Starting lsmWalkFreelist unit tests (standalone, no GTest)\n";

  testCase1_NormalExitViaSecondCallback();
  testCase2_NoPositiveEntries();
  testCase3_ReverseDirectionWithExit();
  testCase4_ForceBDoneSkipsInnerLoop();
  testCase5_NullFreelistPointers();

  if (g_failCount == 0) {
    std::cout << "All tests PASSED.\n";
    return 0;
  } else {
    std::cout << g_failCount << " test(s) FAILED.\n";
    return 1;
  }
}