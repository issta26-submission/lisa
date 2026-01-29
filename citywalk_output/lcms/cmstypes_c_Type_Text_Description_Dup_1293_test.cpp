// File: cms_stubs.c
// This stub provides a minimal, test-friendly implementation of cmsMLUdup
// so that Type_Text_Description_Dup can be exercised without relying on
// the full upstream cmsMLU duplication logic. The stub returns a stable
// sentinel object regardless of the input pointer, enabling predictable
// comparisons in tests.

// Note: This file is compiled as C code (not C++), to match the linkage
// expectations of the focal C file cmstypes.c.

#include <lcms2_internal.h>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>


// Forward declare the cmsMLU type to match the expected signature used by the
// focal function (Type_Text_Description_Dup casts Ptr to cmsMLU* and passes to cmsMLUdup).
typedef struct cmsMLU cmsMLU;

// Minimal cmsMLU structure for the stub; its internal layout is irrelevant
// for the test harness since we do not dereference its internals here.
struct cmsMLU {
    int dummy; // placeholder to ensure size != 0
};

// Sentinel object and its address to be returned by cmsMLUdup
static cmsMLU sentinel_object = { 0 };
static cmsMLU* sentinel = &sentinel_object;

// cmsMLUdup stub: duplicates a cmsMLU object. For testing purposes, we ignore
// the source and return the shared sentinel. This allows tests to verify
// interaction without implementing real duplication logic.
cmsMLU* cmsMLUdup(const cmsMLU* src) {
    (void)src; // suppress unused parameter warning
    // Return the stable sentinel regardless of input
    return sentinel;
}

// Expose sentinel for test code via C linkage
// (In the test binary, tests will compare the result to this sentinel's address.)
extern cmsMLU* sentinel;


// File: test_cmstypes_text_description_dup.cpp
// A lightweight, GTest-free unit test suite for Type_Text_Description_Dup.
// - Uses a small custom test harness with non-terminating assertions.
// - Exercises both null and non-null Ptr inputs to cover different call paths.
// - Relies on the cmsMLUdup stub defined in cms_stubs.c to provide deterministic behavior.
//
// Source dependencies represented (Candidate Keywords):
// - Type_Text_Description_Dup: focal method under test.
// - _cms_typehandler_struct: class dependency used as the 'self' parameter.
// - cmsUInt32Number: parameter type for n (unused in the function).
// - cmsMLU: the type involved in the duplication call (Ptr cast to cmsMLU*).
// - cmsMLUdup: the function invoked by Type_Text_Description_Dup.
// - sentinel: the test-visible return value from the cmsMLUdup stub.
//
// Build note (assumed): Compile with the focal file cmstypes.c and the stub cms_stubs.c
// and the test file test_cmstypes_text_description_dup.cpp, e.g.:
//   g++ test_cmstypes_text_description_dup.cpp cmstypes.c cms_stubs.c -o test_run
//   ./test_run


// Forward declare the minimal type and function signature required to call
// Type_Text_Description_Dup from C++ test code.
typedef unsigned int cmsUInt32Number;               // Domain knowledge: 32-bit unsigned
struct _cms_typehandler_struct;                    // Opaque in tests; only pointer is used
typedef struct cmsMLU cmsMLU;                       // Forward declaration matching stub

// Declaration of the focal function under test (signature from the focal method)
extern "C" void* Type_Text_Description_Dup(struct _cms_typehandler_struct* self,
                                          const void *Ptr,
                                          cmsUInt32Number n);

// Sentinel symbol provided by cms_stubs.c (C linkage)
extern "C" cmsMLU* sentinel;

// Minimal test harness: non-terminating EXPECT-style assertions
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_tests_run; \
        if (!(cond)) { \
            ++g_tests_failed; \
            fprintf(stderr, "TEST FAIL: %s (line %d): %s\n", __FILE__, __LINE__, msg); \
        } \
    } while(0)

int main() {
    // Explanatory comment: We follow a small but deliberate test plan
    // 1) True behavior: Ptr = NULL should still yield a valid cmsMLU-dup result via the stub.
    // 2) False/edge behavior: Ptr = non-NULL should also yield the sentinel from the stub,
    //    demonstrating that the function delegates to cmsMLUdup regardless of input value.
    //
    // This approach yields two test cases, covering both the true and false-like branches
    // in terms of input pointer values, while keeping the test robust against internal
    // copy logic that is outside the scope of the unit under test.

    // Test 1: Null Ptr input
    {
        struct _cms_typehandler_struct* self = nullptr;
        void* result = Type_Text_Description_Dup(self, nullptr, 0);
        EXPECT_TRUE(result == (void*)sentinel,
                    "Type_Text_Description_Dup(nullptr, nullptr, 0) should return sentinel from cmsMLUdup stub");
    }

    // Test 2: Non-null Ptr input
    {
        struct _cms_typehandler_struct* self = nullptr;
        void* dummy_ptr = malloc(16); // Pointer to some memory region (content not inspected)
        void* result = Type_Text_Description_Dup(self, dummy_ptr, 7);
        EXPECT_TRUE(result == (void*)sentinel,
                    "Type_Text_Description_Dup(nullptr, non_null_ptr, 7) should return sentinel from cmsMLUdup stub");
        free(dummy_ptr);
    }

    // Summary
    if (g_tests_failed == 0) {
        printf("ALL TESTS PASSED. Ran %d test(s).\n", g_tests_run);
    } else {
        printf("TESTS FAILED: %d failed out of %d test(s).\n", g_tests_failed, g_tests_run);
    }

    // Return number of failures as exit code
    return g_tests_failed;
}