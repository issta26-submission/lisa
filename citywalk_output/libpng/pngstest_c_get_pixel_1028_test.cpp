// cpp_pngstest_get_pixel_tests.cpp
// A C++11 test suite for the focal function get_pixel from pngstest.c
// Note: This test does not use GoogleTest. It uses a lightweight inline harness.
// It relies on the libpng types/macros and on the gp_* pixel generator functions
// defined in the pngstest.c test utilities.

// The goal is to validate that get_pixel() returns the correct function pointer
// for various combinations of the PNG format flags (LINEAR, COLOR, BGR, ALPHA, AFIRST),
// taking into account the #ifdef-controlled branches (BGR_SUPPORTED, AFIRST_SUPPORTED).

#include <cstdint>
#include <cassert>
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


// Include libpng types/macros for the flags used by the focal function

// Forward declare the Pixel type used by the focal code (in pngstest.c)
extern "C" struct Pixel;
using Pixel = struct Pixel;

// Forward declare all gp_* functions that get_pixel may return.
// We declare them with C linkage so that we can compare function pointers across
// the C/C++ boundary when linking with pngstest.c.
extern "C" Pixel* gp_g8(Pixel*, png_const_voidp);
extern "C" Pixel* gp_ga8(Pixel*, png_const_voidp);
extern "C" Pixel* gp_ag8(Pixel*, png_const_voidp);
extern "C" Pixel* gp_rgb8(Pixel*, png_const_voidp);
extern "C" Pixel* gp_bgr8(Pixel*, png_const_voidp);
extern "C" Pixel* gp_rgba8(Pixel*, png_const_voidp);
extern "C" Pixel* gp_bgra8(Pixel*, png_const_voidp);
extern "C" Pixel* gp_argb8(Pixel*, png_const_voidp);
extern "C" Pixel* gp_abgr8(Pixel*, png_const_voidp);

extern "C" Pixel* gp_g16(Pixel*, png_const_voidp);
extern "C" Pixel* gp_ga16(Pixel*, png_const_voidp);
extern "C" Pixel* gp_ag16(Pixel*, png_const_voidp);
extern "C" Pixel* gp_rgb16(Pixel*, png_const_voidp);
extern "C" Pixel* gp_bgr16(Pixel*, png_const_voidp);
extern "C" Pixel* gp_rgba16(Pixel*, png_const_voidp);
extern "C" Pixel* gp_bgra16(Pixel*, png_const_voidp);
extern "C" Pixel* gp_argb16(Pixel*, png_const_voidp);
extern "C" Pixel* gp_abgr16(Pixel*, png_const_voidp);

// get_pixel is declared in the same C file and uses C linkage when compiled as part
// of the pngstest.c translation unit. We declare its signature here to obtain
// the function pointer for a given PNG format flag set.
extern "C" Pixel* (*get_pixel(png_uint_32 format))(Pixel*, png_const_voidp);

// Lightweight test harness
static int g_total = 0;
static int g_passed = 0;

static void test_pass(const char* name) {
    ++g_total;
    ++g_passed;
    std::cout << "[PASS] " << name << "\n";
}

static void test_fail(const char* name) {
    ++g_total;
    std::cout << "[FAIL] " << name << "\n";
}

// Helper to print summary
static void test_summary() {
    std::cout << "\nTest summary: " << g_passed << " / " << g_total << " tests passed.\n";
}

// Helper: compare two 16-bit function pointers for equality
using PixelFunc = Pixel* (*)(Pixel*, png_const_voidp);

