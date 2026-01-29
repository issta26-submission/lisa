// C++11 test harness for the focal C function:
// image_transform_png_set_filler_mod
// This test suite is designed to run alongside the libpng pngvalid.c test module.
// It uses a minimal C++ harness (no GTest) and relies on the C API signature
// as declared in the provided FOCAL_METHOD.
// The tests focus on validating the behaviour of image_transform_png_set_filler_mod
// with respect to: (a) true branch execution when bit_depth >= 8 and colour_type is RGB/GRAY,
// (b) false branch when conditions are not met, and (c) that the next transform in the chain is invoked.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <iomanip>
#include <float.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>


// Forward declare minimal C structures to mirror the focal function's expected types.
// We do not rely on libpng internal headers for test compilation; we provide minimal
// compatible definitions so that the focal function can be invoked from C++.

extern "C" {

// Forward declarations of opaque libpng-like types (we only pass them through in tests).
typedef void* png_const_structp;             // opaque const PNG struct pointer
struct transform_display;                    // forward-declared (empty for test)

// Minimal image_pixel layout as used by the focal function.
// The fields mirror those accessed in image_transform_png_set_filler_mod.
typedef struct image_pixel {
    unsigned int bit_depth;     // bit depth (e.g., 8)
    unsigned int colour_type;   // PNG_COLOR_TYPE_*
    unsigned int alpha;           // alpha in filler (extracted from filler)
    double alphaf;                // alpha as fraction (0..1)
    unsigned int alphae;          // alpha end marker (not used in test)
    unsigned int alpha_first;     // whether alpha is first (flag)
} image_pixel;

// Minimal image_transform in the chain. The focal function only relies on:
typedef void (*image_transform_mod_fn)(struct image_transform *this,
                                     image_pixel *that,
                                     png_const_structp pp,
                                     const struct transform_display *display);

typedef struct image_transform {
    image_transform_mod_fn mod; // function pointer to next transform (or dummy)
    struct image_transform *next; // next transform in chain
} image_transform;

// Public signature of the focal function under test.
// It takes a const this pointer, a mutable image_pixel, a png_const_structp (opaque),
// and a transform_display pointer.
void image_transform_png_set_filler_mod(const image_transform *this,
                                      image_pixel *that,
                                      png_const_structp pp,
                                      const transform_display *display);

} // extern "C"


// We rely on the external data instance used by pngvalid's tests.
// The actual 'data' symbol is defined in pngvalid.c and contains fields:
//   filler: unsigned int
//   flags:  int
// We declare it as an anonymous struct to access fields directly.
extern "C" struct {
    unsigned int filler;
    int flags;
} data;

// The PNG_FILLER_BEFORE macro is expected from libpng headers. Provide a fallback
// if not available to keep tests self-contained.
#ifndef PNG_FILLER_BEFORE
#define PNG_FILLER_BEFORE 0
#endif

// Helper to print test results without terminating on failure.
static int tests_run = 0;
static int tests_failed = 0;
#define EXPECT(cond, desc) do {                                      \
    ++tests_run;                                                       \
    if (!(cond)) {                                                     \
        ++tests_failed;                                                \
        std::cerr << "Test failure: " << desc << " (line "              \
                  << __LINE__ << ")" << std::endl;                   \
    } else {                                                           \
        // Success path (optional: could print verbose success per test) \
    }                                                                \
} while (0)

// Stub for the 'next' transform's mod function to verify chain invocation.
static bool next_mod_called = false;
static void stub_next_mod(image_transform *this_ptr,
                          image_pixel *that,
                          png_const_structp pp,
                          const transform_display *display)
{
    (void)this_ptr; // unused in test
    (void)that;
    (void)pp;
    (void)display;
    next_mod_called = true;
}

// Test 1: True branch is taken when bit_depth >= 8 and colour_type is RGB.
static void test_filler_mod_true_branch_sets_alpha_and_color_and_calls_next()
{
    // Prepare a simple transform chain: first -> next (stub)
    image_transform next_transform = { .mod = stub_next_mod, .next = nullptr };
    image_transform first_transform = { .mod = nullptr, .next = &next_transform };

    // Prepare image_pixel with bit_depth 8 and colour_type RGB
    image_pixel pix;
    pix.bit_depth = 8;
    pix.colour_type = PNG_COLOR_TYPE_RGB; // RGB
    pix.alpha = 0;
    pix.alphaf = 0.0;
    pix.alphae = 999; // sentinel to ensure it changes
    pix.alpha_first = 0;

    // Set global data.filler and data.flags to drive the true-branch
    data.filler = 0x55;            // 85 decimal
    data.flags = PNG_FILLER_BEFORE;  // ensure alpha_first becomes true

    next_mod_called = false;

    // pp and display are not used by the test harness; pass null
    image_transform_png_set_filler_mod(&first_transform, &pix, nullptr, nullptr);

    // Validate that the true-branch executed and updated fields correctly
    EXPECT(pix.alpha == (data.filler & 0xFF),
           "alpha should be data.filler masked by max (0xFF) when bit_depth=8");

    double expected_alphaf = static_cast<double>(pix.alpha) / 255.0;
    EXPECT(std::fabs(pix.alphaf - expected_alphaf) < 1e-12,
           "alphaf should equal alpha / max where max = 2^bit_depth - 1");

    EXPECT(pix.alphae == 0,
           "alphae should be set to 0 in filler-mod path");

    EXPECT((pix.colour_type & 0x7) == PNG_COLOR_TYPE_RGB || (pix.colour_type & 0x7) == (PNG_COLOR_TYPE_RGB),
           "colour_type should remain RGB before alpha flag injection (test scope)");

    // The code sets colour_type |= 4 to indicate alpha present
    EXPECT((pix.colour_type & 0x7) == PNG_COLOR_TYPE_RGB,
           "colour_type lower bits indicate RGB; alpha flag added via OR with 4 (high-level sanity)");

    // Directly check the alpha flag bit
    EXPECT((pix.colour_type & 4) != 0,
           "colour_type should have the alpha bit set (alpha channel added)");

    // alpha_first should reflect data.flags == PNG_FILLER_BEFORE
    EXPECT(pix.alpha_first != 0,
           "alpha_first should be true when data.flags equals PNG_FILLER_BEFORE");

    // The chain should have been invoked
    EXPECT(next_mod_called, "Next transform's mod() should be invoked in filler_mod path");
}

