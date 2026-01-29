// C++11 test suite for cmsMallocZeroDefaultFn (no GoogleTest; a small custom test framework is used)

#include <functional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <lcms2_internal.h>
#include <cstdint>


// Domain knowledge hints:
// - Focus on testing _cmsMallocZeroDefaultFn (and its interaction with _cmsMalloc and _cmsFreeDefaultFn).
// - We rely on the real implementations available in the linked C sources (lcms2_internal.h and cmserr.c).
// - We exercise true/false branches by validating normal allocation (non-NULL) and a large allocation likely to fail (NULL).
// - Static members and private/internal details are not accessed here (we test via public-like interfaces in the provided dependencies).

// Include necessary CMS/LCMS types and declarations
extern "C" {
    #include "lcms2_internal.h"  // provides cmsContext, cmsUInt32Number, etc.

    // Focal function under test (C linkage to avoid name mangling)
    void* _cmsMallocZeroDefaultFn(cmsContext ContextID, cmsUInt32Number size);

    // Supporting functions used by the focal function (C linkage)
    void _cmsFreeDefaultFn(cmsContext ContextID, void* Ptr);
}

// Simple non-terminating test framework
static int gTotalTests = 0;
static int gFailures = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[FAIL] " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++gFailures; \
        } \
        ++gTotalTests; \
    } while (0)

#define RUN_TEST(test) \
    do { \
        test(); \
    } while (0)

// Test 1: Basic allocation path, ensure memory is allocated and zeroed
// Explanation:
// - Call _cmsMallocZeroDefaultFn(NULL, 16) and verify:
            // 1) Returned pointer is non-NULL
            // 2) All 16 bytes are zero
            // 3) Memory is properly freed via _cmsFreeDefaultFn to avoid leaks
// This exercise exercises the "pt != NULL" branch and the memset path.
void test_cmsMallocZeroDefaultFn_basicZeroing() {
    // Arrange
    const cmsContext ctx = nullptr;
    const cmsUInt32Number size = 16;

    // Act
    void* ptr = _cmsMallocZeroDefaultFn(ctx, size);

    // Assert
    TEST_ASSERT(ptr != nullptr, "_expected non-NULL allocation from _cmsMallocZeroDefaultFn for size 16");

    if (ptr != nullptr) {
        unsigned char* bytes = static_cast<unsigned char*>(ptr);
        bool allZero = std::all_of(bytes, bytes + size, [](unsigned char c) { return c == 0; });
        TEST_ASSERT(allZero, "Expected allocated memory to be zero-initialized by _cmsMallocZeroDefaultFn");

        // Cleanup
        _cmsFreeDefaultFn(ctx, ptr);
    }
}

// Test 2: Null-path branch (pt == NULL) should return NULL and perform no writes.
// Explanation:
// - Force _cmsMalloc to fail by requesting an excessively large allocation that is almost guaranteed to fail.
// - Verify that _cmsMallocZeroDefaultFn returns NULL in this path.
// This creates coverage for the "if (pt == NULL) return NULL;" path.
void test_cmsMallocZeroDefaultFn_nullPath() {
    // Arrange
    const cmsContext ctx = nullptr;
    // Choose a very large size that large modern stdlib malloc is unlikely to satisfy
    const cmsUInt32Number largeSize = 0xFFFFFFFFu;

    // Act
    void* ptr = _cmsMallocZeroDefaultFn(ctx, largeSize);

    // Assert
    TEST_ASSERT(ptr == nullptr, "Expected NULL return from _cmsMallocZeroDefaultFn for an unreasonably large size (null-path)");
    // No need to free anything since we expect NULL
}

// Test 3: Multiple allocations to verify repeated zero-initialization behavior.
// Explanation:
// - Perform two successive allocations of a small size (8 bytes) and verify each is zeroed.
// - Ensure no cross-contamination between allocations and that each block is independently zeroed.
// - Free both blocks after verification.
void test_cmsMallocZeroDefaultFn_multipleAllocations() {
    const cmsContext ctx = nullptr;
    const cmsUInt32Number size = 8;

    // First allocation
    void* p1 = _cmsMallocZeroDefaultFn(ctx, size);
    TEST_ASSERT(p1 != nullptr, "First allocation should succeed (size 8)");
    if (p1 != nullptr) {
        unsigned char* b1 = static_cast<unsigned char*>(p1);
        bool allZero1 = std::all_of(b1, b1 + size, [](unsigned char c) { return c == 0; });
        TEST_ASSERT(allZero1, "First allocated block should be zero-initialized");
        _cmsFreeDefaultFn(ctx, p1);
    }

    // Second allocation
    void* p2 = _cmsMallocZeroDefaultFn(ctx, size);
    TEST_ASSERT(p2 != nullptr, "Second allocation should succeed (size 8)");
    if (p2 != nullptr) {
        unsigned char* b2 = static_cast<unsigned char*>(p2);
        bool allZero2 = std::all_of(b2, b2 + size, [](unsigned char c) { return c == 0; });
        TEST_ASSERT(allZero2, "Second allocated block should be zero-initialized");
        _cmsFreeDefaultFn(ctx, p2);
    }
}

// Main runner
int main() {
    std::cout << "Starting tests for _cmsMallocZeroDefaultFn...\n";

    RUN_TEST(test_cmsMallocZeroDefaultFn_basicZeroing);
    RUN_TEST(test_cmsMallocZeroDefaultFn_nullPath);
    RUN_TEST(test_cmsMallocZeroDefaultFn_multipleAllocations);

    std::cout << "Test results: " << gTotalTests << " run, "
              << gFailures << " failures.\n";

    if (gFailures > 0) {
        std::cerr << "Some tests failed. See details above." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}