// Minimal, self-contained C++11 unit test suite for image_transform_png_set_expand_mod
// This file provides a lightweight, dependency-mocking test harness to verify
// the behavior of the focal method image_transform_png_set_expand_mod as described.
// It avoids GoogleTest and uses simple, non-terminating assertions to maximize coverage.

#include <cstddef>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Domain-specific constants (mimicking libpng color type values)
#define PNG_COLOR_TYPE_GRAY       0
#define PNG_COLOR_TYPE_RGB        2
#define PNG_COLOR_TYPE_PALETTE    3

// Forward-declare lightweight types to simulate the real project interfaces.
struct png_struct_def;
typedef struct png_struct_def* png_const_structp;

// Minimal standard_display representation used by image_pixel_add_alpha
struct standard_display { int dummy; };

// Transform display wrapper used by the image_transform code
struct transform_display {
    standard_display this;
};

// image_pixel represents a pixel in the transform pipeline
struct image_pixel {
    unsigned colour_type;
    unsigned bit_depth;
    unsigned sample_depth;
    int have_tRNS;
};

// image_transform node in the transform chain
struct image_transform {
    image_transform *next;
    void (*mod)(image_transform *self,
                image_pixel *that,
                png_const_structp pp,
                const transform_display *display);
};

// Global flags and capture variables to verify behavior in tests
static bool convert_PLTE_called = false;
static bool add_alpha_called = false;
static const image_pixel* last_called_that = nullptr;
static png_const_structp last_called_pp = nullptr;
static const transform_display* last_called_display = nullptr;

// Captures for add_alpha parameter (display passed to image_pixel_add_alpha)
static const standard_display* captured_add_alpha_display = nullptr;

// Captures for next-mod call (to verify that the next transform in the chain is invoked)
static bool next_mod_called = false;
static const image_pixel* captured_next_that = nullptr;
static png_const_structp captured_next_pp = nullptr;
static const transform_display* captured_next_display = nullptr;

// Mocked dependency implementations
void image_pixel_convert_PLTE(image_pixel *that) {
    convert_PLTE_called = true;
}

// Note: The real signature is image_pixel_add_alpha(image_pixel*, const standard_display*, int)
void image_pixel_add_alpha(image_pixel *that, const standard_display *display, int for_background) {
    add_alpha_called = true;
    captured_add_alpha_display = display;
}

// The actual next transform mod function (captures invocation details)
static void next_mod(image_transform *self, image_pixel *that,
                     png_const_structp pp, const transform_display *display) {
    next_mod_called = true;
    captured_next_that = that;
    captured_next_pp = pp;
    captured_next_display = display;
}

// The focal function under test, adapted for C++ (renaming the 'this' parameter to avoid keyword issues)
void image_transform_png_set_expand_mod(const image_transform *self,
    image_pixel *that, png_const_structp pp,
    const transform_display *display)
{
    // The function replicates the logic:
    // - If palette: convert PLTE
    // - Else if grayscale (<8 bits): set depth to 8
    // - If tRNS present: add alpha
    // - Call next->mod(...)
    {
       if (that->colour_type == PNG_COLOR_TYPE_PALETTE)
          image_pixel_convert_PLTE(that);
       else if (that->bit_depth < 8)
          that->sample_depth = that->bit_depth = 8;
       if (that->have_tRNS)
          image_pixel_add_alpha(that, &display->this, 0/*!for background*/);
       self->next->mod((image_transform*)self->next, that, pp, display);
    }
}

// Lightweight test harness
static int test_failures = 0;
static int total_tests = 0;

#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { ++test_failures; std::cerr << "TEST_FAIL: " << msg << std::endl; } } while(0)

#define ASSERT_EQ(a, b, msg) \
    do { if((a) != (b)) { ++test_failures; std::cerr << "TEST_FAIL: " << msg << " (expected " << (b) << ", got " << (a) << ")" << std::endl; } } while(0)

static void reset_globals() {
    convert_PLTE_called = false;
    add_alpha_called = false;
    last_called_that = nullptr;
    last_called_pp = nullptr;
    last_called_display = nullptr;
    captured_add_alpha_display = nullptr;
    next_mod_called = false;
    captured_next_that = nullptr;
    captured_next_pp = nullptr;
    captured_next_display = nullptr;
}

// Prepare a display object for tests
static transform_display make_display() {
    transform_display d;
    std::memset(&d, 0, sizeof(d));
    return d;
}

// Prepare a pixel baseline
static image_pixel make_pixel(unsigned colour_type, unsigned bit_depth, int have_tRNS) {
    image_pixel p;
    p.colour_type = colour_type;
    p.bit_depth = bit_depth;
    p.sample_depth = bit_depth;
    p.have_tRNS = have_tRNS;
    return p;
}

// Test 1: Palette color with no tRNS should call PLTE conversion, not add alpha, and delegate to next
void test_expand_mod_palette_no_tRNS() {
    reset_globals();
    // Candidate Keywords: PLTE conversion, palette path, next.mod delegation

    image_pixel that = make_pixel(PNG_COLOR_TYPE_PALETTE, 8, 0);

    transform_display display = make_display();

    image_transform next_tf;
    next_tf.next = nullptr;
    next_tf.mod = next_mod;

    image_transform parent;
    parent.next = &next_tf;
    parent.mod = nullptr;

    image_transform_png_set_expand_mod(&parent, &that, reinterpret_cast<png_const_structp>(0xDEADBEEF), &display);

    EXPECT_TRUE(convert_PLTE_called, "PLTE conversion should run for palette color type");
    EXPECT_TRUE(!add_alpha_called, "No tRNS => no alpha addition");
    EXPECT_TRUE(next_mod_called, "Next transform should be invoked");
    ASSERT_EQ(last_called_that != nullptr ? 1 : 0, 1, "last_called_that should be captured when next_mod runs");
    ASSERT_EQ(last_called_pp, reinterpret_cast<png_const_structp>(0xDEADBEEF), "pp should be forwarded to next.mod");
}

