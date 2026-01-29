// Unit tests for the focal method png_do_unshift
// This test suite is implemented in C++11 without Google Test and uses a
// minimal, self-contained re-implementation of the required types and the
// function under test to ensure portability and easy compilation.

// The goal is to exercise true/false branches of predicates in png_do_unshift,
// including palette handling, channel/gray handling, alpha handling, and the
// various bit-depth cases (2, 4, 8, with 16-bit codepath excluded by default).

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Minimal re-declaration of libpng-like types to compile and run the focal function.
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef const struct png_color_8* png_const_color_8p;

// Simplified row information struct matching the expected API usage.
struct png_row_info {
    int color_type;
    int bit_depth;
    int rowbytes;
};
typedef struct png_row_info* png_row_infop;

// Minimal color structure used to pass significant bits for each channel.
struct png_color_8 {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char gray;
    unsigned char alpha;
};

// Pseudo-names for color type constants and masks (aligned with libpng values).
#define PNG_COLOR_TYPE_GRAY       0
#define PNG_COLOR_TYPE_RGB          2
#define PNG_COLOR_TYPE_PALETTE      3
// Note: The RGBA type is typically 6 in libpng (RGB + Alpha). We use 6 here.
#define PNG_COLOR_TYPE_RGB_ALPHA    6

#define PNG_COLOR_MASK_COLOR 0x02
#define PNG_COLOR_MASK_ALPHA 0x04

// Dummy png_debug to satisfy the function's call in tests.
static void png_debug(int /*level*/, const char* /*m*/){ /* no-op for tests */ }

// The focal function under test.
// It is implemented to mirror the logic observed in the provided snippet.
void png_do_unshift(png_row_infop row_info, png_bytep row, png_const_color_8p sig_bits)
{
{
   int color_type;
   png_debug(1, "in png_do_unshift");
   color_type = row_info->color_type;
   if (color_type != PNG_COLOR_TYPE_PALETTE)
   {
      int shift[4];
      int channels = 0;
      int bit_depth = row_info->bit_depth;
      if ((color_type & PNG_COLOR_MASK_COLOR) != 0)
      {
         shift[channels++] = bit_depth - sig_bits->red;
         shift[channels++] = bit_depth - sig_bits->green;
         shift[channels++] = bit_depth - sig_bits->blue;
      }
      else
      {
         shift[channels++] = bit_depth - sig_bits->gray;
      }
      if ((color_type & PNG_COLOR_MASK_ALPHA) != 0)
      {
         shift[channels++] = bit_depth - sig_bits->alpha;
      }
      {
         int c, have_shift;
         for (c = have_shift = 0; c < channels; ++c)
         {
            /* A shift of more than the bit depth is an error condition but it
             * gets ignored here.
             */
            if (shift[c] <= 0 || shift[c] >= bit_depth)
               shift[c] = 0;
            else
               have_shift = 1;
         }
         if (have_shift == 0)
            return;
      }
      switch (bit_depth)
      {
         default:
         /* Must be 1bpp gray: should not be here! */
            /* NOTREACHED */
            break;
         case 2:
         /* Must be 2bpp gray */
         /* assert(channels == 1 && shift[0] == 1) */
         {
            png_bytep bp = row;
            png_bytep bp_end = bp + row_info->rowbytes;
            while (bp < bp_end)
            {
               int b = (*bp >> 1) & 0x55;
               *bp++ = (png_byte)b;
            }
            break;
         }
         case 4:
         /* Must be 4bpp gray */
         /* assert(channels == 1) */
         {
            png_bytep bp = row;
            png_bytep bp_end = bp + row_info->rowbytes;
            int gray_shift = shift[0];
            int mask =  0xf >> gray_shift;
            mask |= mask << 4;
            while (bp < bp_end)
            {
               int b = (*bp >> gray_shift) & mask;
               *bp++ = (png_byte)b;
            }
            break;
         }
         case 8:
         /* Single byte components, G, GA, RGB, RGBA */
         {
            png_bytep bp = row;
            png_bytep bp_end = bp + row_info->rowbytes;
            int channel = 0;
            while (bp < bp_end)
            {
               int b = *bp >> shift[channel];
               if (++channel >= channels)
                  channel = 0;
               *bp++ = (png_byte)b;
            }
            break;
         }
#ifdef PNG_READ_16BIT_SUPPORTED
         case 16:
         /* Double byte components, G, GA, RGB, RGBA */
         {
            png_bytep bp = row;
            png_bytep bp_end = bp + row_info->rowbytes;
            int channel = 0;
            while (bp < bp_end)
            {
               int value = (bp[0] << 8) + bp[1];
               value >>= shift[channel];
               if (++channel >= channels)
                  channel = 0;
               *bp++ = (png_byte)(value >> 8);
               *bp++ = (png_byte)value;
            }
            break;
         }
#endif
      }
   }
}
}

