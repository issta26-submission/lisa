// Unit test suite for the focal method: png_do_expand_palette
// Targeted for C++11, no GTest, standalone test runner.
// This test suite creates minimal scaffolding to exercise the core logic
// of png_do_expand_palette as described in the provided source.
//
// Important: This test file is designed to be compiled and linked against the
// actual implementation of png_do_expand_palette (from pngrtran.c) using C linkage.
// The scaffolding below provides lightweight, self-contained stubs for the
// required types and constants to enable compilation in environments without
// libpng headers. The tests focus on behavior near the dependent components
// (palette expansion, bit depths 1/8, and RGBA expansion with/transparency).

#include <cstdint>
#include <vector>
#include <string>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>


// Provide C linkage for the focal function and necessary types.
// This mirrors minimal libpng-dependent structures used by png_do_expand_palette.

extern "C" {

// Critical constant from libpng (palette color type)
#define PNG_COLOR_TYPE_PALETTE 3

// Lightweight equivalents for the libpng types used by png_do_expand_palette
typedef uint8_t png_byte;
typedef png_byte* png_bytep;
typedef const png_byte* png_const_bytep;

typedef struct png_color_struct {
    png_byte red;
    png_byte green;
    png_byte blue;
} png_color;
typedef png_color* png_colorp;
typedef const png_color* png_const_colorp;

typedef struct png_row_info {
    unsigned int width;
    unsigned int color_type;
    unsigned int bit_depth;
    unsigned int rowbytes;
    unsigned int pixel_depth;
    unsigned int channels;
} png_row_info;
typedef png_row_info* png_row_infop;

// png_ptr type (opaque in this test harness; only riffled_palette is accessed by the code path we exercise)
typedef struct png_struct_struct {
    png_colorp riffled_palette;
} png_struct;
typedef png_struct* png_structrp;

// Prototype of the focal function under test
void png_do_expand_palette(png_structrp png_ptr, png_row_infop row_info,
    png_bytep row, png_const_colorp palette, png_const_bytep trans_alpha,
    int num_trans);
}

// Helper: simple stringify for test output
static void log_test(const char* name, bool ok, const std::string& detail = "")
{
    printf("[%s] %s%s\n",
           ok ? "PASS" : "FAIL",
           name,
           detail.empty() ? "" : (" - " + detail).c_str());
}

// --------------- Test Case A ---------------
// Description: Ensure no modification when color_type != PNG_COLOR_TYPE_PALETTE.
// Setup: color_type set to 2 (non-palette RGB). The function should bypass palette
// expansion entirely and leave the row intact.
bool test_case_A_no_palette_path()
{
    // Prepare a small input row (width = 4)
    uint8_t row_buf[4] = { 9, 9, 9, 9 };
    // Row info with non-palette color type
    png_row_info info;
    info.width = 4;
    info.color_type = 2;      // PNG_COLOR_TYPE_RGB (non-palette)
    info.bit_depth = 8;
    info.rowbytes = 4;
    info.pixel_depth = 24;
    info.channels = 3;

    // Palette and trans-alpha are irrelevant in this path, but we provide sane values.
    png_color palette[2];
    palette[0] = {0, 0, 0};
    palette[1] = {0, 0, 0};
    png_bytep row = row_buf;
    png_colorp pal_ptr = palette;
    png_const_bytep trans_alpha = nullptr;
    int num_trans = 0;

    // The focal function uses only row_info and row in this path.
    png_do_expand_palette(nullptr, &info, row, pal_ptr, trans_alpha, num_trans);

    // Expect no changes to the input row
    bool ok = std::memcmp(row_buf, (uint8_t[]){9,9,9,9}, 4) == 0;
    if (!ok) {
        log_test("test_case_A_no_palette_path", false, "Row was modified when color_type != PALETTE");
    } else {
        log_test("test_case_A_no_palette_path", true);
    }
    return ok;
}

// --------------- Test Case B ---------------
// Description: Expand a 1-bit-per-pixel palette row to 8-bit per pixel.
// Setup: width = 1, bit_depth = 1, color_type = PALETTE. Uses the MSB of the single input byte.
// Expectation: row[0] becomes 1 if MSB is 1, else 0. row_info bit_depth/pixel_depth updated to 8/8.
bool test_case_B_expand_bit_depth_1()
{
    // width = 1
    std::vector<uint8_t> buf(1, 0); // row buffer with 1 input byte
    // Set MSB to 1 to produce an expanded pixel value 1
    buf[0] = 0x80; // MSB set

    png_row_info info;
    info.width = 1;
    info.color_type = PNG_COLOR_TYPE_PALETTE; // palette path
    info.bit_depth = 1;                       // 1-bit input
    info.rowbytes = 1;
    info.pixel_depth = 8;
    info.channels = 1; // irrelevant for this test

    png_color palette[1];
    palette[0] = {0, 0, 0};
    png_bytep row = buf.data();
    png_colorp pal_ptr = palette;
    png_const_bytep trans_alpha = nullptr;
    int num_trans = 0;

    png_struct dummy_ptr; // not used in this path
    dummy_ptr riffled_palette = nullptr; // not used when color_type != PALETTE? but we pass  PALETTE path to be safe
    // Call function
    png_do_expand_palette((png_structrp)&dummy_ptr, &info, row, pal_ptr, trans_alpha, num_trans);

    // Expect row[0] == 1 (because MSB was 1)
    bool ok = (buf[0] == 1);
    if (!ok) {
        log_test("test_case_B_expand_bit_depth_1", false, "Expanded value mismatch. Expected 1 got " + std::to_string(buf[0]));
    } else {
        log_test("test_case_B_expand_bit_depth_1", true);
    }
    return ok;
}

