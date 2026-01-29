/*
  Unit test suite for the focal method:
  image_transform_png_set_add_alpha_mod

  The test suite is crafted to run under a C++11 environment without GTest.
  It uses a minimal in-repo test harness and mocks the necessary dependencies
  to verify the method's behavior, including true/false branches of the
  predicate and interaction with the next transform in the chain.

  Notes on Candidate Keywords (Step 1)
  - image_pixel fields: bit_depth, colour_type, alpha, alphaf, alphae, alpha_first
  - image_transform chain: this->next->mod(..)
  - global data state: data.filler, data.flags, PNG_FILLER_BEFORE
  - colour type constants: PNG_COLOR_TYPE_RGB, PNG_COLOR_TYPE_GRAY
  - Filler flag constants: PNG_FILLER_BEFORE, PNG_FILLER_AFTER
  - bit depth gating: bit_depth >= 8

  Assumptions for test harness (Step 2)
  - We declare minimal representations of the types used by the focal method.
  - The test defines a global data symbol named data to satisfy external reference
    in the focal C file (data.filler, data.flags).
  - The next transform in the pipeline is mocked to ensure the chain is invoked.
  - The test calls the focal function via its C linkage prototype.

  The test code below is self-contained and provides explanatory comments for each test.
*/

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <cmath>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>


extern "C" {
  // Prototype of the focal method under test (C linkage to match the library)
  void image_transform_png_set_add_alpha_mod(const struct image_transform *this,
                                           struct image_pixel *that,
                                           void *pp,
                                           const void *display);

  // Minimal type representations to instantiate and invoke the focal method.
  // These are intentionally lightweight and only contain fields accessed by the test.
  typedef struct image_pixel {
    unsigned int bit_depth;
    unsigned int colour_type;
    unsigned int alpha;
    double alphaf;
    unsigned int alphae;
    unsigned int alpha_first;
  } image_pixel;

  typedef struct image_transform image_transform;
  struct image_transform {
    image_transform *next;
    void (*mod)(image_transform *self, image_pixel *that, void *pp, const void *display);
  };

  // Lightweight placeholders for libpng-like types
  typedef void* png_const_structp;
  typedef void* transform_display;
}

// Step 1: Candidate Keywords (for reference in comments)
// - that->bit_depth, that->colour_type, that->alpha, that->alphaf, that->alphae, that->alpha_first
// - data.filler, data.flags, PNG_FILLER_BEFORE
// - that->colour_type |= 4
// - next transform invocation: this->next->mod(this->next, that, pp, display)

#define PNG_COLOR_TYPE_GRAY 0
#define PNG_COLOR_TYPE_RGB  2
#define PNG_FILLER_BEFORE   0
#define PNG_FILLER_AFTER    1

// Global data symbol expected by the focal method (to satisfy external reference)
extern "C" struct {
  unsigned int filler;
  unsigned int flags;
} data = {0, 0};

// Mock next transform to verify the chain is invoked
static int next_mod_invoked = 0;
static void mock_next_mod(image_transform *self, image_pixel *that, void *pp, const void *display) {
  // Indicate the next transform was called; also record the final state of 'that'
  next_mod_invoked++;
  // No further modification; the test inspects 'that' after the focal method returns.
}

// A static initialization of the next transform in the chain
static image_transform mock_next_transform = {
  /* .next = */ nullptr,
  /* .mod  = */ mock_next_mod
};

// Helper: epsilon for double comparison
static constexpr double EPS = 1e-12;

// Simple test harness state
static int total_tests = 0;
static int failed_tests = 0;

// Helper macros for test assertions (non-terminating)
#define ASSERT_TRUE(cond, msg) do { \
  total_tests++; \
  if (!(cond)) { \
    printf("Test fail: %s\n", msg); \
    failed_tests++; \
  } \
} while (0)

#define ASSERT_INT_EQ(a, b, msg) do { \
  total_tests++; \
  if ((int)(a) != (int)(b)) { \
    printf("Test fail: %s (expected %d, got %d)\n", msg, (int)(b), (int)(a)); \
    failed_tests++; \
  } \
} while (0)

#define ASSERT_DOUBLE_EQ(a, b, msg) do { \
  total_tests++; \
  if (std::fabs((a) - (b)) > EPS) { \
    printf("Test fail: %s (expected %f, got %f)\n", msg, (a), (b)); \
    failed_tests++; \
  } \
} while (0)

