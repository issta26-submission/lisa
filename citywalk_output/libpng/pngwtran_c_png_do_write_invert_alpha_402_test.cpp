// Lightweight C++11 unit tests for the focal function: png_do_write_invert_alpha
// This test harness provides a minimal, self-contained environment to verify
// the behavior of the alpha-inversion logic for RGBA and Gray+Alpha pixel formats.
// It mirrors the original control flow and data manipulations found in the focal method.
// Note: This is a stand-alone test harness to exercise the algorithm without external
// dependencies (e.g., libpng). It uses a small mock-up of the necessary types.

// Includes
#include <cstdint>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain knowledge adaptation: minimal mock of libpng types and constants
// These definitions are crafted to reproduce the decision branches of the focal method.
typedef unsigned char png_bytep;
typedef unsigned int  png_uint_32;

struct png_row_infop {
    int color_type;
    int bit_depth;
    unsigned int width;
};

// Color type constants (subset sufficient for test coverage)
enum {
    PNG_COLOR_TYPE_GRAY       = 0,
    PNG_COLOR_TYPE_RGB        = 2,
    PNG_COLOR_TYPE_GRAY_ALPHA = 4,
    PNG_COLOR_TYPE_RGB_ALPHA  = 6
};

// Optional 16-bit path inclusion for coverage
#define PNG_WRITE_16BIT_SUPPORTED 1

// Stub for the debug function used by the focal method
static inline void png_debug(int /*level*/, const char* /*msg*/) {
    // No-op for test environment
    (void)level; (void)msg;
}

// Forward declaration and implementation of the focal method under test.
// This is a self-contained reproduction of the core algorithm for unit testing.
// Note: In a real project, you would link against the actual pngwtran.c implementation.
// The tests here exercise the same logic in isolation.
static void png_do_write_invert_alpha(png_row_infop row_info, png_bytep row) {
    // Mimic the structure and branches from the focal method
    png_debug(1, "in png_do_write_invert_alpha");
    if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA) {
        if (row_info->bit_depth == 8) {
            // This inverts the alpha channel in RGBA (8-bit per channel)
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;
            for (i = 0, sp = dp = row; i < row_width; i++) {
                // Skip RGB (3 bytes), target alpha (4th byte)
                sp += 3;
                dp = sp;
                *dp = (png_bytep)(255 - *(sp++)); // invert alpha byte
            }
        }
#ifdef PNG_WRITE_16BIT_SUPPORTED
        else {
            // This inverts the alpha channel in RRGGBBAA (16-bit per channel)
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;
            for (i = 0, sp = dp = row; i < row_width; i++) {
                // Skip RRGGBB (6 bytes), focus on AA (two bytes)
                sp += 6;
                dp = sp;
                *(dp++) = (png_byte)(255 - *(sp++)); // invert A_L
                *dp     = (png_byte)(255 - *(sp++)); // invert A_H
            }
        }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
    }
    else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        if (row_info->bit_depth == 8) {
            // This inverts the alpha channel in GA (8-bit)
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;
            for (i = 0, sp = dp = row; i < row_width; i++) {
                *(dp++) = *(sp++);                 // copy Gray unchanged
                *(dp++) = (png_byte)(255 - *(sp++)); // invert Alpha
            }
        }
#ifdef PNG_WRITE_16BIT_SUPPORTED
        else {
            // This inverts the alpha channel in GGAA (16-bit)
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;
            for (i = 0, sp = dp = row; i < row_width; i++) {
                // Skip GG (2 bytes), focus on AA (two bytes)
                sp += 2;
                dp = sp;
                *(dp++) = (png_byte)(255 - *(sp++)); // invert AA_L
                *dp     = (png_byte)(255 - *(sp++)); // invert AA_H
            }
        }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
    }
}

// Simple assertion helpers for non-terminating checks
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

static void test_passed(const char* test_name) {
    ++g_tests_run;
    ++g_tests_passed;
    std::cout << "[OK] " << test_name << "\n";
}

static void test_failed(const char* test_name, const char* reason) {
    ++g_tests_run;
    ++g_tests_failed;
    std::cout << "[FAIL] " << test_name << " - " << reason << "\n";
}

// Helper to compare raw memory
static bool mem_equal(const unsigned char* a, const unsigned char* b, size_t n) {
    return std::memcmp(a, b, n) == 0;
}

