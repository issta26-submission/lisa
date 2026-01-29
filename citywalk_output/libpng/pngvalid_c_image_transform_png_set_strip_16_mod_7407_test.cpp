// Minimal C++11 test harness for the focal function:
// image_transform_png_set_strip_16_mod
// This test provides lightweight stubs for the dependent types used by the focal method
// and injects a fake "next" transform to verify that the focal method correctly:
// - lowers bit_depth and sample_depth from 16 to 8
// - clamps sBIT fields to a maximum of 8
// - invokes the next transform's mod() function
// The approach avoids GTest and uses simple, non-terminating EXPECT_*-style assertions.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Domain-specific test helpers
static int g_test_failures = 0;

// Simple non-terminating assertion macro
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) \
                  << " (false condition: " << #cond << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

// -------------------------------------------------------------
// Lightweight, test-only stubs to model the dependencies of
// image_transform_png_set_strip_16_mod from the actual C source.
//
// Note: We purposefully keep these definitions minimal and only as much
//       as is required for the focal function to operate in tests.
// The real project would use the actual definitions from the library.
// We assume the layout below is sufficient for the unit tests to exercise
// the logic under test without needing the full libpng sources.

extern "C" {

// Forward declaration of types used by the focal function.
// We provide just the pieces we need for testing.

typedef void* png_const_structp;      // opaque handle, not used in tests
typedef void* transform_display;      // opaque handle, not used in tests

// A mod-function pointer type for image_transform::next
struct image_transform; // forward declare

typedef void (*image_transform_mod_f)(
    const struct image_transform *this_ptr,
    struct /*image_pixel*/ image_pixel *that,
    png_const_structp pp,
    const transform_display *display);

// Minimal representation of image_transform used by the focal method.
// Only 'next' is required by the focal method's implementation.
struct image_transform {
    struct image_transform *next;
    image_transform_mod_f mod;
};

// Minimal representation of image_pixel used by the focal method.
// The focal method reads/writes these fields.
struct image_pixel {
    unsigned int bit_depth;
    unsigned int sample_depth;
    unsigned int red_sBIT;
    unsigned int green_sBIT;
    unsigned int blue_sBIT;
    unsigned int alpha_sBIT;
    // In the actual library there are more fields, but for tests this is enough.
};

// Prototype of the focal function under test (as it would be linked from the C source).
// We declare it with C linkage to ensure proper symbol resolution.
void image_transform_png_set_strip_16_mod(const image_transform *this_ptr,
    image_pixel *that, png_const_structp pp, const transform_display *display);

} // extern "C"

// -------------------------------------------------------------
// Test-specific helpers and fixtures

// Global state to verify that the next transform's mod() was invoked
static int g_next_mod_called = 0;
static image_pixel *g_last_that = nullptr;

// Fake "next" mod function to verify call-site behavior and allow
// verification of the arguments passed from the focal method.
static void fake_next_mod(const image_transform *self,
                          image_pixel *that,
                          png_const_structp pp,
                          const transform_display *display)
{
    (void)self;      // unused in tests
    (void)pp;        // unused in tests
    (void)display;   // unused in tests
    g_next_mod_called = 1;
    g_last_that = that;
    // Do not modify 'that' here; we want to observe modifications done by the focal method.
}

// Helper to reset global test state
static void reset_test_state() {
    g_next_mod_called = 0;
    g_last_that = nullptr;
}

// -------------------------------------------------------------
// Test 1: When bit_depth is 16, the focal method should convert to 8 and clamp sBITs.
// It should also call the next transform's mod() function.
static void test_strip_16_mod_converts_and_dispatches()
{
    reset_test_state();

    // Set up a simple transform chain: this -> next(fake)
    image_transform this_transform;
    image_transform next_transform;

    // The "this" object must contain a valid 'next' pointer to the next transform
    this_transform.next = &next_transform;
    this_transform.mod = nullptr; // not used by focal method

    // Configure the next transform to point to our fake mod
    next_transform.next = nullptr;
    next_transform.mod = fake_next_mod;

    // Prepare an image_pixel with 16-bit data and various sBIT values
    image_pixel pixel;
    pixel.bit_depth = 16;
    pixel.sample_depth = 0; // should be set to 8 by focal method
    pixel.red_sBIT   = 10; // should be clamped to 8
    pixel.green_sBIT = 7;  // <= 8, remains unchanged
    pixel.blue_sBIT  = 12; // should be clamped to 8
    pixel.alpha_sBIT = 20; // should be clamped to 8

    // Call the focal function (pp/display are opaque in tests)
    image_transform_png_set_strip_16_mod(&this_transform, &pixel, nullptr, nullptr);

    // Validate modifications to pixel
    EXPECT_TRUE(pixel.bit_depth == 8, "bit_depth should be lowered from 16 to 8");
    EXPECT_TRUE(pixel.sample_depth == 8, "sample_depth should be set to 8 when bit_depth becomes 8");
    EXPECT_TRUE(pixel.red_sBIT   == 8, "red_sBIT should be clamped to 8");
    EXPECT_TRUE(pixel.green_sBIT == 7, "green_sBIT should remain unchanged (<=8)");
    EXPECT_TRUE(pixel.blue_sBIT  == 8, "blue_sBIT should be clamped to 8");
    EXPECT_TRUE(pixel.alpha_sBIT == 8, "alpha_sBIT should be clamped to 8");

    // The next transform's mod() should have been invoked
    EXPECT_TRUE(g_next_mod_called == 1, "next->mod() should have been called");
    if (g_last_that != &pixel) {
        std::cerr << "ERROR: next->mod() received incorrect that pointer" << std::endl;
        ++g_test_failures;
    }
}