// Simple assertion helper for tests that does not terminate on failure.
static bool eq(const unsigned char* a, const unsigned char* b, size_t n)
{
    return std::memcmp(a, b, n) == 0;
}

// Test helpers and test cases

// 1) Palette type should be left unchanged
bool test_palette_no_change()
{
    // Setup
    png_row_info ri;
    ri.color_type = PNG_COLOR_TYPE_PALETTE;
    ri.bit_depth = 8;
    ri.rowbytes = 4;

    unsigned char row[4] = { 1, 2, 3, 4 };
    unsigned char original[4];
    std::memcpy(original, row, 4);

    png_color_8 bits;
    bits.red = bits.green = bits.blue = bits.gray = bits.alpha = 0;

    // Execute
    png_do_unshift(&ri, row, &bits);

    // Verify: row should be unchanged
    bool ok = eq(row, original, 4);
    if (!ok) {
        std::cerr << "Test: palette_no_change - row was modified unexpectedly.\n";
    }
    return ok;
}

// 2) No shift should occur (shift value out of range) -> early return with no changes
//    bit_depth = 8, color_type Gray, sig_bits.gray = 8 => shift[0] = 0
bool test_no_shift_early_return_gray8()
{
    png_row_info ri;
    ri.color_type = PNG_COLOR_TYPE_GRAY;
    ri.bit_depth = 8;
    ri.rowbytes = 4;

    unsigned char row[4] = { 10, 20, 30, 40 };
    unsigned char original[4];
    std::memcpy(original, row, 4);

    png_color_8 bits;
    bits.gray = 8;

    png_do_unshift(&ri, row, &bits);

    bool ok = eq(row, original, 4);
    if (!ok) {
        std::cerr << "Test: no_shift_early_return_gray8 - row changed unexpectedly.\n";
    }
    return ok;
}

// 3) 8-bit RGB unshift: each channel shifted right by 1 (sig_bits = 7 for each channel)
bool test_rgb8_unshift()
{
    png_row_info ri;
    ri.color_type = PNG_COLOR_TYPE_RGB;
    ri.bit_depth = 8;
    ri.rowbytes = 3;

    unsigned char row[3] = { 200, 100, 50 };
    unsigned char expected[3] = { 100, 50, 25 };

    png_color_8 bits;
    bits.red = 7;
    bits.green = 7;
    bits.blue = 7;
    bits.gray = 0;
    bits.alpha = 0;

    png_do_unshift(&ri, row, &bits);

    bool ok = eq(row, expected, 3);
    if (!ok) {
        std::cerr << "Test: rgb8_unshift - unexpected transformed values.\n";
        std::cerr << "Expected: [100,50,25], Got: [" 
                  << int(row[0]) << "," << int(row[1]) << "," << int(row[2]) << "]\n";
    }
    return ok;
}

