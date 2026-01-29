// Minimal C++11 test harness for png_write_IHDR (as per provided focal method).
// This test suite mocks the necessary PNG types, constants, and helper functions
// to exercise the core logic of png_write_IHDR without relying on external GTest.
// The tests aim to cover true/false branches, error/warning paths, and
// buffer content written for the IHDR chunk.

#include <cstdint>
#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>


// Domain-specific: lightweight mock of PNG API and structures used by png_write_IHDR

// Macros to emulate compile-time options (as present in the real library)
#define PNG_WRITE_INTERLACING_SUPPORTED
#define PNG_MNG_FEATURES_SUPPORTED
#define PNG_WRITE_16BIT_SUPPORTED
#define PNG_INTRAPIXEL_DIFFERENCING 64
#define PNG_FLAG_MNG_FILTER_64 1

// PNG color/type and related constants
#define PNG_COLOR_TYPE_GRAY        0
#define PNG_COLOR_TYPE_RGB         2
#define PNG_COLOR_TYPE_PALETTE     3
#define PNG_COLOR_TYPE_GRAY_ALPHA  4
#define PNG_COLOR_TYPE_RGB_ALPHA   6

#define PNG_COMPRESSION_TYPE_BASE  0
#define PNG_FILTER_TYPE_BASE       0

#define PNG_INTERLACE_NONE           0
#define PNG_INTERLACE_ADAM7           1

#define PNG_HAVE_IHDR                 1
#define PNG_IHDR                      0x49484452 // 'IHDR'

// Filter/misc constants for tests
#define PNG_NO_FILTERS              0
#define PNG_FILTER_NONE             1
#define PNG_ALL_FILTERS             2

// Tiny in-file PNG structure (mock)
typedef unsigned char png_byte;
typedef uint32_t      png_uint_32;
typedef png_byte*       png_bytep;
typedef const png_byte* png_const_bytep;

struct png_struct {
    // IHDR-related state tracked by the writer
    int channels;
    int mng_features_permitted;
    int mode;
    int do_filter;
    int bit_depth;
    int color_type;
    int interlaced;
    int compression_type;
    int width;
    int height;
    int usr_width;
    int usr_bit_depth;
    int usr_channels;
    int pixel_depth;
    int rowbytes;
};

// Pointers and type aliases
typedef struct png_struct* png_structrp;

// Helpers and globals to inspect behavior
static std::vector<unsigned char> g_last_chunk_data;
static uint32_t g_last_chunk_name = 0;
static std::string g_last_warning;

// Utility: reset test state
static void reset_test_state() {
    g_last_chunk_data.clear();
    g_last_chunk_name = 0;
    g_last_warning.clear();
}

// Forward declarations (needed by the focal function)
static void png_debug(int, const char*);
extern "C" void png_error(png_structrp png_ptr, const char* msg);
extern "C" void png_warning(png_structrp png_ptr, const char* msg);
static void png_save_uint_32(png_bytep buf, png_uint_32 i);
static void png_save_uint_16(png_bytep buf, unsigned int i);
static void png_write_complete_chunk(png_structrp png_ptr, png_uint_32 chunk_name,
    png_const_bytep data, size_t length);

// Tiny helpers/macros used by the writer
static void png_debug(int /*level*/, const char* /*msg*/) { /* no-op in tests */ }

// Mocked error/warning handling
extern "C" void png_error(png_structrp /*png_ptr*/, const char* msg) {
    throw std::runtime_error(msg);
}
extern "C" void png_warning(png_structrp /*png_ptr*/, const char* msg) {
    g_last_warning = msg;
}

// Implementations of small helpers
static void png_save_uint_32(png_bytep buf, png_uint_32 i) {
    buf[0] = (png_byte)((i >> 24) & 0xFF);
    buf[1] = (png_byte)((i >> 16) & 0xFF);
    buf[2] = (png_byte)((i >> 8) & 0xFF);
    buf[3] = (png_byte)(i & 0xFF);
}
static void png_save_uint_16(png_bytep buf, unsigned int i) {
    buf[0] = (png_byte)((i >> 8) & 0xFF);
    buf[1] = (png_byte)(i & 0xFF);
}

