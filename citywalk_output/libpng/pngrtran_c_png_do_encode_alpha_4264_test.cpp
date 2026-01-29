// A standalone C++11 test suite for the focal function png_do_encode_alpha
// This test suite implements a minimal mocked environment for the libpng
// types used by png_do_encode_alpha, avoiding GTest and relying on a tiny
// custom harness that continues on failures to maximize coverage.

// NOTE: The focal method is included in this test file to ensure it compiles
// and runs in a self-contained manner without external dependencies.

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Minimal type and constant definitions to mirror the necessary parts of libpng
typedef unsigned char png_bytep;
typedef unsigned char png_byte;
typedef unsigned int  png_uint_32;
typedef unsigned short png_uint_16;
typedef png_uint_16* png_uint_16p;
typedef png_uint_16** png_uint_16pp;
typedef png_bytep png_row;
typedef png_bytep png_rowp;

// Forward declaration for the warning/debug hooks used by the focal method
static std::vector<std::string> g_warnings;
static void png_debug(int level, const char* msg);
static void png_warning(void* png_ptr, const char* msg);

// Definition of a minimal png_row_info structure and a png_struct pointer type
struct png_row_info_struct {
    int width;
    int color_type;
    int bit_depth;
};
typedef struct png_row_info_struct* png_row_infop;
struct png_struct_def {};
typedef struct png_struct_def* png_structrp;

// Pixel type masks (taken to match typical libpng masks)
#define PNG_COLOR_MASK_ALPHA 0x08
#define PNG_COLOR_MASK_COLOR 0x04

// Forward declaration of the focal method (as implemented below)
void png_do_encode_alpha(png_row_infop row_info, png_bytep row, png_structrp png_ptr);

// Minimal stub for png_debug (no-op for tests)
static void png_debug(int level, const char* msg) {
    (void)level; (void)msg;
}

// Minimal warning collector to verify branches hit
static void png_warning(void* /*png_ptr*/, const char* msg) {
    g_warnings.push_back(std::string(msg ? msg : "<null>"));
}

// The focal method copied/adapted for testing (C-like implementation in C++)
void png_do_encode_alpha(png_row_infop row_info, png_bytep row, png_structrp png_ptr) {
{
   png_uint_32 row_width = row_info->width;
   png_debug(1, "in png_do_encode_alpha");
   if ((row_info->color_type & PNG_COLOR_MASK_ALPHA) != 0)
   {
      if (row_info->bit_depth == 8)
      {
         png_bytep table = ((png_structrp)png_ptr)->/* gamma_from_1 */ nullptr;
      }
   }
}
}

// The actual focal logic adapted for testing (re-implement cleanly here)
struct PngCtx {
    png_bytep gamma_from_1;
    png_uint_16p* gamma_16_from_1;
    int gamma_shift;
};
static void png_do_encode_alpha_impl(png_row_infop row_info, png_bytep row, PngCtx* png_ptr) {
   png_uint_32 row_width = row_info->width;
   png_debug(1, "in png_do_encode_alpha");
   if ((row_info->color_type & PNG_COLOR_MASK_ALPHA) != 0)
   {
      if (row_info->bit_depth == 8)
      {
         png_bytep table = png_ptr->gamma_from_1;
         if (table != NULL)
         {
            int step = (row_info->color_type & PNG_COLOR_MASK_COLOR) ? 4 : 2;
            /* The alpha channel is the last component: */
            row += step - 1;
            for (; row_width > 0; --row_width, row += step)
               *row = table[*row];
            return;
         }
      }
      else if (row_info->bit_depth == 16)
      {
         png_uint_16pp table = png_ptr->gamma_16_from_1;
         int gamma_shift = png_ptr->gamma_shift;
         if (table != NULL)
         {
            int step = (row_info->color_type & PNG_COLOR_MASK_COLOR) ? 8 : 4;
            /* The alpha channel is the last component: */
            row += step - 2;
            for (; row_width > 0; --row_width, row += step)
            {
               png_uint_16 v;
               v = table[*(row + 1) >> gamma_shift][*row];
               *row = (png_byte)((v >> 8) & 0xff);
               *(row + 1) = (png_byte)(v & 0xff);
            }
            return;
         }
      }
   }
   /* Only get to here if called with a weird row_info; no harm has been done,
    * so just issue a warning.
    */
   png_warning(png_ptr, "png_do_encode_alpha: unexpected call");
}

// Public wrapper with C-like signature that adapts to our internal impl
void png_do_encode_alpha(png_row_infop row_info, png_bytep row, png_structrp png_ptr) {
    // Our test harness uses a simple PngCtx wrapper for gamma tables
    PngCtx* ctx = reinterpret_cast<PngCtx*>(png_ptr);
    if (ctx == nullptr) {
        // If a real ptr is not provided, just warn
        png_warning(png_ptr, "png_do_encode_alpha: null context");
        return;
    }
    png_do_encode_alpha_impl(row_info, row, ctx);
}

