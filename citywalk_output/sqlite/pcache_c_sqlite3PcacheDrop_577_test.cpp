// cpp_pcache_drop_tests.cpp
// A lightweight, non-GTest unit test suite for sqlite3PcacheDrop (C code),
// written in C++11 as requested.
// The tests are designed to be run in a single translation unit alongside
// the pcache.c implementation (or linked in a compatible build).
// - Uses a minimal, self-contained test harness (no external testing framework).
// - Focuses on observable side-effects consistent with the function under test.
// - Keeps static/internal behavior in mind (we only rely on documented side effects).

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cstdint>


// The focal method belongs to a C module (pcache.c). To compile this test in a
// single translation unit together with the C module, we provide compatible
// type definitions and a minimal environment that sqlite3PcacheDrop expects.
// NOTE: This test makes assumptions about the surrounding sqlite3 structures
// (PgHdr, PCache, and sqlite3GlobalConfig). In a real environment, you should
// include the actual SQLite headers and link against the real pcache.c.
// Here, we provide just enough scaffolding to exercise the core logic observable
// from sqlite3PcacheDrop: decrement of nRefSum, and the call to xUnpin.

#ifndef PGHDR_DIRTY
#define PGHDR_DIRTY 0x01
#endif

// Forward declare the types the focal function uses.
// In a real environment these would come from sqliteInt.h or equivalent headers.
typedef uint32_t Pgno;
typedef unsigned char u8;

struct PCache; // forward declaration

// PgHdr is the page header for a cached page.
typedef struct PgHdr {
    unsigned nRef;
    unsigned flags;
    PCache *pCache;
    Pgno pPage;
} PgHdr;

// PCache represents the per-cache structure with a reference count sum.
typedef struct PCache {
    int nRefSum;
    void *pCache; // opaque handle to the underlying cache object
} PCache;

// Minimal representation of sqlite3GlobalConfig.pcache2.xUnpin
struct sqlite3_pcache2_config {
    void (*xUnpin)(void*, Pgno, int);
};

struct sqlite3_global_config {
    struct sqlite3_pcache2_config pcache2;
};

// Global configuration used by sqlite3PcacheDrop
static struct sqlite3_global_config sqlite3GlobalConfig = { { nullptr } };

// Forward declaration of the focal C function.
// We assume C linkage for the function as provided by pcache.c.
extern "C" void sqlite3PcacheDrop(PgHdr *p);

// Test-scoped capture variables to verify behavior
static void *captured_xUnpin_pCache = nullptr;
static Pgno captured_xUnpin_pgno = 0;
static int captured_xUnpin_rc = 0;
static int xUnpin_calls = 0;

// Mock implementation of xUnpin to observe interactions from sqlite3PcacheDrop
static void mock_xUnpin(void *pCache, Pgno pgno, int rc) {
    captured_xUnpin_pCache = pCache;
    captured_xUnpin_pgno = pgno;
    captured_xUnpin_rc = rc;
    xUnpin_calls++;
}

// Helper to reset observation state between tests
static void reset_observation_state() {
    captured_xUnpin_pCache = nullptr;
    captured_xUnpin_pgno = 0;
    captured_xUnpin_rc = 0;
    xUnpin_calls = 0;
}

// Simple non-terminating assertions for test continuity
#define EXPECT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "[FAIL] " << (msg) << " | Expected: " << (expected) \
                  << ", Actual: " << (actual) << std::endl; \
    } else { \
        std::cout << "[PASS] " << (msg) << std::endl; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << std::endl; \
    } else { \
        std::cout << "[PASS] " << (msg) << std::endl; \
    } \
} while(0)


