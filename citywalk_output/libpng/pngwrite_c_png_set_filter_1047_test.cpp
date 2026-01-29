// Single-file C++11 unit test suite for png_set_filter (libpng-like implementation)
//
// This test suite provides a lightweight, self-contained environment to
// exercise the focal method png_set_filter as described in the prompt.
// It implements minimal stubs and a tiny test harness (no GTest) and uses
// only the C++ standard library. Each test includes explanatory comments.
//
// Important notes:
// - The test harness purposely avoids terminating the test process on failures,
//   instead collecting and reporting failures to stdout.
// - The implementation below is self-contained and does not require external
//   libpng installations (it provides the necessary types/constants/macros and
//   a simplified png_set_filter inline with the provided snippet).
// - The tests cover true/false branches, memory allocation behavior, error/warning
//   paths, and boundary conditions (NULL pointers, unknown methods, multi-filter
//   scenarios, etc.).

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <stdexcept>
#include <cstring>


// ------------------------
// Minimal libpng-like types
// ------------------------

typedef unsigned char png_byte;
typedef unsigned int  png_uint_32;
typedef void*           png_voidp;
typedef unsigned char*  png_bytep;
typedef const unsigned char* png_const_bytep;

// Forward declare to emulate "png_structrp" typedef in libpng
struct png_struct;
typedef struct png_struct* png_structrp;

// Helper macro (compatibility shim)
#define png_voidcast(type, value) ((type)(value))

// We define a simplified PNG_ROWBYTES macro equivalent used in png_set_filter
static inline size_t PNG_ROWBYTES(size_t bits_per_pixel, int width)
{
    // bits_per_pixel is in bits; compute bytes per row: ceil(bits_per_pixel/8) * width
    // A straightforward (bits_per_pixel * width + 7) / 8
    return (bits_per_pixel * (size_t)width + 7) / 8;
}

// ------------------------
// Focal constants (subset sufficient for tests)
// ------------------------

#define PNG_FILTER_TYPE_BASE 0
#define PNG_FILTER_NONE       0
#define PNG_FILTER_SUB        1
#define PNG_FILTER_UP           2
#define PNG_FILTER_AVG          3
#define PNG_FILTER_PAETH        4
#define PNG_ALL_FILTERS       0x07
#define PNG_FILTER_VALUE_NONE   0
#define PNG_FLAG_MNG_FILTER_64   0x01

#define PNG_INTRAPIXEL_DIFFERENCING 1

// We enable write-filter support to exercise the branches that map and allocate
#define PNG_WRITE_FILTER_SUPPORTED 1
#define PNG_MNG_FEATURES_SUPPORTED 0  // not used in tests; keep off for simplicity

// ------------------------
// Global test helpers
// ------------------------

static std::string last_warning;

// Minimal logging functions (stubs)
static void png_debug(int level, const char* /*message*/)
{
    // No-op for tests
    (void)level;
}
static void png_app_warning(png_structrp /*png_ptr*/, const char* warn)
{
    // Capture last warning for assertions
    last_warning = warn ? warn : "";
}
static void png_app_error(png_structrp /*png_ptr*/, const char* err)
{
    // Treat as non-recoverable error to be caught by tests
    throw std::runtime_error(err ? err : "png_app_error");
}
static void png_error(png_structrp /*png_ptr*/, const char* err)
{
    // Propagate as exception to allow test harness to verify error paths
    throw std::runtime_error(err ? err : "png_error");
}
static void* png_malloc(png_structrp /*png_ptr*/, size_t size)
{
    return std::malloc(size);
}
static void png_free(png_structrp /*png_ptr*/, void* ptr)
{
    std::free(ptr);
}
static void* png_voidcast_mem(png_structrp /*png_ptr*/, void* p)
{
    return p;
}

// ------------------------
// Focal data structure (simplified)
// ------------------------

struct png_struct {
    int mng_features_permitted;
    unsigned char do_filter;
    unsigned char* row_buf;
    unsigned char* prev_row;
    unsigned char* try_row;
    unsigned char* tst_row;
    int width;
    int height;
    int usr_channels;
    int usr_bit_depth;
};

