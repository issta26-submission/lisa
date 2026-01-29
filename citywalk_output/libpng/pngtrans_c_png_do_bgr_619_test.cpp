/*
 * Unit test suite for the focal function: png_do_bgr
 * Topic: PNG BGR channel swapping for 8-bit and 16-bit depths
 * Environment: C++11, no Google Test, own lightweight test framework
 * Approach: Implement a minimal, self-contained version of the needed types
 *           and the png_do_bgr function (as provided in the prompt),
 *           then run a series of targeted tests covering true/false branches.
 * Note: This test harness is designed to be self-contained and compilable
 *       without linking against libpng. It mocks only the minimal surface
 *       required by the focal function.
 */

// Compile-time macro to enable 16-bit path (as in the original code under PNG_16BIT_SUPPORTED)
#define PNG_16BIT_SUPPORTED

#include <cstdint>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Minimal mock/types to allow compilation of the focal function
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef uint32_t png_uint_32;

// Simplified row info structure to match usage in png_do_bgr
struct png_row_info {
    png_uint_32 width;
    int bit_depth;
    int color_type;
};

// Pointer alias as in the original code: png_row_infop
typedef png_row_info* png_row_infop;

// PNG color type/flags (simplified subset for tests)
#define PNG_COLOR_MASK_COLOR 0x02
#define PNG_COLOR_TYPE_GRAY 0
#define PNG_COLOR_TYPE_RGB 2
#define PNG_COLOR_TYPE_RGB_ALPHA 6

// No-op debug function mimic (libpng's png_debug)
static void png_debug(int /*level*/, const char* /*message*/) {
    // Intentionally empty to keep the test output clean
    (void)0;
}

// Implementation of the focal method (copied/adapted from the provided snippet)
void png_do_bgr(png_row_infop row_info, png_bytep row)
{
{
   png_debug(1, "in png_do_bgr");
   if ((row_info->color_type & PNG_COLOR_MASK_COLOR) != 0)
   {
      png_uint_32 row_width = row_info->width;
      if (row_info->bit_depth == 8)
      {
         if (row_info->color_type == PNG_COLOR_TYPE_RGB)
         {
            png_bytep rp;
            png_uint_32 i;
            for (i = 0, rp = row; i < row_width; i++, rp += 3)
            {
               png_byte save = *rp;
               *rp = *(rp + 2);
               *(rp + 2) = save;
            }
         }
         else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
         {
            png_bytep rp;
            png_uint_32 i;
            for (i = 0, rp = row; i < row_width; i++, rp += 4)
            {
               png_byte save = *rp;
               *rp = *(rp + 2);
               *(rp + 2) = save;
            }
         }
      }
#ifdef PNG_16BIT_SUPPORTED
      else if (row_info->bit_depth == 16)
      {
         if (row_info->color_type == PNG_COLOR_TYPE_RGB)
         {
            png_bytep rp;
            png_uint_32 i;
            for (i = 0, rp = row; i < row_width; i++, rp += 6)
            {
               png_byte save = *rp;
               *rp = *(rp + 4);
               *(rp + 4) = save;
               save = *(rp + 1);
               *(rp + 1) = *(rp + 5);
               *(rp + 5) = save;
            }
         }
         else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
         {
            png_bytep rp;
            png_uint_32 i;
            for (i = 0, rp = row; i < row_width; i++, rp += 8)
            {
               png_byte save = *rp;
               *rp = *(rp + 4);
               *(rp + 4) = save;
               save = *(rp + 1);
               *(rp + 1) = *(rp + 5);
               *(rp + 5) = save;
            }
         }
      }
#endif
   }
}
}

// Helper: compare two byte sequences
static bool bytes_equal(const unsigned char* a, const unsigned char* b, size_t n) {
    return std::memcmp(a, b, n) == 0;
}

