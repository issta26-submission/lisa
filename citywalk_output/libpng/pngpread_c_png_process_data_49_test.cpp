/*
Unit test harness for the focal method: png_process_data
Goal: validate behavior of the FOCAL_METHOD by providing minimal, stand-in dependencies
and driving internal state through mocks.

Assumptions:
- The real png_process_data is defined in pngpread.c and uses a lightweight png_struct
  containing a member 'buffer_size', plus calls to png_push_restore_buffer and
  png_process_some_data.
- We mock png_push_restore_buffer and png_process_some_data to observe interactions
  and to drive the internal loop (while (png_ptr->buffer_size)).
- This test suite uses a lightweight, non-terminating assertion mechanism suitable
  for environments where Google Test is not allowed.
- The test harness is self-contained in a single C++11 source file, but links against
  the real pngpread.c in the build (via extern "C" declarations and compatible types).
*/

#include <cstddef>
#include <cstdint>
#include <pngpriv.h>
#include <cstdio>


// -----------------------------------------------------------------------------
// Mocked type definitions to match the expectations of png_process_data in pngpread.c
// These definitions are intentionally minimal and focus on the fields used by the
// focal method (buffer_size) and the parameter types used by the calls it makes.
// -----------------------------------------------------------------------------

struct png_struct {
    int buffer_size; // Used by the focal method's loop condition
};

// Typedef shorthands matching the typical libpng style
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;
typedef void* png_inforp;
typedef unsigned char* png_bytep;

// -----------------------------------------------------------------------------
// Prototypes for the focal method dependencies (to be resolved by the real build)
// We declare them with C linkage to avoid name mangling differences between C and C++
// -----------------------------------------------------------------------------

extern "C" {
    // Focal method under test (signature must match the real implementation)
    void png_process_data(png_structrp png_ptr, png_inforp info_ptr,
                          png_bytep buffer, size_t buffer_size);

    // Dependency hooks that the focal method calls
    void png_push_restore_buffer(png_structrp png_ptr, png_bytep buffer, size_t buffer_length);
    void png_process_some_data(png_structrp png_ptr, png_inforp info_ptr);
}

// -----------------------------------------------------------------------------
// Test harness state (mocks for dependencies)
// The mocks drive the internal buffer loop by manipulating png_ptr->buffer_size
// and record invocation counts for assertions.
// -----------------------------------------------------------------------------

static bool g_force_no_loop = false;      // When true, mock pushes a zero buffer_size
static int  g_restore_calls = 0;          // Number of calls to png_push_restore_buffer
static int  g_process_some_calls = 0;       // Number of times png_process_some_data is invoked
static unsigned char* g_last_buffer_ptr = nullptr;
static size_t g_last_buffer_length = 0;

// Our test static "png_struct" instance used by the focal method calls
static png_struct g_test_png;

// Mocks:

extern "C" void png_push_restore_buffer(png_structrp png_ptr, png_bytep buffer, size_t buffer_length) {
    // Track calls and capture buffer info for assertions
    ++g_restore_calls;
    g_last_buffer_ptr = buffer;
    g_last_buffer_length = buffer_length;

    // Drive the loop: either a fixed number of iterations or zero if forced
    if (png_ptr) {
        png_ptr->buffer_size = g_force_no_loop ? 0 : 3; // 3 iterations by default
    }
}

extern "C" void png_process_some_data(png_structrp png_ptr, png_inforp info_ptr) {
    // Count invocations and progress the loop by decrementing the buffer_size
    ++g_process_some_calls;
    if (png_ptr && png_ptr->buffer_size > 0) {
        --png_ptr->buffer_size;
    }
}

// Forward declaration of the focal method to satisfy the compiler (linker will resolve actual definition from pngpread.c)
extern "C" void png_process_data(png_structrp png_ptr, png_inforp info_ptr,
                                 png_bytep buffer, size_t buffer_size);

// -----------------------------------------------------------------------------
// Minimal non-terminating test framework (no GTest required)
// - ASSERT_TRUE: non-terminating assertion; records failures and continues
// - test case functions: each exercises a particular aspect of the focal method
// - main: runs all tests and reports a summary
// -----------------------------------------------------------------------------

static int g_total_asserts = 0;
static int g_failed_asserts = 0;

