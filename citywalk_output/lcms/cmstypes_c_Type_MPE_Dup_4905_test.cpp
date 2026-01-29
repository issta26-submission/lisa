// Unit test suite for the focal method Type_MPE_Dup in cmstypes.c
// - No GTest used
// - Custom lightweight test harness with non-terminating assertions
// - Tests rely on a lightweight mock of cmsPipelineDup/cmsPipelineFree to avoid
//   depending on a full CMS pipeline implementation while still exercising
//   Type_MPE_Dup's behavior
// - Tests are written for C++11 compatibility

#include <vector>
#include <iostream>
#include <algorithm>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Import necessary types from LittleCMS (lcms2). We rely on the real headers
// to obtain definitions like cmsPipeline, cmsUInt32Number, cmsContext, etc.

// Forward declaration of the focal function's expected signature.
// The actual implementation is in cmstypes.c (part of the code under test).
struct _cms_typehandler_struct;
extern "C" void* Type_MPE_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// -----------------------------------------------------------------------------------
// Lightweight test scaffolding to mock cmsPipelineDup and cmsPipelineFree
// The goal is to control and observe the interactions without depending on full libcms2 runtime.
// We implement these as "fake" hooks that will override any real library symbols when linked
// together with cmstypes.c in the final binary.
// -----------------------------------------------------------------------------------

typedef struct cmsPipeline cmsPipeline;

// Track state of the mocked CMS pipeline operations
static int g_dup_calls = 0;
static cmsPipeline* g_last_input_ptr = nullptr;

// A simple container to track allocated fake pipelines so we can "free" them.
static std::vector<cmsPipeline*> g_allocated_pipelines;

// Mock implementation of cmsPipelineDup
// Signature is made to be compatible with cmsPipelineDup used by Type_MPE_Dup.
// We return a brand-new cmsPipeline object for non-null inputs and NULL for NULL input.
extern "C" cmsPipeline* cmsPipelineDup(cmsPipeline* p)
{
    ++g_dup_calls;
    g_last_input_ptr = p;
    if (p == nullptr) {
        // Simulate typical behavior: duplicating a NULL pointer yields NULL/invalid
        return nullptr;
    }
    // Allocate a lightweight dummy pipeline object (opaque to tests) to simulate a dup.
    cmsPipeline* dup = new cmsPipeline();
    g_allocated_pipelines.push_back(dup);
    return dup;
}

// Mock implementation of cmsPipelineFree
// We assume a matching freeing pathway for the allocated dummy pipelines.
// The actual signature in liblcms2 is typically: void cmsPipelineFree(cmsContext, cmsPipeline*)
// Here we approximate with the same parameter order to ensure compatibility with linkage.
extern "C" void cmsPipelineFree(cmsContext /*ContextID*/, cmsPipeline* p)
{
    if (!p) return;
    auto it = std::find(g_allocated_pipelines.begin(), g_allocated_pipelines.end(), p);
    if (it != g_allocated_pipelines.end()) {
        delete *it;
        g_allocated_pipelines.erase(it);
    } else {
        // If the pointer wasn't tracked, just free to avoid leaks in tests
        delete p;
    }
}

// -----------------------------------------------------------------------------------
// Test harness helpers
// - Non-terminating checks (do not abort on failure, just record and print)
// - Basic summary reporting at program end
// -----------------------------------------------------------------------------------

static int g_test_passed = 0;
static int g_test_failed = 0;

#define TEST_PASS(msg) do { \
    std::cout << "[PASS] " << msg << std::endl; \
    ++g_test_passed; \
} while (0)

#define TEST_FAIL(msg) do { \
    std::cerr << "[FAIL] " << msg << std::endl; \
    ++g_test_failed; \
} while (0)

static void print_summary() {
    std::cout << "\nTest summary: " << g_test_passed << " passed, "
              << g_test_failed << " failed.\n";
}

// -----------------------------------------------------------------------------------
// Test 1: Type_MPE_Dup should return NULL when Ptr is NULL
// Rationale:
// - The focal method delegates to cmsPipelineDup((cmsPipeline*)Ptr).
// - If Ptr is NULL, the mocked cmsPipelineDup returns NULL, so Type_MPE_Dup must propagate NULL.
// This tests the "false" branch (Ptr == NULL) behavior.
// -----------------------------------------------------------------------------------
void test_Type_MPE_Dup_null_ptr()
{
    // Reset mock state
    g_dup_calls = 0;
    g_last_input_ptr = nullptr;
    g_allocated_pipelines.clear();

    void* result = Type_MPE_Dup(nullptr, nullptr, 0);

    // Expectation: NULL result
    if (result == nullptr) {
        TEST_PASS("Type_MPE_Dup(NULL_PTR) returned NULL as expected.");
    } else {
        TEST_FAIL("Type_MPE_Dup(NULL_PTR) did not return NULL.");
    }

    // cmsPipelineDup should have been called exactly once with NULL input
    if (g_dup_calls == 1 && g_last_input_ptr == nullptr) {
        TEST_PASS("cmsPipelineDup invoked once with input NULL as expected.");
    } else {
        TEST_FAIL("cmsPipelineDup invocation for NULL input did not match expectations.");
    }
}

