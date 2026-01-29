/*
Unit tests for the focal method sBIT_error_fn located in pngvalid.c

Notes:
- The tests are implemented in C++11 (no GTest) and link against libpng.
- We exercise both branches of the color type check:
  1) Palette color type -> bit_depth is forced to 8, so sBIT components should be set to 9.
  2) Non-palette color type (GRAY in this test) -> bit_depth is taken from IHDR; sBIT components should be bit_depth+1.
- We use libpng's write interface to create a minimal PNG IHDR state, invoke sBIT_error_fn(pp, pi),
  and then verify the produced sBIT values via png_get_sBIT.
- We keep the test harness self-contained and run tests from main without any external test framework.

Compile with something like:
g++ -std=c++11 -I/path/to/libpng/include -L/path/to/libpng/lib -lpng -lm test_sBIT_error_fn.cpp -o test_sBIT_error_fn

This file prints test results to stdout and exits with a non-zero status on failure.
*/

#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <png.h>
#include <math.h>



// Ensure C linkage for the focal function when calling from C++
extern "C" void sBIT_error_fn(png_structp pp, png_infop pi);

// Simple assertion helpers (non-terminating): report failures but continue
#define EXPECT_EQ(expected, actual)                                         \
    do {                                                                      \
        if ((expected) != (actual)) {                                       \
            std::cerr << "EXPECT_EQ failed: " << #expected << " ("
                      << (expected) << ") != " << #actual << " ("
                      << (actual) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
            return false;                                                   \
        }                                                                     \
    } while (0)

#define TEST_PASS(message)                                                    \
    do {                                                                      \
        std::cout << "[PASS] " << message << "\n";                          \
    } while (0)

#define TEST_FAIL(message)                                                    \
    do {                                                                      \
        std::cerr << "[FAIL] " << message << "\n";                          \
    } while (0)

// Helper to create a minimal PNG write struct and info struct
static bool init_png_write(png_structp &pp, png_infop &pi) {
    pp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pp) {
        std::cerr << "Failed to create png write struct\n";
        return false;
    }
    pi = png_create_info_struct(pp);
    if (!pi) {
        std::cerr << "Failed to create png info struct\n";
        png_destroy_write_struct(&pp, NULL);
        return false;
    }
    // No custom error handlers; rely on default libpng error handling
    if (setjmp(png_jmpbuf(pp))) {
        std::cerr << "libpng jump encountered during initialization\n";
        png_destroy_write_struct(&pp, &pi);
        return false;
    }
    return true;
}

// Test 1: Palette color type path
// Expectation: sBIT entries set to bit_depth+1 with bit_depth forced to 8 for PALETTE
// Verify all five channels red/green/blue/gray/alpha equal to 9.
static bool test_sBIT_error_fn_palette_path() {
    // Print test header
    std::cout << "Running test_sBIT_error_fn_palette_path...\n";

    png_structp pp = nullptr;
    png_infop pi = nullptr;

    if (!init_png_write(pp, pi)) {
        return false;
    }

    // Create a basic IHDR: width=1, height=1, bit_depth=8 (valid for PALETTE), color type PALETTE
    // Note: For PALETTE, libpng allows bit_depth values in {1,2,4,8}
    png_set_IHDR(pp, pi, 1, 1, 8, PNG_COLOR_TYPE_PALETTE,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    // Call the focal function
    sBIT_error_fn(pp, pi);

    // Retrieve sBIT
    png_color_8 sbit = {0,0,0,0,0};
    png_get_sBIT(pp, pi, &sbit);

    // Expected: all channels set to 9 (bit_depth+1 where bit_depth was forced to 8)
    bool pass = true;
    if (sbit.red   != 9) pass = false;
    if (sbit.green != 9) pass = false;
    if (sbit.blue  != 9) pass = false;
    if (sbit.gray  != 9) pass = false;
    if (sbit.alpha != 9) pass = false;

    png_destroy_write_struct(&pp, &pi);

    if (pass) {
        TEST_PASS("test_sBIT_error_fn_palette_path: all channels set to 9 as expected.");
    } else {
        TEST_FAIL("test_sBIT_error_fn_palette_path: sBIT channels did not all equal 9 as expected.");
    }
    return pass;
}

// Test 2: Non-palette color type path (GRAY)
// Expectation: sBIT entries set to bit_depth+1 using actual bit_depth provided via IHDR.
// We'll use GRAY with bit_depth=4 (valid for GRAY), so expect value 5 for the relevant channel(s).
// We will verify the gray channel equals 5. Other channels may be unused for GRAY; we will not assert them.
static bool test_sBIT_error_fn_gray_path() {
    // Print test header
    std::cout << "Running test_sBIT_error_fn_gray_path...\n";

    png_structp pp = nullptr;
    png_infop pi = nullptr;

    if (!init_png_write(pp, pi)) {
        return false;
    }

    // Create IHDR: width=1, height=1, bit_depth=4, color type GRAY
    png_set_IHDR(pp, pi, 1, 1, 4, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    // Call the focal function
    sBIT_error_fn(pp, pi);

    // Retrieve sBIT
    png_color_8 sbit = {0,0,0,0,0};
    png_get_sBIT(pp, pi, &sbit);

    // Expected: gray equals bit_depth+1 = 5
    bool pass = true;
    if (sbit.gray != 5) pass = false;

    // We avoid asserting on red/green/blue/alpha since GRAY may not populate those channels meaningfully.

    png_destroy_write_struct(&pp, &pi);

    if (pass) {
        TEST_PASS("test_sBIT_error_fn_gray_path: gray channel equals 5 as expected (bit_depth=4).");
    } else {
        TEST_FAIL("test_sBIT_error_fn_gray_path: gray channel did not equal 5 as expected.");
    }
    return pass;
}

int main(void) {
    int total_tests = 0;
    int passed = 0;

    bool r1 = test_sBIT_error_fn_palette_path();
    total_tests++;
    if (r1) ++passed;

    bool r2 = test_sBIT_error_fn_gray_path();
    total_tests++;
    if (r2) ++passed;

    std::cout << "\nTest summary: " << passed << " / " << total_tests << " tests passed.\n";

    // Return non-zero if any test failed
    return (passed == total_tests) ? 0 : 1;
}