// --------------- Test Case C ---------------
// Description: Expand 8-bit-per-pixel palette row with transparency (RGBA) output.
// Setup: width = 2, bit_depth = 8, color_type = PALETTE, num_trans > 0 to trigger RGBA path.
// Memory buffer is row_width*4 bytes, with first row_width bytes containing input palette indices.
// Expectation: Output memory layout is [R,G,B,A] for pixel0 followed by [R,G,B,A] for pixel1 in order.
bool test_case_C_expand_palette_to_rgba8_with_trans()
{
    const int row_width = 2;
    // Prepare input: two palette indices [0, 1], with 0 and 1 as indices.
    std::vector<uint8_t> buf(row_width * 4, 0); // expanded buffer: 8 bytes
    // Initialize input indices in the lower portion of the buffer
    buf[0] = 0; // first input index
    buf[1] = 1; // second input index

    png_row_info info;
    info.width = row_width;
    info.color_type = PNG_COLOR_TYPE_PALETTE;
    info.bit_depth = 8;
    info.rowbytes = row_width; // input rowbytes before expansion
    info.pixel_depth = 24;
    info.channels = 3;

    // Palette: two colors
    png_color palette[2];
    palette[0].red = 1;   palette[0].green = 2; palette[0].blue = 3;
    palette[1].red = 4;   palette[1].green = 5; palette[1].blue = 6;

    // Transparency array: for index0 use 0x11, for index1 use 0xFF (special case)
    png_bytep trans_alpha = new uint8_t[2];
    trans_alpha[0] = 0x11;
    trans_alpha[1] = 0x22; // Note: 1 >= num_trans will cause 0xFF for second pixel, see below
    // We'll set num_trans = 1 so index 1 triggers alpha = 0xFF
    int num_trans = 1;

    // We need a png_ptr; the function will check riffled_palette only under neon path, which is not enabled here.
    png_struct dummy_ptr;
    dummy_ptr riffled_palette = nullptr; // as safety

    png_bytep row_ptr = buf.data();
    png_colorp pal_ptr = palette;

    png_do_expand_palette((png_structrp)&dummy_ptr, &info, row_ptr, pal_ptr, trans_alpha, num_trans);

    // Expected memory arrangement:
    // Pixel 0 (index 0): red=1, green=2, blue=3, alpha=trans_alpha[0] = 0x11
    // Pixel 1 (index 1): red=4, green=5, blue=6, alpha=0xFF (since 1 >= num_trans)
    // Memory (low to high): [1,2,3,0x11, 4,5,6,0xFF]
    const uint8_t expected[8] = {1,2,3,0x11, 4,5,6,0xFF};
    bool ok = std::memcmp(buf.data(), expected, 8) == 0;

    if (!ok) {
        std::string detail = "Buffer content mismatch. Expected:";
        for (int i = 0; i < 8; ++i) detail += " " + std::to_string(expected[i]);
        detail += " ; Got:";
        for (int i = 0; i < 8; ++i) detail += " " + std::to_string(buf[i]);
        log_test("test_case_C_expand_palette_to_rgba8_with_trans", false, detail);
    } else {
        log_test("test_case_C_expand_palette_to_rgba8_with_trans", true);
    }

    delete[] trans_alpha;
    return ok;
}

// --------------- Test Case D ---------------
// Description: Expand 8-bit-per-pixel palette row to RGB (no alpha) when num_trans == 0.
// Setup: width = 1, bit_depth = 8, color_type = PALETTE, num_trans = 0.
// Expectation: Output memory is [red, green, blue] for the single pixel.
bool test_case_D_expand_palette_to_rgb8_no_trans()
{
    const int row_width = 1;
    std::vector<uint8_t> buf(row_width * 3, 0); // expanded buffer: 3 bytes

    // Input: single index 0
    buf[0] = 0;
    png_row_info info;
    info.width = row_width;
    info.color_type = PNG_COLOR_TYPE_PALETTE;
    info.bit_depth = 8;
    info.rowbytes = 1;
    info.pixel_depth = 24;
    info.channels = 3;

    // Palette: color for index 0
    png_color palette[1];
    palette[0].red = 7;   palette[0].green = 8; palette[0].blue = 9;

    png_bytep row = buf.data();
    png_colorp pal_ptr = palette;
    png_const_bytep trans_alpha = nullptr;
    int num_trans = 0;

    png_struct dummy_ptr;
    dummy_ptr riffled_palette = nullptr;

    png_do_expand_palette((png_structrp)&dummy_ptr, &info, row, pal_ptr, trans_alpha, num_trans);

    // Expected memory: [red, green, blue] => [7, 8, 9]
    const uint8_t expected[3] = {7, 8, 9};
    bool ok = std::memcmp(buf.data(), expected, 3) == 0;

    if (!ok) {
        std::string detail = "Expected RGB output [7,8,9], got [";
        for (int i = 0; i < 3; ++i) detail += std::to_string(buf[i]) + (i<2 ? "," : "");
        detail += "]";
        log_test("test_case_D_expand_palette_to_rgb8_no_trans", false, detail);
    } else {
        log_test("test_case_D_expand_palette_to_rgb8_no_trans", true);
    }
    return ok;
}

// --------------- Main ---------------

int main()
{
    int failures = 0;

    if (!test_case_A_no_palette_path()) failures++;
    if (!test_case_B_expand_bit_depth_1()) failures++;
    if (!test_case_C_expand_palette_to_rgba8_with_trans()) failures++;
    if (!test_case_D_expand_palette_to_rgb8_no_trans()) failures++;

    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("%d TEST(S) FAILED\n", failures);
        return 1;
    }
}