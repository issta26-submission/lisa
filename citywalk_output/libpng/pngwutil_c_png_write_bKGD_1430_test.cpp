/*
Unit test suite for the focal method:
png_write_bKGD(png_structrp png_ptr, png_const_color_16p back, int color_type)

This test suite provides a minimal in-file mock environment to exercise the
different branches of png_write_bKGD without relying on the full libpng suite.
It uses only the C++ standard library and is designed to be compiled with C++11.

The tests cover:
- Palette path: invalid and valid background palette index.
- Truecolor path (RGB): 16-bit path (no warning) and 8-bit path (warning).
- Grayscale path: valid and out-of-range (warning) cases.

Note: This file mocks the necessary PNG internals (structures and helpers) to allow
unit testing of the focal function in isolation.
*/

#include <cstdint>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain knowledge: minimal re-implementation of required PNG-ish types and helpers

typedef unsigned char png_byte;
typedef unsigned int  png_uint_32;
typedef unsigned short png_uint_16;
typedef unsigned char* png_bytep;
typedef const unsigned char* png_const_bytep;

#define PNG_COLOR_TYPE_PALETTE 3
#define PNG_COLOR_MASK_COLOR 0x02
#define PNG_MNG_FEATURES_SUPPORTED 1
#define PNG_FLAG_MNG_EMPTY_PLTE 0x01
#define PNG_WRITE_16BIT_SUPPORTED 1

// Chunk type for bKGD
static const png_uint_32 png_bKGD = 0x624B4744; // ASCII 'bKGD'

// Minimal PNG struct to satisfy the focal method
struct png_struct {
  int num_palette;
  unsigned int mng_features_permitted;
  int bit_depth;
};

typedef struct png_struct png_struct;
typedef png_struct* png_structrp;
typedef const png_struct* png_structcp;

typedef struct {
  png_uint_16 red;
  png_uint_16 green;
  png_uint_16 blue;
  png_uint_16 gray;
  png_uint_16 index;
} png_color_16;

typedef png_color_16* png_color_16p;
typedef const png_color_16* png_const_color_16p;

// Global containers to capture behavior in tests
static std::vector<std::pair<png_uint_32, std::vector<png_byte>>> g_written_chunks;
static std::vector<std::string> g_warning_messages;

// Helpers to emulate the library behavior in tests
static void png_debug(int /*level*/, const char* /*msg*/) {
  // No-op for tests
}
static void png_warning(png_struct* /*png_ptr*/, const char* warning) {
  if (warning) g_warning_messages.emplace_back(warning);
  else g_warning_messages.emplace_back("warning");
}
static void png_write_complete_chunk(png_struct* /*png_ptr*/, png_uint_32 chunk_name,
                                     png_const_bytep data, size_t length) {
  std::vector<png_byte> v(length);
  if (data && length > 0) {
    std::memcpy(v.data(), data, length);
  }
  g_written_chunks.emplace_back(chunk_name, v);
}
static void png_save_uint_16(png_bytep buf, png_uint_16 i) {
  buf[0] = static_cast<png_byte>((i >> 8) & 0xFF);
  buf[1] = static_cast<png_byte>(i & 0xFF);
}

