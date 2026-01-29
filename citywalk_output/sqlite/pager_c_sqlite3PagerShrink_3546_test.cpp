// Unit test suite for sqlite3PagerShrink (the focal method)
// This test suite mocks the dependency sqlite3PcacheShrink and validates
// that the pager wrapper correctly forwards the pPCache pointer to the cache shrink.
// The tests are written as plain C++11 code (no Google Test) with a lightweight
// non-terminating assertion mechanism to maximize code coverage.

#include <pager.h>
#include <sqliteInt.h>
#include <iostream>
#include <wal.h>
#include <cstdint>


// Include the project headers to get the real type definitions for Pager and Pcache.
// These headers are assumed to be part of the project under test.

// ---------------------------------------------------------------------------
// Test harness setup (mock for sqlite3PcacheShrink)
// We provide a mock implementation of sqlite3PcacheShrink to capture the argument
// it receives. The focal function sqlite3PagerShrink simply forwards the
// pPager->pPCache to sqlite3PcacheShrink, so verifying the argument proves correct
// forwarding behavior.
// ---------------------------------------------------------------------------

// Pointer to record the last argument passed to sqlite3PcacheShrink
static Pcache *lastShrunkPcachePtr = nullptr;

// Mocked dependency: intercept calls to sqlite3PcacheShrink and record the argument.
extern "C" void sqlite3PcacheShrink(Pcache *pCache){
    lastShrunkPcachePtr = pCache;
}

// Lightweight assertion harness: non-terminating, prints on failure but continues
static int testsPassed = 0;
static int testsFailed = 0;

#define TEST_CHECK(cond, msg) do { \
    if (cond) { \
        ++testsPassed; \
    } else { \
        ++testsFailed; \
        std::cerr << "[TEST FAIL] " << msg << std::endl; \
    } \
} while(0)

// ---------------------------------------------------------------------------
// Test 1: Verify that sqlite3PagerShrink forwards the actual pPCache pointer
// to sqlite3PcacheShrink. This ensures correct linkage between Pager and its cache.
// ---------------------------------------------------------------------------

void test_sqlite3PagerShrink_forwards_pcache_pointer() {
    // Prepare a dummy Pcache object to be used as the pPCache target
    Pcache dummyCache;
    // Prepare a Pager instance with its pPCache pointing to the dummy object
    Pager pagerInstance;
    pagerInstance.pPCache = &dummyCache; // Access is via the real struct layout

    // Reset the mock capture
    lastShrunkPcachePtr = nullptr;

    // Invoke the focal method under test
    sqlite3PagerShrink(&pagerInstance);

    // Validate that sqlite3PcacheShrink received the exact pointer
    TEST_CHECK(lastShrunkPcachePtr == &dummyCache,
               "sqlite3PagerShrink should pass the actual pPCache pointer to sqlite3PcacheShrink");
}

// ---------------------------------------------------------------------------
// Test 2: Verify behavior when pPCache is NULL.
// The wrapper should forward NULL to sqlite3PcacheShrink without crashing.
// ---------------------------------------------------------------------------

void test_sqlite3PagerShrink_forwards_null_pcache() {
    // Prepare a Pager instance with a NULL pPCache
    Pager pagerInstance;
    pagerInstance.pPCache = nullptr;

    // Reset the mock capture
    lastShrunkPcachePtr = reinterpret_cast<Pcache*>(0xDEADBEEF); // non-null sentinel

    // Invoke the focal method under test
    sqlite3PagerShrink(&pagerInstance);

    // Validate that a NULL pointer was forwarded
    TEST_CHECK(lastShrunkPcachePtr == nullptr,
               "sqlite3PagerShrink should forward NULL when pPCache is NULL");
}

// ---------------------------------------------------------------------------
// Test 3 (optional): Multiple invocations to ensure consistent forwarding behavior.
// This test runs two sequential shrinks to validate no state leakage in the mock.
// ---------------------------------------------------------------------------

void test_sqlite3PagerShrink_multiple_invocations() {
    Pcache frontCache;
    Pcache backCache;

    Pager pagerInstance;
    pagerInstance.pPCache = &frontCache;

    lastShrunkPcachePtr = nullptr;
    sqlite3PagerShrink(&pagerInstance);
    TEST_CHECK(lastShrunkPcachePtr == &frontCache,
               "First invocation should forward frontCache");

    // Change the cached pointer and shrink again
    pagerInstance.pPCache = &backCache;
    sqlite3PagerShrink(&pagerInstance);
    TEST_CHECK(lastShrunkPcachePtr == &backCache,
               "Second invocation should forward backCache");
}

// ---------------------------------------------------------------------------
// Main entry: run all tests and report summary.
// ---------------------------------------------------------------------------

int main() {
    std::cout << "Running tests for sqlite3PagerShrink...\n";

    test_sqlite3PagerShrink_forwards_pcache_pointer();
    test_sqlite3PagerShrink_forwards_null_pcache();
    test_sqlite3PagerShrink_multiple_invocations();

    std::cout << "Test summary: "
              << testsPassed << " passed, "
              << testsFailed << " failed.\n";

    // Return non-zero if any test failed to aid in automation
    return (testsFailed > 0) ? 1 : 0;
}