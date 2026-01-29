// Minimal C++11 test suite for the focal function insert_sBIT in makepng.c
// This test uses libpng to exercise the behavior of insert_sBIT under
// multiple color configurations and parameter combinations.
// The tests avoid terminating the process on failures, instead they log
// results and continue to maximize coverage.
// Note: This test assumes libpng is available and linked (e.g., -lpng).

#include <stdint.h>
#include <vector>
#include <setjmp.h>
#include <stddef.h>
#include <string.h>
#include <string>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>



// Declare the C function under test
extern "C" void insert_sBIT(png_structp png_ptr, png_infop info_ptr, int nparams, png_charpp params);

// Simple test harness
static int g_total = 0;
static int g_passed = 0;

#define LOG_FAIL(msg) do { std::cerr << "FAILED: " << (msg) << std::endl; } while(0)
#define LOG_PASS(msg) do { std::cout << "PASSED: " << (msg) << std::endl; } while(0)

#define EXPECT_TRUE(cond, msg) do { ++g_total; if (cond) { ++g_passed; LOG_PASS(msg); } else { ++g_total; LOG_FAIL(msg); } } while(0)
#define EXPECT_EQ(a, b, msg) do { ++g_total; if ((a) == (b)) { ++g_passed; LOG_PASS(msg); } else { ++g_total; LOG_FAIL(msg); } } while(0)

// Helper to create a new png read context
static bool make_png_context(png_structp &png_ptr, png_infop &info_ptr) {
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        LOG_FAIL("Failed to create png read struct");
        return false;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        LOG_FAIL("Failed to create png info struct");
        return false;
    }
    // Set up a harmless error handling path using setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // If we reach here, an error occurred; we'll report from the test caller
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return false;
    }
    return true;
}

// Test 1: RGB color without alpha, 3 parameters
static bool test_sBIT_rgb_no_alpha() {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    if (!make_png_context(png_ptr, info_ptr)) {
        LOG_FAIL("test_sBIT_rgb_no_alpha: Context setup failed");
        return false;
    }

    // IHDR: width=1, height=1, bit_depth=8, color_type=PNG_COLOR_TYPE_RGB
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Prepare 3 parameter values: 4, 6, 8
    char p0[] = "4";
    char p1[] = "6";
    char p2[] = "8";
    char *params[3];
    params[0] = p0;
    params[1] = p1;
    params[2] = p2;

    insert_sBIT(png_ptr, info_ptr, 3, params);

    png_color_8 sBIT;
    png_get_sBIT(png_ptr, info_ptr, &sBIT);

    bool ok = (sBIT.red == 4) && (sBIT.green == 6) && (sBIT.blue == 8) &&
              (sBIT.gray == 42) && (sBIT.alpha == 42);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return ok;
}

// Test 2: RGB color with alpha, 4 parameters
static bool test_sBIT_rgb_with_alpha() {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    if (!make_png_context(png_ptr, info_ptr)) {
        LOG_FAIL("test_sBIT_rgb_with_alpha: Context setup failed");
        return false;
    }

    // IHDR: width=1, height=1, bit_depth=8, color_type=PNG_COLOR_TYPE_RGB Alpha
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB_ALPHA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Prepare 4 parameter values: 4,6,8,2
    char p0[] = "4";
    char p1[] = "6";
    char p2[] = "8";
    char p3[] = "2";
    char *params[4];
    params[0] = p0;
    params[1] = p1;
    params[2] = p2;
    params[3] = p3;

    insert_sBIT(png_ptr, info_ptr, 4, params);

    png_color_8 sBIT;
    png_get_sBIT(png_ptr, info_ptr, &sBIT);

    bool ok = (sBIT.red == 4) && (sBIT.green == 6) && (sBIT.blue == 8) &&
              (sBIT.gray == 42) && (sBIT.alpha == 2);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return ok;
}

// Test 3: Grayscale without alpha, 1 parameter
static bool test_sBIT_gray_no_alpha() {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    if (!make_png_context(png_ptr, info_ptr)) {
        LOG_FAIL("test_sBIT_gray_no_alpha: Context setup failed");
        return false;
    }

    // IHDR: width=1, height=1, bit_depth=8, color_type=PNG_COLOR_TYPE_GRAY
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Prepare 1 parameter: 5
    char p0[] = "5";
    char *params[1];
    params[0] = p0;

    insert_sBIT(png_ptr, info_ptr, 1, params);

    png_color_8 sBIT;
    png_get_sBIT(png_ptr, info_ptr, &sBIT);

    bool ok = (sBIT.red == 42) && (sBIT.green == 42) && (sBIT.blue == 42) &&
              (sBIT.gray == 5) && (sBIT.alpha == 42);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return ok;
}

// Test 4: Grayscale with alpha, 2 parameters
static bool test_sBIT_gray_with_alpha() {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    if (!make_png_context(png_ptr, info_ptr)) {
        LOG_FAIL("test_sBIT_gray_with_alpha: Context setup failed");
        return false;
    }

    // IHDR: width=1, height=1, bit_depth=8, color_type=PNG_COLOR_TYPE_GRAY_ALPHA
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY_ALPHA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Prepare 2 parameters: 5,7
    char p0[] = "5";
    char p1[] = "7";
    char *params[2];
    params[0] = p0;
    params[1] = p1;

    insert_sBIT(png_ptr, info_ptr, 2, params);

    png_color_8 sBIT;
    png_get_sBIT(png_ptr, info_ptr, &sBIT);

    bool ok = (sBIT.red == 42) && (sBIT.green == 42) && (sBIT.blue == 42) &&
              (sBIT.gray == 5) && (sBIT.alpha == 7);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return ok;
}

// Test 5: Wrong parameter count triggers an error (png_error path)
static bool test_sBIT_wrong_param_count() {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    if (!make_png_context(png_ptr, info_ptr)) {
        LOG_FAIL("test_sBIT_wrong_param_count: Context setup failed");
        return false;
    }

    // IHDR: RGB, 8-bit
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Provide 4 parameters while RGB expects 3
    char p0[] = "1";
    char p1[] = "2";
    char p2[] = "3";
    char p3[] = "4";
    char *params[4];
    params[0] = p0;
    params[1] = p1;
    params[2] = p2;
    params[3] = p3;

    bool error_occurred = false;
    // Use a setjmp to catch the png_error longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error path taken
        error_occurred = true;
    } else {
        insert_sBIT(png_ptr, info_ptr, 4, params);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    // Expect an error path
    return error_occurred;
}

int main() {
    std::cout << "Starting insert_sBIT unit tests...\n";

    // Run tests
    bool t1 = test_sBIT_rgb_no_alpha();
    EXPECT_TRUE(t1, "RGB without alpha: sBIT should reflect first three params; alpha=42");

    bool t2 = test_sBIT_rgb_with_alpha();
    EXPECT_TRUE(t2, "RGB with alpha: sBIT should reflect RGBA params including alpha");

    bool t3 = test_sBIT_gray_no_alpha();
    EXPECT_TRUE(t3, "Gray without alpha: sBIT should reflect gray param; color channels = 42");

    bool t4 = test_sBIT_gray_with_alpha();
    EXPECT_TRUE(t4, "Gray with alpha: sBIT should reflect gray and alpha params; RGB=42");

    bool t5 = test_sBIT_wrong_param_count();
    EXPECT_TRUE(t5, "Wrong parameter count should trigger png_error (caught by setjmp)");

    std::cout << "Tests completed. Passed " << g_passed << " of " << g_total << " tests.\n";

    // Exit code: 0 if all tests passed, else non-zero
    return (g_passed == g_total) ? 0 : 1;
}