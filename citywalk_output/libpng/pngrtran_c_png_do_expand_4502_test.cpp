/*
   Lightweight C++11 test suite for the png_do_expand focal routine.
   This file re-implements a minimal subset of the PNG plumbing needed
   to exercise png_do_expand without bringing in the full libpng build.
   It provides four targeted test cases that cover RGB and Gray paths
   with and without color expansion (trans_color), verifying both
   memory contents and updated metadata in png_row_infop.

   Notes:
   - This test is self-contained: the png_do_expand implementation is
     included here to ensure the test runs in isolation.
   - Tests use a tiny, explicit harness (no Google Test). Results are
     printed to stdout with PASS/FAIL counts.
   - Respect domain knowledge: access to static/internal things is avoided;
     only the tested function and its immediate types are used.
*/

#include <cassert>
#include <stdint.h>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <cstddef>


// Domain knowledge: minimal re-implementation of required libpng types/macros
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef const struct png_color_16* png_const_color_16p;
typedef unsigned int png_uint_32;
typedef unsigned int png_uint_16;
typedef unsigned char png_color_8;
typedef png_color_8 png_color_8p;
typedef struct png_row_info {
  unsigned int width;
  int color_type;
  int bit_depth;
  png_uint_32 rowbytes;
  int channels;
  int pixel_depth;
} png_row_info;
typedef png_row_info* png_row_infop;

#define PNG_COLOR_TYPE_GRAY 0
#define PNG_COLOR_TYPE_RGB 2
#define PNG_COLOR_TYPE_GRAY_ALPHA 4
#define PNG_COLOR_TYPE_RGB_ALPHA 6

// Macro to emulate libpng memory-byte layout helpers
#define PNG_ROWBYTES(pixel_depth, row_width) ((((row_width) * (pixel_depth)) + 7) / 8)

// Simple no-op debug stub (as in real libpng)
static inline void png_debug(int, const char*) { (void)0; }

// Minimal color struct used by png_do_expand
typedef struct png_color_16 {
  unsigned int gray;
  unsigned int red;
  unsigned int green;
  unsigned int blue;
} png_color_16;

// Forward declaration for the focal function
extern "C" void png_do_expand(png_row_infop row_info, png_bytep row,
    png_const_color_16p trans_color);

