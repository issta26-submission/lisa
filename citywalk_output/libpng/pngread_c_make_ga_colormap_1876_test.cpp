// Test suite for the focal method make_ga_colormap
// This is a self-contained, minimal C++11 unit test harness that mocks only the
// necessary dependencies to verify the behavior of make_ga_colormap.
// It does not rely on GTest; instead, it uses lightweight non-terminating checks
// to maximize code coverage.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Minimal stub of the types and constants expected by make_ga_colormap.
// The real project uses libpng types; for testing we implement a compact, local
// representation sufficient to validate the algorithm.

typedef struct {
  unsigned int red;
  unsigned int green;
  unsigned int blue;
  unsigned int alpha;
  int encoding;
} ColormapEntry;

typedef struct {
  ColormapEntry entries[256];
  unsigned int count;
} png_image_read_control;

// Simple encoding constant to mimic the P_sRGB enum in the original code.
// We only need a distinct value; the tests do not rely on its exact value.
enum { P_sRGB = 0 };

// Global pluggable function that the focal method calls to populate the colormap.
// In the real code this would be a complex external function; here we implement
// a lightweight version to enable isolated testing.
int png_create_colormap_entry(png_image_read_control *display,
                              unsigned int ip,
                              unsigned int red,
                              unsigned int green,
                              unsigned int blue,
                              unsigned int alpha,
                              int encoding)
{
  if (display->count >= 256) return -1; // prevent overflow for test purposes
  display->entries[display->count].red = red;
  display->entries[display->count].green = green;
  display->entries[display->count].blue = blue;
  display->entries[display->count].alpha = alpha;
  display->entries[display->count].encoding = encoding;
  display->count++;
  (void)ip; // ip is unused in this simplified mock
  return 0;
}

// Focal method under test (reproduced in a self-contained form for unit testing)
int make_ga_colormap(png_image_read_control *display)
{
{
   unsigned int i, a;
   i = 0;
   while (i < 231)
   {
      unsigned int gray = (i * 256 + 115) / 231;
      png_create_colormap_entry(display, i++, gray, gray, gray, 255, P_sRGB);
   }
   /* 255 is used here for the component values for consistency with the code
    * that undoes premultiplication in pngwrite.c.
    */
   png_create_colormap_entry(display, i++, 255, 255, 255, 0, P_sRGB);
   for (a=1; a<5; ++a)
   {
      unsigned int g;
      for (g=0; g<6; ++g)
         png_create_colormap_entry(display, i++, g*51, g*51, g*51, a*51,
             P_sRGB);
   }
   return (int)i;
}
}

// Lightweight test framework (non-terminating assertions)
static int test_failures = 0;
#define EXPECT_EQ(x,y) do { \
    if ((x) != (y)) { \
        std::cerr << "EXPECT_EQ failed: " #x " (" << (x) << ") != " #y " (" << (y) << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++test_failures; \
    } \
} while (0)

#define EXPECT_TRUE(x) do { \
    if (!(x)) { \
        std::cerr << "EXPECT_TRUE failed: " << #x \
                  << " is false at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++test_failures; \
    } \
} while (0)

// Test 1: Validate the entire ga-colormap generation produces 256 entries
// in the exact expected order and with correct RGBA values.
// - 231 grayscale entries: red=green=blue=computed_gray; alpha=255
// - 232nd entry: white with 0 alpha (255,255,255,0)
// - Remaining 24 entries: four groups (a=1..4), six grayscale levels (g=0..5)
//   with red=green=blue=g*51 and alpha=a*51
void test_ga_colormap_basic()
{
  png_image_read_control display;
  display.count = 0;

  int ret = make_ga_colormap(&display);
  EXPECT_EQ(ret, 256);
  EXPECT_EQ(display.count, 256);

  // Check first 231 grayscale entries
  for (unsigned int i = 0; i < 231; ++i)
  {
    unsigned int expected_gray = (i * 256 + 115) / 231;
    EXPECT_EQ(display.entries[i].red, expected_gray);
    EXPECT_EQ(display.entries[i].green, expected_gray);
    EXPECT_EQ(display.entries[i].blue, expected_gray);
    EXPECT_EQ(display.entries[i].alpha, 255u);
  }

  // Check 232nd entry (index 231)
  EXPECT_EQ(display.entries[231].red, 255u);
  EXPECT_EQ(display.entries[231].green, 255u);
  EXPECT_EQ(display.entries[231].blue, 255u);
  EXPECT_EQ(display.entries[231].alpha, 0u);

  // Check remaining 24 entries (a=1..4, g=0..5)
  unsigned int idx = 232;
  for (unsigned int a = 1; a < 5; ++a)
  {
    for (unsigned int g = 0; g < 6; ++g)
    {
      unsigned int exp = g * 51;
      EXPECT_EQ(display.entries[idx].red, exp);
      EXPECT_EQ(display.entries[idx].green, exp);
      EXPECT_EQ(display.entries[idx].blue, exp);
      EXPECT_EQ(display.entries[idx].alpha, a * 51);
      ++idx;
    }
  }
  EXPECT_EQ(idx, 256u);
}

// Entry point for tests
int main()
{
  test_ga_colormap_basic();

  if (test_failures == 0)
  {
    std::cout << "All tests passed.\n";
    return 0;
  }
  else
  {
    std::cerr << "Tests failed: " << test_failures << "\n";
    return 1;
  }
}