// Minimal, self-contained C++11 unit tests for the focal method logic.
// Note: This is a self-contained, simplified test harness that mimics the
// essential behavior of the real C function image_transform_png_set_shift_mod.
// It uses lightweight stubs to avoid external dependencies and focuses on
// exercising the conditional branches and the mod-call chaining.
//
// The tests are designed to be compiled with a standard C++11 compiler
// (no GoogleTest/GMock required) and run from main().

#include <cstdint>
#include <cassert>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>


// -- Lightweight type stubs to mirror the minimal required API -- //
typedef void png_const_structp;
typedef void png_structp;
typedef void transform_display;
struct image_transform; // forward

typedef void (*image_transform_mod_fn)(image_transform* self,
                                     struct image_pixel* that,
                                     png_const_structp pp,
                                     const transform_display* display);

struct image_transform {
    image_transform* next;
    image_transform_mod_fn mod;
};

// Pixel data structure with fields used by the focal method.
// We keep this minimal to exercise the code paths.
struct image_pixel {
    uint8_t colour_type;
    uint8_t sig_bits;
    uint8_t red_sBIT;
    uint8_t green_sBIT;
    uint8_t blue_sBIT;
    uint8_t alpha_sBIT;
};

// -- Global data that the focal method uses via "data" -- //
struct shift_values {
    unsigned red;
    unsigned green;
    unsigned blue;
    unsigned gray;
    unsigned alpha;
};

// The "data" object referenced by the focal method.
// We initialize it to known values to validate correct assignment.
static shift_values data = { 0x11, 0x22, 0x33, 0x44, 0x55 };

// Palette/color type constants (subset needed for tests)
#define PNG_COLOR_TYPE_GRAY     0
#define PNG_COLOR_TYPE_RGB      2
#define PNG_COLOR_TYPE_PALETTE  3

// Flag to verify that the next' mod function is actually invoked.
static bool leaf_called = false;

// leaf_mod: the terminal mod function in the chain.
// It simply records that it was called and leaves the image_pixel untouched.
static void leaf_mod(image_transform* self, image_pixel* that, png_const_structp pp, const transform_display* display)
{
    (void)self; (void)pp; (void)display;
    leaf_called = true;
    // No modification to 'that' to keep test assertions focused on the focal method.
}

// The focal method under test (re-implemented in C++ to be self-contained here).
// This mirrors the logic shown in the prompt.
void image_transform_png_set_shift_mod(image_transform *this_ptr,
                                     image_pixel *that,
                                     png_const_structp pp,
                                     const transform_display *display)
{
    if (that->colour_type != PNG_COLOR_TYPE_PALETTE)
    {
        that->sig_bits = 1;
        if (that->colour_type & 2) // RGB channels present
        {
            that->red_sBIT   = static_cast<uint8_t>(data.red);
            that->green_sBIT = static_cast<uint8_t>(data.green);
            that->blue_sBIT  = static_cast<uint8_t>(data.blue);
        }
        else // One grey channel
        {
            that->red_sBIT = that->green_sBIT = that->blue_sBIT = static_cast<uint8_t>(data.gray);
        }
        that->alpha_sBIT = static_cast<uint8_t>(data.alpha);
    }
    // Propagate to the next transform in the chain, if present.
    if (this_ptr && this_ptr->next && this_ptr->next->mod)
        this_ptr->next->mod(this_ptr->next, that, pp, display);
}

// -- Simple assertion helpers for deterministic test output -- //
static int tests_run = 0;
static int tests_passed = 0;

#define ASSERT_EQ_UINT8(expected, actual, test_desc) do {                    \
    tests_run++;                                                             \
    if ((unsigned)(actual) == (unsigned)(expected)) {                        \
        ++tests_passed;                                                      \
    } else {                                                                 \
        std::fprintf(stderr, "FAIL: %s | expected %u, got %u\n", test_desc, \
                       (unsigned)(expected), (unsigned)(actual));           \
    }                                                                        \
} while (0)

#define ASSERT_TRUE(cond, test_desc) do {                                     \
    tests_run++;                                                              \
    if (cond) {                                                               \
        ++tests_passed;                                                       \
    } else {                                                                  \
        std::fprintf(stderr, "FAIL: %s | condition is false\n", test_desc);  \
    }                                                                         \
} while (0)

#define ASSERT_FALSE(cond, test_desc) do {                                    \
    tests_run++;                                                              \
    if (!(cond)) {                                                            \
        ++tests_passed;                                                       \
    } else {                                                                  \
        std::fprintf(stderr, "FAIL: %s | condition is true (expected false)\n", test_desc); \
    }                                                                         \
} while (0)