// Declaration of the focal function (as per provided snippet)
static void png_set_filter(png_structrp png_ptr, int method, int filters);

// ------------------------
// Focal function implementation (self-contained, mirrors provided snippet)
// ------------------------

static void png_set_filter(png_structrp png_ptr, int method, int filters)
{
{
   png_debug(1, "in png_set_filter");
   if (png_ptr == NULL)
      return;
#if PNG_MNG_FEATURES_SUPPORTED
   if ((png_ptr->mng_features_permitted & PNG_FLAG_MNG_FILTER_64) != 0 &&
       (method == PNG_INTRAPIXEL_DIFFERENCING))
      method = PNG_FILTER_TYPE_BASE;
#endif
   if (method == PNG_FILTER_TYPE_BASE)
   {
      switch (filters & (PNG_ALL_FILTERS | 0x07))
      {
#if PNG_WRITE_FILTER_SUPPORTED
         case 5:
         case 6:
         case 7: png_app_error(png_ptr, "Unknown row filter for method 0");
#endif 
         case PNG_FILTER_VALUE_NONE:
            png_ptr->do_filter = PNG_FILTER_NONE; break;
#if PNG_WRITE_FILTER_SUPPORTED
         case PNG_FILTER_VALUE_SUB:
            png_ptr->do_filter = PNG_FILTER_SUB; break;
         case PNG_FILTER_VALUE_UP:
            png_ptr->do_filter = PNG_FILTER_UP; break;
         case PNG_FILTER_VALUE_AVG:
            png_ptr->do_filter = PNG_FILTER_AVG; break;
         case PNG_FILTER_VALUE_PAETH:
            png_ptr->do_filter = PNG_FILTER_PAETH; break;
         default:
            png_ptr->do_filter = (png_byte)filters; break;
#else
         default:
            png_app_error(png_ptr, "Unknown row filter for method 0");
#endif 
      }
#if PNG_WRITE_FILTER_SUPPORTED
      if (png_ptr->row_buf != NULL)
      {
         int num_filters;
         size_t buf_size;
         if (png_ptr->height == 1)
            filters &= ~(PNG_FILTER_UP|PNG_FILTER_AVG|PNG_FILTER_PAETH);
         if (png_ptr->width == 1)
            filters &= ~(PNG_FILTER_SUB|PNG_FILTER_AVG|PNG_FILTER_PAETH);
         if ((filters & (PNG_FILTER_UP|PNG_FILTER_AVG|PNG_FILTER_PAETH)) != 0
            && png_ptr->prev_row == NULL)
         {
            png_app_warning(png_ptr,
                "png_set_filter: UP/AVG/PAETH cannot be added after start");
            filters &= ~(PNG_FILTER_UP|PNG_FILTER_AVG|PNG_FILTER_PAETH);
         }
         num_filters = 0;
         if (filters & PNG_FILTER_SUB)
            num_filters++;
         if (filters & PNG_FILTER_UP)
            num_filters++;
         if (filters & PNG_FILTER_AVG)
            num_filters++;
         if (filters & PNG_FILTER_PAETH)
            num_filters++;
         /* Allocate needed row buffers if they have not already been
          * allocated.
          */
         buf_size = PNG_ROWBYTES(png_ptr->usr_channels * png_ptr->usr_bit_depth,
             png_ptr->width) + 1;
         if (png_ptr->try_row == NULL)
            png_ptr->try_row = png_voidcast(png_bytep,
                png_malloc(png_ptr, buf_size));
         if (num_filters > 1)
         {
            if (png_ptr->tst_row == NULL)
               png_ptr->tst_row = png_voidcast(png_bytep,
                   png_malloc(png_ptr, buf_size));
         }
      }
      png_ptr->do_filter = (png_byte)filters;
#endif
   }
   else
      png_error(png_ptr, "Unknown custom filter method");
}
}

// ------------------------
// Lightweight test harness
// ------------------------

typedef bool (*TestFunc)();

static bool test_passed = true;

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " (condition: " #cond ").\n"; \
        test_passed = false; \
        return false; \
    } \
} while (0)

