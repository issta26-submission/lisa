/*
Unit Test Suite for Type_Dictionary_Free in cmstypes.c

Overview:
- This test suite targets the focal function:
  void Type_Dictionary_Free(struct _cms_typehandler_struct* self, void* Ptr)

- Dependencies (from the focal class dependencies) revolve around:
  - cmsDictFree, cmsHANDLE, _cms_typehandler_struct
  - cmsUNUSED_PARAMETER(self) usage (self is unused)
- We create a minimal test harness (no GTest) with non-terminating assertions to maximize coverage.

Approach:
- Provide a test-only override for cmsDictFree to capture its invocation and the Ptr argument.
- Expose a C-callable helper to retrieve the last Ptr passed and the number of times cmsDictFree was invoked.
- Verify:
  - Normal non-null Ptr case: cmsDictFree receives the exact Ptr and is called once.
  - Null Ptr case: cmsDictFree is still called with nullptr and counted.
  - Self pointer can be NULL (as Type_Dictionary_Free uses cmsUNUSED_PARAMETER(self)).
  - Multiple successive calls accumulate correctly and the last Ptr matches the last call.

How to interpret the tests:
- The tests exercise the dependency interaction: Type_Dictionary_Free delegates the freeing to cmsDictFree with Ptr cast to cmsHANDLE.
- By overriding cmsDictFree, we can observe the behavior without linking to the real dictionary manager.
- All tests use non-terminating checks (continue execution after failures) and log results to stdout/stderr.

Notes:
- The tests rely on the C linkage of the focal function (cmstypes.c) and expose minimal prototypes sufficient for linking.
- Static/internal specifics of the library are avoided; tests interact via opaque pointers where possible.

Usage:
- Compile this test file together with the cmstypes.c source in your project (and any required headers, e.g., lcms2_internal.h).
- Run the resulting executable to observe test results.

*/

// Compile-as-C++11 test harness for the C focal function Type_Dictionary_Free.
// The focal function is defined in cmstypes.c (C file). We provide C-linkage stubs for testability.

#include <stdio.h>
#include <lcms2_internal.h>
#include <stdint.h>


// Forward declaration of the focal type used by Type_Dictionary_Free
struct _cms_typehandler_struct;

// Prototype of the function under test (C linkage)
extern "C" {
    void Type_Dictionary_Free(struct _cms_typehandler_struct* self, void* Ptr);
}

// We will override cmsDictFree in this test to observe its usage.
// Provide a typedef for cmsHANDLE (commonly void* in CMS code)
typedef void* cmsHANDLE;

// Global test state accessible by the test harness and the cmsDictFree stub
static void* g_last_dictfree_ptr = nullptr;
static int   g_dictfree_call_count = 0;

// Test hook: mock/stub for cmsDictFree to capture call information.
// This is compiled in the test translation unit and linked with cmstypes.c.
// It must match the signature expected by the focal code.
extern "C" void cmsDictFree(cmsHANDLE handle)
{
    g_last_dictfree_ptr = (void*)handle;
    ++g_dictfree_call_count;
}

// Expose a helper to retrieve the captured state from the test harness
extern "C" void GetDictFreeCallInfo(void** out_ptr, int* out_count)
{
    if (out_ptr)  *out_ptr  = g_last_dictfree_ptr;
    if (out_count) *out_count = g_dictfree_call_count;
}

// Lightweight non-terminating test framework (simple EXPECT-like macros)
static int g_total_checks = 0;
static int g_failed_checks = 0;

#define CHECK(cond, desc) do { \
    ++g_total_checks; \
    if(!(cond)) { \
        ++g_failed_checks; \
        fprintf(stderr, "CHECK FAILED: %s (ptr=%p, count=%d, self_ptr_unused=%p)\n", \
                desc, (void*)g_last_dictfree_ptr, g_dictfree_call_count, (void*)0); \
    } \
} while(0)

#define LOG(msg) do { fprintf(stdout, "%s\n", msg); } while(0)

// Forward declaration of test helper functions
static void test_Type_Dictionary_Free_Normal();
static void test_Type_Dictionary_Free_NullPtr();
static void test_Type_Dictionary_Free_SelfNull();
static void test_Type_Dictionary_Free_MultipleCalls();

// Domain knowledge notes embedded as comments for maintainers
// - Core dependency keyword: Type_Dictionary_Free relies on cmsDictFree(Ptr as cmsHANDLE)
// - Ptr is passed as-is after cast in the implementation
// - self is unused (cmsUNUSED_PARAMETER(self)) and should not affect behavior
// - We do not mock private/internal state beyond what is necessary for coverage

