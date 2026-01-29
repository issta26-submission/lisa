// Lightweight C++11 test suite for the function png_do_swap
// This test suite is self-contained and does not rely on GTest.
// It provides minimal type definitions and a direct, reproducible
// implementation of png_do_swap to exercise the 16-bit swap logic.
//
// Notes:
// - The tests cover the 16-bit swap path (bit_depth == 16) with multiple units.
// - They verify that no swap occurs when bit_depth != 16.
// - They verify behavior when width or channels yield zero 16-bit units.
// - Tests are executed from main() as per domain guidance: "call test methods from the main function".

#include <cstdint>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain-adapted minimal definitions to emulate the required libpng-like types
using png_byte = uint8_t;
using png_bytep = png_byte*;
using png_uint_32 = uint32_t;
using png_uint_16 = uint16_t;

struct png_row_info {
    png_uint_32 width;
    png_uint_32 channels;
    int bit_depth;
};
using png_row_infop = png_row_info*;

// Minimal no-op for the internal debug function used by the focal method.
extern "C" void png_debug(int /*level*/, const char* /*message*/, ...) {
    // Intentionally empty to avoid test noise.
    (void)level; (void)message;
}

// The focal method re-implemented locally for standalone testing.
// This mirrors the logic observed in the provided <FOCAL_METHOD> snippet.
extern "C" void png_do_swap(png_row_infop row_info, png_bytep row)
{
{
   png_debug(1, "in png_do_swap");
   if (row_info->bit_depth == 16)
   {
      png_bytep rp = row;
      png_uint_32 i;
      png_uint_32 istop = row_info->width * row_info->channels;
      for (i = 0; i < istop; i++, rp += 2)
      {
#ifdef PNG_BUILTIN_BSWAP16_SUPPORTED
         /* Feature added to libpng-1.6.11 for testing purposes, not
          * enabled by default.
          */
         *(png_uint_16*)rp = __builtin_bswap16(*(png_uint_16*)rp);
#else
         png_byte t = *rp;
         *rp = *(rp + 1);
         *(rp + 1) = t;
#endif
      }
   }
}
}

// Simple testing framework helpers (non-terminating assertions)
static int g_failures = 0;

#define TEST_PRINT(msg) std::cout << "[TEST] " << msg << std::endl
#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "ASSERT FAILED: " << (msg) << "\n"; \
            ++g_failures; \
        } \
    } while (0)
#define ASSERT_ARRAY_EQ(a, b, n) \
    do { \
        if (std::memcmp((a), (b), (n)) != 0) { \
            std::cerr << "ASSERT FAILED: arrays differ (length=" << (n) << ")\n"; \
            ++g_failures; \
        } \
    } while (0)

// Test 1: Basic 16-bit swap with a few 16-bit units
// - bit_depth = 16
// - width * channels = 3 * 2 = 6 bytes -> 6 bytes = 3 16-bit units
// - Each 16-bit unit should have its two bytes swapped.
void test_png_do_swap_basic_16bit()
{
    TEST_PRINT("Test 1: Basic 16-bit swap with multiple 16-bit units");

    // Setup row_info
    png_row_infop row_info = new png_row_info;
    row_info->width = 3;
    row_info->channels = 2;   // 3 * 2 = 6 16-bit units
    row_info->bit_depth = 16;

    // Build a row with deterministic 16-bit units (little-endian style)
    // Each unit i is bytes: [low = i, high = i+1]
    // We interpret as row[2*i] = low byte, row[2*i+1] = high byte
    size_t istop = static_cast<size_t>(row_info->width * row_info->channels);
    std::vector<png_byte> row(istop * 2);
    for (size_t i = 0; i < istop; ++i) {
        row[2*i]   = static_cast<png_byte>(i);     // low byte
        row[2*i+1] = static_cast<png_byte>(i + 1); // high byte
    }

    // Copy original for expected result calculation
    std::vector<png_byte> expected = row;
    // Manual swap to compute expected (swap per 16-bit unit)
    for (size_t i = 0; i < istop; ++i) {
        std::swap(expected[2*i], expected[2*i+1]);
    }

    // Call the focal function
    png_bytep row_ptr = row.data();
    png_do_swap(row_info, row_ptr);

    // Verify result matches expected
    ASSERT_ARRAY_EQ(row.data(), expected.data(), row.size());

    delete row_info;
}

