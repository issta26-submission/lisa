// Test suite for the focal method Type_ProfileSequenceId_Free (in cmstypes.c)
// This test uses a lightweight, self-contained harness without GoogleTest.
// It mocks the external C function cmsFreeProfileSequenceDescription to verify
// that Type_ProfileSequenceId_Free correctly forwards Ptr to the free routine.
// The test is designed to be compiled and linked with the actual cmstypes.c
// and cmslcms2 library sources available in the build environment.
//
// Build notes (example):
// g++ -std=c++11 -O2 -c test_cmstypes_profile_seqid_free.cpp
// g++ -std=c++11 -O2 -c cmstypes.c                 // or link the prebuilt object
// g++ -std=c++11 -O2 test_cmstypes_profile_seqid_free.o cmstypes.o -o test_runner
//
// If building standalone with a build system, ensure the C linkage for the C
// function being mocked and the Type_ProfileSequenceId_Free symbol from cmstypes.c
// are preserved during linking.

// Extern declarations of the function under test and the related type
// The actual Type_ProfileSequenceId_Free is defined in cmstypes.c and uses the
// CMS internal types. We only need to declare its signature to invoke it from C++.
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Forward declaration of the opaque cms internal type used by the focal function.
// The real type is defined inside the cmstypes.c / lcms headers; we only need a
// forward declaration to declare the function prototype correctly.
struct _cms_typehandler_struct;

// Declare the focal function from cmstypes.c with C linkage
extern "C" void Type_ProfileSequenceId_Free(struct _cms_typehandler_struct* self, void* Ptr);

// Globals to capture interactions from the mocked C function
static bool g_cmsFreeCalled = false;
static void* g_lastPtr = nullptr;

// Mock implementation of cmsFreeProfileSequenceDescription
// This function will be linked in instead of the real library function during the test.
// It marks that the function was called and records the pointer passed.
// It also frees the memory pointed to by Ptr to simulate correct memory management.
extern "C" void cmsFreeProfileSequenceDescription(void* Ptr)
{
    g_cmsFreeCalled = true;
    g_lastPtr = Ptr;
    // Simulate actual free to mimic library behavior without leaking memory.
    // Note: Ptr is expected to be a heap-allocated block in tests.
    free(Ptr);
}

// Helper for clean test output
static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        fprintf(stderr, "FAIL: %s\n", msg); \
        ++g_failures; \
    } else { \
        fprintf(stdout, "PASS: %s\n", msg); \
    } \
} while(0)

// Test 1: Non-null Ptr should be passed to cmsFreeProfileSequenceDescription and freed.
// We allocate a small block, call the function, and verify the mock was invoked
// with the same pointer value.
static void test_non_null_ptr()
{
    // Reset state
    g_cmsFreeCalled = false;
    g_lastPtr = nullptr;

    void* p = malloc(32); // non-null pointer to be freed by the mock
    if(!p) { fprintf(stderr, "Setup failed: malloc returned null\n"); g_failures++; return; }

    // self can be any value; not used by the focal function but must be valid
    Type_ProfileSequenceId_Free(nullptr, p);

    // Verify that cmsFreeProfileSequenceDescription was invoked and with the exact pointer
    CHECK(g_cmsFreeCalled, "cmsFreeProfileSequenceDescription should be called for non-null Ptr");
    CHECK(g_lastPtr == p, "cmsFreeProfileSequenceDescription should receive the original Ptr");

    // If cmsFreeProfileSequenceDescription freed the pointer, no double-free occurs here.
}

// Test 2: Null Ptr should be forwarded to cmsFreeProfileSequenceDescription as NULL.
static void test_null_ptr()
{
    // Reset state
    g_cmsFreeCalled = false;
    g_lastPtr = reinterpret_cast<void*>(0xDEADBEEF);

    Type_ProfileSequenceId_Free(nullptr, nullptr);

    CHECK(g_cmsFreeCalled, "cmsFreeProfileSequenceDescription should be called for null Ptr");
    CHECK(g_lastPtr == nullptr, "cmsFreeProfileSequenceDescription should receive NULL Ptr");
}

// Test 3: Self parameter non-null (to ensure static/unused parameter handling does not crash).
static void test_with_self_parameter()
{
    // Reset state
    g_cmsFreeCalled = false;
    g_lastPtr = nullptr;

    void* p = malloc(16);
    if(!p) { fprintf(stderr, "Setup failed: malloc returned null\n"); g_failures++; return; }

    // Pass a non-null self to ensure the function handles unused parameters gracefully
    Type_ProfileSequenceId_Free(reinterpret_cast<struct _cms_typehandler_struct*>(0x1234), p);

    CHECK(g_cmsFreeCalled, "cmsFreeProfileSequenceDescription should be called even when self is non-null");
    CHECK(g_lastPtr == p, "cmsFreeProfileSequenceDescription should receive the non-null Ptr even with non-null self");
}

// Entry point
int main()
{
    fprintf(stdout, "Starting tests for Type_ProfileSequenceId_Free...\n");

    test_non_null_ptr();
    test_null_ptr();
    test_with_self_parameter();

    if(g_failures != 0) {
        fprintf(stderr, "TOTAL FAILURES: %d\n", g_failures);
        return 1;
    }

    fprintf(stdout, "All tests completed. SUMMARY: PASS\n");
    return 0;
}