// cpp_png_header_test.cpp
// A self-contained unit test suite for the focal method: png_image_read_header
// The test suite provides minimal stubs of the required libpng-like interfaces
// to verify key branches and data flow without external dependencies.
// It follows the user's constraints: C++11, no GTest, main() entry point,
// and non-terminating expectations (EXPECT_*).

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain-specific: lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << msg << " (in " << __FUNCTION__ << ")\n"; \
    } else { \
        std::cerr << "[PASS] " << msg << " (in " << __FUNCTION__ << ")\n"; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_total_tests; \
    if(!((a) == (b))) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << msg << " - expected " << (b) << ", got " << (a) \
                  << " (in " << __FUNCTION__ << ")\n"; \
    } else { \
        std::cerr << "[PASS] " << msg << " (in " << __FUNCTION__ << ")\n"; \
    } \
} while(0)


// Domain: Minimal libpng-like types and constants (stubbed for unit testing)
typedef void* png_voidp;
typedef void* png_inforp;
typedef void* png_structrp;
typedef unsigned int png_uint_32;
typedef struct png_image png_image;
typedef png_image* png_imagep;
typedef png_structrp png_structpp;
typedef png_structrp png_structp;
typedef void* png_const_colorp;
typedef void* png_bytepp;
typedef void* png_bytepp_;

// Forward declaration of the focal function under test
extern "C" int png_image_read_header(png_voidp argument);

// ---------------- Mocked dependencies and environment for testing ----------------

#define PNG_COLOR_TYPE_GRAY 0
#define PNG_COLOR_TYPE_RGB 2
#define PNG_COLOR_TYPE_PALETTE 3

#define PNG_FORMAT_FLAG_COLOR 1U
#define PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB 0x01

// Mock global state to drive the fake PNG read info behavior
static int g_mock_width = 0;
static int g_mock_height = 0;
static int g_mock_color_type = PNG_COLOR_TYPE_GRAY;
static int g_mock_bit_depth = 8;
static unsigned int g_mock_num_palette = 0;
static int g_mock_not_srgb = 0;

// The fake PNG structures used by the focal code
struct png_struct {
  int width;
  int height;
  int color_type;
  int bit_depth;
  unsigned int num_palette;
  int not_srgb;
  unsigned int benign_errors; // used when benign errors are enabled
};

struct png_opaque {
  png_struct* png_ptr;
  void* info_ptr;
};

struct png_image {
  png_opaque* opaque;
  unsigned int width;
  unsigned int height;
  unsigned int format;
  unsigned int colormap_entries;
  unsigned int flags;
};

// Typedefs to mimic the expected API (names used by the focal code)
typedef struct png_struct png_struct;
typedef struct png_image png_image;
typedef struct png_opaque png_opaque;
typedef png_image* png_imagep;

// Helper to simulate the conversion used by the focal code
#define png_voidcast(type, value) (type)(value)

// Mocked function: returns not_sRGB status indicating grayscale is treated as sRGB
extern "C" int png_image_is_not_sRGB(png_struct* png_ptr) {
  // Return non-zero if the test configuration requests not-SRGB
  return png_ptr->not_srgb;
}

// Mocked function: determine image format flags based on color type
extern "C" png_uint_32 png_image_format(png_struct* png_ptr) {
  if (png_ptr->color_type == PNG_COLOR_TYPE_GRAY) {
    return 0; // grayscale has no COLOR flag
  }
  // All non-gray colors (RGB, PALETTE) are considered color
  return PNG_FORMAT_FLAG_COLOR;
}

// Mocked function: set benign errors (tracked for tests)
extern "C" void png_set_benign_errors(png_struct* png_ptr, int /*level*/) {
  // Simple tracking proxy
  // In a real libpng, this configures the error behavior.
  png_ptr->benign_errors = static_cast<unsigned int>(level);
}

// Mocked function: simulate reading PNG header information into png_ptr
extern "C" void png_read_info(png_struct* png_ptr, png_inforp /*info_ptr*/) {
  // Populate png_ptr fields to simulate reading a PNG header
  // Values come from the test harness (g_mock_* variables)
  png_ptr->width = g_mock_width;
  png_ptr->height = g_mock_height;
  png_ptr->color_type = g_mock_color_type;
  png_ptr->bit_depth = g_mock_bit_depth;
  png_ptr->num_palette = g_mock_num_palette;
  // Also propagate not_srgb information for color-space decisions
  png_ptr->not_srgb = static_cast<int>(g_mock_not_srgb);
}