// Main test cases
int main(void)
{
    // Test 1: 16-bit linear, color, BGR, ALPHA, AFIRST (if AFIRST_SUPPORTED)
    // Expected: ABGR16 if AFIRST is defined and set, else BGRA16 when BGR & ALPHA & AFIRST (or BGRA16 if AFIRST not supported)
    {
        PNG_UINT_32 f1 = PNG_FORMAT_FLAG_LINEAR | PNG_FORMAT_FLAG_COLOR;
        #if defined(PNG_FORMAT_BGR_SUPPORTED)
        f1 |= PNG_FORMAT_FLAG_BGR;
        #endif
        f1 |= PNG_FORMAT_FLAG_ALPHA;
        #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
        f1 |= PNG_FORMAT_FLAG_AFIRST;
        #endif

        PixelFunc actual = get_pixel(f1);

        PixelFunc expected;
        #if defined(PNG_FORMAT_BGR_SUPPORTED)
            #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
                if (f1 & PNG_FORMAT_FLAG_AFIRST)
                    expected = gp_abgr16;
                else
                    expected = gp_bgra16;
            #else
                expected = gp_bgra16;
            #endif
        #else
            #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
                if (f1 & PNG_FORMAT_FLAG_AFIRST)
                    expected = gp_argb16;
                else
                    expected = gp_rgba16;
            #else
                expected = gp_rgba16;
            #endif
        #endif

        if (actual == expected) {
            test_pass("Test1: 16-bit linear color with BGR/ALPHA (+AFIRST if supported)");
        } else {
            test_fail("Test1: 16-bit linear color with BGR/ALPHA (+AFIRST if supported)");
        }
    }

    // Test 2a: 16-bit linear, color, ALPHA, with AFIRST flag set (when supported); no BGR
    // Expected: ARGB16 if AFIRST is defined and enabled, else RGBA16
    {
        PNG_UINT_32 f2 = PNG_FORMAT_FLAG_LINEAR | PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_FLAG_ALPHA;
        #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
        f2 |= PNG_FORMAT_FLAG_AFIRST;
        #endif

        PixelFunc actual = get_pixel(f2);

        PixelFunc expected;
        #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
            if (f2 & PNG_FORMAT_FLAG_AFIRST)
                expected = gp_argb16;
            else
                expected = gp_rgba16;
        #else
            expected = gp_rgba16;
        #endif

        if (actual == expected) {
            test_pass("Test2a: 16-bit linear color ALPHA with AFIRST (if supported), no BGR");
        } else {
            test_fail("Test2a: 16-bit linear color ALPHA with AFIRST (if supported), no BGR");
        }
    }

    // Test 2b: 16-bit linear, color, ALPHA, WITHOUT AFIRST (when supported) - ensures branch without AFIRST
    {
        PNG_UINT_32 f2b = PNG_FORMAT_FLAG_LINEAR | PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_FLAG_ALPHA;
        // Do not set AFIRST even if supported
        #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
        // ensure AFIRST bit is not set
        // do nothing; not setting is enough
        #endif

        PixelFunc actual = get_pixel(f2b);

        PixelFunc expected;
        #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
            // AFIRST not set => RGBA16
            expected = gp_rgba16;
        #else
            expected = gp_rgba16;
        #endif

        if (actual == expected) {
            test_pass("Test2b: 16-bit linear color ALPHA without AFIRST (when supported)");
        } else {
            test_fail("Test2b: 16-bit linear color ALPHA without AFIRST (when supported)");
        }
    }

    // Test 3: Not linear, color, BGR, ALPHA
    // Expected: ABGR8 if AFIRST is defined and set, else BGRA8
    {
        PNG_UINT_32 f3 = PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_FLAG_BGR | PNG_FORMAT_FLAG_ALPHA;
        // Not linear
        #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
        f3 |= PNG_FORMAT_FLAG_AFIRST;
        #endif

        PixelFunc actual = get_pixel(f3);

        PixelFunc expected;
        #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
        if (f3 & PNG_FORMAT_FLAG_AFIRST)
            expected = gp_abgr8;
        else
            expected = gp_bgra8;
        #else
            expected = gp_bgra8;
        #endif

        if (actual == expected) {
            test_pass("Test3: Not-linear color with BGR/ALPHA (AFIRST if supported)");
        } else {
            test_fail("Test3: Not-linear color with BGR/ALPHA (AFIRST if supported)");
        }
    }

    // Test 4: Not linear, NOT color, ALPHA
    // Expected: GA8 or AG8 depending on AFIRST presence
    {
        PNG_UINT_32 f4 = PNG_FORMAT_FLAG_ALPHA;
        // Not linear and not color
        #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
        f4 |= PNG_FORMAT_FLAG_AFIRST;
        #endif

        PixelFunc actual = get_pixel(f4);

        PixelFunc expected;
        #if defined(PNG_FORMAT_AFIRST_SUPPORTED)
        if (f4 & PNG_FORMAT_FLAG_AFIRST)
            expected = gp_ag8;
        else
            expected = gp_ga8;
        #else
        // AFIRST not supported: GA8
        expected = gp_ga8;
        #endif

        if (actual == expected) {
            test_pass("Test4: Not-linear non-color with ALPHA (AFIRST if supported)");
        } else {
            test_fail("Test4: Not-linear non-color with ALPHA (AFIRST if supported)");
        }
    }

    test_summary();
    // Return number of failures as non-zero exit code could indicate failure in some runners
    return (g_total == g_passed) ? 0 : 1;
}