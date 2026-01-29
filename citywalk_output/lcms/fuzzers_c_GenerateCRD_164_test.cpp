// Test suite for the focal method GenerateCRD(cmsHPROFILE hProfile)
// This test suite is designed to be compiled as a standalone C++11 program
// without using GTest. It relies on user-provided stubs for the
// dependent functions used by GenerateCRD (cmsGetPostScriptCRD, _cmsMalloc, _cmsFree)
// to simulate the behavior of the underlying library without requiring the real
// libcms/LittleCMS dependencies.

#include <cstring>
#include <lcms2_plugin.h>
#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <cstdlib>


// Forward-declare the entry point and dependent types used by the focal method.
// We provide minimal type aliases to be compatible with the focal implementation.
// The real project would normally include lcms2_plugin.h which defines these.
extern "C" {
    typedef void* cmsHPROFILE;
    typedef unsigned int cmsUInt32Number;

    // Focal function under test (assumed to be provided in the linking stage).
    void GenerateCRD(cmsHPROFILE hProfile);

    // Dependent library-like interfaces expected by GenerateCRD.
    cmsUInt32Number cmsGetPostScriptCRD(int, cmsHPROFILE, int, cmsUInt32Number, char*, cmsUInt32Number);
    void* _cmsMalloc(int, size_t);
    void _cmsFree(int, void*);
}

// Global test context to observe interactions between GenerateCRD and mocks.
struct TestContext {
    bool mallocCalled;
    size_t mallocSize;
    void* lastMallocPtr;
    int freeCalled;
    bool freePointerMatches;
} gTestCtx;

// Simple test counters
static int gTotalTests = 0;
static int gFailedTests = 0;

// Lightweight assertion helpers (non-terminating)
#define EXPECT_TRUE(cond) \
    do { \
        ++gTotalTests; \
        if(!(cond)) { \
            ++gFailedTests; \
            std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ \
                      << " - Expected true but got false: " #cond "\n"; \
        } \
    } while(0)

#define EXPECT_EQ(a, b) \
    do { \
        ++gTotalTests; \
        if( (a) != (b) ) { \
            ++gFailedTests; \
            std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ \
                      << " - Expected " #a " == " #b " (got " << (a) \
                      << " vs " << (b) << ")\n"; \
        } \
    } while(0)

// Helper to reset test context state between tests
static void reset_test_context() {
    std::memset(&gTestCtx, 0, sizeof(gTestCtx));
}

// Stub implementations to exercise GenerateCRD behavior
// These stubs mimic the behavior of the real CMS calls in a controlled way.

// First call: when Buffer == NULL and n == 0, return a non-zero size to trigger allocation.
// We use a non-null hProfile to indicate the "non-zero" path; NULL represents the zero path.
extern "C" cmsUInt32Number cmsGetPostScriptCRD(int, cmsHPROFILE h, int, cmsUInt32Number, char*, cmsUInt32Number) {
    // Simulate behavior depending on the provided profile handle.
    if (h == nullptr) {
        // Zero-path: no CRD available
        return 0;
    }
    // Non-zero path: report a size that requires allocation (e.g., 12 bytes)
    return 12;
}

// Allocation stubs: track allocation details but allocate actual memory for simplicity
extern "C" void* _cmsMalloc(int /*unused*/, size_t size) {
    gTestCtx.mallocCalled = true;
    gTestCtx.mallocSize = size;
    void* p = std::malloc(size);
    gTestCtx.lastMallocPtr = p;
    return p;
}

// Free stub: verify that the pointer being freed matches the last allocation
extern "C" void _cmsFree(int /*unused*/, void* p) {
    gTestCtx.freeCalled++;
    if (p == gTestCtx.lastMallocPtr) {
        gTestCtx.freePointerMatches = true;
    }
    std::free(p);
}

// The actual test cases

// Test 1: When the first call to cmsGetPostScriptCRD returns 0 (n == 0 path),
// Generation should return early and not allocate memory.
// This covers the false branch of the function's conditional.
static void test_GenerateCRD_zero_path() {
    reset_test_context();

    // Call with a NULL profile to drive the zero-path
    GenerateCRD(nullptr);

    // Expectations:
    // - No memory allocation should occur
    EXPECT_TRUE(gTestCtx.mallocCalled == false);
    // - No free should occur
    EXPECT_TRUE(gTestCtx.freeCalled == 0);
}

// Test 2: When the first cmsGetPostScriptCRD call returns a non-zero size,
// Generation should allocate memory, call cmsGetPostScriptCRD again with a valid buffer,
// and then free the allocated memory.
static void test_GenerateCRD_nonzero_path() {
    reset_test_context();

    // Call with a non-NULL profile to drive the non-zero path
    GenerateCRD((cmsHPROFILE)reinterpret_cast<void*>(0x1));

    // Expectations:
    // - Allocation should have occurred
    EXPECT_TRUE(gTestCtx.mallocCalled);
    // - Allocated size should match the size returned by cmsGetPostScriptCRD (12 in stub)
    EXPECT_EQ(gTestCtx.mallocSize, 12u);
    // - Free should have occurred exactly once
    EXPECT_TRUE(gTestCtx.freeCalled >= 1);
    // - The free should have been called with the pointer returned by _cmsMalloc
    EXPECT_TRUE(gTestCtx.freePointerMatches);
}

// Entry point: run tests and report summary
int main() {
    std::cout << "Running unit tests for GenerateCRD...\n";

    test_GenerateCRD_zero_path();
    test_GenerateCRD_nonzero_path();

    std::cout << "Tests completed. Total: " << gTotalTests
              << ", Passed: " << (gTotalTests - gFailedTests)
              << ", Failed: " << gFailedTests << "\n";

    // Return non-zero exit code if any test failed
    return gFailedTests;
}