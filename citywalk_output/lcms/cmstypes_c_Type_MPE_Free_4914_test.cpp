// Unit test suite for Type_MPE_Free (cmstypes.c) using a lightweight C++11 harness.
// This test mocks cmsPipelineFree to verify that Type_MPE_Free correctly calls
// cmsPipelineFree with the provided Ptr, both for null and non-null pointers.
// No GTest is used; tests are executed from main with non-terminating assertions.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstdlib>


// Domain knowledge notes embedded as comments (CKN):
// - Focal method: Type_MPE_Free(struct _cms_typehandler_struct* self, void *Ptr)
// - Dependency: cmsPipelineFree(cmsPipeline*)
// - Pointer contract: Ptr is cast to cmsPipeline* and passed to cmsPipelineFree
// - Self parameter is unused in the current implementation
// - Unit tests should cover null and non-null Ptr paths

// Forward declarations to match the project types without pulling in the full header.
// We keep types incomplete to avoid dependencies on internal layouts.
typedef struct cmsPipeline cmsPipeline;                 // Forward declare opaque pipeline type
typedef struct _cms_typehandler_struct _cms_typehandler_struct; // Forward declare typehandler struct

extern "C" {
    // Prototype of the function under test (as it would appear in the C source)
    void Type_MPE_Free(struct _cms_typehandler_struct* self, void *Ptr);

    // Mockable prototype: the real cmsPipelineFree will be intercepted by a test mock.
    void cmsPipelineFree(cmsPipeline* p);
}

// Global hook to capture the argument passed to cmsPipelineFree by Type_MPE_Free.
// It is reset for each test to verify correct calls.
static void* g_lastFreedPtr = nullptr;

// Test harness counters (non-terminating assertions style)
static int g_testTotal = 0;
static int g_testPass = 0;
static int g_testFail = 0;

// Lightweight assertion macro that does not abort on failure (non-terminating)
#define TEST_ASSERT(cond, msg) do { \
    ++g_testTotal; \
    if (cond) { ++g_testPass; } \
    else { \
        ++g_testFail; \
        std::cerr << "ASSERT FAILED: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while (0)

// Test 1: When Ptr is nullptr, cmsPipelineFree should be called with nullptr.
void test_Type_MPE_Free_NullPtr() {
    // Arrange
    g_lastFreedPtr = reinterpret_cast<void*>(0xDEADBEEF); // arbitrary non-null to ensure change detection
    // Act
    Type_MPE_Free(nullptr, nullptr);
    // Assert
    TEST_ASSERT(g_lastFreedPtr == nullptr,
                "cmsPipelineFree should be called with nullptr when Ptr is nullptr");
}

// Test 2: When Ptr is non-null, cmsPipelineFree should be called with the same pointer.
void test_Type_MPE_Free_NonNullPtr() {
    // Arrange
    g_lastFreedPtr = nullptr;
    void* p = std::malloc(64);
    TEST_ASSERT(p != nullptr, "Memory allocation for test Ptr failed");
    if (!p) return; // safety

    // Act
    Type_MPE_Free(nullptr, p);

    // Assert
    TEST_ASSERT(g_lastFreedPtr == p,
                "cmsPipelineFree should be called with the exact Ptr passed to Type_MPE_Free");

    // Cleanup
    std::free(p);
}

// Mock implementation: capture pointer argument passed to cmsPipelineFree.
extern "C" void cmsPipelineFree(cmsPipeline* p) {
    g_lastFreedPtr = reinterpret_cast<void*>(p);
}

// Main entry: run all tests and report summary.
// Per instructions: call test methods from main and provide explanatory comments.
// We rely on non-terminating assertions to allow all tests to execute.
int main() {
    std::cout << "Starting tests for Type_MPE_Free...\n";

    // Run tests
    test_Type_MPE_Free_NullPtr();
    test_Type_MPE_Free_NonNullPtr();

    // Summary
    std::cout << "Test summary: " << g_testPass << " passed, "
              << g_testFail << " failed, "
              << g_testTotal << " total assertions." << std::endl;

    if (g_testFail > 0) {
        std::cerr << "Some tests failed. See details above." << std::endl;
        return 1;
    }
    std::cout << "All tests completed successfully." << std::endl;
    return 0;
}