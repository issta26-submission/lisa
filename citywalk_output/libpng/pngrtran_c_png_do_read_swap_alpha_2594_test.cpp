// Lightweight C++11 unit tests for the focal method
// png_do_read_swap_alpha
// This test reproduces the relevant function locally (self-contained) and validates
// its behavior for several scenarios without using a testing framework like GTest.
// The tests are executed from main() and use non-terminating assertions to maximize coverage.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain-adapted minimal reimplementation of required types and constants
typedef unsigned char png_bytep;
typedef unsigned char png_byte;
typedef unsigned int  png_uint_32;

#define PNG_COLOR_TYPE_RGB_ALPHA 6
#define PNG_COLOR_TYPE_GRAY_ALPHA 4

#define PNG_READ_16BIT_SUPPORTED 1

static void png_debug(int level, const char* message)
{
    // Stub: no-op for tests
    (void)level;
    (void)message;
}

// Local reimplementation of png_do_read_swap_alpha, matching the focal method.
// This is self-contained for unit testing without requiring the real libpng build.
void png_do_read_swap_alpha(png_row_infop row_info, png_bytep row); // forward

typedef struct png_row_info_struct {
    png_uint_32 width;
    png_uint_32 rowbytes;
    int color_type;
    int bit_depth;
} png_row_infop;

// Forward declare to satisfy the old signature used in the focal method
void png_do_read_swap_alpha(png_row_infop row_info, png_bytep row)
{
    {
        png_uint_32 row_width = row_info->width;
        png_debug(1, "in png_do_read_swap_alpha");
        if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
        {
            /* This converts from RGBA to ARGB */
            if (row_info->bit_depth == 8)
            {
                png_bytep sp = row + row_info->rowbytes;
                png_bytep dp = sp;
                png_byte save;
                png_uint_32 i;
                for (i = 0; i < row_width; i++)
                {
                    save = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = save;
                }
            }
#ifdef PNG_READ_16BIT_SUPPORTED
            /* This converts from RRGGBBAA to AARRGGBB */
            else
            {
                png_bytep sp = row + row_info->rowbytes;
                png_bytep dp = sp;
                png_byte save[2];
                png_uint_32 i;
                for (i = 0; i < row_width; i++)
                {
                    save[0] = *(--sp);
                    save[1] = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = save[0];
                    *(--dp) = save[1];
                }
            }
#endif
        }
        else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        {
            /* This converts from GA to AG */
            if (row_info->bit_depth == 8)
            {
                png_bytep sp = row + row_info->rowbytes;
                png_bytep dp = sp;
                png_byte save;
                png_uint_32 i;
                for (i = 0; i < row_width; i++)
                {
                    save = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = save;
                }
            }
#ifdef PNG_READ_16BIT_SUPPORTED
            /* This converts from GGAA to AAGG */
            else
            {
                png_bytep sp = row + row_info->rowbytes;
                png_bytep dp = sp;
                png_byte save[2];
                png_uint_32 i;
                for (i = 0; i < row_width; i++)
                {
                    save[0] = *(--sp);
                    save[1] = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = *(--sp);
                    *(--dp) = save[0];
                    *(--dp) = save[1];
                }
            }
#endif
        }
    }
}

// Simple non-terminating assertion helper
static bool expect_eq_uint(const char* test_name, const char* msg, uint32_t a, uint32_t b)
{
    if (a != b) {
        std::cerr << "TEST FAILED: " << test_name
                  << " - " << (msg ? msg : "")
                  << " expected " << b << ", got " << a << "\n";
        return false;
    }
    return true;
}

static bool expect_mem_eq(const char* test_name, const unsigned char* a, const unsigned char* b, size_t n)
{
    if (std::memcmp(a, b, n) != 0) {
        std::cerr << "TEST FAILED: " << test_name
                  << " - memory contents differ.\nExpected: ";
        for (size_t i = 0; i < n; ++i) {
            if (i) std::cerr << " ";
            std::cerr << std::hex << (int)b[i] << std::dec;
        }
        std::cerr << "\nGot:      ";
        for (size_t i = 0; i < n; ++i) {
            if (i) std::cerr << " ";
            std::cerr << std::hex << (int)a[i] << std::dec;
        }
        std::cerr << "\n";
        return false;
    }
    return true;
}