// Implementation copied/adapted from the provided focal method.
// This keeps behavior identical for testing purposes.
extern "C" void png_do_expand(png_row_infop row_info, png_bytep row,
    png_const_color_16p trans_color)
{
{
   int shift, value;
   png_bytep sp, dp;
   png_uint_32 i;
   png_uint_32 row_width=row_info->width;
   png_debug(1, "in png_do_expand");
   if (row_info->color_type == PNG_COLOR_TYPE_GRAY)
   {
      unsigned int gray = trans_color != NULL ? trans_color->gray : 0;
      if (row_info->bit_depth < 8)
      {
         switch (row_info->bit_depth)
         {
            case 1:
            {
               gray = (gray & 0x01) * 0xff;
               sp = row + (size_t)((row_width - 1) >> 3);
               dp = row + (size_t)row_width - 1;
               shift = 7 - (int)((row_width + 7) & 0x07);
               for (i = 0; i < row_width; i++)
               {
                  if ((*sp >> shift) & 0x01)
                     *dp = 0xff;
                  else
                     *dp = 0;
                  if (shift == 7)
                  {
                     shift = 0;
                     sp--;
                  }
                  else
                     shift++;
                  dp--;
               }
               break;
            }
            case 2:
            {
               gray = (gray & 0x03) * 0x55;
               sp = row + (size_t)((row_width - 1) >> 2);
               dp = row + (size_t)row_width - 1;
               shift = (int)((3 - ((row_width + 3) & 0x03)) << 1);
               for (i = 0; i < row_width; i++)
               {
                  value = (*sp >> shift) & 0x03;
                  *dp = (png_byte)(value | (value << 2) | (value << 4) |
                     (value << 6));
                  if (shift == 6)
                  {
                     shift = 0;
                     sp--;
                  }
                  else
                     shift += 2;
                  dp--;
               }
               break;
            }
            case 4:
            {
               gray = (gray & 0x0f) * 0x11;
               sp = row + (size_t)((row_width - 1) >> 1);
               dp = row + (size_t)row_width - 1;
               shift = (int)((1 - ((row_width + 1) & 0x01)) << 2);
               for (i = 0; i < row_width; i++)
               {
                  value = (*sp >> shift) & 0x0f;
                  *dp = (png_byte)(value | (value << 4));
                  if (shift == 4)
                  {
                     shift = 0;
                     sp--;
                  }
                  else
                     shift = 4;
                  dp--;
               }
               break;
            }
            default:
               break;
         }
         row_info->bit_depth = 8;
         row_info->pixel_depth = 8;
         row_info->rowbytes = row_width;
      }
      if (trans_color != NULL)
      {
         if (row_info->bit_depth == 8)
         {
            gray = gray & 0xff;
            sp = row + (size_t)row_width - 1;
            dp = row + ((size_t)row_width << 1) - 1;
            for (i = 0; i < row_width; i++)
            {
               if ((*sp & 0xffU) == gray)
                  *dp-- = 0;
               else
                  *dp-- = 0xff;
               *dp-- = *sp--;
            }
         }
         else if (row_info->bit_depth == 16)
         {
            unsigned int gray_high = (gray >> 8) & 0xff;
            unsigned int gray_low = gray & 0xff;
            sp = row + row_info->rowbytes - 1;
            dp = row + (row_info->rowbytes << 1) - 1;
            for (i = 0; i < row_width; i++)
            {
               if ((*(sp - 1) & 0xffU) == gray_high &&
                   (*(sp) & 0xffU) == gray_low)
               {
                  *dp-- = 0;
                  *dp-- = 0;
               }
               else
               {
                  *dp-- = 0xff;
                  *dp-- = 0xff;
               }
               *dp-- = *sp--;
               *dp-- = *sp--;
            }
         }
         row_info->color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
         row_info->channels = 2;
         row_info->pixel_depth = (png_byte)(row_info->bit_depth << 1);
         row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth,
             row_width);
      }
   }
   else if (row_info->color_type == PNG_COLOR_TYPE_RGB &&
       trans_color != NULL)
   {
      if (row_info->bit_depth == 8)
      {
         png_byte red = (png_byte)(trans_color->red & 0xff);
         png_byte green = (png_byte)(trans_color->green & 0xff);
         png_byte blue = (png_byte)(trans_color->blue & 0xff);
         sp = row + (size_t)row_info->rowbytes - 1;
         dp = row + ((size_t)row_width << 2) - 1;
         for (i = 0; i < row_width; i++)
         {
            if (*(sp - 2) == red && *(sp - 1) == green && *(sp) == blue)
               *dp-- = 0;
            else
               *dp-- = 0xff;
            *dp-- = *sp--;
            *dp-- = *sp--;
            *dp-- = *sp--;
         }
      }
      else if (row_info->bit_depth == 16)
      {
         png_byte red_high = (png_byte)((trans_color->red >> 8) & 0xff);
         png_byte green_high = (png_byte)((trans_color->green >> 8) & 0xff);
         png_byte blue_high = (png_byte)((trans_color->blue >> 8) & 0xff);
         png_byte red_low = (png_byte)(trans_color->red & 0xff);
         png_byte green_low = (png_byte)(trans_color->green & 0xff);
         png_byte blue_low = (png_byte)(trans_color->blue & 0xff);
         sp = row + row_info->rowbytes - 1;
         dp = row + ((size_t)row_width << 3) - 1;
         for (i = 0; i < row_width; i++)
         {
            if (*(sp - 5) == red_high &&
                *(sp - 4) == red_low &&
                *(sp - 3) == green_high &&
                *(sp - 2) == green_low &&
                *(sp - 1) == blue_high &&
                *(sp    ) == blue_low)
            {
               *dp-- = 0;
               *dp-- = 0;
            }
            else
            {
               *dp-- = 0xff;
               *dp-- = 0xff;
            }
            *dp-- = *sp--;
            *dp-- = *sp--;
            *dp-- = *sp--;
            *dp-- = *sp--;
            *dp-- = *sp--;
            *dp-- = *sp--;
         }
      }
      row_info->color_type = PNG_COLOR_TYPE_RGB_ALPHA;
      row_info->channels = 4;
      row_info->pixel_depth = (png_byte)(row_info->bit_depth << 2);
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, row_width);
   }
}
}

// End of focal method re-implementation
// Note: The above function assumes the tests provide properly allocated buffers.


// Simple test harness (no GTest). Keeps PASS/FAIL tallies.
static int g_passed = 0;
static int g_failed = 0;

static void test_pass(const char* name) {
    printf("[TEST PASS] %s\n", name);
    ++g_passed;
}
static void test_fail(const char* name, const char* detail) {
    printf("[TEST FAIL] %s -- %s\n", name, detail);
    ++g_failed;
}

// Helper to compare memory blocks
static bool memeq(const unsigned char* a, const unsigned char* b, size_t n) {
    return std::memcmp(a, b, n) == 0;
}

// Test 1: RGB 8-bit with trans_color matching -> expect RGBA = [R,G,B,0]
static void run_test_rgb8_match() {
    const char* testname = "png_do_expand_RGB8_match_trans";
    // Prepare row_info for 1-pixel RGB
    png_row_info ri;
    ri.width = 1;
    ri.color_type = PNG_COLOR_TYPE_RGB;
    ri.bit_depth = 8;
    ri.rowbytes = 3; // input rowbytes (R,G,B)
    ri.channels = 3;
    ri.pixel_depth = 24;

    unsigned char row[4] = {0x11, 0x22, 0x33, 0x00}; // input: R,G,B
    unsigned char expected_row[4] = {0x11, 0x22, 0x33, 0x00};

    png_color_16 trans;
    trans.gray = 0; trans.red = 0x11; trans.green = 0x22; trans.blue = 0x33;
    png_do_expand(&ri, row, &trans);

    // Expect memory [R,G,B,A] with A=0 for match
    unsigned char exp[4] = {0x11, 0x22, 0x33, 0x00};
    if (memeq(row, exp, 4) &&
        ri.color_type == PNG_COLOR_TYPE_RGB_ALPHA &&
        ri.channels == 4 &&
        ri.rowbytes == 4) {
        test_pass(testname);
    } else {
        test_fail(testname, "unexpected expanded pixel data or metadata.");
        printf("Expected: {11,22,33,00}, Got: {");
        for (int i=0;i<4;i++){ printf("%02x%s", row[i], i<3?",":""); }
        printf("}\n");
    }
}

