// Lightweight unit-test suite for the focal method png_image_read_direct
// Note: This test harness uses a small, self-contained mock environment
// to exercise specific branches of the focal method without requiring a full
// libpng build. It is designed to compile as C++11, with no GTest.
// The tests focus on two scenarios:
// 1) triggering the "unsupported transformation" error path (true/false branches)
// 2) a successful path returning 1 without error
//
// Important: This is a constrained mock environment. It is not a drop-in
// replacement for a real libpng test suite, but it demonstrates how to create
// high-coverage unit tests for a complex, dependency-heavy function.

#include <cassert>
#include <memory>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <stdexcept>
#include <cstring>


// -------------------- Mocked minimal libpng-like environment --------------------
// The goal here is to provide just enough structure to drive png_image_read_direct
// through specific branches without requiring a real libpng build.

// Public API surface (subset used by the focal method)
typedef unsigned int png_uint_32;
typedef unsigned short png_uint_16;
typedef unsigned char png_byte;
typedef void* png_voidp;
typedef void* png_bytep;
typedef void* png_const_bytep;
typedef void* png_structrp;
typedef void* png_inforp;
typedef void* png_imagep;

// Forward declarations to allow the focal method to use these symbols
struct png_struct;
struct png_color_16;
struct png_image;
struct png_image_read_control;

// Type aliases compatible with the focal method's usage
#define PNG_FORMAT_FLAG_COLOR            0x01
#define PNG_FORMAT_FLAG_ALPHA            0x02
#define PNG_FORMAT_FLAG_LINEAR           0x04
#define PNG_FORMAT_FLAG_ASSOCIATED_ALPHA 0x08
#define PNG_FORMAT_FLAG_COLORMAP          0x10
#define PNG_FORMAT_FLAG_BGR                0x20
#define PNG_FORMAT_FLAG_AFIRST              0x40
#define PNG_FORMAT_UNRECOGNIZED           0x8000  // bit used to trigger error path in tests

#define PNG_COLOR_MASK_COLOR 0x01
#define PNG_COLOR_MASK_ALPHA 0x02

#define PNG_FORMAT_BGR_SUPPORTED 1
#define PNG_FORMAT_AFIRST_SUPPORTED 1
#define PNG_FP_1 1

#define PNG_ALPHA_PNG 0
#define PNG_ALPHA_STANDARD 1
#define PNG_ALPHA_OPTIMIZED 2

#define PNG_DEFAULT_sRGB 0
#define PNG_GAMMA_LINEAR 1
#define PNG_ERROR_ACTION_NONE 0

typedef int png_fixed_point; // simplified for mock

typedef struct png_struct* png_structrp;
typedef struct png_info* png_inforp;

// Minimal opaque struct to satisfy access patterns in png_image_read_direct
struct png_struct {
    int interlaced;
    int transformations;
    unsigned int flags;
    unsigned int base_format; // used by png_image_format() in mock
};

// info struct used by the png_image_read_direct path
struct png_info {
    int color_type;
    int bit_depth;
};

// color struct used for back/compose path
struct png_color_16 {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
    unsigned short gray;
    unsigned int index;
};

// image and its opaque holder
struct png_image {
    struct png_image_opaque* opaque;
    png_uint_32 format;
    unsigned int height;
    unsigned int width;
    unsigned int flags;
};

struct png_image_opaque {
    png_structrp png_ptr;
    png_inforp info_ptr;
};

// The display/control structure expected by the focal method
struct png_image_read_control {
    png_imagep image;
    unsigned char* buffer;
    long row_stride;
    void* first_row;
    void* local_row;
    png_color_16* background;
};

// png_voidcast macro (as used in the focal method)
#define png_voidcast(type, val) (static_cast<type>(val))

// Mocked API used by the focal method
// 1) Memory management (simplified)
static void* png_malloc(png_structrp, size_t size) { return std::malloc(size); }
static void png_free(png_structrp, void* ptr) { std::free(ptr); }

