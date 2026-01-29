// Unit test suite for transform_height in pngvalid.c
// Targeted for C++11 compilation without GTest.
// The test relies on libpng to create valid png color types and bit depths,
// then calls the focal function transform_height(pp, colour_type, bit_depth).
// The tests execute from main() and report a final summary.
//
// Notes:
// - We declare the focal function with C linkage to link against the C implementation.
// - We create and destroy a small png_struct for each test case to provide a valid pp.
// - Each test case is annotated with comments describing the intention and mapping to
//   the bit_size outcome it exercises.
//
// Recommended build (example, adapt to your build system):
// g++ -std=c++11 -I<path_to_png_headers> test_transform_height.cpp -lpng -lz -lm -o test_transform_height
//

#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <math.h>


// libpng C API headers

extern "C" {
    // Declaration of the focal function under test.
    // The actual implementation is in pngvalid.c (C source).
    int transform_height(png_const_structp pp, png_byte colour_type, png_byte bit_depth);
}

// Simple non-terminating test assertion macro.
// It records failures but does not abort the test run.
#define EXPECT_EQ_MAYBE(actual, expected, msg) do {                 \
    if ((actual) != (expected)) {                                   \
        std::cerr << "TEST FAIL: " << (msg) << "\n"                   \
                  << "  Expected: " << (expected)                   \
                  << ", Actual: " << (actual) << std::endl;       \
        ++g_test_failures;                                          \
    }                                                               \
} while (0)

static int g_test_failures = 0;

// Helper to run a single test case for transform_height.
// Creates a small png_struct, calls transform_height, then cleans up.
bool run_case(const std::string& name,
              png_byte colour_type,
              png_byte bit_depth,
              int expected)
{
    // Create a minimal PNG read struct. libpng requires a valid struct for many operations.
    png_structp pp = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!pp) {
        std::cerr << "TEST INIT FAIL: could not create png_struct for test: " << name << std::endl;
        ++g_test_failures;
        return false;
    }

    // Call the focal function with a const-correct cast.
    int result = transform_height((png_const_structp)pp, colour_type, bit_depth);

    // Cleanup
    png_destroy_read_struct(&pp, nullptr, nullptr);

    // Report result
    EXPECT_EQ_MAYBE(result, expected, name);

    // Return whether test passed
    return (result == expected);
}

int main() {
    // Summary header
    std::cout << "Starting unit tests for transform_height...\n";

    // Test matrix:
    // Each test targets a specific bit_size outcome by selecting a color type and bit depth
    // that libpng's bit_size would map to, according to the expected per-pixel bit width.

    // Case 1: Palette color type with 1 bit depth -> bit_size = 1 -> expect 1
    run_case("palette_1bit", PNG_COLOR_TYPE_PALETTE, 1, 1);

    // Case 2: Palette with 2 bits -> bit_size = 2 -> expect 1
    run_case("palette_2bit", PNG_COLOR_TYPE_PALETTE, 2, 1);

    // Case 3: Palette with 4 bits -> bit_size = 4 -> expect 1
    run_case("palette_4bit", PNG_COLOR_TYPE_PALETTE, 4, 1);

    // Case 4: Palette with 8 bits -> bit_size = 8 -> expect 2
    run_case("palette_8bit", PNG_COLOR_TYPE_PALETTE, 8, 2);

    // Case 5: Palette with 16 bits -> bit_size = 16 -> expect 512
    run_case("palette_16bit", PNG_COLOR_TYPE_PALETTE, 16, 512);

    // Case 6: Truecolor RGB with 8-bit samples -> bit_size = 24 -> expect 512
    run_case("rgb8", PNG_COLOR_TYPE_RGB, 8, 512);

    // Case 7: Truecolor RGB with 16-bit samples -> bit_size = 48 -> expect 2048
    run_case("rgb16", PNG_COLOR_TYPE_RGB, 16, 2048);

    // Case 8: Truecolor with alpha RGBA 8-bit samples -> bit_size = 32 -> expect 512
    run_case("rgba8", PNG_COLOR_TYPE_RGB_ALPHA, 8, 512);

    // Case 9: Truecolor with alpha RGBA 16-bit samples -> bit_size = 64 -> expect 2048
    run_case("rgba16", PNG_COLOR_TYPE_RGB_ALPHA, 16, 2048);

    // Case 10: Grayscale with 8-bit samples -> bit_size = 8 -> expect 2
    run_case("gray8", PNG_COLOR_TYPE_GRAY, 8, 2);

    // Case 11: Grayscale with alpha (gray + alpha) 8-bit -> bit_size = 16 -> expect 512
    run_case("grayalpha8", PNG_COLOR_TYPE_GRAY_ALPHA, 8, 512);

    // Case 12: Grayscale with 16-bit samples -> bit_size = 16 -> expect 512
    run_case("gray16", PNG_COLOR_TYPE_GRAY, 16, 512);

    // Final summary
    if (g_test_failures == 0) {
        std::cout << "All transform_height tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " transform_height test(s) FAILED." << std::endl;
        return 1;
    }
}