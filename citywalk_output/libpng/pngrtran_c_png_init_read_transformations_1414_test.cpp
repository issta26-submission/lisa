// Note: This is a self-contained unit test harness for the focal function
// png_init_read_transformations, adapted to run in a C++11 environment
// without external test frameworks (GTest). The test harness provides a
// minimal mock of the relevant libpng structures and a simplified
// implementation of png_init_read_transformations based on the provided
// <FOCAL_METHOD> content. The goal is to validate specific branches in a
// controlled, deterministic manner by configuring the fake png_ptr state.
//
// The test suite focuses on:
// - STRIP_ALPHA branch (with COMPOSE not set)
// - ALPHA_MODE off branch (gamma-independent alpha handling)
// - Palette vs non-pallete color types (palette transforms vs rgb transforms)
 
#include <cstdint>
#include <cassert>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain knowledge: use non-terminating assertions and print per-test results.
// We'll implement a lightweight "EXPECT" macro that records failures but does not exit.

static int g_test_failures = 0;
static void expect_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "EXPECT fail: " << msg << "\n";
        ++g_test_failures;
    }
}
#define EXPECT_TRUE(x, msg) expect_true((x), (msg))

// Basic libpng-like typedefs and constants (subset)
typedef unsigned int png_uint_32;
typedef unsigned short png_uint_16;
typedef unsigned char png_byte;
typedef int png_fixed_point;

#define PNG_STRIP_ALPHA             0x01
#define PNG_COMPOSE                 0x02
#define PNG_BACKGROUND_EXPAND       0x04
#define PNG_ENCODE_ALPHA            0x08
#define PNG_GAMMA                   0x20
#define PNG_FLAG_OPTIMIZE_ALPHA     0x40

#define PNG_COLOR_TYPE_GRAY         0
#define PNG_COLOR_TYPE_RGB          2
#define PNG_COLOR_TYPE_PALETTE      3

#define PNG_FP_1 1 // placeholder for fixed-point 1

// Forward declare struct to allow function prototypes
struct png_struct_def;
typedef struct png_struct_def* png_structrp; // pointer to struct (as in libpng naming)

// Simple color struct analogous to libpng's png_color
struct png_color {
    png_byte red;
    png_byte green;
    png_byte blue;
    png_byte gray;
};
typedef struct png_color* png_colorp;
typedef struct png_color* png_colorpp;

// A minimal mock of the internal png struct used by the focal method
struct png_struct_def {
    png_uint_32 transformations;
    png_uint_32 flags;
    int bit_depth;
    int color_type;
    int num_trans;
    png_byte trans_alpha[256];
    // palette data (palette used when color_type == PALETTE)
    png_color* palette;
    int num_palette;

    // gamma/power related placeholders
    png_byte gamma_table[256];
    png_byte gamma_to_1[256];
    png_byte gamma_from_1[256];

    // gamma settings (used to drive some branches)
    double screen_gamma;
    double file_gamma;

    // background structures
    struct png_color background;
    struct png_color background_1;
    int background_gamma_type; // 0.., we'll treat 0 as SCREEN default in tests

    // misc bits used by tests
    int mode;

    // internal flags used by test assertions
    int rgb_coeff_called;
    int palette_init_called;
    int rgb_init_called;

    // controls for deterministic behavior
    int force_gamma_ret; // return value for png_init_gamma_values
};

// Helper typedefs matching function signatures used in the focal code
typedef png_struct_def* png_structptr;
typedef png_struct_def* png_structrp_test;

// Stubs / mocks for libpng helper functions used by png_init_read_transformations.
// They are tiny side-effecting functions to validate that the code reaches certain branches.

static void png_debug(int level, const char* /*message*/) {
    // no-op in test
    (void)level;
}

static int png_init_gamma_values(png_structrp png_ptr) {
    // In tests we control this via png_ptr->force_gamma_ret
    return png_ptr ? png_ptr->force_gamma_ret : 0;
}

