/*
  Unit test suite for the focal method:
  sqlite3PcacheTruncate(PCache *pCache, Pgno pgno)

  Test plan (high level):
  - Test 1: When pCache->pCache is NULL, function should do nothing (no crash).
  - Test 2: When there are dirty pages with pgno > pgno, those pages are cleaned
            (sqlite3PcacheMakeClean must be invoked and actually clear PGHDR_DIRTY).
  - Test 3: When pgno == 0 and nRefSum > 0, page 1 is fetched and its buffer zeroed, and
            xTruncate is called with pgno+1 (i.e., 2).
  - Test 4: Static/config hooks are invoked as expected (xFetch and xTruncate interactions).

  Notes:
  - This test suite relies on the project-provided headers (e.g., sqliteInt.h) to
    define PCache, PgHdr, Pgno, PGHDR_DIRTY, sqlite3GlobalConfig, etc.
  - Tests are self-contained and use a lightweight, non-terminating assertion approach.
  - The code is intended to be compiled as C++11, without GoogleTest. A simple main()
    drives the test execution and prints per-test results.
  - The test relies on the presence of the real sqlite3PcacheTruncate and related
    functions from the project. It hooks into sqlite3GlobalConfig.pcache2.xFetch and
    xTruncate for Test 3/4 where needed.
  - Static/global function hooks used in the test are set up in a predictable manner
    and reset between tests when possible.

  Important: The test prints PASS/FAIL messages for each case and exits with a non-zero
  status if any test fails.
*/

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


extern "C" {
  // Import the project's internal definitions.
  // This header provides PCache, PgHdr, Pgno, PGHDR_DIRTY, and the global
  // sqlite3GlobalConfig structure as used by sqlite3PcacheTruncate.
  #include "sqliteInt.h"

  // Declare the focal function (C linkage)
  void sqlite3PcacheTruncate(PCache *pCache, Pgno pgno);
}

// Lightweight testing harness
static int gFailures = 0;

// Helper assertion macro: non-terminating, prints message on failure but continues
#define TEST_ASSERT(cond, msg)                               \
  do {                                                         \
    if(!(cond)) {                                              \
      fprintf(stderr, "FAIL: %s: %s\n", __func__, msg);       \
      ++gFailures;                                             \
    } else {                                                   \
      fprintf(stdout, "PASS: %s: %s\n", __func__, msg);       \
    }                                                          \
  } while(0)

// Utility: print a short summary at the end
static void summarize() {
  if (gFailures == 0) {
    fprintf(stdout, "ALL TESTS PASSED\n");
  } else {
    fprintf(stderr, "%d TEST(S) FAILED\n", gFailures);
  }
}

// Test 1: pCache->pCache == NULL -> no action, no crash
static void test_no_pcache() {
  // Prepare a PCache instance with pCache == NULL
  PCache pc;
  // Zero-initialize to avoid unknown values
  std::memset(&pc, 0, sizeof(pc));
  pc.pCache = NULL;
  pc.nRefSum = 0;
  pc.szPage = 512;
  pc.pDirty = NULL;

  // Call the function under test
  sqlite3PcacheTruncate(&pc, 123); // arbitrary pgno

  // If we reached here, no crash likely occurred. We ensure the state is unchanged.
  TEST_ASSERT(pc.pCache == NULL, "pcache with NULL pCache remains unchanged");
  TEST_ASSERT(pc.pDirty == NULL, "pDirty remains NULL for empty cache");
  TEST_ASSERT(pc.nRefSum == 0, "nRefSum remains 0 when untouched");
}

