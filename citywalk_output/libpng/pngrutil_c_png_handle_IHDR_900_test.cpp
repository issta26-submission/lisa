// Self-contained unit-test suite for the focal method png_handle_IHDR
// This test harness mocks the minimal libpng-like environment required
// to compile and exercise the focal function without any external libs.
// It provides multiple test cases to cover the function logic and data flow.

#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstddef>


// Domain-specific: Lightweight mock/types to support the focal function

typedef unsigned char png_byte;
typedef unsigned int  png_uint_32;
typedef int           png_int_32;
typedef unsigned long png_alloc_size_t;
typedef png_byte*     png_bytep;
typedef const png_byte* png_const_bytep;
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;
typedef void* png_inforp;

#define PNG_HAVE_IHDR 0x01

// Color type constants based on typical libpng values
#define PNG_COLOR_TYPE_GRAY        0
#define PNG_COLOR_TYPE_RGB           2
#define PNG_COLOR_TYPE_PALETTE       3
#define PNG_COLOR_TYPE_GRAY_ALPHA    4
#define PNG_COLOR_TYPE_RGB_ALPHA     6

// Feature macro (used to gate optional code in png_handle_IHDR)
// We enable it here to exercise the corresponding code path in tests.
#define PNG_MNG_FEATURES_SUPPORTED 1

#ifndef PNG_UNUSED
#define PNG_UNUSED(x) (void)(x)
#endif

// Basic rowbytes calculation macro used by the focal method
// Width in pixels, depth in bits per pixel (not bytes)
#define PNG_ROWBYTES(depth, width) ((((width) * (depth)) + 7) / 8)

// Forward declarations of needed helpers (mimicking libpng API)
static void png_debug(int, const char*);
static void png_debug1(int, const char*, int);
static void png_crc_read(png_structrp, png_bytep, png_uint_32);
static void png_crc_finish(png_structrp, png_uint_32);
static png_uint_32 png_get_uint_31(png_structrp, png_const_bytep);
static void png_set_IHDR(png_structrp, png_inforp, png_uint_32, png_uint_32,
                         int, int, int, int, int);

// Minimal implementation details for the focal function environment

struct png_struct {
  unsigned int mode;
  unsigned int width;
  unsigned int height;
  png_byte bit_depth;
  png_byte interlaced;
  png_byte color_type;
  png_byte filter_type;
  png_byte compression_type;
  png_byte channels;
  png_byte pixel_depth;
  unsigned long rowbytes;

  // Mocked data source for "CRC reads"
  const unsigned char* data;
  size_t data_len;
  size_t read_pos;
};

typedef struct png_struct* png_structrp;
typedef void* png_inforp;

// No-op debug helpers to keep test output clean
static void png_debug(int, const char*) { (void)0; }
static void png_debug1(int, const char*, int) { (void)0; }

// Mocked internal helpers

static void png_crc_read(png_structrp png_ptr, png_bytep buf, png_uint_32 length)
{
  if (png_ptr == nullptr || buf == nullptr) return;
  // Clamp read length to available data
  size_t remaining = (png_ptr->data_len > png_ptr->read_pos) ? (png_ptr->data_len - png_ptr->read_pos) : 0;
  size_t read_len = (length <= remaining) ? length : remaining;
  if (read_len > 0) {
    memcpy(buf, png_ptr->data + png_ptr->read_pos, read_len);
    png_ptr->read_pos += read_len;
  }
  // If not enough data, fill with zeros to keep function robust in tests
  if (read_len < length) {
    memset(buf + read_len, 0, length - read_len);
  }
}

static void png_crc_finish(png_structrp, png_uint_32 /*skip*/) { /* no-op for test */ }

static png_uint_32 png_get_uint_31(png_structrp /*png_ptr*/, png_const_bytep buf)
{
  // Big-endian 4-byte unsigned int, mask to 31 bits as per PNG convention
  unsigned long v =
      ((unsigned long)buf[0] << 24) |
      ((unsigned long)buf[1] << 16) |
      ((unsigned long)buf[2] << 8)  |
       (unsigned long)buf[3];
  return (png_uint_32)(v & 0x7FFFFFFF);
}

static void png_set_IHDR(png_structrp /*png_ptr*/, png_inforp /*info_ptr*/,
                         png_uint_32 /*width*/, png_uint_32 /*height*/,
                         int /*bit_depth*/, int /*color_type*/, int /*interlace_type*/,
                         int /*compression_type*/, int /*filter_type*/)
{
  // In this test harness, we do not validate IHDR details here.
  // The focal function assigns parsed values to png_ptr fields.
}