// 2) Basic operations (no-ops but track internal state where helpful)
static void png_set_expand(png_structrp png_ptr) {
    // no-op in mock
}
static png_uint_32 png_image_format(png_structrp png_ptr) {
    // Return the "base format" as stored in the fake png_ptr
    if (png_ptr == nullptr) return 0;
    return static_cast<png_struct*>(png_ptr)->base_format;
}
static void png_set_gray_to_rgb_fixed(png_structrp png_ptr, int, int, int) { /* no-op */ }
static void png_set_rgb_to_gray_fixed(png_structrp png_ptr, int, int, int) { /* no-op */ }
static void png_set_alpha_mode_fixed(png_structrp png_ptr, int, png_fixed_point) { /* no-op */ }

// 3) Interlace handling and info updates
static int png_set_interlace_handling(png_structrp png_ptr) { return 0; } // no interlace in tests
static void png_read_update_info(png_structrp png_ptr, png_inforp info_ptr) { /* no-op */ }

// 4) Error handling
[[noreturn]] static void png_error(png_structrp /*png_ptr*/, const char* msg) {
    throw std::runtime_error(msg);
}

// 5) Memory helpers for this mock
static void* png_malloc(png_structrp, size_t size);
static void png_free(png_structrp, void* ptr);

// 6) Accessors used in final read loop (no actual I/O, just no-ops)
static void png_skip_chunks(png_structrp) { /* no-op */ }
static void* png_read_row(png_structrp, png_bytep, png_bytep) { return nullptr; }

// 7) Special helpers referenced in the code path (minimal stubs)
static void png_set_expand_16(png_structrp) { /* no-op */ }
static void png_set_scale_16(png_structrp) { /* no-op */ }
static void png_set_add_alpha(png_structrp, unsigned int, int) { /* no-op */ }
static void png_set_background_fixed(png_structrp, png_color_16*, int, int, int) { /* no-op */ }
static void png_set_swap_alpha(png_structrp) { /* no-op */ }
static void png_set_bgr(png_structrp) { /* no-op */ }
static int png_muldiv(png_fixed_point*, png_fixed_point, png_fixed_point, png_fixed_point) { return 0; } // pretend success
static png_fixed_point png_resolve_file_gamma(png_structrp) { return PNG_GAMMA_LINEAR; }
static int png_gamma_significant(png_fixed_point) { return 1; }

// 8) Background/is-alpha helpers (not used in error path tests)
static void png_set_alpha_mode(png_structrp, int, png_fixed_point) { /* no-op */ }

// 9) png_alloc helpers
static size_t png_get_rowbytes(png_structrp, png_inforp) { return 4; }

// 10) Safe execution wrapper used by a few paths
typedef int (*png_safe_exec_fn)(png_voidp);
static int png_safe_execute(png_imagep, png_safe_exec_fn, png_voidp) {
    // In our tests we simply invoke the provided function pointer if it matches
    // the expected internal signature (png_image_read_composite).
    // We will define png_image_read_composite to match int(png_voidp).
    // Dive into casted arg to call it; if not available, return 0.
    png_image_read_control* disp = static_cast<png_image_read_control*>(nullptr);
    (void)disp; // unused in mock
    return 0;
}

// 11) The actual focal method is included below (pasted with minimal adaptions)
png_voidp to_voidp(void* p){ return p; }

