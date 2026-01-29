// Unit test suite for sqlite3PcacheSetPageSize (pcache.c) using a handcrafted framework
// - No GoogleTest/GTest; uses simple C-style assertions and a main() entry point.
// - Tests are designed to compile against the project's SQLite C sources in a C++11 environment.
// - We rely on the internal API/structures (PCache, sqlite3_pcache, sqlite3GlobalConfig) being
//   available via sqlite3.h and internal headers already present in the project.
// - We override the pcache2 interface function pointers to fake behavior (xCreate, xCachesize, xDestroy).

#include <cstdio>
#include <sqlite3.h>
#include <cstring>
#include <sqliteInt.h>
#include <cassert>


// Include public and internal SQLite headers to obtain necessary types.
// The internal PCache structure and sqlite3GlobalConfig live in the SQLite sources.
extern "C" {
}

// Ensure we declare the function under test with C linkage to avoid name mangling issues.
extern "C" int sqlite3PcacheSetPageSize(PCache *pCache, int szPage);

// Global state to intercept and verify interactions with the pcache2 API.
static int g_xCreate_called;
static int g_xDestroy_called;
static int g_xCachesize_called;
static sqlite3_pcache *g_xCreate_return;
static int g_last_xCreate_szPage;
static int g_last_xCreate_szExtra;
static int g_last_xCreate_bPurgeable;
static sqlite3_pcache *g_last_xCachesize_p;
static int g_last_xCachesize_n;
static sqlite3_pcache *g_xCreate_return_for_test2 = NULL;

// Fake implementations to replace sqlite3GlobalConfig.pcache2.xCreate / xCachesize / xDestroy
static sqlite3_pcache* fake_xCreate(int szPage, int szExtra, int bPurgeable) {
    g_xCreate_called++;
    g_last_xCreate_szPage = szPage;
    g_last_xCreate_szExtra = szExtra;
    g_last_xCreate_bPurgeable = bPurgeable;
    // Return a non-null dummy pointer to simulate a real cache
    g_xCreate_return = (sqlite3_pcache*)0xCAFEBABE;
    return g_xCreate_return;
}
static void fake_xCachesize(sqlite3_pcache* p, int n) {
    g_xCachesize_called++;
    g_last_xCachesize_p = p;
    g_last_xCachesize_n = n;
}
static void fake_xDestroy(sqlite3_pcache* p) {
    g_xDestroy_called++;
    // We could log the pointer if needed
}

// Helper to reset the global fake state between tests
static void reset_fake_state() {
    g_xCreate_called = 0;
    g_xDestroy_called = 0;
    g_xCachesize_called = 0;
    g_xCreate_return = NULL;
    g_last_xCreate_szPage = 0;
    g_last_xCreate_szExtra = 0;
    g_last_xCreate_bPurgeable = 0;
    g_last_xCachesize_p = NULL;
    g_last_xCachesize_n = 0;
    g_xCreate_return_for_test2 = NULL;
}

// Test 1: Basic path where szPage != 0 and there is no existing pCache.
//         Expect: SQLITE_OK, pCache->pCache updated with new pointer, pCache->szPage updated.
//                 xCreate is invoked, xDestroy not invoked (no prior pCache).
static bool test_basic_set_page_size_updates_cache() {
    reset_fake_state();

    // Install fake pcache2 methods
    sqlite3GlobalConfig.pcache2.xCreate = fake_xCreate;
    sqlite3GlobalConfig.pcache2.xCachesize = fake_xCachesize;
    sqlite3GlobalConfig.pcache2.xDestroy = fake_xDestroy;

    PCache pCache;
    memset(&pCache, 0, sizeof(pCache));

    // Prepare a non-zero page size to trigger the path
    pCache.nRefSum = 0;
    pCache.pDirty = 0;
    pCache.szPage = 1024;      // non-zero to enter the if-block
    pCache.szExtra = 32;
    pCache.bPurgeable = 1;
    pCache.pCache = NULL;      // no prior cache to destroy

    int rc = sqlite3PcacheSetPageSize(&pCache, 4096);

    // Assertions
    if (rc != SQLITE_OK) {
        printf("test_basic_set_page_size_updates_cache: rc != SQLITE_OK\n");
        return false;
    }
    if (pCache.szPage != 4096) {
        printf("test_basic_set_page_size_updates_cache: szPage not updated\n");
        return false;
    }
    if (pCache.pCache != (sqlite3_pcache*)0xCAFEBABE) {
        printf("test_basic_set_page_size_updates_cache: pCache not updated to new cache\n");
        return false;
    }
    if (g_xCreate_called != 1) {
        printf("test_basic_set_page_size_updates_cache: xCreate not called exactly once\n");
        return false;
    }
    if (g_xDestroy_called != 0) {
        printf("test_basic_set_page_size_updates_cache: xDestroy should not be called when there was no prior cache\n");
        return false;
    }

    return true;
}

