// pcache_test.cpp
//
// This test suite exercises the focal function:
//   void sqlite3PcacheClearSyncFlags(PCache *pCache)
//
// It verifies:
// - All PgHdr nodes in the dirty list have their PGHDR_NEED_SYNC flag cleared.
// - The PCache's pSynced pointer is updated to the dirty-tail (pDirtyTail).
// - Edge cases: empty dirty list and single-element dirty lists.
// The tests are written without Google Test (no external framework) and use a
// small in-house assertion mechanism that continues execution on failure.
//
// The test relies on internal SQLite data structures (PCache, PgHdr) defined in
// sqliteInt.h. It includes the header with C linkage to ensure compatibility with
// the focal C function implemented in pcache.c.
//
// Step-by-step alignment with the task:
//
// Step 1: Program Understanding
// - Core dependent components (Candidate Keywords):
//   PCache, PgHdr, pDirty, pDirtyNext, pDirtyTail, pSynced, PGHDR_NEED_SYNC, sqlite3PcacheClearSyncFlags
// - Functionality: iterate over pCache->pDirty list, clear PGHDR_NEED_SYNC on each, then
//   set pCache->pSynced = pCache->pDirtyTail.
//
// Step 2: Unit Test Generation
// - Tests cover true/false branches: dirty list non-empty, empty, single-element.
// - Static/internal helpers are not mocked; use real types from sqliteInt.h.
//
// Step 3: Test Case Refinement
// - Domain knowledge: use a minimal harness, non-terminating assertions, and C linkage
//   for the function under test. Access static members via provided types; do not mock.

#include <iostream>
#include <sqliteInt.h>
#include <vector>
#include <string>


// Bring in SQLite internal types with C linkage.
extern "C" {
}

// Ensure the focal function is visible with C linkage.
extern "C" void sqlite3PcacheClearSyncFlags(PCache *pCache);

// Simple in-house assertion framework (non-terminating) for test continuity.
static int g_failures = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " (Line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(false)

#define ASSERT_PTR_NOT_NULL(p, msg) ASSERT_TRUE((p) != nullptr, (msg))

// Candidate Keywords (Step 1 summary embedded as comments in code)
/*
Candidate Keywords:
- PCache: central cache structure
- PgHdr: page header structure in the dirty list
- pDirty: head of the dirty list
- pDirtyNext: link to next dirty page
- pDirtyTail: tail of the dirty list
- pSynced: pointer to the last synced page
- PGHDR_NEED_SYNC: flag bit indicating a page needs syncing
- sqlite3PcacheClearSyncFlags: function under test
*/

static void test_clearSyncFlags_normal_chain(void) {
    // Explanatory comment:
    // Test a normal non-empty dirty list with multiple pages.
    // Some pages have PGHDR_NEED_SYNC; the function should clear that bit on all.
    // After invocation, pSynced should be set to pDirtyTail.

    // Allocate pages and form a chain: p1 -> p2 -> p3
    PgHdr *p1 = new PgHdr();
    PgHdr *p2 = new PgHdr();
    PgHdr *p3 = new PgHdr();

    // Initialize links
    p1->pDirtyNext = p2;
    p2->pDirtyNext = p3;
    p3->pDirtyNext = nullptr;

    // Set initial flags (p1 and p2 need sync)
    p1->flags = PGHDR_NEED_SYNC;
    p2->flags = PGHDR_NEED_SYNC;
    p3->flags = 0; // already clean

    // Build PCache
    PCache cache;
    cache.pDirty = p1;
    cache.pDirtyTail = p3;
    cache.pSynced = nullptr;

    // Call the focal function
    sqlite3PcacheClearSyncFlags(&cache);

    // Validate: all pages cleared of PGHDR_NEED_SYNC
    ASSERT_TRUE((p1->flags & PGHDR_NEED_SYNC) == 0, "p1 PGHDR_NEED_SYNC should be cleared");
    ASSERT_TRUE((p2->flags & PGHDR_NEED_SYNC) == 0, "p2 PGHDR_NEED_SYNC should be cleared");
    ASSERT_TRUE((p3->flags & PGHDR_NEED_SYNC) == 0, "p3 PGHDR_NEED_SYNC should remain cleared");

    // Validate: pSynced updated to tail
    ASSERT_TRUE(cache.pSynced == p3, "cache.pSynced should be set to pDirtyTail (p3)");

    // Cleanup
    delete p1;
    delete p2;
    delete p3;
}

static void test_clearSyncFlags_empty_list(void) {
    // Explanatory comment:
    // Test behavior when the dirty list is empty (NULL).
    // Expect that pSynced becomes pDirtyTail (likely NULL) and no access occurs.

    PCache cache;
    cache.pDirty = nullptr;
    cache.pDirtyTail = nullptr;
    cache.pSynced = (PgHdr*)0xDEADBEEF; // ensure it's changed to NULL afterwards

    sqlite3PcacheClearSyncFlags(&cache);

    // Validate: pSynced updated to NULL (pDirtyTail)
    ASSERT_TRUE(cache.pSynced == nullptr, "cache.pSynced should be NULL when dirty list is empty");
}

static void test_clearSyncFlags_single_element(void) {
    // Explanatory comment:
    // Test with a single-element dirty list. Ensure the single page flag is cleared
    // and pSynced points to that element.

    PgHdr *p = new PgHdr();

    // Chain: p -> NULL
    p->pDirtyNext = nullptr;
    p->flags = PGHDR_NEED_SYNC; // needs sync initially

    PCache cache;
    cache.pDirty = p;
    cache.pDirtyTail = p;
    cache.pSynced = nullptr;

    sqlite3PcacheClearSyncFlags(&cache);

    // Validate: flag cleared
    ASSERT_TRUE((p->flags & PGHDR_NEED_SYNC) == 0, "Single element: PGHDR_NEED_SYNC should be cleared");

    // Validate: pSynced updated to the only element
    ASSERT_TRUE(cache.pSynced == p, "cache.pSynced should be set to the single dirty element");

    delete p;
}

// Main test runner
int main() {
    std::cout << "Running pcache.c focused tests for sqlite3PcacheClearSyncFlags (C linkage)\n";

    test_clearSyncFlags_normal_chain();
    test_clearSyncFlags_empty_list();
    test_clearSyncFlags_single_element();

    if(g_failures == 0) {
        std::cout << "[PASSED] All tests succeeded.\n";
        return 0;
    } else {
        std::cout << "[FAILED] " << g_failures << " test(s) failed.\n";
        return 1;
    }
}