// Unit test suite for DebugRealloc (C API) without using GTest.
// This test suite is designed to be compiled with a C++11 compiler
// and links against the existing test harness that provides
// DebugMalloc, DebugFree, DebugRealloc and related types.
// The tests avoid private access and use the public API only.

#include <cstring>
#include <vector>
#include <iostream>
#include <testcms2.h>
#include <cstdint>


// Include the test harness header that provides the C API used by the focal method.
// This header is expected to declare: cmsContext, cmsUInt32Number, DebugMalloc,
// DebugFree, DebugRealloc, and any relevant typedefs used by the library.
extern "C" {
}

// Local test framework utilities
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple assertion helper for test results
#define ASSERT(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "Test failure: " << (msg) << std::endl; \
    } \
} while(0)

/*
Test 1: DebugRealloc with Ptr == NULL should allocate a new block and return it.
This covers the true branch of the Ptr == NULL condition.
*/
bool test_DebugRealloc_NullPtr_Returns_NewPtr() {
    cmsContext ctx = (cmsContext)0;
    cmsUInt32Number newSize = 32;

    // Ptr is NULL; DebugRealloc should allocate and return a new block
    void* newPtr = DebugRealloc(ctx, nullptr, newSize);

    ASSERT(newPtr != nullptr, "DebugRealloc with NULL Ptr should return a valid new pointer (non-NULL).");

    // Cleanup
    if (newPtr != nullptr)
        DebugFree(ctx, newPtr);

    return g_failed_tests == 0; // success if no failures recorded so far
}

/*
Test 2: DebugRealloc when NewSize > original size.
- Allocate an original block (size = oldSize), fill with a known pattern.
- Realloc to a larger size (newSize).
- The function should copy min(oldSize, newSize) bytes from the old block
  into the new block.
- The new block pointer should be different from the old one.
This validates the copying behavior for the 'greater' path.
*/
bool test_DebugRealloc_Copy_GreaterSize() {
    cmsContext ctx = (cmsContext)0;
    cmsUInt32Number oldSize = 32;
    cmsUInt32Number newSize = 64;

    // Allocate original block and fill with known pattern
    void* oldPtr = DebugMalloc(ctx, oldSize);
    ASSERT(oldPtr != nullptr, "DebugMalloc for original block should not return NULL.");

    unsigned char* oldBytes = reinterpret_cast<unsigned char*>(oldPtr);
    for (cmsUInt32Number i = 0; i < oldSize; ++i) {
        oldBytes[i] = static_cast<unsigned char>(i & 0xFF);
    }

    // Keep a host copy of the original data for verification
    std::vector<unsigned char> hostPattern(oldSize);
    std::memcpy(hostPattern.data(), oldPtr, oldSize);

    // Perform realloc
    void* newPtr = DebugRealloc(ctx, oldPtr, newSize);
    ASSERT(newPtr != nullptr, "DebugRealloc should return a non-NULL pointer when expanding.");

    // Verify that the new block is different from the old one
    ASSERT(newPtr != oldPtr, "DebugRealloc should return a different pointer when Ptr is non-NULL.");

    // The copied size should be min(oldSize, newSize) according to the implementation
    cmsUInt32Number copied = (oldSize < newSize) ? oldSize : newSize;
    int compareRes = std::memcmp(newPtr, hostPattern.data(), copied);
    ASSERT(compareRes == 0, "DebugRealloc should copy the old data into the new block for the overlapping range.");

    // Cleanup
    if (newPtr != nullptr)
        DebugFree(ctx, newPtr);

    return g_failed_tests == 0;
}

/*
Test 3: DebugRealloc when NewSize < original size.
- Allocate an original block (size = oldSize), fill with a known pattern.
- Realloc to a smaller size (newSize).
- The function should copy min(oldSize, newSize) bytes from the old block
  into the new block (i.e., first newSize bytes).
- The new block pointer should be different from the old one.
This validates the copying behavior for the 'smaller' path.
*/
bool test_DebugRealloc_Copy_SmallerSize() {
    cmsContext ctx = (cmsContext)0;
    cmsUInt32Number oldSize = 64;
    cmsUInt32Number newSize = 16;

    // Allocate original block and fill with known pattern
    void* oldPtr = DebugMalloc(ctx, oldSize);
    ASSERT(oldPtr != nullptr, "DebugMalloc for original block should not return NULL.");

    unsigned char* oldBytes = reinterpret_cast<unsigned char*>(oldPtr);
    for (cmsUInt32Number i = 0; i < oldSize; ++i) {
        oldBytes[i] = static_cast<unsigned char>((i * 3) & 0xFF);
    }

    // Keep a host copy of the original data for verification
    std::vector<unsigned char> hostPattern(oldSize);
    std::memcpy(hostPattern.data(), oldPtr, oldSize);

    // Perform realloc to a smaller size
    void* newPtr = DebugRealloc(ctx, oldPtr, newSize);
    ASSERT(newPtr != nullptr, "DebugRealloc should return a non-NULL pointer when shrinking.");

    // Verify that the copied region matches the first newSize bytes of the original data
    cmsUInt32Number copied = (oldSize < newSize) ? oldSize : newSize; // here, newSize < oldSize, so copied == newSize
    int compareRes = std::memcmp(newPtr, hostPattern.data(), copied);
    ASSERT(compareRes == 0, "DebugRealloc should copy the first NewSize bytes from the original data when shrinking.");

    // Ensure new pointer is different from old
    ASSERT(newPtr != oldPtr, "DebugRealloc should return a different pointer when Ptr is non-NULL.");

    // Cleanup
    if (newPtr != nullptr)
        DebugFree(ctx, newPtr);

    return g_failed_tests == 0;
}

// Entry point of the test suite
int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "Starting DebugRealloc unit tests (no GTest).\n";

    // Run tests
    bool t1 = test_DebugRealloc_NullPtr_Returns_NewPtr();
    bool t2 = test_DebugRealloc_Copy_GreaterSize();
    bool t3 = test_DebugRealloc_Copy_SmallerSize();

    // Report summary
    std::cout << "DebugRealloc tests completed. "
              << "Total: " << g_total_tests
              << ", Failures: " << g_failed_tests << "\n";

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}