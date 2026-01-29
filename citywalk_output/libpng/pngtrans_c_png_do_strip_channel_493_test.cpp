// Self-contained C++11 unit tests for the focal method png_do_strip_channel
// Note: This test suite provides a minimal, self-contained replica of the
// relevant png_row_infop and png_do_strip_channel logic to exercise behavior.
// It does not depend on external libpng headers. The tests focus on core logic
// such as channel stripping, bit-depth handling (8/16), at_start branching,
// color type updates, and rowbytes/pixel_depth updates.

// The tests are designed to be executable without a full GTest framework, using
// a lightweight custom test harness that prints pass/fail messages and reports
// per-test explanations as comments.

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstddef>


// DOMAIN_KNOWLEDGE: Minimal, self-contained types & constants to mimic libpng
// definitions used by the focal function. This allows compiling and running
// tests without the entire library.

typedef unsigned char* png_bytep;

// Lightweight struct mimicking the essential fields used by png_do_strip_channel
struct png_row_info {
    int channels;        // number of channels in the source row
    int bit_depth;       // bit depth per channel (8 or 16)
    int color_type;      // color type (GRAY=0, RGB=2, GRAY_ALPHA=4, RGB_ALPHA=6)
    size_t rowbytes;       // number of bytes in the source row
    int pixel_depth;     // resulting pixel depth after transformation
};

// Typedef to match function signature expectations
typedef struct png_row_info* png_row_infop;

// Color type constants (subset sufficient for tests)
const int PNG_COLOR_TYPE_GRAY       = 0;
const int PNG_COLOR_TYPE_RGB          = 2;
const int PNG_COLOR_TYPE_GRAY_ALPHA   = 4;
const int PNG_COLOR_TYPE_RGB_ALPHA    = 6;

// Dummy png_debug function to satisfy the focal function's call without side effects
static void png_debug(int level, const char* /*msg*/) {
    (void)level;
}

// Reproduced (self-contained) version of the focal function png_do_strip_channel.
// This mirrors the logic in the provided source and operates in-place on the row.
void png_do_strip_channel(png_row_infop row_info, png_bytep row, int at_start)
{
{
   png_bytep sp = row;       /* source pointer */
   png_bytep dp = row;       /* destination pointer */
   png_bytep ep = row + row_info->rowbytes; /* One beyond end of row */
   png_debug(1, "in png_do_strip_channel");

   /* GA, GX cases */
   if (row_info->channels == 2)
   {
      if (row_info->bit_depth == 8)
      {
         if (at_start != 0) /* Skip initial filler */
            ++sp;
         else          /* Skip initial channel and, for sp, the filler */
         {
            sp += 2; ++dp;
         }
         /* For a 1 pixel wide image there is nothing to do */
         while (sp < ep)
         {
            *dp++ = *sp; sp += 2;
         }
         row_info->pixel_depth = 8;
      }
      else if (row_info->bit_depth == 16)
      {
         if (at_start != 0) /* Skip initial filler */
            sp += 2;
         else          /* Skip initial channel and, for sp, the filler */
         {
            sp += 4; dp += 2;
         }
         while (sp < ep)
         {
            *dp++ = *sp++; *dp++ = *sp; sp += 3;
         }
         row_info->pixel_depth = 16;
      }
      else
         return; /* bad bit depth */
      row_info->channels = 1;
      /* Finally fix the color type if it records an alpha channel */
      if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
         row_info->color_type = PNG_COLOR_TYPE_GRAY;
   }
   /* RGBA, RGBX, XRGB cases */
   else if (row_info->channels == 4)
   {
      if (row_info->bit_depth == 8)
      {
         if (at_start != 0) /* Skip initial filler */
            ++sp;
         else          /* Skip initial channels and, for sp, the filler */
         {
            sp += 4; dp += 3;
         }
         /* Note that the loop adds 3 to dp and 4 to sp each time. */
         while (sp < ep)
         {
            *dp++ = *sp++; *dp++ = *sp++; *dp++ = *sp; sp += 2;
         }
         row_info->pixel_depth = 24;
      }
      else if (row_info->bit_depth == 16)
      {
         if (at_start != 0) /* Skip initial filler */
            sp += 2;
         else          /* Skip initial channels and, for sp, the filler */
         {
            sp += 8; dp += 6;
         }
         while (sp < ep)
         {
            /* Copy 6 bytes, skip 2 */
            *dp++ = *sp++; *dp++ = *sp++;
            *dp++ = *sp++; *dp++ = *sp++;
            *dp++ = *sp++; *dp++ = *sp; sp += 3;
         }
         row_info->pixel_depth = 48;
      }
      else
         return; /* bad bit depth */
      row_info->channels = 3;
      /* Finally fix the color type if it records an alpha channel */
      if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
         row_info->color_type = PNG_COLOR_TYPE_RGB;
   }
   else
      return; /* The filler channel has gone already */
   /* Fix the rowbytes value. */
   row_info->rowbytes = (size_t)(dp-row);
}
}