int main(void)
{
    LOG("Starting Type_Dictionary_Free test suite (no GTest, non-terminating assertions)");
    test_Type_Dictionary_Free_Normal();
    test_Type_Dictionary_Free_NullPtr();
    test_Type_Dictionary_Free_SelfNull();
    test_Type_Dictionary_Free_MultipleCalls();

    // Summary
    if (g_failed_checks == 0) {
        printf("ALL TESTS PASSED: %d checks\n", g_total_checks);
        return 0;
    } else {
        fprintf(stderr, "TESTS FAILED: %d/%d checks failed\n", g_failed_checks, g_total_checks);
        return 1;
    }
}

// Test: Normal case with non-null Ptr
// Verifies that cmsDictFree is called with the same Ptr and exactly once.
static void test_Type_Dictionary_Free_Normal()
{
    // Reset captured state
    g_last_dictfree_ptr = nullptr;
    g_dictfree_call_count = 0;

    struct _cms_typehandler_struct dummy = {};
    void* test_ptr = (void*)0x12345678;

    // Call the function under test
    Type_Dictionary_Free(&dummy, test_ptr);

    // Retrieve information about cmsDictFree invocation
    void* captured_ptr = nullptr;
    int   captured_count = 0;
    GetDictFreeCallInfo(&captured_ptr, &captured_count);

    // Assertions (non-terminating)
    CHECK(captured_ptr == test_ptr, "Normal: cmsDictFree should receive the exact Ptr passed to Type_Dictionary_Free");
    CHECK(captured_count == 1, "Normal: cmsDictFree should be called exactly once in normal case");

    LOG("Completed test_Type_Dictionary_Free_Normal");
}

// Test: Ptr is nullptr
// Verifies that cmsDictFree is still invoked with nullptr and counted.
static void test_Type_Dictionary_Free_NullPtr()
{
    g_last_dictfree_ptr = nullptr;
    g_dictfree_call_count = 0;

    struct _cms_typehandler_struct dummy = {};
    void* test_ptr = nullptr;

    Type_Dictionary_Free(&dummy, test_ptr);

    void* captured_ptr = nullptr;
    int   captured_count = 0;
    GetDictFreeCallInfo(&captured_ptr, &captured_count);

    CHECK(captured_ptr == nullptr, "Null Ptr: cmsDictFree should receive nullptr when Ptr is NULL");
    CHECK(captured_count == 1, "Null Ptr: cmsDictFree should be called once even for NULL Ptr");

    LOG("Completed test_Type_Dictionary_Free_NullPtr");
}

// Test: self parameter NULL
// Verifies that passing a NULL self does not affect behavior (since cmsUNUSED_PARAMETER should suppress usage).
static void test_Type_Dictionary_Free_SelfNull()
{
    g_last_dictfree_ptr = nullptr;
    g_dictfree_call_count = 0;

    void* test_ptr = (void*)0xDEADBEEF;
    Type_Dictionary_Free(nullptr, test_ptr);

    void* captured_ptr = nullptr;
    int   captured_count = 0;
    GetDictFreeCallInfo(&captured_ptr, &captured_count);

    CHECK(captured_ptr == test_ptr, "Self NULL: Ptr should be passed to cmsDictFree even if self is NULL");
    CHECK(captured_count == 1, "Self NULL: cmsDictFree should be called once");

    LOG("Completed test_Type_Dictionary_Free_SelfNull");
}

// Test: Multiple consecutive calls reflect correct last Ptr and count
static void test_Type_Dictionary_Free_MultipleCalls()
{
    g_last_dictfree_ptr = nullptr;
    g_dictfree_call_count = 0;

    struct _cms_typehandler_struct dummy = {};

    Type_Dictionary_Free(&dummy, (void*)0x1);
    Type_Dictionary_Free(&dummy, (void*)0x2);

    void* captured_ptr = nullptr;
    int   captured_count = 0;
    GetDictFreeCallInfo(&captured_ptr, &captured_count);

    CHECK(captured_count == 2, "MultipleCalls: cmsDictFree should be called twice after two calls");
    CHECK(captured_ptr == (void*)0x2, "MultipleCalls: Last Ptr should reflect the last call (0x2)");

    LOG("Completed test_Type_Dictionary_Free_MultipleCalls");
}