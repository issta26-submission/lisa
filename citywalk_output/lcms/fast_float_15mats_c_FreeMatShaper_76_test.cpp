// Test suite for FreeMatShaper function in fast_float_15mats.c
// This test focuses on the behavior of FreeMatShaper(ContextID, Data):
// - When Data is NULL, no call to _cmsFree should occur.
// - When Data is non-NULL, FreeMatShaper should call _cmsFree with p->real_ptr.
// - When p->real_ptr is NULL, _cmsFree should still be invoked with NULL.
// Notes:
// - We provide a minimal mock for _cmsFree to observe its invocation without freeing real resources.
// - We assume XMatShaperData has a member named real_ptr as seen in the focal method.
// - The tests are written for C++11, without using GoogleTest; a lightweight harness is provided.

#include <iostream>
#include <fast_float_internal.h>
#include <cstdlib>


// Domain-specific: provide C linkage for the focal function and any C helpers.
extern "C" {

    // Lightweight, minimal type definitions to compile against FreeMatShaper.
    // We do not rely on internal library headers; we only need the signatures/types used by FreeMatShaper.
    typedef void* cmsContext;

    // Forward declaration of the focal function under test.
    void FreeMatShaper(cmsContext ContextID, void* Data);

    // We also declare the XMatShaperData layout as expected by the focal function.
    // In this test, we assume the first member is real_ptr (void*). This mirrors the access:
    //   XMatShaperData* p = (XMatShaperData*)Data;
    //   p->real_ptr
    typedef struct {
        void* real_ptr;
    } XMatShaperData;
}

// Global variables to capture calls to _cmsFree (the function under test should call this).
static bool g_FreeCalled = false;
static void* g_LastFreedPtr = NULL;
static cmsContext g_LastContext = NULL;

// Mock implementation of the libc/LCMS internal allocator/deallocator hook.
// This overrides the real _cmsFree during tests to observe behavior.
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr)
{
    g_FreeCalled = true;
    g_LastFreedPtr = Ptr;
    g_LastContext = ContextID;
}

// Helpers to reset test state
static void reset_free_call_state()
{
    g_FreeCalled = false;
    g_LastFreedPtr = NULL;
    g_LastContext = NULL;
}

// Test 1: Data is NULL -> FreeMatShaper should not attempt any free operation.
static void test_FreeMatShaper_NullData()
{
    reset_free_call_state();
    cmsContext ctx = (cmsContext)0xDEADBEEF;
    // Call with NULL Data; Expect no call to _cmsFree.
    FreeMatShaper(ctx, NULL);

    if (g_FreeCalled) {
        std::cerr << "[Test 1] Failure: _cmsFree was called when Data is NULL.\n";
    } else {
        std::cout << "[Test 1] Passed: No call to _cmsFree when Data is NULL.\n";
    }
}

// Test 2: Data is non-NULL and real_ptr is non-NULL -> _cmsFree should be called with that pointer.
static void test_FreeMatShaper_NonNullData_RealPtrNonNull()
{
    reset_free_call_state();

    XMatShaperData p;
    void* sentinel = malloc(32); // allocate some memory to be freed
    p.real_ptr = sentinel;

    cmsContext ctx = (cmsContext)0xBEEFCAFE;
    FreeMatShaper(ctx, &p);

    if (!g_FreeCalled) {
        std::cerr << "[Test 2] Failure: _cmsFree was not called for non-NULL Data.\n";
    } else if (g_LastFreedPtr != sentinel) {
        std::cerr << "[Test 2] Failure: _cmsFree was called with incorrect pointer.\n";
    } else if (g_LastContext != ctx) {
        std::cerr << "[Test 2] Failure: _cmsFree was called with incorrect context.\n";
    } else {
        std::cout << "[Test 2] Passed: _cmsFree was invoked with correct real_ptr and context.\n";
    }

    free(sentinel);
}

// Test 3: Data is non-NULL and real_ptr is NULL -> _cmsFree should be invoked with NULL.
static void test_FreeMatShaper_NonNullData_RealPtrNull()
{
    reset_free_call_state();

    XMatShaperData p;
    p.real_ptr = NULL;

    cmsContext ctx = (cmsContext)0x12345678;
    FreeMatShaper(ctx, &p);

    if (!g_FreeCalled) {
        std::cerr << "[Test 3] Failure: _cmsFree was not called when real_ptr is NULL.\n";
    } else if (g_LastFreedPtr != NULL) {
        std::cerr << "[Test 3] Failure: _cmsFree was called with non-NULL pointer while real_ptr is NULL.\n";
    } else if (g_LastContext != ctx) {
        std::cerr << "[Test 3] Failure: _cmsFree was called with incorrect context.\n";
    } else {
        std::cout << "[Test 3] Passed: _cmsFree invoked with NULL real_ptr as expected.\n";
    }
}

// Test 4: Multiple sequential calls with different data blocks to ensure independent behavior.
static void test_FreeMatShaper_MultipleCalls_DifferentData()
{
    reset_free_call_state();

    XMatShaperData p1;
    XMatShaperData p2;

    void* t1 = malloc(16);
    void* t2 = malloc(16);
    p1.real_ptr = t1;
    p2.real_ptr = t2;

    // First call
    FreeMatShaper((cmsContext)0x1001, &p1);
    if (g_LastFreedPtr != t1) {
        std::cerr << "[Test 4] Failure: First call freed wrong pointer.\n";
        // Cleanup and continue to next test gracefully
    }

    // Second call with different data
    FreeMatShaper((cmsContext)0x2002, &p2);
    if (g_LastFreedPtr != t2) {
        std::cerr << "[Test 4] Failure: Second call freed wrong pointer.\n";
    } else if (g_LastContext != (cmsContext)0x2002) {
        std::cerr << "[Test 4] Failure: Second call context incorrect.\n";
    } else {
        std::cout << "[Test 4] Passed: Multiple calls maintained correct per-call behavior.\n";
    }

    free(t1);
    free(t2);
}

// Entry point: run all tests
int main()
{
    std::cout << "Starting FreeMatShaper unit tests...\n";

    test_FreeMatShaper_NullData();
    test_FreeMatShaper_NonNullData_RealPtrNonNull();
    test_FreeMatShaper_NonNullData_RealPtrNull();
    test_FreeMatShaper_MultipleCalls_DifferentData();

    // Basic summary
    // We do not aggregate failure counts here since we print per-test result.
    std::cout << "Unit tests completed.\n";
    return 0;
}