// Unit test suite for png_do_write_swap_alpha (pngwtran.c) in a C++11 test harness.
// This test suite assumes a libpng-like environment is available in the build
// (headers and symbols). The focal function is png_do_write_swap_alpha which
// rewrites a row buffer in-place depending on color type and bit depth.
// The tests cover multiple branches of the conditional predicates as described
// in the Step 1-3 guidance.

// Note: These tests use non-terminating assertions to maximize code coverage.
// Failures are reported but do not abort the test run.

#include <cstdint>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>


// Try to use the public libpng types when available.
// If libpng headers are not found, this test will fail to compile in that environment.
// The test treats png_row_infop and png_bytep as provided by libpng, along with
// the color type constants and the function prototype for png_do_write_swap_alpha.


// Ensure the focal function has C linkage when called from C++
extern "C" {
    // Prototype of the focal method under test (as expected by libpng's naming)
    void png_do_write_swap_alpha(png_row_infop row_info, png_bytep row);
}

// Simple non-terminating assertion helpers
static int g_test_failures = 0;

#define KVOP(x) #x
#define LOG(msg, ...) do { fprintf(stderr, msg "\n", ##__VA_ARGS__); } while (0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        LOG("EXPECT_TRUE failed: %s at %s:%d", #cond, __FILE__, __LINE__); \
        ++g_test_failures; \
    } \
} while (0)

#define EXPECT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        LOG("EXPECT_EQ failed: expected %ld, got %ld at %s:%d", (long)(expected), (long)(actual), __FILE__, __LINE__); \
        ++g_test_failures; \
    } \
} while (0)

#define EXPECT_MEMEQ(expected_ptr, actual_ptr, len) do { \
    if (std::memcmp((expected_ptr), (actual_ptr), (len)) != 0) { \
        LOG("EXPECT_MEMEQ failed: memory regions differ at %s:%d", __FILE__, __LINE__); \
        ++g_test_failures; \
    } \
} while (0)

// Helper: print a summary
static void print_summary() {
    if (g_test_failures == 0) {
        std::cout << "All tests passed for png_do_write_swap_alpha." << std::endl;
    } else {
        std::cerr << g_test_failures << " test failure(s) detected for png_do_write_swap_alpha." << std::endl;
    }
}

// Test 1: RGB_ALPHA, bit_depth = 8
// This tests the ARGB -> RGBA conversion per pixel for 8-bit components.
// Input per pixel: [A, R, G, B]  -> Output per pixel: [R, G, B, A]
static void test_argb8_swap_alpha() {
    // Prepare row_info: width=3, color_type=PNG_COLOR_TYPE_RGB_ALPHA, bit_depth=8
    png_row_infop row_info = new png_row_info; // libpng typedef: pointer to struct
    if (!row_info) { LOG("Memory allocation failed for row_info"); return; }
    row_info->width = 3;
    row_info->bit_depth = 8;
    row_info->color_type = PNG_COLOR_TYPE_RGB_ALPHA;

    // Prepare input row: 3 pixels, ARGB order per pixel
    // Pixel 1: A=0x11, R=0x22, G=0x33, B=0x44
    // Pixel 2: A=0x55, R=0x66, G=0x77, B=0x88
    // Pixel 3: A=0x99, R=0xAA, G=0xBB, B=0xCC
    std::vector<uint8_t> row = {
        0x11,0x22,0x33,0x44,
        0x55,0x66,0x77,0x88,
        0x99,0xAA,0xBB,0xCC
    };

    // Expected after swap: [R,G,B,A] per pixel
    std::vector<uint8_t> expected = {
        0x22,0x33,0x44,0x11,
        0x66,0x77,0x88,0x55,
        0xAA,0xBB,0xCC,0x99
    };

    // Call the focal function
    png_do_write_swap_alpha(row_info, row.data());

    // Verify
    EXPECT_MEMEQ(expected.data(), row.data(), expected.size());
    // Cleanup
    delete row_info;
}

// Test 2: RGB_ALPHA, bit_depth = 16 (16-bit path)
// This tests the AARRGGBB -> RRGGBBAA conversion per pixel for 16-bit components.
// Input per pixel: [A_lo, A_hi, R_lo, R_hi, G_lo, G_hi, B_lo, B_hi]
static void test_argb16_swap_alpha() {
#if defined(PNG_WRITE_16BIT_SUPPORTED)
    // Prepare row_info: width=1, color_type=PNG_COLOR_TYPE_RGB_ALPHA, bit_depth=16
    png_row_infop row_info = new png_row_info;
    if (!row_info) { LOG("Memory allocation failed for row_info"); return; }
    row_info->width = 1;
    row_info->bit_depth = 16;
    row_info->color_type = PNG_COLOR_TYPE_RGB_ALPHA;

    // Construct input: A=0x0102, R=0x0304, G=0x0506, B=0x0708
    std::vector<uint8_t> row = {
        0x02,0x01, 0x04,0x03, 0x06,0x05, 0x08,0x07
    };

    // Expected output: [R, G, B, A]
    std::vector<uint8_t> expected = {
        0x04,0x03, 0x06,0x05, 0x08,0x07, 0x02,0x01
    };

    png_do_write_swap_alpha(row_info, row.data());

    EXPECT_MEMEQ(expected.data(), row.data(), expected.size());

    delete row_info;
#else
    // If 16-bit support is not compiled in, skip this test gracefully.
    std::cout << "Skipping test_argb16_swap_alpha because PNG_WRITE_16BIT_SUPPORTED is not defined." << std::endl;
#endif
}

