// C++11 unit test suite for the focal function png_do_read_intrapixel
// This test suite does not rely on GTest. It provides a small, self-contained
// test harness using the project's own main() entry point to execute tests.

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Minimal re-implementation of the necessary libpng-like types and constants
// to exercise the focal function logic without requiring the entire libpng
// build environment.

typedef uint8_t  png_byte;
typedef uint32_t png_uint_32;

struct png_row_info {
    int color_type;
    int bit_depth;
    png_uint_32 width;
};

// Pseudo-typedefs matching the usage in the focal function
typedef png_row_info* png_row_infop;
typedef png_byte*      png_bytep;

// Constants approximating libpng color type flags used by the focal method
static const int PNG_COLOR_MASK_COLOR    = 0x02;
static const int PNG_COLOR_TYPE_RGB        = 2;
static const int PNG_COLOR_TYPE_RGB_ALPHA  = 6;

// The focal function under test (copied here to allow independent compilation
// without linking against libpng in the test environment).
extern "C" void png_do_read_intrapixel(png_row_infop row_info, png_bytep row)
{
    {
        // Original diagnostic call (no-op in this test harness)
        // In real libpng, would be: png_debug(1, "in png_do_read_intrapixel");
        (void)0;
        if (
            (row_info->color_type & PNG_COLOR_MASK_COLOR) != 0)
        {
            int bytes_per_pixel;
            png_uint_32 row_width = row_info->width;
            if (row_info->bit_depth == 8)
            {
                png_bytep rp;
                png_uint_32 i;
                if (row_info->color_type == PNG_COLOR_TYPE_RGB)
                    bytes_per_pixel = 3;
                else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
                    bytes_per_pixel = 4;
                else
                    return;
                for (i = 0, rp = row; i < row_width; i++, rp += bytes_per_pixel)
                {
                    *(rp) = (png_byte)((256 + *rp + *(rp + 1)) & 0xff);
                    *(rp+2) = (png_byte)((256 + *(rp + 2) + *(rp + 1)) & 0xff);
                }
            }
            else if (row_info->bit_depth == 16)
            {
                png_bytep rp;
                png_uint_32 i;
                if (row_info->color_type == PNG_COLOR_TYPE_RGB)
                    bytes_per_pixel = 6;
                else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
                    bytes_per_pixel = 8;
                else
                    return;
                for (i = 0, rp = row; i < row_width; i++, rp += bytes_per_pixel)
                {
                    png_uint_32 s0   = (png_uint_32)(*(rp    ) << 8) | *(rp + 1);
                    png_uint_32 s1   = (png_uint_32)(*(rp + 2) << 8) | *(rp + 3);
                    png_uint_32 s2   = (png_uint_32)(*(rp + 4) << 8) | *(rp + 5);
                    png_uint_32 red  = (s0 + s1 + 65536) & 0xffff;
                    png_uint_32 blue = (s2 + s1 + 65536) & 0xffff;
                    *(rp    ) = (png_byte)((red >> 8) & 0xff);
                    *(rp + 1) = (png_byte)(red & 0xff);
                    *(rp + 4) = (png_byte)((blue >> 8) & 0xff);
                    *(rp + 5) = (png_byte)(blue & 0xff);
                }
            }
        }
    }
}

//=============================================================
// Simple test harness (non-terminating assertions)
//=============================================================

// Global test result accumulator
static int g_failures = 0;

// Helper to print a summary at the end
static void print_summary(const std::string& title)
{
    std::cerr << "Test suite: " << title << " -> "
              << (g_failures ? "FAILED" : "PASSED")
              << " (" << g_failures << " failure" << (g_failures==1?"":"s") << ")\n";
}

// Logging helper for failures
static void log_failure(const std::string& test_name, const std::string& detail)
{
    ++g_failures;
    std::cerr << "[FAIL] " << test_name << ": " << detail << "\n";
}

// Equality assertion helpers specialized for clarity and safety
#define EXPECT_EQ_U8(expected, actual, test_name)                          \
    do { if ((static_cast<uint8_t>(expected) != static_cast<uint8_t>(actual)))  \
        log_failure(test_name, "Expected " + std::to_string(static_cast<int>(expected)) + \
                    " but got " + std::to_string(static_cast<int>(actual))); } while(0)

#define EXPECT_EQ_U64(expected, actual, test_name)                         \
    do { if ((static_cast<uint64_t>(expected) != static_cast<uint64_t>(actual)))  \
        log_failure(test_name, "Expected " + std::to_string(static_cast<uint64_t>(expected)) + \
                    " but got " + std::to_string(static_cast<uint64_t>(actual))); } while(0)

#define EXPECT_BUF_EQ(expected_ptr, actual_ptr, len, test_name)               \
    do {                                                                       \
        bool _ok = true;                                                     \
        for (size_t _i = 0; _i < (len); ++_i) {                               \
            if (static_cast<uint8_t>(expected_ptr[_i]) !=                         \
                static_cast<uint8_t>(actual_ptr[_i])) {                        \
                _ok = false; break;                                           \
            }                                                                 \
        }                                                                    \
        if (!_ok) { log_failure(test_name, "Buffer contents mismatch"); }     \
    } while(0)

//=============================================================
// Unit Tests
//=============================================================

