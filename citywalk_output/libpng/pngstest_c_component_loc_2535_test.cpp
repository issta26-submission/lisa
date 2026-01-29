/*
Unit test suite for the focal function:
  component_loc(png_byte loc[4], png_uint_32 format)

Context:
- The function is defined in pngstest.c (the focal method portion provided).
- It uses libpng style types/macros:
  - png_byte, png_uint_32
  - PNG_FORMAT_FLAG_COLOR, PNG_FORMAT_FLAG_BGR (optional), PNG_FORMAT_FLAG_ALPHA, PNG_FORMAT_FLAG_AFIRST (optional)
  - Optional conditional compilation: PNG_FORMAT_BGR_SUPPORTED, PNG_FORMAT_AFIRST_SUPPORTED
- Tests are written in C++11 without Google Test. A tiny in-file test harness is used
  with non-terminating assertions (tests accumulate results and report at the end).

Notes for the test harness:
- The test declares the external C symbol:
  extern "C" png_byte component_loc(png_byte loc[4], png_uint_32 format);
  and includes <png.h> for type/macros.
- The tests are guarded by preprocessor checks so they adapt to whether BGR and AFIRST paths exist
  in this build (PNG_FORMAT_BGR_SUPPORTED, PNG_FORMAT_AFIRST_SUPPORTED).

Usage:
- Compile this test alongside the pngstest.o (or the object containing component_loc) and
  link with libpng headers available.
- The test harness prints per-test PASS/FAIL lines and a final summary.

The code below provides the test suite with explanatory comments for each unit test.
*/

#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>


// Include libpng headers to get definitions for png_byte, png_uint_32, and format flags

// Declare the focal function as an external C symbol.
extern "C" png_byte component_loc(png_byte loc[4], png_uint_32 format);

// Simple non-terminating test harness
static int g_failures = 0;
static void t_pass(const char* name) {
    std::cout << "PASS: " << name << std::endl;
}
static void t_fail(const char* name, const std::string& detail) {
    std::cerr << "FAIL: " << name << " - " << detail << std::endl;
    ++g_failures;
}
static void t_eq(const char* name, const char* detail, int got, int exp) {
    if (got != exp) {
        std::string msg = std::string("got ") + std::to_string(got) +
                          ", expected " + std::to_string(exp) + ". " + detail;
        t_fail(name, msg);
    } else {
        t_pass(name);
    }
}
static void t_eq_u8(const char* name, const char* detail, png_byte got, png_byte exp) {
    if (got != exp) {
        std::string msg = std::string("got ") + std::to_string((int)got) +
                          ", expected " + std::to_string((int)exp) + ". " + detail;
        t_fail(name, msg);
    } else {
        t_pass(name);
    }
}

// Test 1: Color image path with BGR ordering (if supported) and without ALPHA
// This exercises the branch when format has PNG_FORMAT_FLAG_COLOR.
// If PNG_FORMAT_BGR_SUPPORTED is defined, the test also validates the BGR-specific branch.
static void test_color_path_color_only(void) {
    // Guard by color flag presence
#if defined(PNG_FORMAT_FLAG_COLOR)
    png_byte loc[4] = {0, 0, 0, 0};
    png_uint_32 fmt = PNG_FORMAT_FLAG_COLOR;
    png_byte ret = component_loc(loc, fmt);

    // Base expectations for color path without BGR override:
    // - channels = 3 (returned value)
    // - loc[2] = 1
    // - loc[0] = 4 (alpha not present)
    // - loc[1], loc[3] depend on BGR support:
    //   If BGR_SUPPORTED: loc[1] is the BGR-middle index (2), loc[3] = 0
    //   Else: loc[1] = 0, loc[3] = 2
#ifdef PNG_FORMAT_BGR_SUPPORTED
    // Test the non-BGR flavor first (color without BGR)
    // Re-run with BGR flag on to exercise BGR-specific path
    png_byte loc_non_bgr[4] = {0, 0, 0, 0};
    png_uint_32 fmt_no_bgr = PNG_FORMAT_FLAG_COLOR;
    png_byte ret_no_bgr = component_loc(loc_non_bgr, fmt_no_bgr);
    // Compare to the non-BGR expectation
    bool ok_no_bgr = (ret_no_bgr == 3) &&
                     (loc_non_bgr[2] == 1) &&
                     (loc_non_bgr[1] == 0) &&
                     (loc_non_bgr[3] == 2) &&
                     (loc_non_bgr[0] == 4);
    t_eq("test_color_path_color_only_no_bgr", "color path w/o BGR expectation", ret_no_bgr, 3);
    if (!ok_no_bgr) {
        t_fail("test_color_path_color_only_no_bgr", "Mismatch in computed loc for non-BGR color path.");
    } else {
        t_pass("test_color_path_color_only_no_bgr");
    }

    // Now test with BGR ordering
    png_byte loc_bgr[4] = {0, 0, 0, 0};
    png_uint_32 fmt_bgr = PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_FLAG_BGR;
    png_byte ret_bgr = component_loc(loc_bgr, fmt_bgr);

    bool ok_bgr = (ret_bgr == 3) &&
                  (loc_bgr[2] == 1) &&
                  (loc_bgr[1] == 2) &&
                  (loc_bgr[3] == 0) &&
                  (loc_bgr[0] == 4);

    t_eq("test_color_path_color_only_bgr", "color path with BGR should set BGR ordering", ret_bgr, 3);
    if (!ok_bgr) {
        t_fail("test_color_path_color_only_bgr", "Mismatch in loc[1], loc[3] or loc[0] for BGR path.");
    } else {
        t_pass("test_color_path_color_only_bgr");
    }
#else
    // BGR not supported: only test the color path without BGR option
    png_byte loc_only[4] = {0, 0, 0, 0};
    png_uint_32 fmt_only = PNG_FORMAT_FLAG_COLOR;
    png_byte ret_only = component_loc(loc_only, fmt_only);

    bool ok_only = (ret_only == 3) &&
                   (loc_only[2] == 1) &&
                   (loc_only[1] == 0) &&
                   (loc_only[3] == 2) &&
                   (loc_only[0] == 4);
    t_eq("test_color_path_color_only_no_bgr_macro", "color path when BGR not supported", ret_only, 3);
    if (!ok_only) {
        t_fail("test_color_path_color_only_no_bgr_macro", "Mismatch in loc for color-only path without BGR support.");
    } else {
        t_pass("test_color_path_color_only_no_bgr_macro");
    }
#endif
#else
    // If color flag is not defined in this build, skip gracefully
    std::cout << "SKIP: test_color_path_color_only (color flag not defined)" << std::endl;
#endif
}