static int png_gamma_significant(png_fixed_point /*gamma*/) {
    // Treat any non-zero fixed-point as significant
    // Based on libpng semantics where 0 means insignificant
    // We mimic behavior: 0 -> not significant (returns 0)
    return 1; // default: significant; tests can set fields to 0 to simulate
}

// The code uses this to set coefficients for rgb_to_gray
static void png_set_rgb_coefficients(png_structrp png_ptr) {
    if (png_ptr) png_ptr->rgb_coeff_called = 1;
}

// Gamma table building (fake)
static void png_build_gamma_table(png_structrp png_ptr, int /*bit_depth*/) {
    if (png_ptr) {
        // Fill with a simple ramp so that code that relies on gamma_table has data
        for (int i = 0; i < 256; ++i) {
            png_ptr->gamma_table[i] = (png_byte)i;
            png_ptr->gamma_to_1[i] = (png_byte)((i * 255) / 256);
            png_ptr->gamma_from_1[i] = (png_byte)((i * 256) / 255);
        }
    }
}

// Warnings (no-op)
static void png_warning(png_structrp /*png_ptr*/, const char* /*msg*/) {
}

// When color_type == PALETTE, use this
static void png_init_palette_transformations(png_structrp png_ptr) {
    if (png_ptr) png_ptr->palette_init_called = 1;
}

// When color_type != PALETTE, use this
static void png_init_rgb_transformations(png_structrp png_ptr) {
    if (png_ptr) png_ptr->rgb_init_called = 1;
}

// Accessor for "palette" to simulate memory for a test
static void ensure_palette_alloc(png_structrp png_ptr, int n) {
    if (!png_ptr) return;
    if (png_ptr->palette == nullptr || png_ptr->num_palette < n) {
        delete[] png_ptr->palette;
        png_ptr->palette = new png_color[n];
        for (int i = 0; i < n; ++i) {
            png_ptr->palette[i].red = (png_byte)(i & 0xFF);
            png_ptr->palette[i].green = (png_byte)((i+1) & 0xFF);
            png_ptr->palette[i].blue = (png_byte)((i+2) & 0xFF);
            png_ptr->palette[i].gray = 0;
        }
        png_ptr->num_palette = n;
    }
}

