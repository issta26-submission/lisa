// Test suite for the focal method: FloatCMYKAlloc in fast_float_cmyk.c
// This test is implemented in C++11 without using GoogleTest.
// It relies on providing a mock _cmsMallocZero allocator to simulate allocation
// success/failure and to inspect the allocation behavior of FloatCMYKAlloc.

// The production code is assumed to be compiled as part of this translation unit
// by including the C source file directly (extern "C" linkage) so that the
// FloatCMYKData type and its fields ContextID and p are available for testing.

#include <cstddef>
#include <iostream>
#include <fast_float_internal.h>
#include <cstdlib>


// Bring in the necessary type definitions and the function under test.
// We include the internal header first to ensure cmsContext, cmsInterpParams, and
// the FloatCMYKData type are declared, then include the C source to pull in
// the focal function and its related types.
extern "C" {
}

// Prototype for the allocator hook used by the focal function.
// We declare it here so that the linker can resolve the symbol _cmsMallocZero
// when FloatCMYKAlloc calls it. The actual implementation is provided below
// to allow controlled success/failure behavior in tests.
extern "C" void* _cmsMallocZero(cmsContext ContextID, size_t size);

// Global flags to control allocation behavior in the mock allocator.
static bool g_failMalloc = false;
static size_t g_lastMallocRequestedSize = 0;

// Mock implementation of _cmsMallocZero to control test behavior.
// - Records the requested allocation size for verification.
// - Returns NULL when g_failMalloc is true to simulate allocation failure.
// - Otherwise uses calloc to yield a zero-initialized block (as real code would).
extern "C" void* _cmsMallocZero(cmsContext ContextID, size_t size) {
    (void)ContextID; // ContextID is not used by the mock, but kept for signature compatibility
    g_lastMallocRequestedSize = size;
    if (g_failMalloc) {
        return NULL;
    }
    void* p = std::calloc(1, size); // zero-initialized block
    return p;
}

// Lightweight assertion helpers: non-terminating, print on failure but continue.
#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while(0)

#define EXPECT_EQ(a, b) \
    do { \
        if(!((a) == (b))) { \
            std::cerr << "EXPECT_EQ failed: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ") at " \
                      << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while(0)

#define EXPECT_NEQ(a, b) \
    do { \
        if((a) == (b)) { \
            std::cerr << "EXPECT_NEQ failed: " << #a << " (" << (a) << ") == " << #b << " (" << (b) << ") at " \
                      << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while(0)

// Forward declarations to access the production data structures.
// FloatCMYKAlloc returns a pointer to FloatCMYKData. The test relies on inspecting
// its ContextID and p fields to verify correct assignment.
static void test_FloatCMYKAlloc_successful_allocation();
static void test_FloatCMYKAlloc_allocation_failure();
static void test_FloatCMYKAlloc_allocation_uses_correct_size();

// Entry point for the test suite.
int main() {
    std::cout << "Starting FloatCMYKAlloc tests...\n";

    // Run tests
    test_FloatCMYKAlloc_successful_allocation();
    test_FloatCMYKAlloc_allocation_failure();
    test_FloatCMYKAlloc_allocation_uses_correct_size();

    std::cout << "FloatCMYKAlloc tests completed.\n";
    return 0;
}

// Test 1: Successful allocation path
// - When _cmsMallocZero returns a non-NULL pointer, FloatCMYKAlloc should return
//   a non-NULL pointer and set the ContextID and p fields correctly.
static void test_FloatCMYKAlloc_successful_allocation() {
    // Arrange
    g_failMalloc = false;                    // Ensure allocator succeeds
    g_lastMallocRequestedSize = 0;           // Reset tracking
    cmsInterpParams* testP = reinterpret_cast<cmsInterpParams*>(0xDEADBEEF);
    cmsContext testContext = 123; // Arbitrary context ID; type defined by library

    // Act
    FloatCMYKData* fd = FloatCMYKAlloc(testContext, testP);

    // Assert
    EXPECT_TRUE(fd != nullptr);
    if (fd != nullptr) {
        EXPECT_EQ(fd->ContextID, testContext);
        EXPECT_EQ(fd->p, testP);
        // Optional: verify that the allocated block was zero-initialized
        // beyond the fields we set (ContextID and p). We avoid guessing other
        // internal fields to keep tests stable; focusing on core behavior.
        free(fd);
    }
}

// Test 2: Allocation failure path
// - When _cmsMallocZero returns NULL, FloatCMYKAlloc must return NULL.
static void test_FloatCMYKAlloc_allocation_failure() {
    // Arrange
    g_failMalloc = true;                     // Force allocator to fail
    g_lastMallocRequestedSize = 0;
    cmsInterpParams* testP = nullptr;
    cmsContext testContext = 999;

    // Act
    FloatCMYKData* fd = FloatCMYKAlloc(testContext, testP);

    // Assert
    EXPECT_TRUE(fd == nullptr);
}

// Test 3: Allocation size correctness
// - Verify that _cmsMallocZero is invoked with the correct sizeof(FloatCMYKData).
static void test_FloatCMYKAlloc_allocation_uses_correct_size() {
    // Arrange
    g_failMalloc = false;
    g_lastMallocRequestedSize = 0;
    cmsInterpParams* testP = nullptr;
    cmsContext testContext = 42;

    // Act
    FloatCMYKData* fd = FloatCMYKAlloc(testContext, testP);

    // Assert
    EXPECT_TRUE(fd != nullptr);
    if (fd != nullptr) {
        // The size requested should match the size of the actual FloatCMYKData type
        // as defined by the production code.
        EXPECT_EQ(g_lastMallocRequestedSize, sizeof(FloatCMYKData));
        free(fd);
    } else {
        // If allocation failed unexpectedly, also record a failure for this test.
        EXPECT_TRUE(false);
    }
}

// Note: The FloatCMYKAlloc implementation allocates a zero-initialized block of size
// sizeof(FloatCMYKData) via _cmsMallocZero, then assigns ContextID and p.
// Our tests focus on allocation success/failure and basic field assignment.
// We deliberately do not attempt to mutate private/private-semantics beyond what
// the focal method exposes (ContextID and p).

// End of test suite.