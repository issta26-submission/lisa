/*
  Unit test suite for lsmTreeMakeOld(lsm_db *pDb)

  - No GoogleTest; a lightweight in-process test harness is implemented.
  - Tests rely on the project's existing types defined in lsmInt.h and related headers.
  - Tests cover:
      1) When iOldShmid == 0, fields are migrated/initialized as per function logic.
      2) When iOldShmid != 0, the function is a no-op (no state changes).
  - Static helpers and domain-specific notes are included as comments.

  Important: This test file assumes the standard project headers (e.g., lsmInt.h) provide
  the lsm_db, TreeRoot, TreeHeader, and related structures used by lsmTreeMakeOld.
  It links against lsm_tree.c (or the corresponding compilation unit providing lsmTreeMakeOld).

  To compile:
    g++ -std=c++11 -I<path-to-project-headers> test_lsmTreeMakeOld.cpp lsm_tree.c -ldl
  Adjust paths and library dependencies as required by your build system.
*/

#include <string.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <iostream>


extern "C" {
  // Declaring the function under test.
  void lsmTreeMakeOld(struct lsm_db *pDb);
  // If needed, declare additional entry points (not used here) via externs.
}

// Include project headers to obtain real type definitions.
// This is essential to ensure field access matches the actual lsm_db layout.
// Adjust include paths as per your project setup.

// Simple test harness (non-terminating assertions)
static int g_pass = 0;
static int g_fail = 0;

// NON-terminating assertion macro:
// Logs failure message but continues execution to maximize coverage.
#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "TEST_FAIL: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    ++g_fail; \
  } else { \
    ++g_pass; \
  } \
} while(0)


// Helper: Print a separator for readability
static void printHeader(const char* title) {
  std::cout << "\n=== " << title << " ===" << std::endl;
}


// Candidate Keywords derived from the focal method:
// - iOldShmid, iNextShmid, iOldLog, oldcksum0, oldcksum1, oldroot, root, iTransId, iRoot, nHeight, nByte
// - pDb, pDb->iReader, pDb->pClient->iLogOff
// - The branch when iOldShmid == 0 (update path) vs != 0 (no-op path)


// Test 1: When iOldShmid == 0, lsmTreeMakeOld initializes/updates several fields.
// Verifies: iOldLog calculation, oldcksum preservation, iOldShmid assignment,
// oldroot copy, and root reset to the new small tree state.
static bool test_lsmTreeMakeOld_updatesFields() {
  printHeader("Test 1: lsmTreeMakeOld updates fields when iOldShmid == 0");

  // Prepare a minimal valid lsm_db instance with required fields.
  lsm_db db;
  std::memset(&db, 0, sizeof(db));

  // pClient mock with iLogOff value
  struct ClientStub {
    int iLogOff;
  } clientStub;
  clientStub.iLogOff = 0x01; // choose a value to affect iOldLog calculation

  // Link the mock client into db (assuming field name pClient exists as in focal code)
  // Cast to avoid compiler warnings if actual type differs in test environment.
  db.pClient = (typeof(db.pClient)) &clientStub;

  // iReader must be >= 0 to pass the assert in code
  db.iReader = 0;

  // Prepare iNextShmid and iOldShmid
  db.treehdr.iNextShmid = 42;
  db.treehdr.iOldShmid = 0; // triggering the "make old" branch

  // Prepare log region with region[2].iEnd value
  db.treehdr.log.aRegion[2].iEnd = 0x0A0A0A0A; // arbitrary nonzero

  // Set cksums
  db.treehdr.log.cksum0 = 0x11111111;
  db.treehdr.log.cksum1 = 0x22222222;

  // Set initial root values (so we can verify oldroot copies)
  TreeRoot initialRoot;
  initialRoot.iTransId = 5;
  initialRoot.iRoot = 7;
  initialRoot.nHeight = 2;
  initialRoot.nByte = 8;
  db.treehdr.root = initialRoot;

  // Snapshot of oldroot before call for post-call comparison
  TreeRoot oldroot_before;
  std::memset(&oldroot_before, 0, sizeof(oldroot_before));
  // Copy of root is stored before calling function
  oldroot_before = db.treehdr.root;

  // Call the function under test
  lsmTreeMakeOld(&db);

  // Expectations:
  // 1) iOldShmid should be iNextShmid - 1
  TEST_ASSERT(db.treehdr.iOldShmid == (db.treehdr.iNextShmid - 1),
              "iOldShmid should equal iNextShmid - 1 after update");

  // 2) oldcksum0/1 should match log.cksum0/cksum1
  TEST_ASSERT(db.treehdr.oldcksum0 == db.treehdr.log.cksum0,
              "oldcksum0 should equal log.cksum0");
  TEST_ASSERT(db.treehdr.oldcksum1 == db.treehdr.log.cksum1,
              "oldcksum1 should equal log.cksum1");

  // 3) iOldLog should be computed as (iEnd << 1) | (~iLogOff & 1)
  long long expectedOldLog = (db.treehdr.log.aRegion[2].iEnd << 1) |
                             ((~(clientStub.iLogOff)) & 0x1);
  TEST_ASSERT(db.treehdr.iOldLog == expectedOldLog,
              "iOldLog should be correctly computed from iEnd and iLogOff");

  // 4) oldroot should be a copy of the pre-call root
  // We compare the raw memory to avoid needing an operator== for TreeRoot
  bool rootsMatch = (std::memcmp(&db.treehdr.oldroot, &oldroot_before, sizeof(TreeRoot)) == 0);
  TEST_ASSERT(rootsMatch, "oldroot should be a copy of the pre-call root");

  // 5) root should be reset: iTransId=1, iRoot=0, nHeight=0, nByte=0
  TEST_ASSERT(db.treehdr.root.iTransId == 1 &&
              db.treehdr.root.iRoot == 0 &&
              db.treehdr.root.nHeight == 0 &&
              db.treehdr.root.nByte == 0,
              "root should be reset to initial empty state after update");

  // Return overall status: pass if all TEST_ASSERT calls passed
  // The test framework tracks individual assertions; for simplicity, rely on global counters here.
  return (g_fail == 0);
}


