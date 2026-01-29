// C++11 unit test suite for the focal method: png_write_filtered_row
// This test suite is designed to be compiled in two modes:
// 1) TEST_WITH_FILTER: tests the PNG_WRITE_FILTER_SUPPORTED path (swap of prev_row and row_buf).
// 2) TEST_WITHOUT_FILTER: tests the non-filter path (swap block not compiled) and basic flush behavior.
// Build guidance (separate compilations):
// - To test with the filter and flush support: g++ -DTEST_WITH_FILTER=1 -DTEST_WITHOUT_FILTER=0 -std=c++11 -Wall -Wextra -O2 -o test_with_filter this_file.cpp
// - To test without the filter swap code (no PNG_WRITE_FILTER_SUPPORTED): g++ -DTEST_WITH_FILTER=0 -DTEST_WITHOUT_FILTER=1 -std=c++11 -Wall -Wextra -O2 -o test_without_filter this_file.cpp
// Note: The tests provided here implement a minimal mock of the libpng related environment to exercise the focal function
// without requiring the real libpng library. The focal function is re-implemented here with the same signature and
// equivalent behavior gated by the same macros, so tests reflect the intended control flow.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <cstddef>


// -----------------------------------------------------------------------------
// Configuration macros (activate per build as shown in comments above)
// If not defined by the build system, default to 0 (disabled).
#ifndef TEST_WITH_FILTER
#define TEST_WITH_FILTER 0
#endif
#ifndef TEST_WITHOUT_FILTER
#define TEST_WITHOUT_FILTER 0
#endif

// The actual focal function uses these macros to enable/disable code paths.
// We derive their values from the chosen test variant.
#if defined(TEST_WITH_FILTER) && (TEST_WITH_FILTER == 1)
#define PNG_WRITE_FILTER_SUPPORTED 1
#define PNG_WRITE_FLUSH_SUPPORTED 1
#else
// In the non-filter variant, we keep filter path disabled.
// We still enable flush to allow testing of the flushing behavior if desired.
#define PNG_WRITE_FILTER_SUPPORTED 0
#if defined(TEST_WITHOUT_FILTER) && (TEST_WITHOUT_FILTER == 1)
#define PNG_WRITE_FLUSH_SUPPORTED 0
#else
#define PNG_WRITE_FLUSH_SUPPORTED 0
#endif
#endif

// -----------------------------------------------------------------------------
// Minimal type definitions to mimic the used libpng types in the focal method.
// This is a lightweight mock sufficient for unit testing the control flow and side effects.

typedef unsigned char png_bytep;
typedef const unsigned char* png_const_bytep;
typedef unsigned int png_uint_32;
typedef size_t png_alloc_size_t;

// Forward declaration of the internal png struct (mock)
struct png_struct_def;
typedef struct png_struct_def png_struct_def;
typedef png_struct_def* png_structrp;

// Pointer types as used by the focal method
typedef png_structrp png_structrp;

// NULL for safety
#ifndef NULL
#define NULL nullptr
#endif

// A simple allocation-free "buffer" to hold test data if needed
static unsigned char test_buf[256];

// Internal mock state (used by the mock functions)
static bool mock_compress_idat_called = false;
static size_t mock_compress_input_len = 0;
static png_bytep mock_compress_input_ptr = NULL;

static bool mock_finish_row_called = false;
static bool mock_flush_called = false;
static unsigned int mock_debug_calls = 0;
static unsigned int mock_debug1_calls = 0;

// The mock libpng-like structure used by the focal method
struct png_struct_def {
    png_bytep prev_row;
    png_bytep row_buf;
    unsigned int flush_rows;
    unsigned int flush_dist;
};

// -----------------------------------------------------------------------------
// Mocked external functions that the focal method calls.
// Implementations are minimal and tailored for verification in tests.

extern "C" {

void png_debug(int /*level*/, const char* /*msg*/) {
    // lightweight no-op for tests; could record messages if needed
    (void)0;
}

void png_debug1(int /*level*/, const char* /*format*/, int /*param*/) {
    // lightweight no-op for tests; increment a counter to indicate a call.
    mock_debug1_calls++;
}

void png_compress_IDAT(png_structrp png_ptr, png_const_bytep input,
                       png_alloc_size_t input_len, int /*flush*/) {
    // Record that compression was invoked and the input used
    (void)png_ptr;
    mock_compress_idat_called = true;
    mock_compress_input_len = static_cast<size_t>(input_len);
    mock_compress_input_ptr = const_cast<png_bytep>(const_cast<png_const_bytep>(input));
    // Note: We do not perform actual compression in tests.
    // The test asserts that the function was invoked with the expected data.
}

void png_write_finish_row(png_structrp png_ptr) {
    // Indicate finish row was invoked
    (void)png_ptr;
    mock_finish_row_called = true;
}

void png_write_flush(png_structrp /*png_ptr*/) {
    // Indicate a flush was performed
    mock_flush_called = true;
}

// Close extern "C"
}

