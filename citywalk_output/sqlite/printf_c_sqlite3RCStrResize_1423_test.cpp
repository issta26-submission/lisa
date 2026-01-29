// Minimal C++11 test harness for sqlite3RCStrResize
// This test suite provides two scenarios:
// 1) Successful resize path: ensures a new RCStr header is allocated and the payload pointer is returned.
// 2) Failure path: simulates realloc64 failure and verifies old RCStr block is freed and NULL is returned.
// Notes:
// - We provide lightweight, non-terminating assertions and a simple test runner (no GTest).
// - We implement mock allocations for sqlite3_realloc64 and sqlite3_free to observe behavior.
// - We define a local RCStr struct with nRCRef as the first field to align with the usage in sqlite3RCStrResize.
// - The tests interact with the focal function sqlite3RCStrResize(char*, u64) declared extern "C".
// - This code assumes the project under test compiles with C++11 support and is linkable with the test harness.

#include <cstring>
#include <sqliteInt.h>
#include <unordered_map>
#include <iostream>
#include <cstdlib>


// Define u64 as used by the focal method (assuming typical sqlite typedefs)
using u64 = unsigned long long;

// Forward declaration of the focal function to test (as provided)
extern "C" {
    char *sqlite3RCStrResize(char *z, u64 N);
}

// Local RCStr layout (we only rely on the first member being nRCRef)
struct RCStr {
    unsigned long long nRCRef;
};

// Lightweight, non-terminating test assertion helpers
static int g_failures = 0;
static void test_log_pass(const char* msg) {
    std::cout << "PASS: " << msg << "\n";
}
static void test_log_fail(const char* msg) {
    std::cerr << "FAIL: " << msg << "\n";
    ++g_failures;
}
#define TEST_CHECK(cond, msg) \
    do { \
        if (!(cond)) { test_log_fail(msg); } \
        else { test_log_pass(msg); } \
    } while (0)
#define TEST_CHECK_EQ(a, b, msg) \
    do { \
        if (!((a) == (b))) { \
            std::cerr << "EXPECTED EQUAL: " << msg << " (actual: " << (void*)(a) << ", " << (void*)(b) << ")\n"; \
            ++g_failures; \
        } else { test_log_pass(msg); } \
    } while (0)
#define TEST_CHECK_NE(a, b, msg) \
    do { \
        if ((a) == (b)) { \
            std::cerr << "EXPECTED NOT EQUAL: " << msg << "\n"; \
            ++g_failures; \
        } else { test_log_pass(msg); } \
    } while (0)

// Simple in-test memory tracking to simulate sqlite3_realloc64 / sqlite3_free behavior
static std::unordered_map<void*, bool> g_alloc; // true = allocated and alive

static void resetAllocState() {
    g_alloc.clear();
}

// Mock implementations of sqlite3_realloc64 and sqlite3_free used by sqlite3RCStrResize
extern "C" {

// Simulate realloc: allocate new block of size n, copy header bytes from old block (sizeof(RCStr) of test),
// and return pointer to new header. We do not free the old block here (the caller (sqlite3RCStrResize)
// will decide on freeing on failure or success paths in the real sqlite code).
void* sqlite3_realloc64(void* p, unsigned long long n) {
    // If NULL pointer, mimic malloc
    if (p == nullptr) {
        void* q = std::malloc((size_t)n);
        if (q) g_alloc[q] = true;
        return q;
    }

    // Very lightweight fail path to exercise error handling in tests.
    // Choose a large threshold to simulate allocation failure.
    const unsigned long long kFailThreshold = (1ULL << 40); // ~1 TB (ample for tests)
    if (n > kFailThreshold) {
        // Simulate failure (caller should free old block)
        return nullptr;
    }

    // Allocate new block
    void* q = std::malloc((size_t)n);
    if (q == nullptr) return nullptr;

    // Copy a sensible amount of header data to preserve nRCRef (we copy sizeof(RCStr) in test sense)
    // This mirrors the behavior of a realloc that preserves the existing header fields.
    size_t copySize = sizeof(RCStr);
    if (copySize > (size_t)n) copySize = (size_t)n;
    std::memcpy(q, p, copySize);

    // Mark as allocated in the fake allocator
    g_alloc[q] = true;
    return q;
}

// Free function used by the focal code on failure path
void sqlite3_free(void* p) {
    if (p == nullptr) return;
    auto it = g_alloc.find(p);
    if (it != g_alloc.end()) {
        // Mark as freed, then actually free memory
        it->second = false;
        g_alloc.erase(it);
    }
    std::free(p);
}
} // extern "C"

