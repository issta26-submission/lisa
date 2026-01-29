/*
Unit test suite for the focal C function:
   png_do_write_intrapixel(png_row_infop row_info, png_bytep row)

Goals:
- Exercise true/false branches of the function's predicates.
- Cover 8-bit RGB and RGB_ALPHA paths.
- Cover 8-bit "no color" and "invalid color" short-circuits.
- Optionally cover 16-bit paths when PNG_WRITE_16BIT_SUPPORTED is defined.
- Use only C++11 standard library and libpng (no GTest).
- Use a lightweight, non-terminating assertion framework implemented in this file.
- Access libpng public API (types and macros) via <png.h>.

Notes:
- This test relies on libpng being available in the environment (headers and linker).
- The tests invoke the focal function directly; care is taken to build minimal, deterministic row buffers that verify expected pixel transformations.
- Static analysis or advanced mocking is avoided; tests are real calls to the libpng-dependent function.
*/

#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


// Declaration of the focal function (C linkage)
extern "C" void png_do_write_intrapixel(png_row_infop row_info, png_bytep row);

// Lightweight non-terminating assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define NTEST(name) bool name(); \
    static bool name(); \
    static bool _RUN_##name = (/* register test at load time (no-op) */, true)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " (condition " #cond " evaluated to false)\n"; \
        ++g_failed_tests; \
    } \
    else { \
        /* Non-terminating: continue execution */ \
        /* Optional success message commented out to reduce noise. */ \
        // std::cout << "[PASS] " << (msg) << "\n"; \
    } \
    ++g_total_tests; \
} while(false)

#define ASSERT_BYTES_EQ(expected, actual, len, msg) do { \
    if (std::memcmp((expected), (actual), (len)) != 0) { \
        std::cerr << "[FAIL] " << (msg) << " - memory mismatch on " << (len) << " bytes\n"; \
        std::cerr << "  Expected:"; for (size_t i = 0; i < (len); ++i) std::cerr << ' ' << (int)(unsigned char)(expected[i]); \
        std::cerr << "\n  Actual  :"; for (size_t i = 0; i < (len); ++i) std::cerr << ' ' << (int)(unsigned char)(actual[i]); \
        std::cerr << "\n"; \
        ++g_failed_tests; \
    } else { \
        /* Non-terminating success path */ \
        /* std::cout << "[PASS] " << (msg) << "\n"; */ \
    } \
    ++g_total_tests; \
} while(false)

static bool test_intrapixel_8bit_rgb() {
    // Setup: 8-bit depth, RGB, 2 pixels
    png_row_info row_struct;
    row_struct.width = 2;
    row_struct.color_type = PNG_COLOR_TYPE_RGB; // 3 bytes/pixel
    row_struct.bit_depth = 8;

    // Initial row: Pixel0 = (100,20,50); Pixel1 = (60,40,10)
    unsigned char row[6] = {100, 20, 50,  60, 40, 10};

    // Call focal method
    png_do_write_intrapixel((png_row_infop)&row_struct, row);

    // Expected: Pixel0 -> (100-20, 20, 50-20) = (80, 20, 30)
    //           Pixel1 -> (60-40, 40, 10-40 mod 256) = (20, 40, 226)
    unsigned char expected[6] = {80, 20, 30,  20, 40, 226};

    ASSERT_BYTES_EQ(expected, row, sizeof(row), "8-bit RGB intrapixel transform matches expected");
    return true;
}

static bool test_intrapixel_8bit_rgb_alpha() {
    // Setup: 8-bit depth, RGB_ALPHA, 2 pixels
    png_row_info row_struct;
    row_struct.width = 2;
    row_struct.color_type = PNG_COLOR_TYPE_RGB_ALPHA; // 4 bytes/pixel
    row_struct.bit_depth = 8;

    // Initial row: Pixel0 = (100,20,30,255); Pixel1 = (60,40,10,255)
    unsigned char row[8] = {100, 20, 30, 255,  60, 40, 10, 255};

    // Call focal method
    png_do_write_intrapixel((png_row_infop)&row_struct, row);

    // Expected:
    // Pixel0: (100-20, 20, 30-20, 255) -> (80, 20, 10, 255)
    // Pixel1: (60-40, 40, 10-40, 255) -> (20, 40, 246, 255)
    unsigned char expected[8] = {80, 20, 10, 255,  20, 40, 246, 255};

    ASSERT_BYTES_EQ(expected, row, sizeof(row), "8-bit RGB_ALPHA intrapixel transform matches expected");
    return true;
}

static bool test_intrapixel_no_color_type() {
    // Setup: 8-bit depth, Gray (non-color), 3 pixels
    png_row_info row_struct;
    row_struct.width = 3;
    row_struct.color_type = PNG_COLOR_TYPE_GRAY; // non-color
    row_struct.bit_depth = 8;

    // Initial row: simple arbitrary data
    unsigned char row[3] = {7, 13, 19};

    // Call focal method
    png_do_write_intrapixel((png_row_infop)&row_struct, row);

    // Expected: unchanged
    unsigned char expected[3] = {7, 13, 19};
    ASSERT_BYTES_EQ(expected, row, sizeof(row), "8-bit Gray (non-color) should not modify row");
    return true;
}