// -----------------------------------------------------------------------------
// Re-implementation of the focal method under test, matching the provided code,
// with behavior controlled by the macros PNG_WRITE_FILTER_SUPPORTED and
// PNG_WRITE_FLUSH_SUPPORTED (provided above in this file).
extern "C" {

void png_write_filtered_row(png_structrp png_ptr, png_bytep filtered_row,
                            size_t full_row_length)
{
{
   png_debug(1, "in png_write_filtered_row");
   png_debug1(2, "filter = %d", filtered_row[0]);
   png_compress_IDAT(png_ptr, filtered_row, full_row_length, /*Z_NO_FLUSH*/ 0);
#if PNG_WRITE_FILTER_SUPPORTED
   /* Swap the current and previous rows */
   if (png_ptr->prev_row != NULL)
   {
      png_bytep tptr;
      tptr = png_ptr->prev_row;
      png_ptr->prev_row = png_ptr->row_buf;
      png_ptr->row_buf = tptr;
   }
#endif /* WRITE_FILTER */
   /* Finish row - updates counters and flushes zlib if last row */
   png_write_finish_row(png_ptr);
#if PNG_WRITE_FLUSH_SUPPORTED
   png_ptr->flush_rows++;
   if (png_ptr->flush_dist > 0 &&
       png_ptr->flush_rows >= png_ptr->flush_dist)
   {
      png_write_flush(png_ptr);
   }
#endif /* WRITE_FLUSH */
}
}
} // extern "C"

// -----------------------------------------------------------------------------
// Lightweight test harness and helpers

static int test_failures = 0;

static void test_log(const char* msg) {
    std::cerr << "[TEST] " << msg << std::endl;
}

static void expect_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "EXPECT_TRUE FAILED: " << msg
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl;
        ++test_failures;
    }
}

static void expect_eq_ptr(void* a, void* b, const char* a_expr = "", const char* b_expr = "") {
    if (a != b) {
        std::cerr << "EXPECT_EQ_PTR FAILED: " << a_expr << " != " << b_expr
                  << " [actual: " << a << ", expected: " << b << "] "
                  << "(" << __FILE__ << ":" << __LINE__ << ")" << std::endl;
        ++test_failures;
    }
}

// Convenience macro wrappers for readability
#define EXPECT_TRUE_COND(cond, msg) expect_true((cond), (msg))
#define EXPECT_EQ_PTR_EXPR(a,b) expect_eq_ptr((void*)(a), (void*)(b), #a, #b)

// -----------------------------------------------------------------------------
// Test cases (executed only for the corresponding build variant)

// Variant 1: TEST_WITH_FILTER == 1 (swap path and flush path both enabled)
#if (TEST_WITH_FILTER == 1)

static void run_test_swap_buffers_when_prev_non_null() {
    test_log("Test: swap prev_row and row_buf when prev_row != NULL (PNG_WRITE_FILTER_SUPPORTED)");
    png_struct_def s;
    unsigned char a_buf[16];
    unsigned char b_buf[16];
    // Initialize buffers with distinct values to detect swaps
    std::memset(a_buf, 0xAA, sizeof(a_buf)); // initial prev_row data
    std::memset(b_buf, 0x55, sizeof(b_buf)); // initial row_buf data

    s.prev_row = a_buf;
    s.row_buf = b_buf;
    s.flush_rows = 0;
    s.flush_dist = 1; // ensure flush occurs after first row

    unsigned char filtered[4] = {0, 1, 2, 3};
    png_structrp ptr = &s;

    // Call focal method
    png_write_filtered_row(ptr, filtered, 4);

    // Expect swap happened: prev_row should now point to original row_buf (b_buf)
    EXPECT_EQ_PTR_EXPR(s.prev_row, b_buf);

    // After swap, row_buf should point to original prev_row (a_buf)
    EXPECT_EQ_PTR_EXPR(s.row_buf, a_buf);

    // Expect that compress and finish were invoked
    EXPECT_TRUE_COND(mock_compress_idat_called, "png_compress_IDAT should be called");
    EXPECT_TRUE_COND(mock_finish_row_called, "png_write_finish_row should be called");

    // After first call with flush_dist=1, a flush should occur
    EXPECT_TRUE_COND(mock_flush_called, "png_write_flush should be called due to flush_dist");
}

