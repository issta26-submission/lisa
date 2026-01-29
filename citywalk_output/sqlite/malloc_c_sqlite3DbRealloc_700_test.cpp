// C++11 unit test suite for sqlite3DbRealloc (no GTest, plain main-based tests)

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <cstdlib>


extern "C" {
}

// Provide a lightweight mock of sqlite3_mutex_held to satisfy asserts in tests.
// We assume any non-null mutex handle means "held".
typedef struct sqlite3_mutex sqlite3_mutex;
extern "C" int sqlite3_mutex_held(sqlite3_mutex *);

/* Define the mock to always return true for non-null mutex pointers. */
extern "C" int sqlite3_mutex_held(sqlite3_mutex *mutex) {
    (void)mutex;
    return 1; // pretend the mutex is always held in unit tests
}

// Helper to safely print test status and abort on failure
static void testFail(const char* testName, const char* msg) {
    fprintf(stderr, "TEST FAIL: %s - %s\n", testName, msg);
    std::exit(1);
}

// Test 1: p == 0 should allocate via sqlite3DbMallocRawNN
static void test_sqlite3DbRealloc_pNull_allocates() {
    // Create a minimal sqlite3 db object (zero-initialized)
    sqlite3 *db = (sqlite3*)calloc(1, sizeof(sqlite3));
    if (db == nullptr) {
        testFail("test_sqlite3DbRealloc_pNull_allocates", "Failed to allocate sqlite3 object");
    }

    // Call with p == NULL
    void *result = sqlite3DbRealloc(db, nullptr, (u64)16);

    if (result == nullptr) {
        testFail("test_sqlite3DbRealloc_pNull_allocates", "sqlite3DbRealloc returned null for p==NULL");
    }

    // Cleanup
    free(result);
    free(db);
}

// Test 2: p within lookaside middle area and n small -> should return p
// Setup:
// - pStart ... pEnd define a region
// - pMiddle = pStart (so p >= pMiddle is true)
// - n = 0 (should satisfy n <= LOOKASIDE_SMALL regardless of exact value)
static void test_sqlite3DbRealloc_pMiddleHandles_small() {
    sqlite3 *db = (sqlite3*)calloc(1, sizeof(sqlite3));
    if (!db) {
        testFail("test_sqlite3DbRealloc_pMiddleHandles_small", "Failed to allocate sqlite3 object");
    }

    // Create a memory region to act as lookaside
    char *region = (char*)malloc(256);
    if (!region) {
        free(db);
        testFail("test_sqlite3DbRealloc_pMiddleHandles_small", "Failed to allocate lookaside region");
    }

    // Configure lookaside in db
    db->lookaside.pStart = region;
    db->lookaside.pEnd   = region + 256;
    db->lookaside.pMiddle = region; // pMiddle == pStart
    db->lookaside.szTrue = 64;      // not used in this path, but set plausibly

    // p equals pMiddle (which is region_start), n is small (0)
    void *p = region;
    u64 n = 0;

    void *ret = sqlite3DbRealloc(db, p, n);
    if (ret != p) {
        // If this branch works, it should return the same pointer
        char msg[128];
        snprintf(msg, sizeof(msg),
                 "Expected return of p (%p), got %p", (void*)p, ret);
        testFail("test_sqlite3DbRealloc_pMiddleHandles_small", msg);
    }

    // Cleanup
    free(region);
    free(db);
}

// Test 3: pStart <= p < pMiddle and n <= szTrue -> should return p
// Setup:
// - pMiddle > pStart to create a different branch
// - p between start and middle
// - n <= szTrue to trigger return
static void test_sqlite3DbRealloc_pStart_to_pMiddle_returns_p() {
    sqlite3 *db = (sqlite3*)calloc(1, sizeof(sqlite3));
    if (!db) {
        testFail("test_sqlite3DbRealloc_pStart_to_pMiddle_returns_p", "Failed to allocate sqlite3 object");
    }

    // Create a memory region to act as lookaside
    char *region = (char*)malloc(256);
    if (!region) {
        free(db);
        testFail("test_sqlite3DbRealloc_pStart_to_pMiddle_returns_p", "Failed to allocate lookaside region");
    }

    // Configure lookaside in db
    db->lookaside.pStart = region;
    db->lookaside.pEnd   = region + 256;
    db->lookaside.pMiddle = region + 128; // pMiddle after pStart
    db->lookaside.szTrue = 1;              // ensure small n triggers path

    // Choose p between pStart and pMiddle
    void *p = region + 64;
    u64 n = 1; // n <= szTrue

    void *ret = sqlite3DbRealloc(db, p, n);
    if (ret != p) {
        char msg[128];
        snprintf(msg, sizeof(msg),
                 "Expected return of p (%p), got %p", (void*)p, ret);
        testFail("test_sqlite3DbRealloc_pStart_to_pMiddle_returns_p", msg);
    }

    // Cleanup
    free(region);
    free(db);
}

// Main runner
int main() {
    // Run tests with explanatory comments in code
    test_sqlite3DbRealloc_pNull_allocates();
    test_sqlite3DbRealloc_pMiddleHandles_small();
    test_sqlite3DbRealloc_pStart_to_pMiddle_returns_p();

    printf("ALL SQLITE3 DB realloc tests passed.\n");
    return 0;
}