// Test 1: Basic drop path without PGHDR_DIRTY flag
// - Verifies that nRefSum on the underlying PCache is decremented.
// - Verifies that xUnpin is called with the underlying cache pointer, the page id, and 1.
// - This test does not test the static pcacheManageDirtyList path (since it's static in the C file).
static void test_sqlite3PcacheDrop_basic_noDirty() {
    std::cout << "Running test_sqlite3PcacheDrop_basic_noDirty..." << std::endl;

    // Arrange: wire up a fake PCache and PgHdr that resemble the real structures.
    PCache cache;
    cache.nRefSum = 5;
    cache.pCache = reinterpret_cast<void*>(0xDEADBEEF);

    PgHdr hdr;
    hdr.nRef = 1;
    hdr.flags = 0;               // Not dirty
    hdr.pCache = &cache;
    hdr.pPage = 42;

    // Install mock xUnpin to observe interaction
    sqlite3GlobalConfig.pcache2.xUnpin = mock_xUnpin;

    reset_observation_state();

    // Act: call the function under test
    sqlite3PcacheDrop(&hdr);

    // Assert: verify side-effects
    EXPECT_EQ(cache.nRefSum, 4, "Decrement nRefSum by 1 on drop");
    EXPECT_TRUE(captured_xUnpin_pCache == cache.pCache, "Unpin called with correct pCache");
    EXPECT_EQ(captured_xUnpin_pgno, hdr.pPage, "Unpin called with correct pPage");
    EXPECT_EQ(captured_xUnpin_rc, 1, "Unpin called with rc == 1");
    EXPECT_EQ(xUnpin_calls, 1, "Unpin should be called exactly once");

    std::cout << "Finished test_sqlite3PcacheDrop_basic_noDirty." << std::endl << std::endl;
}

// Test 2: Basic drop path with PGHDR_DIRTY flag set
// - Ensures that the dirty path (pcacheManageDirtyList with REMOVE) is exercised conceptually
//   by verifying xUnpin is still invoked and nRefSum is decremented.
// - We cannot observe pcacheManageDirtyList's internal list mutation because it is static,
//   but we can confirm that the rest of the observable effects occur.
static void test_sqlite3PcacheDrop_basic_dirtyFlag() {
    std::cout << "Running test_sqlite3PcacheDrop_basic_dirtyFlag..." << std::endl;

    // Arrange: reuse a PCache and PgHdr, set dirty flag
    PCache cache;
    cache.nRefSum = 7;
    cache.pCache = reinterpret_cast<void*>(0xCAFEBABE);

    PgHdr hdr;
    hdr.nRef = 1;
    hdr.flags = PGHDR_DIRTY;      // Dirty flag set
    hdr.pCache = &cache;
    hdr.pPage = 99;

    // Install mock xUnpin to observe interaction
    sqlite3GlobalConfig.pcache2.xUnpin = mock_xUnpin;

    reset_observation_state();

    // Act
    sqlite3PcacheDrop(&hdr);

    // Assert
    EXPECT_EQ(cache.nRefSum, 6, "Decrement nRefSum by 1 even when page is dirty");
    EXPECT_TRUE(captured_xUnpin_pCache == cache.pCache, "Unpin called with correct pCache (dirty path)");
    EXPECT_EQ(captured_xUnpin_pgno, hdr.pPage, "Unpin called with correct pPage (dirty path)");
    EXPECT_EQ(captured_xUnpin_rc, 1, "Unpin called with rc == 1 (dirty path)");
    EXPECT_EQ(xUnpin_calls, 1, "Unpin should be called exactly once (dirty path)");

    std::cout << "Finished test_sqlite3PcacheDrop_basic_dirtyFlag." << std::endl << std::endl;
}

int main() {
    // Note: In a real test, ensure the pcache.c module is linked and sqlite3GlobalConfig
    // is properly wired to the test environment. The tests below rely on the
    // observable effects of sqlite3PcacheDrop:
    // - Decrement of pCache->nRefSum
    // - Call to sqlite3GlobalConfig.pcache2.xUnpin(pCache->pCache, pPage, 1)

    test_sqlite3PcacheDrop_basic_noDirty();
    test_sqlite3PcacheDrop_basic_dirtyFlag();

    std::cout << "All tests completed." << std::endl;
    return 0;
}