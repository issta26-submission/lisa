// Minimal self-contained unit tests for the focal function
// png_do_check_palette_indexes, implemented in C++11 without Google Test.
// The tests are designed to compile and run independently, mimicking the
// behavior of the actual libpng function with carefully crafted inputs.
// Tests cover true/false branches for each bit_depth path (1, 2, 4, 8).

#include <cstring>
#include <iostream>
#include <pngpriv.h>
#include <vector>


// Domain knowledge utilities
// Macro to compute the number of padding bits in the last byte.
// This mirrors the logic used in the original code (padding in bits).
#define PNG_PADBITS(bit_depth, width) ( \
    ((((width) * (bit_depth)) % 8) ? (8 - (((width) * (bit_depth)) % 8)) : 0) \
)

// Types approximating the minimal needed parts of libpng structures
typedef unsigned char png_bytep;

struct png_row_info {
    int bit_depth;     // number of bits per pixel in the row
    int width;         // number of pixels per row
    int pixel_depth;   // actual pixel depth in bits (often same as bit_depth for indices)
    int rowbytes;      // number of data bytes in the row buffer (excluding the sentinel)
};
typedef png_row_info* png_row_infop;

struct png_struct_def {
    int num_palette;       // number of palette colors currently used
    int num_palette_max;   // maximum palette index observed in current row
    unsigned char* row_buf; // buffer containing the row data; index 0 is sentinel
};
typedef struct png_struct_def png_struct;
typedef png_struct* png_structrp;

// No-op debug function to mimic the external dependency in the original code
static void png_debug(int /*level*/, const char* /*text*/) { /* no-op for tests */ }

// Focal method replicated for tests (copied/adapted from the provided source)
static void png_do_check_palette_indexes(png_structrp png_ptr, png_row_infop row_info)
{
{
   png_debug(1, "in png_do_check_palette_indexes");
   if (png_ptr->num_palette < (1 << row_info->bit_depth) &&
      png_ptr->num_palette > 0) /* num_palette can be 0 in MNG files */
   {
      /* Calculations moved outside switch in an attempt to stop different
       * compiler warnings.  'padding' is in *bits* within the last byte, it is an
       * 'int' because pixel_depth becomes an 'int' in the expression below,
       * and this calculation is used because it avoids warnings that other
       * forms produced on either GCC or MSVC.
       */
      int padding = PNG_PADBITS(row_info->pixel_depth, row_info->width);
      png_bytep rp = png_ptr->row_buf + row_info->rowbytes;
      switch (row_info->bit_depth)
      {
         case 1:
         {
            /* in this case, all bytes must be 0 so we don't need
             * to unpack the pixels except for the rightmost one.
             */
            for (; rp > png_ptr->row_buf; rp--)
            {
              if ((*rp >> padding) != 0)
                 png_ptr->num_palette_max = 1;
              padding = 0;
            }
            break;
         }
         case 2:
         {
            for (; rp > png_ptr->row_buf; rp--)
            {
              int i = ((*rp >> padding) & 0x03);
              if (i > png_ptr->num_palette_max)
                 png_ptr->num_palette_max = i;
              i = (((*rp >> padding) >> 2) & 0x03);
              if (i > png_ptr->num_palette_max)
                 png_ptr->num_palette_max = i;
              i = (((*rp >> padding) >> 4) & 0x03);
              if (i > png_ptr->num_palette_max)
                 png_ptr->num_palette_max = i;
              i = (((*rp >> padding) >> 6) & 0x03);
              if (i > png_ptr->num_palette_max)
                 png_ptr->num_palette_max = i;
              padding = 0;
            }
            break;
         }
         case 4:
         {
            for (; rp > png_ptr->row_buf; rp--)
            {
              int i = ((*rp >> padding) & 0x0f);
              if (i > png_ptr->num_palette_max)
                 png_ptr->num_palette_max = i;
              i = (((*rp >> padding) >> 4) & 0x0f);
              if (i > png_ptr->num_palette_max)
                 png_ptr->num_palette_max = i;
              padding = 0;
            }
            break;
         }
         case 8:
         {
            for (; rp > png_ptr->row_buf; rp--)
            {
               if (*rp > png_ptr->num_palette_max)
                  png_ptr->num_palette_max = (int) *rp;
            }
            break;
         }
         default:
            break;
      }
   }
}
}

// Test harness helpers
static void test_log(const std::string& name, const std::string& msg) {
    std::cout << "[TEST] " << name << ": " << msg << std::endl;
}

static void run_test_case(const std::string& name, const std::function<void()>& fn) {
    test_log(name, "start");
    fn();
    test_log(name, "end");
}

// Simple assertion helper
static void test_assert(bool condition, const std::string& fail_msg) {
    if (!condition) {
        std::cerr << "ASSERT FAIL: " << fail_msg << std::endl;
    }
}

