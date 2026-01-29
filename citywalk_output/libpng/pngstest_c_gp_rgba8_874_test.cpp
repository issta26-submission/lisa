// Test suite for the focal method gp_rgba8
// This test is implemented in C++11 style, without using GTest.
// It re-implements the minimal necessary environment to exercise gp_rgba8 logic.

#include <cstdint>
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


// Domain-context notes (for maintainers):
// - Candidate Keywords: Pixel, r, g, b, a, pb (png_const_voidp), pp (png_const_bytep),
//   voidcast, gp_rgba8, rgba channels.
// - We mimic the essential parts of the environment needed by gp_rgba8:
//   Pixel structure, png_const_voidp and the helper cast (voidcast).

// Lightweight re-creation of the types used by gp_rgba8

typedef uint8_t  png_byte;
typedef const png_byte* png_const_bytep;
typedef const void* png_const_voidp;

// Minimal mimic of the voidcast used in the focal method.
// In the real code, voidcast(png_const_bytep, pb) yields a png_const_bytep from a void*.
// We implement a simple, type-safe version suitable for tests.
static inline png_const_bytep voidcast(png_const_bytep /*ignored*/, png_const_voidp pb)
{
    return static_cast<png_const_bytep>(pb);
}

// Pixel structure as used by the focal method.
struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// Focal method under test (replicated here for self-contained unit tests)
void gp_rgba8(Pixel *p, png_const_voidp pb)
{
    png_const_bytep pp = voidcast(png_const_bytep, pb);
    p->r = pp[0];
    p->g = pp[1];
    p->b = pp[2];
    p->a = pp[3];
}

// Simple non-terminating test framework (no terminate on failure)
static int tests_run = 0;
static int tests_failed = 0;

#define TEST_ASSERT_TRUE(cond, msg) do {                               \
    ++tests_run;                                                         \
    if (!(cond)) {                                                       \
        ++tests_failed;                                                  \
        fprintf(stderr, "TEST_FAIL: %s\n", msg);                       \
    } else {                                                             \
        fprintf(stderr, "TEST_PASS: %s\n", msg);                       \
    }                                                                    \
} while(0)

#define TEST_ASSERT_EQ_U8(actual, expected, msg) do {                    \
    ++tests_run;                                                         \
    if (static_cast<uint8_t>(actual) != static_cast<uint8_t>(expected)) {\
        ++tests_failed;                                                  \
        fprintf(stderr, "TEST_FAIL: %s (expected %u, got %u)\n",        \
                msg, static_cast<unsigned>(expected),                   \
                static_cast<unsigned>(actual));                       \
    } else {                                                             \
        fprintf(stderr, "TEST_PASS: %s\n", msg);                        \
    }                                                                    \
} while(0)

// Test 1: Basic RGBA assignment with typical values
// - Verifies that gp_rgba8 correctly maps 4 input bytes to r, g, b, a.
void test_gp_rgba8_basic()
{
    // Arrange
    Pixel p{};
    uint8_t bytes[4] = { 10, 20, 30, 40 }; // RGBA
    // Act
    gp_rgba8(&p, static_cast<png_const_voidp>(bytes));
    // Assert
    TEST_ASSERT_EQ_U8(p.r, 10, "gp_rgba8_basic: r channel should be 10");
    TEST_ASSERT_EQ_U8(p.g, 20, "gp_rgba8_basic: g channel should be 20");
    TEST_ASSERT_EQ_U8(p.b, 30, "gp_rgba8_basic: b channel should be 30");
    TEST_ASSERT_EQ_U8(p.a, 40, "gp_rgba8_basic: a channel should be 40");
}

// Test 2: Boundary values for each channel
// - Ensures correct handling of min and max values (0x00 and 0xFF).
void test_gp_rgba8_boundary_values()
{
    Pixel p{};
    uint8_t bytes[4] = { 0x00, 0xFF, 0x7F, 0x80 }; // 0,255,127,128
    gp_rgba8(&p, static_cast<png_const_voidp>(bytes));
    TEST_ASSERT_EQ_U8(p.r, 0x00, "gp_rgba8_boundary: r should be 0");
    TEST_ASSERT_EQ_U8(p.g, 0xFF, "gp_rgba8_boundary: g should be 255");
    TEST_ASSERT_EQ_U8(p.b, 0x7F, "gp_rgba8_boundary: b should be 127");
    TEST_ASSERT_EQ_U8(p.a, 0x80, "gp_rgba8_boundary: a should be 128");
}

// Test 3: Multiple consecutive invocations with different input buffers
// - Verifies that no state persists between calls and that each call uses the provided data.
void test_gp_rgba8_multiple_calls()
{
    Pixel p1{}, p2{};
    uint8_t a1[4] = { 1, 2, 3, 4 };
    uint8_t a2[4] = { 5, 6, 7, 8 };

    gp_rgba8(&p1, static_cast<png_const_voidp>(a1));
    gp_rgba8(&p2, static_cast<png_const_voidp>(a2));

    TEST_ASSERT_EQ_U8(p1.r, 1, "gp_rgba8_multiple_calls: p1.r should be 1");
    TEST_ASSERT_EQ_U8(p1.g, 2, "gp_rgba8_multiple_calls: p1.g should be 2");
    TEST_ASSERT_EQ_U8(p1.b, 3, "gp_rgba8_multiple_calls: p1.b should be 3");
    TEST_ASSERT_EQ_U8(p1.a, 4, "gp_rgba8_multiple_calls: p1.a should be 4");

    TEST_ASSERT_EQ_U8(p2.r, 5, "gp_rgba8_multiple_calls: p2.r should be 5");
    TEST_ASSERT_EQ_U8(p2.g, 6, "gp_rgba8_multiple_calls: p2.g should be 6");
    TEST_ASSERT_EQ_U8(p2.b, 7, "gp_rgba8_multiple_calls: p2.b should be 7");
    TEST_ASSERT_EQ_U8(p2.a, 8, "gp_rgba8_multiple_calls: p2.a should be 8");
}

// Test 4: Data length beyond 4 bytes does not affect gp_rgba8 behavior
// - Ensure only the first four bytes are read (consistent with implementation).
void test_gp_rgba8_ignores_extra_bytes()
{
    Pixel p{};
    uint8_t buffer[8] = { 9, 8, 7, 6, 255, 0, 0, 0 };
    gp_rgba8(&p, static_cast<png_const_voidp>(buffer));
    TEST_ASSERT_EQ_U8(p.r, 9, "gp_rgba8_ignores_extra: r should be 9");
    TEST_ASSERT_EQ_U8(p.g, 8, "gp_rgba8_ignores_extra: g should be 8");
    TEST_ASSERT_EQ_U8(p.b, 7, "gp_rgba8_ignores_extra: b should be 7");
    TEST_ASSERT_EQ_U8(p.a, 6, "gp_rgba8_ignores_extra: a should be 6");
}

// Main function to run all tests (no gtest, per requirements)
int main(void)
{
    // Announce test start
    fprintf(stdout, "Starting gp_rgba8 unit tests (C++11, no GTest)\n");

    test_gp_rgba8_basic();
    test_gp_rgba8_boundary_values();
    test_gp_rgba8_multiple_calls();
    test_gp_rgba8_ignores_extra_bytes();

    // Summary
    fprintf(stdout, "Tests run: %d\n", tests_run);
    if (tests_failed == 0) {
        fprintf(stdout, "ALL TESTS PASSED\n");
        return 0;
    } else {
        fprintf(stderr, "TOTAL FAILURES: %d\n", tests_failed);
        return 1;
    }
}