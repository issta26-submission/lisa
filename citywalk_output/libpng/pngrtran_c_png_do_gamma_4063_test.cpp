// This test suite provides high-level unit tests for the focal method png_do_gamma.
// It includes a minimal re-implementation of the necessary PNG data structures and
// a localized copy of the png_do_gamma function (as provided) to allow compilation
// without external dependencies. The tests cover several branches and scenarios
// (8-bit RGB/RGBA, 16-bit RGB, Gray types, and null gamma tables) to exercise
// true/false branches of the predicate and color-channel handling.
// Explanatory comments accompany each test case.

// Note: This file is designed to be runnable in a C++11 environment without GTest.
// It uses a lightweight assertion mechanism and prints results to stdout.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// ------------------------ Minimal PNG-like Types ------------------------

// Public-domain-like minimal type aliases to mirror the focal function's usage.
typedef unsigned char png_byte;
typedef unsigned char* png_bytep;
typedef const unsigned char* png_const_bytep;

typedef const uint16_t* png_const_uint_16p;
typedef const uint16_t* const * png_const_uint_16pp; // pointer to array of pointers to uint16

typedef uint16_t png_uint_16;
typedef uint32_t png_uint_32;

struct png_struct {
    png_const_bytep gamma_table;       // identity mapping by default (8-bit)
    png_const_uint_16pp gamma_16_table; // 16-bit gamma table (2D: [first_dim][second_dim])
    int gamma_shift;                    // shift value used in indexing gamma_16_table
};

// Opaque pointer style used by the focal function
typedef png_struct* png_structrp;

// Row information structure (as used by png_do_gamma)
struct png_row_info_struct {
    png_uint_32 width;
    int bit_depth;
    int color_type;
};

// Pointers to the row info type
typedef struct png_row_info_struct* png_row_infop;

// Color type constants (subset used by tests)
#define PNG_COLOR_TYPE_GRAY        0
#define PNG_COLOR_TYPE_RGB           2
#define PNG_COLOR_TYPE_GRAY_ALPHA    4
#define PNG_COLOR_TYPE_RGB_ALPHA     6

// Debug stub (no-op)
static void png_debug(int, const char*) {}

// Focal method under test (adapted for in-file testing)
void png_do_gamma(png_row_infop row_info, png_bytep row, png_structrp png_ptr)
{
{
   png_const_bytep gamma_table = png_ptr->gamma_table;
   png_const_uint_16pp gamma_16_table = png_ptr->gamma_16_table;
   int gamma_shift = png_ptr->gamma_shift;
   png_bytep sp;
   png_uint_32 i;
   png_uint_32 row_width=row_info->width;
   png_debug(1, "in png_do_gamma");
   if (((row_info->bit_depth <= 8 && gamma_table != NULL) ||
       (row_info->bit_depth == 16 && gamma_16_table != NULL)))
   {
      switch (row_info->color_type)
      {
         case PNG_COLOR_TYPE_RGB:
         {
            if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp++;
                  *sp = gamma_table[*sp];
                  sp++;
                  *sp = gamma_table[*sp];
                  sp++;
               }
            }
            else /* if (row_info->bit_depth == 16) */
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  png_uint_16 v;
                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;
                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;
                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;
               }
            }
            break;
         }
         case PNG_COLOR_TYPE_RGB_ALPHA:
         {
            if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp++;
                  *sp = gamma_table[*sp];
                  sp++;
                  *sp = gamma_table[*sp];
                  sp++;
                  sp++;
               }
            }
            else /* if (row_info->bit_depth == 16) */
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  png_uint_16 v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;
                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;
                  v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 4;
               }
            }
            break;
         }
         case PNG_COLOR_TYPE_GRAY_ALPHA:
         {
            if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp += 2;
               }
            }
            else /* if (row_info->bit_depth == 16) */
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  png_uint_16 v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 4;
               }
            }
            break;
         }
         case PNG_COLOR_TYPE_GRAY:
         {
            if (row_info->bit_depth == 2)
            {
               sp = row;
               for (i = 0; i < row_width; i += 4)
               {
                  int a = *sp & 0xc0;
                  int b = *sp & 0x30;
                  int c = *sp & 0x0c;
                  int d = *sp & 0x03;
                  *sp = (png_byte)(
                      ((((int)gamma_table[a|(a>>2)|(a>>4)|(a>>6)])   ) & 0xc0)|
                      ((((int)gamma_table[(b<<2)|b|(b>>2)|(b>>4)])>>2) & 0x30)|
                      ((((int)gamma_table[(c<<4)|(c<<2)|c|(c>>2)])>>4) & 0x0c)|
                      ((((int)gamma_table[(d<<6)|(d<<4)|(d<<2)|d])>>6) ));
                  sp++;
               }
            }
            if (row_info->bit_depth == 4)
            {
               sp = row;
               for (i = 0; i < row_width; i += 2)
               {
                  int msb = *sp & 0xf0;
                  int lsb = *sp & 0x0f;
                  *sp = (png_byte)((((int)gamma_table[msb | (msb >> 4)]) & 0xf0)
                      | (((int)gamma_table[(lsb << 4) | lsb]) >> 4));
                  sp++;
               }
            }
            else if (row_info->bit_depth == 8)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  *sp = gamma_table[*sp];
                  sp++;
               }
            }
            else if (row_info->bit_depth == 16)
            {
               sp = row;
               for (i = 0; i < row_width; i++)
               {
                  png_uint_16 v = gamma_16_table[*(sp + 1) >> gamma_shift][*sp];
                  *sp = (png_byte)((v >> 8) & 0xff);
                  *(sp + 1) = (png_byte)(v & 0xff);
                  sp += 2;
               }
            }
            break;
         }
         default:
            break;
      }
   }
}
}