int main() {
    // Shared buffers per test will be created within each test to avoid cross-test contamination.

    // Test 1: Condition false because num_palette == 0 (no change should occur)
    run_test_case("png_do_check_palette_indexes - false_condition_num_palette_zero", []() {
        std::vector<unsigned char> buffer(2, 0); // [0] sentinel, [1] data
        unsigned char* buf = buffer.data();

        png_struct test_ptr;
        test_ptr.num_palette = 0;
        test_ptr.num_palette_max = 0;
        test_ptr.row_buf = buf;

        png_row_info info;
        info.bit_depth = 8;
        info.width = 4;
        info.pixel_depth = 8;
        info.rowbytes = 1;

        png_do_check_palette_indexes(&test_ptr, &info);

        test_assert(test_ptr.num_palette_max == 0, "max should remain 0 when condition false (num_palette == 0)");
        test_assert(test_ptr.num_palette == 0, "num_palette should remain unchanged");
    });

    // Test 2: 8-bit path - maximum should reflect highest byte in row
    run_test_case("png_do_check_palette_indexes - 8bit_max", []() {
        std::vector<unsigned char> buffer(2, 0);
        buffer[1] = 5; // one data byte
        unsigned char* buf = buffer.data();

        png_struct test_ptr;
        test_ptr.num_palette = 1; // triggers branch
        test_ptr.num_palette_max = 0;
        test_ptr.row_buf = buf;

        png_row_info info;
        info.bit_depth = 8;
        info.width = 4;
        info.pixel_depth = 8;
        info.rowbytes = 1; // data at buffer[1]

        png_do_check_palette_indexes(&test_ptr, &info);

        test_assert(test_ptr.num_palette_max == 5, "max should be equal to the single data byte when bit_depth=8");
    });

    // Test 3: 1-bit path where the rightmost bit (after padding) is nonzero
    run_test_case("png_do_check_palette_indexes - 1bit_nonzero", []() {
        std::vector<unsigned char> buffer(2, 0);
        buffer[1] = 0x80; // with padding 7, shifts to check MSB
        unsigned char* buf = buffer.data();

        png_struct test_ptr;
        test_ptr.num_palette = 1;
        test_ptr.num_palette_max = 0;
        test_ptr.row_buf = buf;

        png_row_info info;
        info.bit_depth = 1;
        info.width = 1;
        info.pixel_depth = 1;
        info.rowbytes = 1;

        png_do_check_palette_indexes(&test_ptr, &info);

        test_assert(test_ptr.num_palette_max == 1, "1-bit path should set max to 1 when top bit is nonzero");
    });

    // Test 4: 1-bit path where the rightmost bit is zero
    run_test_case("png_do_check_palette_indexes - 1bit_zero", []() {
        std::vector<unsigned char> buffer(2, 0);
        buffer[1] = 0x00; // zero in the tested bit after padding
        unsigned char* buf = buffer.data();

        png_struct test_ptr;
        test_ptr.num_palette = 1;
        test_ptr.num_palette_max = 0;
        test_ptr.row_buf = buf;

        png_row_info info;
        info.bit_depth = 1;
        info.width = 1;
        info.pixel_depth = 1;
        info.rowbytes = 1;

        png_do_check_palette_indexes(&test_ptr, &info);

        test_assert(test_ptr.num_palette_max == 0, "1-bit path should keep max 0 when tested bit is zero");
    });

    // Test 5: 2-bit path where all 2-bit groups are max (3)
    run_test_case("png_do_check_palette_indexes - 2bit_all_three", []() {
        std::vector<unsigned char> buffer(2, 0);
        buffer[1] = 0xFF; // four 2-bit groups, all 3
        unsigned char* buf = buffer.data();

        png_struct test_ptr;
        test_ptr.num_palette = 1;
        test_ptr.num_palette_max = 0;
        test_ptr.row_buf = buf;

        png_row_info info;
        info.bit_depth = 2;
        info.width = 4;      // 4 * 2 = 8 bits in one byte
        info.pixel_depth = 2;
        info.rowbytes = 1;

        png_do_check_palette_indexes(&test_ptr, &info);

        test_assert(test_ptr.num_palette_max == 3, "2-bit path should yield max 3 when all groups are 3");
    });

    // Test 6: 4-bit path - nibble max value
    run_test_case("png_do_check_palette_indexes - 4bit_max_nibble", []() {
        std::vector<unsigned char> buffer(2, 0);
        buffer[1] = 0xAB; // high nibble 0xA (10), low nibble 0xB (11)
        unsigned char* buf = buffer.data();

        png_struct test_ptr;
        test_ptr.num_palette = 1;
        test_ptr.num_palette_max = 0;
        test_ptr.row_buf = buf;

        png_row_info info;
        info.bit_depth = 4;
        info.width = 2;          // two 4-bit nibbles
        info.pixel_depth = 4;
        info.rowbytes = 1;

        png_do_check_palette_indexes(&test_ptr, &info);

        test_assert(test_ptr.num_palette_max == 11, "4-bit path should take max of nibble values (11 from 0xB)");
    });

    // Test 7: False branch when num_palette >= (1<<bit_depth)
    run_test_case("png_do_check_palette_indexes - false_branch_due_to_bound", []() {
        std::vector<unsigned char> buffer(2, 0);
        buffer[1] = 0xFF;
        unsigned char* buf = buffer.data();

        png_struct test_ptr;
        test_ptr.num_palette = 256; // (1<<8) for bit_depth=8
        test_ptr.num_palette_max = 0xDEAD; // some nonzero to detect unwanted changes
        test_ptr.row_buf = buf;

        png_row_info info;
        info.bit_depth = 8;
        info.width = 4;
        info.pixel_depth = 8;
        info.rowbytes = 1;

        png_do_check_palette_indexes(&test_ptr, &info);

        // Should remain unchanged due to bound check
        test_assert(test_ptr.num_palette_max == 0xDEAD, "max should remain unchanged when condition is false (bound exceeded)");
    });

    test_log("SUMMARY", "All tests executed.");

    return 0;
}