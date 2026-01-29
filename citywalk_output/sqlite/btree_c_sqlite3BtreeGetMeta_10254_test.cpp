// High-quality C++11 unit tests for sqlite3BtreeGetMeta
// This test harness provides minimal stubs and a controlled environment
// to exercise the focal method's behavior without relying on Google Test.
// It uses a small, non-terminating assertion style and a simple main driver.

#include <btreeInt.h>
#include <iostream>
#include <cstring>
#include <cstdint>


// -----------------------------------------------------------------------------
// Domain headers and types (assumed to exist in the project under test).
// We rely on the project's internal header to define Btree, BtShared, Pager,
// and related structures and constants used by sqlite3BtreeGetMeta.
// If your build exposes different headers, replace the include with the correct path.
// -----------------------------------------------------------------------------


// Convenience typedefs matching SQLite's common usage
typedef unsigned int u32;
typedef unsigned char u8;
typedef long long i64;

// -----------------------------------------------------------------------------
// Minimal stubs to control the environment for sqlite3BtreeGetMeta
// These stubs mimic just enough behavior to drive the focal logic.
// They are designed to be linked with the existing sqlite3BtreeGetMeta in btree.c.
// -----------------------------------------------------------------------------

// A tiny, global pager data-version provider used by sqlite3PagerDataVersion(...)
static u32 g_pagerDataVersion = 0;

// Forward declarations of functions implemented by the real library.
// We provide lightweight, non-terminating test implementations here.
extern "C" void sqlite3BtreeEnter(Btree *p);
extern "C" void sqlite3BtreeLeave(Btree *p);

// Query the shared-cache lock. We simulate success (SQLITE_OK) for deterministic tests.
extern "C" int querySharedCacheTableLock(Btree *p, Pgno iTab, u8 eLock);

// Pager data version accessor used by sqlite3BtreeGetMeta when idx == BTREE_DATA_VERSION
extern "C" u32 sqlite3PagerDataVersion(Pager *pPager);

// Macro-like constants from the real project (values chosen to align with asserts).
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef READ_LOCK
#define READ_LOCK 0
#endif
#ifndef SCHEMA_ROOT
#define SCHEMA_ROOT 0
#endif

// In the actual project, BTREE_DATA_VERSION and BTREE_LARGEST_ROOT_PAGE are defined.
// We declare them here to align with the test expectations.
#ifndef BTREE_DATA_VERSION
#define BTREE_DATA_VERSION 0
#endif
#ifndef BTREE_LARGEST_ROOT_PAGE
#define BTREE_LARGEST_ROOT_PAGE 15
#endif
#ifndef TRANS_NONE
#define TRANS_NONE 0
#endif

// Implementations of the stubs

void sqlite3BtreeEnter(Btree * /*p*/) {
  // No-op for tests; the real function ensures transaction state in the library.
}

void sqlite3BtreeLeave(Btree * /*p*/) {
  // No-op for tests
}

int querySharedCacheTableLock(Btree * /*p*/, Pgno /*iTab*/, u8 /*eLock*/) {
  // Always succeed for test scenarios
  return SQLITE_OK;
}

u32 sqlite3PagerDataVersion(Pager * /*pPager*/) {
  // Return the test-controlled data version
  return g_pagerDataVersion;
}

// -----------------------------------------------------------------------------
// Test helpers and test environment setup
// -----------------------------------------------------------------------------

// Helper: Build a minimal Btree environment suitable for sqlite3BtreeGetMeta
struct TestEnv {
  Btree *p;          // Focal object
  BtShared *pBt;       // Shared state for the Btree
  Pager *pPager;       // Pager backing the database file
  Page1 *pPage1;       // Root page (pPage1) containing aData
  TestEnv() : p(nullptr), pBt(nullptr), pPager(nullptr), pPage1(nullptr) {}
  ~TestEnv() {
    delete pPage1;
    delete pPager;
    delete pBt;
    delete p;
  }
  void init() {
    // Allocate and wire up minimal objects
    p = new Btree();
    pBt = new BtShared();
    pPager = new Pager();
    pPage1 = new Page1();

    // Wire fields (structure layout matches library expectations)
    // We assume public accessibility for testing; actual project headers expose these.
    p->pBt = pBt;
    p->inTrans = TRANS_NONE + 1;  // Ensure in-trans state passes the assertion

    // BtShared contents
    pBt->pPager = pPager;
    pBt->pPage1 = pPage1;
    pBt->btsFlags = 0;

    // iBDataVersion for the focal path
    p->iBDataVersion = 0;

    // Prepare aData for direct memory access scenarios
    std::memset(pPage1->aData, 0, sizeof(pPage1->aData));

    // Initialize defaults for the pager data version
    g_pagerDataVersion = 0;
  }
};