// Test 2: Non-palette with grayscale bit_depth < 8 should coerce to 8-bit and delegate
void test_expand_mod_gray_to_8bits() {
    reset_globals();
    // Candidate Keywords: grayscale handling, depth normalization, next.mod delegation

    image_pixel that = make_pixel(PNG_COLOR_TYPE_RGB, 4, 0);

    transform_display display = make_display();

    image_transform next_tf;
    next_tf.next = nullptr;
    next_tf.mod = next_mod;

    image_transform parent;
    parent.next = &next_tf;
    parent.mod = nullptr;

    image_transform_png_set_expand_mod(&parent, &that, nullptr, &display);

    EXPECT_TRUE(!convert_PLTE_called, "PLTE conversion should not run for non-palette");
    EXPECT_TRUE(!add_alpha_called, "No tRNS => no alpha addition");
    EXPECT_TRUE(next_mod_called, "Next transform should be invoked");
    ASSERT_EQ(that.bit_depth, 8, "Bit depth should be promoted to 8");
    ASSERT_EQ(that.sample_depth, 8, "Sample depth should be promoted to 8");
}

// Test 3: tRNS present should trigger alpha addition
void test_expand_mod_with_tRNS_alpha() {
    reset_globals();
    // Candidate Keywords: tRNS handling, alpha channel addition

    image_pixel that = make_pixel(PNG_COLOR_TYPE_RGB, 16, 1);

    transform_display display = make_display();

    image_transform next_tf;
    next_tf.next = nullptr;
    next_tf.mod = next_mod;

    image_transform parent;
    parent.next = &next_tf;
    parent.mod = nullptr;

    image_transform_png_set_expand_mod(&parent, &that, nullptr, &display);

    EXPECT_TRUE(add_alpha_called, "tRNS should trigger alpha addition");
    ASSERT_EQ(captured_add_alpha_display, &display.this, "Alpha addition should receive display->this");
    EXPECT_TRUE(next_mod_called, "Next transform should be invoked when tRNS handling occurs");
}

// Test 4: Palette with tRNS should trigger both PLTE conversion and alpha addition
void test_expand_mod_palette_with_tRNS() {
    reset_globals();
    // Candidate Keywords: PLTE path + tRNS path together

    image_pixel that = make_pixel(PNG_COLOR_TYPE_PALETTE, 1, 1);

    transform_display display = make_display();

    image_transform next_tf;
    next_tf.next = nullptr;
    next_tf.mod = next_mod;

    image_transform parent;
    parent.next = &next_tf;
    parent.mod = nullptr;

    image_transform_png_set_expand_mod(&parent, &that, (png_const_structp)NULL, &display);

    EXPECT_TRUE(convert_PLTE_called, "PLTE conversion should occur for palette");
    EXPECT_TRUE(add_alpha_called, "tRNS should trigger alpha addition along with PLTE");
    EXPECT_TRUE(next_mod_called, "Next transform should be invoked");
}

// Test 5: Validate that the next transform receives the exact display and pp passed
void test_expand_mod_argument_passing() {
    reset_globals();

    image_pixel that = make_pixel(PNG_COLOR_TYPE_RGB, 8, 0);

    transform_display display = make_display();

    image_transform next_tf;
    next_tf.next = nullptr;
    next_tf.mod = next_mod;

    image_transform parent;
    parent.next = &next_tf;
    parent.mod = nullptr;

    png_const_structp test_pp = reinterpret_cast<png_const_structp>(0xA5A5A5);

    image_transform_png_set_expand_mod(&parent, &that, test_pp, &display);

    ASSERT_EQ(captured_next_pp, test_pp, "pp should be passed through to next.mod");
    ASSERT_EQ(captured_next_display, &display, "display should be passed through to next.mod");
}

// Entry point: run all tests and report summary
int main() {
    std::cout << "Running unit tests for image_transform_png_set_expand_mod (self-contained harness)" << std::endl;

    total_tests = 0;
    test_failures = 0;

    // Manually invoke tests to keep the output straightforward without a full test framework
    std::cout << "Test 1: Palette color with no tRNS" << std::endl;
    test_expand_mod_palette_no_tRNS();

    std::cout << "Test 2: Non-palette grayscale <8 bits" << std::endl;
    test_expand_mod_gray_to_8bits();

    std::cout << "Test 3: Palette with tRNS triggers alpha" << std::endl;
    test_expand_mod_with_tRNS_alpha();

    std::cout << "Test 4: Palette with tRNS triggers PLTE + alpha" << std::endl;
    test_expand_mod_palette_with_tRNS();

    std::cout << "Test 5: Correct forwarding of arguments to next transform" << std::endl;
    test_expand_mod_argument_passing();

    // Simple summary (non-terminating, prints counts)
    int tests_passed = 5 - test_failures;
    std::cout << "\nTest summary: " << tests_passed << " passed, " << test_failures << " failed." << std::endl;

    return test_failures == 0 ? 0 : 1;
}