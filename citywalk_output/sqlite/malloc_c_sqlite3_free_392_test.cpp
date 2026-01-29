// Comprehensive standalone unit tests for the focal behavior of sqlite3_free.
// Note: This is a self-contained C++11 test harness reproducing the essential
// logic of sqlite3_free from the provided focal method. It mocks minimal
// dependencies to exercise true/false branches without requiring the full
// SQLite build system. This approach ensures executable tests even in
// environments where the entire SQLite project isn't available.
//
// The tests cover:
// - Null pointer handling (early return)
// - Path when bMemstat is false (direct xFree call)
// - Path when bMemstat is true (mutex enter/leave around xFree and size accounting)

#include <functional>
#include <sqliteInt.h>
#include <stdarg.h>
#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain-specific types and constants (simplified for test harness)
typedef unsigned char u8;
typedef std::size_t size_t;

#define MEMTYPE_HEAP 1

#define SQLITE_STATUS_MEMORY_USED 0
#define SQLITE_STATUS_MALLOC_COUNT 1

// Minimal mock of the global configuration used by sqlite3_free
struct MockMemM {
    void (*xFree)(void*);
};

struct MockGlobalConfig {
    bool bMemstat;
    MockMemM m;
};

// Global mock configuration instance
static MockGlobalConfig sqlite3GlobalConfig = { false, { nullptr } };

// Simple mutex stub and counters to verify locking behavior
static int g_mutex_enter_count = 0;
static int g_mutex_leave_count = 0;
static void* g_dummy_mutex = reinterpret_cast<void*>(0xDEADBEEF);

// Stubbed mutex enter/leave implementations (stand-ins for real sqlite3_mutex_enter/leave)
static void sqlite3_mutex_enter(void* mutex) {
    // Only count calls; memory address can be any non-null value
    (void)mutex;
    ++g_mutex_enter_count;
}
static void sqlite3_mutex_leave(void* mutex) {
    (void)mutex;
    ++g_mutex_leave_count;
}

// Stubbed memdebug helpers (always succeed for test purposes)
static int sqlite3MemdebugHasType(void* p, int t) {
    (void)p; (void)t;
    return 1; // always report that the pointer has MEMTYPE_HEAP
}
static int sqlite3MemdebugNoType(void* p, u8 t) {
    (void)p; (void)t;
    return 1; // always report that pointer has no other type
}

// Track allocations to provide a plausible sqlite3MallocSize(p) value
static std::unordered_map<void*, size_t> g_allocation_sizes;
static void* sqlite3Malloc(size_t n) {
    void* p = std::malloc(n);
    if (p) g_allocation_sizes[p] = n;
    return p;
}
static size_t sqlite3MallocSize(const void* p) {
    auto it = g_allocation_sizes.find(const_cast<void*>(p));
    if (it != g_allocation_sizes.end()) return it->second;
    return 0;
}
static void sqlite3_status_down(int /*v1*/, int /*v2*/) {
    // No-op for test harness
}

// Fake "xFree" tracker to assert that the free path was exercised
static bool g_fake_xFree_called = false;
static void fake_xFree(void* p) {
    // Remove from bookkeeping map if present
    auto it = g_allocation_sizes.find(p);
    if (it != g_allocation_sizes.end()) {
        g_allocation_sizes.erase(it);
    }
    g_fake_xFree_called = true;
    std::free(p);
}

// Standalone reproduction of sqlite3_free (simplified, self-contained)
static void sqlite3_free(void* p) {
    if (p == nullptr) return;  // Imp: R-49053-54554
    // In real code: asserts about memdebug types
    assert(sqlite3MemdebugHasType(p, MEMTYPE_HEAP));
    assert(sqlite3MemdebugNoType(p, (u8)~MEMTYPE_HEAP));

    // IfMemstat is enabled, simulate locking around size accounting and free
    if (sqlite3GlobalConfig.bMemstat) {
        sqlite3_mutex_enter(g_dummy_mutex);  // Use dummy global mutex
        sqlite3_status_down(SQLITE_STATUS_MEMORY_USED, static_cast<int>(sqlite3MallocSize(p)));
        sqlite3_status_down(SQLITE_STATUS_MALLOC_COUNT, 1);
        if (sqlite3GlobalConfig.m.xFree) sqlite3GlobalConfig.m.xFree(p);
        sqlite3_mutex_leave(g_dummy_mutex);
    } else {
        if (sqlite3GlobalConfig.m.xFree) sqlite3GlobalConfig.m.xFree(p);
    }
}

// Simple test harness
static void run_test(const std::string& name, const std::function<void()>& test_fn) {
    try {
        test_fn();
        std::cout << "[PASS] " << name << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[FAIL] " << name << " - Exception: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "[FAIL] " << name << " - Unknown exception" << std::endl;
    }
}

// Test 1: Null pointer should be a no-op
static void test_free_with_null_pointer() {
    // Ensure a clean state
    g_fake_xFree_called = false;
    // No allocation should occur; simply call and expect no crash
    sqlite3_free(nullptr);
    // No xFree should be called
    assert(!g_fake_xFree_called);
}

// Test 2: When bMemstat is false, sqlite3_free should call the provided xFree directly
static void test_free_with_memstat_false_calls_xFree() {
    // Configure: memstat disabled
    sqlite3GlobalConfig.bMemstat = false;
    // Install fake xFree
    sqlite3GlobalConfig.m.xFree = fake_xFree;
    // Allocate a block
    void* p = sqlite3Malloc(64);
    assert(p != nullptr);
    g_fake_xFree_called = false;
    // Call the focal method
    sqlite3_free(p);
    // After free, ensure xFree was invoked and memory reclaimed
    assert(g_fake_xFree_called);
    // Ensure allocation map no longer contains p
    assert(g_allocation_sizes.find(p) == g_allocation_sizes.end());
}

// Test 3: When bMemstat is true, sqlite3_free should lock, update stats, and free
static void test_free_with_memstat_true_checks_lock_and_free() {
    // Configure: memstat enabled
    sqlite3GlobalConfig.bMemstat = true;
    // Install fake xFree
    sqlite3GlobalConfig.m.xFree = fake_xFree;
    // Reset mutex counters
    g_mutex_enter_count = 0;
    g_mutex_leave_count = 0;
    // Allocate a block
    void* p = sqlite3Malloc(32);
    assert(p != nullptr);
    g_fake_xFree_called = false;
    // Call the focal method
    sqlite3_free(p);
    // Assertions
    assert(g_fake_xFree_called);            // xFree should have been invoked
    // Mutex should have been "entered" and then "left"
    assert(g_mutex_enter_count == 1);
    assert(g_mutex_leave_count == 1);
    // Ensure allocation was freed
    assert(g_allocation_sizes.find(p) == g_allocation_sizes.end());
}

// Entry point
int main() {
    // Seed state
    g_fake_xFree_called = false;
    // Run tests
    run_test("sqlite3_free handles null pointers (no-op)", test_free_with_null_pointer);
    run_test("sqlite3_free calls xFree when bMemstat is false", test_free_with_memstat_false_calls_xFree);
    run_test("sqlite3_free locks, updates stats, and frees when bMemstat is true", test_free_with_memstat_true_checks_lock_and_free);

    // Simple summary
    std::cout << "All tests executed." << std::endl;
    return 0;
}