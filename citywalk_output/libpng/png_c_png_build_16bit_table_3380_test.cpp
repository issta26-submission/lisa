// Unit test suite for png_build_16bit_table (recreated in test environment)
// This test harness is self-contained (no external GTest) and uses only the
// C++11 standard library. It provides minimal mock implementations of the
// dependencies required by the focal method and exercises multiple code paths.

#include <cstdint>
#include <cmath>
#include <pngpriv.h>
#include <cstdlib>
#include <iostream>
#include <cstring>


// Domain knowledge
//  - Implement only what's necessary for png_build_16bit_table
//  - Use non-terminating assertions implemented as test result counters
//  - Access static-like helpers via functions here (no private access required)

// Namespace: use std for standard library components
using namespace std;

// Rome: Define minimal types and macros to mimic the libpng-style API used by the focal method.

typedef unsigned char png_byte;
typedef uint32_t png_uint_32;
typedef uint16_t png_uint_16;
typedef int png_int_32;
typedef int png_fixed_point; // signed 32-bit fixed/floating as used in focal method

typedef struct png_struct_def png_struct;
typedef png_struct* png_structrp; // pointer to png_struct (alias for compatibility)

// Pseudo libpng pointer-to-table types used by the focal method
typedef png_uint_16 png_uint_16t;
typedef png_uint_16* png_uint_16p;
typedef png_uint_16p* png_uint_16pp; // pointer to an array of 256-entry sub-tables

// Forward declarations of minimal mock dependencies used by png_build_16bit_table
static void* png_malloc(png_structrp png_ptr, size_t size);
static void* png_calloc(png_structrp png_ptr, size_t size);
static int png_gamma_significant(png_fixed_point gamma_val);
static png_uint_16 png_gamma_16bit_correct(png_uint_32 value, png_fixed_point gamma_val);

// Minimal mock of a libpng structure (empty, as test does not rely on internals)
struct png_struct_def { };

// PNG_FLOATING_ARITHMETIC_SUPPORTED flag (activate floating path in focal method)
#ifndef PNG_FLOATING_ARITHMETIC_SUPPORTED
#define PNG_FLOATING_ARITHMETIC_SUPPORTED 1
#endif

// The focal method under test (recreated here for self-contained unit tests)
extern "C" {

png_uint_16pp png_build_16bit_table(png_structrp png_ptr, png_uint_16pp *ptable,
    unsigned int shift, png_fixed_point gamma_val);

png_uint_16pp png_build_16to8_table(png_structrp png_ptr, png_uint_16pp *ptable,
    unsigned int shift, png_fixed_point gamma_val)
{
    // Not used in tests; provide empty placeholder to satisfy potential linker
    (void)png_ptr; (void)ptable; (void)shift; (void)gamma_val;
    return nullptr;
}

} // extern "C"

// Implementation of mock dependencies

static void* png_malloc(png_structrp /*png_ptr*/, size_t size)
{
    return std::malloc(size);
}
static void* png_calloc(png_structrp /*png_ptr*/, size_t size)
{
    void* p = std::calloc(1, size);
    return p;
}
static int png_gamma_significant(png_fixed_point gamma_val)
{
    // Non-zero gamma indicates significance (as in the focal code)
    return (gamma_val != 0);
}
static png_uint_16 png_gamma_16bit_correct(png_uint_32 value, png_fixed_point /*gamma_val*/)
{
    // Minimal gamma correction: clamp to 0..65535
    unsigned int v = static_cast<unsigned int>(value);
    if (v > 65535U) v = 65535U;
    return static_cast<png_uint_16>(v);
}

