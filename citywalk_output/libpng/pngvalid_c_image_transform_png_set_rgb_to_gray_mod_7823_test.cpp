// Unit test suite for the focal method: image_transform_png_set_rgb_to_gray_mod
// This test is written for C++11, does not rely on Google Test, and uses a minimal
// handcrafted test harness with plain asserts and descriptive comments.
// It assumes the project provides the appropriate headers for the involved types
// (image_transform, image_pixel, transform_display, png_const_structp, etc.).
// The tests exercise key execution paths: RGB color type path, color type conversion,
// and the end-state adjustments of sBIT and color type, plus a basic check that the
// "next" transform is invoked.

#include <cstdint>
#include <cassert>
#include <pngvalid.h>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <png.h>
#include <math.h>


// Include project-specific headers that declare the involved types and the focal method.
// The exact header may vary by project configuration; adjust include paths accordingly.
// For example, if the project provides pngvalid.h or a public API header exposing
// image_transform, image_pixel, transform_display, and the rgb_to_gray function,
// include it here.
// If the above header name differs in your environment, replace accordingly.

////////////////////////////////////////////////////////////////////////////////
// Forward declarations for test-only helpers and to satisfy the compiler when
// building against the provided sources. These declarations rely on the public
// API exposed by the project headers.
extern "C" {
   // Focal method under test
   image_transform_png_set_rgb_to_gray_mod(const image_transform *this_ptr,
                                           image_pixel *that, png_const_structp pp,
                                           const transform_display *display);

   // A no-op "next" transform function to complete the transform chain
   void dummy_next_mod(image_transform *self, image_pixel *that,
                       png_const_structp pp, const transform_display *display);
}

// Global counter to verify that the next-transform hook has been invoked.
static int g_dummy_next_mod_calls = 0;

// Test helper: the dummy "next" transform mod function increments a counter to verify execution.
extern "C" void dummy_next_mod(image_transform *self, image_pixel *that,
                              png_const_structp pp, const transform_display *display)
{
    (void)self; (void)that; (void)pp; (void)display;
    ++g_dummy_next_mod_calls;
}