// Test 2: False branch when conditions are not met (bit_depth < 8 or colour_type not RGB/GRAY)
static void test_filler_mod_false_branch_no_alpha_change_and_calls_next()
{
    // Prepare a simple transform chain: first -> next (stub)
    image_transform next_transform = { .mod = stub_next_mod, .next = nullptr };
    image_transform first_transform = { .mod = nullptr, .next = &next_transform };

    // Prepare image_pixel with bit_depth < 8 to skip the if-block
    image_pixel pix;
    pix.bit_depth = 4; // < 8
    pix.colour_type = PNG_COLOR_TYPE_RGB; // still RGB, but bit_depth fails
    pix.alpha = 0;
    pix.alphaf = 0.0;
    pix.alphae = 0; 
    pix.alpha_first = 0;

    // Set data to a value that would trigger the branch if checked
    data.filler = 0x77;
    data.flags = 0; // not PNG_FILLER_BEFORE

    next_mod_called = false;

    // Call the function under test
    image_transform_png_set_filler_mod(&first_transform, &pix, nullptr, nullptr);

    // Validate that the fields were not changed by the filler logic
    // bit_depth remains 4, colour_type remains RGB
    EXPECT(pix.bit_depth == 4, "bit_depth should remain unchanged when not meeting branch condition");
    EXPECT(pix.colour_type == PNG_COLOR_TYPE_RGB, "colour_type should remain unchanged when not meeting branch condition");
    // alpha fields should remain as initialized
    EXPECT(pix.alpha == 0, "alpha should remain unchanged when branch not taken");
    EXPECT(pix.alphaf == 0.0, "alphaf should remain unchanged when branch not taken");
    EXPECT(pix.alphae == 0, "alphae should remain unchanged when branch not taken");

    // Next transform should still be invoked
    EXPECT(next_mod_called, "Next transform's mod() should be invoked even when branch is not taken");
}

// Test 3: False branch when colour_type is not RGB/GRAY despite bit_depth being >= 8
static void test_filler_mod_false_branch_color_type_not_rgb_gray()
{
    // Prepare chain
    image_transform next_transform = { .mod = stub_next_mod, .next = nullptr };
    image_transform first_transform = { .mod = nullptr, .next = &next_transform };

    // bit_depth >= 8 but colour_type is not RGB/GRAY (e.g., COLOR_TYPE_PALETTE)
    image_pixel pix;
    pix.bit_depth = 8;
    pix.colour_type = PNG_COLOR_TYPE_PALETTE; // not RGB/GRAY (test condition failure)
    pix.alpha = 0;
    pix.alphaf = 0.0;
    pix.alphae = 123;
    pix.alpha_first = 0;

    data.filler = 0xAA;
    data.flags = PNG_FILLER_BEFORE;

    next_mod_called = false;

    image_transform_png_set_filler_mod(&first_transform, &pix, nullptr, nullptr);

    // Should not modify alpha fields because the colour_type check fails
    EXPECT(pix.alphaf == 0.0, "alphaf should remain unchanged when branch not taken due to colour_type");
    EXPECT(pix.alphae == 123, "alphae should remain unchanged when branch not taken");
    // Next transform should still be invoked
    EXPECT(next_mod_called, "Next transform's mod() should be invoked even when branch not taken");
}

// Simple entry point: run tests
int main() {
    std::cout << "Starting tests for image_transform_png_set_filler_mod (C API) using C++11 harness." << std::endl;

    test_filler_mod_true_branch_sets_alpha_and_color_and_calls_next();
    test_filler_mod_false_branch_no_alpha_change_and_calls_next();
    test_filler_mod_false_branch_color_type_not_rgb_gray();

    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << std::endl;
    return (tests_failed == 0) ? 0 : 1;
}
```