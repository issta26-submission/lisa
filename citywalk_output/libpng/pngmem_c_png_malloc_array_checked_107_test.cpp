// Test harness for png_malloc_array_checked in pngmem.c
// This test is designed to compile under C++11 without GoogleTest.
// It relies on the project's pngpriv.h and the pngmem.c implementation in the
// same project. A lightweight C-style mock for png_malloc_base is provided to
// observe allocation behavior without touching the real allocator.

#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <cstring>
#include <limits>


// It is assumed that the project provides pngpriv.h with the following
// typedefs and declarations (as used by pngmem.c):
//   typedef void* png_voidp;
//   typedef const void* png_const_structrp;
//   typedef void* png_structrp;
//   typedef unsigned long long png_alloc_size_t;  // or equivalent
//   #define PNG_SIZE_MAX ...                         // defined in the project
//   png_voidp png_malloc_array_checked(png_const_structrp, int, size_t);
//   // plus other internal declarations used by pngmem.c



// Forward declaration of the function under test in pngmem.c
extern "C" png_voidp png_malloc_array_checked(png_const_structrp png_ptr,
                                             int nelements,
                                             size_t element_size);

// Provide a C-compatible mock for the underlying memory allocator used by
// png_malloc_array_checked. The real pngmem.c will call png_malloc_base(png_ptr, size)
// to perform allocation when the request is within limits. We override that symbol
// to capture the last allocation request and return a malloc'ed pointer.
extern "C" png_voidp png_malloc_base(png_const_structrp png_ptr, png_alloc_size_t size);

// Global hooks to observe allocator behavior from tests
static png_alloc_size_t g_last_alloc_size = 0;
static png_voidp       g_last_alloc_ptr  = NULL;

// Implementation of the mock allocator used by the tests.
// It records the requested allocation size and returns a usable pointer.
extern "C" png_voidp png_malloc_base(png_const_structrp png_ptr, png_alloc_size_t size)
{
    (void)png_ptr; // parameter is unused by the test mock
    g_last_alloc_size = size;

    // Guard against zero-size allocations by allocating at least 1 byte.
    size_t alloc_size = (size > 0) ? static_cast<size_t>(size) : 1;
    g_last_alloc_ptr = std::malloc(alloc_size);
    return g_last_alloc_ptr;
}

// Simple non-terminating test assertion helpers.
// They log failures but do not exit, allowing multiple checks per test.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failed_tests; \
    } else { \
        std::cout << "[PASS] " << (msg) << "\n"; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    ++g_total_tests; \
    if((a) != (b)) { \
        std::cerr << "[FAIL] " << (msg) << " - pointers differ: " << (void*)(a) << " != " << (void*)(b) \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failed_tests; \
    } else { \
        std::cout << "[PASS] " << (msg) << "\n"; \
    } \
} while(0)

// Test 1: True branch when the requested size fits within the allowed maximum.
// Preconditions:
// - nelements = 1
// - element_size = 1
// Expectation:
// - A non-NULL pointer is returned.
// - The allocator was invoked with size = 1.
// - The returned pointer equals the pointer observed by the allocator.
static void test_true_branch_allocation()
{
    std::cout << "Running test_true_branch_allocation: expect successful allocation.\n";

    // Reset allocator observation
    g_last_alloc_size = 0;
    g_last_alloc_ptr = NULL;
    // Call the function under test
    png_voidp result = png_malloc_array_checked(nullptr, 1, 1);

    // Validate results
    EXPECT(result != NULL, "png_malloc_array_checked should allocate for nelements=1, element_size=1");
    EXPECT(g_last_alloc_size == 1, "Allocator should be invoked with size 1");
    EXPECT_PTR_EQ(result, g_last_alloc_ptr, "Returned pointer should equal allocator's pointer");

    // Cleanup any allocated memory
    if (result != NULL) {
        std::free(result);
    }
}

// Test 2: False branch when the requested size is too large to allocate.
// Approach:
// - Use a very large element_size to force the condition (PNG_SIZE_MAX/element_size) to be 0.
// - We select element_size as the maximum size_t value to make the quotient 0 (assuming PNG_SIZE_MAX < SIZE_MAX).
// - nelements is kept at a small value (1) to ensure the calculation would overflow the allowed maximum.
// Expectation:
// - Function returns NULL, indicating allocation was not attempted.
static void test_false_branch_no_allocation()
{
    std::cout << "Running test_false_branch_no_allocation: expect NULL when request is too large.\n";

    // Reset allocator observation
    g_last_alloc_size = 0;
    g_last_alloc_ptr = NULL;

    // Large element_size to trigger the false branch reliably across common PNG_SIZE_MAX values
    size_t large_size = std::numeric_limits<size_t>::max();
    png_voidp result = png_malloc_array_checked(nullptr, 1, large_size);

    // Validate results
    EXPECT(result == NULL, "png_malloc_array_checked should return NULL for excessively large request");
    // The allocator should not have been invoked in this path
    EXPECT(g_last_alloc_ptr == NULL, "Allocator should not be called in false branch");
}

// Main test runner
int main()
{
    // Run tests
    test_true_branch_allocation();
    test_false_branch_no_allocation();

    // Summary
    if (g_failed_tests == 0) {
        std::cout << "All tests passed. (" << g_total_tests << " tests executed)\n";
        return 0;
    } else {
        std::cerr << g_failed_tests << " of " << g_total_tests
                  << " tests FAILED. (Summary printed above)\n";
        return 1;
    }
}