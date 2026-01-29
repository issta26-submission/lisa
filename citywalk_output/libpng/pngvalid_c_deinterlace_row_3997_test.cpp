// Unit test suite for the deinterlace_row function (pngvalid.c) using a C++11 test harness.
// This test suite assumes the presence of the libpng headers and the deinterlace_row symbol
// is linked from the project under test (pngvalid.c). No GoogleTest is used; tests are driven
// by a simple main() function with asserts and informative comments for each test case.

#include <cstdint>
#include <cassert>
#include <vector>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Import libpng types/macros for correctness of signatures and interlace constants.
extern "C" {
}

// Focal method under test signature (assumed to be provided by the project under test).
// We declare it with C linkage to ensure correct symbol resolution when linked with C code.
// The test suite uses this declaration to exercise the actual implementation inside pngvalid.c.
extern "C" void
deinterlace_row(png_bytep buffer, png_const_bytep row,
                unsigned int pixel_size, png_uint_32 w,
                int pass, int littleendian);

// Simple test harness
static int g_passed = 0;
static int g_failed = 0;

// Helper: run a single test and report result
static void run_test(const char* test_name, bool condition) {
    if (condition) {
        ++g_passed;
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        ++g_failed;
        std::cout << "[FAIL] " << test_name << std::endl;
    }
}

// Compute the expected mapping for deinterlace_row given width w, pixel_size, pass, and input row.
// This uses the same PNG interlace mapping logic (start column and step) to determine which
// destination indices (in buffer) should be written and which input pixel (xin) maps to them.
static void compute_expected_mapping(std::vector<uint8_t>& expected,
                                     unsigned int w,
                                     unsigned int pixel_size,
                                     int pass,
                                     const std::vector<uint8_t>& input_row) {
    // Initialize expected with a sentinel value so we can distinguish written vs. unwritten
    // positions. We'll use 0xFF for clear visibility.
    expected.assign(static_cast<size_t>(w) * pixel_size, 0xFF);

    // xin starts at 0 and increments for every written destination pixel
    png_uint_32 xin = 0;
    // The loop in the real function uses xout starting at PNG_PASS_START_COL(pass),
    // with a step of xstep = 1 << PNG_PASS_COL_SHIFT(pass)
    png_uint_32 xout = PNG_PASS_START_COL(pass);
    png_uint_32 xstep = static_cast<png_uint_32>(1U) << PNG_PASS_COL_SHIFT(pass);

    while (xout < w) {
        // Destination index in bytes
        size_t toIndex = static_cast<size_t>(xout) * pixel_size;

        // Source index (xin) in bytes
        size_t fromIndex = static_cast<size_t>(xin) * pixel_size;

        // Copy the pixel data from input_row to expected buffer (byte-wise)
        if (fromIndex + pixel_size <= input_row.size() && toIndex + pixel_size <= expected.size()) {
            std::memcpy(&expected[toIndex], &input_row[fromIndex], pixel_size);
        } else {
            // If the input_row is shorter than expected, mark as failure to indicate
            // a mis-match in test setup (this should not normally happen in valid tests).
            // We still attempt to compare, but the sentinel will indicate a mismatch.
        }
        ++xin;
        xout += xstep;
    }
}

// Test 1: Basic interlaced mapping for pass 0 (Adam7 first pass) with pixel_size = 1
// w = 10 -> expected writes at columns: 0, 8
static void test_deinterlace_row_pass0_pixel1_basic() {
    const char* test_name = "deinterlace_row_pass0_pixel1_basic";
    unsigned int pixel_size = 1;
    png_uint_32 w = 10;

    // Prepare an input row with enough pixels for the expected xin count.
    // For pass 0, there should be two writes (at xout 0 and 8) -> xin = 2
    std::vector<uint8_t> input_row(2 * pixel_size);
    // Distinct values per pixel for easy verification
    input_row[0] = 11; // pixel 0
    input_row[1] = 22; // pixel 0 (since pixel_size=1, only one byte per pixel)
    // Actually with pixel_size=1, just set two bytes consecutively:
    // pixel 0 -> value 11, pixel 1 -> value 22
    // We already set input_row[0]=11, input_row[1]=22

    // Prepare destination buffer (w * pixel_size bytes)
    std::vector<uint8_t> buffer(w * pixel_size, 0xAA); // sentinel 0xAA
    // Call the focal function
    deinterlace_row(buffer.data(), input_row.data(), pixel_size, w, 0, 0);

    // Compute expected mapping
    std::vector<uint8_t> expected;
    // Build input_row as two pixels ((pixel0 = 11), (pixel1 = 22)) -> we used two bytes
    compute_expected_mapping(expected, w, pixel_size, 0, input_row);

    bool ok = (buffer == expected);
    run_test(test_name, ok);

    // If not ok, print a diagnostic for debugging
    if (!ok) {
        std::cerr << "Expected:";
        for (size_t i = 0; i < expected.size(); ++i) std::cerr << " " << int(expected[i]);
        std::cerr << "\nActual:";
        for (size_t i = 0; i < buffer.size(); ++i) std::cerr << " " << int(buffer[i]);
        std::cerr << "\n";
    }
}

