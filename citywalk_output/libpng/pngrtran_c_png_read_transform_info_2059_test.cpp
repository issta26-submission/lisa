// Minimal C++11 test harness for the focal C method: png_read_transform_info
// This test suite avoids external test frameworks (no GTest) and uses a small
// custom assertion approach. It provides a self-contained mock environment to
// compile and exercise the focal function with representative scenarios.
//
// Note: The test redefines a compact subset of the PNG-related API needed to
// exercise png_read_transform_info. It aims to cover true/false branches while
// keeping the scope manageable for unit testing without the actual libpng.";

#include <vector>
#include <sstream>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>


// Domain-specific: provide an isolated, minimal environment for the focal method.
// We implement a lightweight mock of the structures and constants used by the
// function and a local copy of the function itself so tests can be executed
// in a pure C++11 environment without linking against libpng.


// 1) Dependency constants and macros (subset sufficient for tests)
#define PNG_EXPAND                 0x01
#define PNG_EXPAND_tRNS             0x02
#define PNG_COMPOSE                 0x04
#define PNG_STRIP_ALPHA             0x100
#define PNG_ADD_ALPHA               0x200
#define PNG_FILLER                  0x400
#define PNG_16_TO_8                  0x800
#define PNG_SCALE_16_TO_8            0x1000
#define PNG_PACK                    0x2000

#define PNG_COLOR_MASK_ALPHA        0x04
#define PNG_COLOR_MASK_COLOR        0x02

#define PNG_COLOR_TYPE_GRAY          0
#define PNG_COLOR_TYPE_GRAY_ALPHA    4
#define PNG_COLOR_TYPE_PALETTE       3
#define PNG_COLOR_TYPE_RGB             2
#define PNG_COLOR_TYPE_RGB_ALPHA       6

// Pretend we are building with the following capabilities (activate blocks)
#define PNG_READ_EXPAND_SUPPORTED          1
#define PNG_READ_BACKGROUND_SUPPORTED        1
#define PNG_READ_GAMMA_SUPPORTED             1
#define PNG_READ_16BIT_SUPPORTED             1
#define PNG_READ_SCALE_16_TO_8_SUPPORTED     1
#define PNG_READ_STRIP_16_TO_8_SUPPORTED     1
#define PNG_READ_EXPAND_16_SUPPORTED         1
#define PNG_READ_PACK_SUPPORTED              1
#define PNG_READ_STRIP_ALPHA_SUPPORTED        1
#define PNG_READ_GRAY_TO_RGB_SUPPORTED        1
#define PNG_READ_RGB_TO_GRAY_SUPPORTED        1
#define PNG_READ_QUANTIZE_SUPPORTED           0
#define PNG_READ_USER_TRANSFORM_SUPPORTED     0
#define PNG_USER_TRANSFORM_PTR_SUPPORTED      0

// 2) Lightweight types mimicking libpng usage (subset)
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef const png_byte* png_const_color_8p;
typedef unsigned int png_uint_32;
typedef unsigned int png_uint_16;

// Forward declare minimal png_struct and png_info structures
struct png_struct {
    unsigned int transformations;
    int num_trans;
    void* palette;
    double file_gamma;
    double background;
    // Kept for compatibility with the focal code path
    unsigned int info_rowbytes;
};
struct png_info {
    unsigned int color_type;
    unsigned int bit_depth;
    unsigned int num_trans;
    unsigned int background;
    double gamma;
    unsigned int width;
    unsigned int height;
    unsigned int channels;
    unsigned int pixel_depth;
    unsigned int rowbytes;
};

// Typedefs to mimic libpng names used by the focal function
typedef struct png_struct png_structrp;
typedef struct png_info png_inforp;

// Helper: simple rowbytes calculation (bit_depth per pixel) used in focal code
static inline unsigned int PNG_ROWBYTES(unsigned int bit_depth, unsigned int width)
{
    return (width * bit_depth + 7) / 8;
}

// 3) Minimal error/debug implementations
static void png_debug(int /*level*/, const char* msg)
{
    // Lightweight no-op logger; could be extended for verbose tests
    (void)level; (void)msg;
}