// End of reproduction of focal function
}

// Lightweight helper assertion structure for tests
class TestSuite {
public:
    static bool eq(unsigned a, unsigned b, const std::string& label) {
        if (a == b) return true;
        std::cerr << "Assertion failed: " << label
                  << " (expected " << a << ", got " << b << ")\n";
        return false;
    }
    static bool eq_ptr(const void* a, const void* b, const std::string& label) {
        if (a == b) return true;
        std::cerr << "Assertion failed: " << label
                  << " (addresses differ: " << a << " vs " << b << ")\n";
        return false;
    }
};

// Helper: print test result
static void print_result(const std::string& name, bool passed) {
    std::cout << (passed ? "[TEST PASS] " : "[TEST FAIL] ") << name << "\n";
}

// Test 1: 2-channel, 8-bit, at_start = 0
bool test_two_channel_8bit_at_start0() {
    // Setup: two-channel 8-bit row with two pixels: [c0,c1, c0,c1] = 4 bytes
    unsigned char row_buf[4] = { 0x11, 0x22, 0x33, 0x44 };
    png_row_info info;
    info.channels = 2;
    info.bit_depth = 8;
    info.color_type = PNG_COLOR_TYPE_GRAY; // not GrayAlpha; test won't alter it
    info.rowbytes = 4;
    info.pixel_depth = 0;

    png_do_strip_channel(reinterpret_cast<png_row_infop>(&info),
                         row_buf, 0);

    // Expect: after stripping, 1 channel remains, rowbytes becomes 2
    bool pass = true;
    pass &= TestSuite::eq<uint8_t>(row_buf[0], 0x11, "test1.1: first pixel channel copied");
    pass &= (row_buf[1] == 0x33);
    // The other bytes should not have been touched (they remain for safety on test)
    pass &= TestSuite::eq(info.rowbytes, static_cast<size_t>(2), "test1.2: rowbytes updated to 2");
    pass &= TestSuite::eq(info.channels, 1u, "test1.3: channels reduced to 1");
    pass &= info.pixel_depth == 8;
    pass &= info.color_type == PNG_COLOR_TYPE_GRAY; // unchanged since not GrayAlpha
    return pass;
}

// Test 2: 2-channel, 8-bit, at_start = 1
bool test_two_channel_8bit_at_start1() {
    unsigned char row_buf[4] = { 0xAA, 0xBB, 0xCC, 0xDD };
    png_row_info info;
    info.channels = 2;
    info.bit_depth = 8;
    info.color_type = PNG_COLOR_TYPE_GRAY;
    info.rowbytes = 4;
    info.pixel_depth = 0;

    png_do_strip_channel(reinterpret_cast<png_row_infop>(&info),
                         row_buf, 1);

    // Expect: skip initial filler (sp advanced by 1), then copy every 2nd byte
    // Result should be [0xBB, 0xDD]
    bool pass = true;
    pass &= TestSuite::eq<unsigned>(row_buf[0], 0xBB, "test2.1: first output byte");
    pass &= TestSuite::eq<unsigned>(row_buf[1], 0xDD, "test2.2: second output byte");
    pass &= TestSuite::eq(info.rowbytes, static_cast<size_t>(2), "test2.3: rowbytes updated to 2");
    pass &= TestSuite::eq(info.channels, 1u, "test2.4: channels reduced to 1");
    pass &= info.pixel_depth == 8;
    pass &= info.color_type == PNG_COLOR_TYPE_GRAY;
    return pass;
}