// Test 2: pass 6 (the last pass) with pixel_size = 1 and w = 8
// Expect a full write of all w pixels in order: buffer[i] == row[i] for i in [0..w-1]
static void test_deinterlace_row_pass6_pixel1_full() {
    const char* test_name = "deinterlace_row_pass6_pixel1_full";
    unsigned int pixel_size = 1;
    png_uint_32 w = 8;

    // For pass 6, xout starts at 0 and steps by 1, so xin will cover 0..7
    std::vector<uint8_t> input_row(8 * pixel_size);
    // Fill input row with distinct sequential values
    for (png_uint_32 i = 0; i < w; ++i) {
        input_row[i] = static_cast<uint8_t>(i + 1); // 1..8
    }

    std::vector<uint8_t> buffer(w * pixel_size, 0x00);
    deinterlace_row(buffer.data(), input_row.data(), pixel_size, w, 6, 0);

    // Expected mapping for pass 6
    std::vector<uint8_t> expected;
    compute_expected_mapping(expected, w, pixel_size, 6, input_row);

    bool ok = (buffer == expected);
    run_test(test_name, ok);

    if (!ok) {
        std::cerr << "Expected:";
        for (size_t i = 0; i < expected.size(); ++i) std::cerr << " " << int(expected[i]);
        std::cerr << "\nActual:";
        for (size_t i = 0; i < buffer.size(); ++i) std::cerr << " " << int(buffer[i]);
        std::cerr << "\n";
    }
}

// Test 3: pixel_size = 2 (two bytes per pixel) with pass 6 and w = 4
// This verifies that two-byte pixels are copied as a unit to the destination buffer
static void test_deinterlace_row_pass6_pixel2_two_byte_pixels() {
    const char* test_name = "deinterlace_row_pass6_pixel2_two_byte_pixels";
    unsigned int pixel_size = 2;
    png_uint_32 w = 4;

    // Input row contains 4 pixels, each 2 bytes
    std::vector<uint8_t> input_row(4 * pixel_size);
    // Pixel 0: 0x11 0x22, Pixel 1: 0x33 0x44, Pixel 2: 0x55 0x66, Pixel 3: 0x77 0x88
    input_row[0] = 0x11; input_row[1] = 0x22;
    input_row[2] = 0x33; input_row[3] = 0x44;
    input_row[4] = 0x55; input_row[5] = 0x66;
    input_row[6] = 0x77; input_row[7] = 0x88;

    std::vector<uint8_t> buffer(w * pixel_size, 0xAA);
    deinterlace_row(buffer.data(), input_row.data(), pixel_size, w, 6, 0);

    // Compute expected: for pass 6 and w=4, copy 4 pixels, in order
    std::vector<uint8_t> expected;
    compute_expected_mapping(expected, w, pixel_size, 6, input_row);

    bool ok = (buffer == expected);
    run_test(test_name, ok);

    if (!ok) {
        std::cerr << "Expected:";
        for (size_t i = 0; i < expected.size(); ++i) std::cerr << " " << std::hex << int(expected[i]);
        std::cerr << std::dec << "\nActual:";
        for (size_t i = 0; i < buffer.size(); ++i) std::cerr << " " << std::hex << int(buffer[i]);
        std::cerr << std::dec << "\n";
    }
}

// Entry point: run all tests and report summary
int main(void) {
    // Run a battery of tests to achieve reasonable coverage of the deinterlace_row logic.
    // Our tests focus on the mapping of input pixel indices to output buffer positions
    // for different interlace passes (primarily pass 0 and pass 6) and pixel sizes.

    test_deinterlace_row_pass0_pixel1_basic();
    test_deinterlace_row_pass6_pixel1_full();
    test_deinterlace_row_pass6_pixel2_two_byte_pixels();

    // Summary
    std::cout << "Tests passed: " << g_passed << ", Tests failed: " << g_failed << std::endl;
    return (g_failed == 0) ? 0 : 1;
}