/*
 Step 1 (Program Understanding + Candidate Keywords)
 - Core dependent components for gp_bgra16:
   Candidate Keywords: Pixel, png_const_voidp, png_const_uint_16p, voidcast, gp_bgra16
 - Function purpose: Convert 4 x 16-bit channel values from a little-endian-like array pb
   to a Pixel with channels mapped as: r = pb[2], g = pb[1], b = pb[0], a = pb[3]
 - Dependencies: libpng types (png_uint_16, png_const_voidp) and the Pixel structure
 - Note: We'll test gp_bgra16 in isolation by providing a Pixel instance and a 4-element
   array of 16-bit values, verifying correct channel assignments.
*/

/*
 Step 2 (Unit Test Generation)
 - Tests target gp_bgra16(Pixel *p, png_const_voidp pb) defined in pngstest.c
 - Tests use a Pixel layout compatible with gp_bgra16: 16-bit RGBA components.
 - Test strategy:
   1) Basic mapping test with a known pb array: [B, G, R, A] -> r=R, g=G, b=B, a=A
   2) Multiple-pattern tests to verify mapping for various input values
   3) Consistency test across several iterations to exercise the mapping logic
 - Test harness uses direct main() invocation (no Google Test); non-terminating assertions via counters
 - We declare gp_bgra16 as extern "C" to link with the C implementation in pngstest.c
 - Pixel type is defined to match the expected layout using libpng's png_uint_16
 - We rely on libpng's types (png_uint_16, png_const_voidp)
*/

/* Unit test code for gp_bgra16 using C++11, no GTest framework. */
#include <cstdint>
#include <vector>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <string>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <png.h>
#include <ctype.h>


// Forward declaration of the focal function (defined in pngstest.c)
extern "C" void gp_bgra16(Pixel *p, png_const_voidp pb);

// Pixel layout expected by gp_bgra16
typedef struct Pixel {
    png_uint_16 r;
    png_uint_16 g;
    png_uint_16 b;
    png_uint_16 a;
} Pixel;

/*
 Helper: simple test logging
 Keeps track of failures and reports at the end.
*/
static int g_test_failures = 0;

static void log_fail(const std::string &desc) {
    std::cerr << "TEST_FAIL: " << desc << std::endl;
    ++g_test_failures;
}

// Helper: test macro-like function for 16-bit equality with message
static void expect_eq16(const char* desc, png_uint_16 got, png_uint_16 expected) {
    if (got != expected) {
        log_fail(std::string(desc) + " (got " + std::to_string(got) +
                 ", expected " + std::to_string(expected) + ")");
    }
}

/*
 Test 1: Basic true mapping
   pb array in order [B, G, R, A] should map to r=R, g=G, b=B, a=A
*/
static void test_gp_bgra16_basic_mapping() {
    Pixel p;
    // pb contains: B=0x0011, G=0x0022, R=0x0033, A=0x0044
    png_uint_16 pb_vals[4] = { 0x0011, 0x0022, 0x0033, 0x0044 };
    gp_bgra16(&p, pb_vals);  // pb as void*

    expect_eq16("gp_bgra16_basic_mapping: r should equal pb[2] (R)",
                p.r, 0x0033);
    expect_eq16("gp_bgra16_basic_mapping: g should equal pb[1] (G)",
                p.g, 0x0022);
    expect_eq16("gp_bgra16_basic_mapping: b should equal pb[0] (B)",
                p.b, 0x0011);
    expect_eq16("gp_bgra16_basic_mapping: a should equal pb[3] (A)",
                p.a, 0x0044);
}

/*
 Test 2: Multiple patterns to ensure correctness across a set of inputs
   Loop over several pb patterns and verify mapping each time.
*/
static void test_gp_bgra16_multiple_patterns() {
    for (int i = 0; i < 5; ++i) {
        Pixel p;
        png_uint_16 B = static_cast<png_uint_16>(i);
        png_uint_16 G = static_cast<png_uint_16>(i + 1);
        png_uint_16 R = static_cast<png_uint_16>(i + 2);
        png_uint_16 A = static_cast<png_uint_16>(i + 3);
        png_uint_16 pb_vals[4] = { B, G, R, A };

        gp_bgra16(&p, pb_vals);  // pb as void*

        expect_eq16(("gp_bgra16_multiple_patterns: r (pattern " + std::to_string(i) + ")").c_str(),
                    p.r, R);
        expect_eq16(("gp_bgra16_multiple_patterns: g (pattern " + std::to_string(i) + ")").c_str(),
                    p.g, G);
        expect_eq16(("gp_bgra16_multiple_patterns: b (pattern " + std::to_string(i) + ")").c_str(),
                    p.b, B);
        expect_eq16(("gp_bgra16_multiple_patterns: a (pattern " + std::to_string(i) + ")").c_str(),
                    p.a, A);
    }
}

/*
 Test 3: Consistency check across varying pb values to ensure consistent mapping
   Variation: ensure(pb[2]) becomes r, pb[1] becomes g, pb[0] becomes b, pb[3] becomes a
*/
static void test_gp_bgra16_consistency() {
    Pixel p;
    std::vector<std::array<png_uint_16, 4>> samples = {
        {{ 10, 20, 30, 40 }},
        {{ 0xAA55, 0x00AA, 0x55AA, 0xFFFF }},
        {{ 0, 1, 2, 3 }},
        {{ 65535, 32768, 16384, 4095 }},
    };

    for (size_t idx = 0; idx < samples.size(); ++idx) {
        const auto &s = samples[idx];
        png_uint_16 pb_vals[4] = { s[0], s[1], s[2], s[3] }; // [B, G, R, A]
        gp_bgra16(&p, pb_vals);

        expect_eq16(("gp_bgra16_consistency: r pattern " + std::to_string(idx)).c_str(),
                    p.r, s[2]);
        expect_eq16(("gp_bgra16_consistency: g pattern " + std::to_string(idx)).c_str(),
                    p.g, s[1]);
        expect_eq16(("gp_bgra16_consistency: b pattern " + std::to_string(idx)).c_str(),
                    p.b, s[0]);
        expect_eq16(("gp_bgra16_consistency: a pattern " + std::to_string(idx)).c_str(),
                    p.a, s[3]);
    }
}

/*
 Step 3 (Test Case Refinement)
 - The tests above cover true branches (assignment paths) and exercise multiple inputs.
 - Static members: not applicable in this C translation unit; we only test a free function.
 - All tests are run from main(), without terminating on first failure.
 - Non-terminating assertions are implemented via log_fail() and a global counter.
*/

int main() {
    // Run tests
    test_gp_bgra16_basic_mapping();
    test_gp_bgra16_multiple_patterns();
    test_gp_bgra16_consistency();

    if (g_test_failures == 0) {
        std::cout << "ALL GP_BGRA16 TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " GP_BGRA16 TEST(S) FAILED" << std::endl;
        return g_test_failures;
    }
}