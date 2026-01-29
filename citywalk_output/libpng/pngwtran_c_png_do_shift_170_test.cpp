#include <algorithm>
#include <vector>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


extern "C" {
}

// Forward declaration of the function under test (linking will resolve the actual implementation)
extern "C" void png_do_shift(png_row_infop row_info, png_bytep row, png_const_color_8p bit_depth);

// Simple non-terminating assertion helpers
static int g_fail_count = 0;
#define EXPECT_EQ(a,b) do { if ((a) != (b)) { \
    printf("EXPECT_EQ failed at %s:%d: %s != %s\n", __FILE__, __LINE__, #a, #b); g_fail_count++; } } while(0)
#define EXPECT_TRUE(cond) do { if (!(cond)) { \
    printf("EXPECT_TRUE failed at %s:%d: %s is false\n", __FILE__, __LINE__, #cond); g_fail_count++; } } while(0)
#define EXPECT_MEM_EQ(a,b,n) do { if (std::memcmp((a),(b),(n)) != 0) { \
    printf("EXPECT_MEM_EQ failed at %s:%d: memory regions differ\n", __FILE__, __LINE__); g_fail_count++; } } while(0)

// Helper to compare two raw byte buffers
static bool bytes_equal(const png_bytep a, const png_bytep b, size_t n) {
    return std::memcmp(a, b, n) == 0;
}

/*
Test 1: Palette color type should cause png_do_shift to skip any processing.
- Setup a row with color_type = PNG_COLOR_TYPE_PALETTE.
- Call png_do_shift and verify the row bytes remain unchanged.
*/
static bool test_palette_no_op() {
    printf("Test 1: Palette type bypass (no operation)\n");
    struct png_row_info ri;
    // Zero initialize and set required fields
    std::memset(&ri, 0, sizeof(ri));
    ri.width = 1;
    ri.rowbytes = 3; // 1 pixel, 3 channels would be typical, but for palette we won't enter the branch
    ri.color_type = PNG_COLOR_TYPE_PALETTE;
    ri.bit_depth = 8;

    std::vector<png_byte> row(ri.rowbytes);
    // Initial data
    row[0] = 0x11;
    row[1] = 0x22;
    row[2] = 0x33;

    std::vector<png_byte> orig = row;

    // Bit depth descriptor (values won't affect execution for PALETTE)
    png_color_8 color_depth;
    color_depth.red = 8;
    color_depth.green = 8;
    color_depth.blue = 8;
    color_depth.gray = 8;
    color_depth.alpha = 8;

    png_do_shift(&ri, row.data(), &color_depth);

    bool ok = bytes_equal(row.data(), orig.data(), ri.rowbytes);
    EXPECT_TRUE(ok);
    if (!ok) {
        printf("Row changed in palette mode (unexpected).\n");
    }
    return ok;
}

/*
Test 2: 8-bit source depth with RGB (true color) and destination depth matching source (no effective shift).
- Expect that with bit_depth values set to 8 for all channels and row_depth = 8,
  the operation yields identical data (no change).
*/
static bool test_8bit_no_shift() {
    printf("Test 2: 8-bit true color with no shift (bit_depth == 8 for all channels)\n");
    struct png_row_info ri;
    std::memset(&ri, 0, sizeof(ri));
    ri.width = 1;
    ri.rowbytes = 3;
    ri.color_type = PNG_COLOR_TYPE_RGB;
    ri.bit_depth = 8;

    std::vector<png_byte> row(ri.rowbytes);
    // Arbitrary values
    row[0] = 0x12; // red
    row[1] = 0x34; // green
    row[2] = 0x56; // blue

    std::vector<png_byte> orig = row;

    // bit_depth descriptor with 8 bits per channel (no shift)
    png_color_8 color_depth;
    color_depth.red = 8;
    color_depth.green = 8;
    color_depth.blue = 8;
    color_depth.gray = 8;
    color_depth.alpha = 8;

    png_do_shift(&ri, row.data(), &color_depth);

    bool ok = bytes_equal(row.data(), orig.data(), ri.rowbytes);
    EXPECT_TRUE(ok);
    if (!ok) {
        printf("8-bit no-shift test: data changed unexpectedly.\n");
    }
    return ok;
}

/*
Test 3: 16-bit source depth with RGB and destination depth 8 per channel (typical 16->16 style transform scenario).
- We'll set row_depth to 16 and bit_depth to 8 for all channels, which should produce
  a predictable pattern: for each channel value v (16-bit), the code computes
  value_out = (v << 8) | (v >> 0) and stores as two bytes (high, low):
  high = (value_out >> 8) & 0xFF, low = value_out & 0xFF.
  If v is 0x0001, result is 0x01 0x01; for 0x0002 -> 0x02 0x02; etc.
*/
static bool test_16bit_shift() {
    printf("Test 3: 16-bit source depth with RGB and 8-bit per channel destination\n");
    struct png_row_info ri;
    std::memset(&ri, 0, sizeof(ri));
    ri.width = 1;
    ri.rowbytes = 6; // 3 channels * 2 bytes
    ri.color_type = PNG_COLOR_TYPE_RGB;
    ri.bit_depth = 16;

    std::vector<png_byte> row(ri.rowbytes);
    // Set 16-bit values per channel: R=0x0001, G=0x0002, B=0x0003
    row[0] = 0x00; row[1] = 0x01; // R
    row[2] = 0x00; row[3] = 0x02; // G
    row[4] = 0x00; row[5] = 0x03; // B

    // Prepare expected results
    std::vector<png_byte> expected = {
        0x01, 0x01, // R becomes 0x0101
        0x02, 0x02, // G becomes 0x0202
        0x03, 0x03  // B becomes 0x0303
    };

    png_color_8 color_depth;
    // Destination depth 8 bits per channel
    color_depth.red = 8;
    color_depth.green = 8;
    color_depth.blue = 8;
    color_depth.gray = 8;
    color_depth.alpha = 8;

    png_do_shift(&ri, row.data(), &color_depth);

    bool ok = bytes_equal(row.data(), expected.data(), ri.rowbytes);
    EXPECT_TRUE(ok);
    if (!ok) {
        printf("16-bit shift test failed. Expected:");
        for (size_t i = 0; i < expected.size(); ++i) printf(" %02x", (unsigned)expected[i]);
        printf(" but got:");
        for (size_t i = 0; i < row.size(); ++i) printf(" %02x", (unsigned)row[i]);
        printf("\n");
    }
    return ok;
}

// Entry point for running tests
int main() {
    printf("Starting png_do_shift unit tests (C++11, standalone harness, no GTest)\n");

    // Run tests
    bool t1 = test_palette_no_op();
    bool t2 = test_8bit_no_shift();
    bool t3 = test_16bit_shift();

    int total = 3;
    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0) + (t3 ? 1 : 0);

    if (g_fail_count == 0) {
        printf("All tests passed (%d/%d)\n", passed, total);
        return 0;
    } else {
        printf("Some tests failed: %d failures out of %d tests\n", g_fail_count, total);
        return 1;
    }
}