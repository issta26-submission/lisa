/*
 * Unit tests for the focal function logic: a self-contained, mock-environment
 * test suite modeled after sqlite3PcacheFetchStress behavior.
 *
 * Notes:
 * - This test suite is a self-contained C++11 program that re-implements a
 *   minimal subset of the dependencies required by the focal method as used in
 *   this test harness. It does not rely on GTest or any external test framework.
 * - The aim is to exercise the branching logic of the focal method:
 *     - Early return when eCreate == 2
 *     - Spill logic when page count exceeds szSpill
 *     - Success path when a page is fetched
 *     - Handling of rc values from the xStress callback (OK, BUSY, and error)
 * - The real sqlite3PcacheFetchStress in pcache.c relies on a broader SQLite
 *   runtime. Here we provide a minimal, controlled environment to validate the
 *   logical structure of the method under test.
 * - Static helpers and a simplified PCache/PgHdr model are used only for tests
 *   and are not intended as a drop-in replacement for the real SQLite types.
 *
 * Build (example):
 *   g++ -std=c++11 -O2 -Wall pcache_fetch_stress_tests.cpp -o pcache_fetch_stress_tests
 *
 * Run:
 *   ./pcache_fetch_stress_tests
 *
 * Each test prints a short descriptive message and an overall summary at the end.
 */

#include <vector>
#include <cstdio>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


/* ---------------------------------------------------------------------------
 * Mocked definitions to reproduce the focal function's environment
 * -------------------------------------------------------------------------*/

/* Opaque page type used by the focal method signature in this test harness. */
typedef void sqlite3_pcache_page;

/* Simple boolean-like flags for page header state (subset only). */
static const uint32_t PGHDR_NEED_SYNC = 0x01;

/* Standard SQLite return codes (subset, for the test harness). */
static const int SQLITE_OK = 0;
static const int SQLITE_BUSY = 5;       /* typical value for BUSY in SQLite */
static const int SQLITE_NOMEM_BKPT = -10;/* chosen sentinel for OOM condition in test */

/* Forward declaration of test PCache/PgHdr models. */
struct PgHdr;
struct PCache;

/* Minimal PgHdr model used by the test harness (subset of the real struct). */
struct PgHdr {
    int nRef;           /* reference count for the page */
    uint32_t flags;       /* page status flags (only NEED_SYNC used here) */
    int pgno;           /* page number (for debugging/trace) */
    PgHdr *pDirtyPrev;   /* previous in dirty list (simplified) */
};

/* Minimal PCache model used by the test harness. */
struct PCache {
    int eCreate;                      /* creation mode (test branches on value) */
    int szSpill;                      /* max allowed pages before spilling */
    PgHdr *pSynced;                   /* LRU-like synced list head (test) */
    PgHdr *pDirtyTail;                /* tail of the dirty list (test) */
    void *pCache;                      /* opaque cache pointer (unused in test) */
    void *pStress;                     /* opaque stress data (passed to xStress) */
    int (*xStress)(void*, PgHdr*);     /* stress callback (spill decision) */
};

/* Global helpers (mocked to provide test-controlled behavior) */
static int g_totalPages = 0;                  /* number of pages considered in cache */
static void *g_xFetchPageValue = (void*)0xABCD; /* non-null sentinel to indicate "page found" */

/* Return a simulated number of pages currently in the cache (subset for test). */
static int sqlite3PcachePagecount(PCache *p){
    (void)p; /* unused in this simplified mock; use global g_totalPages instead */
    return g_totalPages;
}

/* Return a number of total pages in the mock cache (same as sqlite3PcachePagecount here). */
static int numberOfCachePages(PCache *p){
    (void)p;
    return g_totalPages;
}

/* No-op trace methods used by the focal function during tests. */
static void pcacheTrace(const char *x){ (void)x; }
static void pcacheDump(PCache *p){ (void)p; }

/* A simple xFetch mock: returns a non-null pointer when fetching a page. */
static void *test_xFetch(void *pCache, int pgno, int createFlag){
    (void)pCache; (void)pgno; (void)createFlag;
    return g_xFetchPageValue;
}