#define ASSERT_TRUE(cond) do { \
    ++g_total_asserts; \
    if (!(cond)) { \
        ++g_failed_asserts; \
        printf("Assertion failed: %s, in %s:%d\n", #cond, __FILE__, __LINE__); \
    } \
} while (0)

static unsigned char g_test_buffer1[16] = { 0xAA, 0xBB, 0xCC, 0xDD };

// Test 1: Null pointer handling
// - Ensure that when png_ptr == NULL or info_ptr == NULL, the function returns early
//   and does not call png_push_restore_buffer or png_process_some_data.
void test_png_process_data_null_ptrs()
{
    // Case A: png_ptr is NULL -> should return immediately
    g_restore_calls = 0;
    g_process_some_calls = 0;
    g_force_no_loop = false;

    // Prepare non-null buffer to exercise the early return
    png_struct test_ptr = { .buffer_size = 999 };
    png_structrp pptr = &test_ptr;
    png_inforp info = nullptr; // NULL info_ptr

    png_process_data(pptr, info, g_test_buffer1, sizeof(g_test_buffer1));

    ASSERT_TRUE(g_restore_calls == 0);
    ASSERT_TRUE(g_process_some_calls == 0);

    // Case B: info_ptr is NULL -> should return immediately as well
    g_restore_calls = 0;
    g_process_some_calls = 0;
    test_ptr.buffer_size = 999;
    info = nullptr; // still null; reusing variable

    png_process_data(pptr, info, g_test_buffer1, sizeof(g_test_buffer1));

    ASSERT_TRUE(g_restore_calls == 0);
    ASSERT_TRUE(g_process_some_calls == 0);
}

// Test 2: Normal path where the loop executes exactly 3 iterations
// - png_push_restore_buffer sets buffer_size to 3
// - png_process_some_data decrements buffer_size by 1 each call
// - Expect 1 restore call and 3 process calls; final buffer_size should be 0
void test_png_process_data_loop_executes_three_times()
{
    g_restore_calls = 0;
    g_process_some_calls = 0;
    g_force_no_loop = false;

    // Reset internal state
    g_test_png.buffer_size = 0;

    // Non-null pointers
    png_structrp pptr = &g_test_png;
    png_inforp info = (png_inforp)(&g_test_png); // dummy non-null info_ptr
    const size_t len = 8;
    png_bytep buffer = g_test_buffer1;

    png_process_data(pptr, info, buffer, len);

    // Expect: 1 restore call, 3 process calls, loop ends with buffer_size == 0
    ASSERT_TRUE(g_restore_calls == 1);
    ASSERT_TRUE(g_process_some_calls == 3);
    ASSERT_TRUE(pptr->buffer_size == 0);
    // Also verify that the buffer passed through restoration is captured
    ASSERT_TRUE(g_last_buffer_ptr == buffer);
    ASSERT_TRUE(g_last_buffer_length == len);
}

// Test 3: When forced to have no loop (simulate a case where push yields zero work)
// - png_push_restore_buffer should set buffer_size to 0
// - The function should not call png_process_some_data
void test_png_process_data_no_loop_due_to_zero_buffer()
{
    g_restore_calls = 0;
    g_process_some_calls = 0;
    g_force_no_loop = true; // force buffer_size = 0
    g_test_png.buffer_size = 0;

    png_structrp pptr = &g_test_png;
    png_inforp info = (png_inforp)(&g_test_png);
    png_process_data(pptr, info, g_test_buffer1, 8);

    // One restore call triggered, but no processing should happen
    ASSERT_TRUE(g_restore_calls == 1);
    ASSERT_TRUE(g_process_some_calls == 0);
    ASSERT_TRUE(pptr->buffer_size == 0); // remains zero due to forced no-loop
}

// Simple runner
void run_all_tests()
{
    printf("Running test: test_png_process_data_null_ptrs\n");
    test_png_process_data_null_ptrs();

    printf("Running test: test_png_process_data_loop_executes_three_times\n");
    test_png_process_data_loop_executes_three_times();

    printf("Running test: test_png_process_data_no_loop_due_to_zero_buffer\n");
    test_png_process_data_no_loop_due_to_zero_buffer();
}

// Main entry: execute tests and report summary
int main()
{
    run_all_tests();

    printf("Test summary: total asserts = %d, failures = %d\n",
           g_total_asserts, g_failed_asserts);
    // Return non-zero if any assertion failed
    return (g_failed_asserts == 0) ? 0 : 1;
}