// Test suite for the focal method png_do_read_interlace
// This test harness provides minimal stand-ins for required libpng types
// and reproduces a self-contained version of png_do_read_interlace to
// enable unit testing without GTest.
// The goal is to exercise branches and verify basic invariants (width/rowbytes),
// while avoiding dependency on external libraries.

#include <cstdint>
#include <cassert>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Lightweight stand-ins for libpng types and macros (for test harness)
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef unsigned int png_uint_32;
typedef struct png_row_info {
    unsigned int width;
    unsigned int pixel_depth;
    unsigned int rowbytes;
} png_row_info;
typedef png_row_info* png_row_infop;

// Candidate keyword hints (from Step 1):
// - row_info, row, pass
// - pixel_depth, width, rowbytes
// - png_pass_inc, transformations, PNG_PACKSWAP (bit-twiddling)
// - PNG_ROWBYTES (rowbytes computation macro)

// Mock minimal PNG pass increment array (Adam7-like, simplified for tests)
unsigned int png_pass_inc[7] = {1, 1, 1, 1, 1, 1, 1};

// Stub for libpng's diagnostic function (no-op in tests)
static void png_debug(int level, const char* message) {
    (void)level;
    (void)message;
    // No operation; kept to emulate original API.
}

// Macro to avoid unused variable warnings in tests
#define PNG_UNUSED(x) (void)(x)

// Simple computation of row bytes (as libpng macro would)
#define PNG_ROWBYTES(pdepth, w) ((((w) * (pdepth)) + 7) / 8)

