// C++11 compatible unit tests for the focal method
// png_read_filter_row_paeth_multibyte_pixel
// This test suite reproduces the core algorithm (Paeth multibyte pixel variant)
// and validates it against a clear, deterministic reference implementation.
// The tests are self-contained (no GTest) and rely only on the C++ standard library.

#include <cstdint>
#include <cassert>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cmath>
#include <cstring>


// Lightweight C-style type aliases to mirror the libpng-like API used by the focal method.
typedef uint8_t png_byte;
typedef png_byte* png_bytep;
typedef const png_byte* png_const_bytep;

// Minimal reproduction of the necessary struct used by the focal method.
struct png_row_info_stub {
    unsigned int pixel_depth; // bits per pixel
    unsigned int rowbytes;    // total bytes in the row
};
typedef png_row_info_stub* png_row_infop;

// Forward declaration of the focal method under test (reproduced locally for testability).
png_byte png_read_filter_row_paeth_multibyte_pixel(png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row);

// ----------------- Focal METHOD REIMPLEMENTATION (LOCAL) -----------------
// This is a faithful, self-contained replica of the provided focal method.
// It uses the non-ABS path to exercise the core logic (pb < pa, pc < pa branches
// can be triggered via crafted inputs in tests). This function is defined with
// the exact signature shape used by the focal method in the prompt adapted to
// the local test environment (no external dependencies).

png_byte png_read_filter_row_paeth_multibyte_pixel(png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row)
{
{
   unsigned int bpp = (row_info->pixel_depth + 7) >> 3;
   png_bytep rp_end = row + bpp;
   /* Process the first pixel in the row completely (this is the same as 'up'
    * because there is only one candidate predictor for the first row).
    */
   while (row < rp_end)
   {
      int a = *row + *prev_row++;
      *row++ = (png_byte)a;
   }
   /* Remainder */
   rp_end = rp_end + (row_info->rowbytes - bpp);
   while (row < rp_end)
   {
      int a, b, c, pa, pb, pc, p;
      c = *(prev_row - bpp);
      a = *(row - bpp);
      b = *prev_row++;
      p = b - c;
      pc = a - c;
#ifdef PNG_USE_ABS
      pa = abs(p);
      pb = abs(pc);
      pc = abs(p + pc);
#else
      pa = p < 0 ? -p : p;
      pb = pc < 0 ? -pc : pc;
      pc = (p + pc) < 0 ? -(p + pc) : p + pc;
#endif
      if (pb < pa)
      {
         pa = pb; a = b;
      }
      if (pc < pa) a = c;
      a += *row;
      *row++ = (png_byte)a;
   }
}
}

// ----------------- REFERENCE IMPLEMENTATION (for EXPECTED VALUES) ---------------
// A straightforward reference that mirrors the same algorithm using explicit indexing.
// This is used to compute the expected output for given input buffers without relying
// on the focal function's internal pointer arithmetic.

static void paeth_ref_1bpp(const std::vector<uint8_t>& row_in,
                          const std::vector<uint8_t>& prev_row,
                          std::vector<uint8_t>& out)
{
    // Only support 1-byte pixels for these tests (bpp = 1)
    size_t n = row_in.size();
    out.assign(n, 0);
    if (n == 0) return;

    // First pixel: up predictor
    out[0] = static_cast<uint8_t>((row_in[0] + prev_row[0]) & 0xFF);
    uint8_t a_left = out[0];

    // Remainder
    for (size_t i = 1; i < n; ++i)
    {
        int c = static_cast<int>(prev_row[i - 1]); // up-left
        int a = static_cast<int>(a_left);          // left (updated each step)
        int b = static_cast<int>(prev_row[i]);     // up
        int p = b - c;
        int t1 = a - c;

        int pa = p < 0 ? -p : p;        // |p|  -> |b - c|
        int pb = t1 < 0 ? -t1 : t1;     // |a - c|
        int pc = p + t1;
        pc = pc < 0 ? -pc : pc;         // |p + (a - c)|

        int left = a; // predictor value before accumulation

        if (pb < pa)
        {
            pa = pb;
            left = b;
        }
        if (pc < pa)
        {
            left = c;
        }

        int result = left + static_cast<int>(row_in[i]);
        out[i] = static_cast<uint8_t>(result & 0xFF);

        a_left = out[i];
    }
}

// -------------------------- TEST INFRASTRUCTURE -----------------------------

// Helper: run a single test and report pass/fail
static bool run_test(const char* test_name, bool (*test_func)())
{
    bool ok = false;
    try {
        ok = test_func();
    } catch (...) {
        ok = false;
        std::cerr << "[ERROR] Exception in " << test_name << "\n";
    }
    std::cout << "[" << (ok ? "PASS" : "FAIL") << "] " << test_name << "\n";
    return ok;
}