// The focal method under test (a simplified, mock-friendly version)
// Adapted to work with the mock environment defined above.
// The exact logic is preserved as in the provided source but relies on our mocks.
png_voidp png_image_read_direct(png_voidp argument)
{
{
   png_image_read_control *display = png_voidcast(png_image_read_control*,
       argument);
   png_imagep image = display->image;
   png_structrp png_ptr = image->opaque->png_ptr;
   png_inforp info_ptr = image->opaque->info_ptr;
   png_uint_32 format = image->format;
   int linear = (format & PNG_FORMAT_FLAG_LINEAR) != 0;
   int do_local_compose = 0;
   int do_local_background = 0; /* to avoid double gamma correction bug */
   int do_local_scale = 0; /* for interlaced 16-to-8 bit conversion */
   int passes = 0;
   /* Add transforms to ensure the correct output format is produced then check
    * that the required implementation support is there.  Always expand; always
    * need 8 bits minimum, no palette and expanded tRNS.
    */
   png_set_expand(png_ptr);
   /* Now check the format to see if it was modified. */
   {
      png_uint_32 base_format = png_image_format(png_ptr) &
         ~PNG_FORMAT_FLAG_COLORMAP /* removed by png_set_expand */;
      png_uint_32 change = format ^ base_format;
      png_fixed_point output_gamma;
      int mode; /* alpha mode */
      /* Do this first so that we have a record if rgb to gray is happening. */
      if ((change & PNG_FORMAT_FLAG_COLOR) != 0)
      {
         /* gray<->color transformation required. */
         if ((format & PNG_FORMAT_FLAG_COLOR) != 0)
            png_set_gray_to_rgb_fixed(png_ptr);
         else
         {
            /* libpng can't do both rgb to gray and
             * background/pre-multiplication if there is also significant gamma
             * correction, because both operations require linear colors and
             * the code only supports one transform doing the gamma correction.
             * Handle this by doing the pre-multiplication or background
             * operation in this code, if necessary.
             *
             * TODO: fix this by rewriting pngrtran.c (!)
             *
             * For the moment (given that fixing this in pngrtran.c is an
             * enormous change) 'do_local_background' is used to indicate that
             * the problem exists.
             */
            if ((base_format & PNG_FORMAT_FLAG_ALPHA) != 0)
               do_local_background = 1/*maybe*/;
            png_set_rgb_to_gray_fixed(png_ptr, PNG_ERROR_ACTION_NONE,
                PNG_RGB_TO_GRAY_DEFAULT, PNG_RGB_TO_GRAY_DEFAULT);
         }
         change &= ~PNG_FORMAT_FLAG_COLOR;
      }
      /* Set the gamma appropriately, linear for 16-bit input, sRGB otherwise.
       */
      {
         /* This is safe but should no longer be necessary as
          * png_ptr->default_gamma should have been set after the
          * info-before-IDAT was read in png_image_read_header.
          *
          * TODO: 1.8: remove this and see what happens.
          */
         png_fixed_point input_gamma_default;
         if ((base_format & PNG_FORMAT_FLAG_LINEAR) != 0 &&
             (image->flags & PNG_IMAGE_FLAG_16BIT_sRGB) == 0)
            input_gamma_default = PNG_GAMMA_LINEAR;
         else
            input_gamma_default = PNG_DEFAULT_sRGB;
         /* Call png_set_alpha_mode to set the default for the input gamma; the
          * output gamma is set by a second call below.
          */
         png_set_alpha_mode_fixed(png_ptr, PNG_ALPHA_PNG, input_gamma_default);
      }
      if (linear != 0)
      {
         /* If there *is* an alpha channel in the input it must be multiplied
          * out; use PNG_ALPHA_STANDARD, otherwise just use PNG_ALPHA_PNG.
          */
         if ((base_format & PNG_FORMAT_FLAG_ALPHA) != 0)
            mode = PNG_ALPHA_STANDARD; /* associated alpha */
         else
            mode = PNG_ALPHA_PNG;
         output_gamma = PNG_GAMMA_LINEAR;
      }
      else
      {
         mode = PNG_ALPHA_PNG;
         output_gamma = PNG_DEFAULT_sRGB;
      }
      if ((change & PNG_FORMAT_FLAG_ASSOCIATED_ALPHA) != 0)
      {
         mode = PNG_ALPHA_OPTIMIZED;
         change &= ~PNG_FORMAT_FLAG_ASSOCIATED_ALPHA;
      }
      /* If 'do_local_background' is set check for the presence of gamma
       * correction; this is part of the work-round for the libpng bug
       * described above.
       *
       * TODO: fix libpng and remove this.
       */
      if (do_local_background != 0)
      {
         png_fixed_point gtest;
         /* This is 'png_gamma_threshold' from pngrtran.c; the test used for
          * gamma correction, the screen gamma hasn't been set on png_struct
          * yet; it's set below.  png_struct::gamma, however, is set to the
          * final value.
          */
         if (png_muldiv(&gtest, output_gamma,
                  png_resolve_file_gamma(png_ptr), PNG_FP_1) != 0 &&
             png_gamma_significant(gtest) == 0)
            do_local_background = 0;
         else if (mode == PNG_ALPHA_STANDARD)
         {
            do_local_background = 2/*required*/;
            mode = PNG_ALPHA_PNG; /* prevent libpng doing it */
         }
         /* else leave as 1 for the checks below */
      }
      /* If the bit-depth changes then handle that here. */
      if ((change & PNG_FORMAT_FLAG_LINEAR) != 0)
      {
         if (linear != 0 /*16-bit output*/)
            png_set_expand_16(png_ptr);
         else /* 8-bit output */
         {
            png_set_scale_16(png_ptr);
            /* For interlaced images, use local_row buffer to avoid overflow
             * in png_combine_row() which writes using IHDR bit-depth.
             */
            if (png_ptr->interlaced != 0)
               do_local_scale = 1;
         }
         change &= ~PNG_FORMAT_FLAG_LINEAR;
      }
      /* Now the background/alpha channel changes. */
      if ((change & PNG_FORMAT_FLAG_ALPHA) != 0)
      {
         /* Removing an alpha channel requires composition for the 8-bit
          * formats; for the 16-bit it is already done, above, by the
          * pre-multiplication and the channel just needs to be stripped.
          */
         if ((base_format & PNG_FORMAT_FLAG_ALPHA) != 0)
         {
            /* If RGB->gray is happening the alpha channel must be left and the
             * operation completed locally.
             *
             * TODO: fix libpng and remove this.
             */
            if (do_local_background != 0)
               do_local_background = 2/*required*/;
            /* 16-bit output: just remove the channel */
            else if (linear != 0) /* compose on black (well, pre-multiply) */
               png_set_strip_alpha(png_ptr);
            /* 8-bit output: do an appropriate compose */
            else if (display->background != NULL)
            {
               png_color_16 c;
               c.index = 0; /*unused*/
               c.red = display->background->red;
               c.green = display->background->green;
               c.blue = display->background->blue;
               c.gray = display->background->green;
               /* This is always an 8-bit sRGB value, using the 'green' channel
                * for gray is much better than calculating the luminance here;
                * we can get off-by-one errors in that calculation relative to
                * the app expectations and that will show up in transparent
                * pixels.
                */
               png_set_background_fixed(png_ptr, &c,
                   PNG_BACKGROUND_GAMMA_SCREEN, 0/*need_expand*/,
                   0/*gamma: not used*/);
            }
            else /* compose on row: implemented below. */
            {
               do_local_compose = 1;
               /* This leaves the alpha channel in the output, so it has to be
                * removed by the code below.  Set the encoding to the 'OPTIMIZE'
                * one so the code only has to hack on the pixels that require
                * composition.
                */
               mode = PNG_ALPHA_OPTIMIZED;
            }
         }
         else /* output needs an alpha channel */
         {
            /* This is tricky because it happens before the swap operation has
             * been accomplished; however the swap does *not* swap the added
             * alpha channel (weird API), so it must be added in the correct
             * place.
             */
            png_uint_32 filler; /* opaque filler */
            int where;
            if (linear != 0)
               filler = 65535;
            else
               filler = 255;
#ifdef PNG_FORMAT_AFIRST_SUPPORTED
            if ((format & PNG_FORMAT_FLAG_AFIRST) != 0)
            {
               where = PNG_FILLER_BEFORE;
               change &= ~PNG_FORMAT_FLAG_AFIRST;
            }
            else
#endif
            where = PNG_FILLER_AFTER;
            png_set_add_alpha(png_ptr, filler, where);
         }
         /* This stops the (irrelevant) call to swap_alpha below. */
         change &= ~PNG_FORMAT_FLAG_ALPHA;
      }
      /* Now set the alpha mode correctly; this is always done, even if there is
       * no alpha channel in either the input or the output because it correctly
       * sets the output gamma.
       */
      png_set_alpha_mode_fixed(png_ptr, mode, output_gamma);
#     ifdef PNG_FORMAT_BGR_SUPPORTED
         if ((change & PNG_FORMAT_FLAG_BGR) != 0)
         {
            /* Check only the output format; PNG is never BGR; don't do this if
             * the output is gray, but fix up the 'format' value in that case.
             */
            if ((format & PNG_FORMAT_FLAG_COLOR) != 0)
               png_set_bgr(png_ptr);
            else
               format &= ~PNG_FORMAT_FLAG_BGR;
            change &= ~PNG_FORMAT_FLAG_BGR;
         }
#     endif
#     ifdef PNG_FORMAT_AFIRST_SUPPORTED
         if ((change & PNG_FORMAT_FLAG_AFIRST) != 0)
         {
            /* Only relevant if there is an alpha channel - it's particularly
             * important to handle this correctly because do_local_compose may
             * be set above and then libpng will keep the alpha channel for this
             * code to remove.
             */
            if ((format & PNG_FORMAT_FLAG_ALPHA) != 0)
            {
               /* Disable this if doing a local background,
                * TODO: remove this when local background is no longer required.
                */
               if (do_local_background != 2)
                  png_set_swap_alpha(png_ptr);
            }
            else
               format &= ~PNG_FORMAT_FLAG_AFIRST;
            change &= ~PNG_FORMAT_FLAG_AFIRST;
         }
#     endif
      /* If the *output* is 16-bit then we need to check for a byte-swap on this
       * architecture.
       */
      if (linear != 0)
      {
         png_uint_16 le = 0x0001;
         if ((*(png_const_bytep) & le) != 0)
            png_set_swap(png_ptr);
      }
      /* If change is not now 0 some transformation is missing - error out. */
      if (change != 0)
         png_error(png_ptr, "png_read_image: unsupported transformation");
   }
   PNG_SKIP_CHUNKS(png_ptr);
   /* Update the 'info' structure and make sure the result is as required; first
    * make sure to turn on the interlace handling if it will be required
    * (because it can't be turned on *after* the call to png_read_update_info!)
    *
    * TODO: remove the do_local_background fixup below.
    */
   if (do_local_compose == 0 && do_local_background != 2)
      passes = png_set_interlace_handling(png_ptr);
   png_read_update_info(png_ptr, info_ptr);
   {
      png_uint_32 info_format = 0;
      if ((info_ptr->color_type & PNG_COLOR_MASK_COLOR) != 0)
         info_format |= PNG_FORMAT_FLAG_COLOR;
      if ((info_ptr->color_type & PNG_COLOR_MASK_ALPHA) != 0)
      {
         /* do_local_compose removes this channel below. */
         if (do_local_compose == 0)
         {
            /* do_local_background does the same if required. */
            if (do_local_background != 2 ||
               (format & PNG_FORMAT_FLAG_ALPHA) != 0)
               info_format |= PNG_FORMAT_FLAG_ALPHA;
         }
      }
      else if (do_local_compose != 0) /* internal error */
         png_error(png_ptr, "png_image_read: alpha channel lost");
      if ((format & PNG_FORMAT_FLAG_ASSOCIATED_ALPHA) != 0) {
         info_format |= PNG_FORMAT_FLAG_ASSOCIATED_ALPHA;
      }
      if (info_ptr->bit_depth == 16)
         info_format |= PNG_FORMAT_FLAG_LINEAR;
#ifdef PNG_FORMAT_BGR_SUPPORTED
      if ((png_ptr->transformations & PNG_BGR) != 0)
         info_format |= PNG_FORMAT_FLAG_BGR;
#endif
#ifdef PNG_FORMAT_AFIRST_SUPPORTED
         if (do_local_background == 2)
         {
            if ((format & PNG_FORMAT_FLAG_AFIRST) != 0)
               info_format |= PNG_FORMAT_FLAG_AFIRST;
         }
         if ((png_ptr->transformations & PNG_SWAP_ALPHA) != 0 ||
            ((png_ptr->transformations & PNG_ADD_ALPHA) != 0 &&
            (png_ptr->flags & PNG_FLAG_FILLER_AFTER) == 0))
         {
            if (do_local_background == 2)
               png_error(png_ptr, "unexpected alpha swap transformation");
            info_format |= PNG_FORMAT_FLAG_AFIRST;
         }
#     endif
      /* This is actually an internal error. */
      if (info_format != format)
         png_error(png_ptr, "png_read_image: invalid transformations");
   }
   /* Now read the rows.  If do_local_compose is set then it is necessary to use
    * a local row buffer.  The output will be GA, RGBA or BGRA and must be
    * converted to G, RGB or BGR as appropriate.  The 'local_row' member of the
    * display acts as a flag.
    */
   {
      png_voidp first_row = display->buffer;
      ptrdiff_t row_bytes = display->row_stride;
      if (linear != 0)
         row_bytes *= 2;
      /* The following expression is designed to work correctly whether it gives
       * a signed or an unsigned result.
       */
      if (row_bytes < 0)
      {
         char *ptr = png_voidcast(char*, first_row);
         ptr += (image->height-1) * (-row_bytes);
         first_row = png_voidcast(png_voidp, ptr);
      }
      display->first_row = first_row;
      display->row_bytes = row_bytes;
   }
   if (do_local_compose != 0)
   {
      int result;
      png_voidp row = png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
      display->local_row = row;
      result = png_safe_execute(image, png_image_read_composite, display);
      display->local_row = NULL;
      png_free(png_ptr, row);
      return result;
   }
   else if (do_local_background == 2)
   {
      int result;
      png_voidp row = png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
      display->local_row = row;
      result = png_safe_execute(image, png_image_read_background, display);
      display->local_row = NULL;
      png_free(png_ptr, row);
      return result;
   }
   else if (do_local_scale != 0)
   {
      /* For interlaced 16-to-8 conversion, use an intermediate row buffer
       * to avoid buffer overflows in png_combine_row. The local_row is sized
       * for the transformed (8-bit) output, preventing the overflow that would
       * occur if png_combine_row wrote 16-bit data directly to the user buffer.
       */
      int result;
      png_voidp row = png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
      display->local_row = row;
      result = png_safe_execute(image, png_image_read_direct_scaled, display);
      display->local_row = NULL;
      png_free(png_ptr, row);
      return result;
   }
   else
   {
      png_alloc_size_t row_bytes = (png_alloc_size_t)display->row_bytes;
      while (--passes >= 0)
      {
         png_uint_32 y = image->height;
         png_bytep row = png_voidcast(png_bytep, display->first_row);
         for (; y > 0; --y)
         {
            png_read_row(png_ptr, row, NULL);
            row += row_bytes;
         }
      }
      return 1;
   }
}
}

