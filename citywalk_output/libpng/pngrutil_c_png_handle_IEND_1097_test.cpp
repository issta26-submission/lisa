// Minimal C++11 unit test harness for the focal method png_handle_IEND
// Approach:
// - Provide a self-contained, minimal reproduction of the necessary types and
//   dependencies used by png_handle_IEND (as per the focal method snippet).
// - Stub dependent functions (png_debug, png_chunk_benign_error, png_crc_finish_critical)
//   to observe side effects without requiring a full libpng build.
// - Implement tests that exercise both branches of the length check (length == 0 and length != 0).
// - Use a lightweight, non-terminating assertion mechanism (EXPECT_*) and report results
//   from main() as a simple test harness (no external testing framework).

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>


// Domain knowledge helpers (minimal, local definitions)
typedef uint32_t png_uint_32;
typedef void*    png_inforp;
typedef void*    png_structrp;

// Expose the same layout as the focal method expects for png_ptr
struct png_struct_type {
    png_uint_32 mode;
};

// Public constants used by the focal method
const int handled_ok = 1;
#define PNG_AFTER_IDAT 0x01
#define PNG_HAVE_IEND   0x02

// Macro to silence unused parameters (as in the original code)
#define PNG_UNUSED(x) (void)(x)

// Minimal stubs to observe behavior of the focal method without a real libpng
static int g_benign_error_called = 0;
static int g_crc_finish_called = 0;
static uint32_t g_crc_finish_skip = 0;

// Lightweight, test-only replacements for internal libpng helpers
void png_debug(int level, const char* msg) {
    // No-op for tests; could be extended to log debug traces if needed
    (void)level; (void)msg;
}

void png_chunk_benign_error(png_structrp png_ptr, const char* msg) {
    (void)png_ptr; // in a full test we could inspect png_ptr for further assertions
    (void)msg;
    g_benign_error_called = 1;
    // Do not throw; the real function would log/handle, but we just observe
}

void png_crc_finish_critical(png_structrp png_ptr, png_uint_32 skip, int handle_as_ancillary) {
    (void)png_ptr;
    (void)handle_as_ancillary;
    g_crc_finish_called = 1;
    g_crc_finish_skip = skip;
}

// The focal method under test (exact behavior replicated per the provided snippet)
int png_handle_IEND(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
{
   png_debug(1, "in png_handle_IEND");
   png_ptr->mode |= (PNG_AFTER_IDAT | PNG_HAVE_IEND);
   if (length != 0)
      png_chunk_benign_error(png_ptr, "invalid");
   png_crc_finish_critical(png_ptr, length, 1);
   return handled_ok;
   PNG_UNUSED(info_ptr)
}
}

// Lightweight test framework (non-terminating assertions)
static int g_test_failures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { std::cerr << "EXPECT_TRUE failed: " << (msg) \
        << " | " << __FILE__ << ":" << __LINE__ << std::endl; ++g_test_failures; } } while(0)

#define EXPECT_EQ(a, b, msg) \
    do { if((a) != (b)) { std::cerr << "EXPECT_EQ failed: " << (msg) \
        << " (expected " << (a) << " vs actual " << (b) << ") " \
        << " | " << __FILE__ << ":" << __LINE__ << std::endl; ++g_test_failures; } } while(0)

// Helper to create and initialize a fake png_struct
png_structrp make_fake_png_ptr() {
    png_struct_type* ptr = new png_struct_type();
    ptr->mode = 0;
    return reinterpret_cast<png_structrp>(ptr);
}

// Helper to clean up fake png_struct
void destroy_fake_png_ptr(png_structrp p) {
    delete reinterpret_cast<png_struct_type*>(p);
}

// Test 1: length == 0 should set mode flags and return handled_ok without invoking benign error
void test_handle_IEND_length_zero() {
    // Setup
    g_benign_error_called = 0;
    g_crc_finish_called = 0;
    g_crc_finish_skip = 0;

    png_structrp fake_ptr = make_fake_png_ptr();
    png_inforp info_ptr = nullptr;
    png_uint_32 length = 0;

    // Exercise
    int ret = png_handle_IEND(fake_ptr, info_ptr, length);

    // Verify
    uint32_t expected_mode = (PNG_AFTER_IDAT | PNG_HAVE_IEND);
    uint32_t actual_mode = reinterpret_cast<png_struct_type*>(fake_ptr)->mode;
    EXPECT_TRUE((actual_mode & expected_mode) == expected_mode,
        "png_handle_IEND should set both PNG_AFTER_IDAT and PNG_HAVE_IEND bits when length == 0");
    EXPECT_TRUE(ret != 0, "png_handle_IEND should return a non-zero handle status (handled_ok) when length == 0");
    // Sanity: benign_error should not have been called for length == 0
    EXPECT_TRUE(g_benign_error_called == 0, "png_chunk_benign_error should not be called when length == 0");
    // CRC finish should be invoked with length == 0
    EXPECT_TRUE(g_crc_finish_called == 1, "png_crc_finish_critical should be called for length == 0");
    EXPECT_EQ(g_crc_finish_skip, length, "png_crc_finish_critical should be called with skip == length (0)");
    
    // Cleanup
    destroy_fake_png_ptr(fake_ptr);
}

// Test 2: length != 0 should still update mode and call benign error
void test_handle_IEND_length_nonzero() {
    // Setup
    g_benign_error_called = 0;
    g_crc_finish_called = 0;
    g_crc_finish_skip = 0;

    png_structrp fake_ptr = make_fake_png_ptr();
    png_inforp info_ptr = nullptr;
    png_uint_32 length = 5;

    // Exercise
    int ret = png_handle_IEND(fake_ptr, info_ptr, length);

    // Verify
    uint32_t expected_mode = (PNG_AFTER_IDAT | PNG_HAVE_IEND);
    uint32_t actual_mode = reinterpret_cast<png_struct_type*>(fake_ptr)->mode;
    EXPECT_TRUE((actual_mode & expected_mode) == expected_mode,
        "png_handle_IEND should set both PNG_AFTER_IDAT and PNG_HAVE_IEND bits when length != 0");
    EXPECT_TRUE(ret != 0, "png_handle_IEND should return a non-zero handle status (handled_ok) when length != 0");
    // Benign error path should be taken for non-zero length
    EXPECT_TRUE(g_benign_error_called == 1, "png_chunk_benign_error should be called when length != 0");
    // CRC finish should still be invoked with the provided length
    EXPECT_TRUE(g_crc_finish_called == 1, "png_crc_finish_critical should be called for non-zero length");
    EXPECT_EQ(g_crc_finish_skip, length, "png_crc_finish_critical should be called with skip == length (non-zero)");
    
    // Cleanup
    destroy_fake_png_ptr(fake_ptr);
}

int main() {
    // Run tests
    test_handle_IEND_length_zero();
    test_handle_IEND_length_nonzero();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "All tests passed for png_handle_IEND." << std::endl;
        return EXIT_SUCCESS;
    } else {
        std::cerr << g_test_failures << " test(s) failed for png_handle_IEND." << std::endl;
        return EXIT_FAILURE;
    }
}