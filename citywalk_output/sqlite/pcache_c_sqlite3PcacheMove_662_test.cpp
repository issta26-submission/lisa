// Test suite for sqlite3PcacheMove (C-style function) using C++11 (no GTest).
// The test harness provides minimal shims for the necessary SQLite-like
// structures and behavior to exercise the focal method under controlled scenarios.
//
// Notes:
// - This test duplicates a minimal subset of the SQLite cache structures.
// - We implement the focal function locally in this translation unit to allow
//   precise control over dependencies and static/dynamic behavior.
// - No terminating tests: assertions inside sqlite3PcacheMove are honored,
//   but test logic ensures they are exercised on valid paths. We cannot
//   trigger ASSERT failures safely here without aborting tests; instead we
//   cover the true-branch semantics and side-effects.
// - Each test includes explanatory comments describing what branch/behavior is covered.

#include <cstdio>
#include <cstdarg>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain-specific constants and types (subset sufficient for tests)
using Pgno = unsigned int;

static const int PGHDR_DIRTY = 0x01;
static const int PGHDR_NEED_SYNC = 0x02;
static const unsigned PCACHE_DIRTYLIST_FRONT = 0;

// Forward declarations to mirror focal dependencies
struct PCache;
struct PgHdr;
struct sqlite3_pcache_page;

// Lightweight sqlite3GlobalConfig replacement (pcache2.xFetch, xRekey)
struct PCache2Driver {
  sqlite3_pcache_page* (*xFetch)(void*, Pgno, int);
  void (*xRekey)(void*, void*, Pgno, Pgno);
};

struct {
  PCache2Driver pcache2;
} sqlite3GlobalConfig;

// Minimal Page Cache Page wrapper
struct sqlite3_pcache_page {
  void* pExtra;
};

// Page header structure used by sqlite3PcacheMove
struct PgHdr {
  PCache* pCache;
  int nRef;
  Pgno pgno;
  int flags;
  void* pPage;
};

// Minimal PCache structure
struct PCache {
  void* pCache;
  int nRefSum;
};

// Helper counters and state used by test harness
static int g_dropCount = 0;
static int g_rekeyOld = -1;
static int g_rekeyNew = -1;
static int g_moveTestsRun = 0;
static int g_dirtyListCallCount = 0;

// Test artifacts for simulating other page fetch results
static sqlite3_pcache_page g_pOtherPage; // pOther (returned by xFetch)
static PgHdr g_pXPage;                   // pXPage where pOther->pExtra points
static sqlite3_pcache_page* g_fakeFetchResult = nullptr; // what xFetch returns

static PCache g_cache; // actual PCache instance used by Move
static void* g_dummyCachePtr = (void*)0xCAFEBABE; // dummy cache pointer

// Helper: all-or-nothing sanity check for a page (we keep it true in tests)
static bool sqlite3PcachePageSanity(PgHdr* /*p*/) {
  return true;
}

// Helper: trace (no-op, but keeps parity with original function)
static void pcacheTrace(const char* /*fmt*/, ...) {
  // Intentionally no-op for tests
}

// Helper: dummy dirty-list management
static void pcacheManageDirtyList(PgHdr* /*pPage*/, unsigned char /*addRemove*/) {
  ++g_dirtyListCallCount;
}

// Test hook: simulate dropping a page (increments drop counter)
extern "C" void sqlite3PcacheDrop(PgHdr* /*p*/) {
  ++g_dropCount;
}

// Test hook: simulate relaxation of a page reference (increase ref count)
static void sqlite3PcacheRef(PgHdr* /*p*/) {
  // Not used in tests directly, but provided for completeness
}

// Test hook: simulate making a page dirty
static void sqlite3PcacheMakeDirty(PgHdr* /*p*/) {}

// Test hook: simulate making a page clean
static void sqlite3PcacheMakeClean(PgHdr* /*p*/) {}

// Test hook: simple xFetch implementation controlled by global state
static sqlite3_pcache_page* test_xFetch(void* pCache, Pgno pgno, int /*flags*/) {
  (void)pCache; (void)pgno;
  // Return whatever is configured by test scenario
  return g_fakeFetchResult;
}

// Test hook: xRekey implementation to capture parameters and simulate behavior
static void test_xRekey(void* pCache, void* pPage, Pgno oldPgno, Pgno newPgno) {
  (void)pCache; (void)pPage;
  g_rekeyOld = (int)oldPgno;
  g_rekeyNew = (int)newPgno;
}

