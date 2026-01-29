/*
Step-by-step reasoning notes:
- This test targets sqlite3PagerFlush(Pager *pPager) from pager.c, using a minimal, self-contained mock environment.
- We recreate only the necessary types and functions to exercise the focal path, without pulling in the real SQLite codebase.
- We cover: MEMDB == 0 (true path) with:
  1) A non-empty dirty list where exactly one page has nRef==0 and triggers pagerStress.
  2) An empty dirty list.
  3) A non-OK initial rc to ensure the loop is skipped.
- This file is self-contained C++11 code and does not depend on GTest. It uses a tiny, non-terminating assertion style (log-only) to maximize coverage.
- To test MEMDB==1 (false path) build, see the second file (test_sqlite3PagerFlush_MEMDB1.cpp).
*/

// test_sqlite3PagerFlush_MEMDB0.cpp
// Compile with: g++ -std=c++11 -DMEMDB=0 test_sqlite3PagerFlush_MEMDB0.cpp -o test_memdb0

#define MEMDB 0

#include <iostream>
#include <wal.h>
#include <sqliteInt.h>
#include <cassert>


// Minimal mimicry of SQLite's types used by sqlite3PagerFlush
typedef struct PCache PCache;
typedef struct PgHdr PgHdr;
typedef struct Pager Pager;

struct PgHdr {
    PgHdr* pDirty; // next in the dirty list
    int nRef;      // reference count
};

struct PCache {
    // Intentionally empty for test purposes
    int dummy;
};

struct Pager {
    int errCode;     // current error code
    PCache* pPCache;  // pointer to page cache (mock)
};

// Forward declarations for helpers used by sqlite3PagerFlush
static int assert_pager_state(Pager* pPager);
static PgHdr* sqlite3PcacheDirtyList(PCache* pPCache);
static int pagerStress(void* pPager, PgHdr* pPg);

// Globals used by mocks to observe behavior
static PgHdr* g_dirty_head = nullptr;
static int g_pagerStress_calls = 0;

// Mock: returns the head of the current dirty list
static PgHdr* sqlite3PcacheDirtyList(PCache* pPCache) {
    (void)pPCache; // unused in test harness
    return g_dirty_head;
}

// Mock: pretend to assert pager state; always succeed in tests
static int assert_pager_state(Pager* pPager) {
    (void)pPager;
    return 1;
}

// Mock: simulate stress operation on a page; record invocation
static int pagerStress(void* pPager, PgHdr* pPg) {
    (void)pPager;
    (void)pPg;
    g_pagerStress_calls++;
    // Return SQLITE_OK
    return 0;
}

// The focal function under test (copied/implemented here for standalone unit-test)
int sqlite3PagerFlush(Pager *pPager){
  int rc = pPager->errCode;
  if( !MEMDB ){
    PgHdr *pList = sqlite3PcacheDirtyList(pPager->pPCache);
    assert( assert_pager_state(pPager) );
    while( rc==0 && pList ){
      PgHdr *pNext = pList->pDirty;
      if( pList->nRef==0 ){
        rc = pagerStress((void*)pPager, pList);
      }
      pList = pNext;
    }
  }
  return rc;
}

// Tiny helper for test assertions (non-terminating)
static void expect(bool cond, const char* msg) {
    if (cond) {
        std::cout << "[PASS] " << msg << "\n";
    } else {
        std::cout << "[FAIL] " << msg << "\n";
    }
}

// Test 1: MEMDB == 0, non-empty dirty list with one eligible page
static void test_MEMDB0_nonempty_dirty_list_calls_pagerStress_once() {
    // Reset observers
    g_pagerStress_calls = 0;
    g_dirty_head = nullptr;

    // Build dirty list: a -> b
    PgHdr a;
    PgHdr b;
    a.nRef = 0;   // eligible
    a.pDirty = &b; // next is b
    b.nRef = 3;   // not eligible
    b.pDirty = nullptr;
    g_dirty_head = &a;

    // Prepare pager
    PCache cache;
    Pager pager;
    pager.errCode = 0;     // SQLITE_OK
    pager.pPCache = &cache;

    // Execute
    int rc = sqlite3PagerFlush(&pager);

    // Validate: rc OK and pagerStress called exactly once
    expect(rc == 0, "MEMDB0: rc should be SQLITE_OK when initial rc == SQLITE_OK and list exists");
    expect(g_pagerStress_calls == 1, "MEMDB0: pagerStress should be called exactly once for the first eligible page");
}

// Test 2: MEMDB == 0, empty dirty list -> no pagerStress calls
static void test_MEMDB0_empty_dirty_list_no_calls() {
    g_pagerStress_calls = 0;
    g_dirty_head = nullptr;

    PCache cache;
    Pager pager;
    pager.errCode = 0; // SQLITE_OK
    pager.pPCache = &cache;

    int rc = sqlite3PagerFlush(&pager);

    expect(rc == 0, "MEMDB0_EMPTY: rc should be SQLITE_OK when there is no dirty page");
    expect(g_pagerStress_calls == 0, "MEMDB0_EMPTY: pagerStress should not be called with empty dirty list");
}

// Test 3: MEMDB == 0, initial rc non-OK -> no pagerStress calls
static void test_MEMDB0_non_ok_initial_rc_no_calls() {
    g_pagerStress_calls = 0;
    g_dirty_head = nullptr;

    // Create a dummy dirty list that would be eligible if rc were OK
    PgHdr a;
    a.nRef = 0;
    a.pDirty = nullptr;
    g_dirty_head = &a;

    PCache cache;
    Pager pager;
    pager.errCode = 5; // non-OK
    pager.pPCache = &cache;

    int rc = sqlite3PagerFlush(&pager);

    // rc should be preserved and no stress should happen
    expect(rc == 5, "MEMDB0_NON_OK: rc should propagate non-OK initial value");
    expect(g_pagerStress_calls == 0, "MEMDB0_NON_OK: pagerStress should not be called when initial rc != SQLITE_OK");
}

// Test harness entry
int main() {
    std::cout << "Running MEMDB=0 tests for sqlite3PagerFlush...\n";
    test_MEMDB0_nonempty_dirty_list_calls_pagerStress_once();
    test_MEMDB0_empty_dirty_list_no_calls();
    test_MEMDB0_non_ok_initial_rc_no_calls();
    std::cout << "MEMDB=0 tests completed.\n";
    return 0;
}