// Focal method replicated for test (adapted to compile in this test file)
void png_write_bKGD(png_structrp png_ptr, png_const_color_16p back, int color_type)
{
{
   png_byte buf[6];
   png_debug(1, "in png_write_bKGD");
   if (color_type == PNG_COLOR_TYPE_PALETTE)
   {
      if (
#ifdef PNG_MNG_FEATURES_SUPPORTED
          (png_ptr->num_palette != 0 ||
          (png_ptr->mng_features_permitted & PNG_FLAG_MNG_EMPTY_PLTE) == 0) &&
#endif
         back->index >= png_ptr->num_palette)
      {
         png_warning(png_ptr, "Invalid background palette index");
         return;
      }
      buf[0] = back->index;
      png_write_complete_chunk(png_ptr, png_bKGD, buf, 1);
   }
   else if ((color_type & PNG_COLOR_MASK_COLOR) != 0)
   {
      png_save_uint_16(buf, back->red);
      png_save_uint_16(buf + 2, back->green);
      png_save_uint_16(buf + 4, back->blue);
#ifdef PNG_WRITE_16BIT_SUPPORTED
      if (png_ptr->bit_depth == 8 && (buf[0] | buf[2] | buf[4]) != 0)
#else
      if ((buf[0] | buf[2] | buf[4]) != 0)
#endif
      {
         png_warning(png_ptr,
             "Ignoring attempt to write 16-bit bKGD chunk "
             "when bit_depth is 8");
         return;
      }
      png_write_complete_chunk(png_ptr, png_bKGD, buf, 6);
   }
   else
   {
      if (back->gray >= (1 << png_ptr->bit_depth))
      {
         png_warning(png_ptr,
             "Ignoring attempt to write bKGD chunk out-of-range for bit_depth");
         return;
      }
      png_save_uint_16(buf, back->gray);
      png_write_complete_chunk(png_ptr, png_bKGD, buf, 2);
   }
}
}

// Test helpers for assertions
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " << msg << std::endl; \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_EQ_UINT(expected, actual, msg) do { \
  if(static_cast<unsigned long long>(expected) != static_cast<unsigned long long>(actual)) { \
    std::cerr << "EXPECT_EQ failed: " << msg << " expected " << (expected) \
              << " but got " << (actual) << std::endl; \
    ++g_failures; \
  } \
} while(0)

static void reset_env() {
  g_written_chunks.clear();
  g_warning_messages.clear();
}

// Individual tests

void test_palette_invalid_index() {
  reset_env();
  png_struct png_ptr;
  png_color_16 back;

  png_ptr.num_palette = 5;
  png_ptr.mng_features_permitted = 0;
  png_ptr.bit_depth = 8;

  back.index = 5; // invalid: equal to num_palette

  png_write_bKGD(&png_ptr, &back, PNG_COLOR_TYPE_PALETTE);

  EXPECT_TRUE(g_written_chunks.empty(), "No bKGD chunk should be written for invalid palette index");
  EXPECT_TRUE(g_warning_messages.size() == 1 &&
              g_warning_messages[0].find("Invalid background palette index") != std::string::npos,
              "Expected invalid palette index warning");
}

void test_palette_valid_index() {
  reset_env();
  png_struct png_ptr;
  png_color_16 back;

  png_ptr.num_palette = 5;
  png_ptr.mng_features_permitted = 0;
  png_ptr.bit_depth = 8;

  back.index = 2;

  png_write_bKGD(&png_ptr, &back, PNG_COLOR_TYPE_PALETTE);

  EXPECT_TRUE(g_written_chunks.size() == 1, "One bKGD chunk should be written for valid palette index");
  EXPECT_EQ_UINT(png_bKGD, g_written_chunks[0].first, "Chunk name should be bKGD");
  EXPECT_EQ_UINT(1, g_written_chunks[0].second.size(), "Palette index data length should be 1");
  EXPECT_EQ_UINT(back.index, g_written_chunks[0].second[0], "Palette index value in chunk should match");
  EXPECT_TRUE(g_warning_messages.empty(), "No warnings expected for valid palette index");
}

void test_color_rgb_16bit_valid() {
  reset_env();
  png_struct png_ptr;
  png_color_16 back;

  png_ptr.num_palette = 0;
  png_ptr.mng_features_permitted = 0;
  png_ptr.bit_depth = 16;

  back.red = 0x0000;
  back.green = 0x0000;
  back.blue = 0x0000;
  back.index = 0;

  png_write_bKGD(&png_ptr, &back, PNG_COLOR_TYPE_RGB); // color type has color mask set

  EXPECT_TRUE(g_written_chunks.size() == 1, "One bKGD chunk should be written for 16-bit RGB with zero components");
  EXPECT_EQ_UINT(png_bKGD, g_written_chunks[0].first, "Chunk name should be bKGD");
  EXPECT_EQ_UINT(6, g_written_chunks[0].second.size(), "RGB bKGD data length should be 6");
  // Verify all zeros
  for (int i = 0; i < 6; ++i) {
    EXPECT_TRUE(g_written_chunks[0].second[i] == 0, "RGB bKGD data bytes should be zero when inputs are zero");
  }
  EXPECT_TRUE(g_warning_messages.empty(), "No warnings expected for zero RGB values with 16-bit depth");
}

