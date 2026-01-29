// Pixel comparison unit tests for the focal method pixel_cmp
// This test suite is crafted to exercise the pixel_cmp function logic
// (as defined in the provided FOCAL_METHOD) without relying on GTest.
// It uses simple, non-terminating assertions via a custom reporter.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Domain-type aliases to mirror png_uint_32 and related types
using png_uint_32 = uint32_t;
using png_const_bytep = const unsigned char*;

// Minimal re-implementation of the focal pixel_cmp function
// Signature mirrors the original for compatibility with the test harness.
static png_uint_32 pixel_cmp(png_const_bytep pa, png_const_bytep pb, png_uint_32 bit_width)
{
    // Compare the first (bit_width+7)>>3 bytes
    if (memcmp(pa, pb, (bit_width + 7) >> 3) == 0)
        return 0;

    // Return the 1-based index of the first differing byte
    {
        png_uint_32 where = 0;
        while (pa[where] == pb[where]) ++where;
        return 1 + where;
    }
}

// Simple test harness (non-terminating, prints results)
static int total_tests = 0;
static int passed_tests = 0;

void report_test(const std::string& name, bool ok, const std::string& detail)
{
    ++total_tests;
    if (ok) {
        ++passed_tests;
        std::cout << "[PASSED] " << name;
        if (!detail.empty()) std::cout << " - " << detail;
        std::cout << std::endl;
    } else {
        std::cout << "[FAILED] " << name;
        if (!detail.empty()) std::cout << " - " << detail;
        std::cout << std::endl;
    }
}

// Helper to run a single test case
void run_case(const std::string& name,
              const uint8_t* pa, const uint8_t* pb,
              png_uint_32 bit_width, uint32_t expected)
{
    uint32_t result = pixel_cmp(pa, pb, bit_width);
    bool ok = (result == expected);
    std::string detail = "got " + std::to_string(result) + ", expected " + std::to_string(expected);
    report_test(name, ok, detail);
}

int main()
{
    using namespace std;

    // Test Case 1:
    // Bit width 0: N = (0+7)>>3 = 0, so memcmp length is 0.
    // Only the zero-length comparison is performed; result should be 0.
    {
        const uint8_t a[4] = {0x01, 0x02, 0x03, 0x04};
        const uint8_t b[4] = {0x01, 0xFF, 0xEE, 0xDD};
        run_case("bit_width=0: identical (N=0) returns 0",
                 a, b, 0, 0);
    }

    // Test Case 2:
    // Bit width 7 -> N = 1. If first byte is identical, pixel_cmp should return 0
    // even if later bytes differ (demonstrates the early memcmp check).
    {
        const uint8_t a[3] = {0xAA, 0xBB, 0xCC};
        const uint8_t b[3] = {0xAA, 0x99, 0x99}; // later bytes differ, but N=1
        run_case("bit_width=7: first byte equal, later bytes differ -> returns 0",
                 a, b, 7, 0);
    }

    // Test Case 3:
    // Bit width 7 -> N = 1. First byte differs -> should return 1
    {
        const uint8_t a[2] = {0x01, 0xAA};
        const uint8_t b[2] = {0x02, 0xAA};
        run_case("bit_width=7: first byte differs -> returns 1",
                 a, b, 7, 1);
    }

    // Test Case 4:
    // Bit width 14 -> N = 2. First byte equal, second byte differs -> should return 2
    {
        const uint8_t a[3] = {0x10, 0x20, 0x33};
        const uint8_t b[3] = {0x10, 0x21, 0x44};
        run_case("bit_width=14: second byte differs -> returns 2",
                 a, b, 14, 2);
    }

    // Test Case 5:
    // Bit width 15 -> N = 2. First byte differs -> should return 1
    {
        const uint8_t a[2] = {0x05, 0x06};
        const uint8_t b[2] = {0x06, 0x06};
        run_case("bit_width=15: first byte differs -> returns 1",
                 a, b, 15, 1);
    }

    // Summary
    std::cout << "\nTest summary: " << passed_tests << " / " << total_tests
              << " tests passed." << std::endl;

    // Return code: 0 if all tests passed, non-zero otherwise
    return (passed_tests == total_tests) ? 0 : 1;
}