/*
 Test 1: RGBA (8-bit) inversion
 - Setup: 3 pixels RGBA, all channels 8-bit.
 - Expect: alpha channel inverted to 255 - alpha; RGB channels unchanged.
 - Color type: PNG_COLOR_TYPE_RGB_ALPHA, bit depth: 8
*/
static void test_rgb_alpha_8bit_invert() {
    const unsigned int width = 3;
    std::vector<unsigned char> row(width * 4, 0);
    // Initialize pixels: [R,G,B,A] for 3 pixels
    // Pixel 0: A=50, Pixel 1: A=100, Pixel 2: A=200
    row[0] = 10; row[1] = 20; row[2] = 30; row[3] = 50;
    row[4] = 11; row[5] = 21; row[6] = 31; row[7] = 100;
    row[8] = 12; row[9] = 22; row[10] = 32; row[11] = 200;

    png_row_infop info;
    info.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    info.bit_depth  = 8;
    info.width      = width;

    png_do_write_invert_alpha(&info, row.data());

    // Expected: A' = 255 - A, RGB unchanged
    unsigned char expected[12] = {
        10, 20, 30, static_cast<unsigned char>(205), // 255-50
        11, 21, 31, static_cast<unsigned char>(155), // 255-100
        12, 22, 32, static_cast<unsigned char>(55)    // 255-200
    };
    if (mem_equal(row.data(), expected, 12)) {
        test_passed("test_rgb_alpha_8bit_invert");
    } else {
        test_failed("test_rgb_alpha_8bit_invert", "RGBA8 inversion did not produce expected alpha values.");
        std::cerr << "Expected: ";
        for (int i = 0; i < 12; ++i) std::cerr << int(expected[i]) << " ";
        std::cerr << "\nActual:   ";
        for (int i = 0; i < 12; ++i) std::cerr << int(row[i]) << " ";
        std::cerr << "\n";
    }
}

/*
 Test 2: RGBA (8-bit) branch false path (color_type mismatch)
 - Setup: color_type RGB (no alpha)
 - Expect: No changes to the row buffer.
*/
static void test_rgb_alpha_8bit_color_type_mismatch_noop() {
    const unsigned int width = 2;
    std::vector<unsigned char> row(width * 3, 0);
    // Initialize pattern
    row[0] = 1; row[1] = 2; row[2] = 3;
    row[3] = 4; row[4] = 5; row[5] = 6;

    png_row_infop info;
    info.color_type = PNG_COLOR_TYPE_RGB; // not RGBA
    info.bit_depth  = 8;
    info.width      = width;

    // Copy original data for later comparison
    std::vector<unsigned char> before = row;

    png_do_write_invert_alpha(&info, row.data());

    if (mem_equal(row.data(), before.data(), row.size())) {
        test_passed("test_rgb_alpha_8bit_color_type_mismatch_noop");
    } else {
        test_failed("test_rgb_alpha_8bit_color_type_mismatch_noop", "Row was modified despite color_type mismatch.");
    }
}

/*
 Test 3: Gray+Alpha (8-bit) inversion
 - Setup: 4 pixels GA with 8-bit per channel
 - Expect: Gray values unchanged; Alpha values inverted.
 - Color type: PNG_COLOR_TYPE_GRAY_ALPHA, bit depth: 8
*/
static void test_gray_alpha_8bit_invert() {
    const unsigned int width = 4;
    std::vector<unsigned char> row(width * 2, 0);
    // Pixel i: Gray = i, Alpha = i*10
    for (unsigned int i = 0; i < width; ++i) {
        row[i*2 + 0] = static_cast<unsigned char>(i);         // Gray
        row[i*2 + 1] = static_cast<unsigned char>(i * 10);    // Alpha
    }

    png_row_infop info;
    info.color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    info.bit_depth  = 8;
    info.width      = width;

    png_do_write_invert_alpha(&info, row.data());

    // Expected: Gray unchanged; Alpha inverted: 255 - (i*10)
    unsigned char expected[8];
    for (unsigned int i = 0; i < width; ++i) {
        expected[2*i + 0] = static_cast<unsigned char>(i);              // Gray unchanged
        expected[2*i + 1] = static_cast<unsigned char>(255 - (i*10));   // Alpha inverted
    }

    if (mem_equal(row.data(), expected, 8)) {
        test_passed("test_gray_alpha_8bit_invert");
    } else {
        test_failed("test_gray_alpha_8bit_invert", "GA8 inversion did not produce expected results.");
        std::cerr << "Expected: ";
        for (int i = 0; i < 8; ++i) std::cerr << int(expected[i]) << " ";
        std::cerr << "\nActual:   ";
        for (int i = 0; i < 8; ++i) std::cerr << int(row[i]) << " ";
        std::cerr << "\n";
    }
}