static void run_test_no_swap_when_prev_null() {
    test_log("Test: no swap when prev_row == NULL (PNG_WRITE_FILTER_SUPPORTED)");
    png_struct_def s;
    unsigned char a_buf[16];
    unsigned char b_buf[16];

    std::memset(a_buf, 0x11, sizeof(a_buf)); // prev_row data (will become NULL)
    std::memset(b_buf, 0x22, sizeof(b_buf)); // row_buf data

    s.prev_row = NULL;
    s.row_buf = b_buf;
    s.flush_rows = 0;
    s.flush_dist = 2; // ensure flush would require two rows

    unsigned char filtered[4] = {0, 9, 9, 9};
    png_structrp ptr = &s;

    // Call focal method
    png_write_filtered_row(ptr, filtered, 4);

    // Expect no swap occurred: prev_row remains NULL
    EXPECT_TRUE_COND(s.prev_row == NULL, "prev_row should remain NULL when no swap occurs");

    // row_buf should remain unchanged (still the same pointer)
    EXPECT_EQ_PTR_EXPR(s.row_buf, b_buf);

    // compress and finish should still be invoked
    EXPECT_TRUE_COND(mock_compress_idat_called, "png_compress_IDAT should be called");
    EXPECT_TRUE_COND(mock_finish_row_called, "png_write_finish_row should be called");

    // Since flush_dist == 2 and flush_rows becomes 1, flush should not yet happen
    EXPECT_TRUE_COND(!mock_flush_called, "png_write_flush should NOT be called yet (threshold not reached)");
}

static void run_all_tests_variant_with_filter() {
    // Reset mock state
    mock_compress_idat_called = false;
    mock_compress_input_len = 0;
    mock_compress_input_ptr = NULL;
    mock_finish_row_called = false;
    mock_flush_called = false;
    mock_debug_calls = 0;
    mock_debug1_calls = 0;

    run_test_swap_buffers_when_prev_non_null();

    // Reset for second test
    mock_compress_idat_called = false;
    mock_compress_input_len = 0;
    mock_compress_input_ptr = NULL;
    mock_finish_row_called = false;
    mock_flush_called = false;

    run_test_no_swap_when_prev_null();
}

#endif // (TEST_WITH_FILTER == 1)

// Variant 2: TEST_WITHOUT_FILTER == 1 (swap path disabled, only flush path remains)
#if (TEST_WITHOUT_FILTER == 1)

static void run_test_no_swap_and_flush_behaviour_without_filter() {
    test_log("Test: behavior without PNG_WRITE_FILTER_SUPPORTED (swap path not compiled)");
    png_struct_def s;
    unsigned char a_buf[16];
    unsigned char b_buf[16];

    std::memset(a_buf, 0x33, sizeof(a_buf));
    std::memset(b_buf, 0x44, sizeof(b_buf));

    s.prev_row = a_buf; // this pointer would be ignored by code path without swap
    s.row_buf = b_buf;
    s.flush_rows = 0;
    s.flush_dist = 1; // trigger flush on first row if flush path is enabled

    unsigned char filtered[4] = {0, 7, 7, 7};
    png_structrp ptr = &s;

    // Call focal method
    png_write_filtered_row(ptr, filtered, 4);

    // Since swap path is disabled, prev_row should remain unchanged, i.e., still a_buf
    EXPECT_EQ_PTR_EXPR(s.prev_row, a_buf);

    // row_buf should remain unchanged as well
    EXPECT_EQ_PTR_EXPR(s.row_buf, b_buf);

    // compress and finish should still be invoked
    EXPECT_TRUE_COND(mock_compress_idat_called, "png_compress_IDAT should be called (even without swap)");
    EXPECT_TRUE_COND(mock_finish_row_called, "png_write_finish_row should be called (even without swap)");

    // If flush is enabled in this variant, it should flush after first row
    // In this configuration, PNG_WRITE_FLUSH_SUPPORTED is 0 by default for this test harness;
    // thus, we do not assert on flush here unless the macro is set to enable it.
}

static void run_all_tests_variant_without_filter() {
    // Reset mock state
    mock_compress_idat_called = false;
    mock_compress_input_len = 0;
    mock_compress_input_ptr = NULL;
    mock_finish_row_called = false;
    mock_flush_called = false;
    mock_debug_calls = 0;
    mock_debug1_calls = 0;

    run_test_no_swap_and_flush_behaviour_without_filter();
}

#endif // (TEST_WITHOUT_FILTER == 1)

// -----------------------------------------------------------------------------
// Main entrypoint

int main() {
    int overall = 0;

#if (TEST_WITH_FILTER == 1)
    test_log("Running tests with PNG_WRITE_FILTER_SUPPORTED and PNG_WRITE_FLUSH_SUPPORTED defined.");
    run_all_tests_variant_with_filter();
    overall = test_failures;
#elif (TEST_WITHOUT_FILTER == 1)
    test_log("Running tests WITHOUT PNG_WRITE_FILTER_SUPPORTED (swap path disabled).");
    run_all_tests_variant_without_filter();
    overall = test_failures;
#else
    test_log("No test variant selected. Re-run with -DTEST_WITH_FILTER=1 or -DTEST_WITHOUT_FILTER=1.");
    overall = 1;
#endif

    if (overall == 0) {
        std::cout << "[TEST] All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[TEST] Some tests FAILED. Failures: " << overall << std::endl;
        return 1;
    }
}

// End of test suite code.