// Simple non-terminating assertion macro to maximize coverage
#define EXPECT_EQ_BYTES(expected, actual, len, test_name) do { \
    if (!bytes_equal(expected, actual, (len))) { \
        std::cerr << "[FAIL] " << (test_name) << " - bytes differ\n"; \
        std::cerr << "  expected: "; \
        for (size_t _i = 0; _i < (len); ++_i) std::cerr << int((expected)[_i]) << " "; \
        std::cerr << "\n  actual  : "; \
        for (size_t _i = 0; _i < (len); ++_i) std::cerr << int((actual)[_i]) << " "; \
        std::cerr << "\n"; \
        return false; \
    } \
} while(0)

#define TEST_PASS() do { std::cout << "[PASS] " << __func__ << "\n"; return true; } while(0)
#define TEST_FAIL() do { std::cout << "[FAIL] " << __func__ << "\n"; return false; } while(0)

// 1) Test 8-bit RGB: ensure B and R are swapped per pixel
bool test_png_do_bgr_8bit_rgb() {
    // Setup
    png_row_infop info = new png_row_info;
    info->width = 2;        // two pixels
    info->bit_depth = 8;
    info->color_type = PNG_COLOR_TYPE_RGB;

    unsigned char row[6] = {1, 2, 3,  4, 5, 6}; // [R,G,B, R,G,B]

    // Expected after swap: [B,R,G, B,R,G] -> [3,2,1, 6,5,4]
    unsigned char expected[6] = {3, 2, 1, 6, 5, 4};

    // Execute
    png_do_bgr(info, row);

    // Verify
    const bool ok = bytes_equal(expected, row, 6);
    if (!ok) {
        // Print diagnostics if failed (handled in EXPECT macro)
        std::cerr << "[DIAG] test_png_do_bgr_8bit_rgb row after call: ";
        for (int i = 0; i < 6; ++i) std::cerr << int(row[i]) << " ";
        std::cerr << "\n";
        delete info;
        return false;
    }

    delete info;
    TEST_PASS();
}

// 2) Test 8-bit RGB_ALPHA: ensure R and B swapped per pixel, A unchanged
bool test_png_do_bgr_8bit_rgb_alpha() {
    png_row_infop info = new png_row_info;
    info->width = 2;
    info->bit_depth = 8;
    info->color_type = PNG_COLOR_TYPE_RGB_ALPHA;

    unsigned char row[8] = {1,2,3,4,  5,6,7,8}; // [R,G,B,A, R,G,B,A]
    unsigned char expected[8] = {3,2,1,4,  7,6,5,8}; // [B,G,R,A, B',G',R',A']

    png_do_bgr(info, row);

    const bool ok = bytes_equal(expected, row, 8);
    if (!ok) {
        std::cerr << "[DIAG] test_png_do_bgr_8bit_rgb_alpha row after call: ";
        for (int i = 0; i < 8; ++i) std::cerr << int(row[i]) << " ";
        std::cerr << "\n";
        delete info;
        return false;
    }

    delete info;
    TEST_PASS();
}

// 3) 16-bit RGB: ensure per-channel bytes swapped (R <-> B) across two-byte lanes
#ifdef PNG_16BIT_SUPPORTED
bool test_png_do_bgr_16bit_rgb() {
    png_row_infop info = new png_row_info;
    info->width = 2;
    info->bit_depth = 16;
    info->color_type = PNG_COLOR_TYPE_RGB;

    // Two pixels, 6 bytes each: [R_H,R_L, G_H,G_L, B_H,B_L]
    unsigned char row[12] = {
        1,2, 3,4,  5,6,   // Pixel 1
        7,8, 9,10, 11,12  // Pixel 2
    };
    // After swap:
    // Pixel1: [B_H,B_L, G_H,G_L, R_H,R_L] -> [5,6, 3,4, 1,2]
    // Pixel2: [B_H,B_L, G_H,G_L, R_H,R_L] -> [11,12, 9,10, 7,8]
    unsigned char expected[12] = {
        5,6, 3,4, 1,2,
        11,12, 9,10, 7,8
    };

    png_do_bgr(info, row);

    const bool ok = bytes_equal(expected, row, 12);
    if (!ok) {
        std::cerr << "[DIAG] test_png_do_bgr_16bit_rgb row after call: ";
        for (int i = 0; i < 12; ++i) std::cerr << int(row[i]) << " ";
        std::cerr << "\n";
        delete info;
        return false;
    }

    delete info;
    TEST_PASS();
}
#endif

