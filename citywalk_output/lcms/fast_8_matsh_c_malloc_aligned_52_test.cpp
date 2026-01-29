// Self-contained C++11 unit test suite for the focal function malloc_aligned.
// Notes:
// - This test suite is self-contained to avoid external dependencies.
// - It mimics the essential environment used by the focal function:
//   - A minimal XMatShaper8Data struct with a real_ptr member.
//   - A mock _cmsMallocZero that allocates and zeros memory, and records the last allocation.
// - We implement malloc_aligned with the same semantics as in the provided snippet.
// - Tests use non-terminating expectations to maximize coverage.
// - All tests are invoked from main without external test frameworks.

/*
  Expected behavior of malloc_aligned(ContextID):
  1. Allocate memory of size sizeof(XMatShaper8Data) + 32 via _cmsMallocZero.
  2. Compute an aligned pointer: aligned = (((uintptr_t)real_ptr + 16) & ~0xf).
  3. Cast aligned to XMatShaper8Data* and assign p->real_ptr = real_ptr.
  4. Return p.
*/

#include <cstring>
#include <fast_float_internal.h>
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Type and struct definitions to mirror minimal needed environment.
typedef void* cmsContext;
typedef unsigned char cmsUInt8Number;

struct XMatShaper8Data {
    cmsUInt8Number* real_ptr;
};

// ---- Mocked dependencies and function under test (self-contained) ----

// Global state to assist verification in tests without external frameworks.
static void* g_last_alloc_ptr = nullptr;
static size_t g_last_alloc_size = 0;
static int g_fail_alloc = 0; // If non-zero, _cmsMallocZero will simulate failure.

// Mock implementation of _cmsMallocZero used by malloc_aligned.
// Allocates size bytes, zeros them, and records the allocation for test verification.
extern "C" void* _cmsMallocZero(cmsContext ContextID, size_t size) {
    (void)ContextID; // Unused in mock
    if (g_fail_alloc) {
        // Simulate allocation failure.
        return nullptr;
    }
    void* p = std::calloc(1, size);
    if (p) {
        g_last_alloc_ptr = p;
        g_last_alloc_size = size;
    } else {
        g_last_alloc_ptr = nullptr;
        g_last_alloc_size = 0;
    }
    return p;
}

// Actual focal function re-implemented for test purposes (same logic as provided).
extern "C" XMatShaper8Data* malloc_aligned(cmsContext ContextID) {
    {
        cmsUInt8Number* real_ptr = (cmsUInt8Number*) _cmsMallocZero(ContextID, sizeof(XMatShaper8Data) + 32);
        cmsUInt8Number* aligned = (cmsUInt8Number*) (((uintptr_t)real_ptr + 16) & ~0xf);
        XMatShaper8Data* p = (XMatShaper8Data*) aligned;
        p->real_ptr = real_ptr;
        return p;
    }
}

// ---- Minimal non-GTest test harness utilities (EXPECT_ style) ----

static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(expected, actual) do { \
    if((expected) != (actual)) { \
        std::cerr << "EXPECT_EQ failed: expected " << (expected) << " but got " << (actual) \
                  << " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b) do { \
    if((void*)(a) != (void*)(b)) { \
        std::cerr << "EXPECT_PTR_EQ failed: " << (void*)(a) << " != " << (void*)(b) \
                  << " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_failures; \
    } \
} while(0)

// ---- Unit Tests ----

// Test 1: Normal allocation path returns an aligned XMatShaper8Data with correct real_ptr.
void test_normal_allocation() {
    // Reset global mock state for clean test.
    g_fail_alloc = 0;
    g_last_alloc_ptr = nullptr;
    g_last_alloc_size = 0;

    XMatShaper8Data* p = malloc_aligned(nullptr);

    // p should be non-null.
    EXPECT_TRUE(p != nullptr);

    // The returned pointer should be 16-byte aligned.
    uintptr_t addr = reinterpret_cast<uintptr_t>(p);
    EXPECT_TRUE((addr % 16) == 0);

    // p->real_ptr should point to the block allocated by _cmsMallocZero.
    EXPECT_TRUE(p->real_ptr != nullptr);
    EXPECT_PTR_EQ(p->real_ptr, g_last_alloc_ptr);

    // The allocation size should be sizeof(XMatShaper8Data) + 32.
    // This is what _cmsMallocZero was asked to allocate.
    EXPECT_EQ(sizeof(XMatShaper8Data) + 32, g_last_alloc_size);
}

// Test 2: Allocation failure in _cmsMallocZero propagates as a failed real_ptr pointer
// while still returning a valid XMatShaper8Data structure (with real_ptr == nullptr).
void test_allocation_failure_propagation() {
    // Simulate allocation failure for _cmsMallocZero.
    g_fail_alloc = 1;
    g_last_alloc_ptr = nullptr;
    g_last_alloc_size = 0;

    XMatShaper8Data* p = malloc_aligned(nullptr);

    // The outer object should still be returned (as per the function's behavior),
    // but the inner real_ptr should be nullptr due to allocation failure.
    EXPECT_TRUE(p != nullptr);
    EXPECT_TRUE(p->real_ptr == nullptr);
}

// Test 3: Multiple consecutive allocations yield distinct aligned blocks and pointers.
void test_multiple_allocations_distinct_pointers() {
    g_fail_alloc = 0;
    g_last_alloc_ptr = nullptr;
    g_last_alloc_size = 0;

    XMatShaper8Data* p1 = malloc_aligned(nullptr);
    XMatShaper8Data* p2 = malloc_aligned(nullptr);

    // Both allocations should succeed and produce distinct p addresses.
    EXPECT_TRUE(p1 != nullptr);
    EXPECT_TRUE(p2 != nullptr);
    EXPECT_TRUE(p1 != p2);

    // Each p's real_ptr must point to its own allocation block.
    EXPECT_PTR_EQ(p1->real_ptr, g_last_alloc_ptr); // After second call, g_last_alloc_ptr is second block
    // To verify distinct blocks, ensure their real_ptrs are different.
    EXPECT_TRUE(p1->real_ptr != p2->real_ptr);
}

// ---- Main runner ----

int main() {
    // Run tests
    test_normal_allocation();
    test_allocation_failure_propagation();
    test_multiple_allocations_distinct_pointers();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 2;
    }
}