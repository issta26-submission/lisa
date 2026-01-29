/*
This is a self-contained C++11 test suite for the focal function:
  png_build_gamma_table(png_structrp png_ptr, int bit_depth)

The test suite includes minimal, self-contained stubs of the dependencies
and the environment required by the function under test. It uses a small
in-house test harness (no Google Test) with non-terminating expectations.

Notes:
- The goal is to achieve coverage across the branching logic in png_build_gamma_table.
- The test scaffold is designed to be compiled as a single translation unit.
- All necessary helpers are implemented within this file to keep it portable.

Compilation:
  g++ -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=1 <this_file.cpp> -O2 -Wall

*/

// Begin: Minimal in-file implementation of the dependencies and the focal function

#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <pngpriv.h>
#include <iostream>
#include <cstdio>
#include <cstring>


// Domain-specific macros copied/adapted to enable branching in tests
#define GAMMA_TRANSFORMS 1
#define PNG_16BIT_SUPPORTED 1
#define PNG_MAX_GAMMA_8 11

#define PNG_COLOR_MASK_COLOR 0x1
#define PNG_COMPOSE 1
#define PNG_RGB_TO_GRAY 2
#define PNG_16_TO_8 4
#define PNG_SCALE_16_TO_8 8

#define PNG_FP_1 1

typedef int png_fixed_point; // Simplified fixed-point placeholder

// Forward declaration of the focal struct type
struct png_struct;

// Typedefs used by the focal function
typedef struct png_struct* png_structrp;

// Simple, small, C-compatible API surface (stubs) used by the focal method
typedef unsigned char png_byte;
typedef unsigned short png_uint_16;
typedef png_byte* png_bytep;
typedef png_bytep* png_bytepp;
typedef png_uint_16* png_uint_16p;
typedef png_uint_16* png_uint_16pp; // used via &gamma_16_table in tests

// Global counters for observable side-effects in tests
static int g_warning_count = 0;
static int g_destroy_calls = 0;

// Minimal, test-friendly representation of sig_bit
struct png_sig_bits {
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int gray;
};

// Minimal png_struct used by the focal function
struct png_struct {
    png_byte* gamma_table;            // 8-bit gamma lookup table
    png_uint_16* gamma_16_table;      // 16-bit gamma lookup table
    png_byte* gamma_to_1;               // 8-bit to 1 (for transforms)
    png_byte* gamma_from_1;             // 8-bit from 1 (for transforms)

    int file_gamma;                     // simplified gamma for file
    int screen_gamma;                   // simplified gamma for screen
    int color_type;                     // PNG_COLOR_MASK_COLOR usage
    int transformations;                // transform flags
    unsigned int gamma_shift;           // 16->8 shift

    png_sig_bits sig_bit;               // per-channel bit depths
};