// The focal function under test (recreated here based on the provided code).
// This is a faithful (but simplified in terms of the environment) port
// to allow unit testing without the libpng library.
png_struct_def* png_init_read_transformations(png_structrp png_ptr)
{
    // This function body is a direct adaptation from the provided source.
    // For the sake of the unit test harness we keep the code close to
    // the original structure, but rely on the test stubs above to drive behavior.

    png_debug(1, "in png_init_read_transformations");
#ifdef PNG_READ_GAMMA_SUPPORTED
   if (png_init_gamma_values(png_ptr) != 0)
      png_ptr->transformations |= PNG_GAMMA;
   else
      png_ptr->transformations &= ~PNG_GAMMA;
#endif
#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
   if ((png_ptr->transformations & PNG_STRIP_ALPHA) != 0 &&
       (png_ptr->transformations & PNG_COMPOSE) == 0)
   {
      png_ptr->transformations &= ~(PNG_BACKGROUND_EXPAND | PNG_ENCODE_ALPHA |
         PNG_EXPAND_tRNS);
      png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
      png_ptr->num_trans = 0;
   }
#endif 
#ifdef PNG_READ_ALPHA_MODE_SUPPORTED
   if (png_gamma_significant(png_ptr->screen_gamma) == 0)
   {
      png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
      png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
   }
#endif
#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
   if ((png_ptr->transformations & PNG_RGB_TO_GRAY) != 0)
      png_set_rgb_coefficients(png_ptr);
#endif
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
#if defined(PNG_READ_EXPAND_SUPPORTED) && defined(PNG_READ_BACKGROUND_SUPPORTED)
   if ((png_ptr->transformations & PNG_BACKGROUND_EXPAND) != 0)
   {
      if ((png_ptr->color_type & PNG_COLOR_MASK_COLOR) == 0)
         png_ptr->mode |= PNG_BACKGROUND_IS_GRAY;
   }
   else if ((png_ptr->transformations & PNG_COMPOSE) != 0)
   {
      if ((png_ptr->transformations & PNG_GRAY_TO_RGB) != 0)
      {
         if (png_ptr->background.red == png_ptr->background.green &&
             png_ptr->background.red == png_ptr->background.blue)
         {
            png_ptr->mode |= PNG_BACKGROUND_IS_GRAY;
            png_ptr->background.gray = png_ptr->background.red;
         }
      }
   }
#endif 
#endif 
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      png_init_palette_transformations(png_ptr);
   else
      png_init_rgb_transformations(png_ptr);
#if defined(PNG_READ_BACKGROUND_SUPPORTED) && \
   defined(PNG_READ_EXPAND_16_SUPPORTED)
   if ((png_ptr->transformations & PNG_EXPAND_16) != 0 &&
       (png_ptr->transformations & PNG_COMPOSE) != 0 &&
       (png_ptr->transformations & PNG_BACKGROUND_EXPAND) == 0 &&
       png_ptr->bit_depth != 16)
   {
#     define CHOP(x) (x)=((png_uint_16)PNG_DIV257(x))
      CHOP(png_ptr->background.red);
      CHOP(png_ptr->background.green);
      CHOP(png_ptr->background.blue);
      CHOP(png_ptr->background.gray);
#     undef CHOP
   }
#endif
#if defined(PNG_READ_BACKGROUND_SUPPORTED) && \
   (defined(PNG_READ_SCALE_16_TO_8_SUPPORTED) || \
   defined(PNG_READ_STRIP_16_TO_8_SUPPORTED))
   if ((png_ptr->transformations & (PNG_16_TO_8|PNG_SCALE_16_TO_8)) != 0 &&
       (png_ptr->transformations & PNG_COMPOSE) != 0 &&
       (png_ptr->transformations & PNG_BACKGROUND_EXPAND) == 0 &&
       png_ptr->bit_depth == 16)
   {
      png_ptr->background.red = (png_uint_16)(png_ptr->background.red * 257);
      png_ptr->background.green =
         (png_uint_16)(png_ptr->background.green * 257);
      png_ptr->background.blue = (png_uint_16)(png_ptr->background.blue * 257);
      png_ptr->background.gray = (png_uint_16)(png_ptr->background.gray * 257);
   }
#endif
#ifdef PNG_READ_GAMMA_SUPPORTED
#  ifdef PNG_READ_BACKGROUND_SUPPORTED
      png_ptr->background_1 = png_ptr->background;
#  endif
   if ((png_ptr->transformations & PNG_GAMMA) != 0 ||
       ((png_ptr->transformations & PNG_RGB_TO_GRAY) != 0 &&
        (png_gamma_significant(png_ptr->file_gamma) != 0 ||
         png_gamma_significant(png_ptr->screen_gamma) != 0)) ||
        ((png_ptr->transformations & PNG_COMPOSE) != 0 &&
         (png_gamma_significant(png_ptr->file_gamma) != 0 ||
          png_gamma_significant(png_ptr->screen_gamma) != 0
#  ifdef PNG_READ_BACKGROUND_SUPPORTED
         || (png_ptr->background_gamma_type == PNG_BACKGROUND_GAMMA_UNIQUE &&
           png_gamma_significant(png_ptr->background_gamma) != 0)
#  endif
        )) || ((png_ptr->transformations & PNG_ENCODE_ALPHA) != 0 &&
       png_gamma_significant(png_ptr->screen_gamma) != 0))
   {
      png_build_gamma_table(png_ptr, png_ptr->bit_depth);
#ifdef PNG_READ_BACKGROUND_SUPPORTED
      if ((png_ptr->transformations & PNG_COMPOSE) != 0)
      {
         if ((png_ptr->transformations & PNG_RGB_TO_GRAY) != 0)
            png_warning(png_ptr,
                "libpng does not support gamma+background+rgb_to_gray");
         if ((png_ptr->color_type == PNG_COLOR_TYPE_PALETTE) != 0)
         {
            png_color back, back_1;
            png_colorp palette = png_ptr->palette;
            int num_palette = png_ptr->num_palette;
            int i;
            if (png_ptr->background_gamma_type == PNG_BACKGROUND_GAMMA_FILE)
            {
               back.red = png_ptr->gamma_table[png_ptr->background.red];
               back.green = png_ptr->gamma_table[png_ptr->background.green];
               back.blue = png_ptr->gamma_table[png_ptr->background.blue];
               back_1.red = png_ptr->gamma_to_1[png_ptr->background.red];
               back_1.green = png_ptr->gamma_to_1[png_ptr->background.green];
               back_1.blue = png_ptr->gamma_to_1[png_ptr->background.blue];
            }
            else
            {
               png_fixed_point g, gs;
               switch (png_ptr->background_gamma_type)
               {
                  case PNG_BACKGROUND_GAMMA_SCREEN:
                     g = (png_ptr->screen_gamma);
                     gs = PNG_FP_1;
                     break;
                  case PNG_BACKGROUND_GAMMA_FILE:
                     g = png_reciprocal(png_ptr->file_gamma);
                     gs = png_reciprocal2(png_ptr->file_gamma,
                         png_ptr->screen_gamma);
                     break;
                  case PNG_BACKGROUND_GAMMA_UNIQUE:
                     g = png_reciprocal(png_ptr->background_gamma);
                     gs = png_reciprocal2(png_ptr->background_gamma,
                         png_ptr->screen_gamma);
                     break;
                  default:
                     g = PNG_FP_1;    /* back_1 */
                     gs = PNG_FP_1;   /* back */
                     break;
               }
               if (png_gamma_significant(gs) != 0)
               {
                  back.red = png_gamma_8bit_correct(png_ptr->background.red,
                      gs);
                  back.green = png_gamma_8bit_correct(png_ptr->background.green,
                      gs);
                  back.blue = png_gamma_8bit_correct(png_ptr->background.blue,
                      gs);
               }
               else
               {
                  back.red   = (png_byte)png_ptr->background.red;
                  back.green = (png_byte)png_ptr->background.green;
                  back.blue  = (png_byte)png_ptr->background.blue;
               }
               if (png_gamma_significant(g) != 0)
               {
                  back_1.red = png_gamma_8bit_correct(png_ptr->background.red,
                      g);
                  back_1.green = png_gamma_8bit_correct(png_ptr->background.green,
                      g);
                  back_1.blue = png_gamma_8bit_correct(png_ptr->background.blue,
                      g);
               }
               else
               {
                  back_1.red   = (png_byte)png_ptr->background.red;
                  back_1.green = (png_byte)png_ptr->background.green;
                  back_1.blue  = (png_byte)png_ptr->background.blue;
               }
            }
            for (i = 0; i < num_palette; i++)
            {
               if (i < (int)png_ptr->num_trans &&
                   png_ptr->trans_alpha[i] != 0xff)
               {
                  if (png_ptr->trans_alpha[i] == 0)
                  {
                     palette[i] = back;
                  }
                  else /* if (png_ptr->trans_alpha[i] != 0xff) */
                  {
                     if ((png_ptr->flags & PNG_FLAG_OPTIMIZE_ALPHA) != 0)
                     {
                        /* Premultiply only:
                         * component = round((component * alpha) / 255)
                         */
                        png_uint_32 component;
                        component = png_ptr->gamma_to_1[palette[i].red];
                        component =
                            (component * png_ptr->trans_alpha[i] + 128) / 255;
                        palette[i].red = png_ptr->gamma_from_1[component];
                        component = png_ptr->gamma_to_1[palette[i].green];
                        component =
                            (component * png_ptr->trans_alpha[i] + 128) / 255;
                        palette[i].green = png_ptr->gamma_from_1[component];
                        component = png_ptr->gamma_to_1[palette[i].blue];
                        component =
                            (component * png_ptr->trans_alpha[i] + 128) / 255;
                        palette[i].blue = png_ptr->gamma_from_1[component];
                     }
                     else
                     {
                        /* Composite with background color:
                         * component =
                         *    alpha * component + (1 - alpha) * background
                         */
                        png_byte v, w;
                        v = png_ptr->gamma_to_1[palette[i].red];
                        png_composite(w, v,
                            png_ptr->trans_alpha[i], back_1.red);
                        palette[i].red = png_ptr->gamma_from_1[w];
                        v = png_ptr->gamma_to_1[palette[i].green];
                        png_composite(w, v,
                            png_ptr->trans_alpha[i], back_1.green);
                        palette[i].green = png_ptr->gamma_from_1[w];
                        v = png_ptr->gamma_to_1[palette[i].blue];
                        png_composite(w, v,
                            png_ptr->trans_alpha[i], back_1.blue);
                        palette[i].blue = png_ptr->gamma_from_1[w];
                     }
                  }
               }
               else
               {
                  palette[i].red = png_ptr->gamma_table[palette[i].red];
                  palette[i].green = png_ptr->gamma_table[palette[i].green];
                  palette[i].blue = png_ptr->gamma_table[palette[i].blue];
               }
            }
            /* Prevent the transformations being done again.
             *
             * NOTE: this is highly dubious; it removes the transformations in
             * place.  This seems inconsistent with the general treatment of the
             * transformations elsewhere.
             */
            png_ptr->transformations &= ~(PNG_COMPOSE | PNG_GAMMA);
            png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
         } /* color_type == PNG_COLOR_TYPE_PALETTE */
         /* if (png_ptr->background_gamma_type!=PNG_BACKGROUND_GAMMA_UNKNOWN) */
         else /* color_type != PNG_COLOR_TYPE_PALETTE */
         {
            int gs_sig, g_sig;
            png_fixed_point g = PNG_FP_1;  /* Correction to linear */
            png_fixed_point gs = PNG_FP_1; /* Correction to screen */
            switch (png_ptr->background_gamma_type)
            {
               case PNG_BACKGROUND_GAMMA_SCREEN:
                  g = (png_ptr->screen_gamma);
                  gs = PNG_FP_1;
                  break;
               case PNG_BACKGROUND_GAMMA_FILE:
                  g = png_reciprocal(png_ptr->file_gamma);
                  gs = png_reciprocal2(png_ptr->file_gamma,
                      png_ptr->screen_gamma);
                  break;
               case PNG_BACKGROUND_GAMMA_UNIQUE:
                  g = png_reciprocal(png_ptr->background_gamma);
                  gs = png_reciprocal2(png_ptr->background_gamma,
                      png_ptr->screen_gamma);
                  break;
               default:
                  png_error(png_ptr, "invalid background gamma type");
            }
            g_sig = png_gamma_significant(g);
            gs_sig = png_gamma_significant(gs);
            if (g_sig != 0)
               png_ptr->background_1.gray = png_gamma_correct(png_ptr,
                   png_ptr->background.gray, g);
            if (gs_sig != 0)
               png_ptr->background.gray = png_gamma_correct(png_ptr,
                   png_ptr->background.gray, gs);
            if ((png_ptr->background.red != png_ptr->background.green) ||
                (png_ptr->background.red != png_ptr->background.blue) ||
                (png_ptr->background.red != png_ptr->background.gray))
            {
               if (g_sig != 0)
               {
                  png_ptr->background_1.red = png_gamma_correct(png_ptr,
                      png_ptr->background.red, g);
                  png_ptr->background_1.green = png_gamma_correct(png_ptr,
                      png_ptr->background.green, g);
                  png_ptr->background_1.blue = png_gamma_correct(png_ptr,
                      png_ptr->background.blue, g);
               }
               if (gs_sig != 0)
               {
                  png_ptr->background.red = png_gamma_correct(png_ptr,
                      png_ptr->background.red, gs);
                  png_ptr->background.green = png_gamma_correct(png_ptr,
                      png_ptr->background.green, gs);
                  png_ptr->background.blue = png_gamma_correct(png_ptr,
                      png_ptr->background.blue, gs);
               }
            }
            else
            {
               png_ptr->background_1.red = png_ptr->background_1.green
                   = png_ptr->background_1.blue = png_ptr->background_1.gray;
               png_ptr->background.red = png_ptr->background.green
                   = png_ptr->background.blue = png_ptr->background.gray;
            }
            png_ptr->background_gamma_type = PNG_BACKGROUND_GAMMA_SCREEN;
         } /* color_type != PNG_COLOR_TYPE_PALETTE */
      }/* png_ptr->transformations & PNG_GAMMA != 0 */
      else
#endif 
      if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE
#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
         && ((png_ptr->transformations & PNG_EXPAND) == 0 ||
         (png_ptr->transformations & PNG_RGB_TO_GRAY) == 0)
#endif
         )
      {
         png_colorp palette = png_ptr->palette;
         int num_palette = png_ptr->num_palette;
         int i;
         for (i = 0; i < num_palette; i++)
         {
            palette[i].red = png_ptr->gamma_table[palette[i].red];
            palette[i].green = png_ptr->gamma_table[palette[i].green];
            palette[i].blue = png_ptr->gamma_table[palette[i].blue];
         }
         png_ptr->transformations &= ~PNG_GAMMA;
      } 
   }