static void png_error(png_structrp /*png_ptr*/, const char* msg)
{
    // In tests, throw an exception to signal an error condition
    throw std::runtime_error(msg ? msg : "png_error");
}

// 4) Local implementation of the focal function (reproduced in this test)
static void png_read_transform_info(png_structrp png_ptr, png_inforp info_ptr)
{
    {
       png_debug(1, "in png_read_transform_info");
#ifdef PNG_READ_EXPAND_SUPPORTED
       if ((png_ptr->transformations & PNG_EXPAND) != 0)
       {
          if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
          {
             /* This check must match what actually happens in
              * png_do_expand_palette; if it ever checks the tRNS chunk to see if
              * it is all opaque we must do the same (at present it does not.)
              */
             if (png_ptr->num_trans > 0)
                info_ptr->color_type = PNG_COLOR_TYPE_RGB_ALPHA;
             else
                info_ptr->color_type = PNG_COLOR_TYPE_RGB;
             info_ptr->bit_depth = 8;
             info_ptr->num_trans = 0;
             if (png_ptr->palette == NULL)
                png_error (png_ptr, "Palette is NULL in indexed image");
          }
          else
          {
             if (png_ptr->num_trans != 0)
             {
                if ((png_ptr->transformations & PNG_EXPAND_tRNS) != 0)
                   info_ptr->color_type |= PNG_COLOR_MASK_ALPHA;
             }
             if (info_ptr->bit_depth < 8)
                info_ptr->bit_depth = 8;
             info_ptr->num_trans = 0;
          }
       }
#endif
#if defined(PNG_READ_BACKGROUND_SUPPORTED) ||\
   defined(PNG_READ_ALPHA_MODE_SUPPORTED)
       /* The following is almost certainly wrong unless the background value is in
        * the screen space!
        */
       if ((png_ptr->transformations & PNG_COMPOSE) != 0)
          info_ptr->background = png_ptr->background;
#endif
#ifdef PNG_READ_GAMMA_SUPPORTED
       /* The following used to be conditional on PNG_GAMMA (prior to 1.5.4),
        * however it seems that the code in png_init_read_transformations, which has
        * been called before this from png_read_update_info->png_read_start_row
        * sometimes does the gamma transform and cancels the flag.
        *
        * TODO: this is confusing.  It only changes the result of png_get_gAMA and,
        * yes, it does return the value that the transformed data effectively has
        * but does any app really understand this?
        */
       info_ptr->gamma = png_ptr->file_gamma;
#endif
       if (info_ptr->bit_depth == 16)
       {
#  ifdef PNG_READ_16BIT_SUPPORTED
#     ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
           if ((png_ptr->transformations & PNG_SCALE_16_TO_8) != 0)
              info_ptr->bit_depth = 8;
#     endif
#     ifdef PNG_READ_STRIP_16_TO_8_SUPPORTED
           if ((png_ptr->transformations & PNG_16_TO_8) != 0)
              info_ptr->bit_depth = 8;
#     endif
#  else
          /* No 16-bit support: force chopping 16-bit input down to 8, in this case
           * the app program can chose if both APIs are available by setting the
           * correct scaling to use.
           */
#     ifdef PNG_READ_STRIP_16_TO_8_SUPPORTED
           png_ptr->transformations |= PNG_16_TO_8;
           info_ptr->bit_depth = 8;
#     else
#        ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
            png_ptr->transformations |= PNG_SCALE_16_TO_8;
            info_ptr->bit_depth = 8;
#        else
            //CONFIGURATION ERROR: you must enable at least one 16 to 8 method
#        endif
#     endif
#endif /* !READ_16BIT */
       }
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
       if ((png_ptr->transformations & PNG_GRAY_TO_RGB) != 0)
          info_ptr->color_type = (png_byte)(info_ptr->color_type |
             PNG_COLOR_MASK_COLOR);
#endif
#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
       if ((png_ptr->transformations & PNG_RGB_TO_GRAY) != 0)
          info_ptr->color_type = (png_byte)(info_ptr->color_type &
             ~PNG_COLOR_MASK_COLOR);
#endif
#ifdef PNG_READ_QUANTIZE_SUPPORTED
       if ((png_ptr->transformations & PNG_QUANTIZE) != 0)
       {
          if (((info_ptr->color_type == PNG_COLOR_TYPE_RGB) ||
              (info_ptr->color_type == PNG_COLOR_TYPE_RGB_ALPHA)) &&
              png_ptr->palette_lookup != 0 && info_ptr->bit_depth == 8)
          {
             info_ptr->color_type = PNG_COLOR_TYPE_PALETTE;
          }
       }
#endif
#ifdef PNG_READ_EXPAND_16_SUPPORTED
       if ((png_ptr->transformations & PNG_EXPAND_16) != 0 &&
           info_ptr->bit_depth == 8 &&
           info_ptr->color_type != PNG_COLOR_TYPE_PALETTE)
       {
          info_ptr->bit_depth = 16;
       }
#endif
#ifdef PNG_READ_PACK_SUPPORTED
       if ((png_ptr->transformations & PNG_PACK) != 0 &&
           (info_ptr->bit_depth < 8))
          info_ptr->bit_depth = 8;
#endif
       if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
          info_ptr->channels = 1;
       else if ((info_ptr->color_type & PNG_COLOR_MASK_COLOR) != 0)
          info_ptr->channels = 3;
       else
          info_ptr->channels = 1;
#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
       if ((png_ptr->transformations & PNG_STRIP_ALPHA) != 0)
       {
          info_ptr->color_type = (png_byte)(info_ptr->color_type &
             ~PNG_COLOR_MASK_ALPHA);
          info_ptr->num_trans = 0;
       }
#endif
       if ((info_ptr->color_type & PNG_COLOR_MASK_ALPHA) != 0)
          info_ptr->channels++;
#ifdef PNG_READ_FILLER_SUPPORTED
       /* STRIP_ALPHA and FILLER allowed:  MASK_ALPHA bit stripped above */
       if ((png_ptr->transformations & PNG_FILLER) != 0 &&
           (info_ptr->color_type == PNG_COLOR_TYPE_RGB ||
           info_ptr->color_type == PNG_COLOR_TYPE_GRAY))
       {
          info_ptr->channels++;
          /* If adding a true alpha channel not just filler */
          if ((png_ptr->transformations & PNG_ADD_ALPHA) != 0)
             info_ptr->color_type |= PNG_COLOR_MASK_ALPHA;
       }
#endif
#if defined(PNG_USER_TRANSFORM_PTR_SUPPORTED) && \
defined(PNG_READ_USER_TRANSFORM_SUPPORTED)
       if ((png_ptr->transformations & PNG_USER_TRANSFORM) != 0)
       {
          if (png_ptr->user_transform_depth != 0)
             info_ptr->bit_depth = png_ptr->user_transform_depth;
          if (png_ptr->user_transform_channels != 0)
             info_ptr->channels = png_ptr->user_transform_channels;
       }
#endif
       info_ptr->pixel_depth = (png_byte)(info_ptr->channels *
           info_ptr->bit_depth);
       info_ptr->rowbytes = PNG_ROWBYTES(info_ptr->pixel_depth, info_ptr->width);
       /* Adding in 1.5.4: cache the above value in png_struct so that we can later
        * check in png_rowbytes that the user buffer won't get overwritten.  Note
        * that the field is not always set - if png_read_update_info isn't called
        * the application has to either not do any transforms or get the calculation
        * right itself.
        */
       png_ptr->info_rowbytes = info_ptr->rowbytes;
#ifndef PNG_READ_EXPAND_SUPPORTED
       if (png_ptr != NULL)
          return;
#endif
    }
}

