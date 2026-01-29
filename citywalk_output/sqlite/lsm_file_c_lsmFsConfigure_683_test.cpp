// C++11 unit tests for the focal method lsmFsConfigure (located in lsm_file.c)
// Note: No Google Test is used. A lightweight test harness with non-terminating assertions
// is implemented to maximize code coverage while keeping tests executable standalone.
// The tests rely on the library's C interfaces defined in lsmInt.h (included with extern "C").

/*
Domain considerations followed:
- Access to static/internal file-scope helpers is avoided.
- Test cases exercise true/false branches of the public method's stateful predicates.
- Static file-scope helpers remain untested directly (as per constraints).
- Only standard library plus provided C interfaces are used.
- Non-terminating assertions are implemented via a small EXPECT-like macro.
- Tests are invoked from main() as GTest is not used.
*/

#include <sys/types.h>
#include <lsmInt.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>


// Bring C headers into the C++ test environment
extern "C" {
// We assume lsmInt.h sufficiently declares the required types used by lsmFsConfigure.
}

// Lightweight test harness to mimic EXPECT_* behavior without dragging in GTest.
// Provides non-terminating assertion that logs failures but keeps running tests.
static int g_failures = 0;

#define EXPECT_TRUE(cond)                                                        \
  do {                                                                           \
    if(!(cond)) {                                                                \
      std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__  \
                << std::endl;                                                    \
      ++g_failures;                                                             \
    }                                                                            \
  } while(0)

#define EXPECT_EQ(a, b)                                                         \
  do {                                                                           \
    if((a) != (b)) {                                                             \
      std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) \
                << ") at " __FILE__ ":" << __LINE__ << std::endl;              \
      ++g_failures;                                                             \
    }                                                                            \
  } while(0)

#define TEST_MSG(msg)                                                            \
  do { std::cout << "[TEST] " << msg << std::endl; } while(0)

////////////////////////////////////////////////////////////////////////////////
// Helper: allocate a simple Page object compatible with lsmFsConfigure expectations
////////////////////////////////////////////////////////////////////////////////
static Page* create_test_page_with_free_flag(lsm_env *pEnv) {
    Page *pPg = (Page*)malloc(sizeof(Page));
    if(!pPg) return NULL;
    // Minimal initialization to satisfy lsmFsConfigure's expectations:
    pPg->pLruNext = NULL;
    pPg->pFreeNext = NULL;
    pPg->aData = malloc(128);
    pPg->flags = PAGE_FREE; // mask that satisfies the assert( pPg->flags & PAGE_FREE )
    // If lsmFree uses pEnv for bookkeeping, passing a valid allocation is fine.
    return pPg;
}

////////////////////////////////////////////////////////////////////////////////
// Test 1: lsmFsConfigure with NULL FileSystem (db.pFS == NULL) should return LSM_OK
////////////////////////////////////////////////////////////////////////////////
static void test_lsMFsConfigure_null_pFS() {
    TEST_MSG("Test 1: lsmFsConfigure with NULL pFS should return LSM_OK");
    lsm_db db;
    std::memset(&db, 0, sizeof(db));
    db.pFS = NULL;

    int rc = lsmFsConfigure(&db);
    EXPECT_TRUE(rc == LSM_OK);
}

// Test 2: lsmFsConfigure with a valid FileSystem (no compression, iMmap=1) exercises core cleanup
// paths: zeroing buffers, clearing hash, clearing pointers, etc.
////////////////////////////////////////////////////////////////////////////////
static void test_lsMFsConfigure_basic_cleanup_unlimited() {
    TEST_MSG("Test 2: Basic cleanup path (no compression, unlimited mmap) validation");

    // Prepare a dummy environment and FileSystem
    lsm_env *pEnv = (lsm_env*)calloc(1, sizeof(lsm_env));
    FileSystem *pFS = (FileSystem*)calloc(1, sizeof(FileSystem));
    if(!pEnv || !pFS) {
        std::cerr << "Allocation failure in Test 2 setup" << std::endl;
        g_failures++;
        return;
    }

    // Initialize FileSystem preconditions
    pFS->pEnv = pEnv;
    pFS->nOut = 0;
    pFS->pWaiting = 0;
    pFS->pMapped = 0;

    // Compression buffers to be freed (set to NULL to avoid complexity)
    pFS->aIBuffer = NULL;
    pFS->aOBuffer = NULL;
    pFS->nBuffer = 5; // non-zero to verify reset to 0

    // No mapping present (pMap == NULL)
    pFS->pMap = NULL;
    pFS->fdDb = 3; // arbitrary fd
    pFS->nMap = 0;
    pFS->nMapLimit = 42; // will be overwritten (to 0 or unlimited depending on path)

    // Prepare a small LRU page chain with one free page
    Page *pPg = create_test_page_with_free_flag(pEnv);
    if(!pPg) {
        std::cerr << "Failed to allocate Page for Test 2" << std::endl;
        g_failures++;
        free(pFS);
        free(pEnv);
        return;
    }
    // Link into the pLruFirst chain
    pFS->pLruFirst = pPg;
    pFS->pLruLast = NULL; // not used in this test
    pFS->pFree = NULL;

    // apHash array with non-zero entries to ensure memset zeroes them
    pFS->nHash = 3;
    pFS->apHash = (void**)malloc(pFS->nHash * sizeof(void*));
    for(int i = 0; i < (int)pFS->nHash; ++i) {
        pFS->apHash[i] = (void*)(0x100 + i);
    }

    // Build db with compression disabled and iMmap == 1 (Unlimited mapping)
    lsm_db db;
    std::memset(&db, 0, sizeof(db));
    db.pFS = pFS;
    db.compress.xCompress = 0; // no compression
    db.iMmap = 1;               // unlimited
    db.iCache = 0;

    int rc = lsmFsConfigure(&db);
    EXPECT_TRUE(rc == LSM_OK);

    // Post-conditions checks
    EXPECT_TRUE(pFS->nBuffer == 0);
    // pMap remains unchanged (NULL), so no remap should have happened
    EXPECT_TRUE(pFS->nMapLimit == ((i64)1 << 60)); // Unlimited mapping path

    // Pointers cleared
    EXPECT_TRUE(pFS->pLruFirst == 0);
    EXPECT_TRUE(pFS->pLruLast == 0);
    EXPECT_TRUE(pFS->pFree == 0);

    // apHash should be zeroed
    for(int i = 0; i < (int)pFS->nHash; ++i) {
        EXPECT_TRUE(pFS->apHash[i] == NULL);
    }

    // Cleanup remaining allocations
    free(pPg->aData);
    free(pPg);
    free(pFS->apHash);
    free(pFS);
    free(pEnv);
}