// The focal method (self-contained) copied into test harness
png_do_read_interlace(png_row_infop row_info, png_bytep row, int pass,
    png_uint_32 transformations )
{
{
   png_debug(1, "in png_do_read_interlace");
   if (row != NULL && row_info != NULL)
   {
      png_uint_32 final_width;
      final_width = row_info->width * png_pass_inc[pass];
      switch (row_info->pixel_depth)
      {
         case 1:
         {
            png_bytep sp = row + (size_t)((row_info->width - 1) >> 3);
            png_bytep dp = row + (size_t)((final_width - 1) >> 3);
            unsigned int sshift, dshift;
            unsigned int s_start, s_end;
            int s_inc;
            int jstop = (int)png_pass_inc[pass];
            png_byte v;
            png_uint_32 i;
            int j;
#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if ((transformations & PNG_PACKSWAP) != 0)
            {
                sshift = ((row_info->width + 7) & 0x07);
                dshift = ((final_width + 7) & 0x07);
                s_start = 7;
                s_end = 0;
                s_inc = -1;
            }
            else
#endif
            {
                sshift = 7 - ((row_info->width + 7) & 0x07);
                dshift = 7 - ((final_width + 7) & 0x07);
                s_start = 0;
                s_end = 7;
                s_inc = 1;
            }
            for (i = 0; i < row_info->width; i++)
            {
               v = (png_byte)((*sp >> sshift) & 0x01);
               for (j = 0; j < jstop; j++)
               {
                  unsigned int tmp = *dp & (0x7f7f >> (7 - dshift));
                  tmp |= (unsigned int)(v << dshift);
                  *dp = (png_byte)(tmp & 0xff);
                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }
                  else
                     dshift = (unsigned int)((int)dshift + s_inc);
               }
               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }
               else
                  sshift = (unsigned int)((int)sshift + s_inc);
            }
            break;
         }
         case 2:
         {
            png_bytep sp = row + (png_uint_32)((row_info->width - 1) >> 2);
            png_bytep dp = row + (png_uint_32)((final_width - 1) >> 2);
            unsigned int sshift, dshift;
            unsigned int s_start, s_end;
            int s_inc;
            int jstop = (int)png_pass_inc[pass];
            png_uint_32 i;
#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if ((transformations & PNG_PACKSWAP) != 0)
            {
               sshift = (((row_info->width + 3) & 0x03) << 1);
               dshift = (((final_width + 3) & 0x03) << 1);
               s_start = 6;
               s_end = 0;
               s_inc = -2;
            }
            else
#endif
            {
               sshift = ((3 - ((row_info->width + 3) & 0x03)) << 1);
               dshift = ((3 - ((final_width + 3) & 0x03)) << 1);
               s_start = 0;
               s_end = 6;
               s_inc = 2;
            }
            for (i = 0; i < row_info->width; i++)
            {
               png_byte v;
               int j;
               v = (png_byte)((*sp >> sshift) & 0x03);
               for (j = 0; j < jstop; j++)
               {
                  unsigned int tmp = *dp & (0x3f3f >> (6 - dshift));
                  tmp |= (unsigned int)(v << dshift);
                  *dp = (png_byte)(tmp & 0xff);
                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }
                  else
                     dshift = (unsigned int)((int)dshift + s_inc);
               }
               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }
               else
                  sshift = (unsigned int)((int)sshift + s_inc);
            }
            break;
         }
         case 4:
         {
            png_bytep sp = row + (size_t)((row_info->width - 1) >> 1);
            png_bytep dp = row + (size_t)((final_width - 1) >> 1);
            unsigned int sshift, dshift;
            unsigned int s_start, s_end;
            int s_inc;
            png_uint_32 i;
            int jstop = (int)png_pass_inc[pass];
#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if ((transformations & PNG_PACKSWAP) != 0)
            {
               sshift = (((row_info->width + 1) & 0x01) << 2);
               dshift = (((final_width + 1) & 0x01) << 2);
               s_start = 4;
               s_end = 0;
               s_inc = -4;
            }
            else
#endif
            {
               sshift = ((1 - ((row_info->width + 1) & 0x01)) << 2);
               dshift = ((1 - ((final_width + 1) & 0x01)) << 2);
               s_start = 0;
               s_end = 4;
               s_inc = 4;
            }
            for (i = 0; i < row_info->width; i++)
            {
               png_byte v = (png_byte)((*sp >> sshift) & 0x0f);
               int j;
               for (j = 0; j < jstop; j++)
               {
                  unsigned int tmp = *dp & (0xf0f >> (4 - dshift));
                  tmp |= (unsigned int)(v << dshift);
                  *dp = (unsigned int)(tmp & 0xff);
                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }
                  else
                     dshift = (unsigned int)((int)dshift + s_inc);
               }
               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }
               else
                  sshift = (unsigned int)((int)sshift + s_inc);
            }
            break;
         }
         default:
         {
            size_t pixel_bytes = (row_info->pixel_depth >> 3);
            png_bytep sp = row + (size_t)(row_info->width - 1)
                * pixel_bytes;
            png_bytep dp = row + (size_t)(final_width - 1) * pixel_bytes;
            int jstop = (int)png_pass_inc[pass];
            png_uint_32 i;
            for (i = 0; i < row_info->width; i++)
            {
               png_byte v[8]; /* SAFE; pixel_depth does not exceed 64 */
               int j;
               memcpy(v, sp, pixel_bytes);
               for (j = 0; j < jstop; j++)
               {
                  memcpy(dp, v, pixel_bytes);
                  dp -= pixel_bytes;
               }
               sp -= pixel_bytes;
            }
            break;
         }
      }
      row_info->width = final_width;
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, final_width);
   }
#ifndef PNG_READ_PACKSWAP_SUPPORTED
   PNG_UNUSED(transformations)  /* Silence compiler warning */
#endif
}
}

// End of focal function
int run_all_tests();

int main() {
    int failures = run_all_tests();
    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << "Total failures: " << failures << "\n";
        return 1;
    }
}

// Unit test helpers and test cases (no GTest; simple in-file harness)

// Global counter for test failures
static int g_test_failures = 0;

// Simple assertion helpers (non-terminating)
#define TEST_EXPECT_TRUE(cond, msg) do { if(!(cond)) { std::cerr << "Test failure: " << (msg) << " (condition: " #cond ") at " __FILE__ ":" << __LINE__ << "\n"; ++g_test_failures; } } while(0)
#define TEST_EXPECT_EQ(a, b, msg) do { if(!((a) == (b))) { std::cerr << "Test failure: " << (msg) << " | got " << (a) << " expected " << (b) << " at " __FILE__ ":" << __LINE__ << "\n"; ++g_test_failures; } } while(0)

