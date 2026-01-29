/*
Step 1 - Candidate Keywords (representing core dependent components)
- _cmsFree: the memory freeing function that Type_Chromaticity_Free delegates to
- cmsContext: the type of the context handle passed to _cmsFree
- struct _cms_typehandler_struct: the object whose ContextID field is used by Type_Chromaticity_Free
- Type_Chromaticity_Free: focal function under test
- Ptr: user data pointer passed to Type_Chromaticity_Free and forwarded to _cmsFree
This test harness overrides _cmsFree to observe its inputs and to validate that Type_Chromaticity_Free properly forwards its parameters.
*/

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Include library internal definitions to expose Type_Chromaticity_Free and related types.
// Assumes the build environment provides lcms2_internal.h in include path.

// Test harness: provide a controllable override for the external C function _cmsFree.
// We rely on the fact cmstypes.c uses an external _cmsFree; by compiling this test
// together with cmstypes.c, our definition will satisfy the linkage.

extern "C" {
    // Observables from the override
    static cmsContext g_lastContext = nullptr;
    static void*        g_lastPtr     = nullptr;
    static bool          g_wasFreed     = false;

    // Override of the internal allocator/free function used by Type_Chromaticity_Free
    void _cmsFree(cmsContext ContextID, void* Ptr)
    {
        g_lastContext = ContextID;
        g_lastPtr     = Ptr;
        g_wasFreed     = true;
        // No actual freeing performed here to keep test observable separate from allocator.
    }
}

// Simple non-terminating test assertion framework (does not abort on failure)
static int g_test_failures = 0;

#define ASSERT_TRUE(cond, msg) do {                                        \
    if(!(cond)) {                                                           \
        std::cerr << "TEST_FAIL: " << __FUNCTION__ << " - " << msg << "\n";\
        ++g_test_failures;                                                  \
    }                                                                       \
} while(false)

#define ASSERT_EQ(expected, actual, msg) do {                               \
    if(!((expected) == (actual))) {                                        \
        std::cerr << "TEST_FAIL: " << __FUNCTION__ << " - " << msg           \
                  << " (expected: " << (void*)(expected)                    \
                  << ", actual: " << (void*)(actual) << ")\n";            \
        ++g_test_failures;                                                  \
    }                                                                       \
} while(false)

// Step 2 - Unit Test Generation for Type_Chromaticity_Free
// Test 1: Ensure that a valid Ptr is forwarded to _cmsFree with the correct ContextID
void test_TypeChromaticity_Free_ForwardsPointerAndContext()
{
    // Reset observer state
    g_lastContext = nullptr;
    g_lastPtr     = nullptr;
    g_wasFreed     = false;

    // Prepare a dummy self with a specific ContextID
    cmsContext expectedContext = reinterpret_cast<cmsContext>(0x12345678);
    struct _cms_typehandler_struct* self = new _cms_typehandler_struct;
    // Accessing the member as defined in the internal header
    self->ContextID = expectedContext;

    // Allocate a representative pointer
    void* Ptr = malloc(64);
    if (Ptr == nullptr) {
        std::cerr << "Setup failure: malloc() returned NULL\n";
        g_test_failures++;
        delete self;
        return;
    }

    // Call the function under test
    Type_Chromaticity_Free(self, Ptr);

    // Validate behavior: _cmsFree should have been invoked with the exact arguments
    ASSERT_TRUE(g_wasFreed, "Expected _cmsFree to be called");
    ASSERT_EQ(expectedContext, g_lastContext, "ContextID forwarded to _cmsFree mismatch");
    ASSERT_EQ(Ptr, g_lastPtr, "Ptr forwarded to _cmsFree mismatch");

    // Cleanup
    free(Ptr);        // Ptr was not actually freed by _cmsFree in this test harness
    delete self;
}

// Test 2: Ensure that Type_Chromaticity_Free handles a NULL Ptr gracefully by forwarding NULL
// Note: self.ContextID is still forwarded, but Ptr is NULL. This validates both forwarding and null handling.
void test_TypeChromaticity_Free_ForwardsNullPtr()
{
    // Reset observer state
    g_lastContext = nullptr;
    g_lastPtr     = nullptr;
    g_wasFreed     = false;

    cmsContext expectedContext = reinterpret_cast<cmsContext>(0xDEADBEEF);
    struct _cms_typehandler_struct* self = new _cms_typehandler_struct;
    self->ContextID = expectedContext;

    void* Ptr = nullptr;

    Type_Chromaticity_Free(self, Ptr);

    ASSERT_TRUE(g_wasFreed, "Expected _cmsFree to be called even with NULL Ptr");
    ASSERT_EQ(expectedContext, g_lastContext, "ContextID forwarded to _cmsFree mismatch for NULL Ptr");
    ASSERT_TRUE(g_lastPtr == nullptr, "Ptr forwarded to _cmsFree should be NULL when input Ptr is NULL");

    delete self;
}

// Step 3 - Test runner
int main()
{
    // Run tests
    test_TypeChromaticity_Free_ForwardsPointerAndContext();
    test_TypeChromaticity_Free_ForwardsNullPtr();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED\n";
        return 1;
    }
}