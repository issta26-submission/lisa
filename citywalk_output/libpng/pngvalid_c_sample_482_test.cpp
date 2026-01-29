// Test suite for the focal method: sample(...) from pngvalid.c
// This test harness is written for C++11 and does not use Google Test.
// It assumes the focal function is available with C linkage in the linked object
// (e.g., pngvalid.o or the library built from pngvalid.c).
//
// Key ideas:
// - Test a variety of colour types and bit depths to cover true-branch and false-branch paths.
// - Cover 8-bit, 16-bit (with swap16 true/false), and <8-bit (bit-packed) scenarios.
// - Include a couple of palette (indexed color) scenarios to verify non-paletted branch behavior.
// - Use non-terminating assertions to maximize code execution and coverage.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Declaration of the focal function from pngvalid.c with C linkage.
// We mirror the original signature as closely as possible using standard types here.
// The libpng types (png_byte, png_uint_32, etc.) map cleanly to standard equivalents.
extern "C" unsigned int sample(const unsigned char* row, unsigned char colour_type,
                               unsigned char bit_depth, unsigned int x,
                               unsigned int sample_index, int swap16,
                               int littleendian);

// Lightweight test assertion macro: non-terminating, prints failure but continues.
#define TEST_EQ(actual, expected, msg) do { \
    ++total_tests; \
    if ((unsigned long long)(actual) != (unsigned long long)(expected)) { \
        ++failed_tests; \
        fprintf(stderr, "Test failed: %s | expected %llu, got %llu\n", \
                (msg), (unsigned long long)(expected), (unsigned long long)(actual)); \
    } \
} while (0)

// Global counters for test reporting
static int total_tests = 0;
static int failed_tests = 0;

// Helper to print a summary
static void print_summary() {
    if (failed_tests == 0) {
        printf("All tests passed (%d tests).\n", total_tests);
    } else {
        printf("%d out of %d tests FAILED.\n", failed_tests, total_tests);
    }
}

// Test 1: 8-bit, non-palette color with bit_depth = 8
// colour_type = 2 (truecolor), x = 0, sample_index = 0
// Expect to read the first byte directly.
static void test_case_8bit_nonpalette_x0() {
    // Arrange
    unsigned char row[1] = { 0xAB };
    // Act
    unsigned int got = sample(row, 2 /* colour_type */, 8 /* bit_depth */,
                              0 /* x */, 0 /* sample_index */, 0 /* swap16 */, 0 /* littleendian */);
    // Assert
    TEST_EQ(got, 0xAB, "8-bit non-palette, x=0 should return first byte");
}

// Test 2: 8-bit, non-palette color with x = 1
// Expect to read the second byte.
static void test_case_8bit_nonpalette_x1() {
    unsigned char row[2] = { 0x11, 0x22 };
    unsigned int got = sample(row, 2, 8, 1, 0, 0, 0);
    TEST_EQ(got, 0x22, "8-bit non-palette, x=1 should return second byte");
}

// Test 3: 16-bit, big-endian path (swap16 = 0)
// colour_type = 2 (truecolor), bit_depth = 16, x = 0
// Expect to read bytes [row[0], row[1]] as 0x1234
static void test_case_16bit_be() {
    unsigned char row[2] = { 0x12, 0x34 };
    unsigned int got = sample(row, 2, 16, 0, 0, 0, 0);
    TEST_EQ(got, 0x1234, "16-bit big-endian path (swap16 = 0)");
}

// Test 4: 16-bit, little-endian path (swap16 = 1)
// colour_type = 2 (truecolor), bit_depth = 16, x = 0
// Expect to read bytes as 0x3412
static void test_case_16bit_swap16() {
    unsigned char row[2] = { 0x12, 0x34 };
    unsigned int got = sample(row, 2, 16, 0, 0, 1, 0);
    TEST_EQ(got, 0x3412, "16-bit little-endian swap path (swap16 = 1)");
}

// Test 5: <8-bit, bit-packed path (bit_depth = 1) with big-endian-like handling
// colour_type = 0 (gray), littleendian = 0
// x = 3 selects the 4th bit in row[0] (bit 7) due to 1-bit depth
// Use row[0] with bit 7 set to 1 to expect 1
static void test_case_1bit_big_endian_path() {
    unsigned char row0 = 0x80; // 1000 0000b -> bit 7 is 1
    unsigned int got = sample(&row0, 0 /* colour_type */, 1 /* bit_depth */,
                              3 /* x */, 0 /* sample_index */, 0 /* swap16 */, 0 /* littleendian */);
    TEST_EQ(got, 1, "1-bit depth, x=3 should extract bit 7 -> 1");
}

// Test 6: <8-bit, bit-packed path with littleendian = 1
// colour_type = 0 (gray), bit_depth = 1, x = 3
// littleendian true keeps bit_index = 3, so reads bit 3 of row[0].
static void test_case_1bit_littleendian() {
    unsigned char row0 = 0x08; // 0000 1000b, bit 3 = 1
    unsigned int got = sample(&row0, 0, 1, 3, 0, 0 /* swap16 */, 1 /* littleendian */);
    TEST_EQ(got, 1, "1-bit depth, littleendian path with bit 3 set");
}

// Test 7: Palette path (color_type = 3) with bit_depth = 2 (two bits per sample)
// x = 0: sample is in bits 1:0 of row[0]
static void test_case_palette_depth2_x0() {
    unsigned char row0 = 0x02; // bits 1:0 = 10 -> 2
    unsigned int got = sample(&row0, 3 /* colour_type */, 2 /* bit_depth */,
                              0, 0, 0, 0);
    TEST_EQ(got, 2, "Palette path depth 2, x=0 should read lower 2 bits (2)");
}

// Test 8: Palette path (color_type = 3) with x = 1: bits 3:2 of row[0]
static void test_case_palette_depth2_x1() {
    unsigned char row0 = 0x0C; // bits 3:2 = 11 -> 3
    unsigned int got = sample(&row0, 3, 2, 1, 0, 0, 0);
    TEST_EQ(got, 3, "Palette path depth 2, x=1 should read bits 3:2 (3)");
}

// Main test runner
int main() {
    // Run tests with explanatory comments above each test.
    test_case_8bit_nonpalette_x0();          // 8-bit non-palette, x=0 (first byte)
    test_case_8bit_nonpalette_x1();          // 8-bit non-palette, x=1 (second byte)
    test_case_16bit_be();                    // 16-bit, big-endian path
    test_case_16bit_swap16();                // 16-bit, little-endian path (swap16)
    test_case_1bit_big_endian_path();        // 1-bit depth, big-endian-like extraction
    test_case_1bit_littleendian();           // 1-bit depth, little-endian path
    test_case_palette_depth2_x0();             // Palette path, depth 2, x=0
    test_case_palette_depth2_x1();             // Palette path, depth 2, x=1

    print_summary();
    return failed_tests == 0 ? 0 : 1;
}