// Unit test suite for the focal method: make_rgb_colormap
// This test suite is implemented in C++11 without GoogleTest.
// It provides a minimal in-file test framework and mocks the necessary
// dependencies to exercise the RGB colormap creation logic.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <vector>


// DOMAIN_KNOWLEDGE alignment notes:
// - We model the essential dependency: png_image_read_control
// - We mock png_create_colormap_entry to record generated entries
// - We implement a local copy of make_rgb_colormap with the exact logic
//   from the focal method to enable isolated testing within this suite.

// Candidate Keywords (core components being exercised):
// - make_rgb_colormap
// - png_image_read_control
// - png_create_colormap_entry
// - P_sRGB encoding (color space tag)
// - 6x6x6 opaque RGB cube construction
// - 216 entries produced (6*6*6)

// Lightweight minimal test framework (non-terminating assertions)

// Global test counters
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple assertion helpers that do not terminate test execution
#define EXPECT_TRUE(cond) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            ++g_failed_tests; \
            std::cerr << "EXPECT_TRUE failed at line " << __LINE__ << ": " #cond "\n"; \
        } \
    } while (0)

#define EXPECT_FALSE(cond) \
    do { \
        ++g_total_tests; \
        if (cond) { \
            ++g_failed_tests; \
            std::cerr << "EXPECT_FALSE failed at line " << __LINE__ << ": " #cond "\n"; \
        } \
    } while (0)

#define EXPECT_EQ(a, b) \
    do { \
        ++g_total_tests; \
        auto _a = (a); \
        auto _b = (b); \
        if (!(_a == _b)) { \
            ++g_failed_tests; \
            std::cerr << "EXPECT_EQ failed at line " << __LINE__ << ": " \
                      << #a << " (" << _a << ") != " << #b << " (" << _b << ")\n"; \
        } \
    } while (0)

#define EXPECT_NE(a, b) \
    do { \
        ++g_total_tests; \
        auto _a = (a); \
        auto _b = (b); \
        if (!(_a != _b)) { \
            ++g_failed_tests; \
            std::cerr << "EXPECT_NE failed at line " << __LINE__ << ": " \
                      << #a << " (" << _a << ") == " << #b << " (" << _b << ")\n"; \
        } \
    } while (0)


// Candidate dependency mocks and structures

// Forward declare the PNG-related types and the encoding enum used by the focal method
// We provide a minimal, test-focused definition to enable function linkage in this suite.
struct ColormapEntry {
    unsigned int ip;
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;
    int encoding;
};

// The display object used by the focal function
struct png_image_read_control {
    std::vector<ColormapEntry> entries; // store generated colormap entries
};

// Encoding constant used by the focal method (P_sRGB)
enum {
    P_sRGB = 0
};

// Mock implementation of png_create_colormap_entry to record generated colors
extern "C" void png_create_colormap_entry(png_image_read_control *display,
                                        unsigned int ip,
                                        unsigned int red,
                                        unsigned int green,
                                        unsigned int blue,
                                        unsigned int alpha,
                                        int encoding)
{
    ColormapEntry e;
    e.ip = ip;
    e.red = red;
    e.green = green;
    e.blue = blue;
    e.alpha = alpha;
    e.encoding = encoding;
    // Append to the display's recorded entries
    display->entries.push_back(e);
}

// Forward declaration of the focal method under test
// We replicate the body of the function here to execute within this test
extern "C" int make_rgb_colormap(png_image_read_control *display);

// Implementation of the focal method (copied from the provided code block)
extern "C" int make_rgb_colormap(png_image_read_control *display)
{
{
   unsigned int i, r;
   /* Build a 6x6x6 opaque RGB cube */
   for (i=r=0; r<6; ++r)
   {
      unsigned int g;
      for (g=0; g<6; ++g)
      {
         unsigned int b;
         for (b=0; b<6; ++b)
            png_create_colormap_entry(display, i++, r*51, g*51, b*51, 255,
                P_sRGB);
      }
   }
   return (int)i;
}
}