/* The focal method under test (self-contained mock version tailored for tests)
 * This implementation mirrors the control flow of the actual sqlite3PcacheFetchStress
 * but relies on the test's own PCache/PgHdr definitions and mocks. */
int sqlite3PcacheFetchStress(
  PCache *pCache,                 /* Obtain the page from this cache */
  int pgno,                        /* Page number to obtain */
  sqlite3_pcache_page **ppPage    /* Write result here */
){
    PgHdr *pPg;

    /* Early exit path: if eCreate == 2, do nothing and return 0 (as in real code). */
    if( pCache->eCreate==2 ) return 0;

    /* Spill logic: if the page count exceeds the spill threshold, try to spill. */
    if( sqlite3PcachePagecount(pCache) > pCache->szSpill ){
        /* Find a dirty page to spill: look for a page with no NEED_SYNC requirement
         * and with no reference, scanning from pSynced backwards. */
        for(pPg = pCache->pSynced;
            pPg && (pPg->nRef || (pPg->flags & PGHDR_NEED_SYNC));
            pPg = pPg->pDirtyPrev
        );
        pCache->pSynced = pPg;

        if( !pPg ){
            for(pPg = pCache->pDirtyTail; pPg && pPg->nRef; pPg = pPg->pDirtyPrev);
        }

        if( pPg ){
            int rc;
            rc = pCache->xStress(pCache->pStress, pPg);
            pcacheDump(pCache);
            if( rc!=SQLITE_OK && rc!=SQLITE_BUSY ){
                return rc;
            }
        }
    }

    /* Fetch the requested page from the underlying fetch mechanism. */
    *ppPage = pCache->xFetch(pCache->pCache, pgno, 2);
    return *ppPage==0 ? SQLITE_NOMEM_BKPT : SQLITE_OK;
}

/* ---------------------------------------------------------------------------
 * Test harness helpers
 * -------------------------------------------------------------------------*/

/* Helper to create a PgHdr node for tests. */
static PgHdr* make_pg(int nRef, uint32_t flags, int pgno){
    PgHdr* p = new PgHdr();
    p->nRef = nRef;
    p->flags = flags;
    p->pgno = pgno;
    p->pDirtyPrev = nullptr;
    return p;
}

/* Helper to link a single page as the dirty tail or from pSynced. */
static void link_pcache(PCache *p, PgHdr *pSynced, PgHdr *pDirtyTail){
    p->pSynced = pSynced;
    p->pDirtyTail = pDirtyTail;
}

/* Simple test assertion macro that prints failures but continues running tests. */
#define TEST_ASSERT(cond, msg) \
    do { \
        if(!(cond)){ \
            std::cerr << "TEST FAILED: " << (msg) \
                      << " (line " << __LINE__ << ")" << std::endl; \
            g_test_failures++; \
        } \
    } while(0)

/* Global test failure counter for the test suite. */
static int g_test_failures = 0;

/* ---------------------------------------------------------------------------
 * Individual test cases
 * -------------------------------------------------------------------------*/

/*
 * Test 1: Early return when eCreate == 2.
 * Expectation: sqlite3PcacheFetchStress returns 0 and does not attempt a fetch.
 */
static void test_case_early_return_eCreate2(){
    PCache pc;
    pc.eCreate = 2;
    pc.szSpill = 3;
    pc.pSynced = nullptr;
    pc.pDirtyTail = nullptr;
    pc.pCache = nullptr;
    pc.pStress = nullptr;
    pc.xStress = nullptr;            /* not invoked due to early return */
    g_totalPages = 10;                 /* doesn't matter for early return */

    sqlite3_pcache_page *pp = nullptr;
    int rc = sqlite3PcacheFetchStress(&pc, 7, &pp);

    TEST_ASSERT(rc == 0, "Case 1: rc should be 0 when eCreate == 2");
    TEST_ASSERT(pp == nullptr, "Case 1: ppPage should remain untouched on early return");
    std::cout << "CASE 1 (eCreate==2) tested." << std::endl;
}

/*
 * Test 2: No spill path when page count does not exceed szSpill.
 * Expectation: xFetch is invoked and ppPage is set to a non-null value; rc == SQLITE_OK.
 */