// Implementation of the focal method (self-contained version)
extern "C" {

static double fmax_builtin(double x) { return x; } // helper for readability (not used)
png_uint_16pp png_build_16bit_table(png_structrp png_ptr, png_uint_16pp *ptable,
    unsigned int shift, png_fixed_point gamma_val)
{
{
   /* Various values derived from 'shift': */
   unsigned int num = 1U << (8U - shift);
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
   /* CSE the division and work round wacky GCC warnings (see the comments
    * in png_gamma_8bit_correct for where these come from.)
    */
   double fmax = 1.0 / (((png_int_32)1 << (16U - shift)) - 1);
#endif
   unsigned int max = (1U << (16U - shift)) - 1U;
   unsigned int max_by_2 = 1U << (15U - shift);
   unsigned int i;
   png_uint_16pp table = *ptable =
       (png_uint_16pp)png_calloc(png_ptr, num * (sizeof (png_uint_16p)));
   for (i = 0; i < num; i++)
   {
      png_uint_16p sub_table = table[i] =
          (png_uint_16p)png_malloc(png_ptr, 256 * (sizeof (png_uint_16)));
      /* The 'threshold' test is repeated here because it can arise for one of
       * the 16-bit tables even if the others don't hit it.
       */
      if (png_gamma_significant(gamma_val) != 0)
      {
         /* The old code would overflow at the end and this would cause the
          * 'pow' function to return a result >1, resulting in the
          * arithmetic error.  This code follows the spec exactly; ig is
          * the recovered input sample, it always has 8-16 bits.
          *
          * We want input * 65535/max, rounded, the arithmetic fits in 32
          * bits (unsigned) so long as max <= 32767.
          */
         unsigned int j;
         for (j = 0; j < 256; j++)
         {
            png_uint_32 ig = (j << (8-shift)) + i;
#           ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
               /* Inline the 'max' scaling operation: */
               /* See png_gamma_8bit_correct for why the cast to (int) is
                * required here.
                */
               double d = floor(65535.*pow(ig*fmax, gamma_val*.00001)+.5);
               sub_table[j] = (png_uint_16)d;
#           else
               if (shift != 0)
                  ig = (ig * 65535U + max_by_2)/max;
               sub_table[j] = png_gamma_16bit_correct(ig, gamma_val);
#           endif
         }
      }
      else
      {
         /* We must still build a table, but do it the fast way. */
         unsigned int j;
         for (j = 0; j < 256; j++)
         {
            png_uint_32 ig = (j << (8-shift)) + i;
            if (shift != 0)
               ig = (ig * 65535U + max_by_2)/max;
            sub_table[j] = (png_uint_16)ig;
         }
      }
   }
}
}

return nullptr;
}

} // extern "C"

// Simple test harness (self-contained)
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define CHECK(cond, msg) do { if (cond) { ++tests_passed; } else { ++tests_failed; std::cerr << "Test failed: " << msg << std::endl; } ++tests_run; } while(0)

// Helper to free allocated tables
static void free_tables(png_uint_16pp table, unsigned int num)
{
    if (table == nullptr) return;
    for (unsigned int i = 0; i < num; ++i)
    {
        free(table[i]);
    }
    free(table);
}

static void test_no_gamma_shift0()
{
    // Test purpose:
    // - Gamma is zero; expects "fast path" to be taken
    // - shift = 0 -> num = 256; first sub_table is table[0]
    // - For i=0, j=0 => ig = 0, so sub_table[0] should be 0
    // - For i=0, j=1 => ig = 256, so sub_table[1] should be 256
    // - For i=1, j=0 => sub_table should be 1
    png_struct dummy;
    png_structrp png_ptr = &dummy;

    png_uint_16pp table_out = nullptr;
    png_uint_16pp *ptable_ptr = &table_out;

    unsigned int shift = 0;
    png_fixed_point gamma_val = 0; // no gamma

    png_build_16bit_table(png_ptr, ptable_ptr, shift, gamma_val);

    unsigned int num = 1U << (8 - shift); // 256
    // Validate the top-level array was allocated
    CHECK(table_out != nullptr, "No table allocated for gamma=0, shift=0");
    if (table_out == nullptr) return;

    // Validate values for the first sub-table (i=0)
    png_uint_16p sub0 = table_out[0];
    CHECK(sub0 != nullptr, "First sub-table (i=0) is null");
    if (sub0 != nullptr)
    {
        CHECK(sub0[0] == 0, "sub0[0] should be 0 when i=0, j=0, shift=0, gamma=0");
        CHECK(sub0[1] == 256, "sub0[1] should be 256 for i=0, j=1, shift=0, gamma=0");
        CHECK(sub0[255] == 255 * 256, "sub0[255] should be 65280 for i=0, j=255");
    }

    // Validate values for i=1, j=0
    png_uint_16p sub1 = table_out[1];
    CHECK(sub1 != nullptr, "Second sub-table (i=1) is null");
    if (sub1 != nullptr)
    {
        CHECK(sub1[0] == 1, "sub1[0] should be 1 for i=1, j=0, shift=0, gamma=0");
        CHECK(sub1[255] == 255 * 256 + 1, "sub1[255] should be 65281 for i=1, j=255");
    }

    // Free resources
    free_tables(table_out, num);
}

