// Unit test suite for focal method: MyMtxCreate
// Target: C++11, no GoogleTest. Use a lightweight in-file test harness with non-terminating assertions.
// This test suite mocks the memory allocator _cmsMalloc used by MyMtxCreate and validates
// basic semantics: allocation occurs, nlocks is initialized to 0, and multiple calls yield distinct pointers.

#include <vector>
#include <cstddef>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>


// Domain-specific notes (Candidate Keywords):
// - MyMtxCreate: the focal function under test.
// - MyMtx: the struct allocated by MyMtxCreate, with member nlocks.
// - _cmsMalloc: allocation function used by MyMtxCreate (we mock it).
// - cmsContext: context identifier passed to _cmsMalloc (we model as void* here).
// - nlocks: field initialized to 0 by MyMtxCreate.
// - Context propagation: ensure the id/context passed to _cmsMalloc is recorded.
// - Pointer identity: multiple calls should return distinct pointers.

// Step 1: Basic type and function declarations to link with the focal method.
// We assume cmsContext is a pointer type (as commonly defined in LittleCMS-like projects).
typedef void* cmsContext;

// Forward declarations of the focal function (assumed to be linked from testplugin.c)
extern "C" void* MyMtxCreate(cmsContext id);

// Local test structure mirroring the expected memory layout of MyMtx in tests.
// If the real project defines a different layout, this test assumes the first member is nlocks (int).
struct MyMtx {
    int nlocks;
};

// Step 2: Mocking the allocator _cmsMalloc and a corresponding free function.
// We provide C linkage so the linker resolves to these definitions when MyMtxCreate calls _cmsMalloc.

static std::vector<void*> g_alloc_contexts; // Records all ContextID values passed to _cmsMalloc (in order)
static size_t               g_last_alloc_size = 0; // Last size requested to _cmsMalloc
static std::vector<void*> g_allocated_ptrs;     // Pointers returned by _cmsMalloc (allocated blocks)

// Mock allocation function
extern "C" void* _cmsMalloc(cmsContext ContextID, size_t size) {
    g_alloc_contexts.push_back(ContextID);
    g_last_alloc_size = size;
    void* p = std::malloc(size);
    g_allocated_ptrs.push_back(p);
    return p;
}

// Mock deallocation function (to be used if tests choose to free memory)
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr) {
    // Simple free; we don't strictly need ContextID here for the test, but keep signature
    (void)ContextID; // suppress unused warning
    std::free(Ptr);
}

// Step 3: Lightweight test harness (non-terminating assertions)
static int g_total_asserts = 0;
static int g_failed_asserts = 0;

#define EXPECT_TRUE(cond, desc)                                      \
    do {                                                               \
        ++g_total_asserts;                                             \
        if(!(cond)) {                                                  \
            ++g_failed_asserts;                                        \
            std::cerr << "[FAIL] " << (desc)                          \
                      << " (in file " << __FILE__ << ", line "             \
                      << __LINE__ << ")" << std::endl;               \
        }                                                              \
    } while(0)

#define EXPECT_NOT_NULL(ptr, desc)                                    \
    EXPECT_TRUE((ptr) != nullptr, (desc))

// Step 4: The actual test cases

// Test 1: Basic allocation and initialization
// - Verifies that MyMtxCreate returns a non-null pointer.
// - Verifies that the allocated memory maps to MyMtx and that nlocks is initialized to 0.
// - Verifies that _cmsMalloc was invoked (non-zero allocation size) and that the context was recorded.
// - Cleans up by freeing the allocated memory.
void test_MyMtxCreate_allocates_and_initializes() {
    // Reset mock state for isolated test
    g_alloc_contexts.clear();
    g_allocated_ptrs.clear();
    g_last_alloc_size = 0;
    // Use a non-null fake context
    cmsContext fakeCtx = (cmsContext)0xDEADBEEF;

    void* p = MyMtxCreate(fakeCtx);

    // Assertions
    EXPECT_NOT_NULL(p, "MyMtxCreate should return a non-null pointer");

    // Interpret the returned memory as MyMtx (as used by the implementation)
    MyMtx* mtx = static_cast<MyMtx*>(p);
    EXPECT_TRUE(mtx != nullptr, "Returned pointer should be interpretable as MyMtx");

    // Check the initialized field
    EXPECT_TRUE(mtx->nlocks == 0, "mtx->nlocks should be initialized to 0 by MyMtxCreate");

    // Check allocator behavior
    EXPECT_TRUE(!g_alloc_contexts.empty(), "_cmsMalloc should have been called");
    if (!g_alloc_contexts.empty()) {
        EXPECT_TRUE(g_alloc_contexts[0] == fakeCtx, "First _cmsMalloc ContextID should match input ctx");
    }
    EXPECT_TRUE(g_last_alloc_size > 0, "_cmsMalloc should be called with a non-zero size");

    // Cleanup
    _cmsFree(fakeCtx, p);
}

// Test 2: Multiple allocations should return distinct pointers
// - Calls MyMtxCreate twice with different contexts and ensures distinct memory blocks are returned.
void test_MyMtxCreate_distinct_pointers() {
    g_alloc_contexts.clear();
    g_allocated_ptrs.clear();
    g_last_alloc_size = 0;

    cmsContext ctx1 = (cmsContext)0x1111;
    cmsContext ctx2 = (cmsContext)0x2222;

    void* p1 = MyMtxCreate(ctx1);
    void* p2 = MyMtxCreate(ctx2);

    EXPECT_NOT_NULL(p1, "First allocation should be non-null");
    EXPECT_NOT_NULL(p2, "Second allocation should be non-null");
    EXPECT_TRUE(p1 != p2, "Two successive allocations should yield distinct pointers");

    // Optional: verify contexts were recorded in order
    if (!g_alloc_contexts.empty()) {
        EXPECT_TRUE(g_alloc_contexts[0] == ctx1, "First _cmsMalloc ContextID should be ctx1");
        if (g_alloc_contexts.size() >= 2) {
            EXPECT_TRUE(g_alloc_contexts[1] == ctx2, "Second _cmsMalloc ContextID should be ctx2");
        }
    }

    // Cleanup
    _cmsFree(ctx1, p1);
    _cmsFree(ctx2, p2);
}

// Step 5: Main entry to run all tests and report summary
int main() {
    // Run tests
    test_MyMtxCreate_allocates_and_initializes();
    test_MyMtxCreate_distinct_pointers();

    // Summary
    std::cout << "Test summary: " << g_total_asserts << " assertions, "
              << g_failed_asserts << " failures." << std::endl;

    // Exit code indicates success if no failures
    return (g_failed_asserts == 0) ? 0 : 1;
}

// The code above is designed to be compiled together with the focal implementation (testplugin.c or equivalent).
// It mocks _cmsMalloc to observe allocation size and input context, while validating that MyMtxCreate
// returns a non-null pointer and initializes the nlocks member to zero. It also ensures that multiple
// allocations yield distinct pointers. All assertions are non-terminating to maximize coverage.