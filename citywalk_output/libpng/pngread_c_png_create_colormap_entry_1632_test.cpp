// C++11 test suite for the focal method: png_create_colormap_entry
// This test harness provides minimal necessary scaffolding to compile and run
// in a standalone C++ environment without GTest. It reuses the focal method's
// logic by embedding a self-contained version of the function under test.
// Tests cover key branches: index bounds, 3-channel update, and 4-channel (alpha) update.

#include <vector>
#include <functional>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>


// -------------------------------------------------------------
// Minimal stand-ins for the PNG-related types and constants
// ------------------------------------------------------------

// Fixed-point type alias (simplified for testing)
typedef int png_fixed_point;

// Basic unsigned types used in the focal method
typedef unsigned int  png_uint_32;
typedef unsigned short png_uint_16;
typedef unsigned char png_byte;

// Pointers/types used in the focal method
typedef png_uint_16* png_uint_16p;
typedef png_byte*      png_bytep;
typedef void*          png_voidp;
#define png_voidcast(type, expr) ((type)(expr))

// Forward declarations for helper/test-specific functions
static void set_file_encoding(struct png_image_read_control *display);
static void png_error(void *png_ptr, const char *msg);

// Image/opaque structures used by the focal method
struct png_opaque {
    void *png_ptr; // kept abstract for testing
};
struct png_image {
    int format;
    struct png_opaque *opaque;
};
typedef struct png_image* png_imagep;

// The display/control structure used by the focal method
struct png_image_read_control {
    png_imagep image;
    int file_encoding;
    png_fixed_point gamma_to_linear;
    void *colormap; // can be 16-bit or 8-bit data depending on encoding
};

// Public constants used by the focal method
// Encoding modes
const int P_FILE   = 0;
const int P_LINEAR = 1;
const int P_sRGB   = 2;
const int P_LINEAR8 = 3;
const int P_NOTSET = -1;

// PNG format flags (simplified for testing)
const int PNG_FORMAT_FLAG_LINEAR = 0x01;
const int PNG_FORMAT_FLAG_COLOR  = 0x02;
const int PNG_FORMAT_FLAG_ALPHA  = 0x04;
const int PNG_FORMAT_FLAG_AFIRST = 0x08;
const int PNG_FORMAT_FLAG_BGR     = 0x10;

// Optional feature flags (to control code branches in the focal method)
#define PNG_FORMAT_AFIRST_SUPPORTED
#define PNG_FORMAT_BGR_SUPPORTED

// Macro to determine number of channels based on ALPHA flag
#define PNG_IMAGE_SAMPLE_CHANNELS(fmt) ( ((fmt) & PNG_FORMAT_FLAG_ALPHA) ? 4 : 3 )

// Helpers used by the focal method (simplified for testing)
static unsigned int PNG_sRGB_FROM_LINEAR(unsigned int v) { return v; }
static unsigned int PNG_DIV257(unsigned int v) { return v / 257u; }
static unsigned int png_gamma_16bit_correct(unsigned int v, int g) { (void)g; return v; }

// A tiny global 8-bit table used when encoding to sRGB.
// We fill a simple increasing sequence to have deterministic values.
static unsigned short png_sRGB_table[256];

// Simple placeholder for the error reporting function used by the focal method
static void png_error(void *png_ptr, const char *msg) {
    (void)png_ptr;
    throw std::runtime_error(std::string("png_error: ") + msg);
}

// A minimal set_file_encoding implementation: choose a deterministic default
static void set_file_encoding(struct png_image_read_control *display) {
    // For testing, pick a predictable encoding (e.g., sRGB)
    display->file_encoding = P_sRGB;
}