// Test 1: True branch for RGB with bit_depth >= 8
// Expect alpha derived from data.filler, alphaf computed, alphae = 0,
// colour_type updated with alpha bit (OR 4), alpha_first set according to data.flags,
// and next transform invoked.
static void test_add_alpha_mod_true_rgb() {
  printf("Test 1: True branch RGB with bit_depth >= 8\n");
  // Setup: RGB, bit_depth = 8
  image_transform root;
  root.next = &mock_next_transform;
  root.mod = nullptr; // not used by focal method directly; it uses only next
  // Prepare the pixel struct
  image_pixel pix;
  pix.bit_depth = 8;
  pix.colour_type = PNG_COLOR_TYPE_RGB;
  pix.alpha = 0;
  pix.alphaf = 0.0;
  pix.alphae = 123; // should be overwritten
  pix.alpha_first = 0;
  // Prepare data for the filler
  data.filler = 0xAB;      // 171
  data.flags = PNG_FILLER_BEFORE;
  next_mod_invoked = 0;

  image_transform *pt = &root;
  image_pixel *p = &pix;

  image_transform_png_set_add_alpha_mod((const image_transform*)pt, p, nullptr, nullptr);

  // Expected: alpha = filler & 0xFF = 0xAB
  // alphaf = alpha / 255.0
  unsigned int max = (1U << p->bit_depth) - 1; // 255
  unsigned int expected_alpha = data.filler & max; // 0xAB
  double expected_alphaf = ((double)expected_alpha) / (double)max;
  unsigned int expected_colour_type = (PNG_COLOR_TYPE_RGB) | 4;

  ASSERT_INT_EQ(p->alpha, expected_alpha, "alpha should be derived from filler (RGB)");
  ASSERT_DOUBLE_EQ(p->alphaf, expected_alphaf, "alphaf should be alpha/max");
  ASSERT_INT_EQ(p->alphae, 0, "alphae should be 0");
  ASSERT_INT_EQ((int)p->colour_type, (int)expected_colour_type, "colour_type should have alpha bit set (RGB)");
  ASSERT_INT_EQ(p->alpha_first, 1, "alpha_first should be true when data.flags == PNG_FILLER_BEFORE");
  ASSERT_TRUE(next_mod_invoked == 1, "next->mod should be invoked exactly once");
  printf("Test 1 completed\n");
}

// Test 2: True branch for Gray with bit_depth >= 8
// Ensure same alpha handling for grayscale input
static void test_add_alpha_mod_true_gray() {
  printf("Test 2: True branch Gray with bit_depth >= 8\n");
  image_transform root;
  root.next = &mock_next_transform;
  image_pixel pix;
  pix.bit_depth = 8;
  pix.colour_type = PNG_COLOR_TYPE_GRAY;
  pix.alpha = 0;
  pix.alphaf = 0.0;
  pix.alphae = 99;
  pix.alpha_first = 0;

  data.filler = 0x7F; // 127
  data.flags = PNG_FILLER_BEFORE;

  next_mod_invoked = 0;

  image_transform *pt = &root;
  image_pixel *p = &pix;

  image_transform_png_set_add_alpha_mod((const image_transform*)pt, p, nullptr, nullptr);

  unsigned int max = (1U << p->bit_depth) - 1; // 255
  unsigned int expected_alpha = data.filler & max;
  double expected_alphaf = ((double)expected_alpha) / (double)max;
  unsigned int expected_colour_type = (PNG_COLOR_TYPE_GRAY) | 4;

  ASSERT_INT_EQ(p->alpha, expected_alpha, "alpha should be derived from filler (GRAY)");
  ASSERT_DOUBLE_EQ(p->alphaf, expected_alphaf, "alphaf should be alpha/max for GRAY");
  ASSERT_INT_EQ((int)p->colour_type, (int)expected_colour_type, "GRAY colour_type should have alpha bit set (with 4)");
  ASSERT_INT_EQ(p->alphae, 0, "alphae should be 0 for GRAY");
  ASSERT_INT_EQ(p->alpha_first, 1, "alpha_first should be true for PNG_FILLER_BEFORE");
  ASSERT_TRUE(next_mod_invoked == 1, "next->mod should be invoked exactly once for GRAY");
  printf("Test 2 completed\n");
}