// The focal method under test (copied and adapted for the test harness)
int png_handle_IHDR(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
{
   png_byte buf[13];
   png_uint_32 width, height;
   int bit_depth, color_type, compression_type, filter_type;
   int interlace_type;
   png_debug(1, "in png_handle_IHDR");
   /* Length and position are checked by the caller. */
   png_ptr->mode |= PNG_HAVE_IHDR;
   png_crc_read(png_ptr, buf, 13);
   png_crc_finish(png_ptr, 0);
   width = png_get_uint_31(png_ptr, buf);
   height = png_get_uint_31(png_ptr, buf + 4);
   bit_depth = buf[8];
   color_type = buf[9];
   compression_type = buf[10];
   filter_type = buf[11];
   interlace_type = buf[12];
   /* Set internal variables */
   png_ptr->width = width;
   png_ptr->height = height;
   png_ptr->bit_depth = (png_byte)bit_depth;
   png_ptr->interlaced = (png_byte)interlace_type;
   png_ptr->color_type = (png_byte)color_type;
#ifdef PNG_MNG_FEATURES_SUPPORTED
   png_ptr->filter_type = (png_byte)filter_type;
#endif
   png_ptr->compression_type = (png_byte)compression_type;
   /* Find number of channels */
   switch (png_ptr->color_type)
   {
      default: /* invalid, png_set_IHDR calls png_error */
      case PNG_COLOR_TYPE_GRAY:
      case PNG_COLOR_TYPE_PALETTE:
         png_ptr->channels = 1;
         break;
      case PNG_COLOR_TYPE_RGB:
         png_ptr->channels = 3;
         break;
      case PNG_COLOR_TYPE_GRAY_ALPHA:
         png_ptr->channels = 2;
         break;
      case PNG_COLOR_TYPE_RGB_ALPHA:
         png_ptr->channels = 4;
         break;
   }
   /* Set up other useful info */
   png_ptr->pixel_depth = (png_byte)(png_ptr->bit_depth * png_ptr->channels);
   png_ptr->rowbytes = PNG_ROWBYTES(png_ptr->pixel_depth, png_ptr->width);
   png_debug1(3, "bit_depth = %d", png_ptr->bit_depth);
   png_debug1(3, "channels = %d", png_ptr->channels);
   png_debug1(3, "rowbytes = %lu", (unsigned long)png_ptr->rowbytes);
   /* Rely on png_set_IHDR to completely validate the data and call png_error if
    * it's wrong.
    */
   png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth,
       color_type, interlace_type, compression_type, filter_type);
   return handled_ok;
   PNG_UNUSED(length)
}
}

// Global test state
static int g_failures = 0;

// Simple helper assertions (non-terminating)
#define EXPECT_TRUE(cond, msg) \
  do { if(!(cond)) { ++g_failures; std::cout << "[FAIL] " << msg << std::endl; } else { /* pass */ } } while(0)

#define EXPECT_EQ_UINT(expected, actual, msg) \
  do { if((unsigned int)(expected) != (unsigned int)(actual)) { ++g_failures; std::cout << "[FAIL] " << msg \
       << " | expected: " << (unsigned int)(expected) \
       << ", actual: " << (unsigned int)(actual) << std::endl; } } while(0)

#define EXPECT_EQ_INT(expected, actual, msg) \
  do { if((int)(expected) != (int)(actual)) { ++g_failures; std::cout << "[FAIL] " << msg \
       << " | expected: " << (int)(expected) \
       << ", actual: " << (int)(actual) << std::endl; } } while(0)