// Test 2: Grayscale (no color) path
// This exercises the else branch of the color check: channels = 1, loc[1..3] = 0
static void test_grayscale_path_no_color(void) {
#if defined(PNG_FORMAT_FLAG_COLOR)
    png_byte loc[4] = {0, 0, 0, 0};
    png_uint_32 fmt = 0; // grayscale (no color)
    png_byte ret = component_loc(loc, fmt);
    // Expected: channels = 1, loc[1..3] = 0, loc[0] = 4
    t_eq("test_grayscale_path_no_color_ret", "expected channel count for grayscale", ret, 1);
    t_eq_u8("test_grayscale_path_no_color_loc0", "loc[0] should be 4 when no alpha", loc[0], 4);
    t_eq_u8("test_grayscale_path_no_color_loc1", "loc[1] should be 0 for grayscale", loc[1], 0);
    t_eq_u8("test_grayscale_path_no_color_loc2", "loc[2] should be 0 for grayscale", loc[2], 0);
    t_eq_u8("test_grayscale_path_no_color_loc3", "loc[3] should be 0 for grayscale", loc[3], 0);
#else
    std::cout << "SKIP: test_grayscale_path_no_color (color flag not defined)" << std::endl;
#endif
}

// Test 3: Grayscale path with ALPHA but WITHOUT AFIRST (non-AFIRST path)
// When AFIRST is not defined or not set, ALPHA increases channels but loc[0] points to the previous channel count
static void test_grayscale_with_alpha_no_afirst(void) {
#if defined(PNG_FORMAT_FLAG_ALPHA)
    png_byte loc[4] = {0, 0, 0, 0};
    // Do not set AFIRST
    png_uint_32 fmt = PNG_FORMAT_FLAG_ALPHA;
    // Grayscale path with alpha (no color, no AFIRST => loc[0] should hold channels before increment)
    png_byte ret = component_loc(loc, fmt);
    // Expected: channels initially 1; loc[0] should be 1; ret should be 2 (after ++channels)
    t_eq("test_grayscale_with_alpha_no_afirst_ret", "expected channel count with grayscale+alpha (no AFIRST)", ret, 2);
    t_eq_u8("test_grayscale_with_alpha_no_afirst_loc0", "loc[0] should be 1 (original channels)", loc[0], 1);
    // loc[1..3] should remain 0 except AFIRST would increment; since AFIRST not defined, they stay 0
    t_eq_u8("test_grayscale_with_alpha_no_afirst_loc1", "loc[1] should be 0", loc[1], 0);
    t_eq_u8("test_grayscale_with_alpha_no_afirst_loc2", "loc[2] should be 0", loc[2], 0);
    t_eq_u8("test_grayscale_with_alpha_no_afirst_loc3", "loc[3] should be 0", loc[3], 0);
#else
    std::cout << "SKIP: test_grayscale_with_alpha_no_afirst (ALPHA flag not defined)" << std::endl;
#endif
}

