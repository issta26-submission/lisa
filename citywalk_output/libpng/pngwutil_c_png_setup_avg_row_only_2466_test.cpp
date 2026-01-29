// Minimal C++11 unit test suite for the focal function
// png_setup_avg_row_only, implemented here to be self-contained for testing.
// The tests avoid GTest and use a small in-file test harness with
// explanatory comments for each case.

#include <cstdint>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain Knowledge considerations implemented here:
// - Only standard library dependencies are used.
// - Static aspects are not needed; we keep simple struct usage.
// - We provide a small, non-terminating assertion mechanism via test return values.
// - Main function drives all tests to satisfy "call test methods from the main function" requirement.

// Typedefs and minimal dependencies to mimic the focal API
typedef unsigned char png_byte;
typedef unsigned char* png_bytep;
typedef unsigned int png_uint_32;
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;

#define PNG_FILTER_VALUE_AVG 2  // Minimal stub of the PNG AVG filter constant

// Minimal struct to mimic libpng's png_struct used by the focal method
struct png_struct {
    png_bytep row_buf;   // pointer to current row data (includes extra leading byte)
    png_bytep prev_row;  // pointer to previous row data (includes extra leading byte)
    png_bytep try_row;   // buffer where the computed "average" row is stored (includes leading byte)
};

// Forward declaration of the focal function under test (re-implemented for isolated testing)
void png_setup_avg_row_only(png_structrp png_ptr, png_uint_32 bpp, size_t row_bytes);

// Implementation of the focal method under test (reproduced here for testability)
void png_setup_avg_row_only(png_structrp png_ptr, png_uint_32 bpp, size_t row_bytes)
{
{
   png_bytep rp, dp, pp, lp;
   png_uint_32 i;
   png_ptr->try_row[0] = PNG_FILTER_VALUE_AVG;
   for (i = 0, rp = png_ptr->row_buf + 1, dp = png_ptr->try_row + 1,
       pp = png_ptr->prev_row + 1; i < bpp; i++)
   {
      *dp++ = (png_byte)(((int)*rp++ - ((int)*pp++ / 2)) & 0xff);
   }
   for (lp = png_ptr->row_buf + 1; i < row_bytes; i++)
   {
      *dp++ = (png_byte)(((int)*rp++ - (((int)*pp++ + (int)*lp++) / 2))
          & 0xff);
   }
}
}

// Lightweight test harness helpers

// Assert equality for a single byte (unsigned char)
static bool assert_byte_eq(const unsigned char a, const unsigned char b, const char* msg) {
    if (a != b) {
        std::cerr << "Assertion failed: " << msg
                  << " expected " << static_cast<int>(b)
                  << " got " << static_cast<int>(a) << std::endl;
        return false;
    }
    return true;
}

// Compute expected result using a reference implementation mirroring the focal logic.
// This function does not modify the actual buffers; it only computes the expected vector.
static void compute_expected_avg_row_only(unsigned int bpp, size_t row_bytes,
                                        const unsigned char* row_buf,
                                        const unsigned char* prev_row,
                                        unsigned char* expected_out) {
    // expected_out length should be at least row_bytes+1
    expected_out[0] = (unsigned char)PNG_FILTER_VALUE_AVG;
    // First bpp values
    for (size_t i = 0; i < bpp; ++i) {
        size_t idx = 1 + i;
        expected_out[idx] = (unsigned char)(((int)row_buf[idx] - ((int)prev_row[idx] / 2)) & 0xff);
    }
    // Remaining values
    for (size_t i = bpp; i < row_bytes; ++i) {
        size_t idx = 1 + i;
        expected_out[idx] = (unsigned char)(((int)row_buf[idx] - (((int)prev_row[idx] + (int)row_buf[1 + (i - bpp)]) / 2)) & 0xff);
    }
}

