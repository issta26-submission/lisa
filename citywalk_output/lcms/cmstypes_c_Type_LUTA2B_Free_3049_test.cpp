/*
Unit tests for Type_LUTA2B_Free in cmstypes.c

Goal:
- Verify that Type_LUTA2B_Free delegates the freeing operation to cmsPipelineFree with the same pointer
- Do so without requiring the real cmsPipelineFree to run (to avoid depending on lcms internals)
- Use GCC linker wrap feature to intercept cmsPipelineFree calls and record the argument
- No GTest; a small C++11 test harness is implemented with lightweight EXPECT-style assertions

Build note (not part of code):
- Compile with linker wrapping enabled:
  g++ -std=c++11 test_cmstypes_free.cpp cmstypes.c -Wl,--wrap=cmsPipelineFree -o test_cmstypes_free
- The wrapper __wrap_cmsPipelineFree will be invoked instead of the real cmsPipelineFree during tests.

Key dependencies and exploration (from Step 1):
- Focal function: Type_LUTA2B_Free(struct _cms_typehandler_struct* self, void* Ptr)
- Core dependency: cmsPipelineFree (wrapped in tests)
- Parameter self is unused in the implementation (we still pass it to keep signature valid)
- Ptr is forwarded to cmsPipelineFree as a cmsPipeline* (we test with non-null and null values)
- We rely only on standard C/C++ facilities and the provided function signature

Code below:
*/

#include <lcms2_internal.h>
#include <cstdio>
#include <cstdint>
#include <cstddef>


// Forward declaration of the focal type to match the function signature.
// We do not rely on its internals for the unit test.
struct _cms_typehandler_struct;

/* Prototype of the function under test (as declared in cmstypes.c)
   We declare with C linkage to ensure correct symbol resolution across translation units. */
extern "C" void Type_LUTA2B_Free(struct _cms_typehandler_struct* self, void* Ptr);

// Candidate keyword coverage: cmsPipelineFree, _cms_typehandler_struct, Type_LUTA2B_Free, cmsUNUSED_PARAMETER

// ---------------------------------------------------------------------------
// Test harness using GCC's --wrap feature to intercept cmsPipelineFree calls.
// When linking with -Wl,--wrap=cmsPipelineFree, all calls to cmsPipelineFree(...) in
// cmstypes.c will be redirected to __wrap_cmsPipelineFree(...).
// We implement __wrap_cmsPipelineFree to record the argument passed.
// This allows us to verify that Type_LUTA2B_Free forwards Ptr correctly without
// needing the real memory management implementation.
// ---------------------------------------------------------------------------

// Global test-state for the wrapper
static void* g_last_freed_ptr = nullptr;
static int g_wrap_call_count = 0;

// Wrapper to intercept cmsPipelineFree calls.
// Signature is chosen to be compatible with a pointer-to-opaque-cmsPipeline.
// We use a generic void* parameter to avoid depending on internal cmsPipeline type.
// The actual call will originate from Type_LUTA2B_Free as (cmsPipeline*)Ptr.
extern "C" void __wrap_cmsPipelineFree(void* Ptr)
{
    ++g_wrap_call_count;
    g_last_freed_ptr = Ptr;
    // Intentionally do nothing else; we do not free memory in the test harness.
}

// Simple test result tracking
static int g_total_failures = 0;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_total_failures; \
    } \
} while(0)

#define EXPECT_EQ_PTR(a,b) do { \
    if((void*)(a) != (void*)(b)) { \
        fprintf(stderr, "EXPECT_EQ_PTR failed: %s:%d: %p != %p (expected %p)\n", __FILE__, __LINE__, (void*)(a), (void*)(b), (void*)(b)); \
        ++g_total_failures; \
    } \
} while(0)

#define EXPECT_EQ_INT(a,b) do { \
    if((a) != (b)) { \
        fprintf(stderr, "EXPECT_EQ_INT failed: %s:%d: %d != %d\n", __FILE__, __LINE__, (int)(a), (int)(b)); \
        ++g_total_failures; \
    } \
} while(0)

// Domain knowledge: ensure we expose a C-callable prototype for the function under test
// so this C++ test can call it directly.
extern "C" {
    void Type_LUTA2B_Free(struct _cms_typehandler_struct* self, void* Ptr);
}

// Test 1: Non-null Ptr should cause cmsPipelineFree to be called with that exact pointer
static bool test_free_with_non_null_ptr() {
    g_wrap_call_count = 0;
    g_last_freed_ptr = nullptr;

    void* testPtr = (void*)0xDEADBEEF;
    Type_LUTA2B_Free(nullptr, testPtr); // self is unused by the function

    EXPECT_EQ_INT(g_wrap_call_count, 1);
    EXPECT_EQ_PTR(g_last_freed_ptr, testPtr);
    return true;
}

// Test 2: Null Ptr should still call cmsPipelineFree with NULL
static bool test_free_with_null_ptr() {
    g_wrap_call_count = 0;
    g_last_freed_ptr = nullptr;

    Type_LUTA2B_Free(nullptr, nullptr);

    EXPECT_EQ_INT(g_wrap_call_count, 1);
    EXPECT_EQ_PTR(g_last_freed_ptr, nullptr);
    return true;
}

// Test 3: Multiple sequential calls should accumulate wrapper invocations
static bool test_free_multiple_calls() {
    g_wrap_call_count = 0;
    g_last_freed_ptr = nullptr;

    Type_LUTA2B_Free(nullptr, (void*)0x1111);
    Type_LUTA2B_Free(nullptr, (void*)0x2222);

    EXPECT_EQ_INT(g_wrap_call_count, 2);
    EXPECT_EQ_PTR(g_last_freed_ptr, (void*)0x2222);
    return true;
}

// Main driver: run all tests and report results.
// Note: We do not abort on first failure to maximize code execution coverage.
int main() {
    int tests_passed = 0;

    bool t1 = test_free_with_non_null_ptr();
    bool t2 = test_free_with_null_ptr();
    bool t3 = test_free_multiple_calls();

    tests_passed = (t1 ? 1 : 0) + (t2 ? 1 : 0) + (t3 ? 1 : 0);

    if(g_total_failures == 0) {
        fprintf(stdout, "All tests passed (%d tests).\n", tests_passed);
        return 0;
    } else {
        fprintf(stderr, "Total failures: %d out of %d tests.\n", g_total_failures, tests_passed);
        return 1;
    }
}