// -------------------------------------------------------------
// The focal method under test (adapted to be self-contained)
// -------------------------------------------------------------
png_create_colormap_entry(
    struct png_image_read_control *display,
    png_uint_32 ip,
    png_uint_32 red, png_uint_32 green, png_uint_32 blue,
    png_uint_32 alpha, int encoding)
{
{
   png_imagep image = display->image;
   int output_encoding = (image->format & PNG_FORMAT_FLAG_LINEAR) != 0 ?
       P_LINEAR : P_sRGB;
   int convert_to_Y = (image->format & PNG_FORMAT_FLAG_COLOR) == 0 &&
       (red != green || green != blue);
   if (ip > 255)
      png_error(image->opaque->png_ptr, "color-map index out of range");
   if (encoding == P_FILE)
   {
      if (display->file_encoding == P_NOTSET)
         set_file_encoding(display);
      encoding = display->file_encoding;
   }
   if (encoding == P_FILE)
   {
      png_fixed_point g = display->gamma_to_linear;
      red = png_gamma_16bit_correct(red*257, g);
      green = png_gamma_16bit_correct(green*257, g);
      blue = png_gamma_16bit_correct(blue*257, g);
      if (convert_to_Y != 0 || output_encoding == P_LINEAR)
      {
         alpha *= 257;
         encoding = P_LINEAR;
      }
      else
      {
         red = PNG_sRGB_FROM_LINEAR(red * 255);
         green = PNG_sRGB_FROM_LINEAR(green * 255);
         blue = PNG_sRGB_FROM_LINEAR(blue * 255);
         encoding = P_sRGB;
      }
   }
   else if (encoding == P_LINEAR8)
   {
      red *= 257;
      green *= 257;
      blue *= 257;
      alpha *= 257;
      encoding = P_LINEAR;
   }
   else if (encoding == P_sRGB &&
       (convert_to_Y  != 0 || output_encoding == P_LINEAR))
   {
      red = png_sRGB_table[red];
      green = png_sRGB_table[green];
      blue = png_sRGB_table[blue];
      alpha *= 257;
      encoding = P_LINEAR;
   }
   /* This is set if the color isn't gray but the output is. */
   if (encoding == P_LINEAR)
   {
      if (convert_to_Y != 0)
      {
         /* NOTE: these values are copied from png_do_rgb_to_gray */
         png_uint_32 y = (png_uint_32)6968 * red  + (png_uint_32)23434 * green +
            (png_uint_32)2366 * blue;
         if (output_encoding == P_LINEAR)
            y = (y + 16384) >> 15;
         else
         {
            /* y is scaled by 32768, we need it scaled by 255: */
            y = (y + 128) >> 8;
            y *= 255;
            y = PNG_sRGB_FROM_LINEAR((y + 64) >> 7);
            alpha = PNG_DIV257(alpha);
            encoding = P_sRGB;
         }
         blue = red = green = y;
      }
      else if (output_encoding == P_sRGB)
      {
         red = PNG_sRGB_FROM_LINEAR(red * 255);
         green = PNG_sRGB_FROM_LINEAR(green * 255);
         blue = PNG_sRGB_FROM_LINEAR(blue * 255);
         alpha = PNG_DIV257(alpha);
         encoding = P_sRGB;
      }
   }
   if (encoding != output_encoding)
      png_error(image->opaque->png_ptr, "bad encoding (internal error)");
   /* Store the value. */
   {
#     ifdef PNG_FORMAT_AFIRST_SUPPORTED
         int afirst = (image->format & PNG_FORMAT_FLAG_AFIRST) != 0 &&
            (image->format & PNG_FORMAT_FLAG_ALPHA) != 0;
#     else
#        define afirst 0
#     endif
#     ifdef PNG_FORMAT_BGR_SUPPORTED
         int bgr = (image->format & PNG_FORMAT_FLAG_BGR) != 0 ? 2 : 0;
#     else
#        define bgr 0
#     endif
      if (output_encoding == P_LINEAR)
      {
         png_uint_16p entry = png_voidcast(png_uint_16p, display->colormap);
         entry += ip * PNG_IMAGE_SAMPLE_CHANNELS(image->format);
         /* The linear 16-bit values must be pre-multiplied by the alpha channel
          * value, if less than 65535 (this is, effectively, composite on black
          * if the alpha channel is removed.)
          */
         switch (PNG_IMAGE_SAMPLE_CHANNELS(image->format))
         {
            case 4:
               entry[afirst ? 0 : 3] = (png_uint_16)alpha;
               /* FALLTHROUGH */
            case 3:
               if (alpha < 65535)
               {
                  if (alpha > 0)
                  {
                     blue = (blue * alpha + 32767U)/65535U;
                     green = (green * alpha + 32767U)/65535U;
                     red = (red * alpha + 32767U)/65535U;
                  }
                  else
                     red = green = blue = 0;
               }
               entry[afirst + (2 ^ bgr)] = (png_uint_16)blue;
               entry[afirst + 1] = (png_uint_16)green;
               entry[afirst + bgr] = (png_uint_16)red;
               break;
            case 2:
               entry[1 ^ afirst] = (png_uint_16)alpha;
               /* FALLTHROUGH */
            case 1:
               if (alpha < 65535)
               {
                  if (alpha > 0)
                     green = (green * alpha + 32767U)/65535U;
                  else
                     green = 0;
               }
               entry[afirst] = (png_uint_16)green;
               break;
            default:
               break;
         }
      }
      else /* output encoding is P_sRGB */
      {
         png_bytep entry = png_voidcast(png_bytep, display->colormap);
         entry += ip * PNG_IMAGE_SAMPLE_CHANNELS(image->format);
         switch (PNG_IMAGE_SAMPLE_CHANNELS(image->format))
         {
            case 4:
               entry[afirst ? 0 : 3] = (png_byte)alpha;
               /* FALLTHROUGH */
            case 3:
               entry[afirst + (2 ^ bgr)] = (png_byte)blue;
               entry[afirst + 1] = (png_byte)green;
               entry[afirst + bgr] = (png_byte)red;
               break;
            case 2:
               entry[1 ^ afirst] = (png_byte)alpha;
               /* FALLTHROUGH */
            case 1:
               entry[afirst] = (png_byte)green;
               break;
            default:
               break;
         }
      }
#     ifdef afirst
#        undef afirst
#     endif
#     ifdef bgr
#        undef bgr
#     endif
   }
}
}
 // end of function
}