// ------------------------ Lightweight Test Harness ------------------------

// Simple assertion helper
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) << "\n"; \
        ++g_failures; \
    } else { \
        std::cout << "ASSERT PASSED: " << (msg) << "\n"; \
    } \
} while(0)

// Global failure counter
static int g_failures = 0;

// Helper: compare two memory buffers
static bool mem_equal(const unsigned char* a, const unsigned char* b, size_t n) {
    return std::memcmp(a, b, n) == 0;
}

// Entry point
int main() {
    std::cout << "Starting tests for png_do_gamma...\n";

    // Common constants
    const int COLOR_RGB = PNG_COLOR_TYPE_RGB;
    const int COLOR_RGB_ALPHA = PNG_COLOR_TYPE_RGB_ALPHA;
    const int COLOR_GRAY = PNG_COLOR_TYPE_GRAY;
    const int COLOR_GRAY_ALPHA = PNG_COLOR_TYPE_GRAY_ALPHA;

    // Test 1: 8-bit RGB, gamma_table identity -> output equals input
    {
        unsigned char gamma_identity[256];
        for (int i = 0; i < 256; ++i) gamma_identity[i] = static_cast<unsigned char>(i);

        png_struct png_ptr;
        png_ptr.gamma_table = gamma_identity;
        png_ptr.gamma_16_table = nullptr;
        png_ptr.gamma_shift = 0;

        png_row_info_struct row_info;
        row_info.width = 2;       // two pixels
        row_info.bit_depth = 8;
        row_info.color_type = COLOR_RGB;

        unsigned char row[6] = {1, 2, 3, 4, 5, 6};
        unsigned char row_in[6];
        std::memcpy(row_in, row, 6);

        png_do_gamma(&row_info, row, &png_ptr);

        bool ok = mem_equal(row, row_in, 6);
        ASSERT_TRUE(ok, "Test1 RGB8 identity gamma_table leaves row unchanged");
    }

    // Test 2: 8-bit RGB, gamma_table invert -> each channel inverted
    {
        unsigned char gamma_invert[256];
        for (int i = 0; i < 256; ++i) gamma_invert[i] = static_cast<unsigned char>(255 - i);

        png_struct png_ptr;
        png_ptr.gamma_table = gamma_invert;
        png_ptr.gamma_16_table = nullptr;
        png_ptr.gamma_shift = 0;

        png_row_info_struct row_info;
        row_info.width = 1;       // one pixel
        row_info.bit_depth = 8;
        row_info.color_type = COLOR_RGB;

        unsigned char row[3] = {10, 20, 30};
        unsigned char expected[3] = {245, 235, 225};

        png_do_gamma(&row_info, row, &png_ptr);

        bool ok = mem_equal(row, expected, 3);
        ASSERT_TRUE(ok, "Test2 RGB8 invert gamma_table maps to 255-channel complements");
    }

    // Test 3: 8-bit RGBA, gamma_table identity -> only RGB channels affected, alpha preserved
    {
        unsigned char gamma_identity[256];
        for (int i = 0; i < 256; ++i) gamma_identity[i] = static_cast<unsigned char>(i);

        png_struct png_ptr;
        png_ptr.gamma_table = gamma_identity;
        png_ptr.gamma_16_table = nullptr;
        png_ptr.gamma_shift = 0;

        png_row_info_struct row_info;
        row_info.width = 1;       // one pixel
        row_info.bit_depth = 8;
        row_info.color_type = COLOR_RGB_ALPHA;

        unsigned char row[4] = {5, 6, 7, 0xAA}; // RGBA
        unsigned char expected[4] = {5, 6, 7, 0xAA}; // A should remain unchanged

        png_do_gamma(&row_info, row, &png_ptr);

        bool ok = mem_equal(row, expected, 4);
        ASSERT_TRUE(ok, "Test3 RGBA8 gamma_table identity leaves RGB unchanged; Alpha preserved");
    }

    // Test 4: 16-bit RGB, gamma_16_table present with simple identity-like mapping
    {
        // gamma_16_table[0][k] = (k<<8) | k
        static uint16_t gamma16_tab0[256];
        for (int k = 0; k < 256; ++k) gamma16_tab0[k] = static_cast<uint16_t>((k << 8) | k);

        static const uint16_t* gamma16_table_rows[1] = { gamma16_tab0 };
        png_const_uint_16pp gamma16_table = gamma16_table_rows;

        png_struct png_ptr;
        png_ptr.gamma_table = nullptr; // 8-bit gamma not used
        png_ptr.gamma_16_table = gamma16_table;
        png_ptr.gamma_shift = 0;

        png_row_info_struct row_info;
        row_info.width = 3;       // three pixels
        row_info.bit_depth = 16;
        row_info.color_type = COLOR_RGB;

        // Row order: R=(0x11,0x11), G=(0x22,0x22), B=(0x33,0x33)
        unsigned char row[6] = {0x11,0x11, 0x22,0x22, 0x33,0x33};
        unsigned char expected[6] = {0x11,0x11, 0x22,0x22, 0x33,0x33};

        png_do_gamma(&row_info, row, &png_ptr);

        bool ok = mem_equal(row, expected, 6);
        ASSERT_TRUE(ok, "Test4 RGB16 gamma_16_table maps channels via 0th row to identical bytes (identity-like mapping)");
    }

    // Test 5: 8-bit Gray, gamma_table identity -> each gray sample unchanged
    {
        unsigned char gamma_identity[256];
        for (int i = 0; i < 256; ++i) gamma_identity[i] = static_cast<unsigned char>(i);

        png_struct png_ptr;
        png_ptr.gamma_table = gamma_identity;
        png_ptr.gamma_16_table = nullptr;
        png_ptr.gamma_shift = 0;

        png_row_info_struct row_info;
        row_info.width = 4;       // four grayscale samples
        row_info.bit_depth = 8;
        row_info.color_type = COLOR_GRAY;

        unsigned char row[4] = {0, 128, 64, 255};
        unsigned char expected[4] = {0, 128, 64, 255};

        png_do_gamma(&row_info, row, &png_ptr);

        bool ok = mem_equal(row, expected, 4);
        ASSERT_TRUE(ok, "Test5 Gray8 identity gamma leaves all samples unchanged");
    }

    // Test 6: 8-bit Gray with gamma_table invert -> each sample inverted
    {
        unsigned char gamma_invert[256];
        for (int i = 0; i < 256; ++i) gamma_invert[i] = static_cast<unsigned char>(255 - i);

        png_struct png_ptr;
        png_ptr.gamma_table = gamma_invert;
        png_ptr.gamma_16_table = nullptr;
        png_ptr.gamma_shift = 0;

        png_row_info_struct row_info;
        row_info.width = 4;
        row_info.bit_depth = 8;
        row_info.color_type = COLOR_GRAY;

        unsigned char row[4] = {10, 20, 30, 40};
        unsigned char expected[4] = {245, 235, 225, 215};

        png_do_gamma(&row_info, row, &png_ptr);

        bool ok = mem_equal(row, expected, 4);
        ASSERT_TRUE(ok, "Test6 Gray8 invert gamma_table maps to 255- value");
    }

    // Test 7: 8-bit RGB with gamma_table NULL -> no change
    {
        png_struct png_ptr;
        png_ptr.gamma_table = nullptr;
        png_ptr.gamma_16_table = nullptr;
        png_ptr.gamma_shift = 0;

        png_row_info_struct row_info;
        row_info.width = 2;
        row_info.bit_depth = 8;
        row_info.color_type = COLOR_RGB;

        unsigned char row[6] = {9, 18, 27, 36, 45, 54};
        unsigned char input[6];
        std::memcpy(input, row, 6);

        png_do_gamma(&row_info, row, &png_ptr);

        bool ok = mem_equal(row, input, 6);
        ASSERT_TRUE(ok, "Test7 RGB8 with NULL gamma_table leaves row unchanged");
    }

    // Test 8: 16-bit RGB with gamma_16_table NULL -> no change
    {
        png_struct png_ptr;
        png_ptr.gamma_table = nullptr;
        png_ptr.gamma_16_table = nullptr;
        png_ptr.gamma_shift = 0;

        png_row_info_struct row_info;
        row_info.width = 1;
        row_info.bit_depth = 16;
        row_info.color_type = COLOR_RGB;

        // Row: red=0x11 0x11, green=0x22 0x22, blue=0x33 0x33
        unsigned char row[6] = {0x11,0x11, 0x22,0x22, 0x33,0x33};
        unsigned char input[6];
        std::memcpy(input, row, 6);

        png_do_gamma(&row_info, row, &png_ptr);

        bool ok = mem_equal(row, input, 6);
        ASSERT_TRUE(ok, "Test8 RGB16 with NULL gamma_16_table leaves row unchanged");
    }

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed!\n";
        return 0;
    } else {
        std::cout << g_failures << " test(s) failed.\n";
        return 1;
    }
}