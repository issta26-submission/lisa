// Candidate Keywords gathered from the focal method and its dependencies:
// - png_const_structrp, png_ptr (pointer to a const png_struct)
// - png_struct (the underlying object containing state, including user_chunk_ptr)
// - user_chunk_ptr (the dependent field returned by the function)
// - NULL (null pointer handling)
// - conditional path: non-null png_ptr vs null png_ptr
// - pointer dereference (png_ptr->user_chunk_ptr)
// - return type: void* (png_voidp in libpng conventions, here treated as generic pointer)

#include <iostream>
#include <pngpriv.h>


// Minimal local reproduction of the relevant parts of the C API to test the focal method.
// The real project would rely on libpng, but for isolated unit testing in C++11 without GTest,
// we re-create just enough type definitions to compile and exercise the function logic.

struct png_struct {
    void* user_chunk_ptr; // The internal payload we expose via the accessor
};

// Define the opaque const pointer type used by the focal function.
// This mirrors the idea of a const pointer to a png_struct in the actual library.
typedef const png_struct* png_const_structrp;

// The focal method under test, implemented in C-like style.
// Intentionally mirrors the provided implementation (double-brace style preserved from the source).
void* png_get_user_chunk_ptr(png_const_structrp png_ptr)
{
{
   return (png_ptr ? png_ptr->user_chunk_ptr : NULL);
}
}

// Lightweight simple test harness (no external test framework):
// - Provides non-terminating assertions (logs failures but keeps execution going)
// - Keeps a running count of tests and failures
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define ASSERT(condition, message)                                  \
    do {                                                              \
        ++g_total_tests;                                              \
        if (!(condition)) {                                           \
            ++g_failed_tests;                                         \
            std::cerr << "Assertion failed: " << (message)          \
                      << " (in " << __FILE__ << ":" << __LINE__     \
                      << ")" << std::endl;                          \
        }                                                             \
    } while (0)

// Test 1: Non-null input with a non-null user_chunk_ptr should return the same address.
static void test_png_get_user_chunk_ptr_non_null_input_returns_same_ptr()
{
    // Setup: allocate a dummy payload and attach it to the png_struct
    int dummy_payload;
    png_struct s;
    s.user_chunk_ptr = &dummy_payload;

    png_const_structrp p = &s;
    void* result = png_get_user_chunk_ptr(p);

    // Expect the same pointer value to be returned
    ASSERT(result == &dummy_payload, "png_get_user_chunk_ptr should return the exact user_chunk_ptr when png_ptr is non-NULL");
}

// Test 2: Null input should return NULL.
static void test_png_get_user_chunk_ptr_null_input_returns_null()
{
    png_const_structrp p = NULL;
    void* result = png_get_user_chunk_ptr(p);

    // Expect NULL result when input pointer is NULL
    ASSERT(result == NULL, "png_get_user_chunk_ptr should return NULL when input is NULL");
}

// Test 3: Non-null input but with a NULL user_chunk_ptr field should return NULL.
static void test_png_get_user_chunk_ptr_non_null_input_null_field_returns_null()
{
    png_struct s;
    s.user_chunk_ptr = NULL; // Field explicitly NULL
    png_const_structrp p = &s;
    void* result = png_get_user_chunk_ptr(p);

    // Expect NULL result when the field is NULL
    ASSERT(result == NULL, "png_get_user_chunk_ptr should return NULL if png_ptr->user_chunk_ptr is NULL");
}

// Test 4: Ensure true/false branches are exercised by alternating inputs.
static void test_png_get_user_chunk_ptr_branch_coverage()
{
    // Case A: non-NULL pointer with non-NULL field
    int valA;
    png_struct sA;
    sA.user_chunk_ptr = &valA;
    void* rA = png_get_user_chunk_ptr(&sA);
    ASSERT(rA == &valA, "Branch: non-NULL input with non-NULL field should return non-NULL pointer");

    // Case B: NULL input
    void* rB = png_get_user_chunk_ptr(NULL);
    ASSERT(rB == NULL, "Branch: NULL input should yield NULL");
}

// Helper to run all tests
static void run_all_tests()
{
    test_png_get_user_chunk_ptr_non_null_input_returns_same_ptr();
    test_png_get_user_chunk_ptr_null_input_returns_null();
    test_png_get_user_chunk_ptr_non_null_input_null_field_returns_null();
    test_png_get_user_chunk_ptr_branch_coverage();
}

// Entry point
int main()
{
    run_all_tests();

    std::cout << "Total tests run: " << g_total_tests << std::endl;
    if (g_failed_tests == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Tests failed: " << g_failed_tests << std::endl;
        return 1;
    }
}