// Test Case 1: General case with bpp < row_bytes ensuring both loops run.
// row_bytes = 8, bpp = 3. This exercises both the first loop (bpp iterations)
// and the second loop (row_bytes - bpp iterations).
static bool test_case_general_bpp3_rowbytes8() {
    const size_t row_bytes = 8;
    const unsigned int bpp = 3;

    // Allocate buffers with an extra leading byte (indexing starts at 1)
    unsigned char row_buf[row_bytes + 1];
    unsigned char prev_row[row_bytes + 1];
    unsigned char try_row[row_bytes + 1];

    // Initialize with deterministic values
    // Row data: 1,2,3,4,5,6,7,8 mapped to indices 1..8
    for (size_t i = 0; i <= row_bytes; ++i) {
        row_buf[i] = (unsigned char)(i);       // 0..8 (row_buf[0] unused)
        prev_row[i] = (unsigned char)(10 * i); // 0,10,20,...,80
        try_row[i]  = 0;                        // clear
    }

    // Call the function under test
    png_struct s;
    s.row_buf   = row_buf;
    s.prev_row  = prev_row;
    s.try_row   = try_row;

    png_setup_avg_row_only(&s, bpp, row_bytes);

    // Compute expected results
    unsigned char expected[row_bytes + 1];
    compute_expected_avg_row_only(bpp, row_bytes, row_buf, prev_row, expected);

    // Validate full buffer [0..row_bytes]
    bool ok = true;
    if (!assert_byte_eq(try_row[0], expected[0], "Case1: first byte (AVG filter) mismatch")) ok = false;
    for (size_t i = 1; i <= row_bytes; ++i) {
        if (!assert_byte_eq(try_row[i], expected[i], ("Case1: try_row[" + std::to_string(i) + "] mismatch").c_str())) {
            ok = false;
        }
    }

    return ok;
}

// Test Case 2: bpp == row_bytes to exercise the false branch of the second loop.
// row_bytes = 5, bpp = 5. Only the first loop runs; second loop is skipped.
static bool test_case_bpp_equals_rowbytes() {
    const size_t row_bytes = 5;
    const unsigned int bpp = 5;

    unsigned char row_buf[row_bytes + 1];
    unsigned char prev_row[row_bytes + 1];
    unsigned char try_row[row_bytes + 1];

    // Initialize with deterministic values
    // Row data: 1,2,3,4,5 for indices 1..5
    for (size_t i = 0; i <= row_bytes; ++i) {
        row_buf[i] = (unsigned char)(i);        // 0..5
        prev_row[i] = (unsigned char)(10 * (i+1)); // 10,20,30,40,50,60
        try_row[i]  = 0;
    }

    png_struct s;
    s.row_buf  = row_buf;
    s.prev_row = prev_row;
    s.try_row  = try_row;

    png_setup_avg_row_only(&s, bpp, row_bytes);

    unsigned char expected[row_bytes + 1];
    compute_expected_avg_row_only(bpp, row_bytes, row_buf, prev_row, expected);

    bool ok = true;
    if (!assert_byte_eq(try_row[0], expected[0], "Case2: first byte (AVG filter) mismatch")) ok = false;
    for (size_t i = 1; i <= row_bytes; ++i) {
        if (!assert_byte_eq(try_row[i], expected[i], ("Case2: try_row[" + std::to_string(i) + "] mismatch").c_str())) {
            ok = false;
        }
    }

    return ok;
}

// Driver
int main() {
    int total = 0;
    int passed = 0;

    // Case 1
    {
        ++total;
        bool r = test_case_general_bpp3_rowbytes8();
        if (r) {
            std::cout << "Test Case 1 (bpp=3, row_bytes=8) PASSED\n";
            ++passed;
        } else {
            std::cout << "Test Case 1 (bpp=3, row_bytes=8) FAILED\n";
        }
    }

    // Case 2
    {
        ++total;
        bool r = test_case_bpp_equals_rowbytes();
        if (r) {
            std::cout << "Test Case 2 (bpp=5, row_bytes=5) PASSED\n";
            ++passed;
        } else {
            std::cout << "Test Case 2 (bpp=5, row_bytes=5) FAILED\n";
        }
    }

    std::cout << "Tests run: " << total << ", Passed: " << passed << ", Failed: " << (total - passed) << "\n";
    return (passed == total) ? 0 : 1;
}