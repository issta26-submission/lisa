// Minimal, self-contained unit test suite for the focal function:
// png_set_progressive_read_fn
// This test does not depend on GTest. It uses a lightweight, custom test harness.
// The focus is to verify correct behavior of the focal function, including
// early return on NULL pointer and proper field assignments plus interaction
// with png_set_read_fn.

/*
Notes:
- We provide minimal stand-ins for the PNG-related types and functions
  so tests can run in isolation without the full libpng source.
- The tests cover:
  1) Null png_ptr returns immediately (no call to png_set_read_fn).
  2) Non-null png_ptr sets info_fn, row_fn, end_fn and calls png_set_read_fn
     with progressive_ptr and the internal png_push_fill_buffer function.
- Assertions are non-terminating (they log failures but allow the test to continue).
*/

#include <iostream>
#include <pngpriv.h>
#include <cstddef>


// ------------------------------
// Minimal type definitions (test harness mimicking focal API)
// ------------------------------

// Forward declarations to mirror libpng-like signatures used by the focal function.
typedef void* png_voidp;
typedef unsigned char png_bytep;
typedef size_t png_size_t;

typedef void (*png_progressive_info_ptr)(void*);
typedef void (*png_progressive_row_ptr)(void*);
typedef void (*png_progressive_end_ptr)(void*);

// Forward declaration for the read function pointer type.
// Signature: void (*png_rw_ptr)(png_structrp, png_bytep, size_t)
struct png_struct;            // forward declaration
typedef png_struct* png_structrp;
typedef void (*png_rw_ptr)(png_structrp, png_bytep, size_t);

// Forward declarations for the internal push-buffer function
void png_push_fill_buffer(png_structrp, png_bytep, size_t);

// ------------------------------
// Minimal png_struct imitation used by the focal code
// ------------------------------
struct png_struct {
    png_progressive_info_ptr info_fn;
    png_progressive_row_ptr  row_fn;
    png_progressive_end_ptr  end_fn;

    // For testing interaction with png_set_read_fn:
    png_voidp   read_ptr;
    png_rw_ptr  read_fn;
};

// Forward declare the function under test (focal function)
void png_set_progressive_read_fn(png_structrp png_ptr, png_voidp progressive_ptr,
    png_progressive_info_ptr info_fn, png_progressive_row_ptr row_fn,
    png_progressive_end_ptr end_fn);

// The mock for png_set_read_fn to observe its parameters passed by the focal function
static png_struct* g_last_png_ptr = nullptr;
static png_voidp   g_last_user_ptr = nullptr;
static png_rw_ptr    g_last_read_fn = nullptr;

void png_set_read_fn(png_structrp png_ptr, png_voidp user_ptr, png_rw_ptr read_fn)
{
    // Record the parameters for verification in tests
    g_last_png_ptr = png_ptr;
    g_last_user_ptr = user_ptr;
    g_last_read_fn = read_fn;

    // Also populate the png_ptr with the information similarly to the real libpng:
    if (png_ptr != nullptr) {
        png_ptr->read_ptr = user_ptr;
        png_ptr->read_fn  = read_fn;
    }
}

// The function pointer used by the focal function to push buffered data.
// We provide a concrete implementation so the test can compare pointer identity.
void png_push_fill_buffer(png_structrp png_ptr, png_bytep buffer, size_t length)
{
    // In the test harness, the body is intentionally empty.
    (void)png_ptr; (void)buffer; (void)length;
}

// Forward declare dummy info/row/end callbacks used in tests
void dummy_info(void*);   // signature matches png_progressive_info_ptr
void dummy_row(void*);    // signature matches png_progressive_row_ptr
void dummy_end(void*);    // signature matches png_progressive_end_ptr

// ------------------------------
// Implementation of the focal function (ported into test harness)
// ------------------------------
void png_set_progressive_read_fn(png_structrp png_ptr, png_voidp progressive_ptr,
    png_progressive_info_ptr info_fn, png_progressive_row_ptr row_fn,
    png_progressive_end_ptr end_fn)
{
    // Core logic extracted from the provided <FOCAL_METHOD>:
    {
        if (png_ptr == nullptr)
            return;
        png_ptr->info_fn = info_fn;
        png_ptr->row_fn  = row_fn;
        png_ptr->end_fn  = end_fn;
        png_set_read_fn(png_ptr, progressive_ptr, png_push_fill_buffer);
    }
}