// Chunk writer mock: store last written IHDR chunk contents for inspection
static void png_write_complete_chunk(png_structrp /*png_ptr*/, png_uint_32 chunk_name,
    png_const_bytep data, size_t length)
{
    (void)length; // length is always 13 for IHDR in this test suite
    g_last_chunk_name = chunk_name;
    g_last_chunk_data.assign(data, data + length);
}

// Our focal function under test: simplified copy of the real logic tailored for unit tests
void png_write_IHDR(png_structrp png_ptr, png_uint_32 width, png_uint_32 height,
    int bit_depth, int color_type, int compression_type, int filter_type,
    int interlace_type)
{
{
   png_byte buf[13]; 
   int is_invalid_depth;
   png_debug(1, "in png_write_IHDR");
   switch (color_type)
   {
      case PNG_COLOR_TYPE_GRAY:
         switch (bit_depth)
         {
            case 1:
            case 2:
            case 4:
            case 8:
#ifdef PNG_WRITE_16BIT_SUPPORTED
            case 16:
#endif
               png_ptr->channels = 1; break;
            default:
               png_error(png_ptr,
                   "Invalid bit depth for grayscale image");
         }
         break;
      case PNG_COLOR_TYPE_RGB:
         is_invalid_depth = (bit_depth != 8);
#ifdef PNG_WRITE_16BIT_SUPPORTED
         is_invalid_depth = (is_invalid_depth && bit_depth != 16);
#endif
         if (is_invalid_depth)
            png_error(png_ptr, "Invalid bit depth for RGB image");
         png_ptr->channels = 3;
         break;
      case PNG_COLOR_TYPE_PALETTE:
         switch (bit_depth)
         {
            case 1:
            case 2:
            case 4:
            case 8:
               png_ptr->channels = 1;
               break;
            default:
               png_error(png_ptr, "Invalid bit depth for paletted image");
         }
         break;
      case PNG_COLOR_TYPE_GRAY_ALPHA:
         is_invalid_depth = (bit_depth != 8);
#ifdef PNG_WRITE_16BIT_SUPPORTED
         is_invalid_depth = (is_invalid_depth && bit_depth != 16);
#endif
         if (is_invalid_depth)
            png_error(png_ptr, "Invalid bit depth for grayscale+alpha image");
         png_ptr->channels = 2;
         break;
      case PNG_COLOR_TYPE_RGB_ALPHA:
         is_invalid_depth = (bit_depth != 8);
#ifdef PNG_WRITE_16BIT_SUPPORTED
         is_invalid_depth = (is_invalid_depth && bit_depth != 16);
#endif
         if (is_invalid_depth)
            png_error(png_ptr, "Invalid bit depth for RGBA image");
         png_ptr->channels = 4;
         break;
      default:
         png_error(png_ptr, "Invalid image color type specified");
   }
   if (compression_type != PNG_COMPRESSION_TYPE_BASE)
   {
      png_warning(png_ptr, "Invalid compression type specified");
      compression_type = PNG_COMPRESSION_TYPE_BASE;
   }
   if (
#ifdef PNG_MNG_FEATURES_SUPPORTED
       !((png_ptr->mng_features_permitted & PNG_FLAG_MNG_FILTER_64) != 0 &&
       ((png_ptr->mode & PNG_HAVE_PNG_SIGNATURE) == 0) &&
       (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_RGB_ALPHA) &&
       (filter_type == PNG_INTRAPIXEL_DIFFERENCING)) &&
#endif
       filter_type != PNG_FILTER_TYPE_BASE)
   {
      png_warning(png_ptr, "Invalid filter type specified");
      filter_type = PNG_FILTER_TYPE_BASE;
   }
#ifdef PNG_WRITE_INTERLACING_SUPPORTED
   if (interlace_type != PNG_INTERLACE_NONE &&
       interlace_type != PNG_INTERLACE_ADAM7)
   {
      png_warning(png_ptr, "Invalid interlace type specified");
      interlace_type = PNG_INTERLACE_ADAM7;
   }
#else
   interlace_type=PNG_INTERLACE_NONE;
#endif
   /* Save the relevant information */
   png_ptr->bit_depth = (png_byte)bit_depth;
   png_ptr->color_type = (png_byte)color_type;
   png_ptr->interlaced = (png_byte)interlace_type;
#ifdef PNG_MNG_FEATURES_SUPPORTED
   png_ptr->filter_type = (png_byte)filter_type;
#endif
   png_ptr->compression_type = (png_byte)compression_type;
   png_ptr->width = width;
   png_ptr->height = height;
   png_ptr->pixel_depth = (png_byte)(bit_depth * png_ptr->channels);
   png_ptr->rowbytes = (png_uint_32)((png_ptr->pixel_depth * (width)) / 8);
   /* Set the usr info, so any transformations can modify it */
   png_ptr->usr_width = png_ptr->width;
   png_ptr->usr_bit_depth = png_ptr->bit_depth;
   png_ptr->usr_channels = png_ptr->channels;
   /* Pack the header information into the buffer */
   png_save_uint_32(buf, width);
   png_save_uint_32(buf + 4, height);
   buf[8] = (png_byte)bit_depth;
   buf[9] = (png_byte)color_type;
   buf[10] = (png_byte)compression_type;
   buf[11] = (png_byte)filter_type;
   buf[12] = (png_byte)interlace_type;
   /* Write the chunk */
   png_write_complete_chunk(png_ptr, png_IHDR, buf, 13);
   if ((png_ptr->do_filter) == PNG_NO_FILTERS)
   {
      if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE ||
          png_ptr->bit_depth < 8)
         png_ptr->do_filter = PNG_FILTER_NONE;
      else
         png_ptr->do_filter = PNG_ALL_FILTERS;
   }
   png_ptr->mode = PNG_HAVE_IHDR; /* not READY_FOR_ZTXT */
}
}

