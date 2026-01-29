// Test suite for the focal method: cmpp_realloc
// Environment assumptions:
// - The project builds with C++11 and provides cmpp_realloc defined in c-pp.c.
// - We override realloc and fatal with C linkage to deterministically test both branches.
// - The tests are written without a testing framework (GTest not allowed); a small harness is provided.
// - We use a lightweight memory-tracking layer to deterministically simulate allocations.
// - All code is self-contained in this single file for clarity (link with c-pp.c during build).

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Forward declaration of the focal function from the C source under test.
// We assume the build system links cmpp_realloc from c-pp.c.
extern "C" void * cmpp_realloc(void * p, unsigned n);

// We provide the following C linkage functions to exercise cmpp_realloc deterministically.
extern "C" void * realloc(void * ptr, size_t size);
extern "C" void fatal(char const * zFmt, ...);

// ---------------- Memory management shim for deterministic tests ---------------- //

namespace {

static std::unordered_map<void*, size_t> g_alloc_map; // tracks allocations done via our test malloc-like path
static bool g_force_realloc_fail = false;              // when true, our realloc shim pretends to fail
static int g_fatal_calls = 0;                          // counts number of times fatal is invoked

// Simple helper: allocate memory and register it in our map
static void * mm_alloc(size_t sz) {
    void * p = std::malloc(sz);
    if (p) {
        g_alloc_map[p] = sz;
    }
    return p;
}

// Simple helper: free all tracked allocations (safe cleanup)
static void clear_allocs() {
    std::vector<void*> to_free;
    to_free.reserve(g_alloc_map.size());
    for (auto const &kv : g_alloc_map) to_free.push_back(const_cast<void*>(kv.first));
    for (void* p : to_free) std::free(p);
    g_alloc_map.clear();
}

// A lightweight test harness: non-terminating assertion-like check
static int g_test_failed = 0;
static void expect(bool cond, const char* description) {
    if (!cond) {
        std::cerr << "TEST FAIL: " << description << std::endl;
        ++g_test_failed;
    } else {
        std::cout << "TEST PASS: " << description << std::endl;
    }
}

// Utility to be used in tests: reset global test state
static void test_reset_state() {
    clear_allocs();
    g_force_realloc_fail = false;
    g_fatal_calls = 0;
    g_test_failed = 0;
}

// ---------------- Overridden C linkage implementations ---------------- //

extern "C" void fatal(char const * zFmt, ...) {
    // Minimal fatal stub: just increment a counter; do not terminate the test process.
    (void)zFmt; // suppress unused warning if not used
    ++g_fatal_calls;
}

// Our custom realloc override to deterministically exercise the rc NULL vs non-NULL branches.
// This function shadows the real realloc during the test, allowing us to simulate failure.
extern "C" void * realloc(void * ptr, size_t size) {
    // If we were not given a pointer to track, behave conservatively
    if (ptr == nullptr) {
        if (g_force_realloc_fail) return nullptr; // simulate allocation failure
        void * newp = std::malloc(size);
        if (newp) g_alloc_map[newp] = size;
        return newp;
    }

    // Look up original allocation size if we tracked it
    auto it = g_alloc_map.find(ptr);
    size_t old_size = 0;
    if (it != g_alloc_map.end()) old_size = it->second;

    if (g_force_realloc_fail) {
        // Simulate failure path
        return nullptr;
    }

    // Allocate a new block and (optionally) copy data from old to new
    void * newp = nullptr;
    newp = std::malloc(size);
    if (newp) {
        // Copy up to the minimum of old and new sizes to approximate realloc behavior
        size_t copysz = old_size < size ? old_size : size;
        if (copysz > 0 && ptr != nullptr) {
            // Safe memory copy within bounds since we only copy what we tracked
            std::memcpy(newp, ptr, copysz);
        }
        // Update tracker: remove old entry and add new
        if (ptr != nullptr) {
            g_alloc_map.erase(it);
        }
        g_alloc_map[newp] = size;
        // Free old block
        std::free(ptr);
    }

    return newp;
}

// Type-safe prototype for cmpp_realloc to be used in tests
using CmppReallocFn = void * (*)(void*, unsigned);

// ---------------- Domain-specific test helpers ---------------- //

static void test_cmpp_realloc_success() {
    // STEP: Ensure cmpp_realloc returns a non-null pointer and uses the fallback path correctly.
    test_reset_state();

    // Allocate initial block via our memory manager to ensure old size is known to the overridden realloc.
    void* p = mm_alloc(16);
    expect(p != nullptr, "mm_alloc should succeed for initial block (16 bytes)");

    // Call the focal function: should return a new non-NULL pointer (rc non-NULL)
    void* r = cmpp_realloc(p, 32);

    expect(r != nullptr, "cmpp_realloc should return non-NULL on success path");
    expect(r != p, "cmpp_realloc should return a different pointer after reallocation");

    // fatal should not have been invoked on success
    expect(g_fatal_calls == 0, "fatal should not be called on successful realloc");

    // Cleanup: free both the old (should be freed by our realloc shim) and new blocks
    // The shim frees the old block itself; we free the new one and clear trackers.
    if (r) {
        std::free(r);
        // Remove from tracker if present
        auto it = g_alloc_map.find(r);
        if (it != g_alloc_map.end()) g_alloc_map.erase(it);
    }
    clear_allocs();
}

static void test_cmpp_realloc_failure() {
    // STEP: Trigger the failure branch where rc is NULL and fatal is invoked.
    test_reset_state();

    // Configure the realloc shim to fail
    g_force_realloc_fail = true;

    void* p = mm_alloc(8);
    expect(p != nullptr, "mm_alloc should succeed for initial block (8 bytes) before failure");

    void* r = cmpp_realloc(p, 16);

    // Expect NULL and a fatal call
    expect(r == nullptr, "cmpp_realloc should return NULL when underlying realloc fails");
    expect(g_fatal_calls >= 1, "fatal should be invoked at least once on realloc failure");

    // Cleanup: in failure path, the old block was not freed by cmpp_realloc;
    // We'll free it manually to avoid leaks.
    if (p) {
        std::free(p);
        auto it = g_alloc_map.find(p);
        if (it != g_alloc_map.end()) g_alloc_map.erase(it);
    }
    clear_allocs();
}

// ---------------- Main test runner ---------------- //

int main() {
    std::cout << "Starting tests for cmpp_realloc...\n";

    test_cmpp_realloc_success();
    test_cmpp_realloc_failure();

    if (g_test_failed == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failed << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}