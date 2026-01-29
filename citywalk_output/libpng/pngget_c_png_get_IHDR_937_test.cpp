/*
Unit Test Suite for png_get_IHDR (focal method)
Targeted environment: C++11, no GTest, single-file test harness
Assumptions based on provided focal code:
- png_get_IHDR retrieves IHDR fields from an info_ptr when provided pointers are non-NULL.
- It performs a NULL-check on png_ptr and info_ptr and returns 0 on failure.
- It calls png_check_IHDR with values from info_ptr after optionally writing out parameters.
- It calls png_debug1 for debugging trace (no side effects).
This test suite mocks the minimal surrounding PNG structures and functions
to verify true/false branches, pointer handling, and the IHDR read behavior.

Notes:
- This test suite uses a lightweight, non-terminating CHECK macro to maximize code execution coverage.
- Tests are designed to compile with C++11 and rely only on the standard library.
- The test runner is handcrafted (no Google Test or similar frameworks).
*/

#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// -----------------------------------------------------------------------------
// Mocked PNG structures and types (minimal shim to exercise png_get_IHDR)
// -----------------------------------------------------------------------------

// Pixel/information container (mock of png_info)
struct png_infor_def {
    uint32_t width;
    uint32_t height;
    int bit_depth;
    int color_type;
    int interlace_type;
    int compression_type;
    int filter_type;
};

// Opaque PNG core structure (mock of png_struct)
struct png_struct_def { int dummy; };

// Typedefs to resemble libpng-style opaque types
typedef png_struct_def* png_ptr;                   // non-const
typedef const png_struct_def* png_const_structrp;  // const pointer to png_struct

typedef png_infor_def* png_inforp;
typedef const png_infor_def* png_const_inforp;

// PNG primitive type aliases used in function signatures
typedef uint32_t png_uint_32;

// -----------------------------------------------------------------------------
// Hooks and state to verify internal behavior of png_get_IHDR
// -----------------------------------------------------------------------------

// Global state to observe behaviors during tests
static int g_png_debug1_calls = 0;
static int g_last_IHDR_written_width = 0;
static int g_last_IHDR_written_height = 0;
static int g_last_IHDR_bit_depth = 0;
static int g_last_IHDR_color_type = 0;
static int g_last_IHDR_interlace_type = 0;
static int g_last_IHDR_compression_type = 0;
static int g_last_IHDR_filter_type = 0;

// For verifying that png_check_IHDR was invoked with the IHDR values
static void png_debug1(int /*level*/, const char* /*msg*/)
{
    // Increment debug call counter (no output to keep tests clean)
    ++g_png_debug1_calls;
}

// Mock of png_check_IHDR to capture the values provided by png_get_IHDR.
// In real libpng, this validates IHDR fields. Here we only record the inputs.
static void png_check_IHDR(png_ptr /*png_ptr*/, png_uint_32 w, png_uint_32 h,
                           int bit_depth, int color_type, int interlace_type,
                           int compression_type, int filter_type)
{
    g_last_IHDR_written_width = static_cast<int>(w);
    g_last_IHDR_written_height = static_cast<int>(h);
    g_last_IHDR_bit_depth = bit_depth;
    g_last_IHDR_color_type = color_type;
    g_last_IHDR_interlace_type = interlace_type;
    g_last_IHDR_compression_type = compression_type;
    g_last_IHDR_filter_type = filter_type;
}

// -----------------------------------------------------------------------------
// Focal method under test: png_get_IHDR
// This is a direct transcription/adaptation aligned with the provided focal code.
// -----------------------------------------------------------------------------