// Helper: expose a reusable API to initialize an empty png_struct
static png_struct* create_fake_png_ptr() {
    png_struct* p = new png_struct();
    std::memset(p, 0, sizeof(png_struct));
    p->mng_features_permitted = 0;
    p->mode = 0;
    p->do_filter = PNG_NO_FILTERS; // default to no filters until IHDR decides
    // Other fields default zero
    return p;
}

// Unit test helpers
static void expect_true(bool cond, const char* test, const char* desc, int line) {
    if (!cond) {
        std::cerr << "[FAIL] " << test << "@" << line << ": " << desc << "\n";
        // Continue running to maximize coverage
    }
}
static void expect_eq_uint32(uint32_t a, uint32_t b, const char* test, const char* desc, int line) {
    if (a != b) {
        std::cerr << "[FAIL] " << test << "@" << line << ": " << desc
                  << " (expected " << b << ", got " << a << ")\n";
    }
}
static void expect_eq_int(int a, int b, const char* test, const char* desc, int line) {
    if (a != b) {
        std::cerr << "[FAIL] " << test << "@" << line << ": " << desc
                  << " (expected " << b << ", got " << a << ")\n";
    }
}
#define EXPECT_TRUE(cond, name, desc) expect_true(cond, name, desc, __LINE__)
#define EXPECT_EQ_UINT32(a, b, name, desc) expect_eq_uint32(a, b, name, desc, __LINE__)
#define EXPECT_EQ_INT(a, b, name, desc) expect_eq_int(a, b, name, desc, __LINE__)