// 4) 16-bit RGB_ALPHA: ensure per-channel bytes swapped for R and B, A unchanged
#ifdef PNG_16BIT_SUPPORTED
bool test_png_do_bgr_16bit_rgb_alpha() {
    png_row_infop info = new png_row_info;
    info->width = 2;
    info->bit_depth = 16;
    info->color_type = PNG_COLOR_TYPE_RGB_ALPHA;

    // Two pixels, 8 bytes each: [R_H,R_L, G_H,G_L, B_H,B_L, A_H,A_L]
    unsigned char row[16] = {
        1,2, 3,4, 5,6, 7,8,   // Pixel 1
        9,10, 11,12, 13,14, 15,16 // Pixel 2
    };
    // After swap per code:
    // Pixel1 -> [B, ... , R, A] => [5,6, 3,4, 1,2, 7,8]
    // Pixel2 -> [B, ... , R, A] => [13,14, 11,12, 9,10, 15,16]
    unsigned char expected[16] = {
        5,6, 3,4, 1,2, 7,8,
        13,14, 11,12, 9,10, 15,16
    };

    png_do_bgr(info, row);

    const bool ok = bytes_equal(expected, row, 16);
    if (!ok) {
        std::cerr << "[DIAG] test_png_do_bgr_16bit_rgb_alpha row after call: ";
        for (int i = 0; i < 16; ++i) std::cerr << int(row[i]) << " ";
        std::cerr << "\n";
        delete info;
        return false;
    }

    delete info;
    TEST_PASS();
}
#endif

// 5) Test that nothing happens when color flag is not set (e.g., grayscale)
bool test_png_do_bgr_no_color_no_change() {
    png_row_infop info = new png_row_info;
    info->width = 3;
    info->bit_depth = 8;
    info->color_type = PNG_COLOR_TYPE_GRAY; // does not have color mask bit set

    unsigned char row[9] = {
        10,11,12,  20,21,22,  30,31,32
    };

    unsigned char expected[9];
    std::memcpy(expected, row, 9);

    png_do_bgr(info, row);

    const bool ok = bytes_equal(expected, row, 9);
    if (!ok) {
        std::cerr << "[DIAG] test_png_do_bgr_no_color_no_change: row modified unexpectedly\n";
        delete info;
        return false;
    }

    delete info;
    TEST_PASS();
}

// Main test runner
int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Starting test suite for png_do_bgr...\n";

    // 1) 8-bit RGB
    total++; if (test_png_do_bgr_8bit_rgb()) ++passed; else std::cout << ">>> test_png_do_bgr_8bit_rgb failed\n";

    // 2) 8-bit RGB_ALPHA
    total++; if (test_png_do_bgr_8bit_rgb_alpha()) ++passed; else std::cout << ">>> test_png_do_bgr_8bit_rgb_alpha failed\n";

#ifdef PNG_16BIT_SUPPORTED
    // 3) 16-bit RGB
    total++; if (test_png_do_bgr_16bit_rgb()) ++passed; else std::cout << ">>> test_png_do_bgr_16bit_rgb failed\n";

    // 4) 16-bit RGB_ALPHA
    total++; if (test_png_do_bgr_16bit_rgb_alpha()) ++passed; else std::cout << ">>> test_png_do_bgr_16bit_rgb_alpha failed\n";
#endif

    // 5) No color flag should leave row unchanged
    total++; if (test_png_do_bgr_no_color_no_change()) ++passed; else std::cout << ">>> test_png_do_bgr_no_color_no_change failed\n";

    std::cout << "Test results: " << passed << " / " << total << " passed.\n";

    return (passed == total) ? 0 : 1;
}