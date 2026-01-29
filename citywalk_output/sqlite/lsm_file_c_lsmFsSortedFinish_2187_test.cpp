// lsm_fs_sorted_finish_test.cpp
// A self-contained C++11 test harness for the focal C function:
// int lsmFsSortedFinish(FileSystem *pFS, Segment *p);
// The tests exercise the key branches of the function as described in the
// provided source (Step 1) and Step 2/3 requirements.
// Notes:
// - This file provides a lightweight, non-terminating assertion framework.
// - It is designed to be compiled and linked against the project that contains
//   lsmFsSortedFinish and its dependent C code (no Google Test is used).
// - The test scaffolding focuses on exercising true/false predicates and side
//   effects (append list modification, block freeing, and page release) while
//   respecting static dependencies as described in the prompt.
// - The actual project will provide the real definitions for FileSystem, Segment,
//   Page, Db, Worker, and the various helper functions (e.g., fsLastPageOnPagesBlock,
//   fsPageGet, lsmGetU32, lsmBlockRefree, lsmFsPageRelease, fsBlockNext, etc.).
//   This test suite arranges minimal state and small, deterministic scenarios to
//   verify the intended control flow.
// - This file uses a tiny, custom test runner and a few fake/stubbed helpers to
//   drive the code under test. It does not attempt to reimplement the whole
//   database/file-system layer; it assumes the project exposes the focal API.

#include <vector>
#include <sys/types.h>
#include <lsmInt.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cassert>


// Domain: mimic the necessary dependencies' surface.
// NOTE: These declarations must be compatible with the project headers in your
// environment. If your real project uses different layouts, adjust accordingly.

// Forward declarations of the focal types (as used by lsmFsSortedFinish)
typedef unsigned int LsmPgno;
typedef int LSM_OK_t;
#define LSM_OK 0

// Minimal struct shapes to allow calling the focal function in tests.
// Real project headers provide richer definitions; tests should adapt to those.
// We only rely on fields that are touched by lsmFsSortedFinish.

struct Page {
    unsigned char aData[512]; // size chosen to accommodate nPagesize-4 offset
};

struct Segment {
    int iLastPg;
    void *pRedirect;
    // The real structure in project may be richer (e.g., pointers to parent run, etc.)
};

struct Worker {
    LsmPgno aiAppend[8]; // LSM_APPLIST_SZ assumed to be 8
};

struct Db {
    Worker *pWorker;
};

struct FileSystem {
    Db *pDb;
    int pCompress;       // 0 or non-zero
    int nPagesize;        // page data payload size (at least 4)
};

// Declaration of the focal function (to be linked from the project under test)
extern "C" int lsmFsSortedFinish(FileSystem *pFS, Segment *p);

// Lightweight non-terminating test assertion framework
static int g_pass = 0;
static int g_fail = 0;

#define TEST_EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILED: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)

#define TEST_EXPECT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        std::cerr << "TEST FAILED: " << (msg) \
                  << " (expected " << (a) << " == " << (b) \
                  << ", line " << __LINE__ << ")" << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)


// Helpers to simulate environment and to observe side effects.
// In a real project, these would hook into the project's logging or state-tracking
// facilities, or be implemented as part of a test harness with access to the
// real block/append lists.

static LsmPgno g_lastPageReturnedByFsLastPageOnPagesBlock = 0; // 0 means "normal" for tests
static int g_rc_from_fsPageGet = LSM_OK;
static int g_last_block_freed = -1;
static int g_last_page_released = -1;
static int g_append_modified = 0;

// A tiny wrapper to mimic the appended-list behavior in FileSystem.pDb.pWorker.aiAppend.
// If the first slot is zero, writing iLastPg+1 there should occur.
static void reset_append_state(FileSystem *pFS) {
    if(pFS && pFS->pDb && pFS->pDb->pWorker) {
        for(int i=0; i<8; ++i) pFS->pDb->pWorker->aiAppend[i] = 0;
    }
    g_append_modified = 0;
}

static void set_append_slot(FileSystem *pFS, LsmPgno value) {
    if(pFS && pFS->pDb && pFS->pDb->pWorker) {
        for(int i=0; i<8; ++i) {
            if(pFS->pDb->pWorker->aiAppend[i] == 0) {
                pFS->pDb->pWorker->aiAppend[i] = value;
                g_append_modified = 1;
                break;
            }
        }
    }
}

// Mocked behaviors expected by lsmFsSortedFinish
// The real project uses a set of static helpers; here we provide the minimal
// surface to drive the function deterministically in tests.

