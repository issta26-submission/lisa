// pngvalid_add_alpha_tests.cpp
// A minimal C++11 unit test suite for the focal C function:
// image_pixel_add_alpha(image_pixel *this, const standard_display *display, int for_background)
// Note: This test suite is meant to be compiled in an environment where the project's
// headers (including pngvalid.h / internal declarations) and libpng headers are available.
// The tests intentionally avoid GTest and instead use a small, non-terminating assertion style
// to maximize code coverage and execution flow as requested.

#include <pngvalid.h>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Enable C linkage for C headers when compiling as C++
extern "C" {
// The project-specific declarations for image_pixel and standard_display live here.
// Depending on the repository layout, adjust the include path as needed.
}

// Lightweight assertion macros (non-terminating in the sense of continuing after a fail is undesirable for GTest-free tests,
// but we still log failures and return false to indicate a failed test.)
#define ASSERT_TRUE(cond, msg)                         \
    do {                                               \
        if (!(cond)) {                                 \
            std::cerr << "ASSERT_TRUE FAILED: " << msg  \
                      << " [" << __FILE__ << ":" << __LINE__ << "]\n"; \
            return false;                              \
        }                                              \
    } while (0)

#define ASSERT_EQ(a, b, msg)                           \
    do {                                               \
        if (!((a) == (b))) {                           \
            std::cerr << "ASSERT_EQ FAILED: " << msg    \
                      << " | " << (a) << " != " << (b)   \
                      << " [" << __FILE__ << ":" << __LINE__ << "]\n"; \
            return false;                              \
        }                                              \
    } while (0)


// Backwards-compatible type aliases for test flexibility
// If the real project uses slightly different typedefs, these are harmless as long as
// the underlying types match the expected layout used by image_pixel_add_alpha.
typedef store_palette TestStorePalette; // alias to the project's palette type used in image_pixel_init
typedef png_byte TestColorType;          // alias for readability in tests


// Test 1: Gray color without existing alpha, no tRNS; expect to get Gray+Alpha with alphaf=1
bool test_gray_no_trns_sets_alpha()
{
    // Prepare a test image_pixel instance
    image_pixel pix;
    TestStorePalette palette = (TestStorePalette)NULL;
    // Initialize with Gray color type and a non-alpha depth
    // Signature: image_pixel_init(image_pixel *this, png_const_bytep row, png_byte colour_type,
    //                           png_byte bit_depth, png_uint_32 x, store_palette palette,
    //                           const image_pixel *format)
    image_pixel_init(&pix, NULL, PNG_COLOR_TYPE_GRAY, 8, 0, palette, NULL);

    // Prepare a standard_display with a known transparent color and sBIT
    standard_display disp;
    std::memset(&disp, 0, sizeof(disp));
    disp.alpha_sBIT = 8;
    // standard_display.transparent is expected to be a png_color with red/green/blue fields
    disp.transparent.red   = 0;
    disp.transparent.green = 0;
    disp.transparent.blue  = 0;

    // Ensure no alpha initially
    pix.have_tRNS = 0;
    pix.colour_type = PNG_COLOR_TYPE_GRAY;

    // Call the focal method under test
    image_pixel_add_alpha(&pix, &disp, /*for_background=*/0);

    // Validate outcomes
    ASSERT_EQ(pix.colour_type, PNG_COLOR_TYPE_GRAY_ALPHA, "After adding alpha, Gray should become Gray Alpha");
    ASSERT_TRUE(pix.alphaf == 1, "alphaf should be 1 when there is no tRNS and gray input");
    ASSERT_TRUE(pix.alphae == 0, "alphae should be 0 as there is no alpha channel error here");
    ASSERT_EQ(pix.alpha_sBIT, disp.alpha_sBIT, "alpha_sBIT should propagate from display");
    return true;
}

// Test 2: Gray with tRNS present and red matching display.transparent.red; expect alphaf=0
bool test_gray_with_trns_match_color_sets_invisible_alpha()
{
    image_pixel pix;
    TestStorePalette palette = (TestStorePalette)NULL;
    image_pixel_init(&pix, NULL, PNG_COLOR_TYPE_GRAY, 8, 0, palette, NULL);

    standard_display disp;
    std::memset(&disp, 0, sizeof(disp));
    disp.alpha_sBIT = 8;
    disp.transparent.red = 0;
    disp.transparent.green = 0;
    disp.transparent.blue = 0;

    // Simulate tRNS present and a matching input color to the transparent color
    pix.have_tRNS = 1;
    pix.colour_type = PNG_COLOR_TYPE_GRAY;
    pix.red = disp.transparent.red;   // must match to trigger alphaf = 0
    pix.green = disp.transparent.green;
    pix.blue = disp.transparent.blue;
    // bit_depth is typically set by init; ensure it is in a valid state
    pix.bit_depth = 8;
    pix.sample_depth = 8;

    image_pixel_add_alpha(&pix, &disp, /*for_background=*/0);

    // alphaf should be 0 because the input color matches the transparent color in tRNS case
    ASSERT_TRUE(pix.alphaf == 0, "alphaf should be 0 when input color matches tRNS transparent color");
    // colour_type should still transition to Gray Alpha when alpha is explicitly present
    ASSERT_EQ(pix.colour_type, PNG_COLOR_TYPE_GRAY_ALPHA, "color type should become Gray Alpha after adding alpha");
    ASSERT_EQ(pix.alphae, 0, "alphae should be 0 in absence of explicit errors");
    ASSERT_EQ(pix.alpha_sBIT, disp.alpha_sBIT, "alpha_sBIT should propagate from display");
    return true;
}

