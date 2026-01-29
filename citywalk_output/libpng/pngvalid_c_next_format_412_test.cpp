// Automated unit tests for next_format in pngvalid.c
// Target: C++11, no GTest, lightweight test harness with non-terminating assertions.
// The tests are designed to exercise the various branches of next_format as described
// in the provided focal method. They mock minimal PNG types to avoid external dependencies.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <math.h>


// Minimal type aliases to mirror the libpng types without requiring libpng headers.
// This keeps the tests self-contained while still matching the function signature.
typedef unsigned char png_byte;
typedef png_byte* png_bytep;

// Forward declaration of the focal C function (linker will bind to the actual implementation).
// We declare with C linkage to avoid name mangling.
extern "C" int next_format(png_bytep colour_type, png_bytep bit_depth,
   unsigned int* palette_number, int low_depth_gray, int tRNS);

// Simple non-terminating test harness
static int g_failures = 0;
static int g_tests_run = 0;

#define EXPECT_EQ_INT(expected, actual) do { \
    g_tests_run++; \
    if ((expected) != (actual)) { \
        printf("  [FAIL] Expected %d, got %d (in %s:%d)\n", (int)(expected), (int)(actual), __FILE__, __LINE__); \
        g_failures++; \
    } \
} while(0)

#define EXPECT_EQ_US(expected, actual) do { \
    g_tests_run++; \
    if ((unsigned)(expected) != (unsigned)(actual)) { \
        printf("  [FAIL] Expected %u, got %u (in %s:%d)\n", (unsigned)(expected), (unsigned)(actual), __FILE__, __LINE__); \
        g_failures++; \
    } \
} while(0)

#define EXPECT_NEQ_NULL(ptr) do { \
    g_tests_run++; \
    if ((ptr) == nullptr) { \
        printf("  [FAIL] Pointer unexpectedly null at %s:%d\n", __FILE__, __LINE__); \
        g_failures++; \
    } \
} while(0)

// Test 1: bit_depth == 0 with low_depth_gray == true
// Expectation: colour_type becomes 0 (GRAY), bit_depth becomes 1, palette_number reset to 0, return 1.
static void test_bitdepth_zero_lowgray_true()
{
    unsigned char ct = 0xFF;           // arbitrary
    unsigned char bd = 0;                // triggers initial branch
    unsigned int palette_number = 123;   // arbitrary initial value
    int low_depth_gray = 1;              // triggers 1-bit depth
    int tRNS = 0;

    int ret = next_format(&ct, &bd, &palette_number, low_depth_gray, tRNS);

    EXPECT_EQ_INT(1, ret);
    EXPECT_EQ_INT(0, ct);
    EXPECT_EQ_INT(1, bd);
    EXPECT_EQ_INT(0, palette_number);
}

// Test 2: bit_depth == 0 with low_depth_gray == false
// Expectation: colour_type becomes 0 (GRAY), bit_depth becomes 8, palette_number reset to 0, return 1.
static void test_bitdepth_zero_lowgray_false()
{
    unsigned char ct = 0xFF;
    unsigned char bd = 0;
    unsigned int palette_number = 5;
    int low_depth_gray = 0;
    int tRNS = 0;

    int ret = next_format(&ct, &bd, &palette_number, low_depth_gray, tRNS);

    EXPECT_EQ_INT(1, ret);
    EXPECT_EQ_INT(0, ct);
    EXPECT_EQ_INT(8, bd);
    EXPECT_EQ_INT(0, palette_number);
}

// Test 3: colour_type < 4 with tRNS true causes early return and palette_number increments
// We'll choose Gray (ct=0) with tRNS=1 to hit the first conditional branch.
static void test_color_type_lt4_with_tRNS_increments_palette()
{
    unsigned char ct = 0;      // GRAY
    unsigned char bd = 4;
    unsigned int palette_number = 0;
    int low_depth_gray = 0;
    int tRNS = 1;

    int ret = next_format(&ct, &bd, &palette_number, low_depth_gray, tRNS);

    EXPECT_EQ_INT(1, ret);
    EXPECT_EQ_INT(0, ct);        // unchanged
    EXPECT_EQ_INT(4, bd);        // unchanged
    EXPECT_EQ_INT(1, palette_number); // palette_number incremented to 1
}

// Test 4: colour_type < 4 but early return not triggered; bit_depth doubles and may trigger return due to <=8
// Use ct=1 (within <4) and initial bit_depth=4 so after doubling to 8 we hit the "bit_depth <= 8" short-circuit.
static void test_color_type_lt4_no_immediate_palette()
{
    unsigned char ct = 1;  // within <4
    unsigned char bd = 4;
    unsigned int palette_number = 0;
    int low_depth_gray = 0;
    int tRNS = 0;

    int ret = next_format(&ct, &bd, &palette_number, low_depth_gray, tRNS);

    EXPECT_EQ_INT(1, ret);
    EXPECT_EQ_INT(1, ct);     // unchanged
    EXPECT_EQ_INT(8, bd);     // bit_depth doubles to 8
    EXPECT_EQ_INT(0, palette_number);
}

// Test 5: colour_type == 3 (PALETTE) with bit_depth progression reaching the switch
// After doubling from 8 to 16, we should move to colour_type 4 and bit_depth 8.
static void test_color_type_3_palette_switch_to_rgb()
{
    unsigned char ct = 3;  // PALETTE
    unsigned char bd = 8;
    unsigned int palette_number = 0;
    int low_depth_gray = 0;
    int tRNS = 0;

    int ret = next_format(&ct, &bd, &palette_number, low_depth_gray, tRNS);

    EXPECT_EQ_INT(1, ret);
    EXPECT_EQ_INT(4, ct);    // next color type
    EXPECT_EQ_INT(8, bd);    // bit_depth reset to 8 in that branch
}

// Test 6: Unknown colour_type causes default branch in switch to return 0
// Use ct = 5 (outside the handled range)
static void test_unknown_colour_type_returns_zero()
{
    unsigned char ct = 5;  // unknown
    unsigned char bd = 8;
    unsigned int palette_number = 0;
    int low_depth_gray = 0;
    int tRNS = 0;

    int ret = next_format(&ct, &bd, &palette_number, low_depth_gray, tRNS);

    EXPECT_EQ_INT(0, ret);
    // Post-conditions: function returns 0; bit_depth may have been advanced to 16, ct unchanged or updated
    EXPECT_EQ_INT(5, ct);     // colour_type unchanged in the default path
    EXPECT_EQ_INT(16, bd);    // bit_depth advanced by 1 shift
}

// Entry point
int main(void)
{
    printf("Starting test suite for next_format...\n");

    test_bitdepth_zero_lowgray_true();
    test_bitdepth_zero_lowgray_false();
    test_color_type_lt4_with_tRNS_increments_palette();
    test_color_type_lt4_no_immediate_palette();
    test_color_type_3_palette_switch_to_rgb();
    test_unknown_colour_type_returns_zero();

    printf("Test results: runs=%d, failures=%d\n", g_tests_run, g_failures);
    if (g_failures == 0)
        printf("All tests passed.\n");
    else
        printf("Some tests failed. See above for details.\n");

    return g_failures;
}