// The focal method png_image_read_header is copied from the prompt.
// It relies on the above stubs to be present.
extern "C" int png_image_read_header(png_voidp argument)
{
{
   png_imagep image = png_voidcast(png_imagep, argument);
   png_structrp png_ptr = image->opaque->png_ptr;
   png_inforp info_ptr = image->opaque->info_ptr;
#ifdef PNG_BENIGN_ERRORS_SUPPORTED
   png_set_benign_errors(png_ptr, 1/*warn*/);
#endif
   png_read_info(png_ptr, info_ptr);
   /* Do this the fast way; just read directly out of png_struct. */
   image->width = png_ptr->width;
   image->height = png_ptr->height;
   {
      png_uint_32 format = png_image_format(png_ptr);
      image->format = format;
      /* Greyscale images don't (typically) have colour space information and
       * using it is pretty much impossible, so use sRGB for grayscale (it
       * doesn't matter r==g==b so the transform is irrelevant.)
       */
      if ((format & PNG_FORMAT_FLAG_COLOR) != 0 &&
          png_image_is_not_sRGB(png_ptr))
         image->flags |= PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB;
   }
   /* We need the maximum number of entries regardless of the format the
    * application sets here.
    */
   {
      png_uint_32 cmap_entries;
      switch (png_ptr->color_type)
      {
         case PNG_COLOR_TYPE_GRAY:
            cmap_entries = 1U << png_ptr->bit_depth;
            break;
         case PNG_COLOR_TYPE_PALETTE:
            cmap_entries = (png_uint_32)png_ptr->num_palette;
            break;
         default:
            cmap_entries = 256;
            break;
      }
      if (cmap_entries > 256)
         cmap_entries = 256;
      image->colormap_entries = cmap_entries;
   }
   return 1;
}
}

// -----------------------------------------------------------------------------------
// Test utilities to construct the fake image and simulate different scenarios
// -----------------------------------------------------------------------------------

struct TestImageBuilder {
  png_image image;
  png_opaque opaque;
  png_struct pngs;

  TestImageBuilder() {
    std::memset(&image, 0, sizeof(image));
    // Initialize opaque to point to our png_ptr
    opaque.png_ptr = &pngs;
    opaque.info_ptr = nullptr;
    image.opaque = &opaque;
    // Clear png_struct default state
    std::memset(&pngs, 0, sizeof(pngs));
  }

  // Convenience setters
  void set_width_height(int w, int h) { g_mock_width = w; g_mock_height = h; }
  void set_color_type(int ct) { g_mock_color_type = ct; }
  void set_bit_depth(int bd) { g_mock_bit_depth = bd; }
  void set_num_palette(unsigned int np) { g_mock_num_palette = np; }
  void set_not_srgb(int ns) { g_mock_not_srgb = ns; }

  void commit() {
    image.width = 0;
    image.height = 0;
    image.format = 0;
    image.colormap_entries = 0;
    image.flags = 0;
  }
};

// Helper to run a test case with the provided configuration
static void run_test_case(const char* name,
                          void (*config)(TestImageBuilder&),
                          void (*validate)(const png_image&))
{
  TestImageBuilder builder;
  builder.commit();
  config(builder);
  // Call the focal function
  int result = png_image_read_header((png_voidp)&builder.image);
  (void)result; // We expect 1; we verify in validation

  // Validate post-conditions
  validate(builder.image);

  // trivial cleanup (no dynamic allocations in this test harness)
}

// Specific test validators
static void validate_gray8_no_srgb(const png_image& img) {
  EXPECT_TRUE(img.width == 10, "Gray8: width copied from png_ptr.width");
  EXPECT_TRUE(img.height == 20, "Gray8: height copied from png_ptr.height");
  EXPECT_TRUE(img.format == 0, "Gray8: format should be 0 for grayscale");
  EXPECT_TRUE(img.flags == 0, "Gray8: no colorspace-not-srgb flag for grayscale");
  EXPECT_TRUE(img.colormap_entries == 256, "Gray8: colormap_entries for Gray8 should be 256");
}

static void validate_rgb8_srgb_not(const png_image& img) {
  EXPECT_TRUE(img.width == 11, "RGB8: width copied");
  EXPECT_TRUE(img.height == 22, "RGB8: height copied");
  EXPECT_TRUE(img.format == PNG_FORMAT_FLAG_COLOR, "RGB8: format should indicate COLOR");
  EXPECT_TRUE((img.flags & PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB) != 0, "RGB8: colorspace not-srgb flag set");
  EXPECT_TRUE(img.colormap_entries == 256, "RGB8: colormap_entries should be 256 for RGB");
}

static void validate_gray16_srgb(const png_image& img) {
  EXPECT_TRUE(img.width == 3, "Gray16: width copied");
  EXPECT_TRUE(img.height == 4, "Gray16: height copied");
  EXPECT_TRUE(img.format == 0, "Gray16: format should be 0 for grayscale");
  EXPECT_TRUE(img.colormap_entries == 256, "Gray16: colormap_entries should clamp to 256");
  EXPECT_TRUE(img.flags == 0, "Gray16: no colorspace-not-srgb flag for grayscale");
}