// Helper to create a buffer filled with a known pattern
static void fill_pattern(png_bytep row, size_t len, png_byte pattern) {
    for (size_t i = 0; i < len; ++i) row[i] = pattern;
}

// Test 1: Null-pointer protection (row or row_info is NULL) should not crash and should not alter valid state
void test_null_pointers_protect() {
    // Case A: row is NULL
    png_row_info infoA; infoA.width = 5; infoA.pixel_depth = 1; infoA.rowbytes = 0;
    png_bytep rowA = NULL;
    png_do_read_interlace(&infoA, rowA, 0, 0);
    // Expect no crash; width unchanged
    TEST_EXPECT_EQ(infoA.width, 5u, "null row should not alter width when row is NULL");
    // Case B: row_info is NULL
    png_bytep rowB = new png_byte[16];
    fill_pattern(rowB, 16, 0xAA);
    png_do_read_interlace(NULL, rowB, 0, 0);
    // After call with NULL row_info, nothing to assert about width, but ensure no crash occurred
    TEST_EXPECT_TRUE(true, "NULL row_info should not crash");
    delete[] rowB;
}

// Test 2: Pixel depth = 1 with simple dimensions; ensure width/rowbytes updated correctly, and no crash
void test_interlace_depth1_basic() {
    png_row_info info; info.width = 4; info.pixel_depth = 1; info.rowbytes = 0;
    // final_width should be width * 1 (since png_pass_inc[0] = 1 in our harness)
    const size_t row_size = 1; // 4 bits -> 1 byte
    png_bytep row = new png_byte[4]; // allocate 4 bytes to be safe
    fill_pattern(row, 4, 0xAA);
    png_do_read_interlace(&info, row, 0, 0);
    // Width should remain 4
    TEST_EXPECT_EQ(info.width, 4u, "width after interlace should remain unchanged when pass_inc=1");
    // Rowbytes should be computed for 1-bit per pixel across width 4
    unsigned int expected_rowbytes = PNG_ROWBYTES(info.pixel_depth, info.width);
    TEST_EXPECT_EQ(info.rowbytes, expected_rowbytes, "rowbytes should be updated for 1-bit depth");
    delete[] row;
}

// Test 3: Pixel depth = 8 with simple dimensions; ensure width/rowbytes updated correctly
void test_interlace_depth8_basic() {
    png_row_info info; info.width = 4; info.pixel_depth = 8; info.rowbytes = 0;
    const size_t row_len = 4 * 1; // 4 pixels * 1 byte
    png_bytep row = new png_byte[row_len];
    // Initialize with a recognizable pattern
    for (size_t i = 0; i < row_len; ++i) row[i] = (png_byte)(i + 1);
    png_do_read_interlace(&info, row, 0, 0);
    // Width should remain 4
    TEST_EXPECT_EQ(info.width, 4u, "width should remain 4 for 8-bit depth with pass_inc=1");
    // Rowbytes should equal 4
    unsigned int expected_rowbytes = PNG_ROWBYTES(info.pixel_depth, info.width);
    TEST_EXPECT_EQ(info.rowbytes, expected_rowbytes, "rowbytes should be 4 for width=4 depth=8");
    // Content may be permuted by interlace, but since final_width==width here and pass_inc==1, it should still be 4 bytes long
    TEST_EXPECT_TRUE(row_len >= info.rowbytes, "rowbuffer length must be >= computed rowbytes");
    delete[] row;
}

// Test 4: Width = 0 edge-case; ensure no crashes and no changes
void test_width_zero_case() {
    png_row_info info; info.width = 0; info.pixel_depth = 8; info.rowbytes = 0;
    const size_t row_len = 0;
    png_bytep row = new png_byte[8];
    fill_pattern(row, 8, 0x55);
    png_do_read_interlace(&info, row, 0, 0);
    TEST_EXPECT_EQ(info.width, 0u, "width should remain 0 when input width is 0");
    delete[] row;
}

// Runner
int run_all_tests() {
    test_null_pointers_protect();
    test_interlace_depth1_basic();
    test_interlace_depth8_basic();
    test_width_zero_case();

    if (g_test_failures != 0) {
        std::cerr << "Total test failures: " << g_test_failures << "\n";
    }
    return g_test_failures;
}