extern "C" {

// In the actual project, fsLastPageOnPagesBlock would be a static helper.
// We provide a test-scoped override when linking, but if the real binary uses the
// symbol directly we rely on the project's implementation. Here we expose a
// test hook for test scenarios that require precise control:
// If the test wants to force "not last page on block", set g_lastPageReturnedByFsLastPageOnPagesBlock
// to a non-zero value and ensure the test environment aligns with that.
// The function signature must match the project's use.
LsmPgno fsLastPageOnPagesBlock(FileSystem *pFS, LsmPgno iPg) {
    // Test hook: if set to non-zero, simulate "last page on block" != iPg
    if(g_lastPageReturnedByFsLastPageOnPagesBlock != 0) {
        // Return a value different from iPg to trigger append-path
        return g_lastPageReturnedByFsLastPageOnPagesBlock;
    }
    // Default: pretend iPg is exactly the last page on its block
    return iPg;
}

// Minimal fsPageGet to populate a Page with aBlk at aData[nPagesize-4].
int fsPageGet(FileSystem *pFS, int seg, LsmPgno iPg, int noContent, Page **ppPg, int *pnSpace) {
    if(!pFS || !ppPg) return -1;
    static Page s_dummy;
    // For test stability, clear data
    std::memset(s_dummy.aData, 0, sizeof(s_dummy.aData));
    // Place a known iBlk in the last 4 bytes so lsmGetU32 can read it.
    int iBlkVal = 1234; // arbitrary test value
    // Write into the last 4 bytes of the page payload, assuming pFS->nPagesize >=4
    int end = pFS->nPagesize - 4;
    unsigned char *p = s_dummy.aData;
    // Little-endian write
    p[end+0] = (unsigned char)(iBlkVal & 0xFF);
    p[end+1] = (unsigned char)((iBlkVal >> 8) & 0xFF);
    p[end+2] = (unsigned char)((iBlkVal >> 16) & 0xFF);
    p[end+3] = (unsigned char)((iBlkVal >> 24) & 0xFF);
    *ppPg = &s_dummy;
    if(pnSpace) *pnSpace = 0;
    g_rc_from_fsPageGet = LSM_OK;
    // Indicate a successful read
    return LSM_OK;
}

// Small helper to extract a 32-bit little-endian int from a pointer.
// The real code uses lsmGetU32; provide a compatible implementation for tests.
static unsigned int lsmGetU32(const void *p) {
    unsigned int v;
    std::memcpy(&v, p, 4);
    return v;
}

// Simulated freeing/cleanup hooks. The real code would free memory or adjust
// internal structures. Here we simply record that an operation occurred.
void lsmBlockRefree(Db *pDb, int iBlk) {
    (void)pDb;
    g_last_block_freed = iBlk;
}

void lsmFsPageRelease(Page *pLast) {
    (void)pLast;
    g_last_page_released = 1;
}

// A simple BlockNext stub that assigns the next block and returns success.
int fsBlockNext(FileSystem *pFS, Segment *p, int iBlk, int *piNext) {
    (void)p;
    (void)iBlk;
    if(piNext) *piNext = 999; // arbitrary next block
    return LSM_OK;
}

// Page-to-block mapping stub
int fsPageToBlock(FileSystem *pFS, LsmPgno iPg) {
    (void)pFS;
    (void)iPg;
    return (int)(iPg); // deterministic mapping for testing
}

// Small wrapper to simulate a directory of blocks for tests (not used directly)
static int fsMmapPage(FileSystem *pFS, LsmPgno iReal) { (void)pFS; (void)iReal; return 0; }

} // extern "C"

// Test 1: Null or no last page should not alter state and should return OK
static void test_null_or_no_last_page() {
    // Prepare environment
    FileSystem fs;
    Db db;
    Worker worker;
    std::memset(worker.aiAppend, 0, sizeof(worker.aiAppend));
    db.pWorker = &worker;
    fs.pDb = &db;
    fs.pCompress = 0;
    fs.nPagesize = 16; // >= 4 required by lsmFsSortedFinish

    Segment seg;
    seg.iLastPg = 0;       // "no last page"
    seg.pRedirect = nullptr;

    reset_append_state(&fs);

    int rc = lsmFsSortedFinish(&fs, &seg);

    TEST_EXPECT_EQ(rc, LSM_OK, "rc should be LSM_OK for null segment");
    // Append list should remain untouched (all zeros)
    for(int i=0; i<8; ++i) {
        TEST_EXPECT_TRUE(worker.aiAppend[i] == 0, "aiAppend should remain zero for null last page");
    }
    TEST_EXPECT_TRUE(g_append_modified == 0, "append modification should not happen on null last page");
}