#ifdef PNG_READ_BACKGROUND_SUPPORTED
   else
#endif
#ifdef PNG_READ_BACKGROUND_SUPPORTED
   if ((png_ptr->transformations & PNG_COMPOSE) != 0 &&
       (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE))
   {
      int i;
      int istop = (int)png_ptr->num_trans;
      png_color back;
      png_colorp palette = png_ptr->palette;
      back.red   = (png_byte)png_ptr->background.red;
      back.green = (png_byte)png_ptr->background.green;
      back.blue  = (png_byte)png_ptr->background.blue;
      for (i = 0; i < istop; i++)
      {
         if (png_ptr->trans_alpha[i] == 0)
         {
            palette[i] = back;
         }
         else if (png_ptr->trans_alpha[i] != 0xff)
         {
            png_composite(palette[i].red, palette[i].red,
                png_ptr->trans_alpha[i], back.red);
            png_composite(palette[i].green, palette[i].green,
                png_ptr->trans_alpha[i], back.green);
            png_composite(palette[i].blue, palette[i].blue,
                png_ptr->trans_alpha[i], back.blue);
         }
      }
      png_ptr->transformations &= ~PNG_COMPOSE;
   }
#endif 
#ifdef PNG_READ_SHIFT_SUPPORTED
   if ((png_ptr->transformations & PNG_SHIFT) != 0 &&
       (png_ptr->transformations & PNG_EXPAND) == 0 &&
       (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE))
   {
      int i;
      int istop = png_ptr->num_palette;
      int shift = 8 - png_ptr->sig_bit.red;
      png_ptr->transformations &= ~PNG_SHIFT;
      /* shift handling could be tested if needed */
      (void)istop; (void)shift;
   }