// Test 3: lsmFsConfigure with compression enabled (db.compress.xCompress == 1)
// Expect pFS->pCompress points to &db->compress and nMapLimit is 0
////////////////////////////////////////////////////////////////////////////////
static void test_lsMFsConfigure_compress_enabled() {
    TEST_MSG("Test 3: Compression enabled path sets pCompress and zeroes nMapLimit");

    // Prepare objects
    lsm_env *pEnv = (lsm_env*)calloc(1, sizeof(lsm_env));
    FileSystem *pFS = (FileSystem*)calloc(1, sizeof(FileSystem));
    if(!pEnv || !pFS) {
        std::cerr << "Allocation failure in Test 3 setup" << std::endl;
        g_failures++;
        return;
    }

    pFS->pEnv = pEnv;
    pFS->nOut = 0;
    pFS->pWaiting = 0;
    pFS->pMapped = 0;
    pFS->aIBuffer = NULL;
    pFS->aOBuffer = NULL;
    pFS->nBuffer = 0;
    pFS->pMap = NULL;
    pFS->nMap = 0;
    pFS->nMapLimit = 0;
    pFS->pLruFirst = NULL;
    pFS->pLruLast = NULL;
    pFS->pFree = NULL;
    pFS->apHash = NULL;
    pFS->nHash = 0;

    // Setup db with compression enabled
    lsm_db db;
    std::memset(&db, 0, sizeof(db));
    db.pFS = pFS;
    // Prepare a compress structure
    // The actual type of compress is likely a struct with xCompress bool/flag and fields.
    // We assign values assuming standard layout used by the library.
    // Note: The following assignment relies on public member names present in lsmInt.h
    db.compress.xCompress = 1;
    // We mimic a full compress struct value for safety; if the compiler complains about missing member 'compress',
    // this test should be adapted to match the actual API (the test environment is assumed to provide it).
    // If 'db.compress' is a separate struct, ensure it's defined in the header and accessible here.
    // For safety with compilation environments lacking direct member access, you might need to adapt to the real API.

    // As we cannot guarantee direct access to all nested members of db.compress from here,
    // we defensively zero out iMmap and iCache to avoid side effects.
    db.iMmap = 0;
    db.iCache = 0;

    int rc = lsmFsConfigure(&db);
    EXPECT_TRUE(rc == LSM_OK);

    // Validate expectations
    EXPECT_TRUE(pFS->pCompress == &db.compress);
    EXPECT_TRUE(pFS->nMapLimit == 0); // Unmapped when compression is enabled

    // Cleanup
    free(pFS);
    free(pEnv);
}

// Test 4: lsmFsConfigure with compression disabled and iMmap specified in KB (e.g., 2 KB)
////////////////////////////////////////////////////////////////////////////////
static void test_lsMFsConfigure_iMmap_kb_path() {
    TEST_MSG("Test 4: iMmap path with KB limit (e.g., iMmap=2 -> 2048 bytes)");

    lsm_env *pEnv = (lsm_env*)calloc(1, sizeof(lsm_env));
    FileSystem *pFS = (FileSystem*)calloc(1, sizeof(FileSystem));
    if(!pEnv || !pFS) {
        std::cerr << "Allocation failure in Test 4 setup" << std::endl;
        g_failures++;
        return;
    }

    pFS->pEnv = pEnv;
    pFS->nOut = 0;
    pFS->pWaiting = 0;
    pFS->pMapped = 0;
    pFS->aIBuffer = NULL;
    pFS->aOBuffer = NULL;
    pFS->nBuffer = 0;
    pFS->pMap = NULL;
    pFS->nMap = 0;
    pFS->nMapLimit = 0;
    pFS->pLruFirst = NULL;
    pFS->pLruLast = NULL;
    pFS->pFree = NULL;
    pFS->apHash = NULL;
    pFS->nHash = 0;

    lsm_db db;
    std::memset(&db, 0, sizeof(db));
    db.pFS = pFS;
    db.compress.xCompress = 0;
    db.iMmap = 2; // 2 KB
    int rc = lsmFsConfigure(&db);
    EXPECT_TRUE(rc == LSM_OK);

    // Validate
    EXPECT_TRUE(pFS->nMapLimit == (i64)db.iMmap * 1024);

    // Cleanup
    free(pFS);
    free(pEnv);
}

////////////////////////////////////////////////////////////////////////////////
// Entry point for tests
////////////////////////////////////////////////////////////////////////////////
int main() {
    // Run tests
    test_lsMFsConfigure_null_pFS();
    test_lsMFsConfigure_basic_cleanup_unlimited();
    test_lsMFsConfigure_compress_enabled();
    test_lsMFsConfigure_iMmap_kb_path();

    // Summary
    if(g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}