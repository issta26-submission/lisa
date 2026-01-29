// Lightweight unit tests for lsmFsPagePersist without using any external test framework.
// This test harness embeds the C implementation file to gain access to internal behavior
// and runs a few targeted scenarios to exercise key branches of lsmFsPagePersist.
//
// How to compile (example, adjust paths as needed):
// g++ -std=c++11 -O2 -Wall test_lsm_fs_page_persist.cpp -lm -I.
//
// Note: We include the C source file directly within an extern "C" block to preserve C linkage.

#include <lsm_file.c>
#include <sys/types.h>
#include <lsmInt.h>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cassert>


// Bring C definitions into the test environment
extern "C" {
}

// Include the actual focal C implementation to access lsmFsPagePersist
extern "C" {
}

// Simple non-terminating test assertion macro
#define TEST_OK(cond, msg) \
    do { if (cond) { std::cout << "[OK] " << msg << std::endl; } \
         else { std::cout << "[FAIL] " << msg << std::endl; } \
    } while(0)

// Helper memory management utilities
static void* xmalloc(size_t n) {
    void* p = malloc(n);
    if (!p) {
        std::cerr << "Out of memory" << std::endl;
        exit(1);
    }
    return p;
}

static void xfree(void* p) {
    if (p) free(p);
}

// Test 1: NULL page should be a no-op and return LSM_OK
static void test_null_page() {
    int rc = lsmFsPagePersist(NULL);
    TEST_OK(rc == LSM_OK, "test_null_page: lsmFsPagePersist(NULL) returns LSM_OK");
}

// Test 2: Non-dirty page should be a no-op and return LSM_OK
static void test_non_dirty_page() {
    // Prepare a minimal Page instance. We rely on the public layout of Page from the project headers.
    Page pg;
    memset(&pg, 0, sizeof(pg));
    // Ensure the Page is non-dirty
    pg.flags = 0;
    // Stub required pointers to safe values (not dereferenced due to early return)
    pg.pFS = NULL;
    pg.pSeg = NULL;
    pg.iPg = 0;
    pg.nCompress = 0;
    pg.nData = 0;
    pg.aData = NULL;

    int rc = lsmFsPagePersist(&pg);
    TEST_OK(rc == LSM_OK, "test_non_dirty_page: non-dirty page returns LSM_OK");
}

// Test 3: Dirty page with compression path enabled
// This validates the path through the compression into buffer and hash insertion.
// We construct a minimal but consistent environment so that the code path executes.
static void test_dirty_page_with_compression() {
    // Prepare environment objects
    FileSystem fs;
    memset(&fs, 0, sizeof(fs));

    Segment seg;
    memset(&seg, 0, sizeof(seg));

    // Configure a plausible environment for compression path
    fs.pCompress = (void*)1;          // non-null to enable compression path
    fs.nHash = 8;
    fs.apHash = (Page**)xmalloc(sizeof(Page*) * fs.nHash);
    memset(fs.apHash, 0, sizeof(Page*) * fs.nHash);
    // Simple data buffer for the "output" of compression
    fs.aOBuffer = (u8*)xmalloc(1024);
    fs.nPagesize = 512;
    fs.nWrite = 0;
    // We'll skip actual file I/O by relying on core behavior inside fsAppendData
    // which will operate with the in-memory structures we provide.

    // Prepare a page that is dirty and has the right preconditions for compression
    Page pg;
    memset(&pg, 0, sizeof(pg));
    pg.flags = PAGE_DIRTY;
    pg.pFS = &fs;
    pg.pSeg = &seg;
    pg.iPg = 0;
    pg.nCompress = 0;
    // nData should equal pFS->nPagesize - 4 according to the code path assertion
    pg.nData = fs.nPagesize - 4;
    // Allocate aData buffer and fill with sample data
    pg.aData = (u8*)xmalloc(pg.nData);
    memset(pg.aData, 0xAB, pg.nData);

    // The segment must also be valid (iFirst non-zero for the compression path assertion)
    seg.iFirst = 1;
    seg.nSize = 0;

    int rc = lsmFsPagePersist(&pg);

    // Validate expectations
    bool ok = (rc == LSM_OK);
    ok = ok && (pg.iPg > 0);
    ok = ok && ((pg.pSeg != NULL) && (pg.pSeg == &seg));
    ok = ok && (fs.nWrite > 0);
    ok = ok && ((pg.flags & PAGE_DIRTY) == 0);
    TestCheck:
    if (ok) {
        std::cout << "[OK] test_dirty_page_with_compression: compression path executed as expected" << std::endl;
    } else {
        std::cout << "[FAIL] test_dirty_page_with_compression: compression path did not execute as expected" << std::endl;
    }

    // Cleanup
    if (pg.aData) { xfree(pg.aData); }
    if (fs.aOBuffer) { xfree(fs.aOBuffer); }
    if (fs.apHash) { xfree(fs.apHash); }
}

int main() {
    std::cout << "Starting lsmFsPagePersist unit tests (C++ harness including C implementation)" << std::endl;
    test_null_page();
    test_non_dirty_page();
    test_dirty_page_with_compression();
    std::cout << "Unit tests finished." << std::endl;
    return 0;
}