// Pipeline: ensure GlobalConfig is wired to test hooks
static void setupGlobalsForTests() {
  sqlite3GlobalConfig.pcache2.xFetch = test_xFetch;
  sqlite3GlobalConfig.pcache2.xRekey = test_xRekey;
}

// The focal method under test copied here with the same logic
// to enable isolated unit testing in this environment.
void sqlite3PcacheMove(PgHdr *p, Pgno newPgno){
  PCache *pCache = p->pCache;
  sqlite3_pcache_page *pOther;
  assert( p->nRef>0 );
  assert( newPgno>0 );
  assert( sqlite3PcachePageSanity(p) );
  pcacheTrace(("%p.MOVE %d -> %d\n",pCache,p->pgno,newPgno));
  pOther = sqlite3GlobalConfig.pcache2.xFetch(pCache->pCache, newPgno, 0);
  if( pOther ){
    PgHdr *pXPage = (PgHdr*)pOther->pExtra;
    assert( pXPage->nRef==0 );
    pXPage->nRef++;
    pCache->nRefSum++;
    sqlite3PcacheDrop(pXPage);
  }
  sqlite3GlobalConfig.pcache2.xRekey(pCache->pCache, p->pPage, p->pgno,newPgno);
  p->pgno = newPgno;
  if( (p->flags&PGHDR_DIRTY) && (p->flags&PGHDR_NEED_SYNC) ){
    pcacheManageDirtyList(p, PCACHE_DIRTYLIST_FRONT);
    assert( sqlite3PcachePageSanity(p) );
  }
}

// Simple test framework helpers
static int g_totalTests = 0;
static int g_failedTests = 0;

#define ASSERT_TRUE(cond, msg) \
  do { \
    ++g_totalTests; \
    if(!(cond)) { \
      ++g_failedTests; \
      std::cerr << "Assertion failed: " << (msg) << " (in " << __func__ << ")" << std::endl; \
    } \
  } while(0)

#define ASSERT_EQ(a,b, msg) \
  do { \
    ++g_totalTests; \
    if(!((a) == (b))) { \
      ++g_failedTests; \
      std::cerr << "Assertion failed: " << (msg) << " | " << (a) << " != " << (b) \
                << " (in " << __func__ << ")" << std::endl; \
    } \
  } while(0)

// Test 1: Move when a matching pOther exists (fetch returns an existing page at newPgno)
// Expected effects:
// - pXPage.nRef becomes 1
// - pCache.nRefSum increments by 1
// - sqlite3PcacheDrop(pXPage) is invoked
// - p->pgno updated to newPgno
// - xRekey called with (oldPgno, newPgno)
// - No dirty-list management since we don't set dirty flags in this test
static void test_Move_With_existing_pOther() {
  g_moveTestsRun++;
  // Reset state
  g_dropCount = 0;
  g_rekeyOld = -1;
  g_rekeyNew = -1;
  g_dirtyListCallCount = 0;
  g_fakeFetchResult = nullptr;
  g_pXPage.nRef = 0;
  g_pXPage.pCache = &g_cache;
  g_pXPage.pPage = (void*)0x200; // arbitrary
  g_pXPage.pgno = 99;
  // Prepare pOther that xFetch will return, with pExtra pointing to g_pXPage
  g_pOtherPage.pExtra = &g_pXPage;
  g_fakeFetchResult = &g_pOtherPage;

  // Prepare cache and page to move
  g_cache.pCache = g_dummyCachePtr;
  g_cache.nRefSum = 0;

  PgHdr p;
  p.pCache = &g_cache;
  p.nRef = 1;
  p.pgno = 2;
  p.pPage = (void*)0x100;
  p.flags = 0; // not dirty

  setupGlobalsForTests();

  // Execute
  sqlite3PcacheMove(&p, 5);

  // Validate post-conditions
  ASSERT_EQ(p.pgno, 5u, "p.pgno should be updated to newPgno");
  ASSERT_EQ(g_pXPage.nRef, 1, "pXPage.nRef should be incremented to 1");
  ASSERT_EQ(g_cache.nRefSum, 1, "pCache.nRefSum should be incremented by 1");
  ASSERT_EQ(g_dropCount, 1, "sqlite3PcacheDrop should be called once for pXPage");
  ASSERT_EQ(g_rekeyOld, 2, "xRekey should be called with oldPgno (2)");
  ASSERT_EQ(g_rekeyNew, 5, "xRekey should be called with newPgno (5)");
  // No dirty-list changes expected
  ASSERT_TRUE(g_dirtyListCallCount == 0, "pcacheManageDirtyList should not be called in non-dirty path");
}