static void reset_warning() { last_warning.clear(); }

// Helper to construct a test PNG struct with given parameters
static png_struct* make_ptr(int width, int height, int channels, int bit_depth,
                            bool row_buf_nonnull, bool prev_row_nonnull)
{
    png_struct* p = new png_struct();
    p->mng_features_permitted = 0;
    p->do_filter = 0;
    p->row_buf = row_buf_nonnull ? nullptr : nullptr; // We'll allocate explicitly when needed
    p->prev_row = prev_row_nonnull ? nullptr : nullptr; // will allocate if needed
    p->try_row = nullptr;
    p->tst_row = nullptr;
    p->width = width;
    p->height = height;
    p->usr_channels = channels;
    p->usr_bit_depth = bit_depth;
    // If a row_buf is required for the test, allocate a dummy buffer and assign
    if (row_buf_nonnull) {
        // Allocate a small non-NULL buffer to trigger allocation path
        p->row_buf = (unsigned char*)malloc(64);
        // In tests we will also optionally allocate prev_row
        if (prev_row_nonnull) {
            p->prev_row = (unsigned char*)malloc(64);
        } else {
            p->prev_row = NULL;
        }
    } else {
        p->row_buf = NULL;
        p->prev_row = NULL;
    }
    return p;
}

static void free_ptr(png_struct* p)
{
    if (!p) return;
    if (p->row_buf) free(p->row_buf);
    if (p->prev_row) free(p->prev_row);
    if (p->try_row) free(p->try_row);
    if (p->tst_row) free(p->tst_row);
    delete p;
}

// Test 1: Null pointer should cause early return without crash
static bool test_png_set_filter_null_ptr()
{
    // Should not crash
    try {
        png_set_filter(nullptr, 0, 0);
    } catch (...) {
        return false;
    }
    return true;
}

// Test 2: Unknown custom filter method should raise an error
static bool test_png_set_filter_unknown_method_raises()
{
    png_struct* p = make_ptr(4, 4, 1, 8, false, false);
    bool caught = false;
    try {
        png_set_filter(p, 99, 0); // Unknown custom method
    } catch (const std::exception& ex) {
        caught = true;
        // Expect an error message to be emitted
        if (std::string(ex.what()).empty()) {
            free_ptr(p);
            return false;
        }
    }
    free_ptr(p);
    if (!caught) {
        std::cerr << "Test failure: Expected exception for unknown method, but none thrown.\n";
        return false;
    }
    return true;
}

// Test 3: BASE method with NONE filter and no row_buf should set do_filter accordingly
static bool test_png_set_filter_base_none_no_allocation()
{
    png_struct* p = make_ptr(10, 10, 3, 8, false, false);
    // method BASE, filters NONE
    png_set_filter(p, PNG_FILTER_TYPE_BASE, PNG_FILTER_VALUE_NONE);
    // expect do_filter == NONE and no allocations
    ASSERT_TRUE(p->do_filter == PNG_FILTER_NONE, "do_filter should be NONE for NONE filter");
    ASSERT_TRUE(p->try_row == NULL && p->tst_row == NULL, "No buffers should be allocated when row_buf is NULL");
    free_ptr(p);
    return true;
}

// Test 4: Single filter SUB with row_buf allocated should allocate try_row but not tst_row
static bool test_png_set_filter_base_sub_allocates_try_row_only()
{
    // width=2, height=2, 2 channels, 8-bit
    png_struct* p = make_ptr(2, 2, 1, 8, true, true);
    // ensure prev_row is NULL to avoid warnings; we want allocation path
    // method BASE, filter SUB
    png_set_filter(p, PNG_FILTER_TYPE_BASE, PNG_FILTER_VALUE_SUB);
    // Assertions
    ASSERT_TRUE(p->do_filter == PNG_FILTER_SUB, "do_filter should be set to SUB");
    ASSERT_TRUE(p->try_row != NULL, "try_row should be allocated when a row_buf exists and one filter is used");
    ASSERT_TRUE(p->tst_row == NULL, "tst_row should not be allocated for a single filter");
    free_ptr(p);
    return true;
}