static void test_gamma_significant_shift0()
{
    // Test purpose:
    // - Gamma non-zero with shifting 0 triggers gamma path where
    //   256 entries per sub-table are computed using floating arithmetic when
    //   PNG_FLOATING_ARITHMETIC_SUPPORTED is defined.
    png_struct dummy;
    png_structrp png_ptr = &dummy;

    png_uint_16pp table_out = nullptr;
    png_uint_16pp *ptable_ptr = &table_out;

    unsigned int shift = 0;
    png_fixed_point gamma_val = 1; // non-zero gamma

    png_build_16bit_table(png_ptr, ptable_ptr, shift, gamma_val);

    unsigned int num = 1U << (8 - shift); // 256
    CHECK(table_out != nullptr, "Table allocated for gamma significant, shift=0");
    if (table_out == nullptr) return;

    // Check that the first sub-table exists and first value is 0 (ig=0 leads to 0)
    png_uint_16p sub0 = table_out[0];
    CHECK(sub0 != nullptr, "First sub-table (i=0) is null in gamma path");
    if (sub0 != nullptr)
    {
        CHECK(sub0[0] == 0, "gamma significant, ig=0 yields sub0[0] == 0");
    }

    // Clean up
    free_tables(table_out, num);
}

static void test_shift4_gamma_zero()
{
    // Test purpose:
    // - Gamma zero but shift non-zero to exercise fast-path with non-trivial ig
    // - shift = 4 => num = 16; expect 256 entries per sub-table
    png_struct dummy;
    png_structrp png_ptr = &dummy;

    png_uint_16pp table_out = nullptr;
    png_uint_16pp *ptable_ptr = &table_out;

    unsigned int shift = 4;
    png_fixed_point gamma_val = 0;

    png_build_16bit_table(png_ptr, ptable_ptr, shift, gamma_val);

    unsigned int num = 1U << (8 - shift); // 16
    CHECK(table_out != nullptr, "Table allocated for gamma=0, shift=4");
    if (table_out == nullptr) return;

    png_uint_16p sub0 = table_out[0];
    CHECK(sub0 != nullptr, "First sub-table (i=0) null");
    if (sub0 != nullptr)
    {
        // When shift != 0 and gamma==0, ig = (j << (8-shift)) + i
        // For i=0, j=0 => ig=0 => sub0[0] == 0
        // For i=0, j=1 => ig=1 << 4 = 16 => sub0[1] == 16
        CHECK(sub0[0] == 0, "sub0[0] should be 0 when i=0, j=0, shift=4, gamma=0");
        CHECK(sub0[1] == 16, "sub0[1] should be 16 when i=0, j=1, shift=4, gamma=0");
    }

    // Clean up
    free_tables(table_out, num);
}

int main()
{
    // Run unit tests
    test_no_gamma_shift0();
    test_gamma_significant_shift0();
    test_shift4_gamma_zero();

    // Summary
    cout << "Tests run: " << tests_run
         << ", Passed: " << tests_passed
         << ", Failed: " << tests_failed << endl;
    return (tests_failed == 0) ? 0 : 1;
}