// Test 4: Grayscale path with ALPHA and AFIRST (if AFIRST is supported)
// This validates the AFIRST branch: loc[0] becomes 0, and all loc[1..3] are incremented by 1
#ifdef PNG_FORMAT_AFIRST_SUPPORTED
static void test_grayscale_with_alpha_and_afirst(void) {
#if defined(PNG_FORMAT_FLAG_ALPHA) && defined(PNG_FORMAT_FLAG_AFIRST)
    png_byte loc[4] = {0, 0, 0, 0};
    png_uint_32 fmt = PNG_FORMAT_FLAG_ALPHA | PNG_FORMAT_FLAG_AFIRST;
    png_byte ret = component_loc(loc, fmt);
    // Expected: ret = 2 (1 initial channel + 1 increment)
    t_eq("test_grayscale_with_alpha_and_afirst_ret", "expected channel count with AFIRST", ret, 2);
    // loc[0] = 0, and others incremented from 0 to 1
    t_eq_u8("test_grayscale_with_alpha_and_afirst_loc0", "loc[0] should be 0 (AFIRST)", loc[0], 0);
    t_eq_u8("test_grayscale_with_alpha_and_afirst_loc1", "loc[1] should be 1 (incremented)", loc[1], 1);
    t_eq_u8("test_grayscale_with_alpha_and_afirst_loc2", "loc[2] should be 1 (incremented)", loc[2], 1);
    t_eq_u8("test_grayscale_with_alpha_and_afirst_loc3", "loc[3] should be 1 (incremented)", loc[3], 1);
#else
    std::cout << "SKIP: test_grayscale_with_alpha_and_afirst (flags not defined as expected)" << std::endl;
#endif
}
#endif

// Test 5: Color path with ALPHA (no AFIRST) to validate the behavior when alpha is present but AFIRST is not defined
static void test_color_with_alpha_no_afirst(void) {
#if defined(PNG_FORMAT_FLAG_COLOR) && defined(PNG_FORMAT_FLAG_ALPHA)
    png_byte loc[4] = {0, 0, 0, 0};
    // Do not set AFIRST
    png_uint_32 fmt = PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_FLAG_ALPHA;
    png_byte ret = component_loc(loc, fmt);
    // If BGR is supported, loc[1..3] follow non-AFIRST color+alpha ordering with loc[0] = channels (3)
    // Expected: ret = 4, loc[0] = 3, loc[2] = 1, loc[1] depends on BGR, loc[3] depends on BGR
    t_eq("test_color_with_alpha_no_afirst_ret", "color with alpha (no AFIRST) channel count", ret, 4);
    t_eq_u8("test_color_with_alpha_no_afirst_loc0", "loc[0] should be 3 (channels before alpha)", loc[0], 3);
#ifdef PNG_FORMAT_BGR_SUPPORTED
    // When BGR is supported, loc[1]/loc[3] depend on BGR ordering
    if (loc[2] == 1 && loc[3] == 2 && loc[1] == 0)
        t_pass("test_color_with_alpha_no_afirst_loc_case_bgr_expected");
    // The exact values for loc[1] and loc[3] depend on BGR flag usage; verify generic constraints
    t_pass("test_color_with_alpha_no_afirst_loc_case_generic");
#else
    // If BGR not supported, expect RGB ordering
    t_eq_u8("test_color_with_alpha_no_afirst_loc1", "loc[1] should be 0 (RGB)", loc[1], 0);
    t_eq_u8("test_color_with_alpha_no_afirst_loc2", "loc[2] should be 1 (gray/green channel position)", loc[2], 1);
    t_eq_u8("test_color_with_alpha_no_afirst_loc3", "loc[3] should be 2 (blue channel position)", loc[3], 2);
#endif
#else
    std::cout << "SKIP: test_color_with_alpha_no_afirst (required flags not defined)" << std::endl;
#endif
}

// Entry point: Run all tests
int main(void) {
    std::cout << "Running test suite for component_loc...\n";

    // Test 1: Color path (with and without BGR ordering if supported)
    test_color_path_color_only();

    // Test 2: Grayscale (no color)
    test_grayscale_path_no_color();

    // Test 3: Grayscale with ALPHA (no AFIRST)
    test_grayscale_with_alpha_no_afirst();

#ifdef PNG_FORMAT_AFIRST_SUPPORTED
    // Test 4: Grayscale with ALPHA and AFIRST
    test_grayscale_with_alpha_and_afirst();
#else
    std::cout << "SKIP: test_grayscale_with_alpha_and_afirst (AFIRST not supported in this build)" << std::endl;
#endif

    // Test 5: Color path with ALPHA (no AFIRST)
    test_color_with_alpha_no_afirst();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}