// Proto-types for helper routines (stubs)
static void png_debug(int level, const char* message) {
    // no-op in test harness
    (void)level; (void)message;
}
static void png_warning(png_structrp png_ptr, const char* message) {
    (void)png_ptr;
    // Increment a simple warning counter; in real libpng this would log
    ++g_warning_count;
    (void)message;
}
static void png_destroy_gamma_table(png_structrp png_ptr) {
    if (png_ptr == nullptr) return;
    if (png_ptr->gamma_table != nullptr) {
        free(png_ptr->gamma_table);
        png_ptr->gamma_table = nullptr;
    }
    if (png_ptr->gamma_16_table != nullptr) {
        free(png_ptr->gamma_16_table);
        png_ptr->gamma_16_table = nullptr;
    }
    // Also free any other optional tables if allocated
    if (png_ptr->gamma_to_1 != nullptr) { free(png_ptr->gamma_to_1); png_ptr->gamma_to_1 = nullptr; }
    if (png_ptr->gamma_from_1 != nullptr) { free(png_ptr->gamma_from_1); png_ptr->gamma_from_1 = nullptr; }
    ++g_destroy_calls;
}
static void png_build_8bit_table(png_structrp png_ptr, png_bytepp ptable, png_fixed_point /*gamma_val*/) {
    // allocate a simple 256-entry 8-bit table
    *ptable = (png_bytep)malloc(256 * sizeof(png_byte));
    if (*ptable == nullptr) return;
    for (int i = 0; i < 256; ++i) {
        (*ptable)[i] = (png_byte)i; // identity-ish mapping for test
    }
}
static void png_build_16bit_table(png_structrp png_ptr, png_uint_16pp ptable,
                                 unsigned int /*shift*/, png_fixed_point /*gamma_val*/) {
    // allocate 256x256 = 65536 entries of 16-bit
    *ptable = (png_uint_16*)malloc(256 * 256 * sizeof(png_uint_16));
    if (*ptable == nullptr) return;
    for (int i = 0; i < 256 * 256; ++i) {
        (*ptable)[i] = (png_uint_16)(i & 0xFFFF);
    }
}
static void png_build_16to8_table(png_structrp png_ptr, png_uint_16pp ptable,
                                 unsigned int /*shift*/, png_fixed_point /*gamma_val*/) {
    // simple 256-entry 16->8 table
    *ptable = (png_uint_16*)malloc(256 * sizeof(png_uint_16));
    if (*ptable == nullptr) return;
    for (int i = 0; i < 256; ++i) {
        (*ptable)[i] = (png_uint_16)i;
    }
}
static png_fixed_point png_reciprocal(png_fixed_point a) {
    // Simplified reciprocal placeholder
    if (a == 0) return 0;
    return 1; // non-zero to enable test flow
}
static png_fixed_point png_reciprocal2(png_fixed_point /*a*/, png_fixed_point /*b*/) {
    return 1;
}
static void png_build_gamma_table(png_structrp png_ptr, int bit_depth);

