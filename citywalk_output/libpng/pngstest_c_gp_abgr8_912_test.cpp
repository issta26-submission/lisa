// C++11 standalone test suite for the focal method gp_abgr8
// This test suite is designed to be run without GoogleTest.
// It tests the mapping from a 4-byte ABGR input to a Pixel RGBA structure.

#include <cstdint>
#include <cstddef>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Minimal Pixel structure to mirror the dependency in the focal code.
struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// Focal method under test.
// In the original code, pb is a pointer to 4 bytes in ABGR order.
// It maps to r,g,b,a accordingly: r = pb[3], g = pb[2], b = pb[1], a = pb[0].
void gp_abgr8(Pixel *p, const void* pb)
{
    const uint8_t* pp = reinterpret_cast<const uint8_t*>(pb);
    p->r = pp[3];
    p->g = pp[2];
    p->b = pp[1];
    p->a = pp[0];
}

// Simple test framework (non-terminating assertions)
static int total_tests = 0;
static int failed_tests = 0;

#define RUN_TEST(fn) do { ++total_tests; bool _ok = (fn)(); if (!_ok) ++failed_tests; } while(0)

// Helper to print a failure message for uint8 comparisons
static bool expect_uint8_eq(uint8_t actual, uint8_t expected, const char* label)
{
    if (actual != expected) {
        std::cerr << "FAIL: " << label
                  << " - expected: " << static_cast<int>(expected)
                  << ", actual: " << static_cast<int>(actual) << "\n";
        return false;
    }
    return true;
}

// Test 1: Basic ABGR -> RGBA mapping
// Input: pb = {A=0x11, B=0x22, G=0x33, R=0x44}
// Expected: r=0x44, g=0x33, b=0x22, a=0x11
static bool test_gp_abgr8_basic()
{
    Pixel p;
    uint8_t pb[4] = { 0x11, 0x22, 0x33, 0x44 };
    gp_abgr8(&p, pb);
    bool ok = true;
    ok &= expect_uint8_eq(p.r, 0x44, "gp_abgr8_basic.r");
    ok &= expect_uint8_eq(p.g, 0x33, "gp_abgr8_basic.g");
    ok &= expect_uint8_eq(p.b, 0x22, "gp_abgr8_basic.b");
    ok &= expect_uint8_eq(p.a, 0x11, "gp_abgr8_basic.a");
    return ok;
}

// Test 2: Different ABGR pattern to ensure correct byte indexing
// Input: pb = {A=0xAA, B=0xBB, G=0xCC, R=0xDD}
// Expected: r=0xDD, g=0xCC, b=0xBB, a=0xAA
static bool test_gp_abgr8_pattern2()
{
    Pixel p;
    uint8_t pb[4] = { 0xAA, 0xBB, 0xCC, 0xDD };
    gp_abgr8(&p, pb);
    bool ok = true;
    ok &= expect_uint8_eq(p.r, 0xDD, "gp_abgr8_pattern2.r");
    ok &= expect_uint8_eq(p.g, 0xCC, "gp_abgr8_pattern2.g");
    ok &= expect_uint8_eq(p.b, 0xBB, "gp_abgr8_pattern2.b");
    ok &= expect_uint8_eq(p.a, 0xAA, "gp_abgr8_pattern2.a");
    return ok;
}

// Test 3: Ensure memory location (pb) is not modified by the function
// pb should remain exactly as provided after gp_abgr8 is called.
static bool test_gp_abgr8_pb_immutable()
{
    Pixel p;
    uint8_t pb[4] = { 0x01, 0x02, 0x03, 0x04 };
    uint8_t pb_before[4];
    std::memcpy(pb_before, pb, 4);
    gp_abgr8(&p, pb);
    // Verify pb is unchanged
    bool unchanged = (std::memcmp(pb, pb_before, 4) == 0);
    if (!unchanged) {
        std::cerr << "FAIL: gp_abgr8_pb_immutable - pb memory was modified\n";
        return false;
    }
    // Also verify the mapping still holds
    bool ok = true;
    ok &= expect_uint8_eq(p.r, 0x04, "gp_abgr8_pb_immutable.r");
    ok &= expect_uint8_eq(p.g, 0x03, "gp_abgr8_pb_immutable.g");
    ok &= expect_uint8_eq(p.b, 0x02, "gp_abgr8_pb_immutable.b");
    ok &= expect_uint8_eq(p.a, 0x01, "gp_abgr8_pb_immutable.a");
    return ok;
}

// Test 4: Edge values (0x00 and 0xFF) to verify full byte range handling
// Input: pb = {A=0x00, B=0xFF, G=0x00, R=0xFF}
// Expected: r=0xFF, g=0x00, b=0xFF, a=0x00
static bool test_gp_abgr8_extremes()
{
    Pixel p;
    uint8_t pb[4] = { 0x00, 0xFF, 0x00, 0xFF };
    gp_abgr8(&p, pb);
    bool ok = true;
    ok &= expect_uint8_eq(p.r, 0xFF, "gp_abgr8_extremes.r");
    ok &= expect_uint8_eq(p.g, 0x00, "gp_abgr8_extremes.g");
    ok &= expect_uint8_eq(p.b, 0xFF, "gp_abgr8_extremes.b");
    ok &= expect_uint8_eq(p.a, 0x00, "gp_abgr8_extremes.a");
    return ok;
}

// Main function to run all tests
int main(void)
{
    // Run all tests; collect and report results
    RUN_TEST(test_gp_abgr8_basic);
    RUN_TEST(test_gp_abgr8_pattern2);
    RUN_TEST(test_gp_abgr8_pb_immutable);
    RUN_TEST(test_gp_abgr8_extremes);

    std::cout << "Total tests: " << total_tests
              << ", Failures: " << failed_tests << "\n";

    // Return non-zero if any test failed
    return (failed_tests == 0) ? 0 : 1;
}