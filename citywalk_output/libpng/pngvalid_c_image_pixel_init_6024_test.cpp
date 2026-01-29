// Test harness for the focal method: image_pixel_init
// This test suite is designed to exercise various branches of image_pixel_init
// from the provided pngvalid.c, using a minimal C++11 test harness (no GTest).

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Lightweight type aliases to mirror the types used by image_pixel_init
typedef unsigned char png_byte;
typedef const png_byte* png_const_bytep;
typedef unsigned int png_uint_32;

// Forward declaration of the image_pixel struct as used by the focal method.
// We mirror the layout by declaring the fields that image_pixel_init touches.
typedef struct image_pixel {
    unsigned int palette_index;
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;
    unsigned int red_sBIT;
    unsigned int green_sBIT;
    unsigned int blue_sBIT;
    unsigned int alpha_sBIT;
    unsigned int colour_type;
    unsigned int bit_depth;
    unsigned int sample_depth;
    unsigned int have_tRNS;
    unsigned int swap_rgb;
    unsigned int alpha_first;
    unsigned int alpha_inverted;
    unsigned int mono_inverted;
    unsigned int swap16;
    unsigned int littleendian;
    unsigned int sig_bits;
} image_pixel;

// Palette entry type as used by store_palette
typedef struct store_palette_entry {
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;
} store_palette_entry;

typedef store_palette_entry* store_palette;

// External C functions used by image_pixel_init (as provided in pngvalid.c)
extern "C" {
    // Reads a single sample (byte) from the input row according to the PNG color type.
    png_byte sample(png_const_bytep row, unsigned int colour_type,
                    unsigned int bit_depth, unsigned int x,
                    unsigned int sample_index, int swap16, int littleendian);

    // Sets the scaled/floating-point max values on the image_pixel instance.
    void image_pixel_setf(image_pixel *this, unsigned int rMax,
                          unsigned int gMax, unsigned int bMax,
                          unsigned int aMax);

    // The focal method under test
    void image_pixel_init(image_pixel *this, png_const_bytep row,
                          unsigned int colour_type, unsigned int bit_depth,
                          unsigned int x, store_palette palette,
                          const image_pixel *format);
}

// Simple, non-terminating test assertion utilities
static int tests_run = 0;
static int tests_failed = 0;

template <typename T>
void expect_eq(const char* test_name, const T& a, const T& b) {
    ++tests_run;
    if (a != b) {
        ++tests_failed;
        std::cerr << "[FAILED] " << test_name
                  << "  - expected: " << +b << ", got: " << +a << "\n";
    } else {
        std::cout << "[OK] " << test_name << "\n";
    }
}

// Helper to report final results
void report_results() {
    std::cout << "\nTests run: " << tests_run
              << ", Failures: " << tests_failed << "\n";
}

// Test 1: Palette color type with palette provided should override RGB values
// Expected: red/green/blue/alpha come from palette[palette_index]
void test_palette_override() {
    const unsigned char row_bytes[1] = { 2 }; // palette index = 2
    png_const_bytep row = row_bytes;

    image_pixel pix = {}; // zero-initialized
    store_palette_entry pal[3];
    pal[2].red = 10; pal[2].green = 20; pal[2].blue = 30; pal[2].alpha = 40;

    image_pixel_init(&pix, row, 3, 8, 0, pal, nullptr);

    expect_eq("palette_override.palette_index", pix.palette_index, 2u);
    expect_eq("palette_override.red", pix.red, 10u);
    expect_eq("palette_override.green", pix.green, 20u);
    expect_eq("palette_override.blue", pix.blue, 30u);
    expect_eq("palette_override.alpha", pix.alpha, 40u);
}

// Test 2: Palette color type with no palette provided should keep initial sample value
// Expected: palette_index and RGB equal to sample(row, ...)[0], alpha remains max (255)
void test_palette_none_overrides_none() {
    const unsigned char row_bytes[1] = { 5 }; // sample index 0
    png_const_bytep row = row_bytes;

    image_pixel pix = {}; // zero-initialized

    // No palette provided (palette == 0) and no format.
    image_pixel_init(&pix, row, 3, 8, 0, nullptr, nullptr);

    expect_eq("palette_none.palette_index", pix.palette_index, 5u);
    expect_eq("palette_none.red", pix.red, 5u);
    expect_eq("palette_none.green", pix.green, 5u);
    expect_eq("palette_none.blue", pix.blue, 5u);
    // Alpha defaults to opaque when not palette and no alpha channel
    expect_eq("palette_none.alpha", pix.alpha, 255u);
}