// Helper: create a RCStr header block and its payload base pointer (z)
static void* allocateHeaderWithPayload(unsigned long long payloadSize, RCStr& outHeader) {
    // Allocate header + payload memory; header size is determined by our test RCStr
    size_t headerSize = sizeof(RCStr);
    void* base = std::malloc(headerSize + (size_t)payloadSize);
    if (base == nullptr) return nullptr;
    // Initialize header
    RCStr* header = (RCStr*)base;
    header->nRCRef = 1;
    // Track allocation for test observer
    g_alloc[base] = true;
    outHeader = *header;
    return base;
}

// Test 1: Resize succeeds and returns a valid payload pointer, with header nRCRef preserved
static void test_resize_success() {
    // Explanation:
    // - Create a base RCStr header with nRCRef = 1 and z pointing just after the header.
    // - Request a resize to N; the mock realloc should allocate a new block with a new header.
    // - Validate that the return pointer is non-null, that the new header's nRCRef is preserved (== 1),
    //   and that the new header is at a different address than the old one.
    resetAllocState();
    RCStr oldHeader;
    unsigned long long N = 32;
    // Allocate header + payload
    void* base = allocateHeaderWithPayload(128, oldHeader);
    TEST_CHECK(base != nullptr, "test_resize_success: header+payload allocation");
    RCStr* hdr = (RCStr*)base;
    // Sanity: nRCRef should be 1
    TEST_CHECK_EQ(hdr->nRCRef, 1, "test_resize_success: initial nRCRef == 1");

    size_t headerSize = sizeof(RCStr);
    char* z = (char*)base + headerSize;

    // Call focal function
    char* ret = sqlite3RCStrResize(z, N);
    TEST_CHECK(ret != nullptr, "test_resize_success: resize returns non-null");

    // Derive new header from returned pointer
    RCStr* newHdr = (RCStr*)(ret) - 1;
    TEST_CHECK_NE((void*)newHdr, (void*)hdr, "test_resize_success: header moved to new block");

    TEST_CHECK_EQ(newHdr->nRCRef, 1, "test_resize_success: new header nRCRef preserved as 1");
    TEST_CHECK_EQ((void*)ret, (void*)(newHdr + 1), "test_resize_success: ret points to payload after new header");

    // Cleanup: free both blocks if still allocated
    // The old header memory should have been kept by realloc; we simulate freeing both blocks
    sqlite3_free((void*)newHdr);
    sqlite3_free((void*)hdr); // may be same as original base; ensure both freed
    // Note: In a real environment, the realloc would take care of old block; here we simply ensure no crash
}

// Test 2: Resize fails (simulate realloc64 failure) and old RCStr block gets freed, NULL is returned
static void test_resize_failure() {
    // Explanation:
    // - Create a header+payload as in success case.
    // - Request an extremely large N to trigger alloc failure in our mock sqlite3_realloc64.
    // - The focal function should return 0 and free the old header.
    resetAllocState();
    RCStr dummyHeader;
    void* base = allocateHeaderWithPayload(64, dummyHeader);
    TEST_CHECK(base != nullptr, "test_resize_failure: header+payload allocation");
    RCStr* hdr = (RCStr*)base;
    hdr->nRCRef = 1;
    unsigned long long N = (1ULL << 62); // large value to force failure
    size_t headerSize = sizeof(RCStr);
    char* z = (char*)base + headerSize;

    // Call focal function
    char* ret = sqlite3RCStrResize(z, N);
    TEST_CHECK(ret == nullptr, "test_resize_failure: resize returns NULL on failure");

    // Verify old header block is freed (simulated by allocator map)
    auto it = g_alloc.find((void*)hdr);
    bool freed = true;
    if (it != g_alloc.end()) {
        freed = !(it->second); // if marked false, freed
    }
    TEST_CHECK(freed, "test_resize_failure: old header block freed on failure");

    // Cleanup: if old header still considered allocated in map, free it
    if (g_alloc.find((void*)hdr) != g_alloc.end()) {
        sqlite3_free((void*)hdr);
    }
    // No new header to free since allocation failed
    (void)base; // suppress unused warning if any
}

// Simple test runner
int main() {
    std::cout << "Running sqlite3RCStrResize unit tests (non-GTest, C++11):\n";

    test_resize_success();
    test_resize_failure();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Total failures: " << g_failures << "\n";
        return 1;
    }
}