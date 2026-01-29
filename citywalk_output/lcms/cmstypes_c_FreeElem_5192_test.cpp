// Test suite for FreeElem(_cmsDICelem* e) in cmstypes.c
// This test uses a lightweight, custom test harness (no GoogleTest) and relies on
// a stubbed _cmsFree to observe correct behavior without requiring the full
// memory management implementation from the real lcms2 library.
// The tests focus on the conditional branches inside FreeElem and verify that
// Offsets and Sizes pointers are NULL'ed after FreeElem, while also recording
// how many times and with which pointers _cmsFree would be invoked.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// Include the internal cms types header to obtain the _cmsDICelem type.
// This header is expected to be available in the environment where the focal
// method cmstypes.c is compiled.
extern "C" {
}

// Declare the focal method from cmstypes.c so we can call it in tests
// Assuming FreeElem uses C linkage in the original file.
extern "C" void FreeElem(_cmsDICelem* e);

// Stubbed _cmsFree to intercept calls instead of using the real allocator.
// We provide C linkage for compatibility with FreeElem's call site.
static void* freedPtrsObserved[64];
static int freedCountObserved = 0;

extern "C" void _cmsFree(cmsContext ContextID, void* ptr)
{
    // Record every pointer passed to _cmsFree for verification.
    if (freedCountObserved < (int)(sizeof(freedPtrsObserved) / sizeof(freedPtrsObserved[0])))
    {
        freedPtrsObserved[freedCountObserved++] = ptr;
    }
    // Do not actually free memory; we only observe calls to this function.
}

// Helper to reset observation state before each test
static void reset_observation()
{
    freedCountObserved = 0;
    for (size_t i = 0; i < sizeof(freedPtrsObserved) / sizeof(freedPtrsObserved[0]); ++i)
        freedPtrsObserved[i] = nullptr;
}

// Simple test framework macros (non-terminating assertions)
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { \
    std::fprintf(stderr, "Assertion failed: %s\n", (msg)); \
    return false; \
} } while(0)

#define ASSERT_EQ(a, b, msg) do { if(!((a) == (b))) { \
    std::fprintf(stderr, "Assertion failed: %s (expected %lld, got %lld)\n", (msg), \
        static_cast<long long>(b), static_cast<long long>(a)); \
    return false; \
} } while(0)


// Test 1: Both Offsets and Sizes are non-NULL -> expect two _cmsFree calls and both pointers NULLified
static bool test_FreeElem_bothNonNull()
{
    reset_observation();

    _cmsDICelem e;
    std::memset(&e, 0, sizeof(e));

    // Populate with dummy non-NULL pointers
    void* off = std::malloc(32);
    void* siz = std::malloc(64);
    // If malloc failed, fail fast
    if (off == nullptr || siz == nullptr) {
        if (off) std::free(off);
        if (siz) std::free(siz);
        std::fprintf(stderr, "Test setup failed: malloc failed\n");
        return false;
    }

    // Populate required fields
    e.ContextID = (cmsContext)0x1234;  // arbitrary context
    e.Offsets = off;
    e.Sizes   = siz;

    // Call the focal method
    FreeElem(&e);

    // After FreeElem, both pointers should be NULL
    ASSERT_TRUE(e.Offsets == nullptr && e.Sizes == nullptr, "Both Offsets and Sizes must be NULL after FreeElem");

    // We should have observed two frees with the original pointers (order may vary)
    ASSERT_TRUE(freedCountObserved == 2, "Expected two calls to _cmsFree when both elements are non-NULL");

    bool order_ok = ((freedPtrsObserved[0] == off && freedPtrsObserved[1] == siz) ||
                     (freedPtrsObserved[0] == siz && freedPtrsObserved[1] == off));
    ASSERT_TRUE(order_ok, "Observed freed pointers must match the original Offsets and Sizes order (any order acceptable)");

    // Cleanup any remaining allocated memory (should be NULLified by FreeElem, but just in case)
    // Note: If FreeElem did NULL both, these would be the original pointers; free them now to avoid leaks.
    if (off) std::free(off);
    if (siz) std::free(siz);

    return true;
}