// -- Test Case 1: RGB path (non-palette, RGB channels) -- //
void test_shift_mod_rgb_sets_sbits_true_branch() {
    // Arrange
    image_transform parent;
    image_transform leaf;
    parent.next = &leaf;
    parent.mod = nullptr; // Not used in this test
    leaf.next = nullptr;
    leaf.mod = leaf_mod;
    leaf_called = false;

    image_pixel that;
    that.colour_type = PNG_COLOR_TYPE_RGB; // 2 -> RGB
    that.sig_bits = 0;
    that.red_sBIT = 0;
    that.green_sBIT = 0;
    that.blue_sBIT = 0;
    that.alpha_sBIT = 0;

    // Act
    image_transform_png_set_shift_mod(&parent, &that, nullptr, nullptr);

    // Assert
    ASSERT_EQ_UINT8(1, that.sig_bits, "RGB path: sig_bits should be set to 1");
    ASSERT_EQ_UINT8(static_cast<uint8_t>(data.red), that.red_sBIT, "RGB path: red_sBIT set");
    ASSERT_EQ_UINT8(static_cast<uint8_t>(data.green), that.green_sBIT, "RGB path: green_sBIT set");
    ASSERT_EQ_UINT8(static_cast<uint8_t>(data.blue), that.blue_sBIT, "RGB path: blue_sBIT set");
    ASSERT_EQ_UINT8(static_cast<uint8_t>(data.alpha), that.alpha_sBIT, "RGB path: alpha_sBIT set");
    ASSERT_TRUE(leaf_called, "RGB path: next.mod should be called");
}

// -- Test Case 2: Gray path (non-palette, grey channel) -- //
void test_shift_mod_gray_sets_sbits_gray_branch() {
    // Arrange
    image_transform parent;
    image_transform leaf;
    parent.next = &leaf;
    parent.mod = nullptr;
    leaf.next = nullptr;
    leaf.mod = leaf_mod;
    leaf_called = false;

    image_pixel that;
    that.colour_type = PNG_COLOR_TYPE_GRAY; // 0 -> Gray
    that.sig_bits = 0;
    that.red_sBIT = 0;
    that.green_sBIT = 0;
    that.blue_sBIT = 0;
    that.alpha_sBIT = 0;

    // Act
    image_transform_png_set_shift_mod(&parent, &that, nullptr, nullptr);

    // Assert
    ASSERT_EQ_UINT8(1, that.sig_bits, "Gray path: sig_bits should be set to 1");
    ASSERT_EQ_UINT8(static_cast<uint8_t>(data.gray), that.red_sBIT, "Gray path: red_sBIT (tied to gray) set");
    ASSERT_EQ_UINT8(static_cast<uint8_t>(data.gray), that.green_sBIT, "Gray path: green_sBIT (tied to gray) set");
    ASSERT_EQ_UINT8(static_cast<uint8_t>(data.gray), that.blue_sBIT, "Gray path: blue_sBIT (tied to gray) set");
    ASSERT_EQ_UINT8(static_cast<uint8_t>(data.alpha), that.alpha_sBIT, "Gray path: alpha_sBIT set");
    ASSERT_TRUE(leaf_called, "Gray path: next.mod should be called");
}

// -- Test Case 3: Palette path (palette color type) -- //
void test_shift_mod_palette_no_sbits_change() {
    // Arrange
    image_transform parent;
    image_transform leaf;
    parent.next = &leaf;
    parent.mod = nullptr;
    leaf.next = nullptr;
    leaf.mod = leaf_mod;
    leaf_called = false;

    image_pixel that;
    that.colour_type = PNG_COLOR_TYPE_PALETTE; // 3 -> Palette
    // Initialize with non-zero values to ensure no modification happens
    that.sig_bits = 5;
    that.red_sBIT = 7;
    that.green_sBIT = 8;
    that.blue_sBIT = 9;
    that.alpha_sBIT = 10;

    // Act
    image_transform_png_set_shift_mod(&parent, &that, nullptr, nullptr);

    // Assert: palette path should not modify channel sBIT fields or sig_bits
    ASSERT_EQ_UINT8(5, that.sig_bits, "Palette path: sig_bits should remain unchanged");
    ASSERT_EQ_UINT8(7, that.red_sBIT, "Palette path: red_sBIT should remain unchanged");
    ASSERT_EQ_UINT8(8, that.green_sBIT, "Palette path: green_sBIT should remain unchanged");
    ASSERT_EQ_UINT8(9, that.blue_sBIT, "Palette path: blue_sBIT should remain unchanged");
    ASSERT_EQ_UINT8(10, that.alpha_sBIT, "Palette path: alpha_sBIT should remain unchanged");
    ASSERT_TRUE(leaf_called, "Palette path: next.mod should be called");
}

// -- Main entry: run all tests and report summary -- //
int main() {
    // Run tests
    test_shift_mod_rgb_sets_sbits_true_branch();
    test_shift_mod_gray_sets_sbits_gray_branch();
    test_shift_mod_palette_no_sbits_change();

    // Summary
    std::printf("Tests run: %d, Passed: %d\n", tests_run, tests_passed);
    if (tests_run == tests_passed) {
        std::printf("All tests passed.\n");
        return 0;
    } else {
        std::printf("Some tests failed.\n");
        return 1;
    }
}