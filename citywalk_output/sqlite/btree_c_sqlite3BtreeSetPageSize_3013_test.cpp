// Unit test harness for sqlite3BtreeSetPageSize
// This test suite targets the focal method sqlite3BtreeSetPageSize located in btree.c.
// It is designed to be compiled in a C++11 environment without GTest, using a minimal
// in-file test framework (simple assertions and a main() entry point).
// NOTE:
// - The test creates lightweight stand-ins for Btree and BtShared structures with
//   fields accessed by sqlite3BtreeSetPageSize. This mirrors the members used in the
//   function under test.
// - This test relies on the real sqlite3BtreeSetPageSize implementation being linked in.
// - If your build environment exposes different internal layouts, adapt the BtShared/Btree
//   struct definitions accordingly. The tests assume the same field names/types as used in
//   the focal code snippet.
// - The test uses a small set of scenarios designed to exercise true/false branches and
//   edge cases described in the Step-by-step instructions.

#include <btreeInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Lightweight stand-ins for the external types used by sqlite3BtreeSetPageSize.
// In a real test environment these should match the definitions in the project (btreeInt.h, etc.),
// and the function sqlite3BtreeSetPageSize should be linked from btree.c.
struct BtShared {
    uint32_t nReserveWanted;  // corresponds to nReserveWanted in the real code
    uint32_t pageSize;          // pBt->pageSize
    uint16_t usableSize;        // pBt->usableSize
    uint32_t btsFlags;           // pBt->btsFlags
    void *pPager;                // pBt->pPager
    int pCursor;                 // pBt->pCursor
};

struct Btree {
    BtShared *pBt;
};

// Bit flag mirrors (must match the real code's values)
#define BTS_PAGESIZE_FIXED 0x01

// Return codes (mirrors from SQLite)
#define SQLITE_OK 0
#define SQLITE_READONLY 1

// External function declarations expected to be provided by the project linkage.
// We declare them here to simulate the environment the focal method expects.
// In the actual test harness, these symbols come from the real btree.c and related files.
extern "C" int sqlite3BtreeSetPageSize(Btree *p, int pageSize, int nReserve, int iFix);
extern "C" void sqlite3BtreeEnter(Btree *p);
extern "C" void sqlite3BtreeLeave(Btree *p);
extern "C" void freeTempSpace(BtShared *pBt);
extern "C" int sqlite3PagerSetPagesize(void *pPager, unsigned int *pPageSize, int nReserve);

// Mock implementations for the environment (only used if the real ones are unavailable).
// In a real test run, these would be resolved to the actual implementations.
// For portability in this standalone harness, provide no-ops or deterministic behavior.
extern "C" void sqlite3BtreeEnter(Btree *p) { (void)p; /* no-op for test harness */ }
extern "C" void sqlite3BtreeLeave(Btree *p) { (void)p; /* no-op for test harness */ }
extern "C" void freeTempSpace(BtShared *pBt) { (void)pBt; /* no-op for test harness */ }
extern "C" int sqlite3PagerSetPagesize(void *pPager, unsigned int *pPageSize, int nReserve) {
    // In a real environment, this would adjust the pager/page size accounting for reserve.
    // For testing, accept the call and pretend success.
    (void)pPager;
    (void)nReserve;
    // We won't mutate *pPageSize here; sqlite3BtreeSetPageSize already updates BtShared.pageSize.
    return SQLITE_OK;
}

// Simple test framework
static int g_testCount = 0;
static int g_failCount = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "Test failed: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
            ++g_failCount; \
        } else { \
            std::cout << "Test passed: " << (msg) << std::endl; \
        } \
        ++g_testCount; \
    } while(0)

static void printSummary() {
    std::cout << "\nTest Summary: " << g_testCount << " tests, "
              << g_failCount << " failures.\n";
}

// Helper to set up a Btree+BtShared pair for tests
static void initBtShared(BtShared &bt, uint32_t pageSize, uint16_t usableSize, uint32_t flags = 0, void *pager = (void*)0x1) {
    bt.nReserveWanted = 0;
    bt.pageSize = pageSize;
    bt.usableSize = usableSize;
    bt.btsFlags = flags;
    bt.pPager = pager;
    bt.pCursor = 0;
}

// Test 1: Verify that when BTS_PAGESIZE_FIXED is set, the function returns SQLITE_READONLY
// and does not modify the page size or usable size.
static bool test_readonly_when_pagesize_fixed() {
    BtShared bt;
    Btree t;
    initBtShared(bt, 1024, 800, BTS_PAGESIZE_FIXED); // fixed flag set
    t.pBt = &bt;

    int rc = sqlite3BtreeSetPageSize(&t, 4096, 0, 0);

    // Expect read-only and no crash
    TEST_ASSERT(rc == SQLITE_READONLY, "READONLY when BTS_PAGESIZE_FIXED is set");
    // Page size should remain unchanged when read-only
    TEST_ASSERT(bt.pageSize == 1024, "Page size unchanged on READONLY path");
    return true;
}

