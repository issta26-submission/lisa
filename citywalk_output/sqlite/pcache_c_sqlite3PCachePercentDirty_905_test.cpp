// Test suite for sqlite3PCachePercentDirty in pcache.c
// Language: C++11
// Test framework: Minimal custom test runner (no GoogleTest) with non-terminating assertions.
// This test relies on the public API and the internal PCache/PgHdr structures as provided by sqlite's headers.
// The tests focus on validating the core logic of sqlite3PCachePercentDirty:
//   - Counts dirty pages linked from pCache->pDirty
//   - Uses numberOfCachePages(pCache) to obtain total cache pages
//   - Returns 0 when nCache == 0, otherwise floor((nDirty * 100) / nCache)
// Notes:
//   - We provide a small set of scenarios to ensure both branches of the final conditional are exercised.
//   - We assume the project under test exposes a non-static numberOfCachePages(PCache*) that can be controlled
//     (as per typical sqlite internal API). If in your environment numberOfCachePages is static in pcache.c,
//     this test may need adjustment to align with the actual build (e.g., expose a controllable stub).

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <vector>


// Include the sqlite internal types and the declaration of sqlite3PCachePercentDirty.
// The actual path may vary depending on your build system; adjust if needed.

// If the build environment provides a non-static numberOfCachePages(PCache*), we can override it in tests.
// We attempt to declare it with C linkage to be picked up by the linker (the actual symbol must not be static in pcache.c).
extern "C" int numberOfCachePages(PCache *p);

// Global control for test to influence numberOfCachePages behavior.
// If your environment uses a different mechanism to control/cache the value, adapt accordingly.
static int g_test_nCache = 0;

// Stubbed override for numberOfCachePages to provide deterministic behavior in tests.
// This must align with how the real project exposes this symbol. If the real code uses a static
// function, this override will be ignored; in that case test adaptivity is required.
extern "C" int numberOfCachePages(PCache *p) {
    (void)p; // unused
    return g_test_nCache;
}

// Helper to construct a dirty list of length n and attach it to a PCache instance.
static PgHdr* buildDirtyList(int n) {
    // Build a simple singly-linked list where pDirty points to the first element.
    PgHdr *head = nullptr;
    PgHdr *prev = nullptr;
    for (int i = 0; i < n; ++i) {
        PgHdr* node = new PgHdr();
        node->pDirtyNext = nullptr;
        if (!head) {
            head = node;
        }
        if (prev) {
            prev->pDirtyNext = node;
        }
        prev = node;
    }
    return head; // May be nullptr if n==0
}

// Simple non-terminating assertion helper: prints message on failure but continues.
static void ASSERT_EQ_INT(const std::string &msg, int expected, int actual) {
    if (expected != actual) {
        std::cerr << "ASSERT FAILED: " << msg
                  << " | expected: " << expected << ", actual: " << actual << "\n";
    } else {
        std::cout << "ASSERT PASSED: " << msg << "\n";
    }
}

// Test 1: No dirty pages, non-zero cache size -> expect 0
static void test_no_dirty_pages_nonzero_cache() {
    PCache cache;
    cache.pDirty = nullptr;

    g_test_nCache = 5; // nCache = 5
    int result = sqlite3PCachePercentDirty(&cache);
    ASSERT_EQ_INT("No dirty pages with non-zero cache should yield 0", 0, result);
}

// Test 2: One dirty page, non-zero cache -> expect floor(1*100 / nCache)
static void test_one_dirty_page_nonzero_cache() {
    PCache cache;
    cache.pDirty = buildDirtyList(1);

    g_test_nCache = 5; // nCache = 5
    int result = sqlite3PCachePercentDirty(&cache);
    int expected = (int)(( (int64_t)1 * 100) / 5);
    ASSERT_EQ_INT("One dirty page with cache=5", expected, result);

    // cleanup
    // Since buildDirtyList allocated on heap, delete nodes
    PgHdr* cur = cache.pDirty;
    while (cur) {
        PgHdr* nxt = cur->pDirtyNext;
        delete cur;
        cur = nxt;
    }
    cache.pDirty = nullptr;
}

// Test 3: Three dirty pages, cache equals 3 -> expect 100
static void test_all_dirty_pages_match_cache() {
    PCache cache;
    cache.pDirty = buildDirtyList(3);

    g_test_nCache = 3; // nCache = 3
    int result = sqlite3PCachePercentDirty(&cache);
    ASSERT_EQ_INT("Three dirty pages with cache=3 should yield 100", 100, result);

    // cleanup
    PgHdr* cur = cache.pDirty;
    while (cur) {
        PgHdr* nxt = cur->pDirtyNext;
        delete cur;
        cur = nxt;
    }
    cache.pDirty = nullptr;
}

// Test 4: Dirty pages present, cache size = 0 -> expect 0
static void test_dirty_pages_zero_cache() {
    PCache cache;
    cache.pDirty = buildDirtyList(4);

    g_test_nCache = 0; // nCache = 0
    int result = sqlite3PCachePercentDirty(&cache);
    ASSERT_EQ_INT("Dirty pages with cache=0 should yield 0", 0, result);

    // cleanup
    PgHdr* cur = cache.pDirty;
    while (cur) {
        PgHdr* nxt = cur->pDirtyNext;
        delete cur;
        cur = nxt;
    }
    cache.pDirty = nullptr;
}

// Test 5: Large numbers to ensure 64-bit math and integer division behavior
static void test_large_numbers() {
    PCache cache;
    // Create 1000 dirty pages
    cache.pDirty = buildDirtyList(1000);

    g_test_nCache = 7; // small cache to exercise truncation
    int result = sqlite3PCachePercentDirty(&cache);
    // expected = floor((1000 * 100) / 7)
    int expected = (int)(( (int64_t)1000 * 100) / 7);
    ASSERT_EQ_INT("Large dirty list with cache=7", expected, result);

    // cleanup
    PgHdr* cur = cache.pDirty;
    while (cur) {
        PgHdr* nxt = cur->pDirtyNext;
        delete cur;
        cur = nxt;
    }
    cache.pDirty = nullptr;
}

int main() {
    std::cout << "Starting test suite: sqlite3PCachePercentDirty\n";

    test_no_dirty_pages_nonzero_cache();
    test_one_dirty_page_nonzero_cache();
    test_all_dirty_pages_match_cache();
    test_dirty_pages_zero_cache();
    test_large_numbers();

    std::cout << "Test suite completed.\n";

    // Note: In a full CI, you might return a non-zero value if any assertion failed.
    // For simplicity here, we rely on visible stdout/stderr to determine success.
    return 0;
}