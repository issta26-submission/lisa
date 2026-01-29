/*
Unit test suite for the focal method: _cmsReallocDefaultFn
Location: cmserr.c
Goal: Validate the behavior of the realloc wrapper with a guard against oversized allocations.

Notes:
- The focal function signature (as seen in the provided snippet) is:
  void* _cmsReallocDefaultFn(cmsContext ContextID, void* Ptr, cmsUInt32Number size)
- Behavior:
  - If size > MAX_MEMORY_FOR_ALLOC, return NULL (no large reallocs allowed).
  - Otherwise, return realloc(Ptr, size); and mark ContextID as unused.
- Core dependencies/keywords captured for test design:
  - MAX_MEMORY_FOR_ALLOC
  - realloc
  - cmsContext (opaque context type)
  - cmsUInt32Number (size type, typically unsigned int)
  - cmsUNUSED_PARAMETER(ContextID) (unused parameter policy)

The tests below are designed to:
- Cover the false branch (size > MAX_MEMORY_FOR_ALLOC) and ensure NULL is returned.
- Cover the true branch (size <= MAX_MEMORY_FOR_ALLOC) with a non-NULL result.
- Cover usage of a NULL original pointer (Ptr == NULL) to exercise malloc-like behavior.
- Confirm that the function does not misuse the ContextID parameter (non-impact).

Important: This test suite assumes the presence of the actual implementation cmserr.c and corresponding headers, so the test can link against _cmsReallocDefaultFn. The tests rely on standard C realloc semantics for small allocations and on the known guard condition for large allocations.

Code (C++11, no GTest, self-contained test runner):
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Domain knowledge: The real signature uses external types.
// We re-declare minimal types here to compile and link with the focal function.
typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;

// Function under test (extern C to link with the C implementation in cmserr.c)
extern "C" void* _cmsReallocDefaultFn(cmsContext ContextID, void* Ptr, cmsUInt32Number size);

// Simple non-terminating test assertion helpers
static int g_test_failures = 0;

#define TEST_LOG(msg) do { std::cerr << "[TEST] " << msg << std::endl; } while(0)
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        TEST_LOG(std::string("EXPECT_TRUE failed: ") + (msg)); \
        ++g_test_failures; \
    } \
} while(0)
#define EXPECT_NULL(p, msg) EXPECT_TRUE((p) == nullptr, msg)
#define EXPECT_NOT_NULL(p, msg) EXPECT_TRUE((p) != nullptr, msg)


// Test 1: Oversized allocation should return NULL (branch false)
static void test_realloc_oversized_returns_null() {
    // Arrange
    const cmsUInt32Number large_size = 0xFFFFFFFF; // 4294967295, guaranteed > typical MAX_MEMORY_FOR_ALLOC
    void* original = std::malloc(128);
    EXPECT_NOT_NULL(original, "malloc for test should not fail");

    // Act
    void* result = _cmsReallocDefaultFn((cmsContext)0xDEADBEEF, original, large_size);

    // Assert
    EXPECT_NULL(result, "Expected NULL when size exceeds MAX_MEMORY_FOR_ALLOC");

    // Cleanup: original should remain valid if realloc failed; free it
    if (original) std::free(original);
}

// Test 2: Normal realloc within limits should return non-NULL and preserve data
static void test_realloc_within_limits_preserves_data() {
    // Arrange
    const cmsUInt32Number initial_size = 16;
    const cmsUInt32Number new_size = 32;
    void* ptr = std::malloc(initial_size);
    EXPECT_NOT_NULL(ptr, "malloc for test should not fail");

    // Initialize known pattern
    unsigned char* p = static_cast<unsigned char*>(ptr);
    for (unsigned int i = 0; i < initial_size; ++i) p[i] = static_cast<unsigned char>(0xAA);

    // Act
    void* result = _cmsReallocDefaultFn((cmsContext)0x1234, ptr, new_size);

    // Assert
    EXPECT_NOT_NULL(result, "Expected non-NULL result for realloc within limits");
    unsigned char* res = static_cast<unsigned char*>(result);
    // The original data should have been preserved for the first 'initial_size' bytes if realloc moved memory
    if (initial_size > 0) {
        EXPECT_TRUE(res[0] == static_cast<unsigned char>(0xAA), "First byte should preserve data after realloc");
    }

    // Try to use newly allocated memory to ensure it's writable
    res[0] = static_cast<unsigned char>(0xBB);

    // Cleanup
    std::free(result);
}

// Test 3: Pointer is NULL; realloc(NULL, size) should behave like malloc (within limits)
static void test_realloc_null_ptr_within_limits() {
    // Arrange
    const cmsUInt32Number size = 24;

    // Act
    void* result = _cmsReallocDefaultFn((cmsContext)0, nullptr, size);

    // Assert
    EXPECT_NOT_NULL(result, "Expected non-NULL result when realloc with NULL pointer within limits");

    // Use the new memory
    unsigned char* r = static_cast<unsigned char*>(result);
    r[0] = static_cast<unsigned char>(0xCC);
    r[size - 1] = static_cast<unsigned char>(0xDD);

    // Cleanup
    std::free(result);
}

// Test 4: ContextID should be unused; ensure behavior is unchanged with different ContextID
static void test_context_id_is_ignored() {
    // Arrange
    const cmsUInt32Number size = 16;
    void* p1 = std::malloc(size);
    void* p2 = std::malloc(size);
    EXPECT_NOT_NULL(p1, "malloc1 should not fail");
    EXPECT_NOT_NULL(p2, "malloc2 should not fail");

    // Initialize
    unsigned char* a1 = static_cast<unsigned char*>(p1);
    unsigned char* a2 = static_cast<unsigned char*>(p2);
    for (unsigned int i = 0; i < size; ++i) a1[i] = static_cast<unsigned char>(i & 0xFF);
    for (unsigned int i = 0; i < size; ++i) a2[i] = static_cast<unsigned char>((i + 1) & 0xFF);

    // Act with two different context IDs
    void* r1 = _cmsReallocDefaultFn((cmsContext)0xABCDEF, p1, size * 2);
    void* r2 = _cmsReallocDefaultFn((cmsContext)0x0, p2, size * 2);

    // Assert
    EXPECT_NOT_NULL(r1, "Realloc with ContextID 0xABCDEF should not fail");
    EXPECT_NOT_NULL(r2, "Realloc with ContextID 0x0 should not fail");

    // Cleanup
    if (r1) std::free(r1);
    if (r2) std::free(r2);
}

// Main: run all tests and report
int main() {
    TEST_LOG("Starting _cmsReallocDefaultFn unit tests (C++11, non-GTest)...");
    test_realloc_oversized_returns_null();
    test_realloc_within_limits_preserves_data();
    test_realloc_null_ptr_within_limits();
    test_context_id_is_ignored();

    if (g_test_failures == 0) {
        std::cout << "[TEST STATUS] ALL tests PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "[TEST STATUS] " << g_test_failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}