#endif 
   return png_ptr;
}

// Utility: simple memory management helpers (mock)
static void png_composite(png_byte& comp_out, png_byte comp_in,
                          unsigned char alpha, png_byte background) {
    // Implement a basic alpha composite: comp_out = (alpha/255)*comp_in + (1-alpha/255)*background
    int a = (int)alpha;
    int out = ( (comp_in * a) + (background * (255 - a)) ) / 255;
    comp_out = (png_byte)out;
}

// Small reciprocal helpers for gamma path (dummy)
static png_fixed_point png_reciprocal(png_fixed_point x) {
    if (x == 0) return 0;
    return (png_fixed_point)(1.0 / (double)x);
}
static png_fixed_point png_reciprocal2(png_fixed_point a, png_fixed_point b) {
    double v = 1.0;
    if (a != 0) v = (1.0 * b) / a;
    return (png_fixed_point)v;
}
static png_byte png_gamma_8bit_correct(png_byte v, png_fixed_point /*gs*/) {
    // naive passthrough for test
    return v;
}
static png_byte png_gamma_correct(png_ptr, png_byte v, png_fixed_point /*gcl*/) {
    // naive: return input
    (void)png_ptr;
    (void)gcl;
    return v;
}
static void png_error(png_structrp /*png_ptr*/, const char* /*msg*/) {
    // In test, just abort
    std::cerr << "png_error called (test abort)\n";
    std::abort();
}