// Test 2: Offsets non-NULL, Sizes NULL -> expect one _cmsFree call and both pointers NULLified
static bool test_FreeElem_onlyOffsets()
{
    reset_observation();

    _cmsDICelem e;
    std::memset(&e, 0, sizeof(e));

    void* off = std::malloc(24);
    if (off == nullptr) {
        std::fprintf(stderr, "Test setup failed: malloc failed\n");
        return false;
    }

    e.ContextID = (cmsContext)0xABCD;
    e.Offsets = off;
    e.Sizes   = nullptr;

    FreeElem(&e);

    ASSERT_TRUE(e.Offsets == nullptr && e.Sizes == nullptr, "Both Offsets and Sizes must be NULL after FreeElem");

    // Only one free should have occurred for Offsets
    ASSERT_TRUE(freedCountObserved == 1, "Expected one _cmsFree call when only Offsets is non-NULL");

    ASSERT_TRUE(freedPtrsObserved[0] == off, "Observed freed pointer must match the original Offsets pointer");

    std::free(off);
    return true;
}

// Test 3: Sizes non-NULL, Offsets NULL -> expect one _cmsFree call and both pointers NULLified
static bool test_FreeElem_onlySizes()
{
    reset_observation();

    _cmsDICelem e;
    std::memset(&e, 0, sizeof(e));

    void* siz = std::malloc(40);
    if (siz == nullptr) {
        std::fprintf(stderr, "Test setup failed: malloc failed\n");
        return false;
    }

    e.ContextID = (cmsContext)0x0FED;
    e.Offsets = nullptr;
    e.Sizes   = siz;

    FreeElem(&e);

    ASSERT_TRUE(e.Offsets == nullptr && e.Sizes == nullptr, "Both Offsets and Sizes must be NULL after FreeElem");

    // Only one free should have occurred for Sizes
    ASSERT_TRUE(freedCountObserved == 1, "Expected one _cmsFree call when only Sizes is non-NULL");
    ASSERT_TRUE(freedPtrsObserved[0] == siz, "Observed freed pointer must match the original Sizes pointer");

    std::free(siz);
    return true;
}

// Test 4: Both Offsets and Sizes are NULL -> ensure no _cmsFree calls and pointers remain NULL
static bool test_FreeElem_bothNull()
{
    reset_observation();

    _cmsDICelem e;
    std::memset(&e, 0, sizeof(e));

    e.ContextID = (cmsContext)0x0; // arbitrary

    e.Offsets = nullptr;
    e.Sizes   = nullptr;

    FreeElem(&e);

    ASSERT_TRUE(e.Offsets == nullptr && e.Sizes == nullptr, "Both Offsets and Sizes must remain NULL when already NULL");
    ASSERT_TRUE(freedCountObserved == 0, "No _cmsFree should be called when both pointers are NULL");
    return true;
}


// Entry point for running tests
int main()
{
    bool all_passed = true;

    // Run tests in sequence; each test returns true on success
    if (!test_FreeElem_bothNonNull()) {
        std::fprintf(stderr, "Test 1 (both non-NULL) FAILED\n");
        all_passed = false;
    } else {
        std::printf("Test 1 (both non-NULL) PASSED\n");
    }

    if (!test_FreeElem_onlyOffsets()) {
        std::fprintf(stderr, "Test 2 (only Offsets non-NULL) FAILED\n");
        all_passed = false;
    } else {
        std::printf("Test 2 (only Offsets non-NULL) PASSED\n");
    }

    if (!test_FreeElem_onlySizes()) {
        std::fprintf(stderr, "Test 3 (only Sizes non-NULL) FAILED\n");
        all_passed = false;
    } else {
        std::printf("Test 3 (only Sizes non-NULL) PASSED\n");
    }

    if (!test_FreeElem_bothNull()) {
        std::fprintf(stderr, "Test 4 (both NULL) FAILED\n");
        all_passed = false;
    } else {
        std::printf("Test 4 (both NULL) PASSED\n");
    }

    if (all_passed) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        std::printf("SOME TESTS FAILED\n");
        return 1;
    }
}