// 5) Lightweight test framework (pure C++11, no external libs)
// Each test returns true on success, false on failure. A small main reports results.

static bool test_expand_palette_with_trans_not_zero() {
    // This test ensures: when expanding and palette with >0 transparency, color_type becomes RGB_ALPHA and bit_depth 8
    png_struct png_ptr;
    png_info info_ptr;
    std::memset(&png_ptr, 0, sizeof(png_ptr));
    std::memset(&info_ptr, 0, sizeof(info_ptr));

    png_ptr.transformations = PNG_EXPAND;
    png_ptr.num_trans = 1;
    png_ptr.palette = (void*)0x1234; // non-NULL

    info_ptr.color_type = PNG_COLOR_TYPE_PALETTE;
    info_ptr.bit_depth = 8;
    info_ptr.width = 4;
    info_ptr.height = 2;

    try {
        png_read_transform_info(&png_ptr, &info_ptr);
    } catch (const std::exception& ex) {
        std::cerr << "Exception in test_expand_palette_with_trans_not_zero: " << ex.what() << "\n";
        return false;
    }

    bool ok = (info_ptr.color_type == PNG_COLOR_TYPE_RGB_ALPHA) &&
              (info_ptr.bit_depth == 8) &&
              (info_ptr.num_trans == 0);
    if (!ok) {
        std::cerr << "test_expand_palette_with_trans_not_zero failed: "
                  << "color_type=" << info_ptr.color_type
                  << ", bit_depth=" << info_ptr.bit_depth
                  << ", num_trans=" << info_ptr.num_trans << "\n";
    }
    return ok;
}