// Helper: reset global counters before each test.
static void reset_test_harness()
{
    g_dummy_next_mod_calls = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Test case 1: RGB color path with RGB value normalization to gray (Gamma = 1, non-DIGITIZE path)
// Intent: Ensure the function computes gray correctly when r==g==b and converts RGB/RGB_ALPHA
// to Gray/Gray_ALPHA, respectively, updates sBIT fields to the minimum channel sBIT value,
// and invokes the next transform in the chain.
static void test_rgb_to_gray_rgb_equal_values_converts_to_gray_rgb(void)
{
    reset_test_harness();

    // Prepare transform chain: root -> dummy_next_mod
    image_transform root;
    image_transform next;
    root.next = &next;
    next.next = nullptr;
    next.mod = dummy_next_mod; // The actual type signature should match; via header.

    // Initialize transform data (coefficients for grayscale) and gamma
    // Use zero coefficients to keep calculations deterministic (gray == 0 when r=g=b != 0).
    root.data.red_coefficient   = 0.0;
    root.data.green_coefficient = 0.0;
    root.data.blue_coefficient  = 0.0;
    root.data.gamma             = 1.0; // Gamma path: identity

    // Pixel to transform: RGB input with color type colors enabled
    image_pixel pix;
    pix.colour_type = PNG_COLOR_TYPE_RGB; // (PNG_COLOR_MASK_COLOR) != 0
    pix.sample_depth = 8;                  // 8-bit per channel
    pix.redf   = 0.6;  pix.rede  = 0.0;
    pix.greenf = 0.6;  pix.greene = 0.0;
    pix.bluef  = 0.6;  pix.bluee = 0.0;

    // Set sBIT fields so that the final min tracking changes occur as expected
    pix.red_sBIT   = 3;
    pix.green_sBIT = 2;
    pix.blue_sBIT  = 1;

    // No PNG info struct needed for this test
    png_const_structp pp = nullptr;

    // Display/pm mock
    png_modifier pm;
    pm.limit = 1.0e9;
    pm.assume_16_bit_calculations = 0;
    transform_display display;
    display.pm = &pm;
    display.max_gamma_8 = 255; // gamma-depth when 8-bit

    // Run focal method
    image_transform_png_set_rgb_to_gray_mod(&root, &pix, pp, &display);

    // Assertions: check that gray = 0 (since coefficients are zero) and values updated
    assert(pix.redf   == 0.0);
    assert(pix.greenf == 0.0);
    assert(pix.bluef  == 0.0);

    assert(pix.rede  == 0.0);
    assert(pix.greene == 0.0);
    assert(pix.bluee == 0.0);

    // After RGB->GRAY conversion, colour_type should be Gray
    assert(pix.colour_type == PNG_COLOR_TYPE_GRAY);

    // sBIT: minimum among RGB channels should be applied to all three
    // Original: red_sBIT=3, green_sBIT=2, blue_sBIT=1 => min = 1
    assert(pix.red_sBIT == 1);
    assert(pix.green_sBIT == 1);
    assert(pix.blue_sBIT == 1);

    // Ensure the next transform in chain was invoked exactly once
    assert(g_dummy_next_mod_calls == 1);

    // If you print for debugging (optional)
    // std::cout << "test_rgb_to_gray_rgb_equal_values_converts_to_gray_rgb passed\n";
}

// Test case 2: RGBAlpha path converts to GrayAlpha and verifies end-state
// Intent: Ensure that when initial colour_type is RGB_ALPHA, the conversion yields Gray_ALPHA
// and the next transform is invoked. Also verify sBIT propagation logic.
static void test_rgb_to_gray_alpha_conversion_to_gray_alpha(void)
{
    reset_test_harness();

    // Prepare transform chain: root -> dummy_next_mod
    image_transform root;
    image_transform next;
    root.next = &next;
    next.next = nullptr;
    next.mod = dummy_next_mod;

    // Coefficients zero, gamma = 1 for deterministic behavior
    root.data.red_coefficient   = 0.0;
    root.data.green_coefficient = 0.0;
    root.data.blue_coefficient  = 0.0;
    root.data.gamma             = 1.0;

    // Pixel: RGB_ALPHA input
    image_pixel pix;
    pix.colour_type = PNG_COLOR_TYPE_RGB_ALPHA;
    pix.sample_depth = 8;
    pix.redf   = 0.6;  pix.rede  = 0.0;
    pix.greenf = 0.6;  pix.greene = 0.0;
    pix.bluef  = 0.6;  pix.bluee = 0.0;

    // sBIT fields
    pix.red_sBIT   = 4;
    pix.green_sBIT = 2;
    pix.blue_sBIT  = 1;

    png_const_structp pp = nullptr;

    // Display/pm mock
    png_modifier pm;
    pm.limit = 1.0e9;
    pm.assume_16_bit_calculations = 0;
    transform_display display;
    display.pm = &pm;
    display.max_gamma_8 = 255;

    image_transform_png_set_rgb_to_gray_mod(&root, &pix, pp, &display);

    // After conversion: color type should be Gray_ALPHA
    assert(pix.colour_type == PNG_COLOR_TYPE_GRAY_ALPHA);

    // Gray channels should be equal and set to 0 due to zero coefficients
    assert(pix.redf   == 0.0);
    assert(pix.greenf == 0.0);
    assert(pix.bluef  == 0.0);

    assert(pix.rede  == 0.0);
    assert(pix.greene == 0.0);
    assert(pix.bluee == 0.0);

    // sBIT minor value: minimum among original should propagate
    // min(4,2,1) = 1
    assert(pix.red_sBIT   == 1);
    assert(pix.green_sBIT == pix.red_sBIT);
    assert(pix.blue_sBIT  == pix.red_sBIT);

    // Ensure the next transform in chain was invoked exactly once
    assert(g_dummy_next_mod_calls == 1);
}

////////////////////////////////////////////////////////////////////////////////
// Main test runner
int main(void)
{
    // Simple summary
    int tests_passed = 0;
    int tests_total  = 2;

    // Run tests with descriptive output
    try {
        test_rgb_to_gray_rgb_equal_values_converts_to_gray_rgb();
        ++tests_passed;
        std::cout << "[PASS] test_rgb_to_gray_rgb_equal_values_converts_to_gray_rgb\n";
    } catch (const std::exception &e) {
        std::cerr << "[FAIL] test_rgb_to_gray_rgb_equal_values_converts_to_gray_rgb: "
                  << e.what() << "\n";
    } catch (...) {
        std::cerr << "[FAIL] test_rgb_to_gray_rgb_equal_values_converts_to_gray_rgb: unknown error\n";
    }

    try {
        test_rgb_to_gray_alpha_conversion_to_gray_alpha();
        ++tests_passed;
        std::cout << "[PASS] test_rgb_to_gray_alpha_conversion_to_gray_alpha\n";
    } catch (const std::exception &e) {
        std::cerr << "[FAIL] test_rgb_to_gray_alpha_conversion_to_gray_alpha: "
                  << e.what() << "\n";
    } catch (...) {
        std::cerr << "[FAIL] test_rgb_to_gray_alpha_conversion_to_gray_alpha: unknown error\n";
    }

    if (tests_passed == tests_total) {
        std::cout << "All tests passed (" << tests_passed << "/" << tests_total << ").\n";
        return 0;
    } else {
        std::cout << "Some tests failed (" << tests_passed << "/" << tests_total << ").\n";
        return 1;
    }
}