// -------------------------------------------------------------
// Test 2: When bit_depth is 16 but some sBIT values are <= 8, only values > 8 are clamped.
// This ensures no unintended changes to in-range sBITs.
static void test_strip_16_mod_partial_sBIT_clamping()
{
    reset_test_state();

    image_transform this_transform;
    image_transform next_transform;

    this_transform.next = &next_transform;
    this_transform.mod = nullptr;

    next_transform.next = nullptr;
    next_transform.mod = fake_next_mod;

    image_pixel pixel;
    pixel.bit_depth = 16;
    pixel.sample_depth = 0;
    pixel.red_sBIT   = 9;  // >8 -> clamp to 8
    pixel.green_sBIT = 8;  // exactly 8 -> unchanged
    pixel.blue_sBIT  = 8;  // exactly 8 -> unchanged
    pixel.alpha_sBIT = 8;  // exactly 8 -> unchanged

    image_transform_png_set_strip_16_mod(&this_transform, &pixel, nullptr, nullptr);

    EXPECT_TRUE(pixel.bit_depth == 8, "bit_depth should be lowered from 16 to 8");
    EXPECT_TRUE(pixel.sample_depth == 8, "sample_depth should be set to 8 when bit_depth becomes 8");
    EXPECT_TRUE(pixel.red_sBIT   == 8, "red_sBIT should be clamped to 8");
    EXPECT_TRUE(pixel.green_sBIT == 8, "green_sBIT should remain 8");
    EXPECT_TRUE(pixel.blue_sBIT  == 8, "blue_sBIT should remain 8");
    EXPECT_TRUE(pixel.alpha_sBIT == 8, "alpha_sBIT should remain 8");

    EXPECT_TRUE(g_next_mod_called == 1, "next->mod() should have been called");
    if (g_last_that != &pixel) {
        std::cerr << "ERROR: next->mod() received incorrect that pointer" << std::endl;
        ++g_test_failures;
    }
}

// -------------------------------------------------------------
// Test 3: When bit_depth is not 16, the focal method should not modify depth or sBITs,
// but should still call the next transform's mod() function.
static void test_strip_16_mod_no_depth_change_but_dispatches()
{
    reset_test_state();

    image_transform this_transform;
    image_transform next_transform;

    this_transform.next = &next_transform;
    this_transform.mod = nullptr;

    next_transform.next = nullptr;
    next_transform.mod = fake_next_mod;

    image_pixel pixel;
    pixel.bit_depth = 8;     // not 16
    pixel.sample_depth = 0;
    pixel.red_sBIT   = 15;   // would be clamped if 16, but since bit_depth != 16 we don't touch
    pixel.green_sBIT = 4;
    pixel.blue_sBIT  = 2;
    pixel.alpha_sBIT = 0;

    image_transform_png_set_strip_16_mod(&this_transform, &pixel, nullptr, nullptr);

    // No changes to bit_depth or sample_depth expected
    EXPECT_TRUE(pixel.bit_depth == 8, "bit_depth should remain 8 when input is not 16");
    EXPECT_TRUE(pixel.sample_depth == 0, "sample_depth should remain 0 when bit_depth is not 16");

    // sBITs should remain unchanged
    EXPECT_TRUE(pixel.red_sBIT   == 15, "red_sBIT should remain unchanged");
    EXPECT_TRUE(pixel.green_sBIT == 4,  "green_sBIT should remain unchanged");
    EXPECT_TRUE(pixel.blue_sBIT  == 2,  "blue_sBIT should remain unchanged");
    EXPECT_TRUE(pixel.alpha_sBIT == 0,  "alpha_sBIT should remain unchanged");

    EXPECT_TRUE(g_next_mod_called == 1, "next->mod() should have been called even when bit_depth != 16");
    if (g_last_that != &pixel) {
        std::cerr << "ERROR: next->mod() received incorrect that pointer" << std::endl;
        ++g_test_failures;
    }
}

// -------------------------------------------------------------
// Main: run tests and report summary
int main()
{
    // Run tests
    test_strip_16_mod_converts_and_dispatches();
    test_strip_16_mod_partial_sBIT_clamping();
    test_strip_16_mod_no_depth_change_but_dispatches();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}