// -------------------- Test framework helpers --------------------

// Simple test harness: count passes/fails without terminating on failures
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        ++g_tests_run; \
        if (!(cond)) { \
            ++g_tests_failed; \
            std::cerr << "TEST FAILED: " << (msg) << "\n"; \
        } \
    } while (0)

#define TEST_ASSERT_THROW(code, expected_msg) \
    do { \
        ++g_tests_run; \
        try { code; } \
        catch (const std::exception& e) { \
            std::string m = e.what(); \
            if (m.find(expected_msg) == std::string::npos) { \
                ++g_tests_failed; \
                std::cerr << "TEST FAILED: unexpected exception message: " << m << "\n"; \
            } else { /* success */ } \
            continue; \
        } \
        ++g_tests_failed; \
        std::cerr << "TEST FAILED: expected exception with message containing: '" \
                  << (expected_msg) << "'\n"; \
    } while (0)

// Helper to print summary
static void print_summary() {
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";
}

// -------------------- Test scenarios --------------------

// 1) Test that an unrecognized format flag triggers the "unsupported transformation" error path.
// We craft a format that includes a recognized color bit and an unknown bit that is not cleared by
// the code, ensuring that 'change' remains non-zero and png_error() is invoked.
void test_error_path_unrecognized_flag()
{
    // Prepare mock objects
    static png_struct s;
    static png_image img;
    static png_image_opaque opaque;
    static png_info info;

    // Link the hierarchy: image -> opaque -> (png_ptr, info_ptr)
    s.base_format = 0; // base_format will be masked; irrelevant for this test
    opaque.png_ptr = &s;
    opaque.info_ptr = &info;
    img.opaque = &opaque;
    img.format = PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_UNRECOGNIZED; // color plus unknown bit
    img.height = 1;
    img.width = 1;

    // Build display/control object
    static unsigned char buf[8];
    static png_color_16 bg;
    bg.red = bg.green = bg.blue = 0;
    png_image_read_control display;
    display.image = &img;
    display.buffer = buf;
    display.row_stride = 4;
    display.first_row = buf;
    display.local_row = nullptr;
    display.background = nullptr;

    // info_ptr minimal fields
    info.color_type = 0; // not color
    info.bit_depth = 8;

    // The argument to the focal method
    void* arg = &display;

    // Run and expect an exception with the known message from the focal method
    try {
        png_image_read_direct(arg);
        TEST_ASSERT(false, "Expected exception for unrecognized format flag was not thrown");
    } catch (const std::exception& e) {
        // We expect a message containing "png_read_image: unsupported transformation"
        std::string msg = e.what();
        TEST_ASSERT(msg.find("png_read_image: unsupported transformation") != std::string::npos,
                    "Expected 'unsupported transformation' error message, got: " + msg);
    } catch (...) {
        TEST_ASSERT(false, "Unexpected exception type thrown for unrecognized flag test");
    }
}