// A tiny test harness state
static int g_tests_run = 0;
static int g_failures = 0;

// Simple assertion macro that does not abort tests
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Utility: reset warnings before each test
static void reset_warnings() { g_warnings.clear(); }

// Test 1: 8-bit RGBA (color) with gamma_from_1 non-null; expect alpha channel remapped
static void test_png_do_encode_alpha_8bit_color_nonnull() {
    g_warnings.clear();
    // Setup row info: 3 pixels, RGBA (color + alpha)
    png_row_infop ri = new png_row_info_struct{3, 0x0C /* COLOR + ALPHA */, 8};
    // Row: R,G,B,A, R,G,B,A, R,G,B,A
    unsigned char row[12] = {0,0,0,10,  0,0,0,20,  0,0,0,30};

    // gamma_from_1: a simple mapping (i -> 255 - i)
    unsigned char gamma_from_1[256];
    for (int i = 0; i < 256; ++i) gamma_from_1[i] = (unsigned char)(255 - i);

    PngCtx ctx;
    ctx.gamma_from_1 = gamma_from_1;
    ctx.gamma_16_from_1 = nullptr;
    ctx.gamma_shift = 0;

    png_structrp png_ptr = reinterpret_cast<png_structrp>(&ctx);

    // Call
    png_do_encode_alpha(ri, row, png_ptr);

    // Validate: A values remapped
    TEST_ASSERT(row[3] == 245, "A0 should be 255 - 10 (245)");
    TEST_ASSERT(row[7] == 235, "A1 should be 255 - 20 (235)");
    TEST_ASSERT(row[11] == 225, "A2 should be 255 - 30 (225)");
    // No warnings expected
    TEST_ASSERT(g_warnings.empty(), "No warnings expected in test 1");
    delete ri;
}

// Test 2: 8-bit grayscale+alpha with non-null gamma_from_1; ensure proper mapping on alpha
static void test_png_do_encode_alpha_8bit_grayscale_nonnull() {
    g_warnings.clear();
    // Setup row info: 3 pixels, GRAY + ALPHA (grayscale with alpha)
    png_row_infop ri = new png_row_info_struct{3, 0x08 /* ALPHA MASK only (no COLOR) */, 8};
    // Row: Gray, Alpha, Gray, Alpha, Gray, Alpha
    unsigned char row[6] = {5, 10, 7, 20, 1, 30};

    unsigned char gamma_from_1[256];
    for (int i = 0; i < 256; ++i) gamma_from_1[i] = (unsigned char)(255 - i);

    PngCtx ctx;
    ctx.gamma_from_1 = gamma_from_1;
    ctx.gamma_16_from_1 = nullptr;
    ctx.gamma_shift = 0;

    png_structrp png_ptr = reinterpret_cast<png_structrp>(&ctx);

    png_do_encode_alpha(ri, row, png_ptr);

    TEST_ASSERT(row[1] == 245, "Alpha0 should map 10 -> 245");
    TEST_ASSERT(row[3] == 235, "Alpha1 should map 20 -> 235");
    TEST_ASSERT(row[5] == 225, "Alpha2 should map 30 -> 225");
    TEST_ASSERT(g_warnings.empty(), "No warnings expected in test 2");
    delete ri;
}

// Test 3: 8-bit grayscale+alpha with NULL gamma_from_1; expect no modification and a warning
static void test_png_do_encode_alpha_8bit_grayscale_nulltable() {
    g_warnings.clear();
    png_row_infop ri = new png_row_info_struct{2, 0x08, 8}; // 2 pixels
    unsigned char row[4] = {12, 99, 34, 200}; // A values 99 and 200

    PngCtx ctx;
    ctx.gamma_from_1 = nullptr;
    ctx.gamma_16_from_1 = nullptr;
    ctx.gamma_shift = 0;

    png_structrp png_ptr = reinterpret_cast<png_structrp>(&ctx);

    png_do_encode_alpha(ri, row, png_ptr);

    // No change expected to alpha values
    TEST_ASSERT(row[1] == 99, "Alpha should remain 99 (no gamma table)");
    TEST_ASSERT(row[3] == 200, "Alpha should remain 200 (no gamma table)");
    TEST_ASSERT(!g_warnings.empty(), "Warning expected for NULL gamma table");
    delete ri;
}