// Test 5: Multiple filters SUB+UP should allocate both try_row and tst_row
static bool test_png_set_filter_base_multiple_allocates_try_and_tst()
{
    png_struct* p = make_ptr(4, 4, 1, 8, true, true);
    // Use two filters: SUB and UP (bits combined)
    int filters = PNG_FILTER_VALUE_SUB | PNG_FILTER_VALUE_UP;
    png_set_filter(p, PNG_FILTER_TYPE_BASE, filters);
    // After call, do_filter should reflect the bitmask (possibly 3)
    ASSERT_TRUE((p->do_filter & PNG_FILTER_VALUE_SUB) != 0 || p->do_filter == (unsigned char)filters,
                "do_filter should reflect SUB bit (or equal combined mask)");
    ASSERT_TRUE(p->try_row != NULL, "try_row should be allocated when row_buf exists");
    ASSERT_TRUE(p->tst_row != NULL, "tst_row should be allocated when multiple filters are active");
    free_ptr(p);
    return true;
}

// Test 6: UP/AVG/PAETH requested after start (prev_row NULL) should warn and clear those filters
static bool test_png_set_filter_warns_and_clears_after_start()
{
    png_struct* p = make_ptr(3, 3, 1, 8, true, false); // prev_row=NULL to simulate "started" state
    reset_warning();
    // Request UP only
    int filters = PNG_FILTER_VALUE_UP;
    png_set_filter(p, PNG_FILTER_TYPE_BASE, filters);
    // A warning should have been emitted and UP bit cleared
    ASSERT_TRUE(!last_warning.empty(), "Expected a warning when adding UP after start with no prev_row");
    ASSERT_TRUE((p->do_filter & PNG_FILTER_VALUE_UP) == 0, "UP bit should be cleared after warning");
    // Since only one filter and it's cleared, no buffers should be allocated
    ASSERT_TRUE(p->try_row == NULL, "No try_row should be allocated when no filters remain");
    ASSERT_TRUE(p->tst_row == NULL, "No tst_row should be allocated when no filters remain");
    free_ptr(p);
    reset_warning();
    return true;
}

// Test 7: Unknown base-method filter values 5/6/7 should raise an error
static bool test_png_set_filter_base_unknown_filter_values_5_6_7()
{
    png_struct* p = make_ptr(6, 6, 1, 8, true, true);
    bool caught = false;
    try {
        png_set_filter(p, PNG_FILTER_TYPE_BASE, 5); // triggers "Unknown row filter for method 0"
    } catch (const std::exception&) {
        caught = true;
    }
    free_ptr(p);
    if (!caught) {
        std::cerr << "Test failure: Expected error for base method with filter 5.\n";
        return false;
    }
    return true;
}

// Test 8: Unknown custom filter method when method != BASE should raise
static bool test_png_set_filter_unknown_custom_method_raises_again()
{
    png_struct* p = make_ptr(8, 8, 1, 8, true, true);
    bool caught = false;
    try {
        png_set_filter(p, 1234, 0);
    } catch (const std::exception&) {
        caught = true;
    }
    free_ptr(p);
    if (!caught) {
        std::cerr << "Test failure: Expected exception for unknown custom method (second case).\n";
        return false;
    }
    return true;
}

// ------------------------
// Test runner
// ------------------------

static bool run_all_tests()
{
    bool ok = true;
    ok = ok && test_png_set_filter_null_ptr();
    ok = ok && test_png_set_filter_unknown_method_raises();
    ok = ok && test_png_set_filter_base_none_no_allocation();
    ok = ok && test_png_set_filter_base_sub_allocates_try_row_only();
    ok = ok && test_png_set_filter_base_multiple_allocates_try_and_tst();
    ok = ok && test_png_set_filter_warns_and_clears_after_start();
    ok = ok && test_png_set_filter_base_unknown_filter_values_5_6_7();
    ok = ok && test_png_set_filter_unknown_custom_method_raises_again();
    return ok;
}

// ------------------------
// Main
// ------------------------

int main()
{
    bool all_passed = run_all_tests();
    if (all_passed) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests failed. See output above for details.\n";
        return 1;
    }
}