// Test 1: 8-bit RGB, two pixels
static void test_png_do_read_intrapixel_8bit_rgb()
{
    const std::string test_name = "test_png_do_read_intrapixel_8bit_rgb";
    // Setup: bit_depth=8, color_type=RGB, width=2
    png_row_info row_info;
    row_info.color_type = PNG_COLOR_TYPE_RGB;
    row_info.bit_depth = 8;
    row_info.width = 2;

    // Row data: two pixels: [R,G,B, R,G,B] = [10,20,30, 100,50,25]
    uint8_t row_buf[6] = { 10, 20, 30, 100, 50, 25 };

    // Expected after processing:
    // Pixel 0: R <- (256 + 10 + 20) & 0xff = 286 & 0xff = 30
    //           B <- (256 + 30 + 20) & 0xff = 306 & 0xff = 50
    // Pixel 1: R <- (256 + 100 + 50) & 0xff = 406 & 0xff = 150
    //           B <- (256 + 25 + 50) & 0xff = 331 & 0xff = 75
    uint8_t expected[6] = { 30, 20, 50, 150, 50, 75 };

    png_do_read_intrapixel(&row_info, row_buf);

    EXPECT_BUF_EQ(expected, row_buf, 6, test_name);
}

// Test 2: 8-bit RGB_ALPHA, single pixel
static void test_png_do_read_intrapixel_8bit_rgb_alpha()
{
    const std::string test_name = "test_png_do_read_intrapixel_8bit_rgb_alpha";
    // Setup: bit_depth=8, color_type=RGB_ALPHA, width=1
    png_row_info row_info;
    row_info.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    row_info.bit_depth = 8;
    row_info.width = 1;

    // Row data: [R,G,B,A] = [120, 60, 200, 255]
    uint8_t row_buf[4] = { 120, 60, 200, 255 };

    // Expected after processing:
    // R <- (256 + 120 + 60) & 0xff = 436 & 0xff = 180
    // B <- (256 + 200 + 60) & 0xff = 516 & 0xff = 4
    uint8_t expected[4] = { 180, 60, 4, 255 };

    png_do_read_intrapixel(&row_info, row_buf);

    EXPECT_BUF_EQ(expected, row_buf, 4, test_name);
}

// Test 3: 16-bit RGB, single pixel
static void test_png_do_read_intrapixel_16bit_rgb()
{
    const std::string test_name = "test_png_do_read_intrapixel_16bit_rgb";
    // Setup: bit_depth=16, color_type=RGB, width=1
    png_row_info row_info;
    row_info.color_type = PNG_COLOR_TYPE_RGB;
    row_info.bit_depth = 16;
    row_info.width = 1;

    // Row data: [R(16bit), G(16bit), B(16bit)] in big-endian storage per libpng style
    // Let's pick: R = 0x0102, G = 0x0304, B = 0x0506
    uint8_t row_buf[6] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };

    // Expected:
    // s0 = 0x0102 = 258
    // s1 = 0x0304 = 772
    // s2 = 0x0506 = 1286
    // red = (s0 + s1 + 65536) & 0xffff = 1030; red >> 8 = 0x04, red & 0xff = 0x06
    // blue = (s2 + s1 + 65536) & 0xffff = 2066; blue >> 8 = 0x08, blue & 0xff = 0x12
    uint8_t expected[6] = { 0x04, 0x06, 0x03, 0x04, 0x08, 0x12 };

    png_do_read_intrapixel(&row_info, row_buf);

    EXPECT_BUF_EQ(expected, row_buf, 6, test_name);
}

// Test 4: Gray (non-color) type should remain unchanged
static void test_png_do_read_intrapixel_no_change_gray()
{
    const std::string test_name = "test_png_do_read_intrapixel_no_change_gray";
    // Setup: bit_depth=8, color_type=GRAY (non-color), width=2
    png_row_info row_info;
    row_info.color_type = 0; // PNG_COLOR_TYPE_GRAY hypothetically
    row_info.bit_depth = 8;
    row_info.width = 2;

    // Row data: 6 bytes, should remain unchanged
    uint8_t row_buf[6] = { 5, 6, 7, 8, 9, 10 };

    uint8_t expected[6] = { 5, 6, 7, 8, 9, 10 };

    png_do_read_intrapixel(&row_info, row_buf);

    EXPECT_BUF_EQ(expected, row_buf, 6, test_name);
}

// Test 5: 16-bit RGB_ALPHA path to ensure multiple channels are updated
static void test_png_do_read_intrapixel_16bit_rgb_alpha()
{
    const std::string test_name = "test_png_do_read_intrapixel_16bit_rgb_alpha";
    // Setup: bit_depth=16, color_type=RGB_ALPHA, width=1
    png_row_info row_info;
    row_info.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    row_info.bit_depth = 16;
    row_info.width = 1;

    // Row data: [R(16), G(16), B(16), A(16)]
    // We'll use small values to predict:
    // R = 0x0001, G = 0x0001, B = 0x0001, A = 0xFFFF
    uint8_t row_buf[8] = { 0x00,0x01, 0x00,0x01, 0x00,0x01, 0xFF,0xFF };

    // Expected as derived in analysis:
    // s0=0x0001, s1=0x0001, s2=0x0001
    // red = (1+1+65536) & 0xffff = 2 -> 0x0002
    // red >> 8 = 0x00, red & 0xff = 0x02
    // blue = (1+1+65536) & 0xffff = 2 -> 0x0002
    // blue >> 8 = 0x00, blue & 0xff = 0x02
    uint8_t expected[8] = { 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0xFF, 0xFF };

    png_do_read_intrapixel(&row_info, row_buf);

    EXPECT_BUF_EQ(expected, row_buf, 8, test_name);
}

//=============================================================
// Main
//=============================================================
int main()
{
    // Run tests
    test_png_do_read_intrapixel_8bit_rgb();
    test_png_do_read_intrapixel_8bit_rgb_alpha();
    test_png_do_read_intrapixel_16bit_rgb();
    test_png_do_read_intrapixel_no_change_gray();
    test_png_do_read_intrapixel_16bit_rgb_alpha();

    // Print summary
    print_summary("png_do_read_intrapixel coverage");

    // Return non-zero if any test failed
    return g_failures ? 1 : 0;
}