// Test 4: 16-bit RGBA (color) with gamma_16_from_1 non-null; verify two pixels mapped
static void test_png_do_encode_alpha_16bit_color_nonnull() {
    g_warnings.clear();
    // Setup row info: 2 pixels, RGBA with 16-bit per component
    png_row_infop ri = new png_row_info_struct{2, 0x0C, 16};
    // Row layout: 2 bytes per component, 4 components per pixel, total 16 bytes
    // We set alpha pairs as:
    // Pixel 0: low=0x02, high=0x01  (row[6]=0x02, row[7]=0x01)
    // Pixel 1: low=0x04, high=0x03  (row[14]=0x04, row[15]=0x03)
    unsigned char row[16] = {0};

    // Pre-fill alphas
    row[6]  = 0x02; // alpha low byte for pixel 0
    row[7]  = 0x01; // alpha high byte for pixel 0
    row[14] = 0x04; // alpha low byte for pixel 1
    row[15] = 0x03; // alpha high byte for pixel 1

    // gamma_16_from_1: create a 256x256 table, with specific mappings
    png_uint_16pp gamma16 = new png_uint_16*[256];
    for (int i = 0; i < 256; ++i) {
        gamma16[i] = new png_uint_16[256];
        std::memset(gamma16[i], 0, 256 * sizeof(png_uint_16));
    }
    // Define mappings used in test
    gamma16[1][2] = 0x1234;
    gamma16[3][4] = 0xABCD;

    PngCtx ctx;
    ctx.gamma_from_1 = nullptr;
    ctx.gamma_16_from_1 = gamma16;
    ctx.gamma_shift = 0;

    png_structrp png_ptr = reinterpret_cast<png_structrp>(&ctx);

    // Call
    png_do_encode_alpha(ri, row, png_ptr);

    // Validate that two alpha values were updated
    TEST_ASSERT(row[6] == 0x12, "Alpha low byte pixel0 should be 0x12");
    TEST_ASSERT(row[7] == 0x34, "Alpha high byte pixel0 should be 0x34");
    TEST_ASSERT(row[14] == 0xAB, "Alpha low byte pixel1 should be 0xAB");
    TEST_ASSERT(row[15] == 0xCD, "Alpha high byte pixel1 should be 0xCD");

    // Cleanup
    for (int i = 0; i < 256; ++i) delete[] gamma16[i];
    delete[] gamma16;
    delete ri;
}

// Test 5: 16-bit RGBA with NULL gamma_16_from_1; expect a warning and no modification
static void test_png_do_encode_alpha_16bit_color_nulltable() {
    g_warnings.clear();
    png_row_infop ri = new png_row_info_struct{2, 0x0C, 16};
    unsigned char row[16] = {0};
    // Set alpha bytes as in test 4
    row[6]  = 0x02; row[7]  = 0x01;
    row[14] = 0x04; row[15] = 0x03;

    PngCtx ctx;
    ctx.gamma_from_1 = nullptr;
    ctx.gamma_16_from_1 = nullptr;
    ctx.gamma_shift = 0;

    png_structrp png_ptr = reinterpret_cast<png_structrp>(&ctx);

    png_do_encode_alpha(ri, row, png_ptr);

    // Expect no modification
    TEST_ASSERT(row[6] == 0x02, "Alpha low byte should remain 0x02 (no gamma table)");
    TEST_ASSERT(row[7] == 0x01, "Alpha high byte should remain 0x01 (no gamma table)");
    TEST_ASSERT(row[14] == 0x04, "Alpha low byte should remain 0x04 (no gamma table)");
    TEST_ASSERT(row[15] == 0x03, "Alpha high byte should remain 0x03 (no gamma table)");
    TEST_ASSERT(!g_warnings.empty(), "Warning expected for NULL gamma_16_from_1");
    delete ri;
}

// Test 6: No alpha bit set in color_type; should warn and do nothing
static void test_png_do_encode_alpha_no_alpha_bit() {
    g_warnings.clear();
    // color_type without ALPHA (only COLOR)
    png_row_infop ri = new png_row_info_struct{2, 0x04, 8}; // COLOR mask set, but no ALPHA mask
    unsigned char row[4] = {1,2, 3,4};
    PngCtx ctx;
    ctx.gamma_from_1 = nullptr;
    ctx.gamma_16_from_1 = nullptr;
    ctx.gamma_shift = 0;
    png_structrp png_ptr = reinterpret_cast<png_structrp>(&ctx);

    png_do_encode_alpha(ri, row, png_ptr);

    TEST_ASSERT(row[0] == 1 && row[1] == 2 && row[2] == 3 && row[3] == 4, "Row should remain unchanged when no alpha channel");
    TEST_ASSERT(!g_warnings.empty(), "Warning expected when alpha channel is not present");
    delete ri;
}

// Main runner
int main() {
    std::cout << "Running test suite for png_do_encode_alpha (mocked environment)" << std::endl;
    g_warnings.clear();

    test_png_do_encode_alpha_8bit_color_nonnull();
    test_png_do_encode_alpha_8bit_grayscale_nonnull();
    test_png_do_encode_alpha_8bit_grayscale_nulltable();
    test_png_do_encode_alpha_16bit_color_nonnull();
    test_png_do_encode_alpha_16bit_color_nulltable();
    test_png_do_encode_alpha_no_alpha_bit();

    int total = g_tests_run;
    std::cout << "Tests attempted: " << total << std::endl;
    std::cout << "Failures: " << g_failures << std::endl;

    // The test harness uses manual assertions; reflect success/failure by exit code
    return (g_failures == 0) ? 0 : 1;
}