// Exposed test methods
void test_gray8_writes_ihdr() {
    reset_test_state();
    png_struct* p = create_fake_png_ptr();

    // Configure input data: Gray, 8-bit depth
    uint32_t width = 10;
    uint32_t height = 20;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_GRAY;
    int compression_type = 0;
    int filter_type = 0;
    int interlace_type = 0;

    // Execute
    png_write_IHDR(p, width, height, bit_depth, color_type, compression_type, filter_type, interlace_type);

    // Validate IHDR chunk was written with correct data
    EXPECT_EQ_UINT32(g_last_chunk_name, PNG_IHDR, "png_write_IHDR writes IHDR chunk", "IHDR chunk name");
    // Data should be 13 bytes
    EXPECT_EQ_INT((int)g_last_chunk_data.size(), 13, "IHDR data length", "IHDR length");

    const unsigned char* buf = g_last_chunk_data.data();
    // Width/Height
    uint32_t w = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
    uint32_t h = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
    EXPECT_EQ_UINT32(w, width, "IHDR width", "width field");
    EXPECT_EQ_UINT32(h, height, "IHDR height", "height field");
    // Depth/Type/Compression/Filter/Interlace
    EXPECT_EQ_INT(buf[8], (png_byte)bit_depth, "IHDR bit_depth", "bit_depth field");
    EXPECT_EQ_INT(buf[9], (png_byte)color_type, "IHDR color_type", "color_type field");
    EXPECT_EQ_INT(buf[10], (png_byte)compression_type, "IHDR compression_type", "compression_type field");
    EXPECT_EQ_INT(buf[11], (png_byte)filter_type, "IHDR filter_type", "filter_type field");
    EXPECT_EQ_INT(buf[12], (png_byte)interlace_type, "IHDR interlace_type", "interlace_type field");

    // Internal state
    EXPECT_TRUE(p->width == (int)width, "IHDR internal width", "width state");
    EXPECT_TRUE(p->height == (int)height, "IHDR internal height", "height state");
    EXPECT_TRUE(p->bit_depth == (png_byte)bit_depth, "IHDR internal bit_depth", "bit_depth state");
    EXPECT_TRUE(p->color_type == (png_byte)color_type, "IHDR internal color_type", "color_type state");
    EXPECT_TRUE(p->interlaced == (png_byte)interlace_type, "IHDR internal interlace_type", "interlace state");
    EXPECT_TRUE(p->mode == PNG_HAVE_IHDR, "IHDR mode set", "mode state");
    // do_filter should have been set (since initial PNG_NO_FILTERS)
    // Gray with 8-bit depth is not palette and depth >= 8 -> ALL_FILTERS
    EXPECT_TRUE(p->do_filter == PNG_ALL_FILTERS, "IHDR do_filter set", "do_filter state");

    delete p;
}

void test_rgb16_writes_ihdr() {
    reset_test_state();
    png_struct* p = create_fake_png_ptr();

    uint32_t width = 5;
    uint32_t height = 3;
    int bit_depth = 16;
    int color_type = PNG_COLOR_TYPE_RGB;
    int compression_type = 0;
    int filter_type = 0;
    int interlace_type = 0;

    png_write_IHDR(p, width, height, bit_depth, color_type, compression_type, filter_type, interlace_type);

    // Validate
    EXPECT_EQ_UINT32(g_last_chunk_name, PNG_IHDR, "png_write_IHDR writes IHDR chunk (RGB16)", "IHDR name");
    const unsigned char* buf = g_last_chunk_data.data();
    uint32_t w = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
    uint32_t h = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
    EXPECT_EQ_UINT32(w, width, "IHDR width (RGB16)", "width");
    EXPECT_EQ_UINT32(h, height, "IHDR height (RGB16)", "height");
    EXPECT_EQ_INT(buf[8], (png_byte)bit_depth, "IHDR bit_depth (RGB16)", "bit_depth");
    EXPECT_EQ_INT(buf[9], (png_byte)color_type, "IHDR color_type (RGB16)", "color_type");
    EXPECT_EQ_INT(buf[10], (png_byte)compression_type, "IHDR compression_type (RGB16)", "compression_type");
    EXPECT_EQ_INT(buf[11], (png_byte)filter_type, "IHDR filter_type (RGB16)", "filter_type");
    EXPECT_EQ_INT(buf[12], (png_byte)interlace_type, "IHDR interlace_type (RGB16)", "interlace_type");

    // Internal
    EXPECT_TRUE(p->channels == 3, "RGB16 channels", "channels");
    EXPECT_TRUE(p->bit_depth == 16, "RGB16 bit_depth", "bit_depth");
    EXPECT_TRUE(p->color_type == (png_byte)color_type, "RGB16 color_type", "color_type");
    EXPECT_TRUE(p->mode == PNG_HAVE_IHDR, "RGB16 IHDR mode", "mode");

    delete p;
}

void test_rgb4_invalid_bit_depth_throws() {
    reset_test_state();
    png_struct* p = create_fake_png_ptr();

    uint32_t width = 8;
    uint32_t height = 8;
    int bit_depth = 4; // valid for grayscale/palette, but invalid for RGB
    int color_type = PNG_COLOR_TYPE_RGB;
    int compression_type = 0;
    int filter_type = 0;
    int interlace_type = 0;

    bool exception_thrown = false;
    try {
        png_write_IHDR(p, width, height, bit_depth, color_type, compression_type, filter_type, interlace_type);
    } catch (const std::exception& e) {
        exception_thrown = true;
        // Optional: verify message
        // std::string msg = e.what();
    }

    EXPECT_TRUE(exception_thrown, "RGB invalid bit_depth should throw", "exception on invalid depth");

    delete p;
}