// -------------------------------------------------------------
// Test harness setup
// ------------------------------------------------------------

// Utility: simple assertion with message
#define ASSERT_TRUE(cond, msg)                                    \
    do { if(!(cond)) {                                         \
        std::cerr << "ASSERTION FAILED: " << (msg) << "\n";     \
        throw std::runtime_error("assertion failed: " msg);    \
    } } while(0)

// Utility: initialize the 256-entry 16-bit table to deterministic values
static void init_png_sRGB_table() {
    for (int i = 0; i < 256; ++i) png_sRGB_table[i] = static_cast<unsigned short>(i);
}

// Helper to create a test image/read-control object
static void init_display_struct(png_image_read_control &display,
                              int format_flags)
{
    static struct png_opaque global_opaque;
    static struct png_image img;
    static png_image_read_control ctrl;

    // Image flags
    img.format = format_flags;
    img.opaque = &global_opaque;

    // Control/Display
    ctrl.image = &img;
    ctrl.file_encoding = P_NOTSET;
    ctrl.gamma_to_linear = 0; // identity for test
    ctrl.colormap = nullptr;

    display.image = &img;
    display.file_encoding = P_NOTSET;
    display.gamma_to_linear = 0;
    display.colormap = nullptr;
}

// Helper: allocate a colormap buffer for 256 entries with given channels
static void alloc_colormap(png_image_read_control &display, int channels)
{
    // Use 256 indices; each with 'channels' 16-bit components
    display.colormap = malloc(256 * channels * sizeof(png_uint_16));
    // Zero initialize
    std::memset(display.colormap, 0, 256 * channels * sizeof(png_uint_16));
}

// Helper: read color values from colormap for a given index
static void read_colormap_16(png_image_read_control &display, int channels,
                             unsigned int ip,
                             unsigned int &r, unsigned int &g, unsigned int &b, unsigned int &a)
{
    unsigned short* entry = static_cast<unsigned short*>(display.colormap);
    unsigned int base = ip * channels;
    if (channels >= 4) a = entry[base + 3];
    if (channels >= 3) { r = entry[base + 2]; g = entry[base + 1]; b = entry[base + 0]; }
    // For readability, values map simple (R,G,B,A) as stored by test cases
    // Note: The mapping in 3-channel path writes [R,G,B] as 0..2
    if (channels == 3) {
        r = entry[0]; g = entry[1]; b = entry[2];
        a = 0;
    }
}