// Test 2: Move when no pOther exists (fetch returns nullptr)
// Expected effects:
// - xRekey is still called
// - p->pgno updated to newPgno
// - No changes to pXPage or drop count
static void test_Move_Without_existing_pOther() {
  g_moveTestsRun++;
  // Reset state
  g_dropCount = 0;
  g_rekeyOld = -1;
  g_rekeyNew = -1;
  g_dirtyListCallCount = 0;
  g_fakeFetchResult = nullptr;

  // Prepare cache and page to move
  g_cache.pCache = g_dummyCachePtr;
  g_cache.nRefSum = 0;

  PgHdr p;
  p.pCache = &g_cache;
  p.nRef = 1;
  p.pgno = 2;
  p.pPage = (void*)0x101;
  p.flags = 0; // not dirty

  setupGlobalsForTests();

  // Execute
  sqlite3PcacheMove(&p, 3);

  // Validate post-conditions
  ASSERT_EQ(p.pgno, 3u, "p.pgno should be updated to newPgno (3)");
  // Since there was no pOther, no drop should occur
  ASSERT_EQ(g_dropCount, 0, "No drop should occur when pOther is NULL");
  // Rekey should still have been called with old=2, new=3
  ASSERT_EQ(g_rekeyOld, 2, "xRekey oldPgno should be 2");
  ASSERT_EQ(g_rekeyNew, 3, "xRekey newPgno should be 3");
  // No dirty-list changes expected
  ASSERT_TRUE(g_dirtyListCallCount == 0, "pcacheManageDirtyList should not be called in non-dirty path");
}

// Test 3: Move when the page is dirty and needs sync
// Expected behavior:
// - After move, dirty-management path is exercised
// - pcacheManageDirtyList should be invoked
// - Sanity assertion after dirty handling should pass (we keep it true)
static void test_Move_WithDirtyFlags() {
  g_moveTestsRun++;
  // Reset state
  g_dropCount = 0;
  g_rekeyOld = -1;
  g_rekeyNew = -1;
  g_dirtyListCallCount = 0;
  g_fakeFetchResult = nullptr;
  g_pXPage.nRef = 0;
  g_pXPage.pCache = &g_cache;
  g_pXPage.pPage = (void*)0x202;
  g_pXPage.pgno = 7;
  g_pXPage.nRef = 0;

  // Setup fetch result to simulate an existing pOther
  g_pOtherPage.pExtra = &g_pXPage;
  g_fakeFetchResult = &g_pOtherPage;

  // Prepare page with dirty flags
  PgHdr p;
  p.pCache = &g_cache;
  p.nRef = 1;
  p.pgno = 4;
  p.pPage = (void*)0x303;
  p.flags = PGHDR_DIRTY | PGHDR_NEED_SYNC;

  setupGlobalsForTests();

  // Execute
  sqlite3PcacheMove(&p, 6);

  // Validate post-conditions
  // Dirty-list path should have been invoked once
  ASSERT_EQ(g_dirtyListCallCount, 1, "pcacheManageDirtyList should be called exactly once for dirty path");
  // Ensure move still occurred
  ASSERT_EQ(p.pgno, 6u, "p.pgno should be updated to newPgno (6)");
  // Rekey should be called
  ASSERT_EQ(g_rekeyOld, 4, "xRekey oldPgno should be 4");
  ASSERT_EQ(g_rekeyNew, 6, "xRekey newPgno should be 6");
  // Drop path occurs for pXPage as before
  ASSERT_EQ(g_dropCount, 1, "sqlite3PcacheDrop should be invoked for pXPage when pOther exists");
}

// Entry point for tests
int main() {
  std::cout << "Running sqlite3PcacheMove unit tests (no GTest, plain C++11)\n";

  test_Move_With_existing_pOther();
  test_Move_Without_existing_pOther();
  test_Move_WithDirtyFlags();

  std::cout << "Tests run: " << g_totalTests << ", Failures: " << g_failedTests << "\n";

  // Return non-zero if any test failed
  return g_failedTests ? 1 : 0;
}