static void run_header_test(const unsigned char header[13],
                            unsigned int expected_width,
                            unsigned int expected_height,
                            unsigned int expected_color_type,
                            unsigned int expected_bit_depth,
                            unsigned int expected_interlace,
                            unsigned int expected_compression,
                            unsigned int expected_filter,
                            unsigned int expected_channels,
                            unsigned int expected_pixel_depth)
{
  png_struct s;
  std::memset(&s, 0, sizeof(s));

  // Prepare mock data source
  s.data = header;
  s.data_len = 13;
  s.read_pos = 0;

  // Call focal function
  int res = png_handle_IHDR(&s, nullptr, 13);

  // Basic result verification
  EXPECT_TRUE((s.mode & PNG_HAVE_IHDR) != 0, "IHDR flag should be set after png_handle_IHDR");
  EXPECT_EQ_UINT(expected_width, s.width, "Width mismatch");
  EXPECT_EQ_UINT(expected_height, s.height, "Height mismatch");
  EXPECT_EQ_UINT(expected_bit_depth, s.bit_depth, "Bit depth mismatch");
  EXPECT_EQ_UINT(expected_color_type, s.color_type, "Color type mismatch");
  EXPECT_EQ_UINT(expected_interlace, s.interlaced, "Interlace type mismatch");
  EXPECT_EQ_UINT(expected_compression, s.compression_type, "Compression type mismatch");
#ifdef PNG_MNG_FEATURES_SUPPORTED
  EXPECT_EQ_UINT(expected_filter, s.filter_type, "Filter type mismatch (PNG_MNG_FEATURES_SUPPORTED defined)");
#else
  // If the feature macro is not defined, there is no field to compare.
  (void)expected_filter;
#endif
  EXPECT_EQ_UINT(expected_channels, s.channels, "Channel count mismatch");
  EXPECT_EQ_UINT(expected_pixel_depth, s.pixel_depth, "Pixel depth mismatch");
  (void)res; // result not asserted beyond internal state in this harness
}

int main() {
  // Helper to build a 13-byte IHDR header in big-endian form
  auto build_header = [](unsigned char out[13],
                         unsigned int width,
                         unsigned int height,
                         unsigned char bit_depth,
                         unsigned char color_type,
                         unsigned char compression_type,
                         unsigned char filter_type,
                         unsigned char interlace_type) {
    out[0] = (unsigned char)((width >> 24) & 0xFF);
    out[1] = (unsigned char)((width >> 16) & 0xFF);
    out[2] = (unsigned char)((width >> 8) & 0xFF);
    out[3] = (unsigned char)(width & 0xFF);

    out[4] = (unsigned char)((height >> 24) & 0xFF);
    out[5] = (unsigned char)((height >> 16) & 0xFF);
    out[6] = (unsigned char)((height >> 8) & 0xFF);
    out[7] = (unsigned char)(height & 0xFF);

    out[8]  = bit_depth;
    out[9]  = color_type;
    out[10] = compression_type;
    out[11] = filter_type;
    out[12] = interlace_type;
  };

  // Test 1: Gray (color type 0) -> 1 channel, 8-bit depth
  {
    unsigned char header[13];
    build_header(header, 16, 32, 8, PNG_COLOR_TYPE_GRAY, 0, 0, 0);
    run_header_test(header, 16, 32, PNG_COLOR_TYPE_GRAY, 8, 0, 0, 0, 1, 8);
  }

  // Test 2: RGB (color type 2) -> 3 channels, 8-bit depth
  {
    unsigned char header[13];
    build_header(header, 4, 2, 8, PNG_COLOR_TYPE_RGB, 0, 0, 0);
    run_header_test(header, 4, 2, PNG_COLOR_TYPE_RGB, 8, 0, 0, 0, 3, 24);
  }

  // Test 3: Gray Alpha (color type 4) -> 2 channels, 8-bit depth
  {
    unsigned char header[13];
    build_header(header, 20, 1, 8, PNG_COLOR_TYPE_GRAY_ALPHA, 0, 0, 0);
    run_header_test(header, 20, 1, PNG_COLOR_TYPE_GRAY_ALPHA, 8, 0, 0, 0, 2, 16);
  }

  // Test 4: RGB Alpha (color type 6) -> 4 channels, 8-bit depth
  {
    unsigned char header[13];
    build_header(header, 3, 3, 8, PNG_COLOR_TYPE_RGB_ALPHA, 0, 0, 0);
    run_header_test(header, 3, 3, PNG_COLOR_TYPE_RGB_ALPHA, 8, 0, 0, 0, 4, 32);
  }

  // Test 5: Palette (color type 3) -> 1 channel, 8-bit depth
  {
    unsigned char header[13];
    build_header(header, 10, 5, 8, PNG_COLOR_TYPE_PALETTE, 0, 0, 0);
    run_header_test(header, 10, 5, PNG_COLOR_TYPE_PALETTE, 8, 0, 0, 0, 1, 8);
  }

  // Summary
  if (g_failures == 0) {
    std::cout << "\nAll tests PASSED.\n";
    return 0;
  } else {
    std::cout << "\nTotal failures: " << g_failures << "\n";
    return 1;
  }
}