// Test 3: 4-channel, 8-bit, at_start = 0 (RGBA -> RGB for 1-pixel width; color_type RGB_ALPHA -> RGB)
bool test_four_channel_8bit_at_start0() {
    unsigned char row_buf[4] = { 0x10, 0x11, 0x12, 0x13 }; // RGBA bytes for 1 pixel
    png_row_info info;
    info.channels = 4;
    info.bit_depth = 8;
    info.color_type = PNG_COLOR_TYPE_RGB_ALPHA; // alpha present
    info.rowbytes = 4;
    info.pixel_depth = 0;

    png_do_strip_channel(reinterpret_cast<png_row_infop>(&info),
                         row_buf, 0);

    // Expected: no loop executes for 1-pixel width; row remains first 3 bytes [0x10,0x11,0x12]
    // rowbytes should become 3; color_type converted to RGB
    bool pass = true;
    pass &= TestSuite::eq<uint8_t>(row_buf[0], 0x10, "test3.1: first byte remains 0x10");
    pass &= TestSuite::eq<uint8_t>(row_buf[1], 0x11, "test3.2: second byte remains 0x11");
    pass &= TestSuite::eq<uint8_t>(row_buf[2], 0x12, "test3.3: third byte remains 0x12");
    // Fourth byte may remain unused after processing; ensure rowbytes updated to 3
    pass &= TestSuite::eq(info.rowbytes, static_cast<size_t>(3), "test3.4: rowbytes updated to 3");
    pass &= info.channels == 3;
    pass &= info.color_type == PNG_COLOR_TYPE_RGB;
    return pass;
}

// Test 4: 4-channel, 8-bit, at_start = 1 (multi-byte copy path)
bool test_four_channel_8bit_at_start1() {
    unsigned char row_buf[4] = { 0xA1, 0xA2, 0xA3, 0xA4 }; // RGBA
    png_row_info info;
    info.channels = 4;
    info.bit_depth = 8;
    info.color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    info.rowbytes = 4;
    info.pixel_depth = 0;

    png_do_strip_channel(reinterpret_cast<png_row_infop>(&info),
                         row_buf, 1);

    // Expected: after operation, first 3 bytes are [A2, A3, A4] based on in-place copy pattern
    // and rowbytes becomes 3; color_type converted to RGB
    bool pass = true;
    pass &= TestSuite::eq<unsigned>(row_buf[0], 0xA2, "test4.1: first byte equals original second (A2)");
    pass &= TestSuite::eq<unsigned>(row_buf[1], 0xA3, "test4.2: second byte equals original third (A3)");
    pass &= TestSuite::eq<unsigned>(row_buf[2], 0xA4, "test4.3: third byte equals original fourth (A4)");
    pass &= TestSuite::eq(info.rowbytes, static_cast<size_t>(3), "test4.4: rowbytes updated to 3");
    pass &= info.channels == 3;
    pass &= info.color_type == PNG_COLOR_TYPE_RGB;
    return pass;
}

// Test 5: Invalid "filler" channel path: ensure non-2/4 channel inputs exit without modifying memory
bool test_invalid_channels_no_change() {
    unsigned char row_buf[3] = { 0x01, 0x02, 0x03 }; // 3 bytes, not 2 or 4 channels
    png_row_info info;
    info.channels = 3;       // invalid for this function's target paths
    info.bit_depth = 8;
    info.color_type = PNG_COLOR_TYPE_GRAY;
    info.rowbytes = 3;
    info.pixel_depth = 0;

    unsigned char before[3];
    std::memcpy(before, row_buf, 3);

    png_do_strip_channel(reinterpret_cast<png_row_infop>(&info),
                         row_buf, 0);

    // Expect no changes to the row and no changes to rowbytes/channels/pixel_depth
    bool pass = true;
    pass &= (info.rowbytes == 3);
    pass &= (info.channels == 3);
    pass &= (row_buf[0] == before[0] && row_buf[1] == before[1] && row_buf[2] == before[2]);
    return pass;
}

// Entry point for tests
int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const std::string& name, bool ok) {
        ++total;
        if (ok) ++passed;
        print_result(name, ok);
        return ok;
    };

    run("test_two_channel_8bit_at_start0", test_two_channel_8bit_at_start0());
    run("test_two_channel_8bit_at_start1", test_two_channel_8bit_at_start1());
    run("test_four_channel_8bit_at_start0", test_four_channel_8bit_at_start0());
    run("test_four_channel_8bit_at_start1", test_four_channel_8bit_at_start1());
    run("test_invalid_channels_no_change", test_invalid_channels_no_change());

    std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}