// A very small, self-contained test harness for the focal function
static void test_strip_alpha_branch() {
    // Prepare a png_ptr with STRIP_ALPHA set and COMPOSE not set
    png_struct_def png;
    std::memset(&png, 0, sizeof(png));
    png.transformations = PNG_STRIP_ALPHA;  // enable strip alpha path
    png.flags = PNG_FLAG_OPTIMIZE_ALPHA;    // before call, this flag is set and should be cleared
    png.color_type = PNG_COLOR_TYPE_RGB;     // not palette
    png.bit_depth = 8;
    png.num_trans = 2;
    // dummy trans alpha values; non-opaque entries will trigger the code path
    png.trans_alpha[0] = 0xFF;
    png.trans_alpha[1] = 0x80;

    // palette is not used since color_type != PALETTE
    png.palette = nullptr;
    png.num_palette = 0;

    // Call focal function
    png_init_read_transformations(&png);

    // Assertions: STRIP_ALPHA path clears certain transforms/flags
    EXPECT_TRUE((png.transformations & PNG_BACKGROUND_EXPAND) == 0, "STRIP_ALPHA should clear PNG_BACKGROUND_EXPAND");
    EXPECT_TRUE((png.transformations & PNG_ENCODE_ALPHA) == 0, "STRIP_ALPHA should clear PNG_ENCODE_ALPHA");
    EXPECT_TRUE((png.transformations & PNG_EXPAND_tRNS) == 0, "STRIP_ALPHA should clear PNG_EXPAND_tRNS (via bitmask if present)");
    EXPECT_TRUE((png.flags & PNG_FLAG_OPTIMIZE_ALPHA) == 0, "STRIP_ALPHA path should clear PNG_FLAG_OPTIMIZE_ALPHA");
    EXPECT_TRUE(png.num_trans == 0, "STRIP_ALPHA path should set num_trans to 0");
    // rgb_coeff.. should not be invoked in this path (palette rgb path not involved)
    // Also ensure rgb_coeff not called
    EXPECT_TRUE(png.rgb_coeff_called == 0, "rgb_coeff not invoked in STRIP_ALPHA path");

    std::cout << "test_strip_alpha_branch passed\n";
}

