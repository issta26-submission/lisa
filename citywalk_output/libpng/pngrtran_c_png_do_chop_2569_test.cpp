// Unit test suite for png_do_chop (pngrtran.c) using libpng types.
// This test is written for C++11, without GoogleTest, and uses a lightweight
// test harness that prints PASS/FAIL messages while continuing execution.

#include <cstdint>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <png.h>


// Import C libpng headers in a C++ friendly way.
extern "C" {
}

// Lightweight test harness
static int g_total = 0;
static int g_failed = 0;

static void ExpectEqInt(const char* test_name, const char* desc, int expected, int actual) {
    ++g_total;
    if (expected != actual) {
        std::cerr << "FAIL: " << test_name << " - " << desc
                  << " | expected: " << expected << ", actual: " << actual << "\n";
        ++g_failed;
    } else {
        std::cout << "PASS: " << test_name << " - " << desc
                  << " (expected/actual = " << expected << ")\n";
    }
}

static void ExpectEqU8(const char* test_name, const char* desc, uint8_t expected, uint8_t actual) {
    ExpectEqInt(test_name, desc, static_cast<int>(expected), static_cast<int>(actual));
}

// Test 1: 16-bit depth chopping with single channel, verify proper row transformation
// and correct metadata updates (bit_depth, pixel_depth, rowbytes).
static bool test_png_do_chop_16bit_basic() {
    const int width = 3;
    const int channels = 1;
    png_row_info row_info;
    png_row_infop ri = &row_info;

    // Initialize row_info (as per libpng structure layout exposed in png.h)
    ri->bit_depth = 16;
    ri->width = width;
    ri->channels = channels;
    ri->rowbytes = width * channels * 2; // 6 bytes for 3 samples at 16 bits each

    // Prepare a 6-byte row with recognizable pattern
    // Layout: [0,1][2,3][4,5] (pairs for 3 samples)
    // Values chosen so that after chopping, first 3 bytes should reflect
    // the sequence of bytes at indices 0, 2, 4 (as per pixel processing)
    std::vector<png_byte> row(6);
    row[0] = 0x01; row[1] = 0x02;
    row[2] = 0x03; row[3] = 0x04;
    row[4] = 0x05; row[5] = 0x06;

    png_do_chop(ri, row.data());

    // After chop:
    // first 3 bytes should be: [row[0], row[2], row[4]] = 0x01, 0x03, 0x05
    ExpectEqU8("test_png_do_chop_16bit_basic", "row[0] after chop", 0x01, row[0]);
    ExpectEqU8("test_png_do_chop_16bit_basic", "row[1] after chop", 0x03, row[1]);
    ExpectEqU8("test_png_do_chop_16bit_basic", "row[2] after chop", 0x05, row[2]);

    // Ensure tail bytes unchanged (row[3], row[4], row[5])
    ExpectEqU8("test_png_do_chop_16bit_basic", "tail row[3] unchanged", 0x04, row[3]);
    ExpectEqU8("test_png_do_chop_16bit_basic", "tail row[4] unchanged", 0x05, row[4]);
    ExpectEqU8("test_png_do_chop_16bit_basic", "tail row[5] unchanged", 0x06, row[5]);

    // Metadata updates
    ExpectEqInt("test_png_do_chop_16bit_basic", "bit_depth updated to 8", 8, row_info.bit_depth);
    ExpectEqInt("test_png_do_chop_16bit_basic", "pixel_depth updated to 8 * channels", 8 * channels, row_info.pixel_depth);
    ExpectEqInt("test_png_do_chop_16bit_basic", "rowbytes updated to width * channels", width * channels, row_info.rowbytes);

    return g_failed == 0;
}

// Test 2: If bit_depth != 16, the function should perform no changes to row or metadata
static bool test_png_do_chop_noop_when_8bit() {
    const int width = 2;
    const int channels = 2;
    png_row_info row_info;
    png_row_infop ri = &row_info;

    ri->bit_depth = 8;
    ri->width = width;
    ri->channels = channels;
    ri->rowbytes = width * channels; // not used by 8-bit path, but realistic

    // Prepare a row of length rowbytes (4 bytes)
    std::vector<png_byte> row(ri->rowbytes);
    row[0] = 0x11; row[1] = 0x22;
    row[2] = 0x33; row[3] = 0x44;

    // Copy original state for later comparison
    std::vector<uint8_t> row_original = row;

    png_do_chop(ri, row.data());

    // Row should be unchanged
    for (size_t i = 0; i < row.size(); ++i) {
        ExpectEqInt("test_png_do_chop_noop_when_8bit", "row data unchanged", row_original[i], row[i]);
    }
    // Metadata unchanged
    ExpectEqInt("test_png_do_chop_noop_when_8bit", "bit_depth unchanged", 8, row_info.bit_depth);
    ExpectEqInt("test_png_do_chop_noop_when_8bit", "rowbytes unchanged", static_cast<int>(ri->rowbytes), 4);

    return g_failed == 0;
}

