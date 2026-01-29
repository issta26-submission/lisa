// Unit test suite for Performance8free using a custom lightweight test harness.
// This test mocks the underlying memory free function _cmsFree to verify that
// Performance8free correctly forwards its arguments to the free routine.
// The test is designed to compile with a C/C++03/C++11 compatible toolchain and
// does not rely on GTest. It uses only the C++ standard library.

#include <iostream>
#include <fast_float_internal.h>


// Domain assumptions (adapted for testing without the real library headers).
using cmsContext = void*; // Mirror the common cmsContext typedef as a void* for testing.

// Forward declaration of the function under test (C linkage to match the real symbol).
extern "C" void Performance8free(cmsContext ContextID, void* ptr);

// Mock storage to observe calls to _cmsFree
static cmsContext g_lastContext = nullptr;
static void*      g_lastPtr = nullptr;
static int        g_callCount = 0;

// Mock implementation of the free routine expected by Performance8free.
// This function is defined with C linkage so the linker resolves correctly to calls
// from the C implementation of Performance8free.
extern "C" void _cmsFree(cmsContext ContextID, void* ptr)
{
    g_lastContext = ContextID;
    g_lastPtr = ptr;
    ++g_callCount;
}

// Minimal test harness helpers
static void resetMockState()
{
    g_lastContext = nullptr;
    g_lastPtr = nullptr;
    g_callCount = 0;
}

// Test 1: Verify that Performance8free forwards the exact ContextID and ptr to _cmsFree.
bool test_Performance8free_CallsCmsFree_WithGivenParams()
{
    resetMockState();

    cmsContext ctx = (cmsContext)0x12345678;
    void* ptr = (void*)0xDEADBEEF;

    // Call the function under test
    Performance8free(ctx, ptr);

    // Validate that _cmsFree was called exactly once with the same arguments
    bool ok = (g_callCount == 1) && (g_lastContext == ctx) && (g_lastPtr == ptr);
    if (!ok) {
        std::cerr << "[FAIL] test_Performance8free_CallsCmsFree_WithGivenParams: "
                  << "Expected callCount=1, lastContext=" << ctx
                  << ", lastPtr=" << ptr << " but got callCount=" << g_callCount
                  << ", lastContext=" << g_lastContext
                  << ", lastPtr=" << g_lastPtr << std::endl;
    } else {
        std::cout << "[PASS] test_Performance8free_CallsCmsFree_WithGivenParams" << std::endl;
    }
    return ok;
}

// Test 2: Verify behavior when ptr is null. The function should still forward correctly.
bool test_Performance8free_NullPtr()
{
    resetMockState();

    cmsContext ctx = (cmsContext)0x0CAFEBABE;
    void* ptr = nullptr;

    Performance8free(ctx, ptr);

    bool ok = (g_callCount == 1) && (g_lastContext == ctx) && (g_lastPtr == nullptr);
    if (!ok) {
        std::cerr << "[FAIL] test_Performance8free_NullPtr: "
                  << "Expected callCount=1, lastContext=" << ctx
                  << ", lastPtr=nullptr but got callCount=" << g_callCount
                  << ", lastContext=" << g_lastContext
                  << ", lastPtr=" << g_lastPtr << std::endl;
    } else {
        std::cout << "[PASS] test_Performance8free_NullPtr" << std::endl;
    }
    return ok;
}

// Test 3: Verify multiple successive calls are handled and the latest call is observed.
// This ensures there are no side effects between invocations.
bool test_Performance8free_MultipleCalls()
{
    resetMockState();

    cmsContext ctx1 = (cmsContext)0x11111111;
    void* p1 = (void*)0x1001;

    cmsContext ctx2 = (cmsContext)0x22222222;
    void* p2 = (void*)0x2002;

    Performance8free(ctx1, p1);
    Performance8free(ctx2, p2);

    // We expect exactly two calls cumulatively; the last observed call should be ctx2/p2
    bool ok = (g_callCount == 2) && (g_lastContext == ctx2) && (g_lastPtr == p2);
    if (!ok) {
        std::cerr << "[FAIL] test_Performance8free_MultipleCalls: "
                  << "Expected last call to be ctx2/p2 with callCount=2, but got callCount="
                  << g_callCount << ", lastContext=" << g_lastContext << ", lastPtr=" << g_lastPtr
                  << std::endl;
    } else {
        std::cout << "[PASS] test_Performance8free_MultipleCalls" << std::endl;
    }
    return ok;
}

// Entry point: run all tests and report overall status
int main()
{
    bool t1 = test_Performance8free_CallsCmsFree_WithGivenParams();
    bool t2 = test_Performance8free_NullPtr();
    bool t3 = test_Performance8free_MultipleCalls();

    int failures = 0;
    if (!t1) ++failures;
    if (!t2) ++failures;
    if (!t3) ++failures;

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}