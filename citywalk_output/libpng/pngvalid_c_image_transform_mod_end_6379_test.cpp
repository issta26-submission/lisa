// ----------------------------------------------------------------------------
// Lightweight C++11 test suite for the focal method:
// image_transform_mod_end in the provided codebase.
// - No Google Test; a small in-file test harness is implemented.
// - Tests aim to cover true/false branches of the dependent code paths.
// - Uses the real types from libpng (image_transform, image_pixel, etc.).
// - Explanatory comments accompany each unit test.
// ----------------------------------------------------------------------------

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Include libpng headers to obtain type definitions and constants.
// The project normally provides these in the build environment.

// Ensure C linkage for the focal C function when declaring the prototype.
extern "C" {
    // The function under test; signature assumed from the focal method.
    void image_transform_mod_end(const struct image_transform *this_ptr,
                                 struct image_pixel *that,
                                 png_const_structp pp,
                                 const struct transform_display *display);
}

// Helper structure aliasing (if needed) will come from libpng headers above.

// ----------------------------------------------------------------------------
// Small, non-terminating assertion helpers for test diagnostics
// ----------------------------------------------------------------------------
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_PASS(msg) do { \
    ++g_tests_run; \
    ++g_tests_passed; \
    std::cout << "[TEST PASSED] " << msg << std::endl; \
} while(0)

#define TEST_FAIL(msg) do { \
    ++g_tests_run; \
    ++g_tests_failed; \
    std::cerr << "[TEST FAILED] " << msg << std::endl; \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    if (cond) { TEST_PASS(std::string(msg)); } \
    else { TEST_FAIL(std::string(msg)); } \
} while(0)

#define EXPECT_DOUBLE_NEAR(a, b, tol, msg) do { \
    double _va = (a); \
    double _vb = (b); \
    if (std::fabs(_va - _vb) <= (tol)) { TEST_PASS(std::string(msg)); } \
    else { TEST_FAIL(std::string(msg) + " (got " + std::to_string(_va) + ", expected " + std::to_string(_vb) + ")"); } \
} while(0)


// ----------------------------------------------------------------------------
// Domain-specific tests for image_transform_mod_end
// ----------------------------------------------------------------------------

// Test 1: Grayscale (no color, no alpha) path.
// Verifies that red/green/blue are left at 0 and the "digitized" error terms
// rede/greene/bluee start from the expected 1/(2*(2^sBIT-1)) base, and
// alpha becomes opaque (255) with exact representation.
// This exercises the non-color/non-alpha branch and the final clamping logic.
static void test_grayscale_no_alpha()
{
    // Prepare input objects (zero-initialized to start from a known baseline)
    image_transform it;
    image_pixel pix;

    std::memset(&it, 0, sizeof(it));
    std::memset(&pix, 0, sizeof(pix));

    // Configure input pixel transform state
    pix.sample_depth = 8;      // 8-bit samples
    pix.sig_bits = 0;          // no extra precision loss
    pix.redf = 0.0;
    pix.red_sBIT = 7;          // 7 bits for red in sBIT
    pix.greenf = 0.0;
    pix.green_sBIT = 7;
    pix.bluef = 0.0;
    pix.blue_sBIT = 7;
    pix.colour_type = 0;        // not PNG_COLOR_MASK_COLOR, i.e., grayscale
    pix.alpha_sBIT = 7;         // for completeness
    pix.alphae = 0.0;
    pix.rede = 0.0;
    pix.greene = 0.0;
    pix.bluee = 0.0;
    pix.alphaf = 0.0;
    pix.alpha = 0;

    // Call the focal function
    image_transform_mod_end(&it, &pix, nullptr, nullptr);

    // Assertions
    EXPECT_TRUE(pix.red == 0, "Grayscale: red must remain 0");
    EXPECT_TRUE(pix.green == 0, "Grayscale: green must remain 0");
    EXPECT_TRUE(pix.blue == 0, "Grayscale: blue must remain 0");
    EXPECT_TRUE(pix.alpha == 255, "Grayscale: alpha must be opaque (255)");
    // rede, greene, bluee should have been incremented from 0 by 1/(2*(2^sBIT-1))
    double expected_delta = 1.0/(2.0*((1U<<pix.red_sBIT)-1)); // same as used for rede
    EXPECT_DOUBLE_NEAR(pix.rede, expected_delta, 1e-12,
                       "Grayscale: rede should equal expected delta");
    EXPECT_DOUBLE_NEAR(pix.greene, expected_delta, 1e-12,
                       "Grayscale: greene should equal rede delta (since set from rede)");
    EXPECT_DOUBLE_NEAR(pix.bluee, expected_delta, 1e-12,
                       "Grayscale: bluee should equal rede delta (since set from rede)");
}