static void validate_palette_128_no_srgb(const png_image& img) {
  EXPECT_TRUE(img.width == 5, "Palette128: width copied");
  EXPECT_TRUE(img.height == 6, "Palette128: height copied");
  EXPECT_TRUE(img.format == PNG_FORMAT_FLAG_COLOR, "Palette128: format should indicate COLOR");
  EXPECT_TRUE((img.flags & PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB) == 0, "Palette128: colorspace-not-srgb not set when not requested");
  EXPECT_TRUE(img.colormap_entries == 128, "Palette128: colormap_entries should reflect num_palette");
}

static void validate_palette_300_srgb(const png_image& img) {
  EXPECT_TRUE(img.colormap_entries == 256, "Palette300: colormap_entries should clamp to 256");
  EXPECT_TRUE(img.format == PNG_FORMAT_FLAG_COLOR, "Palette300: format should indicate COLOR");
  EXPECT_TRUE((img.flags & PNG_IMAGE_FLAG_COLORSPACE_NOT_sRGB) != 0, "Palette300: colorspace-not-srgb flag set");
  EXPECT_TRUE(img.width == 5 && img.height == 6, "Palette300: dimensions preserved");
}

// -----------------------------------------------------------------------------------
// Individual test cases (explanatory comments included)
// -----------------------------------------------------------------------------------

void test_gray8_no_srgb_case() {
  // Arrange: Gray image, 8-bit, no not_srgb, small dimensions
  auto config = [](TestImageBuilder& b) {
    b.set_width_height(10, 20);
    b.set_color_type(PNG_COLOR_TYPE_GRAY);
    b.set_bit_depth(8);
    b.set_not_srgb(0);
  };
  auto validate = [](const png_image& img) { validate_gray8_no_srgb(img); };
  run_test_case("test_gray8_no_srgb", config, validate);
}

void test_rgb8_srgb_not_case() {
  // Arrange: RGB image, 8-bit, not_srgb = true
  auto config = [](TestImageBuilder& b) {
    b.set_width_height(11, 22);
    b.set_color_type(PNG_COLOR_TYPE_RGB);
    b.set_bit_depth(8);
    b.set_not_srgb(1);
  };
  auto validate = [](const png_image& img) { validate_rgb8_srgb_not(img); };
  run_test_case("test_rgb8_srgb_not", config, validate);
}

void test_gray16_srgb_case() {
  // Arrange: Gray image, 16-bit, not_srgb = true (colorspace flag should stay off due to Gray)
  auto config = [](TestImageBuilder& b) {
    b.set_width_height(3, 4);
    b.set_color_type(PNG_COLOR_TYPE_GRAY);
    b.set_bit_depth(16);
    b.set_not_srgb(1);
  };
  auto validate = [](const png_image& img) { validate_gray16_srgb(img); };
  run_test_case("test_gray16_srgb", config, validate);
}

void test_palette_128_no_srgb_case() {
  // Arrange: PALETTE image with 128 colors, not_srgb = false
  auto config = [](TestImageBuilder& b) {
    b.set_width_height(5, 6);
    b.set_color_type(PNG_COLOR_TYPE_PALETTE);
    b.set_bit_depth(8);
    b.set_num_palette(128);
    b.set_not_srgb(0);
  };
  auto validate = [](const png_image& img) { validate_palette_128_no_srgb(img); };
  run_test_case("test_palette_128_no_srgb", config, validate);
}

void test_palette_300_srgb_case() {
  // Arrange: PALETTE image with 300 colors, color-space not-srgb enabled
  auto config = [](TestImageBuilder& b) {
    b.set_width_height(5, 6);
    b.set_color_type(PNG_COLOR_TYPE_PALETTE);
    b.set_bit_depth(8);
    b.set_num_palette(300);
    b.set_not_srgb(1);
  };
  auto validate = [](const png_image& img) { validate_palette_300_srgb(img); };
  run_test_case("test_palette_300_srgb", config, validate);
}

// -----------------------------------------------------------------------------------
// Main entry: run all tests
// -----------------------------------------------------------------------------------
int main() {
  std::cerr << "Starting png_image_read_header test suite (custom stubbed environment).\n";

  test_gray8_no_srgb_case();
  test_rgb8_srgb_not_case();
  test_gray16_srgb_case();
  test_palette_128_no_srgb_case();
  test_palette_300_srgb_case();

  std::cerr << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << "\n";
  return g_failed_tests ? 1 : 0;
}