// Test 2: Path where there is an existing pCache and we set a new page size.
//         Expect: SQLITE_OK, old pCache destroyed via xDestroy, pCache replaced, xCreate invoked again.
static bool test_set_page_size_replaces_existing_cache() {
    reset_fake_state();

    sqlite3GlobalConfig.pcache2.xCreate = fake_xCreate;
    sqlite3GlobalConfig.pcache2.xCachesize = fake_xCachesize;
    sqlite3GlobalConfig.pcache2.xDestroy = fake_xDestroy;

    PCache pCache;
    memset(&pCache, 0, sizeof(pCache));

    // Existing cache present
    pCache.nRefSum = 0;
    pCache.pDirty = 0;
    pCache.szPage = 1024;
    pCache.szExtra = 16;
    pCache.bPurgeable = 1;
    pCache.pCache = (sqlite3_pcache*)0xDEADBEEF; // old cache

    int rc = sqlite3PcacheSetPageSize(&pCache, 2048);

    // Assertions
    if (rc != SQLITE_OK) {
        printf("test_set_page_size_replaces_existing_cache: rc != SQLITE_OK\n");
        return false;
    }
    if (pCache.szPage != 2048) {
        printf("test_set_page_size_replaces_existing_cache: szPage not updated\n");
        return false;
    }
    if (pCache.pCache != (sqlite3_pcache*)0xCAFEBABE) {
        printf("test_set_page_size_replaces_existing_cache: pCache not replaced with new cache\n");
        return false;
    }
    if (g_xDestroy_called != 1) {
        printf("test_set_page_size_replaces_existing_cache: xDestroy should be called once for old cache\n");
        return false;
    }
    if (g_xCreate_called != 1) {
        printf("test_set_page_size_replaces_existing_cache: xCreate should be called for new cache\n");
        return false;
    }

    return true;
}

// Test 3: Failure path when xCreate returns NULL (simulated NOMEM).
//         Expect: SQLITE_NOMEM_BKPT and pCache unchanged.
static bool test_xCreate_failure_returns_nOMEM() {
    reset_fake_state();

    // Prepare a failing xCreate by returning NULL; we replace the function pointer temporarily.
    // Define a local failing create
    auto failing_xCreate = [](int, int, int)->sqlite3_pcache* {
        (void)0; // suppress unused warnings
        return NULL;
    };

    // We need to adapt to C linkage expectations; provide a small wrapper function with C linkage.
    // Since we cannot bind a lambda to a C function pointer, we implement a static function instead.
    // For simplicity, swap in a real static function above. We'll reuse fake_xCreate by simulating failure
    // through a separate test-level override if available. We'll implement a dedicated static function below.

    // To keep within C linkage constraints, implement an explicit static failure create function:
    extern "C" sqlite3_pcache* test_xCreate_failure_allows_none(int szPage, int szExtra, int bPurgeable) {
        (void)szPage; (void)szExtra; (void)bPurgeable;
        return NULL;
    }

    // Bind the failing create
    sqlite3GlobalConfig.pcache2.xCreate = test_xCreate_failure_allows_none;
    sqlite3GlobalConfig.pcache2.xCachesize = fake_xCachesize;
    sqlite3GlobalConfig.pcache2.xDestroy = fake_xDestroy;

    PCache pCache;
    memset(&pCache, 0, sizeof(pCache));
    pCache.nRefSum = 0;
    pCache.pDirty = 0;
    pCache.szPage = 1024;
    pCache.szExtra = 16;
    pCache.bPurgeable = 1;
    pCache.pCache = NULL;

    int rc = sqlite3PcacheSetPageSize(&pCache, 2048);

    // Assertions
    if (rc != SQLITE_NOMEM_BKPT) {
        printf("test_xCreate_failure_returns_nOMEM: rc != SQLITE_NOMEM_BKPT\n");
        return false;
    }
    // pCache should remain unchanged because the creation failed
    if (pCache.pCache != NULL) {
        printf("test_xCreate_failure_returns_nOMEM: pCache should remain NULL on failure\n");
        return false;
    }

    // Restore safe state for potential further tests
    reset_fake_state();

    return true;
}

// Test 4: Branch when szPage is zero; no operation should occur, and function should return SQLITE_OK.
static bool test_zero_szPage_no_op() {
    reset_fake_state();

    // Do not modify pcache2 pointers
    sqlite3GlobalConfig.pcache2.xCreate = fake_xCreate;
    sqlite3GlobalConfig.pcache2.xCachesize = fake_xCachesize;
    sqlite3GlobalConfig.pcache2.xDestroy = fake_xDestroy;

    PCache pCache;
    memset(&pCache, 0, sizeof(pCache));

    pCache.nRefSum = 0;
    pCache.pDirty = 0;
    pCache.szPage = 0; // zero page size should skip creation
    pCache.szExtra = 16;
    pCache.bPurgeable = 1;
    pCache.pCache = NULL;

    int rc = sqlite3PcacheSetPageSize(&pCache, 4096);

    if (rc != SQLITE_OK) {
        printf("test_zero_szPage_no_op: rc != SQLITE_OK\n");
        return false;
    }
    // No changes should have occurred
    if (pCache.pCache != NULL || pCache.szPage != 0) {
        printf("test_zero_szPage_no_op: state should remain unchanged when szPage == 0\n");
        return false;
    }

    return true;
}

// Entry point: run all tests and report results
int main() {
    int failures = 0;

    printf("Running sqlite3PcacheSetPageSize unit tests...\n");

    if (!test_basic_set_page_size_updates_cache()) {
        printf("Test 1 failed.\n");
        ++failures;
    } else {
        printf("Test 1 passed.\n");
    }

    if (!test_set_page_size_replaces_existing_cache()) {
        printf("Test 2 failed.\n");
        ++failures;
    } else {
        printf("Test 2 passed.\n");
    }

    if (!test_xCreate_failure_returns_nOMEM()) {
        printf("Test 3 failed.\n");
        ++failures;
    } else {
        printf("Test 3 passed.\n");
    }

    if (!test_zero_szPage_no_op()) {
        printf("Test 4 failed.\n");
        ++failures;
    } else {
        printf("Test 4 passed.\n");
    }

    if (failures == 0) {
        printf("All tests passed.\n");
    } else {
        printf("%d test(s) failed.\n", failures);
    }

    // Return non-zero if any tests failed
    return failures;
}