// -------------------------------------------------------------
// Test cases
// -------------------------------------------------------------
void test_out_of_range_index() {
    // Test that ip > 255 triggers the internal png_error path
    png_image_read_control display;
    init_png_sRGB_table();
    init_display_struct(display, PNG_FORMAT_FLAG_LINEAR | PNG_FORMAT_FLAG_COLOR); // 3-channel linear color

    // Prepare image/opaque
    struct png_opaque o;
    o.png_ptr = (void*)0x1; // non-null pointer
    static png_image img;
    img.format = PNG_FORMAT_FLAG_LINEAR | PNG_FORMAT_FLAG_COLOR;
    img.opaque = &o;
    display.image = &img;
    display.file_encoding = P_NOTSET;
    display.gamma_to_linear = 0;
    // Colormap with enough space for 256 * 3 entries
    alloc_colormap(display, 3);

    // Expect an error for out-of-range index
    bool caught = false;
    try {
        png_create_colormap_entry(&display, 256, 10, 20, 30, 0, P_LINEAR);
    } catch (const std::exception &e) {
        caught = true;
        std::string msg = e.what();
        if (msg.find("color-map index out of range") == std::string::npos) {
            std::cerr << "Unexpected error message: " << msg << "\n";
        }
    }
    // Cleanup
    free(display.colormap);

    ASSERT_TRUE(caught, "png_create_colormap_entry should throw on ip > 255");
}

void test_three_channel_update() {
    // Test that a 3-channel (RGB) color is written correctly in linear encoding
    png_image_read_control display;
    init_png_sRGB_table();
    init_display_struct(display, PNG_FORMAT_FLAG_LINEAR | PNG_FORMAT_FLAG_COLOR); // 3 channels

    // Prepare image/opaque
    struct png_opaque o;
    o.png_ptr = (void*)0x2;
    static png_image img;
    img.format = PNG_FORMAT_FLAG_LINEAR | PNG_FORMAT_FLAG_COLOR; // 3 channels
    img.opaque = &o;
    display.image = &img;
    display.file_encoding = P_NOTSET;
    display.gamma_to_linear = 0;
    // 256 entries of 3 channels (R,G,B)
    alloc_colormap(display, 3);

    // Call function
    png_create_colormap_entry(&display, 5, 100, 150, 200, 0, P_LINEAR);

    // Read back values
    unsigned int r, g, b, a;
    read_colormap_16(display, 3, 5, r, g, b, a);

    // Expect exact values (no premultiplication since alpha = 0 and path uses direct write)
    ASSERT_TRUE(r == 100 && g == 150 && b == 200, "3-channel update should store original RGB values at index 5");
    free(display.colormap);
}

void test_four_channel_update_alpha() {
    // Test that a 4-channel (RGBA) color stores alpha in the 4th position
    png_image_read_control display;
    init_png_sRGB_table();
    init_display_struct(display, PNG_FORMAT_FLAG_LINEAR | PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_FLAG_ALPHA); // 4 channels

    // Prepare image/opaque
    struct png_opaque o;
    o.png_ptr = (void*)0x3;
    static png_image img;
    img.format = PNG_FORMAT_FLAG_LINEAR | PNG_FORMAT_FLAG_COLOR | PNG_FORMAT_FLAG_ALPHA;
    img.opaque = &o;
    display.image = &img;
    display.file_encoding = P_NOTSET;
    display.gamma_to_linear = 0;
    // 256 entries of 4 channels (R,G,B,A)
    alloc_colormap(display, 4);

    // Use an alpha value that will be written as-is (avoid premultiplication)
    png_create_colormap_entry(&display, 7, 1000, 2000, 3000, 65535, P_LINEAR);

    // Read back values
    unsigned int r, g, b, a;
    read_colormap_16(display, 4, 7, r, g, b, a);

    // Expect RGBA in order with A = 65535
    ASSERT_TRUE(r == 1000 && g == 2000 && b == 3000 && a == 65535, "4-channel update should store RGBA with alpha preserved (65535)");
    free(display.colormap);
}

// -------------------------------------------------------------
// Main: run tests
// -------------------------------------------------------------
int main() {
    try {
        test_out_of_range_index();
        std::cout << "test_out_of_range_index passed.\n";

        test_three_channel_update();
        std::cout << "test_three_channel_update passed.\n";

        test_four_channel_update_alpha();
        std::cout << "test_four_channel_update_alpha passed.\n";

        std::cout << "All tests passed.\n";
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}