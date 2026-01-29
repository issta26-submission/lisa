// Self-contained unit tests for the focal method png_do_invert
// This test suite is designed to run without GoogleTest and uses a minimal
// in-file reimplementation of the function under test to ensure deterministic
// behavior in a C++11 environment.

// Domain considerations addressed:
// - Includes only standard library facilities
// - Tests true/false branches of predicates (GRAY, GRAY_ALPHA 8/16)
// - Handles memory via std::vector and raw pointers as required by the API
// - Uses non-terminating assertions (EXPECT_* style macros) to maximize coverage
// - Provides explanatory comments for each test case

#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstddef>


// Define PNG-related constants and types used by png_do_invert
#define PNG_16BIT_SUPPORTED 1  // Enable 16-bit path for testing

typedef unsigned char png_byte;
typedef png_byte* png_bytep;

enum {
    PNG_COLOR_TYPE_GRAY = 0,
    PNG_COLOR_TYPE_GRAY_ALPHA = 4
};

// Minimal struct to mimic the png_row_infop used by png_do_invert
struct png_row_infop {
    int color_type;
    int bit_depth;
    size_t rowbytes;
};

// Simple no-op replacement for the internal png_debug used by libpng
static inline void png_debug(int /*level*/, const char* /*message*/) {
    // no-op
}

// Reconstructed version of the focal method png_do_invert (self-contained)
extern "C" void png_do_invert(png_row_infop row_info, png_bytep row)
{
    png_debug(1, "in png_do_invert");
    if (row_info.color_type == PNG_COLOR_TYPE_GRAY)
    {
        png_bytep rp = row;
        size_t i;
        size_t istop = row_info.rowbytes;
        for (i = 0; i < istop; i++)
        {
            *rp = (png_byte)(~(*rp));
            rp++;
        }
    }
    else if (row_info.color_type == PNG_COLOR_TYPE_GRAY_ALPHA &&
             row_info.bit_depth == 8)
    {
        png_bytep rp = row;
        size_t i;
        size_t istop = row_info.rowbytes;
        for (i = 0; i < istop; i += 2)
        {
            *rp = (png_byte)(~(*rp));
            rp += 2;
        }
    }
#ifdef PNG_16BIT_SUPPORTED
    else if (row_info.color_type == PNG_COLOR_TYPE_GRAY_ALPHA &&
             row_info.bit_depth == 16)
    {
        png_bytep rp = row;
        size_t i;
        size_t istop = row_info.rowbytes;
        for (i = 0; i < istop; i += 4)
        {
            *rp = (png_byte)(~(*rp));
            *(rp + 1) = (png_byte)(~(*(rp + 1)));
            rp += 4;
        }
    }
#endif
}

// Lightweight test framework (non-terminating assertions)
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond) \
    do { \
        tests_run++; \
        if(!(cond)) { \
            ++tests_failed; \
            std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
        } \
    } while(0)

#define EXPECT_EQ(expected, actual) \
    do { \
        tests_run++; \
        if(!((expected) == (actual))) { \
            ++tests_failed; \
            std::cerr << "EXPECT_EQ failed: expected " << (expected) \
                      << ", got " << (actual) \
                      << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while(0)

#define EXPECT_MEM_EQ(expected_ptr, actual_ptr, n) \
    do { \
        tests_run++; \
        if(std::memcmp((expected_ptr), (actual_ptr), (n)) != 0) { \
            ++tests_failed; \
            std::cerr << "EXPECT_MEM_EQ failed over " << (n) << " bytes at " \
                      << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while(0)

// Test 1: Gray (PNG_COLOR_TYPE_GRAY) path should invert every byte
// Expected: each byte inverted independently
void test_gray_path_inverts_every_byte()
{
    // Arrange
    std::vector<unsigned char> data = {0x00, 0x55, 0xFF}; // 3 bytes
    png_row_infop info;
    info.color_type = PNG_COLOR_TYPE_GRAY;
    info.bit_depth = 8;      // irrelevant for GRAY path
    info.rowbytes = data.size();

    // Act
    png_do_invert(info, data.data());

    // Assert
    std::vector<unsigned char> expected = { (unsigned char)~0x00, (unsigned char)~0x55, (unsigned char)~0xFF };
    EXPECT_MEM_EQ(expected.data(), data.data(), expected.size());
}

// Test 2: Gray Alpha, 8-bit depth path inverts only the first byte of each pixel
// Expected: for each 2-byte pair, invert the first byte, leave second byte unchanged
void test_gray_alpha8_inverts_only_first_of_each_pixel()
{
    // Arrange
    std::vector<unsigned char> data = {0x01, 0x02, 0x03, 0x04}; // two pixels: (1,2), (3,4)
    png_row_infop info;
    info.color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    info.bit_depth = 8;
    info.rowbytes = data.size();

    // Act
    png_do_invert(info, data.data());

    // Assert
    std::vector<unsigned char> expected = { (unsigned char)~0x01, 0x02, (unsigned char)~0x03, 0x04 };
    EXPECT_MEM_EQ(expected.data(), data.data(), expected.size());
}

// Test 3: Gray Alpha, 16-bit depth path (enabled via PNG_16BIT_SUPPORTED)
// Expected: invert the first two bytes of each 4-byte pixel group (only the first channel pair)
void test_gray_alpha16_inverts_first_two_bytes_of_each_pixel()
{
    // Arrange
    std::vector<unsigned char> data = {0x01, 0x02, 0x03, 0x04}; // one 4-byte pixel (16-bit channels)
    png_row_infop info;
    info.color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    info.bit_depth = 16;
    info.rowbytes = data.size();

    // Act
    png_do_invert(info, data.data());

    // Assert: first two bytes inverted; last two bytes unchanged
    std::vector<unsigned char> expected = { (unsigned char)~0x01, (unsigned char)~0x02, 0x03, 0x04 };
    EXPECT_MEM_EQ(expected.data(), data.data(), expected.size());
}

// Test 4: Non-invert path for a color type other than Gray or Gray-Alpha (e.g., RGB)
// Expected: input row remains unchanged
void test_other_color_type_no_inversion()
{
    // Arrange
    std::vector<unsigned char> data = {0x12, 0x34, 0x56, 0x78};
    png_row_infop info;
    info.color_type = 2; // PNG_COLOR_TYPE_RGB (not handled by png_do_invert)
    info.bit_depth = 8;
    info.rowbytes = data.size();

    // Act
    png_do_invert(info, data.data());

    // Assert
    std::vector<unsigned char> expected = {0x12, 0x34, 0x56, 0x78};
    EXPECT_MEM_EQ(expected.data(), data.data(), expected.size());
}

// Entry point to run all tests
int main()
{
    // Run tests
    test_gray_path_inverts_every_byte();
    test_gray_alpha8_inverts_only_first_of_each_pixel();
    test_gray_alpha16_inverts_first_two_bytes_of_each_pixel();
    test_other_color_type_no_inversion();

    // Summary
    if (tests_failed == 0) {
        std::cout << "All tests passed: " << tests_run << " tests executed.\n";
        return 0;
    } else {
        std::cerr << "Tests completed with failures: " << tests_failed
                  << " failed out of " << tests_run << " tests.\n";
        return 1;
    }
}