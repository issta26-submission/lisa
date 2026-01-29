// Test suite for sqlite3BtreeIncrVacuum (Step 2 & Step 3 alignment)
// Language: C++11 (no GoogleTest). We rely on the project's internal SQLite btree.c implementation.
// This test focuses on two branches that are reasonably controllable from the test harness:
//
// 1) When autoVacuum is disabled (pBt->autoVacuum == 0): expect SQLITE_DONE.
// 2) When autoVacuum is enabled but the database cannot be incrementally vacuumed due to
//    simple observed conditions (e.g., large nFree causing corruption path): expect SQLITE_CORRUPT_BKPT.
//
// Note: We are operating with the project’s internal structures (Btree, BtShared, MemPage, etc.)
// and rely on their public-like interfaces as provided by btreeInt.h. Static helpers in btree.c
// are not accessible for mocking from this external test file; we rely on the actual
// implementation behavior for deterministic outcomes in the crafted scenarios.

#include <cstdio>
#include <btreeInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


extern "C" {
  // Include internal SQLite B-Tree declarations (types: Btree, BtShared, MemPage, etc.)
  // and the function under test. This header is part of the project and provides the necessary
  // type definitions and prototypes.
  #include "btreeInt.h"     // Assumed to exist in the project with Btree, BtShared, MemPage, etc.
  // Prototypes for the function under test (non-static in the TU, as provided)
  int sqlite3BtreeIncrVacuum(Btree *p);
  // If the header doesn't declare this symbol, the linker will resolve from the compilation unit.
}

// Lightweight utilities for test tracing
#define TEST_OK 0
#define TEST_FAIL 1

static int run_test_incrVacuum_autovacuum_disabled();
static int run_test_incrVacuum_corrupt_bkpt_case();

// Note about test memory safety:
// We intentionally construct minimal, controlled environments for Btree and BtShared.
// We rely on the project's internal layout for MemPage and Page1 data to influence the
// behavior of sqlite3BtreeIncrVacuum in a deterministic manner.
// We cast and allocate memory conservatively to avoid interference with real allocator logic.

int main() {
  int overall_status = TEST_OK;

  std::cout << "Starting sqlite3BtreeIncrVacuum test suite (C++11 harness)" << std::endl;

  int status1 = run_test_incrVacuum_autovacuum_disabled();
  if (status1 != TEST_OK) {
    std::cerr << "Test 1 (autovacuum disabled) failed." << std::endl;
    overall_status = TEST_FAIL;
  } else {
    std::cout << "Test 1 (autovacuum disabled) passed." << std::endl;
  }

  int status2 = run_test_incrVacuum_corrupt_bkpt_case();
  if (status2 != TEST_OK) {
    std::cerr << "Test 2 (corrupt bkpt path) failed." << std::endl;
    overall_status = TEST_FAIL;
  } else {
    std::cout << "Test 2 (corrupt bkpt path) passed." << std::endl;
  }

  std::cout << "Test suite finished." << std::endl;
  return overall_status;
}

// Helper: allocate a MemPage-like structure and attach to BtShared as pPage1.
// Returns pointer to MemPage (casted) for use by BtShared.
static MemPage* allocate_fake_page1(uint8_t zero_out = 0) {
  // We rely on MemPage having aData as a member that starts at the beginning of the struct.
  // This matches typical internal SQLite MemPage definitions where aData is the first field.
  // We allocate a sufficiently large page object to cover aData[0..63].
  struct MemPageFake {
    uint8_t aData[256]; // generous size to cover offset 36 accesses
    void* pDbPage;
  };

  MemPageFake* p = (MemPageFake*)calloc(1, sizeof(MemPageFake));
  if (!p) return nullptr;

  if (zero_out) {
    std::memset(p->aData, 0, sizeof(p->aData));
  }

  return (MemPage*)p; // safe cast for internal compatibility with MemPage*
}