#ifdef PNG_WRITE_16BIT_SUPPORTED
/*
 Test 4: RGBA (16-bit) inversion
 - Setup: 2 pixels RGBA, 16-bit per channel
 - Expect: Each A_L and A_H byte inverted individually.
*/
static void test_rgb_alpha_16bit_invert() {
    const unsigned int width = 2;
    std::vector<unsigned char> row(width * 8, 0); // 4 channels * 2 bytes

    // Pixel 0: R=1, G=2, B=3, A=4 (little-endian per channel)
    row[0] = 0x01; row[1] = 0x00; // R
    row[2] = 0x02; row[3] = 0x00; // G
    row[4] = 0x03; row[5] = 0x00; // B
    row[6] = 0x04; row[7] = 0x00; // A

    // Pixel 1: R=255, G=170, B=85, A=16
    row[8]  = 0xFF; row[9]  = 0x00; // R
    row[10] = 0xAA; row[11] = 0x00; // G
    row[12] = 0x55; row[13] = 0x00; // B
    row[14] = 0x10; row[15] = 0x00; // A

    png_row_infop info;
    info.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    info.bit_depth  = 16;
    info.width      = width;

    png_do_write_invert_alpha(&info, row.data());

    // Expected A bytes after inversion:
    // Pixel 0: A_L' = 255 - 4 = 251 (0xFB), A_H' = 255 - 0 = 255 (0xFF)
    // Pixel 1: A_L' = 255 - 16 = 239 (0xEF), A_H' = 255 - 0 = 255 (0xFF)
    unsigned char expected[16] = {
        // Pixel 0
        0x01,0x00,  // R
        0x02,0x00,  // G
        0x03,0x00,  // B
        0xFB,0xFF,  // A
        // Pixel 1
        0xFF,0x00,  // R
        0xAA,0x00,  // G
        0x55,0x00,  // B
        0xEF,0xFF   // A
    };

    if (mem_equal(row.data(), expected, 16)) {
        test_passed("test_rgb_alpha_16bit_invert");
    } else {
        test_failed("test_rgb_alpha_16bit_invert", "RGBA16 inversion did not produce expected alpha bytes.");
        std::cerr << "Expected: ";
        for (int i = 0; i < 16; ++i) std::cerr << int(expected[i]) << " ";
        std::cerr << "\nActual:   ";
        for (int i = 0; i < 16; ++i) std::cerr << int(row[i]) << " ";
        std::cerr << "\n";
    }
}

/*
 Test 5: Gray+Alpha (16-bit) inversion
 - Setup: 2 pixels GGAA, 16-bit per channel
 - Expect: Alpha bytes inverted individually; Gray bytes unchanged.
*/
static void test_gray_alpha_16bit_invert() {
    const unsigned int width = 2;
    std::vector<unsigned char> row(width * 4, 0); // 2 channels (Gray) + 2 (Alpha) per pixel

    // Pixel 0: Gray=1 (0x0001), Alpha=2 (0x0002)
    row[0] = 0x01; row[1] = 0x00; // Gray
    row[2] = 0x02; row[3] = 0x00; // Alpha

    // Pixel 1: Gray=3 (0x0003), Alpha=4 (0x0004)
    row[4] = 0x03; row[5] = 0x00; // Gray
    row[6] = 0x04; row[7] = 0x00; // Alpha

    png_row_infop info;
    info.color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    info.bit_depth  = 16;
    info.width      = width;

    png_do_write_invert_alpha(&info, row.data());

    // Expected:
    // Pixel 0: Gray unchanged (0x0001, 0x0000); Alpha -> 0xFFFD, 0xFFFF? Wait: A_L' = 255-2 = 253 (0xFD),
    // A_H' = 255-0 = 255 (0xFF)
    // Pixel 1: Gray unchanged (0x0003, 0x0000); Alpha -> 0xFFFB, 0xFFFF? A_L' = 255-4 = 251 (0xFB), A_H' = 255
    unsigned char expected[8] = {
        0x01,0x00, 0xFD,0xFF,  // Pixel 0
        0x03,0x00, 0xFB,0xFF   // Pixel 1
    };

    if (mem_equal(row.data(), expected, 8)) {
        test_passed("test_gray_alpha_16bit_invert");
    } else {
        test_failed("test_gray_alpha_16bit_invert", "GGAA16 inversion did not produce expected alpha bytes.");
        std::cerr << "Expected: ";
        for (int i = 0; i < 8; ++i) std::cerr << int(expected[i]) << " ";
        std::cerr << "\nActual:   ";
        for (int i = 0; i < 8; ++i) std::cerr << int(row[i]) << " ";
        std::cerr << "\n";
    }
}
#endif // PNG_WRITE_16BIT_SUPPORTED

int main() {
    // Run all tests
    test_rgb_alpha_8bit_invert();
    test_rgb_alpha_8bit_color_type_mismatch_noop();
    test_gray_alpha_8bit_invert();

#ifdef PNG_WRITE_16BIT_SUPPORTED
    test_rgb_alpha_16bit_invert();
    test_gray_alpha_16bit_invert();
#else
    std::cout << "[SKIP] 16-bit tests not compiled (PNG_WRITE_16BIT_SUPPORTED not defined).\n";
#endif

    // Summary
    std::cout << "\nTest Summary: " << g_tests_passed << " passed, "
              << g_tests_failed << " failed, "
              << g_tests_run << " total.\n";

    return g_tests_failed == 0 ? 0 : 1;
}