// Test 2: Dirty list truncation cleans pages with pgno > provided pgno
static void test_dirty_pages_truncate_cleans_greater_pages() {
  // Build a small dirty list: p1 (pgno=6, dirty) -> p2 (pgno=3, dirty) -> NULL
  PgHdr p2;
  PgHdr p1;

  std::memset(&p1, 0, sizeof(p1));
  std::memset(&p2, 0, sizeof(p2));

  p1.pgno = 6;
  p1.flags = PGHDR_DIRTY;
  p1.pDirtyNext = nullptr;

  p2.pgno = 3;
  p2.flags = PGHDR_DIRTY;
  p2.pDirtyNext = &p1;

  // PCache wrapper
  PCache pc;
  std::memset(&pc, 0, sizeof(pc));
  pc.pCache = (void*)0xdeadbeef; // non-null placeholder
  pc.szPage = 1024;
  pc.nRefSum = 0; // not used in this test
  pc.pDirty = &p2;

  // Call the function with pgno = 4. p1 (pgno 6) should be cleaned.
  sqlite3PcacheTruncate(&pc, 4);

  // Verify: p1 should have been cleaned (PGHDR_DIRTY bit cleared)
  bool isCleaned = (p1.flags & PGHDR_DIRTY) == 0;

  TEST_ASSERT(isCleaned, "Dirty page with pgno > 4 should be cleaned (PGHDR_DIRTY cleared)");
}

// Test 3: When pgno == 0 and nRefSum > 0, page 1 is zeroed and xTruncate(2) is called
static sqlite3_pcache_page* g_testPage1 = nullptr;
static unsigned char *g_testBuf1 = nullptr;
static Pgno g_lastTruncatePgno = 0;

static sqlite3_pcache_page* test_xFetch(void* pCache, Pgno pgno, int createFlag) {
  // Return our test page for pgno == 1
  (void)pCache;
  if (pgno == 1) {
    return g_testPage1;
  }
  return nullptr;
}

static void test_xTruncate(void* pCache, Pgno pgno) {
  (void)pCache;
  g_lastTruncatePgno = pgno;
}

static void test_zero_page1_and_truncate() {
  // Prepare cache with nRefSum > 0 to trigger page 1 fetch & zeroing
  PCache pc;
  std::memset(&pc, 0, sizeof(pc));
  pc.pCache = (void*)0xfeedface;
  pc.szPage = 64;
  pc.nRefSum = 1;
  pc.pDirty = nullptr; // not used in this test

  // Allocate a non-zero buffer for page 1 to verify it gets zeroed
  static unsigned char bufSize = 64;
  g_testBuf1 = new unsigned char[pc.szPage];
  for (size_t i = 0; i < pc.szPage; ++i) {
    g_testBuf1[i] = (unsigned char)(0xAA + (i & 0xFF)); // non-zero content
  }

  g_testPage1 = new sqlite3_pcache_page();
  g_testPage1->pBuf = g_testBuf1;

  // Hook into sqlite3GlobalConfig.pcached2.xFetch and xTruncate
  // Note: Access to sqlite3GlobalConfig is through the header. We assume the
  // members exist as in sqlite3.h/sqliteInt.h and are assignable.
  sqlite3GlobalConfig.pcache2.xFetch = test_xFetch;
  sqlite3GlobalConfig.pcache2.xTruncate = test_xTruncate;

  // Reset last truncate
  g_lastTruncatePgno = 0;

  // Run test: pgno = 0 should fetch page 1, zero its buffer, and then truncate to 2
  sqlite3PcacheTruncate(&pc, 0);

  // Validate: page 1 buffer is zeroed
  bool bufferZeroed = true;
  for (size_t i = 0; i < pc.szPage; ++i) {
    if (g_testBuf1[i] != 0) { bufferZeroed = false; break; }
  }

  TEST_ASSERT(bufferZeroed, "Buffer for Page 1 should be zeroed when pgno==0 and nRefSum>0");
  // Validate: xTruncate was called with pgno+1 = 2
  TEST_ASSERT(g_lastTruncatePgno == 2, "xTruncate should be called with (pgno+1) when pgno==0");
  
  // Cleanup
  delete[] g_testBuf1;
  delete g_testPage1;
  g_testBuf1 = nullptr;
  g_testPage1 = nullptr;
  // Restore handler pointers to null to avoid side effects for subsequent tests
  // (Assuming test environment will reconfigure as needed)
  sqlite3GlobalConfig.pcache2.xFetch = nullptr;
  sqlite3GlobalConfig.pcache2.xTruncate = nullptr;
}

// Entry point
int main(void) {
  fprintf(stdout, "Starting sqlite3PcacheTruncate unit tests (C++ harness)\n");

  test_no_pcache();
  test_dirty_pages_truncate_cleans_greater_pages();
  test_zero_page1_and_truncate();

  summarize();
  return (gFailures != 0) ? 1 : 0;
}