static bool test_expand_palette_with_zero_trans_sets_rgb() {
    // Palette with zero transparency expands to RGB with bit_depth 8
    png_struct png_ptr;
    png_info info_ptr;
    std::memset(&png_ptr, 0, sizeof(png_ptr));
    std::memset(&info_ptr, 0, sizeof(info_ptr));

    png_ptr.transformations = PNG_EXPAND;
    png_ptr.num_trans = 0;
    png_ptr.palette = (void*)0xABCD;

    info_ptr.color_type = PNG_COLOR_TYPE_PALETTE;
    info_ptr.bit_depth = 8;
    info_ptr.width = 6;
    info_ptr.height = 4;

    try {
        png_read_transform_info(&png_ptr, &info_ptr);
    } catch (const std::exception& ex) {
        std::cerr << "Exception in test_expand_palette_with_zero_trans_sets_rgb: " << ex.what() << "\n";
        return false;
    }

    bool ok = (info_ptr.color_type == PNG_COLOR_TYPE_RGB) &&
              (info_ptr.bit_depth == 8) &&
              (info_ptr.num_trans == 0);
    if (!ok) {
        std::cerr << "test_expand_palette_with_zero_trans_sets_rgb failed: "
                  << "color_type=" << info_ptr.color_type
                  << ", bit_depth=" << info_ptr.bit_depth
                  << ", num_trans=" << info_ptr.num_trans << "\n";
    }
    return ok;
}

static bool test_strip_alpha_reduces_color_and_transparency() {
    // When STRIP_ALPHA is set, color_type loses alpha mask and num_trans becomes 0
    png_struct png_ptr;
    png_info info_ptr;
    std::memset(&png_ptr, 0, sizeof(png_ptr));
    std::memset(&info_ptr, 0, sizeof(info_ptr));

    png_ptr.transformations = PNG_STRIP_ALPHA;
    info_ptr.color_type = PNG_COLOR_TYPE_RGB_ALPHA; // includes alpha
    info_ptr.bit_depth = 8;
    info_ptr.width = 4;

    try {
        png_read_transform_info(&png_ptr, &info_ptr);
    } catch (const std::exception& ex) {
        std::cerr << "Exception in test_strip_alpha_reduces_color_and_transparency: " << ex.what() << "\n";
        return false;
    }

    bool ok = (info_ptr.color_type == PNG_COLOR_TYPE_RGB) && (info_ptr.num_trans == 0);
    if (!ok) {
        std::cerr << "test_strip_alpha_reduces_color_and_transparency failed: "
                  << "color_type=" << info_ptr.color_type
                  << ", num_trans=" << info_ptr.num_trans << "\n";
    }
    return ok;
}