// Non-terminating assertion helper
#define MY_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "ASSERTION FAILED: " msg << std::endl; \
    return false; \
  } \
} while(0)

// -----------------------------------------------------------------------------
// Tests
// -----------------------------------------------------------------------------

// Test 1: When idx == BTREE_DATA_VERSION, pMeta should be pagerDataVersion + iBDataVersion
bool test_meta_version_path() {
  TestEnv env;
  env.init();
  // Configure environment for test
  g_pagerDataVersion = 7;     // Simulated pager data version
  env.p->iBDataVersion = 5;    // B-tree data version component

  u32 meta = 0;
  // Call the focal function
  sqlite3BtreeGetMeta(env.p, BTREE_DATA_VERSION, &meta);

  // Expected: 7 + 5 = 12
  if(meta != 12) {
    std::cerr << "test_meta_version_path: expected 12, got " << meta << std::endl;
    return false;
  }
  return true;
}

// Test 2: When idx is a non-data-version index, read a 4-byte little-endian value
bool test_meta_data_page_path() {
  TestEnv env;
  env.init();
  // Use a deterministic data payload in aData
  // Place bytes at offset 36 + idx*4 where idx = 3
  const int idx = 3;
  unsigned char valBytes[4] = { 0x11, 0x22, 0x33, 0x44 }; // little-endian 0x44332211
  // Ensure the offset is within aData bounds
  int offset = 36 + idx * 4;
  // Copy bytes into the data page
  std::memcpy(&env.pBt->pPage1->aData[offset], valBytes, 4);

  u32 meta = 0;
  sqlite3BtreeGetMeta(env.p, idx, &meta);

  // Expected: 0x44332211
  const u32 expected = 0x44332211;
  if(meta != expected) {
    std::cerr << "test_meta_data_page_path: expected " << std::hex << expected
              << ", got " << meta << std::dec << std::endl;
    return false;
  }
  return true;
}

// -----------------------------------------------------------------------------
// Main driver
// -----------------------------------------------------------------------------

int main() {
  bool all_pass = true;

  std::cout << "Running sqlite3BtreeGetMeta test suite (no GTest)..." << std::endl;

  if(!test_meta_version_path()) {
    std::cout << "FAILED: test_meta_version_path" << std::endl;
    all_pass = false;
  } else {
    std::cout << "PASSED: test_meta_version_path" << std::endl;
  }

  if(!test_meta_data_page_path()) {
    std::cout << "FAILED: test_meta_data_page_path" << std::endl;
    all_pass = false;
  } else {
    std::cout << "PASSED: test_meta_data_page_path" << std::endl;
  }

  if(all_pass) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << "SOME TESTS FAILED" << std::endl;
    return 1;
  }
}

// Notes:
// - This test suite purposefully uses a minimal, self-contained environment to drive
//   the focal function sqlite3BtreeGetMeta. It relies on the project's Btree/BtShared
//   structures and on stubs for external dependencies (sqlite3BtreeEnter, etc.).
// - The tests cover the primary branches: meta from pager data version and meta from aData.
// - The #ifdef SQLITE_OMIT_AUTOVACUUM branch cannot be reliably tested without rebuilding
//   the focal file with that macro defined/undefined; this harness focuses on the functional
//   logic that is always compiled.
// - The test harness uses non-terminating assertions (via MY_ASSERT-like macro) to maximize
//   test execution across all scenarios without aborting the test run.