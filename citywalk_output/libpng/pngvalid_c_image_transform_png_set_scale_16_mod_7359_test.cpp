/*
Unit test suite for the focal method:
    image_transform_png_set_scale_16_mod

Overview:
- The function modifies the image pixel transform when the pixel bit_depth is 16:
  - Sets that->sample_depth and that->bit_depth to 8
  - Clamps sBIT fields to 8 if they exceed 8
  - Delegates to the next transform in the chain via this->next->mod(...)
- We'll create a minimal test harness in C++11 that mimics the required structures
  and provides a fake "next" transform with a mod function to observe interactions.

Notes on Candidate Keywords (Step 1):
- bit_depth, sample_depth
- 16-bit handling path
- sBIT fields: red_sBIT, green_sBIT, blue_sBIT, alpha_sBIT
- next->mod invocation (chain of responsibility)
- png_const_structp, transform_display (pp, display)
- image_transform (this), image_pixel (that)

Test plan (Step 2 & 3):
- Test 1: bit_depth == 16 triggers depth lowering to 8, clamps sBIT to 8 where > 8, and calls next->mod.
- Test 2: bit_depth != 16 does not modify fields and still calls next->mod.
- For both cases, verify that next->mod was invoked, ensuring the delegation path is exercised.
- Use self-contained, non-terminating assertions to allow full test execution.

Important:
- This test suite uses C++11 only and does not rely on GTest.
- We declare extern "C" for the focal function to ensure proper linkage with the C implementation in pngvalid.c.
- We define minimal struct representations to match the function’s expectations, including the next transform hook.

How to run:
- Compile as part of the project with the C source (pngvalid.c) available in the build.
- The test harness returns 0 on all tests pass; non-zero if any test fails.

Code (test_suite_png_scale16_mod.cpp):
*/

#include <signal.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Step: Provide C linkage for the focal function
extern "C" {

// Forward-declare the minimal structures matching what the focal method expects.
// These definitions are crafted to align with what image_transform_png_set_scale_16_mod
// uses internally based on the FOCAL_CLASS_DEP snippet.
typedef void* png_const_structp; // opaque in tests

// Minimal representation of transform_display (contents unused in tests)
struct transform_display { };

// Minimal image_pixel structure with relevant fields used by the focal method
struct image_pixel {
    unsigned int bit_depth;
    unsigned int sample_depth;
    unsigned int red_sBIT;
    unsigned int green_sBIT;
    unsigned int blue_sBIT;
    unsigned int alpha_sBIT;
};

// Forward declaration of image_transform used by the focal method
struct image_transform {
    // Pointer to the next transform in the chain
    struct image_transform *next;
    // The mod hook invoked by the focal method
    void (*mod)(struct image_transform* this, struct image_pixel* that,
                png_const_structp pp, const struct transform_display* display);
};

// The focal method under test (C linkage)
void image_transform_png_set_scale_16_mod(const struct image_transform *this,
    struct image_pixel *that, png_const_structp pp,
    const struct transform_display *display);
}

// ---------------------------------------------------------------------------
// Test harness scaffolding (non-GTest, non-GMock)
static int g_test_failures = 0;

