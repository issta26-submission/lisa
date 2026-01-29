// Test suite for sqlite3PagerLookup in pager.c
// This test uses a minimal, self-contained harness with lightweight mocks
// of dependent cache lookup functions. It avoids terminating the test on
// assertion failures by using a small, custom non-terminating check macro.
//
// Assumptions:
// - The build environment provides the necessary internal SQLite headers (e.g. pager.h)
//   to define Pager, DbPage, Pgno, sqlite3_pcache, sqlite3_pcache_page, etc.
// - The actual dependent functions sqlite3PcacheFetch and sqlite3PcacheFetchFinish
//   are linkable (non-static) and can be overridden by providing the same
//   signatures in this test unit (as simple mocks).
// - We only exercise true positive paths for sqlite3PagerLookup where:
//   a) page not present in cache -> returns nullptr
//   b) page present in cache -> returns the value from sqlite3PcacheFetchFinish
//
// Note: This file is intended to be compiled with the rest of the project
// (C++11 compiler). It does not depend on GTest; it uses a tiny test harness.

#include <cstddef>
#include <sqliteInt.h>
#include <pager.h>
#include <unordered_map>
#include <iostream>
#include <wal.h>


// Domain headers (assumed to exist in the project)

// Lightweight non-terminating test assertion macro.
// It prints a message on failure but does not abort the test run.
#define TEST_CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " (" << __FUNCTION__ << ")\n"; \
        g_failed_tests++; \
    } else { \
        /* Optional verbose success: could be enabled if needed */ \
    } \
} while(0)

// Global test state
static int g_failed_tests = 0;

// ------------------------------------------------------------------------------------------------
// Mocks for Sqlite internal cache functions
// We provide simple, injectable implementations that sqlite3PagerLookup will call.
// We expose the same signatures as the real ones to satisfy the linker.
// The test uses a map to control which pages are "present" in the cache for a given Pgno.
// ------------------------------------------------------------------------------------------------

// Forward declarations to align with the project's internal types.
// We rely on the project headers (pager.h) for exact type definitions;
// we still provide external C interfaces to the mock functions below.

extern "C" {

// Typedefs for compatibility with common SQLite internal types.
// If the project already defines these, pager.h will provide the correct types.
typedef unsigned int Pgno;
typedef struct sqlite3_pcache sqlite3_pcache;
typedef struct sqlite3_pcache_page sqlite3_pcache_page;

// Forward declaration of DbPage and Pager as provided by pager.h
// (The real layout is defined in the project's headers; we only mock behavior here.)
typedef struct DbPage DbPage;
typedef struct Pager Pager;

// Mock cache storage: maps pgno to a dummy sqlite3_pcache_page pointer
static std::unordered_map<Pgno, sqlite3_pcache_page*> gPages;

// Mock implementation of sqlite3PcacheFetch used by sqlite3PagerLookup.
// Returns a non-null pointer if the page is present in gPages, otherwise nullptr.
sqlite3_pcache_page* sqlite3PcacheFetch(sqlite3_pcache* /*p*/, Pgno pgno, int /*flags*/) {
    auto it = gPages.find(pgno);
    if (it == gPages.end()) {
        return nullptr;
    }
    return it->second;
}

// Mock implementation of sqlite3PcacheFetchFinish used by sqlite3PagerLookup.
// For testing purposes, simply return the provided pPage.
sqlite3_pcache_page* sqlite3PcacheFetchFinish(sqlite3_pcache* /*p*/, Pgno /*pgno*/, sqlite3_pcache_page* pPage) {
    return pPage;
}

} // extern "C"

// ------------------------------------------------------------------------------------------------
// Test helpers and test cases
// Each test exercises a specific path of sqlite3PagerLookup and uses the
// lightweight mocks above to control behavior.
// ------------------------------------------------------------------------------------------------

// Test 1: When the page is not present in the cache, sqlite3PagerLookup should return nullptr.
// Preconditions:
// - A valid Pager with a non-null pPCache
// - pgno is non-zero
// - The mock cache does not contain the page
bool test_PagerLookup_PageMissing_ReturnsNull() {
    // Reset mock cache
    gPages.clear();

    // Prepare a Pager instance (fields must exist per pager.h)
    Pager p;
    // Use an arbitrary non-null cache pointer to satisfy the precondition.
    // Casting away to the internal sqlite3_pcache type as a placeholder.
    p.pPCache = reinterpret_cast<sqlite3_pcache*>(0x1);
    // hasHeldSharedLock not relevant when page is missing
    p.hasHeldSharedLock = 0;

    Pgno pg = 123; // arbitrary non-zero page number

    DbPage *pRes = sqlite3PagerLookup(&p, pg);

    TEST_CHECK(pRes == nullptr, "Expected nullptr when page not cached");
    return g_failed_tests == 0;
}

// Test 2: When the page is present in the cache and the page fetch finish returns
// the same page, sqlite3PagerLookup should return that page.
// Preconditions:
// - The page is inserted into the mock cache (gPages) for the given pgno
// - The Pager has a shared lock held (per the assertion in sqlite3PagerLookup)
bool test_PagerLookup_PagePresent_ReturnsPage() {
    // Reset mock cache
    gPages.clear();

    // Create a dummy sqlite3_pcache_page to represent the cached page
    static sqlite3_pcache_page cachedPage;
    Pgno pg = 456;

    gPages[pg] = &cachedPage;

    // Prepare a Pager with a non-null PCache and a held shared lock
    Pager p;
    p.pPCache = reinterpret_cast<sqlite3_pcache*>(0x1);
    p.hasHeldSharedLock = 1; // required for non-null cached page path

    DbPage *pRes = sqlite3PagerLookup(&p, pg);

    TEST_CHECK(pRes == reinterpret_cast<DbPage*>(&cachedPage), "Expected to return the cached page pointer");

    return g_failed_tests == 0;
}

// ------------------------------------------------------------------------------------------------
// Main entry point: run tests and report summary
// ------------------------------------------------------------------------------------------------
int main() {
    std::cout << "Running sqlite3PagerLookup tests (C++11 harness)..." << std::endl;

    bool ok1 = test_PagerLookup_PageMissing_ReturnsNull();
    bool ok2 = test_PagerLookup_PagePresent_ReturnsPage();

    // Summary
    int total = 2;
    int failures = g_failed_tests;
    if (failures == 0) {
        std::cout << "All tests passed (" << total << " total)." << std::endl;
        return 0;
    } else {
        std::cerr << failures << " test(s) failed out of " << total << "." << std::endl;
        return 1;
    }
}