// C++11 test harness for the focal method: sqlite3PcacheFetch
// This test suite verifies various eCreate handling paths invoked by sqlite3PcacheFetch.
// It relies on the internal SQLite structures declared in sqliteInt.h and the global
// configuration sqlite3GlobalConfig.pcache2.xFetch hook, which we mock here.
//
// The tests do not use GTest; instead, a lightweight assertion mechanism is used
// that logs failures but does not abort execution, allowing all tests to run
// and report a summary at the end.

#include <cstdint>
#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


// Include internal SQLite definitions (C headers). Use extern "C" to ensure C linkage.
extern "C" {
}

// Simple, non-terminating test assertion helpers
static int g_test_failures = 0;

static void expect_eq_int(const char* desc, int a, int b) {
    if (a != b) {
        fprintf(stderr, "TEST_FAIL: %s: expected %d, got %d\n", desc, b, a);
        ++g_test_failures;
    }
}

static void expect_ptr_eq(const char* desc, void* a, void* b) {
    if (a != b) {
        fprintf(stderr, "TEST_FAIL: %s: pointer mismatch: %p != %p\n", desc, a, b);
        ++g_test_failures;
    }
}

// Sentinel page return from the mocked xFetch
static sqlite3_pcache_page* const SENTINEL_PAGE = (sqlite3_pcache_page*)0xDEADBEEF;

// Capture locals for the mocked xFetch to verify invocation parameters
static sqlite3_pcache* g_last_xFetch_pCache_arg = nullptr;
static Pgno            g_last_xFetch_pgno_arg = 0;
static int             g_last_xFetch_createFlag_arg = -1;

// Mock implementation of sqlite3GlobalConfig.pcache2.xFetch
static sqlite3_pcache_page* mock_xFetch(sqlite3_pcache* pCache, Pgno pgno, int createFlag) {
    g_last_xFetch_pCache_arg = pCache;
    g_last_xFetch_pgno_arg = pgno;
    g_last_xFetch_createFlag_arg = createFlag;
    // Return a sentinel page so tests can verify the return value path
    return SENTINEL_PAGE;
}

static void reset_capture() {
    g_last_xFetch_pCache_arg = nullptr;
    g_last_xFetch_pgno_arg = 0;
    g_last_xFetch_createFlag_arg = -1;
}

// Test 1: createFlag = 0 (no allocation). Set up a case where pCache->eCreate would be 1,
// but since createFlag is 0, eCreate is masked to 0 for the actual fetch.
// This exercises the branch where allocate is not attempted.
static void test_pcacheFetch_createFlag0_noAlloc() {
    PCache pcache;
    sqlite3_pcache dummyCache;
    memset(&pcache, 0, sizeof(PCache));
    memset(&dummyCache, 0, sizeof(sqlite3_pcache));

    // Arrange PCache fields so that pCache is non-null and the "dirty" bit is set,
    // which would normally make eCreate == 1, but since createFlag == 0, eCreate is ignored.
    pcache.pCache = &dummyCache;
    pcache.bPurgeable = 1;                 // true
    pcache.pDirty = (sqlite3_pcache_page*)0x1; // non-null (dirty)
    pcache.eCreate = (pcache.bPurgeable && pcache.pDirty) ? 1 : 2; // 1

    // Install mock fetch
    sqlite3GlobalConfig.pcache2.xFetch = mock_xFetch;
    reset_capture();

    sqlite3_pcache_page* res = sqlite3PcacheFetch(&pcache, 10, 0);

    // Validate that xFetch was called with eCreate = 0 (due to createFlag == 0)
    expect_ptr_eq("Fetch called with correct pCache (flag 0)", (void*)g_last_xFetch_pCache_arg, (void*)pcache.pCache);
    expect_eq_int("Fetch called with correct pgno (flag 0)", g_last_xFetch_pgno_arg, 10);
    expect_eq_int("Fetch called with correct createFlag (flag 0)", g_last_xFetch_createFlag_arg, 0);

    // Validate return pointer is our sentinel
    expect_ptr_eq("Return value is sentinel when using mock xFetch", (void*)res, (void*)SENTINEL_PAGE);
}

// Test 2: createFlag = 3 with a purgeable, dirty page, leading to eCreate = 1.
// This exercises the path where allocation is considered inexpensive.
static void test_pcacheFetch_createFlag3_eCreate1() {
    PCache pcache;
    sqlite3_pcache dummyCache;
    memset(&pcache, 0, sizeof(PCache));
    memset(&dummyCache, 0, sizeof(sqlite3_pcache));

    pcache.pCache = &dummyCache;
    pcache.bPurgeable = 1;                 // true
    pcache.pDirty = (sqlite3_pcache_page*)0x2; // non-null (dirty)
    pcache.eCreate = (pcache.bPurgeable && pcache.pDirty) ? 1 : 2; // 1

    sqlite3GlobalConfig.pcache2.xFetch = mock_xFetch;
    reset_capture();

    sqlite3_pcache_page* res = sqlite3PcacheFetch(&pcache, 7, 3);

    // Should compute eCreate = 3 & 1 = 1
    expect_ptr_eq("Fetch called with correct pCache (flag 3, eCreate1)", (void*)g_last_xFetch_pCache_arg, (void*)pcache.pCache);
    expect_eq_int("Fetch called with correct pgno (flag 3)", g_last_xFetch_pgno_arg, 7);
    expect_eq_int("Fetch called with correct createFlag (flag 3)", g_last_xFetch_createFlag_arg, 1);

    expect_ptr_eq("Return value is sentinel when using mock xFetch (flag 3)", (void*)res, (void*)SENTINEL_PAGE);
}

// Test 3: createFlag = 3 with non-purgeable or non-dirty page, leading to eCreate = 2.
// This exercises a harder allocation scenario where allocation is considered difficult.
static void test_pcacheFetch_createFlag3_eCreate2() {
    PCache pcache;
    sqlite3_pcache dummyCache;
    memset(&pcache, 0, sizeof(PCache));
    memset(&dummyCache, 0, sizeof(sqlite3_pcache));

    pcache.pCache = &dummyCache;
    pcache.bPurgeable = 1;                 // true
    pcache.pDirty = nullptr;               // false (not dirty)
    pcache.eCreate = (pcache.bPurgeable && pcache.pDirty) ? 1 : 2; // 2

    sqlite3GlobalConfig.pcache2.xFetch = mock_xFetch;
    reset_capture();

    sqlite3_pcache_page* res = sqlite3PcacheFetch(&pcache, 13, 3);

    // eCreate = 3 & 2 = 2
    expect_ptr_eq("Fetch called with correct pCache (flag 3, eCreate2)", (void*)g_last_xFetch_pCache_arg, (void*)pcache.pCache);
    expect_eq_int("Fetch called with correct pgno (flag 3)", g_last_xFetch_pgno_arg, 13);
    expect_eq_int("Fetch called with correct createFlag (flag 3)", g_last_xFetch_createFlag_arg, 2);

    expect_ptr_eq("Return value is sentinel when using mock xFetch (eCreate2)", (void*)res, (void*)SENTINEL_PAGE);
}

// Entry point
int main() {
    printf("Starting sqlite3PcacheFetch unit tests (C++11 harness, no GTest)...\n");

    // Run tests
    test_pcacheFetch_createFlag0_noAlloc();
    test_pcacheFetch_createFlag3_eCreate1();
    test_pcacheFetch_createFlag3_eCreate2();

    if (g_test_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TESTS FAILED: %d failure(s)\n", g_test_failures);
        return 1;
    }
}