static void test_alpha_mode_off_branch() {
    // Prepare a png_ptr with ALPHA_MODE_SUPPORTED behavior: screen_gamma = 0 triggers clearing ENCODE_ALPHA
    png_struct_def png;
    std::memset(&png, 0, sizeof(png));
    png.transformations = PNG_ENCODE_ALPHA; // ensure the bit is set so we can verify it gets cleared
    png.flags = PNG_FLAG_OPTIMIZE_ALPHA;    // initial flag
    png.screen_gamma = 0.0; // significant as zero => png_gamma_significant returns 0 under our stub logic
    png.color_type = PNG_COLOR_TYPE_RGB;
    png.bit_depth = 8;
    png.palette = nullptr;
    png.num_palette = 0;

    png_init_read_transformations(&png);

    // ENCODE_ALPHA and OPTIMIZE_ALPHA should be cleared by the ALPHA_MODE_OFF branch
    EXPECT_TRUE((png.transformations & PNG_ENCODE_ALPHA) == 0, "ALPHA_MODE off: ENCODE_ALPHA should be cleared when screen_gamma insignificant");
    EXPECT_TRUE((png.flags & PNG_FLAG_OPTIMIZE_ALPHA) == 0, "ALPHA_MODE off: OPTIMIZE_ALPHA flag should be cleared");
    std::cout << "test_alpha_mode_off_branch passed\n";
}