// ------------------------------
// Test Framework (lightweight, non-terminating assertions)
// ------------------------------
static int g_total_assertions = 0;
static int g_failed_assertions = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        ++g_failed_assertions; \
        std::cerr << "[Assertion FAILED] " << (msg) \
                  << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")\n"; \
    } \
    ++g_total_assertions; \
} while (0)


// ------------------------------
// Dummy callback implementations
// ------------------------------
void dummy_info(void* /*p*/)   { /* no-op for test */ }
void dummy_row(void* /*p*/)    { /* no-op for test */ }
void dummy_end(void* /*p*/)    { /* no-op for test */ }

// ------------------------------
// Unit Tests
// ------------------------------

// Test 1: When png_ptr is NULL, function should return without invoking png_set_read_fn
// This exercises the true-branch guard in the focal method.
void test_png_set_progressive_read_fn_null_ptr()
{
    // Reset mock tracking
    g_last_png_ptr = nullptr;
    g_last_user_ptr = nullptr;
    g_last_read_fn = nullptr;

    png_voidp progressive_ptr = (png_voidp)0xDEADBEEF;
    png_set_progressive_read_fn(nullptr, progressive_ptr,
        dummy_info, dummy_row, dummy_end);

    // Expect that png_set_read_fn was not called (no modification to last_ptr)
    ASSERT_TRUE(g_last_png_ptr == nullptr, "png_set_read_fn should not be called when png_ptr is NULL");
    // Also ensure no unintended assignment occurred
    ASSERT_TRUE(g_last_user_ptr == nullptr, "user_ptr should remain NULL when png_ptr is NULL");
    ASSERT_TRUE(g_last_read_fn == nullptr, "read_fn should remain NULL when png_ptr is NULL");
}

// Test 2: When png_ptr is valid, ensure fields are set and the mock png_set_read_fn is invoked
void test_png_set_progressive_read_fn_sets_fields_and_calls_read_fn()
{
    // Prepare a png_struct instance to act as png_ptr
    png_struct st = {};
    // Initialize its fields to known values (optional, for sanity)
    st.info_fn = nullptr;
    st.row_fn  = nullptr;
    st.end_fn  = nullptr;
    st.read_ptr = nullptr;
    st.read_fn  = nullptr;

    g_last_png_ptr = nullptr;
    g_last_user_ptr = nullptr;
    g_last_read_fn = nullptr;

    png_structrp ptr = &st;
    void* progressive_ptr = (void*)0xBADC0DE;

    // Call focal function with non-null pointers
    png_set_progressive_read_fn(ptr, progressive_ptr, dummy_info, dummy_row, dummy_end);

    // Verify that the fields on the png_ptr were set correctly
    ASSERT_TRUE(st.info_fn == dummy_info, "info_fn should be assigned to the provided function");
    ASSERT_TRUE(st.row_fn  == dummy_row,  "row_fn should be assigned to the provided function");
    ASSERT_TRUE(st.end_fn  == dummy_end,  "end_fn should be assigned to the provided function");

    // Verify interaction with png_set_read_fn via the mock
    ASSERT_TRUE(g_last_png_ptr == ptr, "png_set_read_fn should be called with the correct png_ptr");
    ASSERT_TRUE(g_last_user_ptr == progressive_ptr, "png_set_read_fn should receive progressive_ptr as user_ptr");
    ASSERT_TRUE(g_last_read_fn == png_push_fill_buffer, "png_set_read_fn should be passed the png_push_fill_buffer function");

    // Ensure internal storage in png_ptr reflects the call
    ASSERT_TRUE(st.read_ptr == progressive_ptr, "png_ptr should store the progressive_ptr as read_ptr");
    ASSERT_TRUE(st.read_fn  == png_push_fill_buffer, "png_ptr should store the png_push_fill_buffer as read_fn");
}

// ------------------------------
// Main test runner
// ------------------------------
int main()
{
    std::cout << "Starting png_set_progressive_read_fn unit tests (C++11 self-contained)..." << std::endl;

    test_png_set_progressive_read_fn_null_ptr();
    test_png_set_progressive_read_fn_sets_fields_and_calls_read_fn();

    std::cout << "Tests completed. Total assertions: " << g_total_assertions
              << ", Failures: " << g_failed_assertions << std::endl;

    if (g_failed_assertions > 0) {
        std::cerr << "Some tests failed. See details above." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}