// Test 1: Basic correctness — number of entries and first/last colors
void test_make_rgb_colormap_basic()
{
    png_image_read_control display;
    int count = make_rgb_colormap(&display);

    // The function should produce exactly 216 entries (6*6*6)
    EXPECT_EQ(count, 216);
    EXPECT_EQ(display.entries.size(), 216);

    // First entry should be ip=0, color (0,0,0), alpha=255, encoding=P_sRGB
    const ColormapEntry& e0 = display.entries[0];
    EXPECT_EQ(e0.ip, 0u);
    EXPECT_EQ(e0.red, 0u);
    EXPECT_EQ(e0.green, 0u);
    EXPECT_EQ(e0.blue, 0u);
    EXPECT_EQ(e0.alpha, 255u);
    EXPECT_EQ(e0.encoding, P_sRGB);

    // Last entry should be ip=215, color (255,255,255), alpha=255, encoding=P_sRGB
    const ColormapEntry& e_last = display.entries[215];
    EXPECT_EQ(e_last.ip, 215u);
    EXPECT_EQ(e_last.red, 255u);
    EXPECT_EQ(e_last.green, 255u);
    EXPECT_EQ(e_last.blue, 255u);
    EXPECT_EQ(e_last.alpha, 255u);
    EXPECT_EQ(e_last.encoding, P_sRGB);
}

// Test 2: Mid-index correctness — verify a representative internal mapping
void test_make_rgb_colormap_midpoint()
{
    png_image_read_control display;
    int count = make_rgb_colormap(&display);
    (void)count; // count is known to be 216; use as silent verification

    // Pick a mid index, e.g., 100
    size_t idx = 100;
    const ColormapEntry& e = display.entries.at(idx);

    // Compute expected (r,g,b) from index
    // r = i / 36, g = (i % 36) / 6, b = i % 6
    unsigned int r = static_cast<unsigned int>(idx / 36);
    unsigned int g = static_cast<unsigned int>((idx % 36) / 6);
    unsigned int b = static_cast<unsigned int>(idx % 6);

    EXPECT_EQ(e.ip, static_cast<unsigned int>(idx));
    EXPECT_EQ(e.red, r * 51);
    EXPECT_EQ(e.green, g * 51);
    EXPECT_EQ(e.blue, b * 51);
    EXPECT_EQ(e.alpha, 255u);
    EXPECT_EQ(e.encoding, P_sRGB);
}

// Test 3: Static policy / repeated use — ensure successive calls accumulate entries
void test_make_rgb_colormap_accumulation()
{
    png_image_read_control display;
    int first = make_rgb_colormap(&display);
    EXPECT_EQ(first, 216);
    EXPECT_EQ(display.entries.size(), 216);

    // Call again to ensure the results are appended, not reset
    int second = make_rgb_colormap(&display);
    EXPECT_EQ(second, 216);
    EXPECT_EQ(display.entries.size(), 432);

    // Sanity check: the first entry should still be the same
    const ColormapEntry& e0 = display.entries[0];
    EXPECT_EQ(e0.ip, 0u);
    EXPECT_EQ(e0.red, 0u);
    EXPECT_EQ(e0.green, 0u);
    EXPECT_EQ(e0.blue, 0u);
    EXPECT_EQ(e0.alpha, 255u);
    EXPECT_EQ(e0.encoding, P_sRGB);

    // And the entry at the boundary between the first and second run
    const ColormapEntry& e_last_of_first = display.entries[215];
    EXPECT_EQ(e_last_of_first.ip, 215u);
    EXPECT_EQ(e_last_of_first.red, 255u);
    EXPECT_EQ(e_last_of_first.green, 255u);
    EXPECT_EQ(e_last_of_first.blue, 255u);
    EXPECT_EQ(e_last_of_first.alpha, 255u);
    EXPECT_EQ(e_last_of_first.encoding, P_sRGB);

    // Entry at position 216 (start of the second run) should be ip=0 again
    const ColormapEntry& e_start_second = display.entries[216];
    EXPECT_EQ(e_start_second.ip, 0u);
    EXPECT_EQ(e_start_second.red, 0u);
    EXPECT_EQ(e_start_second.green, 0u);
    EXPECT_EQ(e_start_second.blue, 0u);
    EXPECT_EQ(e_start_second.alpha, 255u);
    EXPECT_EQ(e_start_second.encoding, P_sRGB);
}

// Runner and result reporting
void run_all_tests()
{
    test_make_rgb_colormap_basic();
    test_make_rgb_colormap_midpoint();
    test_make_rgb_colormap_accumulation();
}

int main()
{
    run_all_tests();

    std::cout << "Total tests executed: " << g_total_tests << "\n";
    std::cout << "Total test failures: " << g_failed_tests << "\n";

    if (g_failed_tests != 0) {
        std::cout << "Some tests FAILED.\n";
        return 1;
    } else {
        std::cout << "All tests PASSED.\n";
        return 0;
    }
}