// Test 3: 16-bit chopping with multiple channels to verify multi-sample handling
static bool test_png_do_chop_16bit_multi_channel() {
    const int width = 2;
    const int channels = 2; // two samples per pixel, total 4 samples across row
    png_row_info row_info;
    png_row_infop ri = &row_info;

    ri->bit_depth = 16;
    ri->width = width;
    ri->channels = channels;
    ri->rowbytes = width * channels * 2; // 2 px * 2 channels * 2 bytes = 8 bytes

    // Create 8-byte row with pattern so that positions 0,2,4,6 are known
    // We'll set: [0]=0x01,[1]=0x02, [2]=0x03,[3]=0x04, [4]=0x05,[5]=0x06, [6]=0x07,[7]=0x08
    std::vector<png_byte> row(8);
    row[0] = 0x01; row[1] = 0x02;
    row[2] = 0x03; row[3] = 0x04;
    row[4] = 0x05; row[5] = 0x06;
    row[6] = 0x07; row[7] = 0x08;

    png_do_chop(ri, row.data());

    // After chop, first 4 bytes should be from indices 0,2,4,6 -> 0x01,0x03,0x05,0x07
    ExpectEqU8("test_png_do_chop_16bit_multi_channel", "row[0] after chop", 0x01, row[0]);
    ExpectEqU8("test_png_do_chop_16bit_multi_channel", "row[1] after chop", 0x03, row[1]);
    ExpectEqU8("test_png_do_chop_16bit_multi_channel", "row[2] after chop", 0x05, row[2]);
    ExpectEqU8("test_png_do_chop_16bit_multi_channel", "row[3] after chop", 0x07, row[3]);

    // Remaining tail bytes should remain unchanged: indices 4..7
    ExpectEqU8("test_png_do_chop_16bit_multi_channel", "row[4] tail unchanged", 0x05, row[4]);
    ExpectEqU8("test_png_do_chop_16bit_multi_channel", "row[5] tail unchanged", 0x06, row[5]);
    ExpectEqU8("test_png_do_chop_16bit_multi_channel", "row[6] tail unchanged", 0x07, row[6]);
    ExpectEqU8("test_png_do_chop_16bit_multi_channel", "row[7] tail unchanged", 0x08, row[7]);

    // Metadata updates
    ExpectEqInt("test_png_do_chop_16bit_multi_channel", "bit_depth updated to 8", 8, row_info.bit_depth);
    ExpectEqInt("test_png_do_chop_16bit_multi_channel", "pixel_depth updated to 8 * channels", 8 * channels, row_info.pixel_depth);
    ExpectEqInt("test_png_do_chop_16bit_multi_channel", "rowbytes updated to width * channels", width * channels, row_info.rowbytes);

    return g_failed == 0;
}

// Test 4: Edge case with zero width
static bool test_png_do_chop_zero_width() {
    const int width = 0;
    const int channels = 1;
    png_row_info row_info;
    png_row_infop ri = &row_info;

    ri->bit_depth = 16;
    ri->width = width;
    ri->channels = channels;
    ri->rowbytes = width * channels * 2; // 0

    std::vector<png_byte> row(0); // empty row

    png_do_chop(ri, row.data()); // should handle gracefully

    // After operation, bit_depth should have changed to 8 per the code path
    ExpectEqInt("test_png_do_chop_zero_width", "bit_depth == 8", 8, row_info.bit_depth);
    ExpectEqInt("test_png_do_chop_zero_width", "rowbytes == 0", 0, row_info.rowbytes);

    return g_failed == 0;
}

int main() {
    std::cout << "Starting png_do_chop unit tests (C++11, libpng integration)...\n";

    // Run tests; they intentionally continue on failures to maximize coverage.
    test_png_do_chop_16bit_basic();
    test_png_do_chop_noop_when_8bit();
    test_png_do_chop_16bit_multi_channel();
    test_png_do_chop_zero_width();

    // Summary
    if (g_failed == 0) {
        std::cout << "ALL TESTS PASSED (" << g_total << " tests).\n";
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED: " << g_failed << " failures out of " << g_total << " tests.\n";
        return 1;
    }
}