// Implementation of the focal function (self-contained; uses the small stubs above)
static void png_build_gamma_table(png_structrp png_ptr, int bit_depth) {
{
   png_fixed_point file_gamma, screen_gamma;
   png_fixed_point correction;
#  if GAMMA_TRANSFORMS
      png_fixed_point file_to_linear, linear_to_screen;
#  endif
   png_debug(1, "in png_build_gamma_table");
   // Remove any existing table; this copes with multiple calls
   if (png_ptr->gamma_table != NULL || png_ptr->gamma_16_table != NULL) {
      png_warning(png_ptr, "gamma table being rebuilt");
      png_destroy_gamma_table(png_ptr);
   }

   file_gamma = png_ptr->file_gamma;
   screen_gamma = png_ptr->screen_gamma;
#  if GAMMA_TRANSFORMS
      file_to_linear = png_reciprocal(file_gamma);
#  endif
   if (screen_gamma > 0) {
#     if GAMMA_TRANSFORMS
         linear_to_screen = png_reciprocal(screen_gamma);
#     endif
      correction = png_reciprocal2(screen_gamma, file_gamma);
   }
   else /* screen gamma unknown */ {
#     if GAMMA_TRANSFORMS
         linear_to_screen = file_gamma;
#     endif
      correction = PNG_FP_1;
   }
   if (bit_depth <= 8) {
      png_build_8bit_table(png_ptr, &png_ptr->gamma_table, correction);
#if GAMMA_TRANSFORMS
      if ((png_ptr->transformations & (PNG_COMPOSE | PNG_RGB_TO_GRAY)) != 0) {
         png_build_8bit_table(png_ptr, &png_ptr->gamma_to_1, file_to_linear);
         png_build_8bit_table(png_ptr, &png_ptr->gamma_from_1, linear_to_screen);
      }
#endif /* GAMMA_TRANSFORMS */
   }
#ifdef PNG_16BIT_SUPPORTED
   else {
      png_byte shift, sig_bit;
      if ((png_ptr->color_type & PNG_COLOR_MASK_COLOR) != 0) {
         sig_bit = (png_byte)png_ptr->sig_bit.red;
         if (png_ptr->sig_bit.green > sig_bit)
            sig_bit = (png_byte)png_ptr->sig_bit.green;
         if (png_ptr->sig_bit.blue > sig_bit)
            sig_bit = (png_byte)png_ptr->sig_bit.blue;
      }
      else
         sig_bit = (png_byte)png_ptr->sig_bit.gray;
      if (sig_bit > 0 && sig_bit < 16U)
         shift = (png_byte)((16U - sig_bit) & 0xff);
      else
         shift = 0; /* keep all 16 bits */
      if ((png_ptr->transformations & (PNG_16_TO_8 | PNG_SCALE_16_TO_8)) != 0) {
         if (shift < (16U - PNG_MAX_GAMMA_8))
            shift = (16U - PNG_MAX_GAMMA_8);
      }
      if (shift > 8U)
         shift = 8U; /* Guarantees at least one table! */
      png_ptr->gamma_shift = shift;
      if ((png_ptr->transformations & (PNG_16_TO_8 | PNG_SCALE_16_TO_8)) != 0) {
         png_build_16to8_table(png_ptr, &png_ptr->gamma_16_table, shift,
            png_reciprocal(correction));
      } else {
         png_build_16bit_table(png_ptr, &png_ptr->gamma_16_table, shift,
            correction);
      }
#  if GAMMA_TRANSFORMS
      if ((png_ptr->transformations & (PNG_COMPOSE | PNG_RGB_TO_GRAY)) != 0) {
         png_build_16bit_table(png_ptr, &png_ptr->gamma_16_to_1, shift,
            file_to_linear);
         /* Notice that the '16 from 1' table should be full precision, however
          * the lookup on this table still uses gamma_shift, so it can't be.
          * TODO: fix this.
          */
         png_build_16bit_table(png_ptr, &png_ptr->gamma_16_from_1, shift,
            linear_to_screen);
      }
#endif /* GAMMA_TRANSFORMS */
   }
#endif /* 16BIT */
}
}

// End: Minimal in-file implementation

// Begin: Lightweight test harness (no external test framework)

// Simple non-terminating expectations
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            std::cerr << "[FAIL] " << msg << "\n"; \
            ++g_failed_tests; \
        } \
    } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        ++g_total_tests; \
        if (!((a) == (b))) { \
            std::cerr << "[FAIL] " << msg << " (expected " << (a) << " == " << (b) << ")\n"; \
            ++g_failed_tests; \
        } \
    } while (0)

static png_struct* create_png_struct() {
    png_struct* p = (png_struct*)std::calloc(1, sizeof(png_struct));
    p->gamma_table = nullptr;
    p->gamma_16_table = nullptr;
    p->gamma_to_1 = nullptr;
    p->gamma_from_1 = nullptr;
    p->file_gamma = 1;   // default non-zero
    p->screen_gamma = 1; // default positive
    p->color_type = 0;
    p->transformations = 0;
    p->sig_bit.red = p->sig_bit.green = p->sig_bit.blue = p->sig_bit.gray = 0;
    p->gamma_shift = 0;
    return p;
}
static void free_png_struct(png_struct* p) {
    if (!p) return;
    if (p->gamma_table) free(p->gamma_table);
    if (p->gamma_16_table) free(p->gamma_16_table);
    if (p->gamma_to_1) free(p->gamma_to_1);
    if (p->gamma_from_1) free(p->gamma_from_1);
    std::free(p);
}