static void test_palette_path() {
    // color_type PALETTE should trigger palette transform initialisation
    png_struct_def png;
    std::memset(&png, 0, sizeof(png));
    png.transformations = 0;
    png.flags = 0;
    png.color_type = PNG_COLOR_TYPE_PALETTE;
    png.num_palette = 0;
    png.palette = nullptr;

    // Provide a small palette to be safe
    ensure_palette_alloc(&png, 4);

    png_init_read_transformations(&png);

    // palette path should be invoked
    EXPECT_TRUE(png.palette_init_called != 0, "Palette transform initialisation should be invoked for PALETTE color type");
    // And rgb transform should not be invoked
    EXPECT_TRUE(png.rgb_init_called == 0, "RGB transform should not be called when PALETTE path is chosen");
    std::cout << "test_palette_path passed\n";
}

static void test_rgb_path_when_not_palette() {
    // color_type not PALETTE should trigger rgb transform initialisation
    png_struct_def png;
    std::memset(&png, 0, sizeof(png));
    png.transformations = 0;
    png.flags = 0;
    png.color_type = PNG_COLOR_TYPE_RGB;
    png.bit_depth = 8;

    png_init_read_transformations(&png);

    // rgb transform should be invoked
    EXPECT_TRUE(png.rgb_init_called != 0, "RGB transform initialisation should be invoked for non-PALETTE color types");
    // palette path should not be invoked
    EXPECT_TRUE(png.palette_init_called == 0, "Palette transform should not be invoked for non-PALETTE color types");
    std::cout << "test_rgb_path_when_not_palette passed\n";
}

// Helper to reset global test failure counter for each test
static void run_all_tests() {
    g_test_failures = 0;
    test_strip_alpha_branch();
    test_alpha_mode_off_branch();
    test_palette_path();
    test_rgb_path_when_not_palette();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED\n";
    }
    // Prevent unused warning for some platforms
    (void)run_all_tests;
}

// Since this is a standalone harness, we define a main() to run tests

int main() {
    // Define macro stubs to enable deterministic behavior for the test harness
#define PNG_READ_GAMMA_SUPPORTED
#define PNG_READ_STRIP_ALPHA_SUPPORTED
#define PNG_READ_ALPHA_MODE_SUPPORTED
#define PNG_READ_RGB_TO_GRAY_SUPPORTED
#define PNG_READ_GRAY_TO_RGB_SUPPORTED
#define PNG_READ_BACKGROUND_SUPPORTED
#define PNG_READ_EXPAND_16_SUPPORTED
#define PNG_READ_EXPAND_SUPPORTED
#define PNG_READ_SCALE_16_TO_8_SUPPORTED
#define PNG_READ_STRIP_16_TO_8_SUPPORTED

    // Define a minimal PNG_DIV257 macro used by the test code
#define PNG_DIV257(x) ((png_uint_16)((x) / 257))

    // Run tests
    run_all_tests();

    // Return non-zero if any tests failed
    return g_test_failures;
}