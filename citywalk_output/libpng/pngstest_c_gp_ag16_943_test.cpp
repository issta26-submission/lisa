/*
Test suite for gp_ag16(Pixel *p, png_const_voidp pb) taken from pngstest.c

Notes:
- This test uses a minimal, self-contained C++11 harness (no gtest).
- It relies on the libpng-like types declared in <png.h>. We wrap the C header for C linkage.
- We declare Pixel to match the layout expected by gp_ag16 (r,g,b,a as 16-bit fields).
- We exercise basic, offset, and multiple-call scenarios to ensure gp_ag16 correctly assigns r/g/b from pp[1] and a from pp[0].
- All tests use non-terminating expectations (they continue after failures) and a final summary.
- Compile with appropriate include paths to libpng headers so that gp_ag16 is linked from the C source.
*/

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <png.h>
#include <ctype.h>


// Include libpng headers with C linkage
extern "C" {
}

// Match the Pixel structure used by the focal method (gp_ag16 expects r,g,b,a as 16-bit values)
typedef struct Pixel {
    png_uint_16 r;
    png_uint_16 g;
    png_uint_16 b;
    png_uint_16 a;
} Pixel;

// Function prototype of the focal method under test (C linkage)
extern "C" void gp_ag16(Pixel *p, png_const_voidp pb);

// Simple non-terminating test harness
static int g_total = 0;
static int g_failed = 0;

// Expectation macro that does not abort on failure
#define EXPECT_EQ_U16(actual, expected) \
    do { \
        ++g_total; \
        if (static_cast<uint16_t>(actual) != static_cast<uint16_t>(expected)) { \
            ++g_failed; \
            fprintf(stderr, "FAIL: %s == %s, actual=%u, expected=%u (file %s:%d)\n", \
                    #actual, #expected, static_cast<unsigned>(actual), static_cast<unsigned>(expected), __FILE__, __LINE__); \
        } \
    } while (0)

// Test 1: Basic correctness with alpha=pp[0], color=pp[1]
/*
   Given pb = { alpha, color }, gp_ag16 should set:
     p->r = p->g = p->b = color
     p->a = alpha
*/
void test_gp_ag16_basic()
{
    Pixel p{};
    uint16_t data[2] = { 0x1111, 0x2222 }; // alpha=0x1111, color=0x2222
    gp_ag16(&p, data);

    EXPECT_EQ_U16(p.a, 0x1111);
    EXPECT_EQ_U16(p.r, 0x2222);
    EXPECT_EQ_U16(p.g, 0x2222);
    EXPECT_EQ_U16(p.b, 0x2222);
}

// Test 2: Edge values (all zeros and maximal alpha)
void test_gp_ag16_extremes()
{
    Pixel p{};
    uint16_t data[2] = { 0xFFFF, 0x0000 }; // alpha=0xFFFF, color=0x0000

    gp_ag16(&p, data);

    EXPECT_EQ_U16(p.a, 0xFFFF);
    EXPECT_EQ_U16(p.r, 0x0000);
    EXPECT_EQ_U16(p.g, 0x0000);
    EXPECT_EQ_U16(p.b, 0x0000);
}

// Test 3: pb pointing into a larger memory block (offset past first two elements)
void test_gp_ag16_memory_offset()
{
    Pixel p{};
    uint16_t data4[4] = { 0x1111, 0x2222, 0x3333, 0x4444 }; // [0]=alpha, [1]=color, [2]=alpha2, [3]=color2
    // Pass pb as &data4[2], so alpha=data4[2]=0x3333, color=data4[3]=0x4444
    gp_ag16(&p, &data4[2]);

    EXPECT_EQ_U16(p.a, 0x3333);
    EXPECT_EQ_U16(p.r, 0x4444);
    EXPECT_EQ_U16(p.g, 0x4444);
    EXPECT_EQ_U16(p.b, 0x4444);
}

// Test 4: Multiple consecutive calls with different pb values
void test_gp_ag16_multiple_calls()
{
    Pixel p{};

    uint16_t data1[2] = { 0xAAAA, 0x5555 }; // alpha=0xAAAA, color=0x5555
    gp_ag16(&p, data1);
    EXPECT_EQ_U16(p.a, 0xAAAA);
    EXPECT_EQ_U16(p.r, 0x5555);
    EXPECT_EQ_U16(p.g, 0x5555);
    EXPECT_EQ_U16(p.b, 0x5555);

    uint16_t data2[2] = { 0x1234, 0x5678 }; // new alpha/color
    gp_ag16(&p, data2);
    EXPECT_EQ_U16(p.a, 0x1234);
    EXPECT_EQ_U16(p.r, 0x5678);
    EXPECT_EQ_U16(p.g, 0x5678);
    EXPECT_EQ_U16(p.b, 0x5678);
}

// Entry point
int main(void)
{
    // Run all tests
    test_gp_ag16_basic();
    test_gp_ag16_extremes();
    test_gp_ag16_memory_offset();
    test_gp_ag16_multiple_calls();

    // Summary
    int passed = g_total - g_failed;
    std::cout << "gp_ag16 test summary: " << passed << " passed, "
              << g_failed << " failed, out of " << g_total << " tests." << std::endl;

    // Return non-zero if any test failed
    if (g_failed != 0)
        return 1;
    return 0;
}