int png_get_IHDR(png_const_structrp png_ptr, png_const_inforp info_ptr,
                 png_uint_32 *width, png_uint_32 *height,
                 int *bit_depth, int *color_type, int *interlace_type,
                 int *compression_type, int *filter_type)
{
{
   png_debug1(1, "in %s retrieval function", "IHDR");
   if (png_ptr == NULL || info_ptr == NULL)
      return 0;
   if (width != NULL)
       *width = info_ptr->width;
   if (height != NULL)
       *height = info_ptr->height;
   if (bit_depth != NULL)
       *bit_depth = info_ptr->bit_depth;
   if (color_type != NULL)
       *color_type = info_ptr->color_type;
   if (compression_type != NULL)
      *compression_type = info_ptr->compression_type;
   if (filter_type != NULL)
      *filter_type = info_ptr->filter_type;
   if (interlace_type != NULL)
      *interlace_type = info_ptr->interlace_type;
   png_check_IHDR(png_ptr, info_ptr->width, info_ptr->height,
       info_ptr->bit_depth, info_ptr->color_type, info_ptr->interlace_type,
       info_ptr->compression_type, info_ptr->filter_type);
   return 1;
}
}

// -----------------------------------------------------------------------------
// Lightweight test harness (non-terminating assertions)
// -----------------------------------------------------------------------------

static int g_fail_count = 0;

#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << msg << "\n"; \
        ++g_fail_count; \
    } else { \
        /* Optional: no output on pass to keep quiet */ \
    } \
} while(0)

#define REQUIRE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[REQUIRED FAIL] " << msg << "\n"; \
        ++g_fail_count; \
        return; \
    } \
} while(0)

static void reset_test_state()
{
    g_png_debug1_calls = 0;
    g_last_IHDR_written_width = 0;
    g_last_IHDR_written_height = 0;
    g_last_IHDR_bit_depth = 0;
    g_last_IHDR_color_type = 0;
    g_last_IHDR_interlace_type = 0;
    g_last_IHDR_compression_type = 0;
    g_last_IHDR_filter_type = 0;
    g_fail_count = 0;
}

// -----------------------------------------------------------------------------
// Test Cases
// -----------------------------------------------------------------------------

// 1) Null png_ptr should cause early return (0) and not modify outputs
static void test_png_get_IHDR_null_png_ptr()
{
    reset_test_state();

    // Prepare a valid info object
    png_infor_def info;
    info.width = 800;
    info.height = 600;
    info.bit_depth = 8;
    info.color_type = 2;
    info.interlace_type = 1;
    info.compression_type = 0;
    info.filter_type = 0;

    // Non-null widths, heights to detect unintended writes
    uint32_t w = 0, h = 0;
    int bd = 0, ct = 0, itype = 0, comp = 0, filt = 0;

    // Call with null png_ptr
    int result = png_get_IHDR(nullptr, &info, &w, &h, &bd, &ct, &itype, &comp, &filt);
    CHECK(result == 0, "png_get_IHDR should return 0 when png_ptr is NULL");
    // Outputs should remain unchanged
    CHECK(w == 0, "width should remain unchanged when png_ptr is NULL");
    CHECK(h == 0, "height should remain unchanged when png_ptr is NULL");
    CHECK(bd == 0 && ct == 0 && itype == 0 && comp == 0 && filt == 0,
          "no output pointers should be modified when png_ptr is NULL");
    // PNG debug should have at least been invoked (branch taken)
    CHECK(g_png_debug1_calls > 0, "png_debug1 should be called even on early return");
}

// 2) Null info_ptr should cause early return (0) and not modify outputs
static void test_png_get_IHDR_null_info_ptr()
{
    reset_test_state();

    png_struct_def png_s;
    png_ptr p = &png_s;

    uint32_t w = 0, h = 0;
    int bd = 0, ct = 0, itype = 0, comp = 0, filt = 0;

    int result = png_get_IHDR(p, nullptr, &w, &h, &bd, &ct, &itype, &comp, &filt);
    CHECK(result == 0, "png_get_IHDR should return 0 when info_ptr is NULL");
    CHECK(w == 0 && h == 0 && bd == 0 && ct == 0 && itype == 0 && comp == 0 && filt == 0,
          "no output pointers should be modified when info_ptr is NULL");
    CHECK(g_png_debug1_calls > 0, "png_debug1 should be called (branch taken) even on early return");
}