// Test 2: RGB 8-bit with trans_color not matching -> expect RGBA = [R,G,B,0xff]
static void run_test_rgb8_mismatch() {
    const char* testname = "png_do_expand_RGB8_mismatch_trans";
    png_row_info ri;
    ri.width = 1;
    ri.color_type = PNG_COLOR_TYPE_RGB;
    ri.bit_depth = 8;
    ri.rowbytes = 3;
    ri.channels = 3;
    ri.pixel_depth = 24;

    unsigned char row[4] = {0x11, 0x22, 0x34, 0x00}; // input: R,G,B
    unsigned char expected_row[4] = {0x11, 0x22, 0x34, 0xFF};

    png_color_16 trans;
    trans.gray = 0; trans.red = 0x11; trans.green = 0x22; trans.blue = 0x33; // not matching B
    png_do_expand(&ri, row, &trans);

    unsigned char exp[4] = {0x11, 0x22, 0x34, 0xFF};
    if (memeq(row, exp, 4) &&
        ri.color_type == PNG_COLOR_TYPE_RGB_ALPHA &&
        ri.channels == 4 &&
        ri.rowbytes == 4) {
        test_pass(testname);
    } else {
        test_fail(testname, "unexpected expanded pixel data or metadata.");
        printf("Expected: {11,22,34,ff}, Got: {");
        for (int i=0;i<4;i++){ printf("%02x%s", row[i], i<3?",":""); }
        printf("}\n");
    }
}

// Test 3: Gray 8-bit with trans_color matching -> expect GrayAlpha [G,0]
static void run_test_gray8_match() {
    const char* testname = "png_do_expand_GRAY8_match_trans";
    png_row_info ri;
    ri.width = 1;
    ri.color_type = PNG_COLOR_TYPE_GRAY;
    ri.bit_depth = 8;
    ri.rowbytes = 1;
    ri.channels = 1;
    ri.pixel_depth = 8;

    unsigned char row[2] = {0x66, 0x00}; // input: G
    // After expansion, expect two bytes: Gray then Alpha
    png_color_16 trans;
    trans.gray = 0x66; trans.red = 0; trans.green = 0; trans.blue = 0;
    png_do_expand(&ri, row, &trans);

    unsigned char exp[2] = {0x66, 0x00};
    if (memeq(row, exp, 2) &&
        ri.color_type == PNG_COLOR_TYPE_GRAY_ALPHA &&
        ri.channels == 2 &&
        ri.rowbytes == PNG_ROWBYTES(ri.bit_depth << 1, 1)) {
        test_pass(testname);
    } else {
        test_fail(testname, "unexpected GrayAlpha data or metadata.");
        printf("Got: {");
        for (int i=0;i<2;i++){ printf("%02x%s", row[i], i<1?",":""); }
        printf("}\n");
    }
}

// Test 4: Gray 8-bit with trans_color not matching -> expect GrayAlpha [G,0xff]
static void run_test_gray8_mismatch() {
    const char* testname = "png_do_expand_GRAY8_mismatch_trans";
    png_row_info ri;
    ri.width = 1;
    ri.color_type = PNG_COLOR_TYPE_GRAY;
    ri.bit_depth = 8;
    ri.rowbytes = 1;
    ri.channels = 1;
    ri.pixel_depth = 8;

    unsigned char row[2] = {0x77, 0x00}; // input: G
    png_color_16 trans;
    trans.gray = 0x66; trans.red = 0; trans.green = 0; trans.blue = 0;
    png_do_expand(&ri, row, &trans);

    unsigned char exp[2] = {0x77, 0xff};
    if (memeq(row, exp, 2) &&
        ri.color_type == PNG_COLOR_TYPE_GRAY_ALPHA &&
        ri.channels == 2 &&
        ri.rowbytes == PNG_ROWBYTES(ri.bit_depth << 1, 1)) {
        test_pass(testname);
    } else {
        test_fail(testname, "unexpected GrayAlpha data or metadata for mismatch case.");
        printf("Got: {");
        for (int i=0;i<2;i++){ printf("%02x%s", row[i], i<1?",":""); }
        printf("}\n");
    }
}

int main() {
    // Run tests
    run_test_rgb8_match();
    run_test_rgb8_mismatch();
    run_test_gray8_match();
    run_test_gray8_mismatch();

    // Summary
    printf("\nTest Summary: Passed %d, Failed %d\n", g_passed, g_failed);
    return (g_failed == 0) ? 0 : 1;
}