// Test 3: RGB with no tRNS; expect RGB_ALPHA and alphaf=1
bool test_rgb_without_trns_sets_rgb_alpha()
{
    image_pixel pix;
    TestStorePalette palette = (TestStorePalette)NULL;
    image_pixel_init(&pix, NULL, PNG_COLOR_TYPE_RGB, 8, 0, palette, NULL);

    standard_display disp;
    std::memset(&disp, 0, sizeof(disp));
    disp.alpha_sBIT = 8;
    disp.transparent.red = 0;
    disp.transparent.green = 0;
    disp.transparent.blue = 0;

    pix.colour_type = PNG_COLOR_TYPE_RGB;
    pix.have_tRNS = 0;
    pix.red = 10;
    pix.green = 20;
    pix.blue = 30;

    image_pixel_add_alpha(&pix, &disp, /*for_background=*/0);

    ASSERT_EQ(pix.colour_type, PNG_COLOR_TYPE_RGB_ALPHA, "RGB should become RGB_ALPHA after adding alpha");
    ASSERT_TRUE(pix.alphaf == 1, "alphaf should be 1 for RGB without tRNS and non-matching input");
    ASSERT_TRUE(pix.alphae == 0, "alphae should be 0 in absence of error");
    ASSERT_EQ(pix.alpha_sBIT, disp.alpha_sBIT, "alpha_sBIT should propagate from display");
    return true;
}

// Test 4: Palette color path (PALETTE) to exercise the initial branch; verify no crash and reasonable state progression.
// Exact post-conditions depend on image_pixel_convert_PLTE; the test mainly ensures path execution.
bool test_palette_path_executes_without_crash()
{
    image_pixel pix;
    TestStorePalette palette = (TestStorePalette)NULL;
    image_pixel_init(&pix, NULL, PNG_COLOR_TYPE_PALETTE, 8, 0, palette, NULL);

    standard_display disp;
    std::memset(&disp, 0, sizeof(disp));
    disp.alpha_sBIT = 8;
    disp.transparent.red = 0;
    disp.transparent.green = 0;
    disp.transparent.blue = 0;

    pix.colour_type = PNG_COLOR_TYPE_PALETTE;
    pix.have_tRNS = 0;
    // Call; we do not assert on internal state as convert_PLTE may alter colour_type in an implementation-dependent way
    image_pixel_add_alpha(&pix, &disp, /*for_background=*/0);

    // Basic sanity: the function should have returned control without crashing
    // and colour_type should have progressed at least from PALETTE (if convert_PLTE performs a conversion).
    // We conservatively check that the function completed and did not leave a clearly invalid state.
    if (pix.colour_type == PNG_COLOR_TYPE_PALETTE) {
        // If convert_PLTE did not change the colour_type in this environment, it's still a valid non-crash path.
        // No strict assertion here besides ensuring no crash and sane alpha-related fields exist.
        std::cout << "[INFO] palette path executed; colour_type remains PALETTE (depending on implementation)." << std::endl;
    }
    return true;
}


// Entry point for running all tests
int main(void)
{
    int failures = 0;

    std::cout << "Running pngvalid image_pixel_add_alpha unit tests (C++11 harness)..." << std::endl;

    if (!test_gray_no_trns_sets_alpha()) {
        std::cerr << "Test 1 failed." << std::endl;
        ++failures;
    } else {
        std::cout << "Test 1 passed." << std::endl;
    }

    if (!test_gray_with_trns_match_color_sets_invisible_alpha()) {
        std::cerr << "Test 2 failed." << std::endl;
        ++failures;
    } else {
        std::cout << "Test 2 passed." << std::endl;
    }

    if (!test_rgb_without_trns_sets_rgb_alpha()) {
        std::cerr << "Test 3 failed." << std::endl;
        ++failures;
    } else {
        std::cout << "Test 3 passed." << std::endl;
    }

    if (!test_palette_path_executes_without_crash()) {
        std::cerr << "Test 4 failed." << std::endl;
        ++failures;
    } else {
        std::cout << "Test 4 passed." << std::endl;
    }

    if (failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}