// Test 2: True-color path without alpha.
// Ensures red/green/blue are initialized from their f-values (0.0 here) and
// color-specific digits contribute to greene/bluee accordingly, while alpha remains opaque.
static void test_true_color_no_alpha()
{
    image_transform it;
    image_pixel pix;

    std::memset(&it, 0, sizeof(it));
    std::memset(&pix, 0, sizeof(pix));

    pix.sample_depth = 8;
    pix.sig_bits = 0;

    pix.redf = 0.0; pix.red_sBIT = 7;
    pix.greenf = 0.0; pix.green_sBIT = 7;
    pix.bluef = 0.0; pix.blue_sBIT = 7;

    pix.colour_type = PNG_COLOR_MASK_COLOR; // true color path
    pix.alpha_sBIT = 7;
    pix.alphaf = 0.0;
    pix.red = 0; pix.green = 0; pix.blue = 0;
    pix.rede = 0.0; pix.greene = 0.0; pix.bluee = 0.0;
    pix.alphae = 0.0;

    image_transform_mod_end(&it, &pix, nullptr, nullptr);

    // Assertions for color path with no alpha
    EXPECT_TRUE(pix.red == 0, "True color: red remains 0 with zero input redf");
    EXPECT_TRUE(pix.green == 0, "True color: green remains 0 with zero input greenf");
    EXPECT_TRUE(pix.blue == 0, "True color: blue remains 0 with zero input bluef");

    double expected_delta = 1.0/(2.0*((1U<<pix.green_sBIT)-1)); // same for green/blue sBIT
    // rede is incremented by the same amount
    EXPECT_DOUBLE_NEAR(pix.rede, expected_delta, 1e-12,
                       "True color: rede equals expected delta from red sBIT");
    EXPECT_DOUBLE_NEAR(pix.greene, expected_delta, 1e-12,
                       "True color: greene increased by expected delta in color path");
    EXPECT_DOUBLE_NEAR(pix.bluee, expected_delta, 1e-12,
                       "True color: bluee increased by expected delta in color path");

    // alpha should be opaque in this test (palette/alpha not active)
    EXPECT_TRUE(pix.alpha == 255, "True color: alpha should be opaque (255) when no alpha present");
}

// Test 3: Palette with alpha path.
// Ensures palette mode triggers the palette/alpha branch, with alpha digitization
// and red/green/blue channels set to palette defaults (here zeroed).
static void test_palette_with_alpha()
{
    image_transform it;
    image_pixel pix;

    std::memset(&it, 0, sizeof(it));
    std::memset(&pix, 0, sizeof(pix));

    pix.sample_depth = 8;
    pix.sig_bits = 0;

    pix.redf = 0.0; pix.greenf = 0.0; pix.bluef = 0.0;
    pix.red_sBIT = 7; pix.green_sBIT = 7; pix.blue_sBIT = 7;

    pix.alpha_sBIT = 7; pix.alphaf = 0.0; pix.colour_type = PNG_COLOR_TYPE_PALETTE;

    pix.rede = 0.0; pix.greene = 0.0; pix.bluee = 0.0;
    pix.alphae = 0.0;

    image_transform_mod_end(&it, &pix, nullptr, nullptr);

    // In palette mode, color channels are copied from red channel due to palette path
    EXPECT_TRUE(pix.red == 0, "Palette: red should be 0 when redf is 0");
    EXPECT_TRUE(pix.green == 0, "Palette: green should be 0 when redf is 0 (copied from red)");
    EXPECT_TRUE(pix.blue == 0, "Palette: blue should be 0 when redf is 0 (copied from red)");

    // rede/greene/bluee should reflect the delta from red_sBIT (same 0.003937)
    double expected_delta = 1.0/(2.0*((1U<<pix.red_sBIT)-1));
    EXPECT_DOUBLE_NEAR(pix.rede, expected_delta, 1e-12,
                       "Palette: rede increases by expected delta from red_sBIT");
    EXPECT_DOUBLE_NEAR(pix.greene, pix.rede, 1e-12,
                       "Palette: greene set from rede (palette path)");
    EXPECT_DOUBLE_NEAR(pix.bluee, pix.rede, 1e-12,
                       "Palette: bluee set from rede (palette path)");

    // Alpha for palette mode: alphaf is in play; since alphaf=0 here, alpha should be 0
    EXPECT_TRUE(pix.alpha == 0, "Palette: alpha should be 0 when alphaf is 0");
    EXPECT_DOUBLE_NEAR(pix.alphae, expected_delta, 1e-12,
                       "Palette: alphae increments by expected delta for alpha sBIT");
}

// ----------------------------------------------------------------------------
// Main test runner
// ----------------------------------------------------------------------------
int main()
{
    // Run tests
    test_grayscale_no_alpha();
    test_true_color_no_alpha();
    test_palette_with_alpha();

    // Summary
    std::cout << "\nTest Summary: "
              << g_tests_passed << " passed, "
              << g_tests_failed << " failed, "
              << g_tests_run << " total.\n";

    // Return 0 if all tests passed; non-zero otherwise.
    return (g_tests_failed == 0) ? 0 : 1;
}