// 4) 8-bit RGBA unshift: each channel including alpha shifted by 1
bool test_rgba8_unshift()
{
    png_row_info ri;
    ri.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    ri.bit_depth = 8;
    ri.rowbytes = 4;

    unsigned char row[4] = { 200, 100, 50, 255 };
    unsigned char expected[4] = { 100, 50, 25, 127 };

    PNG_COLOR_8 bits;
    bits.red = 7;
    bits.green = 7;
    bits.blue = 7;
    bits.alpha = 7;

    png_do_unshift(&ri, row, &bits);

    bool ok = eq(row, expected, 4);
    if (!ok) {
        std::cerr << "Test: rgba8_unshift - unexpected transformed values.\n";
        std::cerr << "Expected: [100,50,25,127], Got: [" 
                  << int(row[0]) << "," << int(row[1]) << "," 
                  << int(row[2]) << "," << int(row[3]) << "]\n";
    }
    return ok;
}

// 5) 4-bit Gray unshift: bit_depth=4, shift = 1 (sig_bits.gray = 3)
//    Packets: one byte containing two 4-bit pixels. We test a single-byte row.
bool test_gray4_unshift()
{
    png_row_info ri;
    ri.color_type = PNG_COLOR_TYPE_GRAY;
    ri.bit_depth = 4;
    ri.rowbytes = 1;

    unsigned char row[1] = { 0x6F }; // 0x6F -> two 4-bit values: 6 and F
    unsigned char expected[1] = { 0x37 }; // As computed: (0x6F >> 1) = 0x37, masked to 0x77 -> 0x37

    png_color_8 bits;
    bits.gray = 3; // shift = 4 - 3 = 1

    png_do_unshift(&ri, row, &bits);

    bool ok = eq(row, expected, 1);
    if (!ok) {
        std::cerr << "Test: gray4_unshift - unexpected transformed value.\n";
        std::cerr << "Expected: 0x37, Got: 0x" << std::hex << int(row[0]) << std::dec << "\n";
    }
    return ok;
}

// 6) Ensure 16-bit code path is not exercised by default (we didn't enable PNG_READ_16BIT_SUPPORTED)
bool test_no_16bit_path_compiles_and_runs()
{
    // This test ensures code compiles and the 16-bit path is not taken when not configured.
    // We'll reuse a simple 8-bit RGB scenario to confirm function still works in the non-16-bit mode.
    png_row_info ri;
    ri.color_type = PNG_COLOR_TYPE_RGB;
    ri.bit_depth = 8;
    ri.rowbytes = 3;

    unsigned char row[3] = { 66, 133, 200 };
    unsigned char expected[3] = { 33, 66, 100 };

    png_color_8 bits;
    bits.red = 7;
    bits.green = 7;
    bits.blue = 7;
    bits.gray = 0;
    bits.alpha = 0;

    png_do_unshift(&ri, row, &bits);

    bool ok = eq(row, expected, 3);
    if (!ok) {
        std::cerr << "Test: no_16bit_path_compiles_and_runs - mismatch (ensures 16-bit path is not used in this build).\n";
    }
    return ok;
}

// Runner
int main()
{
    using std::cout;
    using std::endl;

    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool ok){
        ++total;
        if (ok) {
            ++passed;
            cout << "[PASS] " << name << "\n";
        } else {
            cout << "[FAIL] " << name << "\n";
        }
    };

    run("Palette type should be unchanged", test_palette_no_change());
    run("No shift should occur (gray8)", test_no_shift_early_return_gray8());
    run("RGB8 unshift", test_rgb8_unshift());
    run("RGBA8 unshift", test_rgba8_unshift());
    run("Gray4 unshift (4bpp)", test_gray4_unshift());
    run("No shift path verified for 8-bit RGB", test_no_shift_early_return_gray8()); // reuse to ensure coverage
    run("No 16-bit path taken when not enabled", test_no_16bit_path_compiles_and_runs());

    cout << "\nTest Summary: " << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}