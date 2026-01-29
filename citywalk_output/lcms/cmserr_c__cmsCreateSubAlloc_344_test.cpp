// Test suite for the focal method: _cmsCreateSubAlloc in cmserr.c
// This test is designed to run without Google Test (GTest) and uses a lightweight
// in-process test harness with non-terminating checks (without std::terminate).
// The test provides stubbed memory allocation behavior to control the branches
// of _cmsCreateSubAlloc (success, allocation failure, and chunk allocation failure).
// The tests rely on the actual cmserr.c implementation linked into the binary.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Include internal CMS/LittleCMS definitions to access required types.
// This header is expected to be present in the project under test.

// Forward declaration of the focal function under test from cmserr.c.
// We declare the prototype here to allow direct invocation from the test harness.
extern "C" _cmsSubAllocator* _cmsCreateSubAlloc(cmsContext ContextID, cmsUInt32Number Initial);

// We also need the memory allocation helper the focal function uses.
// We provide a controlled, testable implementation by overriding the real one
// via linkage (assuming _cmsMallocZero and _cmsFree are externally visible).
extern "C" {
    void* _cmsMallocZero(cmsContext ContextID, cmsUInt32Number size);
    void  _cmsFree(cmsContext ContextID, void* Ptr);
}

// Global state to control the behavior of the allocation stub (_cmsMallocZero).
static bool g_firstMallocZeroShouldFail  = false;
static bool g_secondMallocZeroShouldFail = false;
static int  g_mallocZeroCallCount = 0;

// Lightweight assertion/diagnostic helper to accumulate test results
static int gTestsRun = 0;
static int gTestsFailed = 0;

// Non-terminating CHECK macro: prints diagnostic messages but does not abort tests
#define CHECK(cond, msg) do {                                 \
    ++gTestsRun;                                               \
    if (!(cond)) {                                             \
        ++gTestsFailed;                                        \
        std::cerr << "[CHECK FAILED] " << (msg)             \
                  << " (file " << __FILE__ << ", line " << __LINE__ << ")\n"; \
    }                                                        \
} while (0)

// Test helper: reset shared test state
static void resetTestState() {
    g_firstMallocZeroShouldFail = false;
    g_secondMallocZeroShouldFail = false;
    g_mallocZeroCallCount = 0;
}

// Stubbed _cmsMallocZero to control allocations used by _cmsCreateSubAlloc and its chunk.
// This function intentionally uses malloc to simulate real allocations while
// allowing tests to drive failure scenarios.
extern "C" void* _cmsMallocZero(cmsContext ContextID, cmsUInt32Number size)
{
    (void)ContextID; // Context is not used in the stub
    ++g_mallocZeroCallCount;

    // First allocation: sub allocator container
    if (g_mallocZeroCallCount == 1 && g_firstMallocZeroShouldFail) {
        return NULL;
    }

    // Second allocation: likely inside _cmsCreateSubAllocChunk for the chunk
    if (g_mallocZeroCallCount == 2 && g_secondMallocZeroShouldFail) {
        return NULL;
    }

    // Default behavior: allocate memory
    void* p = std::malloc(static_cast<size_t>(size));
    return p;
}

// Stubbed free: simply release memory
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr)
{
    (void)ContextID;
    std::free(Ptr);
}

// ----------------- Unit Tests -----------------

// Test 1: Successful creation path
// - Expect _cmsCreateSubAlloc to return a non-NULL sub allocator.
// - Expect sub->ContextID to be equal to the provided ContextID.
// - Expect sub->h (the chunk handle) to be non-NULL.
static void test_cmsCreateSubAlloc_success() {
    resetTestState();
    g_firstMallocZeroShouldFail = false;
    g_secondMallocZeroShouldFail = false;
    g_mallocZeroCallCount = 0;

    cmsContext ctx = 1;
    cmsUInt32Number initial = 64;

    _cmsSubAllocator* sub = _cmsCreateSubAlloc(ctx, initial);

    CHECK(sub != NULL, "Expected _cmsCreateSubAlloc to return non-NULL on success");
    if (sub != NULL) {
        // Verify the expected fields were set by the function under test
        CHECK(sub->ContextID == ctx, "sub->ContextID should be equal to input ContextID");
        CHECK(sub->h != NULL, "sub->h (sub-allocator chunk) should be non-NULL on success");
        // Cleanup allocated memory
        _cmsFree(ctx, sub);
    }
}

// Test 2: Failure in the initial allocation of the sub-allocator container
// - Simulate first _cmsMallocZero call failing, which should cause
//   _cmsCreateSubAlloc to return NULL without attempting to allocate the chunk.
static void test_cmsCreateSubAlloc_allocSubFail() {
    resetTestState();
    g_firstMallocZeroShouldFail = true;   // Force failure on the first allocation
    g_secondMallocZeroShouldFail = false;
    g_mallocZeroCallCount = 0;

    cmsContext ctx = 2;
    cmsUInt32Number initial = 32;

    _cmsSubAllocator* sub = _cmsCreateSubAlloc(ctx, initial);

    CHECK(sub == NULL, "Expected NULL when first sub-allocator allocation fails");
    // No cleanup needed since the sub allocation failed and no memory should be leaked.
}

// Test 3: Failure in chunk allocation inside _cmsCreateSubAllocChunk
// - Simulate the second _cmsMallocZero call failing, which should lead to freeing
//   the partially allocated sub-allocator and returning NULL.
static void test_cmsCreateSubAlloc_chunkFail() {
    resetTestState();
    g_firstMallocZeroShouldFail = false;
    g_secondMallocZeroShouldFail = true; // Force failure on the second allocation
    g_mallocZeroCallCount = 0;

    cmsContext ctx = 3;
    cmsUInt32Number initial = 128;

    _cmsSubAllocator* sub = _cmsCreateSubAlloc(ctx, initial);

    CHECK(sub == NULL, "Expected NULL when second allocation (chunk) fails");
}

// ----------------- Test Runner -----------------

static void runAllTests() {
    std::cout << "Running cmsCreateSubAlloc unit tests...\n";
    test_cmsCreateSubAlloc_success();
    test_cmsCreateSubAlloc_allocSubFail();
    test_cmsCreateSubAlloc_chunkFail();
    std::cout << "Tests completed. Ran: " << gTestsRun
              << ", Failures: " << gTestsFailed << "\n";
}

// ----------------- Main -----------------

int main() {
    runAllTests();
    // Return non-zero if any test failed, to integrate cleanly with CI systems
    return (gTestsFailed == 0) ? 0 : 1;
}