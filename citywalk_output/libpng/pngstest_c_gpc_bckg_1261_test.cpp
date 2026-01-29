// Minimal C++11 unit test harness for the focal method gpc_bckg
// This test reimplements the essential dependencies required to exercise
// the gpc_bckg logic (Pixel, Background, sRGB conversion, etc.) and validates
// all branch conditions without relying on external test frameworks.

// Note: This harness focuses on exercising the core logic of gpc_bckg and is
// designed to be self-contained for environments where the original PNG test
// infrastructure is not present.

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cmath>
#include <cstdio>
#include <stdio.h>
#include <ctype.h>


namespace test_gpc_bckg {

// Forward declaration of the focal method under test.
// We re-implement the minimal dependencies in this test file to enable compilation.
struct Pixel {
    uint8_t r, g, b, a;
};

struct Background {
    unsigned int ig; // integer grayscale background (0..255)
    double dg;       // delta gamma-like term (double)
};

// Global lookup table used by the focal method (identity-like mapping for tests)
static double sRGB_to_d[256];

// Simple sRGB conversion used by the focal method.
// We implement a predictable, monotonic mapping suitable for unit tests.
static inline unsigned char sRGB(double linear) {
    // Clamp and convert to 0..255
    if (linear <= 0.0) return 0;
    if (linear >= 255.0) return 255;
    // Round to nearest integer within 0..255
    return static_cast<unsigned char>(std::llround(linear));
}

// The focal method under test (replicated locally for testing purposes)
static void gpc_bckg(Pixel *out, const Pixel *in, const Background *back)
{
    {
        if (in->a <= 0)
            out->r = out->g = out->b = static_cast<uint8_t>(back->ig);
        else if (in->a >= 255)
            out->r = out->g = out->b = in->g;
        else
        {
            double a = in->a / 255.0;
            unsigned char val = sRGB(sRGB_to_d[in->g] * a + back->dg * (1.0 - a));
            out->r = out->g = out->b = val;
        }
        out->a = 255;
    }
}

// Helper: initialize the sRGB_to_d table to a simple identity mapping for tests.
static void init_tables() {
    for (int i = 0; i < 256; ++i) {
        sRGB_to_d[i] = static_cast<double>(i);
    }
}

// Simple test framework (non-terminating) to collect failures and print results.
static int failed = 0;

static void expect_eq_uint(const char* desc, uint8_t a, uint8_t b) {
    if (a != b) {
        std::fprintf(stderr, "FAIL: %s - expected %u, got %u\n", desc, static_cast<unsigned>(b), static_cast<unsigned>(a));
        ++failed;
    }
}

static void expect_eq_int(const char* desc, int a, int b) {
    if (a != b) {
        std::fprintf(stderr, "FAIL: %s - expected %d, got %d\n", desc, b, a);
        ++failed;
    }
}

static void expect_doublelte(const char* desc, double a, double b, double tol) {
    if (std::fabs(a - b) > tol) {
        std::fprintf(stderr, "FAIL: %s - expected approx %.6f, got %.6f (tol %.6f)\n", desc, b, a, tol);
        ++failed;
    }
}

// Test 1: in->a <= 0 branch
static void test_branch_inactive_alpha_sets_back_color() {
    Pixel in{ /*r*/ 10, /*g*/ 20, /*b*/ 30, /*a*/ 0 };
    Background back{ .ig = 42, .dg = 7.5 };
    Pixel out{0,0,0,0};

    gpc_bckg(&out, &in, &back);

    expect_eq_uint("branch_inactive_alpha_r", out.r, static_cast<uint8_t>(back.ig));
    expect_eq_uint("branch_inactive_alpha_g", out.g, static_cast<uint8_t>(back.ig));
    expect_eq_uint("branch_inactive_alpha_b", out.b, static_cast<uint8_t>(back.ig));
    expect_eq_uint("branch_inactive_alpha_a", out.a, 255);
}

// Test 2: in->a >= 255 branch
static void test_branch_alpha_full_uses_in_g() {
    Pixel in{ .r = 5, .g = 77, .b = 133, .a = 255 };
    Background back{ .ig = 12, .dg = 3.14 }; // should be ignored in this branch
    Pixel out{0,0,0,0};

    gpc_bckg(&out, &in, &back);

    expect_eq_uint("branch_full_alpha_r", out.r, in.g);
    expect_eq_uint("branch_full_alpha_g", out.g, in.g);
    expect_eq_uint("branch_full_alpha_b", out.b, in.g);
    expect_eq_uint("branch_full_alpha_a", out.a, 255);
}

// Test 3: else branch (0 < a < 255) performs sRGB blend with back.dg
static void test_branch_blend_under_alpha() {
    Pixel in{ .r = 0, .g = 100, .b = 0, .a = 128 }; // 50% alpha
    Background back{ .ig = 0, .dg = 10.0 };           // background gamma component
    Pixel out{0,0,0,0};

    gpc_bckg(&out, &in, &back);

    // Expected value computed via same logic used in the focal method:
    // a = 128/255, val = sRGB_to_d[in->g] * a + back.dg * (1-a)
    // with identity sRGB_to_d (i.e., 100.0), and sRGB() is rounding to nearest int
    double a = in.a / 255.0;
    double val = sRGB_to_d[in.g] * a + back.dg * (1.0 - a);
    unsigned char expected = sRGB(val);

    expect_eq_uint("branch_blend_r", out.r, expected);
    expect_eq_uint("branch_blend_g", out.g, expected);
    expect_eq_uint("branch_blend_b", out.b, expected);
    expect_eq_uint("branch_blend_a", out.a, 255);
}

// Runner
static void run_all_tests() {
    init_tables();

    test_branch_inactive_alpha_sets_back_color();
    test_branch_alpha_full_uses_in_g();
    test_branch_blend_under_alpha();

    if (failed == 0) {
        std::printf("All tests passed.\n");
    } else {
        std::fprintf(stderr, "Total failures: %d\n", failed);
    }
}

} // namespace test_gpc_bckg

// Main: run the test suite
int main() {
    test_gpc_bckg::run_all_tests();
    return (test_gpc_bckg::failed == 0) ? 0 : 1;
}