// Test 1: RGB_ALPHA with 8-bit depth swaps RGBA -> ARGB for multiple pixels
// This validates the true path for 8-bit RGB_ALPHA.
static bool test_rgb_alpha_8bit()
{
    unsigned char row[8] = {1,2,3,4, 5,6,7,8}; // Pixel0: R=1,G=2,B=3,A=4 ; Pixel1: R=5,G=6,B=7,A=8
    png_row_infop ri;
    ri.width = 2;
    ri.rowbytes = 8;
    ri.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    ri.bit_depth = 8;

    png_do_read_swap_alpha(&ri, row);

    unsigned char expected[8] = {4,1,2,3, 8,5,6,7}; // ARGB, ARGB
    return expect_mem_eq("test_rgb_alpha_8bit", row, expected, 8);
}

// Test 2: RGB_ALPHA with 16-bit depth swaps RRGGBBAA -> AARRGGBB for single pixel
// This validates the 16-bit path under the RGB_ALPHA branch.
static bool test_rgb_alpha_16bit()
{
    unsigned char row[8] = {2,1, 4,3, 6,5, 8,7}; // R=0x0102, G=0x0304, B=0x0506, A=0x0708 (little-endian)
    png_row_infop ri;
    ri.width = 1;
    ri.rowbytes = 8;
    ri.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    ri.bit_depth = 16;

    png_do_read_swap_alpha(&ri, row);

    unsigned char expected[8] = {0x08,0x07, 0x02,0x01, 0x04,0x03, 0x06,0x05}; // A, R, G, B (each 16-bit little-endian pair)
    return expect_mem_eq("test_rgb_alpha_16bit", row, expected, 8);
}

// Test 3: GRAY_ALPHA with 8-bit depth swaps GA -> AG for multiple pixels
// This validates the 8-bit GrayAlpha path.
static bool test_gray_alpha_8bit()
{
    // Row layout per pixel: [G, A], so 3 pixels: [G0,A0, G1,A1, G2,A2]
    unsigned char row[6] = {10,11, 20,21, 30,31}; // (G,A) pairs
    png_row_infop ri;
    ri.width = 3;
    ri.rowbytes = 6;
    ri.color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    ri.bit_depth = 8;

    png_do_read_swap_alpha(&ri, row);

    unsigned char expected[6] = {11,10, 21,20, 31,30}; // [A,G] per pixel
    return expect_mem_eq("test_gray_alpha_8bit", row, expected, 6);
}

// Test 4: GRAY_ALPHA with 16-bit depth swaps GGAA -> AAGG for single pixel
// This validates the 16-bit GrayAlpha path.
static bool test_gray_alpha_16bit()
{
    // Row layout per pixel: [G_L,G_H, A_L,A_H] -> memory: [G_L,G_H, A_L,A_H]
    unsigned char row[4] = {2,1, 4,3}; // G=0x0102 (L=2,H=1), A=0x0304 (L=4,H=3)
    png_row_infop ri;
    ri.width = 1;
    ri.rowbytes = 4;
    ri.color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    ri.bit_depth = 16;

    png_do_read_swap_alpha(&ri, row);

    unsigned char expected[4] = {4,3, 2,1}; // [A_L,A_H, G_L,G_H]
    return expect_mem_eq("test_gray_alpha_16bit", row, expected, 4);
}

// Test 5: Unknown color type should leave memory unchanged
static bool test_unknown_color_type_no_op()
{
    unsigned char row[4] = {9,8,7,6};
    png_row_infop ri;
    ri.width = 2;
    ri.rowbytes = 4;
    ri.color_type = 999; // unknown
    ri.bit_depth = 8;

    unsigned char before[4];
    std::memcpy(before, row, 4);

    png_do_read_swap_alpha(&ri, row);

    return expect_mem_eq("test_unknown_color_type_no_op", row, before, 4);
}

int main()
{
    int failures = 0;
    int total_tests = 5;

    if (!test_rgb_alpha_8bit()) {
        ++failures;
    } else {
        std::cout << "PASS: test_rgb_alpha_8bit\n";
    }

    if (!test_rgb_alpha_16bit()) {
        ++failures;
    } else {
        std::cout << "PASS: test_rgb_alpha_16bit\n";
    }

    if (!test_gray_alpha_8bit()) {
        ++failures;
    } else {
        std::cout << "PASS: test_gray_alpha_8bit\n";
    }

    if (!test_gray_alpha_16bit()) {
        ++failures;
    } else {
        std::cout << "PASS: test_gray_alpha_16bit\n";
    }

    if (!test_unknown_color_type_no_op()) {
        ++failures;
    } else {
        std::cout << "PASS: test_unknown_color_type_no_op\n";
    }

    if (failures == 0)
        std::cout << "ALL TESTS PASSED (" << total_tests << " tests).\n";
    else
        std::cout << "SOME TESTS FAILED: " << failures << " failures out of " << total_tests << " tests.\n";

    return (failures == 0) ? 0 : 1;
}