#define LOG_FAIL(fmt, ...) \
    do { fprintf(stderr, "TEST_FAIL: " fmt "\n", ##__VA_ARGS__); } while(0)

#define ASSERT_TRUE(cond, msg) \
    do { if (!(cond)) { g_test_failures++; LOG_FAIL("ASSERT_TRUE failed: %s", msg); } } while(0)

#define ASSERT_EQ_INT(expected, actual, msg) \
    do { if ((int)(expected) != (int)(actual)) { g_test_failures++; LOG_FAIL("ASSERT_EQ_INT failed: %s (expected %d, got %d)", msg, (int)(expected), (int)(actual)); } } while(0)

#define ASSERT_TRUE_MSG(cond, msg) ASSERT_TRUE(cond, msg)

// Helper to reset failure counter per test
#define RESET_FAILURES() do { g_test_failures = 0; } while(0)
#define GET_FAILURES() (g_test_failures)

// ---------------------------------------------------------------------------
// Fake next transform to observe delegation and capture calls
static bool g_next_mod_called;

// Dummy mod function to attach to the "next" transform
static void fake_next_mod(struct image_transform* this,
                          struct image_pixel* that,
                          png_const_structp pp,
                          const struct transform_display* display)
{
    // Just record that we were invoked; do not alter the pixel here
    (void)this; (void)that; (void)pp; (void)display;
    g_next_mod_called = true;
}

// ---------------------------------------------------------------------------
// Test 1: bit_depth == 16 should lower depth to 8, clamp sBIT fields to 8, and call next->mod
static void test_scale16_mod_reduces_depth_and_clamps_sbits_and_calls_next()
{
    RESET_FAILURES();
    g_next_mod_called = false;

    // Build a transform chain: main_transform -> leaf_transform
    struct image_transform leaf_transform = { nullptr, &fake_next_mod };
    struct image_transform main_transform = { &leaf_transform, nullptr }; // this->next points to leaf

    // Pixel with bit_depth == 16 and sBIT values some above/below 8
    struct image_pixel pix;
    pix.bit_depth = 16;
    pix.sample_depth = 16;
    pix.red_sBIT   = 9;   // >8 -> should clamp to 8
    pix.green_sBIT = 5;   // <=8 -> unchanged
    pix.blue_sBIT  = 20;  // >8 -> should clamp to 8
    pix.alpha_sBIT = 7;   // <=8 -> unchanged

    // Call the focal method
    image_transform_png_set_scale_16_mod(&main_transform,
                                       &pix,
                                       /*pp*/ nullptr,
                                       /*display*/ nullptr);

    // Validate: both depths set to 8
    ASSERT_EQ_INT(8, pix.bit_depth, "bit_depth should be set to 8 when input is 16");
    ASSERT_EQ_INT(8, pix.sample_depth, "sample_depth should be set to 8 when input is 16");

    // sBIT clamping
    ASSERT_EQ_INT(8, pix.red_sBIT, "red_sBIT should be clamped to 8 if > 8");
    ASSERT_EQ_INT(5, pix.green_sBIT, "green_sBIT should remain unchanged if <= 8");
    ASSERT_EQ_INT(8, pix.blue_sBIT, "blue_sBIT should be clamped to 8 if > 8");
    ASSERT_EQ_INT(7, pix.alpha_sBIT, "alpha_sBIT should remain unchanged if <= 8");

    // next->mod should have been called
    ASSERT_TRUE_MSG(g_next_mod_called, "next->mod should be invoked when bit_depth == 16");

    if (GET_FAILURES() == 0) {
        printf("Test 1 passed: scale_16_mod correctly adjusts 16-bit inputs and delegates.\n");
    } else {
        printf("Test 1 failed with %d error(s).\n", GET_FAILURES());
    }
}

// ---------------------------------------------------------------------------
// Test 2: bit_depth != 16 should not modify pixel fields but should still call next->mod
static void test_scale16_mod_no_change_when_bit_depth_not_16_and_calls_next()
{
    RESET_FAILURES();
    g_next_mod_called = false;

    // Reuse the same transform chain
    struct image_transform leaf_transform = { nullptr, &fake_next_mod };
    struct image_transform main_transform = { &leaf_transform, nullptr };

    struct image_pixel pix;
    pix.bit_depth = 8;       // Not 16, so no changes expected
    pix.sample_depth = 12;
    pix.red_sBIT   = 12;     // values that would be changed if 16 path taken
    pix.green_sBIT = 6;
    pix.blue_sBIT  = 2;
    pix.alpha_sBIT = 3;

    image_transform_png_set_scale_16_mod(&main_transform,
                                       &pix,
                                       nullptr,
                                       nullptr);

    // Validate: fields remain unchanged
    ASSERT_EQ_INT(8, pix.bit_depth, "bit_depth should remain unchanged when input != 16");
    ASSERT_EQ_INT(12, pix.sample_depth, "sample_depth should remain unchanged when input != 16");
    ASSERT_EQ_INT(12, pix.red_sBIT, "red_sBIT should remain unchanged when input != 16");
    ASSERT_EQ_INT(6,  pix.green_sBIT, "green_sBIT should remain unchanged when input != 16");
    ASSERT_EQ_INT(2,  pix.blue_sBIT, "blue_sBIT should remain unchanged when input != 16");
    ASSERT_EQ_INT(3,  pix.alpha_sBIT, "alpha_sBIT should remain unchanged when input != 16");

    // next->mod should have been called
    ASSERT_TRUE_MSG(g_next_mod_called, "next->mod should be invoked even when bit_depth != 16");

    if (GET_FAILURES() == 0) {
        printf("Test 2 passed: scale_16_mod leaves values unchanged for non-16-bit input and delegates.\n");
    } else {
        printf("Test 2 failed with %d error(s).\n", GET_FAILURES());
    }
}

// ---------------------------------------------------------------------------
// Helper to run all tests and summarize
static void run_all_tests()
{
    printf("Starting tests for image_transform_png_set_scale_16_mod...\n");

    test_scale16_mod_reduces_depth_and_clamps_sbits_and_calls_next();
    test_scale16_mod_no_change_when_bit_depth_not_16_and_calls_next();

    // Summary
    int total_failures = g_test_failures; // last test's failure count is sufficient
    if (total_failures == 0) {
        printf("ALL TESTS PASSED.\n");
    } else {
        printf("%d TEST(S) FAILED.\n", total_failures);
    }
}

// ---------------------------------------------------------------------------
// Main: run tests
int main(void)
{
    run_all_tests();
    return (g_test_failures == 0) ? 0 : 1;
}