// Test 2: When iOldShmid != 0, lsmTreeMakeOld should perform no changes.
// The function should be a no-op with existing values preserved.
static bool test_lsmTreeMakeOld_noUpdateWhenOldShmidSet() {
  printHeader("Test 2: lsmTreeMakeOld is a no-op when iOldShmid != 0");

  lsm_db db;
  std::memset(&db, 0, sizeof(db));

  struct ClientStub {
    int iLogOff;
  } clientStub;
  clientStub.iLogOff = 0; // arbitrary

  db.pClient = (typeof(db.pClient)) &clientStub;
  db.iReader = 0;

  // Non-zero iOldShmid ensures no-branch execution
  db.treehdr.iNextShmid = 100;
  db.treehdr.iOldShmid = 12345;

  db.treehdr.log.aRegion[2].iEnd = 0x00123456;
  db.treehdr.log.cksum0 = 0xDEADBEEF;
  db.treehdr.log.cksum1 = 0xFEEDBEEF;

  TreeRoot initialRoot;
  initialRoot.iTransId = 9;
  initialRoot.iRoot = 11;
  initialRoot.nHeight = 3;
  initialRoot.nByte = 16;
  db.treehdr.root = initialRoot;

  // Snapshot values to compare after function call
  long long iOldLog_before = db.treehdr.iOldLog;
  long long iOldShmid_before = db.treehdr.iOldShmid;
  long long oldcksum0_before = db.treehdr.oldcksum0;
  long long oldcksum1_before = db.treehdr.oldcksum1;
  TreeRoot root_before = db.treehdr.root;
  TreeRoot oldroot_before;
  std::memset(&oldroot_before, 0, sizeof(oldroot_before));
  oldroot_before = db.treehdr.oldroot;

  lsmTreeMakeOld(&db);

  // Ensure no changes occurred
  TEST_ASSERT(db.treehdr.iOldShmid == iOldShmid_before,
              "iOldShmid should remain unchanged when path not taken");
  TEST_ASSERT(db.treehdr.iOldLog == iOldLog_before,
              "iOldLog should remain unchanged when path not taken");
  TEST_ASSERT(db.treehdr.oldcksum0 == oldcksum0_before,
              "oldcksum0 should remain unchanged when path not taken");
  TEST_ASSERT(db.treehdr.oldcksum1 == oldcksum1_before,
              "oldcksum1 should remain unchanged when path not taken");
  TEST_ASSERT(std::memcmp(&db.treehdr.root, &root_before, sizeof(TreeRoot)) == 0,
              "root should remain unchanged when path not taken");
  TEST_ASSERT(std::memcmp(&db.treehdr.oldroot, &oldroot_before, sizeof(TreeRoot)) == 0,
              "oldroot should remain unchanged when path not taken");

  // Return overall status
  return (g_fail == 0);
}


// Main test harness
int main() {
  // Run tests
  bool ok1 = test_lsmTreeMakeOld_updatesFields();
  bool ok2 = test_lsmTreeMakeOld_noUpdateWhenOldShmidSet();

  // Summary
  std::cout << "\n==== TEST SUMMARY ====" << std::endl;
  std::cout << "Passed assertions: " << g_pass << std::endl;
  std::cout << "Failed assertions: " << g_fail << std::endl;

  // Overall exit status: 0 if all tests succeeded
  if (ok1 && ok2 && g_fail == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << "SOME TESTS FAILED" << std::endl;
    return 1;
  }
}