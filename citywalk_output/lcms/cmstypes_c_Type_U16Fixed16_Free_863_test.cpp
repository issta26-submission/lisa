/*
Unit test suite for the focal method:
  void Type_U16Fixed16_Free(struct _cms_typehandler_struct* self, void* Ptr)

Context:
- The test uses a lightweight, self-contained harness (no Google Test) and relies on
  providing a mock _cmsFree function to observe the interaction.
- We assume the test environment provides the internal CMS structures via lcms2_internal.h.
- We link against the actual cmstypes.c implementation which defines Type_U16Fixed16_Free.
- The tests cover basic path (Ptr non-null) and edge path (Ptr null).
- Static/global state is used to verify that _cmsFree is invoked with expected arguments.

Candidate Keywords (from Step 1):
- _cmsFree, ContextID, Ptr, Type_U16Fixed16_Free, _cms_typehandler_struct, cmsContext
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Import necessary internal CMS declarations to match the focal method's expected types.
// Wrapping in extern "C" to avoid C++ name mangling issues when including C headers.
extern "C" {
}

// Declaration of the focal function under test (C linkage)
extern "C" void Type_U16Fixed16_Free(struct _cms_typehandler_struct* self, void* Ptr);

// We provide a mock implementation of _cmsFree to capture and verify its usage.
// The real _cmsFree in the library would free memory using the given ContextID,
// but here we track the arguments and simulate memory deallocation.
static void* g_last_ContextID = nullptr;
static void* g_last_Ptr = nullptr;
static int g_cmsFree_calls = 0;

extern "C" void _cmsFree(void* ContextID, void* Ptr) {
    g_last_ContextID = ContextID;
    g_last_Ptr = Ptr;
    ++g_cmsFree_calls;
    // Simulate actual free if a non-null pointer was provided.
    if (Ptr) {
        free(Ptr);
    }
}

// Simple test harness
static int g_failures = 0;
#define ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "FAIL: " msg << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "PASS: " msg << std::endl; \
    } \
} while(0)

// Test 1: Basic behavior with a valid non-null Ptr.
// Ensures that Type_U16Fixed16_Free calls _cmsFree with the correct ContextID and Ptr.
static void test_U16Fixed16_Free_Basic() {
    // Reset mock state
    g_cmsFree_calls = 0;
    g_last_ContextID = nullptr;
    g_last_Ptr = nullptr;

    // Prepare a fake type handler instance with a known context
    cmsContext ctx = (cmsContext)0xDEADBEAF;
    struct _cms_typehandler_struct self;
    self.ContextID = (void*)ctx;

    // Allocate a pointer to be freed by the focal function
    void* p = malloc(128);
    if (p) {
        std::memset(p, 0xAB, 128);
    }

    // Invoke the function under test
    Type_U16Fixed16_Free(&self, p);

    // Validate interactions
    ASSERT(g_cmsFree_calls == 1, "Type_U16Fixed16_Free should invoke _cmsFree exactly once");
    ASSERT(g_last_ContextID == (void*)ctx, "ContextID passed to _cmsFree should match self.ContextID");
    ASSERT(g_last_Ptr == p, "Ptr passed to _cmsFree should be the same as the allocated Ptr");
}

// Test 2: Edge case with NULL Ptr.
// Ensures that _cmsFree is still called with the correct ContextID and a NULL Ptr.
static void test_U16Fixed16_Free_NullPtr() {
    // Reset mock state
    g_cmsFree_calls = 0;
    g_last_ContextID = nullptr;
    g_last_Ptr = nullptr;

    cmsContext ctx = (cmsContext)0x12345678;
    struct _cms_typehandler_struct self;
    self.ContextID = (void*)ctx;

    // Pass NULL Ptr
    Type_U16Fixed16_Free(&self, nullptr);

    // Validate interactions
    ASSERT(g_cmsFree_calls == 1, "Type_U16Fixed16_Free should invoke _cmsFree even when Ptr is NULL");
    ASSERT(g_last_ContextID == (void*)ctx, "ContextID passed to _cmsFree should match self.ContextID when Ptr is NULL");
    ASSERT(g_last_Ptr == nullptr, "Ptr passed to _cmsFree should be NULL when input Ptr is NULL");
}

// Entry point of the test suite
int main() {
    std::cout << "Running Type_U16Fixed16_Free unit tests (no GTest)\n";
    test_U16Fixed16_Free_Basic();
    test_U16Fixed16_Free_NullPtr();

    if (g_failures > 0) {
        std::cerr << "TOTAL FAILURES: " << g_failures << std::endl;
        return 1;
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}