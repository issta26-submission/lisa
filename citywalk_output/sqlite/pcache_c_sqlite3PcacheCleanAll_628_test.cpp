// test_pcache_clean_all.cpp
// A lightweight unit-test harness for the focal method sqlite3PcacheCleanAll(PCache *pCache)
// using a minimal, self-contained mock-up of the relevant data structures.
// The goal is to verify that sqlite3PcacheCleanAll traverses the dirty list and
// invokes sqlite3PcacheMakeClean on every dirty page, in order, and that the list is
// advanced correctly as MakeClean would normally mutate the cache state.
//
// Notes / Assumptions:
// - We rely on the real sqlite3PcacheCleanAll function being available via linkage.
// - We provide minimal, compatible forward declarations for PgHdr and PCache,
//   matching the usage in sqlite3PcacheCleanAll (i.e., PCache must expose pDirty).
// - We implement a test-scoped mock for sqlite3PcacheMakeClean to observe side-effects
//   on the PCache's pDirty pointer, so the loop can progress to completion.
// - We do not rely on GoogleTest or any non-standard framework. We use a small custom
//   assertion helper that continues tests after failures (non-terminating assertions).

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Forward declarations to match the external C code we are testing.
extern "C" {
  // The focal function under test
  void sqlite3PcacheCleanAll(void *pCache); // Using void* to avoid strict typedef coupling in this harness

  // The helper invoked by the focal method. We provide a mock implementation below in C++
  // that will be linked with the real sqlite3PcacheCleanAll from pcache.c.
  void sqlite3PcacheMakeClean(void *p);

  // Trace helper used by the focal method; kept as a no-op for tests.
  void pcacheTrace(void *x);
}

// Minimal stand-ins for the actual data structures.
// We mirror only the parts used by sqlite3PcacheCleanAll: PCache has a pointer to a dirty list,
// and each PgHdr (page header) can be linked via pDirtyNext by tests to form a chain.

struct PgHdr {
  int id;                 // identifier to help verify call order
  PgHdr *pDirtyNext;      // next in the dirty list (as used in testing)
};

struct PCache {
  PgHdr *pDirty;           // head of the dirty list
};

// Helpers to bridge between C linkage expected by the real code and C++ test harness.

static PCache *g_test_cache = nullptr;     // the PCache being exercised
static std::vector<int> g_cleaned_ids;       // record of ids passed to sqlite3PcacheMakeClean
static bool g_trace_enabled = false;       // not used, but reserved for potential tracing

// Global hook to mimic the behavior sqlite3PcacheMakeClean would have in the real code.
// The real function would mutate the dirty list; our test harness advances the head so
// the while loop in sqlite3PcacheCleanAll can terminate.
extern "C" void sqlite3PcacheMakeClean(void *p) {
  PgHdr *pg = static_cast<PgHdr*>(p);
  if (pg == nullptr) return;

  // Record the page id to verify correct order of operations.
  g_cleaned_ids.push_back(pg->id);

  // Advance the dirty list head to simulate the real behavior where MakeClean removes the
  // current page from the dirty list. We set g_test_cache->pDirty accordingly.
  if (g_test_cache != nullptr) {
    g_test_cache->pDirty = pg->pDirtyNext;
  }

  // In a real scenario, sqlite3PcacheMakeClean would detach the page etc.
  // For the test, we just detach the next pointer to make sure next iteration progresses cleanly.
  pg->pDirtyNext = nullptr;
}

// Stub for pcacheTrace used by sqlite3PcacheCleanAll; left empty for unit tests.
extern "C" void pcacheTrace(void *x) {
  (void)x; // unused
}

// Minimal helper: run a single test case and report results without terminating on first fail.
static int g_failures = 0;
static void expect_bool(bool cond, const char* msg) {
  if (!cond) {
    std::cerr << "[FAIL] " << msg << "\n";
    ++g_failures;
  }
}
static void expect_vec_eq(const std::vector<int>& actual, const std::vector<int>& expected, const std::string& msg) {
  if (actual != expected) {
    std::cerr << "[FAIL] " << msg << "  Expected:";
    for (int v : expected) std::cerr << " " << v;
    std::cerr << "  Got:";
    for (int v : actual) std::cerr << " " << v;
    std::cerr << "\n";
    ++g_failures;
  }
}

// The test driver uses the actual sqlite3PcacheCleanAll function from the codebase.
// We declare a C-compatible signature to call it from C++.
extern "C" void sqlite3PcacheCleanAll(void *pCache);

// Helper to set up a chained dirty list for tests.
static void build_dirty_chain(PCache *cache, const std::vector<int>& ids) {
  // Build a singly linked list: id1 -> id2 -> ... -> nullptr
  PgHdr *prev = nullptr;
  for (auto it = ids.rbegin(); it != ids.rend(); ++it) {
    PgHdr *node = new PgHdr();
    node->id = *it;
    node->pDirtyNext = prev;
    prev = node;
  }
  cache->pDirty = prev;
}

// Clean up dynamically allocated test nodes to avoid leaks (not strictly necessary for short tests,
// but good practice).
static void free_dirty_chain(PCache *cache) {
  PgHdr *p = cache->pDirty;
  while (p != nullptr) {
    PgHdr *next = p->pDirtyNext;
    delete p;
    p = next;
  }
  cache->pDirty = nullptr;
}

// Test harness
static void run_test(const std::string& name, const std::vector<int>& dirty_ids, const std::vector<int>& expected_cleaned) {
  // Prepare cache
  PCache cache;
  cache.pDirty = nullptr;
  g_test_cache = &cache;
  g_cleaned_ids.clear();

  // Build the dirty list as specified
  build_dirty_chain(&cache, dirty_ids);

  // Call the focal function; ensure MakeClean will advance the dirty list
  sqlite3PcacheCleanAll(&cache);

  // Verify that MakeClean was invoked exactly as many times as there are nodes
  expect_vec_eq(g_cleaned_ids, expected_cleaned, name + ": cleaned ids do not match expected order.");

  // Clean up the chain to avoid leaks for subsequent tests
  free_dirty_chain(&cache);
  g_test_cache = nullptr;
}

// Entry point
int main() {
  // Note: We rely on the external sqlite3PcacheCleanAll symbol being present and using
  // the PCache struct with a pDirty member. Our test harness provides:
  // - A mock sqlite3PcacheMakeClean that advances pDirty
  // - A no-op pcacheTrace
  //
  // Test 1: No dirty pages -> nothing should be cleaned
  {
    PCache cache;
    cache.pDirty = nullptr;
    g_test_cache = &cache;
    g_cleaned_ids.clear();

    sqlite3PcacheCleanAll(&cache);

    expect_bool(g_cleaned_ids.empty(), "Test 1: expected no clean calls when pDirty is null");
    g_test_cache = nullptr;
  }

  // Test 2: Single dirty page
  // Expect exactly one call to sqlite3PcacheMakeClean with that page id
  {
    run_test("Test 2 - single page", { 42 }, { 42 });
  }

  // Test 3: Multiple dirty pages chained (3 pages)
  // Expect three calls in the order [1, 2, 3]
  {
    run_test("Test 3 - three pages", { 1, 2, 3 }, { 1, 2, 3 });
  }

  // Test 4: Non-sequential IDs to ensure proper ordering is preserved
  {
    run_test("Test 4 - non-sequential ids", { 10, 5, 20, 7 }, { 10, 5, 20, 7 });
  }

  // Final result
  if (g_failures == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cerr << "Total failures: " << g_failures << "\n";
    return 1;
  }
}