// Test 2: Validate upgrade logic for 512-byte pages when nReserve > 32 and iFix is false.
// Specifically, 512 -> 1024 upgrade occurs if nReserve > 32 and initial pageSize == 512.
static bool test_upgrade_512_to_1024_with_large_nReserve() {
    BtShared bt;
    Btree t;
    // Start with 512-byte page, sufficiently large usable size so that x <= 255
    // We want x = pageSize - usableSize <= 255. Choose usableSize = 500 => x = 12.
    initBtShared(bt, 512, 500, 0);
    // nReserve set to 40 (>32) to trigger upgrade to 1024
    t.pBt = &bt;

    int rc = sqlite3BtreeSetPageSize(&t, 512, 40, 0);

    // After call: pageSize should upgrade to 1024, usableSize recomputed with nReserve
    TEST_ASSERT(rc == SQLITE_OK, "RC OK for valid upgrade path");
    TEST_ASSERT(bt.pageSize == 1024, "512-path upgrades to 1024 when nReserve>32");
    TEST_ASSERT(bt.nReserveWanted == 40, "nReserveWanted preserved after upgrade");
    TEST_ASSERT(bt.usableSize == 1024 - 40, "usableSize recomputed after upgrade");
    return true;
}

// Test 3: Verify nReserve adjustment to x when nReserve < x, where x = pageSize - usableSize.
// This checks the clamped behavior of reserve space calculation.
static bool test_nReserve_adjusts_to_x() {
    BtShared bt;
    Btree t;
    // Choose values so that x <= 255 and nReserve < x
    // pageSize = 1024, usableSize = 800 => x = 224
    initBtShared(bt, 1024, 800, 0);
    t.pBt = &bt;

    int rc = sqlite3BtreeSetPageSize(&t, 1024, 0, 0);

    TEST_ASSERT(rc == SQLITE_OK, "RC OK for nReserve adjustment case");
    TEST_ASSERT(bt.nReserveWanted == 224, "nReserveAdjustedToX (224) when initial 0");
    TEST_ASSERT(bt.pageSize == 1024, "PageSize unchanged when valid and not upgraded");
    TEST_ASSERT(bt.usableSize == 1024 - 224, "usableSize recomputed after adjustment");
    return true;
}

// Test 4: Ensure iFix flag sets BTS_PAGESIZE_FIXED after successful operation.
// This validates the true branch where iFix triggers a fixed-page-size mode.
static bool test_iFix_sets_pagesize_fixed() {
    BtShared bt;
    Btree t;
    // Start with 1024-byte pages, usableSize 900 -> x = 124
    initBtShared(bt, 1024, 900, 0);
    t.pBt = &bt;

    int rc = sqlite3BtreeSetPageSize(&t, 1024, 50, 1); // iFix = 1, nReserve > x, but path should set fixed flag anyway
    // If the function updates btsFlags to include BTS_PAGESIZE_FIXED when iFix is true:
    TEST_ASSERT((bt.btsFlags & BTS_PAGESIZE_FIXED) != 0, "iFix path sets BTS_PAGESIZE_FIXED");
    TEST_ASSERT(rc == SQLITE_OK, "RC OK for iFix path");
    return true;
}

// Main entry: run all tests
int main() {
    std::cout << "Starting sqlite3BtreeSetPageSize unit tests (C++11 harness)...\n";

    test_readonly_when_pagesize_fixed();
    test_upgrade_512_to_1024_with_large_nReserve();
    test_nReserve_adjusts_to_x();
    test_iFix_sets_pagesize_fixed();

    printSummary();
    if (g_failCount > 0) {
        std::cerr << "Some tests failed. See details above.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}

// Explanatory notes for maintainers:
// - The tests above focus on key decision points inside sqlite3BtreeSetPageSize:
//   1) Early return with SQLITE_READONLY when BTS_PAGESIZE_FIXED is set.
//   2) The 512-byte page path: potential upgrade to 1024 bytes when nReserve > 32
//      and 512-byte pages are requested, including the internal potential adjustment
//      of nReserve if necessary.
//   3) The clamping behavior of nReserve to x = pageSize - usableSize when nReserve < x.
//   4) The iFix branch that marks the page size as fixed (BTS_PAGESIZE_FIXED) if iFix != 0.
// - The BtShared and Btree structs used in tests are simplified adapters to exercise the
//   focal logic. If your build uses a different internal layout, adjust field names/types
//   accordingly while preserving the same test scenarios and expectations.
// - The tests use a lightweight, non-terminating assertion style (no throwing) and print
//   clear success/failure messages to stdout. They can be invoked directly from main
//   without any test runner.