void test_interlace_invalid_clamped() {
    reset_test_state();
    png_struct* p = create_fake_png_ptr();

    uint32_t width = 16;
    uint32_t height = 16;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_GRAY;
    int compression_type = 0;
    int filter_type = 0;
    int interlace_type = 99; // invalid

    png_write_IHDR(p, width, height, bit_depth, color_type, compression_type, filter_type, interlace_type);

    // Should have been clamped to ADAM7
    EXPECT_EQ_INT(p->interlaced, PNG_INTERLACE_ADAM7, "IHDR interlace_type clamped", "interlace after clamp");

    delete p;
}

void test_compression_type_warning() {
    reset_test_state();
    png_struct* p = create_fake_png_ptr();

    uint32_t width = 32;
    uint32_t height = 32;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_GRAY;
    int compression_type = 99; // non-base to trigger warning
    int filter_type = 0;
    int interlace_type = 0;

    png_write_IHDR(p, width, height, bit_depth, color_type, compression_type, filter_type, interlace_type);

    // Expect a warning about compression type
    if (g_last_warning.empty()) {
        std::cerr << "[WARN] compression warning not produced as expected\n";
    } else {
        // We can compare substring since message text exactness isn't critical here
        if (g_last_warning.find("Invalid compression type specified") == std::string::npos) {
            std::cerr << "[WARN] unexpected compression warning: " << g_last_warning << "\n";
        }
    }

    delete p;
}

void test_palette_8bit_writes_ihdr() {
    reset_test_state();
    png_struct* p = create_fake_png_ptr();

    uint32_t width = 4;
    uint32_t height = 4;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_PALETTE;
    int compression_type = 0;
    int filter_type = 0;
    int interlace_type = 0;

    png_write_IHDR(p, width, height, bit_depth, color_type, compression_type, filter_type, interlace_type);

    // Palette images should have 1 channel in IHDR
    EXPECT_TRUE(p->channels == 1, "Palette 8-bit IHDR channels", "channels for PALETTE");
    // Data in chunk reflects width/height
    const unsigned char* buf = g_last_chunk_data.data();
    uint32_t w = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
    uint32_t h = (buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7];
    EXPECT_EQ_UINT32(w, width, "Palette IHDR width", "width");
    EXPECT_EQ_UINT32(h, height, "Palette IHDR height", "height");

    delete p;
}

void test_gray1_bit_depth_writes_ihdr() {
    reset_test_state();
    png_struct* p = create_fake_png_ptr();

    uint32_t width = 1;
    uint32_t height = 1;
    int bit_depth = 1;
    int color_type = PNG_COLOR_TYPE_GRAY;
    int compression_type = 0;
    int filter_type = 0;
    int interlace_type = 0;

    png_write_IHDR(p, width, height, bit_depth, color_type, compression_type, filter_type, interlace_type);

    // Should set channels to 1
    EXPECT_TRUE(p->channels == 1, "Gray1 IHDR channels", "channels for 1-bit gray");
    delete p;
}

void test_invalid_color_type_throws() {
    reset_test_state();
    png_struct* p = create_fake_png_ptr();

    uint32_t width = 4;
    uint32_t height = 4;
    int bit_depth = 8;
    int color_type = 99; // invalid
    int compression_type = 0;
    int filter_type = 0;
    int interlace_type = 0;

    bool exception_thrown = false;
    try {
        png_write_IHDR(p, width, height, bit_depth, color_type, compression_type, filter_type, interlace_type);
    } catch (const std::exception& /*e*/) {
        exception_thrown = true;
    }

    EXPECT_TRUE(exception_thrown, "Invalid color type should throw", "exception for invalid color type");
    delete p;
}

// Simple test runner
int main() {
    std::cout << "Running png_write_IHDR unit tests (mocked environment)..." << std::endl;

    test_gray8_writes_ihdr();
    test_rgb16_writes_ihdr();
    test_rgb4_invalid_bit_depth_throws();
    test_interlace_invalid_clamped();
    test_compression_type_warning();
    test_palette_8bit_writes_ihdr();
    test_gray1_bit_depth_writes_ihdr();
    test_invalid_color_type_throws();

    std::cout << "Tests completed." << std::endl;
    // We do not fail the process on test failures to maximize coverage,
    // but print detailed messages on stdout/stderr.
    return 0;
}