// Test 3: Truecolor (RGB) with no alpha should populate red/green/blue from row[0..2]
void test_rgb_no_alpha() {
    unsigned char r0 = 11, g0 = 22, b0 = 33;
    const unsigned char row_bytes[3] = { r0, g0, b0 };
    png_const_bytep row = row_bytes;

    image_pixel pix = {};

    image_pixel_init(&pix, row, 2, 8, 0, nullptr, nullptr);

    expect_eq("rgb_no_alpha.palette_index", pix.palette_index, 11u);
    expect_eq("rgb_no_alpha.red", pix.red, 11u);
    expect_eq("rgb_no_alpha.green", pix.green, 22u);
    expect_eq("rgb_no_alpha.blue", pix.blue, 33u);
    expect_eq("rgb_no_alpha.alpha", pix.alpha, 255u);

    expect_eq("rgb_no_alpha.red_sBIT", pix.red_sBIT, 8u);
    expect_eq("rgb_no_alpha.green_sBIT", pix.green_sBIT, 8u);
    expect_eq("rgb_no_alpha.blue_sBIT", pix.blue_sBIT, 8u);
    expect_eq("rgb_no_alpha.alpha_sBIT", pix.alpha_sBIT, 8u);
}

// Test 4: Grayscale with mono_inverted set in format
// Expected: red/green/blue inverted by XOR with max (255)
void test_gray_mono_inverted() {
    const unsigned char row_bytes[1] = { 100 };
    png_const_bytep row = row_bytes;

    image_pixel pix = {};
    image_pixel fmt = {};
    fmt.mono_inverted = 1; // request inversion for grayscale path

    image_pixel_init(&pix, row, 0, 8, 0, nullptr, &fmt);

    // Initial sample: 100; inverted: 100 ^ 255 = 155
    expect_eq("gray_mono_inverted.red", pix.red, 155u);
    expect_eq("gray_mono_inverted.green", pix.green, 155u);
    expect_eq("gray_mono_inverted.blue", pix.blue, 155u);
    // palette_index should be the initial red before inversion
    expect_eq("gray_mono_inverted.palette_index", pix.palette_index, 100u);
}

// Test 5: Alpha-first path for RGB with palette-independent data
// colour_type = 6 (RGB_ALPHA), format.alpha_first = true
// row layout: [A0, A1, G, B] with initial A0 used for alpha
void test_alpha_first_path_rgb_alpha() {
    const unsigned char row_bytes[4] = { 10, 20, 30, 40 };
    png_const_bytep row = row_bytes;

    image_pixel pix = {};
    image_pixel fmt = {};
    fmt.alpha_first = 1;
    // Other format flags left at zero (swap16/littleendian/etc not used here)

    image_pixel_init(&pix, row, 6, 8, 0, nullptr, &fmt);

    // Expected per analysis:
    // alpha = A0 (row[0]) = 10
    // initial RGB values updated to A1 (row[1]) = 20
    // green = row[2] = 30
    // blue  = row[3] = 40
    // After the RGB parsing with alpha_first, red should be A1 (20) updated later? 
    // Based on function flow:
    // red = row[3] (40) after swap_rgb is false? Here swap_rgb is 0 -> blue updated; red later set to row[3]? The exact flow yields:
    // red = row[3] = 40, green = row[2] = 30, blue = row[1] = 20, alpha = 10
    // palette_index should equal red (40)
    expect_eq("alpha_first_rgb_alpha.red", pix.red, 40u);
    expect_eq("alpha_first_rgb_alpha.green", pix.green, 30u);
    expect_eq("alpha_first_rgb_alpha.blue", pix.blue, 20u);
    expect_eq("alpha_first_rgb_alpha.alpha", pix.alpha, 10u);
    expect_eq("alpha_first_rgb_alpha.palette_index", pix.palette_index, 40u);
}

// Test 6: Alpha-first path with swap_rgb = true
// colour_type = 6, format.swap_rgb = 1
void test_alpha_first_swap_rgb_true() {
    const unsigned char row_bytes[4] = { 5, 6, 7, 8 };
    png_const_bytep row = row_bytes;

    image_pixel pix = {};
    image_pixel fmt = {};
    fmt.alpha_first = 1;
    fmt.swap_rgb = 1;

    image_pixel_init(&pix, row, 6, 8, 0, nullptr, &fmt);

    // Expected considering swap_rgb: red from row[3], green from row[2], blue from row[1], alpha from row[0]
    expect_eq("alpha_first_swap.red", pix.red, 8u);
    expect_eq("alpha_first_swap.green", pix.green, 7u);
    expect_eq("alpha_first_swap.blue", pix.blue, 6u);
    expect_eq("alpha_first_swap.alpha", pix.alpha, 5u);
    expect_eq("alpha_first_swap.palette_index", pix.palette_index, 8u);
}

// Entry point to run all tests
int main() {
    test_palette_override();
    test_palette_none_overrides_none();
    test_rgb_no_alpha();
    test_gray_mono_inverted();
    test_alpha_first_path_rgb_alpha();
    test_alpha_first_swap_rgb_true();

    report_results();
    // Return non-zero if any test failed
    return (tests_failed != 0) ? 1 : 0;
}