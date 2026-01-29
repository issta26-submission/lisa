// A self-contained unit test for the focal method gpc_pre.
// This test is written in C++11 and does not depend on external testing frameworks.
// It re-implements the essential behavior of gpc_pre for isolated testing.

#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <ctype.h>


// Lightweight replacements for the real project types.
// We intentionally keep the types simple to allow straightforward unit testing.
struct Pixel {
    int r;
    int g;
    int b;
    int a;
};

struct Background {
    // Placeholder: not used by the focal logic in this test.
};

// Forward declaration of ilineara as used by gpc_pre.
// In the real project this would be provided elsewhere; here we implement a
// simple identity-like function to keep tests deterministic.
extern "C" int ilineara(int fixed_srgb, int alpha);

// A minimal, self-contained implementation of ilineara for testing.
// We ignore alpha for simplicity and return the fixed_srgb value.
// This mirrors the behavior used in our test scenarios where ilineara is
// treated as a deterministic mapper from input color component to an output value.
int ilineara(int fixed_srgb, int alpha) {
    (void)alpha; // alpha is not used in this simplified mock
    return fixed_srgb;
}

// The focal function under test (re-implemented here to be self-contained).
// It follows the exact logic provided in the prompt.
void gpc_pre(Pixel *out, const Pixel *in, const Background *back)
{
    (void)back;
    out->r = ilineara(in->r, in->a);
    if (in->g == in->r)
    {
        out->g = out->r;
        if (in->b == in->r)
            out->b = out->r;
        else
            out->b = ilineara(in->b, in->a);
    }
    else
    {
        out->g = ilineara(in->g, in->a);
        if (in->b == in->r)
            out->b = out->r;
        else if (in->b == in->g)
            out->b = out->g;
        else
            out->b = ilineara(in->b, in->a);
    }
    out->a = in->a * 257;
}

// Simple test harness ---------------------------------------------------------

static int g_tests = 0;
static int g_failures = 0;

static void log_pixel_mismatch(const char* test_name,
                               const Pixel& actual, const Pixel& expected,
                               int line)
{
    std::cerr << "Test failed: " << test_name
              << " (line " << line << ")\n"
              << "  expected Pixel(" << expected.r << "," << expected.g << ","
              << expected.b << "," << expected.a << ")\n"
              << "  actual   Pixel(" << actual.r << "," << actual.g << ","
              << actual.b << "," << actual.a << ")\n";
}

static bool pixels_equal(const Pixel& a, const Pixel& b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

// Test helper macros
#define EXPECT_PIXEL_EQ(actual, expected, test_name) do { \
    ++g_tests; \
    if (!pixels_equal((actual), (expected))) { \
        ++g_failures; \
        log_pixel_mismatch((test_name), (actual), (expected), __LINE__); \
    } \
} while (0)

#define EXPECT_EQ(expected, actual, test_name) do { \
    ++g_tests; \
    if ((expected) != (actual)) { \
        ++g_failures; \
        std::cerr << "Test failed: " << (test_name) \
                  << " (line " << __LINE__ << "): expected " \
                  << (expected) << " but got " << (actual) << "\n"; \
    } \
} while (0)

// Entry point
int main(void)
{
    Background back; // unused in tests

    // Test 1: g == r and b == r (true-true path with both nested ifs)
    // Expect r = ilineara(r, a) = r; g = r; b = r; a = a * 257
    {
        Pixel in{10, 10, 10, 2};       // r=g=b
        Pixel out{0,0,0,0};
        gpc_pre(&out, &in, &back);
        Pixel expected{10, 10, 10, 2 * 257};
        EXPECT_PIXEL_EQ(out, expected, "Case1: g==r, b==r");
    }

    // Test 2: g == r and b != r (true-false path inside second if)
    // Expect r = ilineara(r, a) = r; g = r; b = ilineara(b, a) = b; a = a * 257
    {
        Pixel in{10, 10, 20, 3};       // r=g != b
        Pixel out{0,0,0,0};
        gpc_pre(&out, &in, &back);
        Pixel expected{10, 10, 20, 3 * 257};
        EXPECT_PIXEL_EQ(out, expected, "Case2: g==r, b!=r");
    }

    // Test 3: g != r and b == r (first else branch with inner if)
    // Expect r = ilineara(r, a) = r; g = ilineara(g, a) = g;
    // since b == r, out.b = out.r; a = a * 257
    {
        Pixel in{5, 7, 5, 1};          // r != g, b == r
        Pixel out{0,0,0,0};
        gpc_pre(&out, &in, &back);
        Pixel expected{5, 7, 5, 1 * 257};
        EXPECT_PIXEL_EQ(out, expected, "Case3: g!=r, b==r");
    }

    // Test 4: g != r and b == g (second inner else-if)
    // Expect r = ilineara(r, a) = r; g = ilineara(g, a) = g; b = g
    {
        Pixel in{7, 9, 9, 5};          // r != g, b == g
        Pixel out{0,0,0,0};
        gpc_pre(&out, &in, &back);
        Pixel expected{7, 9, 9, 5 * 257};
        // Note: b should copy from g in this path
        EXPECT_PIXEL_EQ(out, expected, "Case4: g!=r, b==g");
    }

    // Test 5: g != r and b != r and b != g (else branch bottom)
    // Expect r = ilineara(r, a) = r; g = ilineara(g, a) = g; b = ilineara(b, a) = b;
    // a = a * 257
    {
        Pixel in{8, 9, 12, 4};         // g != r; b is distinct from r and g
        Pixel out{0,0,0,0};
        gpc_pre(&out, &in, &back);
        Pixel expected{8, 9, 12, 4 * 257};
        EXPECT_PIXEL_EQ(out, expected, "Case5: g!=r, b!=r, b!=g");
    }

    // Summary
    std::cout << "Unit test results: " << g_tests << " tests, "
              << g_failures << " failures.\n";

    // Return non-zero if there were failures to aid integration with build systems.
    return (g_failures == 0) ? 0 : 1;
}