// Test 2: Not last page on the block -> first free slot in aiAppend should be filled with iLastPg+1
static void test_not_last_page_on_block() {
    // Prepare environment
    FileSystem fs;
    Db db;
    Worker worker;
    std::memset(worker.aiAppend, 0, sizeof(worker.aiAppend));
    db.pWorker = &worker;
    fs.pDb = &db;
    fs.pCompress = 0;     // not compressed to exercise the append path
    fs.nPagesize = 16;

    Segment seg;
    seg.iLastPg = 5;        // last page of run
    seg.pRedirect = nullptr;

    // Simulate fsLastPageOnPagesBlock to return a value different from iLastPg
    g_lastPageReturnedByFsLastPageOnPagesBlock = 3; // force "not on last block" path

    reset_append_state(&fs);

    int rc = lsmFsSortedFinish(&fs, &seg);

    // Expect rc OK
    TEST_EXPECT_EQ(rc, LSM_OK, "rc should be LSM_OK when not last page on block");

    // The first available entry in aiAppend should be p.iLastPg + 1 = 6
    int expected = seg.iLastPg + 1;
    bool found = false;
    for(int i=0; i<8; ++i) {
        if(worker.aiAppend[i] == (LsmPgno)expected) { found = true; break; }
        if(worker.aiAppend[i] != 0) break; // if a non-zero but not expected value found early, test could fail
    }
    TEST_EXPECT_TRUE(found, "aiAppend should contain iLastPg+1 in first available slot when not on last block");
}

// Test 3: Last page on block and pCompress == 0 with fsPageGet success -> block freed and page released
static void test_last_page_on_block_compress_zero_pageGet_success() {
    // Prepare environment
    FileSystem fs;
    Db db;
    Worker worker;
    std::memset(worker.aiAppend, 0, sizeof(worker.aiAppend));
    db.pWorker = &worker;
    fs.pDb = &db;
    fs.pCompress = 0; // force the "page-based" path
    fs.nPagesize = 16;

    Segment seg;
    seg.iLastPg = 7;
    seg.pRedirect = nullptr;

    // Force "last page on block" condition by making fsLastPageOnPagesBlock return iLastPg
    g_lastPageReturnedByFsLastPageOnPagesBlock = 0; // 0 means use default behavior (return iPg)

    reset_append_state(&fs);
    // Clear internal flags the test will inspect
    g_last_block_freed = -1;
    g_last_page_released = -1;

    int rc = lsmFsSortedFinish(&fs, &seg);

    // Function should process and free the block referenced in pLast's aData[nPagesize-4].
    TEST_EXPECT_EQ(rc, LSM_OK, "rc should be LSM_OK for last page on block with pageGet path");

    // We expect block to be freed (iBlk value was written as 1234 in fsPageGet mock)
    // In our test harness, lsmBlockRefree records in g_last_block_freed
    TEST_EXPECT_TRUE(g_last_block_freed == 1234, "Block of iBlk should be freed (1234) in pageGet path");
    TEST_EXPECT_TRUE(g_last_page_released == 1, "Page should be released after lsmFsPageRelease call");
}

// Test 4: Last page on block with pCompress != 0 -> fsBlockNext path and block freed if OK
static void test_last_page_on_block_compress_nonzero_fsBlockNext_ok() {
    // Prepare environment
    FileSystem fs;
    Db db;
    Worker worker;
    std::memset(worker.aiAppend, 0, sizeof(worker.aiAppend));
    db.pWorker = &worker;
    fs.pDb = &db;
    fs.pCompress = 1; // trigger fsBlockNext path
    fs.nPagesize = 16;

    Segment seg;
    seg.iLastPg = 9;
    seg.pRedirect = nullptr;

    // Simulate last-page-on-block = same as iLastPg so that we go to the else-branch
    g_lastPageReturnedByFsLastPageOnPagesBlock = 0;

    // Clear counters
    g_last_block_freed = -1;

    int rc = lsmFsSortedFinish(&fs, &seg);

    TEST_EXPECT_EQ(rc, LSM_OK, "rc should be LSM_OK for last page on block with compress non-zero (fsBlockNext path)");

    // Expect fsBlockNext to have produced a block number to free (we can test that
    // our fake fsBlockNext returned 999 as iBlk and lsmBlockRefree would update g_last_block_freed)
    // In this minimal harness, we rely on g_last_block_freed being updated to some int
    // We cannot assert a specific value here without more detailed mock integration;
    // but we ensure that the block-free path was exercised in some way.
    TEST_EXPECT_TRUE(g_last_block_freed != -1, "Block should be freed via fsBlockNext path (some iBlk)");
}

// Runner
int main() {
    std::cout << "Running lsmFsSortedFinish unit tests (non-GTest harness)\n";

    test_null_or_no_last_page();
    test_not_last_page_on_block();
    test_last_page_on_block_compress_zero_pageGet_success();
    test_last_page_on_block_compress_nonzero_fsBlockNext_ok();

    std::cout << "Tests completed. Passed: " << g_pass << ", Failed: " << g_fail << std::endl;
    return (g_fail == 0) ? 0 : 1;
}