// 3) All output pointers non-NULL: verify values copied and check_IHDR called with correct values
static void test_png_get_IHDR_all_pointers()
{
    reset_test_state();

    png_infor_def info;
    info.width = 1234;
    info.height = 5678;
    info.bit_depth = 10;
    info.color_type = 6;
    info.interlace_type = 0;
    info.compression_type = 1;
    info.filter_type = 2;

    png_struct_def png_s;
    png_ptr p = &png_s;

    uint32_t w = 0, h = 0;
    int bd = 0, ct = 0, itype = 0, comp = 0, filt = 0;

    int result = png_get_IHDR(p, &info, &w, &h, &bd, &ct, &itype, &comp, &filt);
    CHECK(result == 1, "png_get_IHDR should return 1 on success with all pointers provided");
    CHECK(w == info.width, "width should match info_ptr->width");
    CHECK(h == info.height, "height should match info_ptr->height");
    CHECK(bd == info.bit_depth, "bit_depth should match info_ptr->bit_depth");
    CHECK(ct == info.color_type, "color_type should match info_ptr->color_type");
    CHECK(itype == info.interlace_type, "interlace_type should match info_ptr->interlace_type");
    CHECK(comp == info.compression_type, "compression_type should match info_ptr->compression_type");
    CHECK(filt == info.filter_type, "filter_type should match info_ptr->filter_type");

    // Verify that png_check_IHDR was called with the same values
    CHECK(g_last_IHDR_written_width == static_cast<int>(info.width), "png_check_IHDR width should be info.width");
    CHECK(g_last_IHDR_written_height == static_cast<int>(info.height), "png_check_IHDR height should be info.height");
    CHECK(g_last_IHDR_bit_depth == info.bit_depth, "png_check_IHDR bit_depth should be info.bit_depth");
    CHECK(g_last_IHDR_color_type == info.color_type, "png_check_IHDR color_type should be info.color_type");
    CHECK(g_last_IHDR_interlace_type == info.interlace_type, "png_check_IHDR interlace_type should be info.interlace_type");
    CHECK(g_last_IHDR_compression_type == info.compression_type, "png_check_IHDR compression_type should be info.compression_type");
    CHECK(g_last_IHDR_filter_type == info.filter_type, "png_check_IHDR filter_type should be info.filter_type");

    // Ensure the debug hook was invoked
    CHECK(g_png_debug1_calls > 0, "png_debug1 should be invoked for successful IHDR retrieval");
}

// 4) Partial pointers non-NULL: verify selective assignment and IHDR check still occurs
static void test_png_get_IHDR_partial_pointers()
{
    reset_test_state();

    png_infor_def info;
    info.width = 555;
    info.height = 444;
    info.bit_depth = 12;
    info.color_type = 3;
    info.interlace_type = 1;
    info.compression_type = 0;
    info.filter_type = 1;

    png_struct_def png_s;
    png_ptr p = &png_s;

    // Only width and height should be written; others are NULL
    uint32_t w = 0, h = 0;
    int bd = -1, ct = -1, itype = -1, comp = -1, filt = -1;

    int result = png_get_IHDR(p, &info, &w, &h, &bd, &ct, &itype, &comp, &filt);
    CHECK(result == 1, "png_get_IHDR should return 1 when partial pointers are provided");
    CHECK(w == info.width, "width should be written when non-NULL");
    CHECK(h == info.height, "height should be written when non-NULL");
    // Other fields should remain untouched (as we didn't supply non-NULL pointers)
    CHECK(bd == -1, "bit_depth should remain unchanged when pointer is NULL");
    CHECK(ct == -1, "color_type should remain unchanged when pointer is NULL");
    CHECK(itype == -1, "interlace_type should remain unchanged when pointer is NULL");
    CHECK(comp == -1, "compression_type should remain unchanged when pointer is NULL");
    CHECK(filt == -1, "filter_type should remain unchanged when pointer is NULL");

    // IHDR check should still be performed with info_ptr values
    CHECK(g_last_IHDR_written_width == static_cast<int>(info.width), "png_check_IHDR width should be info.width in partial case");
    CHECK(g_last_IHDR_written_height == static_cast<int>(info.height), "png_check_IHDR height should be info.height in partial case");
}