void test_color_rgb_8bit_warning() {
  reset_env();
  png_struct png_ptr;
  png_color_16 back;

  png_ptr.num_palette = 0;
  png_ptr.mng_features_permitted = 0;
  png_ptr.bit_depth = 8; // triggers 8-bit path

  back.red = 0x00FF; // non-zero high byte will make buf[0] != 0
  back.green = 0;
  back.blue = 0;
  back.index = 0;

  png_write_bKGD(&png_ptr, &back, PNG_COLOR_TYPE_RGB);

  EXPECT_TRUE(g_written_chunks.empty(), "No bKGD chunk should be written when bit_depth is 8 and any color != 0");
  // The warning is produced by the code path for non-zero color in 8-bit case
  EXPECT_TRUE(!g_warning_messages.empty(), "Expected a warning when writing 16-bit bKGD with 8-bit depth");
  bool saw_warning = false;
  for (const auto& w : g_warning_messages) {
    if (w.find("Ignoring attempt to write 16-bit bKGD chunk") != std::string::npos) {
      saw_warning = true;
      break;
    }
  }
  EXPECT_TRUE(saw_warning, "Expected specific warning about 16-bit bKGD with 8-bit depth");
}

void test_gray_valid() {
  reset_env();
  png_struct png_ptr;
  png_color_16 back;

  png_ptr.num_palette = 0;
  png_ptr.mng_features_permitted = 0;
  png_ptr.bit_depth = 8;

  back.gray = 128;

  png_write_bKGD(&png_ptr, &back, 0); // PNG_COLOR_TYPE_GRAY assumed 0

  EXPECT_TRUE(g_written_chunks.size() == 1, "One bKGD chunk should be written for grayscale");
  EXPECT_EQ_UINT(png_bKGD, g_written_chunks[0].first, "Chunk name should be bKGD");
  EXPECT_EQ_UINT(2, g_written_chunks[0].second.size(), "Gray bKGD data length should be 2");
  // Gray value should be 128 => 0x00 0x80
  EXPECT_EQ_UINT(0x00, g_written_chunks[0].second[0], "Gray high byte");
  EXPECT_EQ_UINT(0x80, g_written_chunks[0].second[1], "Gray low byte");
  EXPECT_TRUE(g_warning_messages.empty(), "No warnings expected for valid grayscale");
}

void test_gray_invalid() {
  reset_env();
  png_struct png_ptr;
  png_color_16 back;

  png_ptr.num_palette = 0;
  png_ptr.mng_features_permitted = 0;
  png_ptr.bit_depth = 8;

  back.gray = 300; // out of range for 8-bit

  png_write_bKGD(&png_ptr, &back, 0);

  EXPECT_TRUE(g_written_chunks.empty(), "No bKGD chunk should be written for out-of-range grayscale");
  EXPECT_TRUE(!g_warning_messages.empty(), "Expected warning for out-of-range grayscale");
  bool saw_warning = false;
  for (const auto& w : g_warning_messages) {
    if (w.find("out-of-range for bit_depth") != std::string::npos) {
      saw_warning = true;
      break;
    }
  }
  EXPECT_TRUE(saw_warning, "Expected specific out-of-range grayscale warning");
}

// Test runner

int main() {
  std::cout << "Running unit tests for png_write_bKGD (self-contained mock)\n";

  test_palette_invalid_index();
  test_palette_valid_index();
  test_color_rgb_16bit_valid();
  test_color_rgb_8bit_warning();
  test_gray_valid();
  test_gray_invalid();

  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cout << g_failures << " TEST(S) FAILED\n";
  }

  return g_failures;
}