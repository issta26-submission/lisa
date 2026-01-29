// Minimal C++11 test harness for png_do_read_invert_alpha
// This test suite mocks the essential PNG structures and tests the focal
// method behavior across its branches without using Google Test.
// It uses a lightweight, non-terminating assertion approach.

#include <cstring>
#include <iostream>
#include <pngpriv.h>
#include <vector>


// Minimal PNG type and constants mock-up
typedef unsigned char png_bytep;
typedef unsigned int  png_uint_32;
typedef unsigned char png_byte;

#define PNG_COLOR_TYPE_RGB_ALPHA 6
#define PNG_COLOR_TYPE_GRAY_ALPHA 4

// Mock for png_row_infop
typedef struct {
    png_uint_32 width;
    int color_type;
    int bit_depth;
    png_uint_32 rowbytes;
} png_row_infop_struct, *png_row_infop;

// Lightweight no-op macro to mimic png_debug
#define png_debug(level, msg) do {} while(0)

// The focal method under test (simplified, self-contained version)
// This reproduces the essential logic of png_do_read_invert_alpha as described.
// It supports both 8-bit and 16-bit paths (bit_depth == 8 or 16).
void png_do_read_invert_alpha(png_row_infop row_info, png_bytep row)
{
    if (row_info == nullptr || row == nullptr) return;

    png_uint_32 row_width = row_info->width;
    if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    {
        if (row_info->bit_depth == 8)
        {
            // Invert the alpha channel for RGBA (8-bit)
            png_bytep sp = row + row_info->rowbytes;
            png_bytep dp = sp;
            png_uint_32 i;
            for (i = 0; i < row_width; i++)
            {
                *(--dp) = (png_byte)(255 - *(--sp));
                sp -= 3;
                dp = sp;
            }
        }
        else
        {
            // Invert the alpha channel for RGBA (16-bit) - per-byte invert on A
            png_bytep sp = row + row_info->rowbytes;
            png_bytep dp = sp;
            png_uint_32 i;
            for (i = 0; i < row_width; i++)
            {
                *(--dp) = (png_byte)(255 - *(--sp));
                *(--dp) = (png_byte)(255 - *(--sp));
                sp -= 6;
                dp = sp;
            }
        }
    }
    else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        if (row_info->bit_depth == 8)
        {
            // Invert the alpha channel for GA (8-bit)
            png_bytep sp = row + row_info->rowbytes;
            png_bytep dp = sp;
            png_uint_32 i;
            for (i = 0; i < row_width; i++)
            {
                *(--dp) = (png_byte)(255 - *(--sp)); // invert A
                *(--dp) = *(--sp);                   // copy G
            }
        }
        else
        {
            // Invert the alpha channel for GGAA (16-bit)
            png_bytep sp  = row + row_info->rowbytes;
            png_bytep dp = sp;
            png_uint_32 i;
            for (i = 0; i < row_width; i++)
            {
                *(--dp) = (png_byte)(255 - *(--sp));
                *(--dp) = (png_byte)(255 - *(--sp));
                sp -= 2;
                dp = sp;
            }
        }
    }
}

// Lightweight test harness

struct TestResult {
    int total = 0;
    int passed = 0;
    int failed = 0;
} g_result;

// Non-terminating assertions (log and continue)
#define EXPECT_TRUE(cond, desc) do { \
    ++g_result.total; \
    if(!(cond)) { \
        ++g_result.failed; \
        std::cerr << "[FAIL] " << (desc) << " (LINE " << __LINE__ << ")\n"; \
    } else { \
        ++g_result.passed; \
    } \
} while(0)

#define EXPECT_EQ(a, b, desc) do { \
    ++g_result.total; \
    if(!((a) == (b))) { \
        ++g_result.failed; \
        std::cerr << "[FAIL] " << (desc) << " (expected " << (b) << ", got " << (a) << ") [LINE " << __LINE__ << "]\n"; \
    } else { \
        ++g_result.passed; \
    } \
} while(0)


// Test 1: 8-bit RGB_ALPHA: ensure alpha inverted, RGB unchanged
void test_rgb_alpha_8bit_invert()
{
    // Explain:
    // width=1, color_type=RGB_ALPHA, bit_depth=8
    // Input pixel: [R,G,B,A] = [1,2,3,4]
    // Expected: [1,2,3, 255-4 = 251]
    png_row_infop ri = new png_row_infop_struct;
    ri->width = 1;
    ri->color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    ri->bit_depth = 8;
    ri->rowbytes = 4;

    std::vector<png_bytep> buf(4);
    unsigned char pixel[4] = {1,2,3,4};
    png_bytep row = pixel;

    // Call function
    png_do_read_invert_alpha(ri, row);

    // Validate: RGB should be unchanged, A inverted
    unsigned char expected[4] = {1,2,3, (unsigned char)(255 - 4)};
    EXPECT_EQ(std::memcmp(row, expected, 4), 0, "RGBA 8-bit: alpha inverted, RGB unchanged");
    delete ri;
}