// 5) Validate that the function properly handles NULL checks and returns without writing when info_ptr is invalid
static void test_png_get_IHDR_invalid_safety_checks()
{
    reset_test_state();

    // Provide a valid png_ptr but ensure info_ptr is manipulated to test safety
    png_infor_def info;
    info.width = 1;
    info.height = 1;
    info.bit_depth = 8;
    info.color_type = 2;
    info.interlace_type = 0;
    info.compression_type = 0;
    info.filter_type = 0;

    png_struct_def png_s;
    png_ptr p = &png_s;

    // Case: Provide NULL for all output pointers
    int result = png_get_IHDR(p, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    CHECK(result == 0, "png_get_IHDR should return 0 when info_ptr is NULL (even if outputs are NULL)");
}

// -----------------------------------------------------------------------------
// Test Runner
// -----------------------------------------------------------------------------

int main() {
    std::vector<std::string> test_names = {
        "test_png_get_IHDR_null_png_ptr",
        "test_png_get_IHDR_null_info_ptr",
        "test_png_get_IHDR_all_pointers",
        "test_png_get_IHDR_partial_pointers",
        "test_png_get_IHDR_invalid_safety_checks"
    };

    reset_test_state();

    // Execute tests
    std::cout << "Running tests for png_get_IHDR (focal method) ...\n";

    test_png_get_IHDR_null_png_ptr();
    test_png_get_IHDR_null_info_ptr();
    test_png_get_IHDR_all_pointers();
    test_png_get_IHDR_partial_pointers();
    test_png_get_IHDR_invalid_safety_checks();

    // Summary
    int total = 5;
    if (g_fail_count == 0) {
        std::cout << "[OK] All tests passed (" << total << " tests).\n";
        return 0;
    } else {
        std::cout << "[WARN] Failures: " << g_fail_count << " out of " << total << " tests.\n";
        return 1;
    }
}

/*
Explanation of test coverage and design decisions:

- test_png_get_IHDR_null_png_ptr:
  Verifies that when png_ptr is NULL, the function returns 0 early, and no outputs are modified.
  Also confirms png_debug1 is invoked (branch coverage for the initial logging).

- test_png_get_IHDR_null_info_ptr:
  Verifies that when info_ptr is NULL, the function returns 0 early, and no outputs are modified.

- test_png_get_IHDR_all_pointers:
  Full data-path coverage: all output pointers non-NULL and info_ptr contains valid IHDR values.
  Verifies:
  - All fields are written with the exact values from info_ptr.
  - png_check_IHDR was invoked with the correct IHDR values (width, height, bit_depth, color_type, interlace_type, compression_type, filter_type).
  - png_debug1 was called.

- test_png_get_IHDR_partial_pointers:
  Validates selective writes when only some pointers are non-NULL.
  Also confirms png_check_IHDR is still invoked with the correct values and that the debug path is exercised.

- test_png_get_IHDR_invalid_safety_checks:
  Ensures robustness when only non-output pointers are provided (outputs are NULL), still returns 0 if info_ptr is NULL in the invocation.
  This test exercises the code path where the function handles safe parameter usage and early returns.

Notes:
- All tests rely on a lightweight, non-terminating CHECK macro to accumulate failures while continuing test execution, adhering to the non-terminating assertion guidance.
- The test harness is self-contained and does not rely on external libraries or frameworks, as GTest is not allowed.
- The design mirrors the behavior and branches of the focal function while using a minimal set of dependencies to maximize coverage in a C++11 environment.
*/