// Test 1: When autoVacuum is disabled, sqlite3BtreeIncrVacuum should return SQLITE_DONE
// and perform no further actions that would modify the page-1 data.
static int run_test_incrVacuum_autovacuum_disabled() {
  // Clear state
  std::cout << "[Test 1] autovacuum disabled: expect SQLITE_DONE" << std::endl;

  // Allocate Btree and BtShared
  Btree* p = (Btree*)calloc(1, sizeof(Btree));
  BtShared* pBt = (BtShared*)calloc(1, sizeof(BtShared));
  if (!p || !pBt) {
    std::cerr << "Allocation failed for Btree/BtShared." << std::endl;
    return TEST_FAIL;
  }

  // Wire up minimal relationships required by the function under test
  p->pBt = pBt;
  p->inTrans = TRANS_WRITE;  // Must satisfy the assert in sqlite3BtreeIncrVacuum
  pBt->inTransaction = TRANS_WRITE; // Must satisfy the assert in sqlite3BtreeIncrVacuum
  pBt->autoVacuum = 0; // Disable autoVacuum to trigger the first branch

  // Setup a fake Page1 with aData array (offset 36 will be accessed)
  MemPage* page1 = allocate_fake_page1(1);
  if (!page1) {
    std::cerr << "Failed to allocate fake MemPage for pPage1." << std::endl;
    free(p);
    free(pBt);
    return TEST_FAIL;
  }
  // Attach to BtShared (assuming pPage1 member exists and is of type MemPage*)
  pBt->pPage1 = page1;

  // Ensure pPage1->aData[36..39] are zero to yield nFree == 0
  // Use 4-byte little-endian zero
  uint32_t zero = 0;
  memcpy(&page1->aData[36], &zero, sizeof(zero));
  // nOrig will be derived from btreePagecount(pBt) in the implementation.
  // With autoVacuum == 0, the function should short-circuit and return SQLITE_DONE.

  int rc = sqlite3BtreeIncrVacuum(p);

  // Cleanup
  free(page1);
  free(p);
  free(pBt);

  if (rc != SQLITE_DONE) {
    std::cerr << "Expected SQLITE_DONE, got: " << rc << std::endl;
    return TEST_FAIL;
  }

  return TEST_OK;
}

// Test 2: When autoVacuum is enabled and a simple corruption condition is hit
// (nFree >= nOrig via crafted page data and nPage), expect SQLITE_CORRUPT_BKPT.
// We carefully craft the environment so that the branch triggers corruption without entering
// the following rc == SQLITE_OK path.
static int run_test_incrVacuum_corrupt_bkpt_case() {
  std::cout << "[Test 2] autovacuum enabled with corrupt bkpt condition: expect SQLITE_CORRUPT_BKPT" << std::endl;

  // Allocate Btree and BtShared
  Btree* p = (Btree*)calloc(1, sizeof(Btree));
  BtShared* pBt = (BtShared*)calloc(1, sizeof(BtShared));
  if (!p || !pBt) {
    std::cerr << "Allocation failed for Btree/BtShared." << std::endl;
    return TEST_FAIL;
  }

  p->pBt = pBt;
  p->inTrans = TRANS_WRITE;      // Must satisfy the assert in sqlite3BtreeIncrVacuum
  pBt->inTransaction = TRANS_WRITE; // Must satisfy the assert in sqlite3BtreeIncrVacuum
  pBt->autoVacuum = 1; // Enable autoVacuum to enter the vacuum branch

  // Craft nOrig (via btreePagecount) and nFree to trigger corruption
  // We'll assume nOrig is small (1) due to nPage initialization; set nPage = 1
  pBt->nPage = 1;

  // Attach a fake Page1 with aData[36] = 2 so that nFree = 2
  MemPage* page1 = allocate_fake_page1(0);
  if (!page1) {
    std::cerr << "Failed to allocate fake MemPage for pPage1." << std::endl;
    free(p);
    free(pBt);
    return TEST_FAIL;
  }

  pBt->pPage1 = page1;
  uint32_t two = 2;
  memcpy(&page1->aData[36], &two, sizeof(two));

  int rc = sqlite3BtreeIncrVacuum(p);

  // Cleanup
  free(page1);
  free(p);
  free(pBt);

  if (rc != SQLITE_CORRUPT_BKPT) {
    std::cerr << "Expected SQLITE_CORRUPT_BKPT, got: " << rc << std::endl;
    return TEST_FAIL;
  }

  return TEST_OK;
}