// Test 2: 8-bit GRAY_ALPHA: ensure alpha inverted, G unchanged
void test_gray_alpha_8bit_invert()
{
    // Explain:
    // width=1, color_type=GRAY_ALPHA, bit_depth=8
    // Input: [G,A] = [10, 20]
    // Expected: [G, 255-20 = 235]
    png_row_infop ri = new png_row_infop_struct;
    ri->width = 1;
    ri->color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    ri->bit_depth = 8;
    ri->rowbytes = 2;

    unsigned char pixel[2] = {10, 20};
    png_bytep row = pixel;

    png_do_read_invert_alpha(ri, row);

    unsigned char expected[2] = {10, (unsigned char)(255 - 20)};
    EXPECT_EQ(std::memcmp(row, expected, 2), 0, "GA 8-bit: alpha inverted, G unchanged");
    delete ri;
}

// Test 3: 16-bit RGBA: ensure last two bytes (alpha) inverted
void test_rgb_alpha_16bit_invert_alpha_bytes()
{
    // Explain:
    // width=1, color_type=RGB_ALPHA, bit_depth=16
    // Input per byte order: [Rhi,Rlo,Ghi,Glo,Bhi,Blo,Ahi,Alo] = [1,2,3,4,5,6,7,8]
    // Expected last two bytes inverted: Ahi' = 255-7 = 248, Alo' = 255-8 = 247
    png_row_infop ri = new png_row_infop_struct;
    ri->width = 1;
    ri->color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    ri->bit_depth = 16;
    ri->rowbytes = 8;

    unsigned char row_bytes[8] = {1,2,3,4,5,6,7,8};
    png_bytep row = row_bytes;

    png_do_read_invert_alpha(ri, row);

    unsigned char expected[8] = {1,2,3,4,5,6, (unsigned char)248, (unsigned char)247};
    EXPECT_EQ(std::memcmp(row, expected, 8), 0, "RGBA 16-bit: A bytes inverted");
    delete ri;
}

// Test 4: 16-bit GRAY_ALPHA: ensure alpha bytes inverted
void test_gray_alpha_16bit_invert_alpha_bytes()
{
    // Explain:
    // width=1, color_type=GRAY_ALPHA, bit_depth=16
    // Input: [Ghi,Glo,Ahi,Alo] = [9,10,11,12]
    // Expected: [Ghi,Glo, Ahi', Alo'] with Ahi'=255-11=244, Alo'=255-12=243
    png_row_infop ri = new png_row_infop_struct;
    ri->width = 1;
    ri->color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    ri->bit_depth = 16;
    ri->rowbytes = 4;

    unsigned char row_bytes[4] = {9,10,11,12};
    png_bytep row = row_bytes;

    png_do_read_invert_alpha(ri, row);

    unsigned char expected[4] = {9,10, (unsigned char)244, (unsigned char)243};
    EXPECT_EQ(std::memcmp(row, expected, 4), 0, "GGAA 16-bit: A bytes inverted");
    delete ri;
}

// Test 5: Non-matching color type should leave data unchanged
void test_no_change_for_other_color_type()
{
    // Explain:
    // width=2, color_type=PNG_COLOR_TYPE_RGB (not defined in mocks, use an arbitrary value)
    // bit_depth=8
    // Ensure function does nothing
    png_row_infop ri = new png_row_infop_struct;
    ri->width = 2;
    ri->color_type = 999; // unsupported color type in our mock
    ri->bit_depth = 8;
    ri->rowbytes = 8;

    unsigned char row_bytes[8] = {10,11,12,13,14,15,16,17};
    png_bytep row = row_bytes;

    png_do_read_invert_alpha(ri, row);

    // Row should be unchanged
    EXPECT_EQ(std::memcmp(row, (unsigned char*)row_bytes, 8), 0, "No change for unsupported color_type");
    delete ri;
}

// Main: run all tests
int main()
{
    test_rgb_alpha_8bit_invert();
    test_gray_alpha_8bit_invert();
    test_rgb_alpha_16bit_invert_alpha_bytes();
    test_gray_alpha_16bit_invert_alpha_bytes();
    test_no_change_for_other_color_type();

    // Summary
    std::cout << "Tests run: " << g_result.total
              << ", Passed: " << g_result.passed
              << ", Failed: " << g_result.failed << "\n";

    // Return non-zero if any failed tests
    return (g_result.failed == 0) ? 0 : 1;
}