// Test 3: False branch due to bit_depth < 8
// Expect no modification to 'that' fields, but next->mod is still invoked.
static void test_add_alpha_mod_false_bit_depth() {
  printf("Test 3: False branch due to bit_depth < 8\n");
  image_transform root;
  root.next = &mock_next_transform;

  image_pixel pix;
  pix.bit_depth = 4; // < 8 should skip
  pix.colour_type = PNG_COLOR_TYPE_RGB;
  pix.alpha = 0x55;
  pix.alphaf = 0.25;
  pix.alphae = 7;
  pix.alpha_first = 0;

  // Record initial values
  unsigned int init_bit_depth = pix.bit_depth;
  unsigned int init_colour_type = pix.colour_type;
  double  init_alphaf = pix.alphaf;
  unsigned int init_alpha = pix.alpha;
  unsigned int init_alphae = pix.alphae;
  unsigned int init_alpha_first = pix.alpha_first;

  data.filler = 0x1A;
  data.flags = PNG_FILLER_AFTER;
  next_mod_invoked = 0;

  image_transform *pt = &root;
  image_pixel *p = &pix;

  image_transform_png_set_add_alpha_mod((const image_transform*)pt, p, nullptr, nullptr);

  // Expect no changes to pixel fields due to bit_depth < 8
  ASSERT_INT_EQ(p->bit_depth, init_bit_depth, "bit_depth should remain unchanged");
  ASSERT_INT_EQ(p->colour_type, init_colour_type, "colour_type should remain unchanged");
  ASSERT_INT_EQ(p->alpha, init_alpha, "alpha should remain unchanged");
  ASSERT_DOUBLE_EQ(p->alphaf, init_alphaf, "alphaf should remain unchanged");
  ASSERT_INT_EQ(p->alphae, init_alphae, "alphae should remain unchanged");
  ASSERT_INT_EQ(p->alpha_first, init_alpha_first, "alpha_first should remain unchanged");
  ASSERT_TRUE(next_mod_invoked == 1, "next->mod should still be invoked exactly once");
  printf("Test 3 completed\n");
}

// Test 4: False branch due to unsupported colour_type (neither RGB nor GRAY)
// Expect no modifications to 'that' fields, but next->mod is invoked.
static void test_add_alpha_mod_false_color_type() {
  printf("Test 4: False branch due to unsupported colour_type\n");
  image_transform root;
  root.next = &mock_next_transform;

  image_pixel pix;
  pix.bit_depth = 8;
  pix.colour_type = 3; // unsupported in our simplified test (neither RGB nor GRAY)
  pix.alpha = 0x10;
  pix.alphaf = 0.0625;
  pix.alphae = 0;
  pix.alpha_first = 0;

  unsigned int init_bit_depth = pix.bit_depth;
  unsigned int init_colour_type = pix.colour_type;
  double init_alphaf = pix.alphaf;
  unsigned int init_alpha = pix.alpha;
  unsigned int init_alphae = pix.alphae;
  unsigned int init_alpha_first = pix.alpha_first;

  data.filler = 0x99;
  data.flags = PNG_FILLER_BEFORE;
  next_mod_invoked = 0;

  image_transform *pt = &root;
  image_pixel *p = &pix;

  image_transform_png_set_add_alpha_mod((const image_transform*)pt, p, nullptr, nullptr);

  // Expect no changes to pixel fields due to unsupported colour_type
  ASSERT_INT_EQ(p->bit_depth, init_bit_depth, "bit_depth should remain unchanged");
  ASSERT_INT_EQ(p->colour_type, init_colour_type, "colour_type should remain unchanged");
  ASSERT_INT_EQ(p->alpha, init_alpha, "alpha should remain unchanged");
  ASSERT_DOUBLE_EQ(p->alphaf, init_alphaf, "alphaf should remain unchanged");
  ASSERT_INT_EQ(p->alphae, init_alphae, "alphae should remain unchanged");
  ASSERT_INT_EQ(p->alpha_first, init_alpha_first, "alpha_first should remain unchanged");
  ASSERT_TRUE(next_mod_invoked == 1, "next->mod should be invoked exactly once");
  printf("Test 4 completed\n");
}

// Main test runner
int main() {
  printf("Starting unit tests for image_transform_png_set_add_alpha_mod\n");
  test_add_alpha_mod_true_rgb();
  test_add_alpha_mod_true_gray();
  test_add_alpha_mod_false_bit_depth();
  test_add_alpha_mod_false_color_type();

  if (failed_tests == 0) {
    printf("ALL TESTS PASSED (%d/%d)\n", total_tests, total_tests);
    return 0;
  } else {
    printf("TESTS FAILED: %d failed, %d total\n", failed_tests, total_tests);
    return 1;
  }
}