static bool test_16bit_path_to_8_with_scale() {
    // When 16-bit input and SCALE_16_TO_8 flag is set, bit_depth should become 8
    png_struct png_ptr;
    png_info info_ptr;
    std::memset(&png_ptr, 0, sizeof(png_ptr));
    std::memset(&info_ptr, 0, sizeof(info_ptr));

    png_ptr.transformations = PNG_SCALE_16_TO_8;
    info_ptr.bit_depth = 16;
    info_ptr.color_type = PNG_COLOR_TYPE_RGB; // ensure not PALETTE
    info_ptr.width = 8;

    try {
        png_read_transform_info(&png_ptr, &info_ptr);
    } catch (const std::exception& ex) {
        std::cerr << "Exception in test_16bit_path_to_8_with_scale: " << ex.what() << "\n";
        return false;
    }

    bool ok = (info_ptr.bit_depth == 8);
    if (!ok) {
        std::cerr << "test_16bit_path_to_8_with_scale failed: bit_depth=" << info_ptr.bit_depth << "\n";
    }
    return ok;
}

static bool test_fill_and_add_alpha_increases_channels_and_color_type() {
    // Filler plus ADD_ALPHA should increase channels and set alpha mask
    png_struct png_ptr;
    png_info info_ptr;
    std::memset(&png_ptr, 0, sizeof(png_ptr));
    std::memset(&info_ptr, 0, sizeof(info_ptr));

    // Starting with RGB (no alpha)
    info_ptr.color_type = PNG_COLOR_TYPE_RGB;
    info_ptr.bit_depth = 8;
    info_ptr.width = 4;

    png_ptr.transformations = PNG_FILLER | PNG_ADD_ALPHA;

    try {
        png_read_transform_info(&png_ptr, &info_ptr);
    } catch (const std::exception& ex) {
        std::cerr << "Exception in test_fill_and_add_alpha_increases_channels: " << ex.what() << "\n";
        return false;
    }

    // After filler and adding alpha, color_type should have alpha bit set and channels++.
    bool ok = (info_ptr.color_type == (PNG_COLOR_TYPE_RGB | PNG_COLOR_MASK_ALPHA))
              && (info_ptr.channels == 4);
    if (!ok) {
        std::cerr << "test_fill_and_add_alpha_increases_channels failed: "
                  << "color_type=" << info_ptr.color_type
                  << ", channels=" << info_ptr.channels << "\n";
    }
    return ok;
}


// 6) Main harness
int main() {
    int failures = 0;

    // Run tests and report outcome
    if (!test_expand_palette_with_trans_not_zero()) {
        std::cout << "[FAILED] test_expand_palette_with_trans_not_zero\n";
        ++failures;
    } else {
        std::cout << "[PASSED] test_expand_palette_with_trans_not_zero\n";
    }

    if (!test_expand_palette_with_zero_trans_sets_rgb()) {
        std::cout << "[FAILED] test_expand_palette_with_zero_trans_sets_rgb\n";
        ++failures;
    } else {
        std::cout << "[PASSED] test_expand_palette_with_zero_trans_sets_rgb\n";
    }

    if (!test_strip_alpha_reduces_color_and_transparency()) {
        std::cout << "[FAILED] test_strip_alpha_reduces_color_and_transparency\n";
        ++failures;
    } else {
        std::cout << "[PASSED] test_strip_alpha_reduces_color_and_transparency\n";
    }

    if (!test_16bit_path_to_8_with_scale()) {
        std::cout << "[FAILED] test_16bit_path_to_8_with_scale\n";
        ++failures;
    } else {
        std::cout << "[PASSED] test_16bit_path_to_8_with_scale\n";
    }

    if (!test_fill_and_add_alpha_increases_channels()) {
        std::cout << "[FAILED] test_fill_and_add_alpha_increases_channels\n";
        ++failures;
    } else {
        std::cout << "[PASSED] test_fill_and_add_alpha_increases_channels\n";
    }

    if (failures > 0) {
        std::cout << "Total failures: " << failures << "\n";
        return 1;
    } else {
        std::cout << "All tests passed.\n";
        return 0;
    }
}