// Test 3: GRAY_ALPHA, bit_depth = 8
// This tests AG -> GA conversion for 8-bit components.
// Input per pixel: [A, G]  -> Output per pixel: [G, A]
static void test_grayalpha8_swap_alpha() {
    png_row_infop row_info = new png_row_info;
    if (!row_info) { LOG("Memory allocation failed for row_info"); return; }
    row_info->width = 3;
    row_info->bit_depth = 8;
    row_info->color_type = PNG_COLOR_TYPE_GRAY_ALPHA;

    // Pixels: (A,G) = (0xAA,0x11), (0xBB,0x22), (0xCC,0x33)
    std::vector<uint8_t> row = {
        0xAA,0x11,
        0xBB,0x22,
        0xCC,0x33
    };

    // Expected after transform: [G,A] per pixel
    std::vector<uint8_t> expected = {
        0x11,0xAA,
        0x22,0xBB,
        0x33,0xCC
    };

    png_do_write_swap_alpha(row_info, row.data());

    EXPECT_MEMEQ(expected.data(), row.data(), expected.size());

    delete row_info;
}

// Test 4: GRAY_ALPHA, bit_depth = 16
// This tests AAGG -> GGAA conversion for 16-bit components.
// Input per pixel: [A_lo,A_hi, G_lo,G_hi]
static void test_grayalpha16_swap_alpha() {
#if defined(PNG_WRITE_16BIT_SUPPORTED)
    png_row_infop row_info = new png_row_info;
    if (!row_info) { LOG("Memory allocation failed for row_info"); return; }
    row_info->width = 2;
    row_info->bit_depth = 16;
    row_info->color_type = PNG_COLOR_TYPE_GRAY_ALPHA;

    // Pixels: (A,G) = P1: A=0x0102, G=0x0304; P2: A=0x0506, G=0x0708
    std::vector<uint8_t> row = {
        0x02,0x01, 0x04,0x03,
        0x06,0x05, 0x08,0x07
    };

    // Expected after trans: per pixel [G_lo,G_hi, A_lo,A_hi]
    std::vector<uint8_t> expected = {
        0x04,0x03, 0x02,0x01,
        0x08,0x07, 0x06,0x05
    };

    png_do_write_swap_alpha(row_info, row.data());

    EXPECT_MEMEQ(expected.data(), row.data(), expected.size());

    delete row_info;
#else
    std::cout << "Skipping test_grayalpha16_swap_alpha because PNG_WRITE_16BIT_SUPPORTED is not defined." << std::endl;
#endif
}

// Test 5: Non-target color type should leave data unchanged
// We'll use a hypothetical RGB color type (not RGBA or GRAY_ALPHA) to verify no modifications.
static void test_no_change_for_other_color_types() {
    // Use actual libpng color type for RGB (if available). We'll assume PNG_COLOR_TYPE_RGB is defined.
    png_row_infop row_info = new png_row_info;
    if (!row_info) { LOG("Memory allocation failed for row_info"); return; }
    row_info->width = 2;
    row_info->bit_depth = 8;
    row_info->color_type = PNG_COLOR_TYPE_RGB; // Not RGBA nor GRAY_ALPHA

    // Prepare row with recognizable data
    std::vector<uint8_t> row = {
        0x10,0x20, 0x30, // Pixel 1: RGB
        0x40,0x50, 0x60  // Pixel 2: RGB
    };

    std::vector<uint8_t> expected = row;

    png_do_write_swap_alpha(row_info, row.data());

    EXPECT_MEMEQ(expected.data(), row.data(), expected.size());

    delete row_info;
}

// Main driver: run all tests and report results
int main() {
    // Announce tests
    std::cout << "Starting tests for png_do_write_swap_alpha..." << std::endl;

    test_argb8_swap_alpha();
    test_argb16_swap_alpha();
    test_grayalpha8_swap_alpha();
    test_grayalpha16_swap_alpha();
    test_no_change_for_other_color_types();

    print_summary();
    return g_test_failures;
}