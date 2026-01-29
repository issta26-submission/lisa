// C++11 test suite for _cmsSubAllocDup in cmserr.c
// Notes:
// - This test suite exercises the focal function _cmsSubAllocDup by calling into the
//   library's internal allocator API (_cmsSubAllocDup, _cmsSubAlloc, _cmsCreateSubAlloc,
//   _cmsSubAllocDestroy).
// - The tests are implemented without Google Test; a lightweight in-file test harness is used.
// - We assume the project provides the necessary C interfaces via lcms2_internal.h. If not,
//   we provide minimal forward declarations compatible with the expected C linkage.
// - Tests cover true/false branches of predicates and verify memory copy behavior.
// - Static members and private details from the original program are not accessed directly.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Domain knowledge: define C linkage for the internal CMS functions/types if not declared by headers.
extern "C" {

// Forward declarations for internal structures/types (opaque to tests)
typedef struct _cmsSubAllocator _cmsSubAllocator;
typedef struct _cmsContext_struct cmsContext;
typedef unsigned int cmsUInt32Number;

// Prototypes of internal functions (as used within the focal file)
_cmsSubAllocator* _cmsCreateSubAlloc(cmsContext ContextID, cmsUInt32Number Initial);
void* _cmsSubAlloc(_cmsSubAllocator* sub, cmsUInt32Number size);
void _cmsSubAllocDestroy(_cmsSubAllocator* sub);
void* _cmsSubAllocDup(_cmsSubAllocator* s, const void *ptr, cmsUInt32Number size);

} // extern "C"

static int g_failures = 0;

// Simple assertion helper to avoid terminating tests prematurely
#define TEST_ASSERT(cond, msg)                                   \
    do {                                                           \
        if (!(cond)) {                                             \
            std::cerr << "TEST_FAIL: " << msg                    \
                      << " (" << __FILE__ << ":" << __LINE__ << ")" \
                      << std::endl;                                \
            ++g_failures;                                         \
        } else {                                                   \
            std::cout << "TEST_PASS: " << msg << std::endl;      \
        }                                                          \
    } while (0)


// Test 1: Dup of a NULL pointer should return NULL (true branch)
void test_subAllocDup_null_ptr_returns_null()
{
    // Given: a NULL source pointer, and arbitrary allocator pointer (nullptr is fine since
    // the early return happens before using the allocator).
    void* result = _cmsSubAllocDup(nullptr, nullptr, 16);

    // Then: the function should return NULL and not touch any allocator.
    TEST_ASSERT(result == NULL, "Dup of NULL pointer returns NULL");
}

// Test 2: When ptr is non-NULL and allocation succeeds, content should be copied.
// This tests both: (ptr != NULL) and (NewPtr != NULL) branches.
void test_subAllocDup_copies_content_on_success()
{
    // Create a small sub-allocator
    _cmsSubAllocator* s = _cmsCreateSubAlloc(reinterpret_cast<cmsContext>(nullptr), 1024);
    TEST_ASSERT(s != nullptr, "Create sub-allocator for content copy test");

    const char src[8] = { 'T','e','s','t','D','u','p','1' };
    void* dst = _cmsSubAllocDup(s, static_cast<const void*>(src), 8);

    TEST_ASSERT(dst != nullptr, "Dup allocated memory (NewPtr != NULL) and returned non-NULL");
    if (dst != nullptr) {
        // Verify the copied content exactly matches the source
        int cmp = std::memcmp(dst, src, 8);
        TEST_ASSERT(cmp == 0, "Copied content matches source data");
    }

    // Cleanup
    _cmsSubAllocDestroy(s);
}

// Test 3: When allocation fails (NewPtr == NULL), _cmsSubAllocDup should return NULL.
// We attempt to force allocation failure by requesting an unrealistically large size.
void test_subAllocDup_allocation_failure_returns_null()
{
    _cmsSubAllocator* s = _cmsCreateSubAlloc(reinterpret_cast<cmsContext>(nullptr), 4);
    TEST_ASSERT(s != nullptr, "Create sub-allocator for allocation-failure test");
    if (s == nullptr) {
        // If allocator creation failed, we cannot proceed with this test reliably.
        // Mark as pass for environments where allocator cannot be created.
        return;
    }

    const char src[8] = { 'F','a','i','l','u','r','e','!' };
    // Use a very large size to encourage failure of allocation within the sub-allocator.
    void* dst = _cmsSubAllocDup(s, static_cast<const void*>(src), 0x80000000u);

    TEST_ASSERT(dst == nullptr, "Dup returns NULL when allocation fails");

    // Cleanup
    _cmsSubAllocDestroy(s);
}


// Main test runner
int main()
{
    std::cout << "Starting _cmsSubAllocDup unit tests (C++11 harness)" << std::endl;

    test_subAllocDup_null_ptr_returns_null();
    test_subAllocDup_copies_content_on_success();
    test_subAllocDup_allocation_failure_returns_null();

    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}