// -----------------------------------------------------------------------------------
// Test 2: Type_MPE_Dup should duplicate a valid pipeline pointer
// Rationale:
// - Ptr is a non-NULL pointer. Type_MPE_Dup should call cmsPipelineDup with that pointer.
// - The returned pointer should be non-NULL (a new pipeline object) and must be distinct from input.
// This exercises the "true" branch (Ptr non-NULL) and ensures duplication flow.
// -----------------------------------------------------------------------------------
void test_Type_MPE_Dup_valid_ptr()
{
    // Reset mock state
    g_dup_calls = 0;
    g_last_input_ptr = nullptr;
    g_allocated_pipelines.clear();

    // Provide a non-NULL fake input pointer (value only; content is not inspected by tests)
    cmsPipeline* fake_input = reinterpret_cast<cmsPipeline*>(0x1234);

    void* result = Type_MPE_Dup(nullptr, fake_input, 42);

    // Expectation: result is non-NULL
    if (result != nullptr) {
        TEST_PASS("Type_MPE_Dup(valid_ptr) returned a non-NULL pointer as expected.");
    } else {
        TEST_FAIL("Type_MPE_Dup(valid_ptr) returned NULL unexpectedly.");
    }

    // The mock should have been invoked with the exact input pointer
    if (g_dup_calls == 1 && g_last_input_ptr == fake_input) {
        TEST_PASS("cmsPipelineDup called with the correct input pointer.");
    } else {
        TEST_FAIL("cmsPipelineDup did not receive the expected input pointer.");
    }

    // Cleanup: free the allocated duplicate if created
    if (result != nullptr) {
        // Our mock cmsPipelineDup returns a newly allocated object; free it
        cmsPipelineFree(nullptr, reinterpret_cast<cmsPipeline*>(result));
    }
}

// -----------------------------------------------------------------------------------
// Test 3: Type_MPE_Dup should behave consistently with non-null self and large n
// Rationale:
// - The self pointer and n payload are unused by the focal logic, but we still exercise
//   a variation to ensure no hidden branches depend on these values.
// -----------------------------------------------------------------------------------
void test_Type_MPE_Dup_nonnull_self_and_large_n()
{
    // Reset mock state
    g_dup_calls = 0;
    g_last_input_ptr = nullptr;
    g_allocated_pipelines.clear();

    cmsPipeline* fake_input = reinterpret_cast<cmsPipeline*>(0xABCD);

    // Use a non-null self pointer and a large n value
    struct _cms_typehandler_struct* fake_self = reinterpret_cast<struct _cms_typehandler_struct*>(0xDEADBEEF);
    cmsUInt32Number large_n = 1000;

    void* result = Type_MPE_Dup(fake_self, fake_input, large_n);

    // Expectation: still a non-NULL result, indicating proper delegation
    if (result != nullptr) {
        TEST_PASS("Type_MPE_Dup(nonnull_self, large_n) returned a non-NULL pointer as expected.");
    } else {
        TEST_FAIL("Type_MPE_Dup(nonnull_self, large_n) returned NULL unexpectedly.");
    }

    // Ensure cmsPipelineDup was invoked and input pointer was passed through
    if (g_dup_calls == 1 && g_last_input_ptr == fake_input) {
        TEST_PASS("cmsPipelineDup called with the expected input pointer under non-null self and large n.");
    } else {
        TEST_FAIL("cmsPipelineDup invocation mismatch for non-null self / large n case.");
    }

    // Cleanup
    if (result != nullptr) {
        cmsPipelineFree(nullptr, reinterpret_cast<cmsPipeline*>(result));
    }
}

// -----------------------------------------------------------------------------------
// Entry point
// -----------------------------------------------------------------------------------
int main()
{
    // Run tests
    test_Type_MPE_Dup_null_ptr();
    test_Type_MPE_Dup_valid_ptr();
    test_Type_MPE_Dup_nonnull_self_and_large_n();

    print_summary();
    return (g_test_failed == 0) ? 0 : 1;
}