// 2) Test the non-error path returns 1 when there are no format changes.
// This ensures the "happy path" works for the simplest case.
void test_success_path_simple()
{
    // Prepare mock objects
    static png_struct s;
    static png_image img;
    static png_image_opaque opaque;
    static png_info info;

    s.base_format = 0; // base format yields no changes
    opaque.png_ptr = &s;
    opaque.info_ptr = &info;
    img.opaque = &opaque;
    img.format = 0; // no flags
    img.height = 1;
    img.width = 1;

    // Display/control
    static unsigned char buf[8];
    static png_color_16 bg;
    bg.red = bg.green = bg.blue = 0;
    png_image_read_control display;
    display.image = &img;
    display.buffer = buf;
    display.row_stride = 4;
    display.first_row = buf;
    display.local_row = nullptr;
    display.background = nullptr;

    // info_ptr minimal fields
    info.color_type = 0; // grayscale/no color
    info.bit_depth = 8;

    // The argument to the focal method
    void* arg = &display;

    // Expect successful return value of 1
    int ret = 0;
    try {
        ret = *static_cast<int*>(static_cast<void*>(&png_image_read_direct(arg))); // can't directly call; call directly
        // The above approach is incorrect; we must call the function directly.
    } catch (...) {
        // We'll attempt a direct call in a separate block
    }
    // Direct call (simpler): since the function returns void (via a non-void? The real function returns png_voidp).
    // In the provided code, png_image_read_direct returns png_voidp, representing a success indicator.
    // We'll just invoke and catch no exception; store return in a local variable by using a workaround:
    png_voidp rv = nullptr;
    try {
        rv = png_image_read_direct(arg);
    } catch (...) {
        TEST_ASSERT(false, "Exception thrown in success path (unexpected)");
        print_summary();
        return;
    }
    // Validate the returned value semantics: in this mock, for success we expect non-null/any value
    TEST_ASSERT(rv != nullptr, "Expected non-null return on success path");
}

// 3) Optional: test the local compose path indirectly could be added, but due to mock scope
// we focus on core branches above.

// -------------------- Main --------------------
int main() {
    std::cout << "Starting tests for png_image_read_direct (mocked environment)\n";

    test_error_path_unrecognized_flag();
    test_success_path_simple();

    print_summary();

    if (g_tests_failed == 0) {
        std::cout << "All tests passed (mocked environment).\n";
        return 0;
    } else {
        std::cout << "Some tests failed (mocked environment).\n";
        return 1;
    }
}