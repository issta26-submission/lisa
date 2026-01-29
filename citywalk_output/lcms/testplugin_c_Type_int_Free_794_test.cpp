// Test suite for the focal method: Type_int_Free in testplugin.c
// This test uses a lightweight, in-process harness without GTest.
// It relies on a mock _cmsFree to validate correct interactions.

#include <cstring>
#include <cstdio>
#include <testcms2.h>
#include <cassert>
#include <cstdlib>
#include <cstdint>


// Step A: Import necessary dependencies
// We rely on the project's provided header to expose core types.
// The header defines cmsContext, struct _cms_typehandler_struct, etc.

// Step B: Declare the focal function under test (C linkage)
extern "C" void Type_int_Free(struct _cms_typehandler_struct* self, void* Ptr);

// Step C: Provide a mock for _cmsFree to capture its arguments for verification
static cmsContext g_lastContext = nullptr;
static void*       g_lastPtr     = nullptr;
static std::uint32_t g_freeCalls   = 0;

// Mock implementation of _cmsFree with C linkage so the linker resolves it
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr) {
    g_lastContext = ContextID;
    g_lastPtr     = Ptr;
    ++g_freeCalls;
}

// Helper to reset mock state between tests
static void resetMockFree() {
    g_lastContext = nullptr;
    g_lastPtr     = nullptr;
    g_freeCalls   = 0;
}

// Utility macro for clearer failure messaging
#define FAIL_MSG(msg) do { \
    std::fprintf(stderr, "TEST FAILED: %s\n", (msg)); \
} while(0)

// Test 1: Basic behavior - Type_int_Free should call _cmsFree with the given self.ContextID and Ptr
static bool test_Type_int_Free_Basic() {
    resetMockFree();

    // Prepare a self object with a known ContextID
    struct _cms_typehandler_struct self;
    // Note: ContextID type is provided by testcms2.h; cast to that type for testing
    self.ContextID = (cmsContext)0x1111222233334444ULL;

    void* testPtr = (void*)0xDEADBEEF;

    // Call the focal method
    Type_int_Free(&self, testPtr);

    // Validate that _cmsFree was invoked exactly once with correct args
    if (g_freeCalls != 1) {
        FAIL_MSG("Type_int_Free did not call _cmsFree exactly once.");
        return false;
    }
    if (g_lastContext != self.ContextID) {
        FAIL_MSG("Type_int_Free passed incorrect ContextID to _cmsFree.");
        return false;
    }
    if (g_lastPtr != testPtr) {
        FAIL_MSG("Type_int_Free passed incorrect Ptr to _cmsFree.");
        return false;
    }

    return true;
}

// Test 2: Null pointer handling - Ptr = nullptr should still invoke _cmsFree with nullptr
static bool test_Type_int_Free_NullPtr() {
    resetMockFree();

    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0xAABBCCDDEEFF0011ULL;

    void* testPtr = nullptr;

    Type_int_Free(&self, testPtr);

    if (g_freeCalls != 1) {
        FAIL_MSG("Type_int_Free with NULL Ptr did not call _cmsFree exactly once.");
        return false;
    }
    if (g_lastContext != self.ContextID) {
        FAIL_MSG("Type_int_Free with NULL Ptr passed incorrect ContextID to _cmsFree.");
        return false;
    }
    if (g_lastPtr != testPtr) {
        FAIL_MSG("Type_int_Free with NULL Ptr passed non-NULL Ptr to _cmsFree.");
        return false;
    }

    return true;
}

// Test 3: Multiple invocations - ensure state resets between calls and multiple frees are tracked
static bool test_Type_int_Free_MultipleCalls() {
    resetMockFree();

    struct _cms_typehandler_struct self1;
    self1.ContextID = (cmsContext)0x1234000011110000ULL;
    void* p1 = (void*)0x1001;

    Type_int_Free(&self1, p1);

    if (g_freeCalls != 1) {
        FAIL_MSG("After first call, expected 1 free call.");
        return false;
    }
    if (g_lastContext != self1.ContextID || g_lastPtr != p1) {
        FAIL_MSG("First call arguments mismatch.");
        return false;
    }

    // Second invocation with different context and pointer
    resetMockFree();
    struct _cms_typehandler_struct self2;
    self2.ContextID = (cmsContext)0x2222333344445555ULL;
    void* p2 = (void*)0x2002;

    Type_int_Free(&self2, p2);

    if (g_freeCalls != 1) {
        FAIL_MSG("After second call, expected 1 free call.");
        return false;
    }
    if (g_lastContext != self2.ContextID || g_lastPtr != p2) {
        FAIL_MSG("Second call arguments mismatch.");
        return false;
    }

    return true;
}

// Step D: Test runner (no external test framework; run all tests in main)
int main() {
    int overall = 0;

    // Run Test 1
    if (test_Type_int_Free_Basic()) {
        std::printf("PASS: test_Type_int_Free_Basic\n");
    } else {
        std::printf("FAIL: test_Type_int_Free_Basic\n");
        overall = 1;
    }

    // Run Test 2
    if (test_Type_int_Free_NullPtr()) {
        std::printf("PASS: test_Type_int_Free_NullPtr\n");
    } else {
        std::printf("FAIL: test_Type_int_Free_NullPtr\n");
        overall = 1;
    }

    // Run Test 3
    if (test_Type_int_Free_MultipleCalls()) {
        std::printf("PASS: test_Type_int_Free_MultipleCalls\n");
    } else {
        std::printf("FAIL: test_Type_int_Free_MultipleCalls\n");
        overall = 1;
    }

    if (overall == 0) {
        std::printf("ALL TESTS PASSED\n");
    } else {
        std::printf("SOME TESTS FAILED\n");
    }

    return overall;
}