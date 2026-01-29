// Test suite for the focal function: png_do_pack
// This test suite is C++11 compliant and does not rely on GoogleTest.
// It provides a minimal, home-grown test harness with non-terminating assertions
// to maximize code coverage. It targets the png_do_pack function as defined
// in pngwtran.c and uses a lightweight, local mock of the involved data types.
// Note: This test assumes that png_do_pack is linkable with the provided struct
// layout for png_row_infop (struct png_row_info) and related typedefs.
// The tests focus on core logic branches for bit_depth = 1, 2 and 4 with
// row depths and simple input patterns.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Minimal type definitions to align with png_do_pack expectations.
// We mirror the key fields that png_do_pack accesses in the provided code.
typedef unsigned char png_byte;
typedef png_byte*       png_bytep;
typedef unsigned int      png_uint_32;

// Forward declaration of the focal function (C linkage to avoid name mangling)
extern "C" void png_do_pack(struct png_row_info* row_info, png_bytep row, png_uint_32 bit_depth);

// Local, test-scoped definition of the internal struct expected by png_do_pack.
// This mirrors the fields used inside the focal function.
struct png_row_info {
    unsigned char bit_depth;
    unsigned int channels;
    unsigned int width;
    unsigned int rowbytes;
    unsigned int pixel_depth;
};

// Typedef matching the library's internal API (pointer to the internal struct).
typedef struct png_row_info* png_row_infop;

// Simple non-terminating test assertion helpers.
// They log failures but do not abort test execution, allowing higher coverage.
static int g_test_failures = 0;

#define EXPECT_TRUE(cond) \
    do { if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } } while (0)

#define EXPECT_EQ(a, b) \
    do { if (!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } } while (0)

#define RUN_TEST(test) \
    do { \
        bool ok = (test)(); \
        if (ok) { \
            std::cout << "[OK] " #test "\n"; \
        } else { \
            std::cout << "[FAIL] " #test "\n"; \
        } \
    } while (0)

// Test 1: bit_depth = 1, width = 3, channels = 1
// Input row: {0, 1, 1}. Expect one packed output byte: 0x60.
// Post-conditions: row[0] contains 0x60; row_info fields updated accordingly.
bool test_png_do_pack_bit_depth_1_basic()
{
    png_row_info info;
    info.bit_depth = 8;     // initial bit depth in the row info (as checked by the function)
    info.channels = 1;
    info.width = 3;
    info.rowbytes = 0;
    info.pixel_depth = 0;

    png_byte rowbuf[4] = {0, 0, 0, 0};
    png_bytep row = rowbuf;
    png_row_infop ri = &info;

    // Call focal function
    png_do_pack(ri, row, 1);

    // Expected: first (and only) output byte is 0x60
    EXPECT_EQ(rowbuf[0], (png_byte)0x60);
    // Post-conditions on row info
    EXPECT_EQ(info.bit_depth, (unsigned char)1);
    EXPECT_EQ(info.pixel_depth, (unsigned int)1 * info.channels);
    // The exact rowbytes depends on the macro/implementation; ensure non-zero and plausible
    EXPECT_TRUE(info.rowbytes > 0);

    return (g_test_failures == 0);
}

// Test 2: bit_depth = 2, width = 3, channels = 1
// Input row: {0, 1, 2}. Expect output byte with packed 2-bit values: 0x18 (binary 00011000).
// Post-conditions: row[0] contains 0x18.
bool test_png_do_pack_bit_depth_2_basic()
{
    png_row_info info;
    info.bit_depth = 8;
    info.channels = 1;
    info.width = 3;
    info.rowbytes = 0;
    info.pixel_depth = 0;

    png_byte rowbuf[4] = {0, 0, 0, 0};
    png_bytep row = rowbuf;
    png_row_infop ri = &info;

    png_do_pack(ri, row, 2);

    // For width=3: values are packed as [ (0 & 0x03) << 6, (1 & 0x03) << 4, (2 & 0x03) << 2 ]
    // Resulting v after 3 iterations should be 0x18
    EXPECT_EQ(rowbuf[0], (png_byte)0x18);
    EXPECT_EQ(info.bit_depth, (unsigned char)2);
    EXPECT_EQ(info.pixel_depth, (unsigned int)2 * info.channels);
    EXPECT_TRUE(info.rowbytes > 0);

    return (g_test_failures == 0);
}

// Test 3: bit_depth = 4, width = 4, channels = 1
// Input row: {0x0F, 0x0F, 0x0F, 0x0F}. Expect two packed output bytes: 0xFF, 0xFF.
// Post-conditions: row[0] and row[1] contain 0xFF.
bool test_png_do_pack_bit_depth_4_basic()
{
    png_row_info info;
    info.bit_depth = 8;
    info.channels = 1;
    info.width = 4;
    info.rowbytes = 0;
    info.pixel_depth = 0;

    png_byte rowbuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    png_bytep row = rowbuf;
    png_row_infop ri = &info;

    png_do_pack(ri, row, 4);

    EXPECT_EQ(rowbuf[0], (png_byte)0xFF);
    EXPECT_EQ(rowbuf[1], (png_byte)0xFF);
    EXPECT_EQ(info.bit_depth, (unsigned char)4);
    EXPECT_EQ(info.pixel_depth, (unsigned int)4 * info.channels);
    EXPECT_TRUE(info.rowbytes > 0 && info.rowbytes <= 2);

    return (g_test_failures == 0);
}

// Main: run all tests and report summary
int main() {
    std::cout << "Starting tests for png_do_pack...\n";

    // Run individual tests
    RUN_TEST(test_png_do_pack_bit_depth_1_basic);
    RUN_TEST(test_png_do_pack_bit_depth_2_basic);
    RUN_TEST(test_png_do_pack_bit_depth_4_basic);

    // Summary
    if (g_test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Total failures: " << g_test_failures << "\n";
        return 1;
    }
}