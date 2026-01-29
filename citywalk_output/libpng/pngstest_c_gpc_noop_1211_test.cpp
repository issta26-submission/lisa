// This is a lightweight, GTest-free unit test suite for the focal method:
//   gpc_noop(Pixel *out, const Pixel *in, const Background *back)
// The function copies RGBA from 'in' to 'out' and ignores 'back'.
// The test suite is written in C++11 and uses simple, non-terminating assertions
// (i.e., it logs failures but continues executing) to maximize coverage.

/*
Assumptions about Pixel and Background (as used by gpc_noop):
struct Pixel {
    unsigned char r, g, b, a;
};
struct Background {
    int dummy; // actual structure may differ; tests treat back as unused
};

The actual implementation in the project matches these basic semantics.
We declare them here to enable testing from C++ code and declare the gpc_noop
function as extern "C" to link against the C implementation.
*/

#include <cstdint>
#include <vector>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


extern "C" {
    // Minimal compatibility typedefs to mirror the focal method's expected types.
    typedef struct Pixel {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } Pixel;

    typedef struct Background {
        int dummy;
    } Background;

    // Focal method under test (assumed to be linked from the project under test)
    void gpc_noop(Pixel *out, const Pixel *in, const Background *back);
}

// Lightweight test framework (non-terminating assertions)
static int g_test_total = 0;
static int g_test_failed = 0;

#define TEST_EXPECT_TRUE(cond, msg) do { \
    ++g_test_total; \
    if (!(cond)) { \
        ++g_test_failed; \
        std::fprintf(stderr, "Test failed: %s\n", (msg)); \
    } \
} while (0)

#define TEST_EXPECT_EQ(actual, expected, msg) do { \
    ++g_test_total; \
    if ((actual) != (expected)) { \
        ++g_test_failed; \
        std::fprintf(stderr, "Test failed: %s (actual=%u, expected=%u)\n", (msg), (unsigned)(actual), (unsigned)(expected)); \
    } \
} while (0)

#define TEST_EXPECT_PIXEL_EQ(p1, p2, msg) do { \
    ++g_test_total; \
    bool eq = ((p1).r == (p2).r) && ((p1).g == (p2).g) && ((p1).b == (p2).b) && ((p1).a == (p2).a); \
    if (!eq) { \
        ++g_test_failed; \
        std::fprintf(stderr, "Test failed: %s (out={%u,%u,%u,%u}, expect={%u,%u,%u,%u})\n", \
            (msg), (unsigned)(p1).r, (unsigned)(p1).g, (unsigned)(p1).b, (unsigned)(p1).a, \
            (unsigned)(p2).r, (unsigned)(p2).g, (unsigned)(p2).b, (unsigned)(p2).a); \
    } \
} while (0)


// Helper: set a Pixel to specific RGBA values
static Pixel make_pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    Pixel p;
    p.r = r; p.g = g; p.b = b; p.a = a;
    return p;
}


// Test 1: Basic copy - ensure gpc_noop copies all channels from 'in' to 'out'
static void test_gpc_noop_basic_copy() {
    // Arrange
    Pixel in = make_pixel(10, 20, 30, 40);
    Pixel out = make_pixel(1, 2, 3, 4);
    Background back = {123}; // arbitrary; should be ignored

    // Act
    gpc_noop(&out, &in, &back);

    // Assert
    TEST_EXPECT_PIXEL_EQ(out, in, "Basic copy: out should equal in after gpc_noop");
    // Also ensure in remains unchanged
    TEST_EXPECT_PIXEL_EQ(in, make_pixel(10, 20, 30, 40), "In should remain unchanged after gpc_noop");
}


// Test 2: Edge values - verify handling of 0 and 255 across channels
static void test_gpc_noop_edge_values() {
    Pixel in = make_pixel(0, 255, 0, 255);
    Pixel out = make_pixel(123, 45, 67, 89);
    Background back = {0};

    gpc_noop(&out, &in, &back);

    TEST_EXPECT_PIXEL_EQ(out, in, "Edge values: out should equal in when values include 0 and 255");
}


// Test 3: Self-aliasing call - out points to the same object as in
static void test_gpc_noop_self_aliasing() {
    Pixel p = make_pixel(7, 14, 21, 28);
    Background back = {0};

    // Call with out == in
    gpc_noop(&p, &p, &back);

    // The operation should be equivalent to no-op except for ensuring no corruption occurs
    TEST_EXPECT_PIXEL_EQ(p, make_pixel(7, 14, 21, 28), "Self-aliasing: pixel should remain unchanged");
}


// Test 4: Multiple outputs - ensure only given out is written; other pixels remain unaffected
static void test_gpc_noop_out_isolated() {
    Pixel in = make_pixel(5, 6, 7, 8);
    Pixel out0 = make_pixel(0, 0, 0, 0);
    Pixel out1 = make_pixel(1, 1, 1, 1);
    // We'll simulate two independent calls, ensuring only the targeted 'out' is modified.
    Background back = {0};

    // Act on first pixel
    gpc_noop(&out0, &in, &back);
    // Copy is independent for second pixel (out1 must stay unchanged)
    TEST_EXPECT_PIXEL_EQ(out0, in, "Out0 updated correctly from in");
    TEST_EXPECT_PIXEL_EQ(out1, make_pixel(1, 1, 1, 1), "Out1 remains unaffected by first call");

    // Now perform second call to ensure no cross-contamination
    gpc_noop(&out1, &in, &back);
    TEST_EXPECT_PIXEL_EQ(out1, in, "Out1 updated correctly after second call");
}


// Test 5: Randomized values - stress test with a few deterministic random combinations
static void test_gpc_noop_random_values() {
    Background back = {0};
    // Deterministic "random" seeds
    const uint32_t seeds[] = { 0xDEADBEEF, 0x12345678, 0x0, 0xFFFFFFFF };
    Pixel out = {0,0,0,0};
    for (size_t si = 0; si < sizeof(seeds)/sizeof(seeds[0]); ++si) {
        uint32_t seed = seeds[si];
        // simple LCG-like update to generate values
        uint8_t r = (uint8_t)(seed ^ 0xAA);
        uint8_t g = (uint8_t)((seed >> 8) ^ 0x55);
        uint8_t b = (uint8_t)((seed >> 16) ^ 0x33);
        uint8_t a = (uint8_t)((seed >> 24) ^ 0x77);
        Pixel in = make_pixel(r, g, b, a);
        // vary out
        Pixel expected = in;
        out = make_pixel(0,0,0,0);

        gpc_noop(&out, &in, &back);

        TEST_EXPECT_PIXEL_EQ(out, expected, "Random values: out should equal in for randomized inputs");
    }
}


// Main test runner
int main(void) {
    // Run each test in sequence
    printf("Starting gpc_noop unit tests (no GTest required)\n");

    test_gpc_noop_basic_copy();
    test_gpc_noop_edge_values();
    test_gpc_noop_self_aliasing();
    test_gpc_noop_out_isolated();
    test_gpc_noop_random_values();

    // Summary
    printf("gpc_noop tests completed. Total: %d, Failed: %d\n",
           g_test_total, g_test_failed);

    // Return non-zero if any test failed
    if (g_test_failed != 0) {
        return 1;
    }
    return 0;
}