// Test 1: Basic path (first pixel processing) and no-ABS path
// Verifies that the first pixel is computed as (row[0] + prev_row[0]) and that
// subsequent pixels follow the non-ABS branch logic to produce expected results.
static bool test_basic_path_no_abs()
{
    // Prepare input buffers
    png_row_info_stub row_info;
    row_info.pixel_depth = 8;  // 1 byte per pixel
    row_info.rowbytes = 3;     // 3 pixels in the row

    uint8_t row_buf[3]  = {10, 20, 30};
    uint8_t prev_buf[3] = {1, 2, 3};

    // Expected output (reference computation)
    std::vector<uint8_t> row_in = {10, 20, 30};
    std::vector<uint8_t> prev_in = {1, 2, 3};
    std::vector<uint8_t> expected;
    paeth_ref_1bpp(row_in, prev_in, expected); // expected[0..2]

    // Copy to non-const buffer for the focal function
    uint8_t test_row[3];
    std::memcpy(test_row, row_buf, 3);

    // Call focal method under test
    png_read_filter_row_paeth_multibyte_pixel(&row_info, test_row, prev_buf);

    // Compare
    bool equal = std::memcmp(test_row, expected.data(), 3) == 0;
    if (!equal) {
        std::cerr << "Expected: {" << int(expected[0]) << "," << int(expected[1])
                  << "," << int(expected[2]) << "}  Got: {"
                  << int(test_row[0]) << "," << int(test_row[1]) << ","
                  << int(test_row[2]) << "}\n";
    }
    return equal;
}

// Test 2: Branch where pb < pa is true (second pixel predictor switches to 'up')
static bool test_pb_less_pa_path()
{
    // Inputs crafted to produce pb < pa on second pixel
    // First pixel: out0 = row_in[0] + prev_row[0]
    // row_in[0] = 0, prev_row[0] = 50 => out0 = 50
    // i = 1: c = prev_row[0] = 50; a_left = 50; b = prev_row[1] = 0; p = -50; t1 = a_left - c = 0
    // pb = |t1| = 0, pa = |p| = 50 => pb < pa => a_left = b = 0
    // Then out1 = a_left + row_in[1] = 0 + 0 = 0
    png_row_info_stub row_info;
    row_info.pixel_depth = 8;
    row_info.rowbytes = 3;

    uint8_t row_buf[3]  = {0, 0, 0};
    uint8_t prev_buf[3] = {50, 0, 0};

    std::vector<uint8_t> row_in = {0, 0, 0};
    std::vector<uint8_t> prev_in = {50, 0, 0};
    std::vector<uint8_t> expected;
    paeth_ref_1bpp(row_in, prev_in, expected); // compute expected with reference

    uint8_t test_row[3];
    std::memcpy(test_row, row_buf, 3);

    png_read_filter_row_paeth_multibyte_pixel(&row_info, test_row, prev_buf);

    bool equal = std::memcmp(test_row, expected.data(), 3) == 0;
    if (!equal) {
        std::cerr << "pb<pa path mismatch. Expected: {"
                  << int(expected[0]) << ", " << int(expected[1]) << ", "
                  << int(expected[2]) << "} Got: {"
                  << int(test_row[0]) << ", " << int(test_row[1]) << ", "
                  << int(test_row[2]) << "}\n";
    }
    return equal;
}

// Test 3: Branch where pc < pa is true (second pixel uses up-left predictor)
static bool test_pc_less_pa_path()
{
    // Crafted to trigger pc < pa on second pixel as described in analysis
    // First: row_in[0] = 253, prev_row[0] = 3 -> out0 = 0 (mod 256)
    // i=1: c=3, a_left=0, b=5 -> p=2, t1=-3 -> pb=3, pa=2 => pb<pa? false; pc=|p+t1|=1 -> pc<pa? true -> a_left = c = 3
    // out1 = 3 + row_in[1] (0) = 3
    // i=2: c=5, a_left=3, b=0 -> p=-5, t1=-2 -> pb=2, pa=5 -> pb<pa? true -> a_left=0
    // out2 = 0 + row_in[2] (7) = 7
    png_row_info_stub row_info;
    row_info.pixel_depth = 8;
    row_info.rowbytes = 3;

    uint8_t row_buf[3]  = {253, 0, 7};
    uint8_t prev_buf[3] = {3, 5, 0};

    std::vector<uint8_t> row_in = {253, 0, 7};
    std::vector<uint8_t> prev_in = {3, 5, 0};
    std::vector<uint8_t> expected;
    paeth_ref_1bpp(row_in, prev_in, expected); // compute expected with reference

    uint8_t test_row[3];
    std::memcpy(test_row, row_buf, 3);

    png_read_filter_row_paeth_multibyte_pixel(&row_info, test_row, prev_buf);

    bool equal = std::memcmp(test_row, expected.data(), 3) == 0;
    if (!equal) {
        std::cerr << "pc<pa path mismatch. Expected: {"
                  << int(expected[0]) << ", " << int(expected[1]) << ", "
                  << int(expected[2]) << "} Got: {"
                  << int(test_row[0]) << ", " << int(test_row[1]) << ", "
                  << int(test_row[2]) << "}\n";
    }
    return equal;
}

// ------------------------------- MAIN ----------------------------------------

int main()
{
    int failures = 0;
    // Run tests and report
    if (!run_test("test_basic_path_no_abs (first pixel + subsequent non-ABS path)", test_basic_path_no_abs)) {
        ++failures;
    }
    if (!run_test("test_pb_less_pa_path (pb < pa branch on 2nd pixel)", test_pb_less_pa_path)) {
        ++failures;
    }
    if (!run_test("test_pc_less_pa_path (pc < pa branch on 2nd pixel)", test_pc_less_pa_path)) {
        ++failures;
    }

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) failed.\n";
        return 1;
    }
}