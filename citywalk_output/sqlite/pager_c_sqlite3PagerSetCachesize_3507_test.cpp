// High-quality C++11 unit test suite for sqlite3PagerSetCachesize
// This test targets the focal function:
//   void sqlite3PagerSetCachesize(Pager *pPager, int mxPage) {
//     sqlite3PcacheSetCachesize(pPager->pPCache, mxPage);
//   }
// The test uses a minimal mocking strategy to observe that the inner
// call sqlite3PcacheSetCachesize receives the exact Pcache pointer from pPager
// and the intended mxPage value.
//
// Notes and design decisions:
// - We rely on the real SQLite types Pager and Pcache as defined in the project headers
//   (pager.h / sqliteInt.h). This ensures we test against true layout and semantics.
// - We attempt to override sqlite3PcacheSetCachesize by providing a mock in this TU.
//   If the build system links with a separate implementation of sqlite3PcacheSetCachesize,
//   this approach may require linker/config tweaks. The test demonstrates the intended
//   mocking strategy and is scaffolded for environments where symbol override is feasible.
// - The tests avoid terminating on first failure; they collect and print a summary.
// - We use simple assertions and a tiny test harness suitable for a manual main-based test
//   (no GTest).

#include <cstdio>
#include <climits>
#include <cstdbool>
#include <sqliteInt.h>
#include <pager.h>
#include <wal.h>


// Bind to the real SQLite types and APIs.
// We assume the project exposes Pager and Pcache types via pager.h (or sqliteInt.h).
extern "C" {
}

// We declare the symbol we want to observe/mimic. In environments where the project
// provides a real implementation, you may need to place this mock in a separate TU
// and ensure linker resolution allows it to be used instead of the production
// sqlite3PcacheSetCachesize. If your build links against the real pcache module,
// you may need to adapt the mocking strategy (e.g., using weak aliases, or a test
// shim library).
//
// Mock state to observe calls to sqlite3PcacheSetCachesize
static Pcache *g_last_called_pcache = nullptr;
static int g_last_called_mxPage = 0;
static bool g_sqlite3PcacheSetCachesize_was_called = false;

// Mock implementation of sqlite3PcacheSetCachesize to capture parameters.
// If your build system already provides a non-mockable definition, consider using a
// different symbol name for a local test shim or adjusting linker order.
extern "C" void sqlite3PcacheSetCachesize(Pcache *pCache, int mxPage) {
    g_last_called_pcache = pCache;
    g_last_called_mxPage = mxPage;
    g_sqlite3PcacheSetCachesize_was_called = true;
}

// Helper: simple test assertion that does not terminate the program on failure.
static int g_test_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        ++g_test_failures; \
        std::printf("Test failure: %s\n", (msg)); \
    } \
} while(0)

// Test 1: Basic contract - given a valid Pager with a valid pPCache, the focal
// function should forward the mxPage to sqlite3PcacheSetCachesize with the same Pcache pointer.
void test_basic_call() {
    // Prepare a dummy Pcache and Pager consistent with the real types.
    // These are intentionally minimal; the real implementation only requires the
    // pPCache pointer to be passed through.
    Pcache dummyCache;
    Pager pager;
    // Initialize the pager to point to our dummy cache.
    // The real Pager struct has many members; we only set what we need.
    // If the real structure has other members, ensure the test populates valid values
    // as required by your build (this test isolates pPCache usage).
    pager.pPCache = &dummyCache;

    // Reset mock state
    g_last_called_pcache = nullptr;
    g_last_called_mxPage = 0;
    g_sqlite3PcacheSetCachesize_was_called = false;

    // Exercise the focal function
    sqlite3PagerSetCachesize(&pager, 42);

    // Verify the mock was invoked with the exact pointer and value
    TEST_ASSERT(g_sqlite3PcacheSetCachesize_was_called, "sqlite3PcacheSetCachesize was not called");
    TEST_ASSERT(g_last_called_pcache == &dummyCache, "Called Pcache pointer does not match input pager's pPCache");
    TEST_ASSERT(g_last_called_mxPage == 42, "Called mxPage value does not match input");
}

// Test 2: Multiple values for mxPage to ensure proper forwarding across range.
// This exercises the function with different mxPage values in succession.
void test_multiple_values() {
    Pcache dummyCacheA, dummyCacheB;
    Pager pagerA, pagerB;

    pagerA.pPCache = &dummyCacheA;
    pagerB.pPCache = &dummyCacheB;

    // First call with a small value
    g_last_called_pcache = nullptr;
    g_last_called_mxPage = 0;
    g_sqlite3PcacheSetCachesize_was_called = false;
    sqlite3PagerSetCachesize(&pagerA, 0);
    TEST_ASSERT(g_sqlite3PcacheSetCachesize_was_called, "First call with mxPage=0 failed to invoke mock");
    TEST_ASSERT(g_last_called_pcache == &dummyCacheA, "First call: Pcache pointer mismatch");
    TEST_ASSERT(g_last_called_mxPage == 0, "First call: mxPage mismatch");

    // Second call with a large value
    g_last_called_pcache = nullptr;
    g_last_called_mxPage = 0;
    g_sqlite3PcacheSetCachesize_was_called = false;
    sqlite3PagerSetCachesize(&pagerB, INT_MAX);
    TEST_ASSERT(g_sqlite3PcacheSetCachesize_was_called, "Second call with mxPage=INT_MAX failed to invoke mock");
    TEST_ASSERT(g_last_called_pcache == &dummyCacheB, "Second call: Pcache pointer mismatch");
    TEST_ASSERT(g_last_called_mxPage == INT_MAX, "Second call: mxPage mismatch");
}

// Test 3: Null pPCache in Pager should still route the call to sqlite3PcacheSetCachesize,
// forwarding NULL as the Pcache pointer. This validates pointer forwarding behavior.
// Note: This relies on the mock being invoked even when the pointer is NULL.
void test_null_pcache() {
    Pager pager;
    pager.pPCache = nullptr; // Intentionally null

    g_last_called_pcache = reinterpret_cast<Pcache*>(0xDEADBEEF); // ensure not accidentally equal to null
    g_last_called_mxPage = -1;
    g_sqlite3PcacheSetCachesize_was_called = false;

    sqlite3PagerSetCachesize(&pager, -1);

    TEST_ASSERT(g_sqlite3PcacheSetCachesize_was_called, "Call with NULL pPCache did not invoke mock");
    TEST_ASSERT(g_last_called_pcache == nullptr, "Call with NULL pPCache should pass NULL to mock");
    TEST_ASSERT(g_last_called_mxPage == -1, "Call with mxPage=-1 should pass through value");
}

// Helper: run all tests and report summary
void run_all_tests() {
    test_basic_call();
    test_multiple_values();
    test_null_pcache();

    if (g_test_failures == 0) {
        std::printf("All tests passed.\n");
    } else {
        std::printf("Tests completed with %d failure(s).\n", g_test_failures);
    }
}

// Entry point
int main() {
    // Execute the test suite
    run_all_tests();
    return g_test_failures ? 1 : 0;
}