// Test 1: Rebuild path when existing gamma tables are present
void test_gamma_rebuild_path() {
    png_struct* p = create_png_struct();
    // Pre-existing tables to trigger rebuild path
    p->gamma_table = (png_byte*)malloc(10);
    p->gamma_16_table = nullptr;
    // Trigger rebuild
    png_ptr:
    png_build_gamma_table(p, 8);
    // After rebuild, gamma_table should be allocated anew
    EXPECT_TRUE(p->gamma_table != nullptr, "gamma_table should be rebuilt and allocated");
    // gamma_16_table should remain NULL for 8-bit path
    EXPECT_TRUE(p->gamma_16_table == nullptr, "gamma_16_table should remain NULL for 8-bit path");
    // A warning should have occurred due to rebuild
    EXPECT_TRUE(g_warning_count > 0, "warning expected on gamma table rebuild");
    free_png_struct(p);
}

// Test 2: 8-bit path with transforms should create gamma_to_1 and gamma_from_1
void test_gamma_8bit_with_transforms() {
    png_struct* p = create_png_struct();
    // Ensure no existing gamma tables to avoid rebuild side-effects
    p->gamma_table = nullptr;
    p->gamma_16_table = nullptr;

    // Enable transforms and an explicit screen gamma to enter correction branch
    p->screen_gamma = 2;
    p->transformations = PNG_COMPOSE | PNG_RGB_TO_GRAY; // requires transforms block
    // Run
    png_build_gamma_table(p, 8);

    // gamma_table must be allocated
    EXPECT_TRUE(p->gamma_table != nullptr, "gamma_table should be allocated for 8-bit path");
    // Since GAMMA_TRANSFORMS is enabled, gamma_to_1 and gamma_from_1 should be created
    EXPECT_TRUE(p->gamma_to_1 != nullptr, "gamma_to_1 should be allocated when transforms enabled");
    EXPECT_TRUE(p->gamma_from_1 != nullptr, "gamma_from_1 should be allocated when transforms enabled");
    free_png_struct(p);
}

// Test 3: 8-bit path without transforms should NOT allocate gamma_to_1 / gamma_from_1
void test_gamma_8bit_without_transforms() {
    png_struct* p = create_png_struct();
    p->screen_gamma = 0; // unknown/zero
    p->transformations = 0; // no transforms
    png_build_gamma_table(p, 8);
    EXPECT_TRUE(p->gamma_table != nullptr, "gamma_table should be allocated");
    EXPECT_TRUE(p->gamma_to_1 == nullptr, "gamma_to_1 should not be allocated without transforms");
    EXPECT_TRUE(p->gamma_from_1 == nullptr, "gamma_from_1 should not be allocated without transforms");
    free_png_struct(p);
}

// Test 4: 16-bit path with PROMINENT shift calculation and 16->8 table path
void test_gamma_16bit_shift_and_table() {
    png_struct* p = create_png_struct();
    // Configure for 16-bit path
    p->file_gamma = 1;
    p->screen_gamma = 1;
    p->color_type = 0; // grayscale (no color)
    p->sig_bit.gray = 8; // triggers non-zero shift
    p->transformations = PNG_16_TO_8; // trigger 16-to-8 table path
    // bit depth 16
    png_build_gamma_table(p, 16);

    // Expect gamma_16_table allocated
    EXPECT_TRUE(p->gamma_16_table != nullptr, "gamma_16_table should be allocated for 16-bit path");
    // Expect gamma_shift to be 8 as per calculation (16 - gray)
    EXPECT_EQ((int)p->gamma_shift, 8, "gamma_shift should be computed as 8 for gray sig_bit=8");
    // In this path, gamma_to_1/gamma_from_1 may be unrelated; ensure no crash
    free_png_struct(p);
}

// Test harness runner
int main() {
    // Reset global counters
    g_warning_count = 0;
    g_destroy_calls = 0;
    g_total_tests = 0;
    g_failed_tests = 0;

    test_gamma_rebuild_path();
    test_gamma_8bit_with_transforms();
    test_gamma_8bit_without_transforms();
    test_gamma_16bit_shift_and_table();

    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << "\n";

    // Exit with non-zero if failures exist
    return (g_failed_tests == 0) ? 0 : 1;
}

// End: Test harness and focal function integration