static bool test_intrapixel_8bit_invalid_color() {
    // Setup: 8-bit depth, invalid color type (palette)
    png_row_info row_struct;
    row_struct.width = 2;
    row_struct.color_type = PNG_COLOR_TYPE_PALETTE; // not RGB or RGB_ALPHA
    row_struct.bit_depth = 8;

    // Initial row: 2 pixels worth of data (palette case, should not modify)
    unsigned char row[6] = {10, 11, 12, 13, 14, 15};

    // Call focal method
    png_do_write_intrapixel((png_row_infop)&row_struct, row);

    // Expected: unchanged
    unsigned char expected[6] = {10, 11, 12, 13, 14, 15};
    ASSERT_BYTES_EQ(expected, row, sizeof(row), "8-bit invalid color_type should not modify row");
    return true;
}

#ifdef PNG_WRITE_16BIT_SUPPORTED
static bool test_intrapixel_16bit_rgb() {
    // Setup: 16-bit depth, RGB, 1 pixel
    png_row_info row_struct;
    row_struct.width = 1;
    row_struct.color_type = PNG_COLOR_TYPE_RGB; // 6 bytes/pixel when 16-bit
    row_struct.bit_depth = 16;

    // Initial row: s0 = 0x2000, s1 = 0x1000, s2 = 0x3000
    // rp[0..1] = s0, rp[2..3] = s1, rp[4..5] = s2
    unsigned char row[6] = {0x20, 0x00, 0x10, 0x00, 0x30, 0x00};

    // Call focal method
    png_do_write_intrapixel((png_row_infop)&row_struct, row);

    // Expected:
    // red = (s0 - s1) & 0xffff = (0x2000 - 0x1000) = 0x1000
    // blue = (s2 - s1) & 0xffff = (0x3000 - 0x1000) = 0x2000
    // rp[0..1] = red >> 8, red & 0xff
    // rp[4..5] = blue >> 8, blue & 0xff
    unsigned char expected[6] = {0x10, 0x00, 0x10, 0x00, 0x20, 0x00};
    ASSERT_BYTES_EQ(expected, row, sizeof(row), "16-bit RGB intrapixel transform matches expected");
    return true;
}

static bool test_intrapixel_16bit_invalid_color() {
    // Setup: 16-bit depth, invalid color type for 16-bit path
    png_row_info row_struct;
    row_struct.width = 1;
    row_struct.color_type = PNG_COLOR_TYPE_PALETTE; // invalid for 16-bit intrapixel path
    row_struct.bit_depth = 16;

    unsigned char row[6] = {0x01,0x02, 0x03,0x04, 0x05,0x06};
    png_do_write_intrapixel((png_row_infop)&row_struct, row);

    // Expected: unchanged
    unsigned char expected[6] = {0x01,0x02,0x03,0x04,0x05,0x06};
    ASSERT_BYTES_EQ(expected, row, sizeof(row), "16-bit invalid color_type should not modify row");
    return true;
}
#endif // PNG_WRITE_16BIT_SUPPORTED

int main() {
    std::cout << "Starting unit tests for png_do_write_intrapixel...\n";

    // Step 3: Run tests
    // Note: Each test serves to exercise different branches in the focal function.

    if (test_intrapixel_8bit_rgb()) {
        // Test passed
        std::cout << "[OK] test_intrapixel_8bit_rgb\n";
    } else {
        std::cout << "[ERR] test_intrapixel_8bit_rgb failed\n";
    }

    if (test_intrapixel_8bit_rgb_alpha()) {
        std::cout << "[OK] test_intrapixel_8bit_rgb_alpha\n";
    } else {
        std::cout << "[ERR] test_intrapixel_8bit_rgb_alpha failed\n";
    }

    if (test_intrapixel_no_color_type()) {
        std::cout << "[OK] test_intrapixel_no_color_type\n";
    } else {
        std::cout << "[ERR] test_intrapixel_no_color_type failed\n";
    }

    if (test_intrapixel_8bit_invalid_color()) {
        std::cout << "[OK] test_intrapixel_8bit_invalid_color\n";
    } else {
        std::cout << "[ERR] test_intrapixel_8bit_invalid_color failed\n";
    }

#ifdef PNG_WRITE_16BIT_SUPPORTED
    if (test_intrapixel_16bit_rgb()) {
        std::cout << "[OK] test_intrapixel_16bit_rgb\n";
    } else {
        std::cout << "[ERR] test_intrapixel_16bit_rgb failed\n";
    }

    if (test_intrapixel_16bit_invalid_color()) {
        std::cout << "[OK] test_intrapixel_16bit_invalid_color\n";
    } else {
        std::cout << "[ERR] test_intrapixel_16bit_invalid_color failed\n";
    }
#else
    std::cout << "Skipping 16-bit tests (PNG_WRITE_16BIT_SUPPORTED not defined).\n";
#endif

    std::cout << "Tests completed. Passed: "
              << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << "\n";

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}