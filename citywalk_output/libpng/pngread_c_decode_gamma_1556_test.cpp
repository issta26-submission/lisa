// Test suite for the focal method: decode_gamma (C code linked with the C project under test).
// This test is written in C++11 without using GTest. It uses a lightweight, non-terminating
// assertion mechanism to exercise multiple code paths of decode_gamma.
//
// Notes:
// - We test a subset of branches due to dependencies (e.g., P_NOTSET and set_file_encoding) that are
//   tightly coupled to internal libpng behavior. We focus on deterministic, arithmetic branches and
//   a couple of combinations that exercise the switch cases.
// - The tests rely on the project providing the internal types and symbols (png_image_read_control, etc.),
//   as reflected in the <FOCAL_CLASS_DEP> section. The tests assume a GCC-like environment.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Bring in the C headers with C linkage. The project provides the types used by decode_gamma.
extern "C" {
}

// Provide C linkage for the focal function under test.
extern "C" {
    // Prototype for the focal function under test (provided in pngread.c / the project under test).
    // The actual signature uses png_uint_32; we declare accordingly.
    png_uint_32 decode_gamma(png_image_read_control *display, png_uint_32 value, int encoding);

    // The symbol png_sRGB_table is used by the P_sRGB path. We declare it here to compute the expected value.
    // If the symbol is not provided by the linked library, this will fail at link time. The test environment
    // is expected to provide the upstream libpng internal symbols.
    extern const png_uint_32 png_sRGB_table[];
}

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_EQ(a, b) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_TRUE(x) \
    do { \
        if (!(x)) { \
            std::cerr << "EXPECT_TRUE failed: " #x " is false at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_FALSE(x) EXPECT_TRUE(!(x))

// Test 1: P_LINEAR path should be a direct pass-through (no encoding change)
void test_decode_gamma_linear_path()
{
    // Arrange
    png_image_read_control display;
    std::memset(&display, 0, sizeof(display)); // Ensure clean, zeroed structure
    display.file_encoding = P_LINEAR;            // Force encoding to a linear path
    const png_uint_32 input = 12345;             // Arbitrary input
    const int encoding = P_LINEAR;                 // Explicitly pick P_LINEAR

    // Act
    png_uint_32 result = decode_gamma(&display, input, encoding);

    // Assert: result should equal input
    EXPECT_EQ(result, input);
}

// Test 2: P_LINEAR8 path should apply 257 multiplier exactly once
void test_decode_gamma_linear8_path()
{
    // Arrange
    png_image_read_control display;
    std::memset(&display, 0, sizeof(display));
    // No file encoding change; we call with P_LINEAR8 directly
    const png_uint_32 input = 3;
    const int encoding = P_LINEAR8;

    // Act
    png_uint_32 result = decode_gamma(&display, input, encoding);

    // Assert: value should be input * 257
    EXPECT_EQ(result, input * 257u);
}

// Test 3: P_FILE path with display.file_encoding set to P_LINEAR8 should behave as P_LINEAR8
// This verifies the initial "if (encoding == P_FILE) encoding = display->file_encoding;" branch.
void test_decode_gamma_file_path_substitution_to_linear8()
{
    // Arrange
    png_image_read_control display;
    std::memset(&display, 0, sizeof(display));
    display.file_encoding = P_LINEAR8; // Substitution target
    const png_uint_32 input = 2;
    const int encoding = P_FILE;         // Start as P_FILE to trigger substitution

    // Act
    png_uint_32 result = decode_gamma(&display, input, encoding);

    // Assert: after substitution to P_LINEAR8, we expect input * 257
    EXPECT_EQ(result, input * 257u);
}

// Test 4: P_sRGB path deterministic check with value 0
// This test assumes the library provides a stable mapping for value 0 (i.e., png_sRGB_table[0] is defined).
void test_decode_gamma_srgb_zero()
{
    // Arrange
    png_image_read_control display;
    std::memset(&display, 0, sizeof(display));
    const png_uint_32 input = 0;
    const int encoding = P_sRGB;

    // Act
    png_uint_32 result = decode_gamma(&display, input, encoding);

    // Expected from the sRGB table at index 0
    const png_uint_32 expected = png_sRGB_table[input];

    // Assert
    EXPECT_EQ(result, expected);
}

// Main runner
int main()
{
    std::cout << "Starting decode_gamma unit tests (non-GTest)..." << std::endl;

    test_decode_gamma_linear_path();
    test_decode_gamma_linear8_path();
    test_decode_gamma_file_path_substitution_to_linear8();

    // Only run the sRGB test if the symbol is available at link time.
    // We attempt to call; if linking fails, the build will fail and the user can adjust.
    test_decode_gamma_srgb_zero();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}