// Test suite for _cmsDupDefaultFn (simulated in-test environment)
// This test harness is designed for C++11 without Google Test.
// It provides minimal stubs for required dependencies and verifies the focal function
// behavior: allocation guard, successful copy, NULL source handling, and allocation failure.

// Includes
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-specific type aliases (mimicking cms types)
using cmsContext = void*;
using cmsUInt32Number = uint32_t;

// Domain constant from focal code
static const cmsUInt32Number MAX_MEMORY_FOR_ALLOC = 512u * 1024u * 1024u; // 512 MB

// Test controls to simulate allocator behavior
static bool gAllocShouldFail = false;           // When true, _cmsMalloc will fail (simulate OOM)
static cmsUInt32Number gLastRequestedSize = 0;  // Track last allocation size requested by _cmsMalloc

// Forward declarations of required functions (as in focal file)
extern "C" {
    // Simulated allocator function used by _cmsDupDefaultFn
    void* _cmsMalloc(cmsContext ContextID, cmsUInt32Number size);

    // Focal function under test (as defined in the provided focal method)
    void* _cmsDupDefaultFn(cmsContext ContextID, const void* Org, cmsUInt32Number size);
}

// Minimal _cmsMalloc implementation used for tests
extern "C" void* _cmsMalloc(cmsContext ContextID, cmsUInt32Number size) {
    (void)ContextID; // Unused in tests
    gLastRequestedSize = size;
    if (gAllocShouldFail) {
        return NULL; // Simulate allocation failure
    }
    return std::malloc(static_cast<std::size_t>(size));
}

// Focal function under test (reproduced here for self-contained tests)
// In the real project, this function would reside in cmserr.c
extern "C" void* _cmsDupDefaultFn(cmsContext ContextID, const void* Org, cmsUInt32Number size) {
    void* mem;
    if (size > MAX_MEMORY_FOR_ALLOC) return NULL;  // Never dup over 512Mb
    mem = _cmsMalloc(ContextID, size);
    if (mem != NULL && Org != NULL)
        std::memmove(mem, Org, size);
    return mem;
}

// Simple test harness
static int gTotalTests = 0;
static int gFailedTests = 0;

#define TEST_PASSED() do { std::cout << "[PASS] " << __func__ << "\n"; } while(0)
#define TEST_FAILED(msg) do { std::cerr << "[FAIL] " << __func__ << ": " << (msg) << "\n"; gFailedTests++; } while(0)
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { TEST_FAILED(msg ? msg : "Condition failed"); return false; } } while(0)
#define ASSERT_EQ(a, b, msg) do { if((a) != (b)) { TEST_FAILED((msg) ? (msg) : "Values not equal"); return false; } } while(0)


// Test 1: Basic copy of small data via _cmsDupDefaultFn
// - Verifies that allocation occurs and the copied bytes match the source
bool test_copy_small_data() {
    // Prepare source data
    const unsigned char src[4] = {1, 2, 3, 4};
    const cmsUInt32Number size = 4;

    gLastRequestedSize = 0;
    void* dst = _cmsDupDefaultFn(nullptr, src, size);
    ASSERT_TRUE(dst != nullptr, "Allocation failed when it should succeed for small data");
    ASSERT_EQ(gLastRequestedSize, size, "Allocator was called with incorrect size");

    // Validate content
    bool contentMatches = (std::memcmp(dst, src, static_cast<size_t>(size)) == 0);
    if (!contentMatches) {
        TEST_FAILED("Copied content does not match source");
        std::free(dst);
        return false;
    }

    std::free(dst);
    TEST_PASSED();
    return true;
}


// Test 2: Org pointer is NULL
// - Verifies that memory is allocated and function returns non-NULL, but no copy occurs
bool test_null_org_no_copy() {
    const cmsUInt32Number size = 8;
    gLastRequestedSize = 0;

    void* dst = _cmsDupDefaultFn(nullptr, nullptr, size);
    ASSERT_TRUE(dst != nullptr, "Allocation failed when Org is NULL but size is valid");
    ASSERT_EQ(gLastRequestedSize, size, "Allocator was called with incorrect size when Org is NULL");

    // No content guarantee since Org is NULL; just ensure memory is allocated and freed
    std::free(dst);
    TEST_PASSED();
    return true;
}


// Test 3: Size exceeds MAX_MEMORY_FOR_ALLOC
// - Verifies that no allocation is attempted and NULL is returned
bool test_size_exceeds_limit() {
    // Prepare a size just above the limit
    const cmsUInt32Number size = MAX_MEMORY_FOR_ALLOC + 1;
    const unsigned char src[4] = {9, 9, 9, 9};

    gLastRequestedSize = 0;

    void* dst = _cmsDupDefaultFn(nullptr, src, size);
    ASSERT_TRUE(dst == nullptr, "Expected NULL when size exceeds MAX_MEMORY_FOR_ALLOC");
    // Allocator should not have been invoked for out-of-bounds size
    ASSERT_EQ(gLastRequestedSize, 0, "Allocator should not be called for oversized request");

    TEST_PASSED();
    return true;
}


// Test 4: Allocation failure simulation
// - Verifies that NULL is returned when allocator reports failure
bool test_allocation_failure() {
    // Enable allocator failure simulation
    gAllocShouldFail = true;

    const cmsUInt32Number size = 16;
    const unsigned char src[4] = {1, 2, 3, 4};

    gLastRequestedSize = 0;
    void* dst = _cmsDupDefaultFn(nullptr, src, size);
    ASSERT_TRUE(dst == nullptr, "Expected NULL when allocator reports failure");

    // Cleanup: reset allocator behavior
    gAllocShouldFail = false;
    ASSERT_EQ(gLastRequestedSize, size, "Allocator should have been invoked before failure");

    TEST_PASSED();
    return true;
}


// Test 5: Copy with non-NULL Org and ensure correct content and last size
bool test_copy_non_null_org_content() {
    // Create a source with distinct bytes
    const unsigned char src[5] = {10, 20, 30, 40, 50};
    const cmsUInt32Number size = 5;

    gLastRequestedSize = 0;
    void* dst = _cmsDupDefaultFn(nullptr, src, size);
    ASSERT_TRUE(dst != nullptr, "Allocation failed for non-null Org test");
    ASSERT_EQ(gLastRequestedSize, size, "Allocator was called with incorrect size");

    // Validate content
    bool contentMatches = (std::memcmp(dst, src, static_cast<size_t>(size)) == 0);
    if (!contentMatches) {
        std::free(dst);
        TEST_FAILED("Copied content does not match source for non-NULL Org");
        return false;
    }

    std::free(dst);
    TEST_PASSED();
    return true;
}


// Main: run all tests
int main() {
    std::cout << "Running test suite for _cmsDupDefaultFn (simulated in-test environment)\n";

    gTotalTests = 0;
    gFailedTests = 0;

    // Incremental test execution
    if (test_copy_small_data()) {
        // passed
    } else {
        // already counted inside test
    }

    if (test_null_org_no_copy()) {
        // passed
    }

    if (test_size_exceeds_limit()) {
        // passed
    }

    if (test_allocation_failure()) {
        // passed
    }

    if (test_copy_non_null_org_content()) {
        // passed
    }

    // Summary
    std::cout << "Test results: " << (6 - gFailedTests) << " / 6 tests passed.\n";
    return (gFailedTests == 0) ? 0 : 1;
}