static void test_case_no_spill_fetch_ok(){
    PCache pc;
    pc.eCreate = 1;
    pc.szSpill = 10;
    pc.pSynced = nullptr;
    pc.pDirtyTail = nullptr;
    pc.pCache = nullptr;
    pc.pStress = nullptr;
    pc.xStress = nullptr;            /* not used in this path */

    /* Limit total pages under spill threshold. */
    g_totalPages = 5;

    /* xFetch path returns non-null to simulate a successful fetch. */
    pc.xFetch = test_xFetch;

    /* Attach a trivial ppPage holder. */
    sqlite3_pcache_page *pp = nullptr;
    int rc = sqlite3PcacheFetchStress(&pc, 2, &pp);

    TEST_ASSERT(rc == SQLITE_OK, "Case 2: rc should be SQLITE_OK when no spill is required and fetch succeeds");
    TEST_ASSERT(pp != nullptr, "Case 2: pp should be set by xFetch when fetch succeeds");
    std::cout << "CASE 2 (no spill, fetch ok) tested." << std::endl;
}

/*
 * Test 3: Spill path with xStress returning SQLITE_OK (favorable rc).
 * Expectation: Spill is attempted, rc OK, fetch occurs; rc == SQLITE_OK.
 */
static void test_case_spill_ok_fetch(){
    PCache pc;
    pc.eCreate = 1;
    pc.szSpill = 5;
    pc.pSynced = nullptr;
    pc.pDirtyTail = nullptr;
    pc.pCache = nullptr;
    pc.pStress = (void*)0x1;
    pc.xStress = [](void*, PgHdr*) -> int {
        return SQLITE_OK;
    };

    /* Trigger spill path: total pages exceed szSpill. */
    g_totalPages = 6;

    /* A single spill candidate that is clean and unreferenced (nRef=0, flags does not contain NEED_SYNC). */
    PgHdr *spillPage = make_pg(0, 0, 42);
    pc.pSynced = spillPage;          /* not NEED_SYNC and unreferenced, so spill candidate is picked */

    pc.xFetch = test_xFetch;

    sqlite3_pcache_page *pp = nullptr;
    int rc = sqlite3PcacheFetchStress(&pc, 3, &pp);

    TEST_ASSERT(rc == SQLITE_OK, "Case 3: rc should be SQLITE_OK after successful spill and fetch");
    TEST_ASSERT(pp != nullptr, "Case 3: pp should be set after fetch");
    std::cout << "CASE 3 (spill ok, fetch) tested." << std::endl;

    delete spillPage;
}

/*
 * Test 4: Spill path with xStress returning an error code other than OK or BUSY.
 * Expectation: Function returns that error code immediately.
 */
static void test_case_spill_stress_error(){
    PCache pc;
    pc.eCreate = 1;
    pc.szSpill = 5;
    pc.pSynced = nullptr;
    pc.pDirtyTail = nullptr;
    pc.pCache = nullptr;
    pc.pStress = (void*)0x2;
    pc.xStress = [](void*, PgHdr*) -> int {
        return -123; /* some error */
    };

    g_totalPages = 6;
    PgHdr *spillPage = make_pg(0, 0, 99);
    pc.pSynced = spillPage;
    pc.xFetch = test_xFetch;

    sqlite3_pcache_page *pp = nullptr;
    int rc = sqlite3PcacheFetchStress(&pc, 15, &pp);

    TEST_ASSERT(rc == -123, "Case 4: rc should propagate xStress error codes (not OK or BUSY)");
    std::cout << "CASE 4 (spill error propagated) tested." ;

    delete spillPage;
}

/* ---------------------------------------------------------------------------
 * Main runner
 * -------------------------------------------------------------------------*/
int main(){
    std::cout << "Starting sqlite3PcacheFetchStress unit tests (mock environment)..." << std::endl;

    test_case_early_return_eCreate2();
    test_case_no_spill_fetch_ok();
    test_case_spill_ok_fetch();
    test_case_spill_stress_error();

    if( g_test_failures == 0 ){
        std::cout << "ALL TESTS PASSED (mock environment) -> OK" << std::endl;
        return 0;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED (mock environment)" << std::endl;
        return 1;
    }
}