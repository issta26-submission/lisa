// Lightweight C++11 unit tests for the function png_do_expand_16
// This test suite focuses on the in-place 8-bit to 16-bit expansion logic
// described in the focal method. It provides a minimal, self-contained
// environment to exercise the core behavior without relying on GTest.
// The tests are designed to be non-terminating (they log failures and continue).

#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstddef>


// Minimal stand-in types and constants to mirror the focal function's expectations.
// We purposefully recreate only the pieces needed for the tests.
typedef unsigned char png_byte;
typedef unsigned char png_bytep;
typedef struct png_row_info {
    int bit_depth;
    int color_type;
    size_t rowbytes;
    int channels;
    int pixel_depth;
} png_row_info;
typedef png_row_info* png_row_infop;

const int PNG_COLOR_TYPE_PALETTE = 3;
const int PNG_COLOR_TYPE_RGB = 2;

// --- Reproduction of the focal method (png_do_expand_16) in this test file ---
// The real project would provide this from pngrtran.c; here we re-implement to
// validate behavior in isolation.
void png_do_expand_16(png_row_infop row_info, png_bytep row)
{
{
   if (row_info->bit_depth == 8 &&
      row_info->color_type != PNG_COLOR_TYPE_PALETTE)
   {
      png_byte *sp = row + row_info->rowbytes; /* source, last byte + 1 */
      png_byte *dp = sp + row_info->rowbytes;  /* destination, end + 1 */
      while (dp > sp)
      {
         dp[-2] = dp[-1] = *--sp; dp -= 2;
      }
      row_info->rowbytes *= 2;
      row_info->bit_depth = 16;
      row_info->pixel_depth = (png_byte)(row_info->channels * 16);
   }
}
}

// Simple non-terminating test assertion macro.
// On failure, logs the message but does not abort the test suite.
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT FAIL: " << msg << "\n"; \
        ++g_failures; \
    } \
} while(false)

// Test 1: Expand 8-bit RGB row (bit_depth=8, color_type=RGB) should double the row size
// and produce an expanded 16-bit per-channel row (pattern: 1,1,2,2,3,3 from input 1,2,3).
void test_expand_8bit_rgb_expansion() {
    // Setup: 3 input bytes, 3 channels (RGB)
    png_row_info ri;
    ri.bit_depth = 8;
    ri.color_type = PNG_COLOR_TYPE_RGB;
    ri.rowbytes = 3;
    ri.channels = 3;
    ri.pixel_depth = 0;

    // Buffer must hold 2 * rowbytes after expansion.
    unsigned char buf[6] = {1, 2, 3, 0, 0, 0};

    // Act
    png_do_expand_16(&ri, buf);

    // Expected: [1,1,2,2,3,3]
    unsigned char expected[6] = {1, 1, 2, 2, 3, 3};

    // Assert
    TEST_ASSERT(ri.rowbytes == 6, "Test1: rowbytes should double to 6 after expansion");
    TEST_ASSERT(ri.bit_depth == 16, "Test1: bit_depth should become 16 after expansion");
    TEST_ASSERT(ri.pixel_depth == 48, "Test1: pixel_depth should be channels*16 (3*16=48)");
    TEST_ASSERT(std::memcmp(buf, expected, 6) == 0,
                "Test1: expanded data should be [1,1,2,2,3,3]");
}

// Test 2: Do not expand when color type is PALETTE
void test_no_expand_palette_color_type() {
    png_row_info ri;
    ri.bit_depth = 8;
    ri.color_type = PNG_COLOR_TYPE_PALETTE;
    ri.rowbytes = 3;
    ri.channels = 3;
    ri.pixel_depth = 0;

    unsigned char buf[6] = {5, 6, 7, 99, 99, 99};
    unsigned char before[6];
    std::memcpy(before, buf, 6);

    png_do_expand_16(&ri, buf);

    // Assert: no changes to row/rowbytes/bit_depth and memory remains unchanged
    TEST_ASSERT(ri.rowbytes == 3, "Test2: rowbytes should remain unchanged for PALETTE type");
    TEST_ASSERT(ri.bit_depth == 8, "Test2: bit_depth should remain unchanged for PALETTE type");
    TEST_ASSERT(std::memcmp(buf, before, 6) == 0,
                "Test2: buffer should remain unchanged when color_type is PALETTE");
}

// Test 3: Do not expand when bit_depth is not 8
void test_no_expand_wrong_bit_depth() {
    png_row_info ri;
    ri.bit_depth = 4; // not 8
    ri.color_type = PNG_COLOR_TYPE_RGB;
    ri.rowbytes = 3;
    ri.channels = 3;
    ri.pixel_depth = 0;

    unsigned char buf[6] = {9, 8, 7, 6, 5, 4};
    unsigned char before[6];
    std::memcpy(before, buf, 6);

    png_do_expand_16(&ri, buf);

    // Assert: no changes
    TEST_ASSERT(ri.rowbytes == 3, "Test3: rowbytes should remain unchanged when bit_depth != 8");
    TEST_ASSERT(ri.bit_depth == 4, "Test3: bit_depth should remain unchanged when bit_depth != 8");
    TEST_ASSERT(std::memcmp(buf, before, 6) == 0,
                "Test3: buffer should remain unchanged when bit_depth != 8");
}

// Test 4: Expand 8-bit single-channel (grayscale) should still expand to 16-bit per sample
// Input: [10, 20, 30] with channels=1 -> [10,10,20,20,30,30]
void test_expand_single_channel() {
    png_row_info ri;
    ri.bit_depth = 8;
    ri.color_type = PNG_COLOR_TYPE_RGB; // not PALETTE
    ri.rowbytes = 3;
    ri.channels = 1;
    ri.pixel_depth = 0;

    unsigned char buf[6] = {10, 20, 30, 0, 0, 0};

    png_do_expand_16(&ri, buf);

    unsigned char expected[6] = {10, 10, 20, 20, 30, 30};

    TEST_ASSERT(ri.rowbytes == 6, "Test4: rowbytes should double to 6 for 3 input bytes with 1 channel");
    TEST_ASSERT(ri.bit_depth == 16, "Test4: bit_depth should become 16 after expansion");
    TEST_ASSERT(ri.pixel_depth == 16, "Test4: pixel_depth should be channels*16 (1*16=16)");
    TEST_ASSERT(std::memcmp(buf, expected, 6) == 0,
                "Test4: expanded data for single-channel should be [10,10,20,20,30,30]");
}

// Helper: Run all tests and report summary
void run_all_tests() {
    test_expand_8bit_rgb_expansion();
    test_no_expand_palette_color_type();
    test_no_expand_wrong_bit_depth();
    test_expand_single_channel();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
    } else {
        std::cout << g_failures << " TEST(S) FAILED\n";
    }
}

int main() {
    run_all_tests();
    return (g_failures == 0) ? 0 : 1;
}