// Test 2: Ensure no swap occurs when bit_depth != 16
// - bit_depth = 8
// - The row should remain unchanged after the call.
void test_png_do_swap_no_swap_when_not_16bit()
{
    TEST_PRINT("Test 2: No swap when bit_depth != 16");

    png_row_infop row_info = new png_row_info;
    row_info->width = 4;
    row_info->channels = 2;
    row_info->bit_depth = 8; // Not 16, should do nothing

    size_t istop = static_cast<size_t>(row_info->width * row_info->channels);
    std::vector<png_byte> row(istop * 2);
    // Fill with a pattern
    for (size_t i = 0; i < istop * 2; ++i) {
        row[i] = static_cast<png_byte>((i * 7) & 0xFF);
    }

    std::vector<png_byte> original = row;

    png_bytep row_ptr = row.data();
    png_do_swap(row_info, row_ptr);

    // Row should be unchanged
    ASSERT_ARRAY_EQ(row.data(), original.data(), row.size());

    delete row_info;
}

// Test 3: Zero 16-bit units (width or channels zero)
// - istop = width * channels = 0 -> no swaps, row should remain unchanged
void test_png_do_swap_zero_units()
{
    TEST_PRINT("Test 3: Zero 16-bit units (width or channels zero)");

    png_row_infop row_info = new png_row_info;
    row_info->width = 0;
    row_info->channels = 5;
    row_info->bit_depth = 16;

    std::vector<png_byte> row(0); // zero bytes

    std::vector<png_byte> original = row;

    png_bytep row_ptr = row.data();
    png_do_swap(row_info, row_ptr);

    // Should remain unchanged (empty)
    ASSERT_ARRAY_EQ(row.data(), original.data(), row.size());

    delete row_info;
}

// Test 4: Large input to ensure scalability and correct per-unit swapping
void test_png_do_swap_large_input()
{
    TEST_PRINT("Test 4: Large input - multiple 16-bit units");

    png_row_infop row_info = new png_row_info;
    row_info->width = 50;      // 50 units per channel
    row_info->channels = 3;    // 150 16-bit units
    row_info->bit_depth = 16;

    size_t istop = static_cast<size_t>(row_info->width * row_info->channels);
    std::vector<png_byte> row(istop * 2);
    for (size_t i = 0; i < istop; ++i) {
        row[2*i]   = static_cast<png_byte>(i & 0xFF);
        row[2*i+1] = static_cast<png_byte>((i >> 8) & 0xFF);
    }

    std::vector<png_byte> expected = row;
    for (size_t i = 0; i < istop; ++i) {
        std::swap(expected[2*i], expected[2*i+1]);
    }

    png_bytep row_ptr = row.data();
    png_do_swap(row_info, row_ptr);

    ASSERT_ARRAY_EQ(row.data(), expected.data(), row.size());

    delete row_info;
}

// Main function to execute tests
int main()
{
    std::cout << "Starting png_do_swap test suite (standalone C++11)..." << std::endl;

    test_png_do_swap_basic_16bit();
    test_png_do_swap_no_swap_when_not_16bit();
    test_png_do_swap_zero_units();
    test_png_do_swap_large_input();

    if (g_failures == 0)
        std::cout << "All tests passed." << std::endl;
    else
        std::cerr << g_failures << " test(s) failed." << std::endl;

    // Return non-zero if any test failed
    return (g_failures == 0) ? 0 : 1;
}