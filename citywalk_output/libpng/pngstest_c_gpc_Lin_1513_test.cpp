/*
   Unit test suite for the focal method gpc_Lin in pngstest.c
   - Implemented in C linkage for compatibility with the C codebase
   - Uses a small custom test framework (no GTest) with non-terminating assertions
   - Covers all branches of the inner conditional logic in gpc_Lin
   - Validates that channels r, g, b are computed via ilinear_g22 and a is set to 65535
   - Demonstrates handling of equal channels and various combinations of (r, g, b)
   - Designed to be compiled and linked with the project (C and C++11 compatible)
*/

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>


// Lightweight assertion helpers (non-terminating)
static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT_EQ(a, b) do {                                    \
    ++tests_run;                                                  \
    if ((static_cast<long long>(a)) != (static_cast<long long>(b))) { \
        std::cerr << "Assertion failed: " #a " == " #b \
                  << " (actual: " << (a) << ", " << (b) << ")" << std::endl; \
        ++tests_failed;                                           \
    }                                                             \
} while (0)

#define ASSERT_TRUE(cond) do {                                    \
    ++tests_run;                                                  \
    if (!(cond)) {                                                \
        std::cerr << "Assertion failed: condition is false: " #cond << std::endl; \
        ++tests_failed;                                           \
    }                                                             \
} while (0)

extern "C" {
    // Minimal forward declarations matching the project's C structures and functions.
    // We rely on the actual PNG test module to provide the real implementations at link time.
    typedef struct Pixel {
        uint16_t r;
        uint16_t g;
        uint16_t b;
        uint16_t a;
    } Pixel;

    typedef struct Background {
        // The actual project may have fields; for tests we only need a placeholder.
        int dummy;
    } Background;

    // Focal method under test
    Pixel gpc_Lin(Pixel *out, const Pixel *in, const Background *back);

    // Helper function used inside gpc_Lin
    int ilinear_g22(int fixed_srgb);
}

// Test suite for gpc_Lin
// Each test corresponds to a path through the nested conditional logic,
// ensuring both true/false branches are exercised.

static void test_all_equal_channels() {
    // All input channels are equal: r == g == b
    // Expected: r,g,b all equal to ilinear_g22(in.r); a should be 65535
    Pixel in{ 12345, 12345, 12345, 0 };
    Pixel out{0, 0, 0, 0};
    Background back{0};

    Pixel expected_r = out; // placeholder to ensure type consistency
    // expected value computed via the internal mapping function
    int mapped = ilinear_g22((int)in.r);
    Pixel expected;
    expected.r = static_cast<uint16_t>(mapped);
    expected.g = static_cast<uint16_t>(mapped);
    expected.b = static_cast<uint16_t>(mapped);
    expected.a = 65535;

    gpc_Lin(&out, &in, &back);

    ASSERT_EQ(out.r, expected.r);
    ASSERT_EQ(out.g, expected.g);
    ASSERT_EQ(out.b, expected.b);
    ASSERT_EQ(out.a, 65535);
}

static void test_r_eq_g_not_b() {
    // r == g but b is different
    // Expected: r and g use ilinear_g22(in.r); b uses ilinear_g22(in.b)
    Pixel in{ 1000, 1000, 2000, 0 };
    Pixel out{0, 0, 0, 0};
    Background back{0};

    int mapped_r = ilinear_g22((int)in.r);
    int mapped_b = ilinear_g22((int)in.b);

    Pixel expected;
    expected.r = static_cast<uint16_t>(mapped_r);
    expected.g = static_cast<uint16_t>(mapped_r);
    expected.b = static_cast<uint16_t>(mapped_b);
    expected.a = 65535;

    gpc_Lin(&out, &in, &back);

    ASSERT_EQ(out.r, expected.r);
    ASSERT_EQ(out.g, expected.g);
    ASSERT_EQ(out.b, expected.b);
    ASSERT_EQ(out.a, 65535);
}

static void test_r_ne_g_b_eq_r() {
    // r != g, but b == r
    // Expected: r = ilinear_g22(in.r); g = ilinear_g22(in.g); b = r
    Pixel in{ 100, 300, 100, 0 };
    Pixel out{0, 0, 0, 0};
    Background back{0};

    int mapped_r = ilinear_g22((int)in.r);
    int mapped_g = ilinear_g22((int)in.g);
    int mapped_b = mapped_r;

    Pixel expected;
    expected.r = static_cast<uint16_t>(mapped_r);
    expected.g = static_cast<uint16_t>(mapped_g);
    expected.b = static_cast<uint16_t>(mapped_b);
    expected.a = 65535;

    gpc_Lin(&out, &in, &back);

    ASSERT_EQ(out.r, expected.r);
    ASSERT_EQ(out.g, expected.g);
    ASSERT_EQ(out.b, expected.b);
    ASSERT_EQ(out.a, 65535);
}

static void test_r_ne_g_b_eq_g() {
    // r != g, but b == g
    // Expected: r = ilinear_g22(in.r); g = ilinear_g22(in.g); b = g
    Pixel in{ 100, 300, 300, 0 };
    Pixel out{0, 0, 0, 0};
    Background back{0};

    int mapped_r = ilinear_g22((int)in.r);
    int mapped_g = ilinear_g22((int)in.g);

    Pixel expected;
    expected.r = static_cast<uint16_t>(mapped_r);
    expected.g = static_cast<uint16_t>(mapped_g);
    expected.b = static_cast<uint16_t>(mapped_g);
    expected.a = 65535;

    gpc_Lin(&out, &in, &back);

    ASSERT_EQ(out.r, expected.r);
    ASSERT_EQ(out.g, expected.g);
    ASSERT_EQ(out.b, expected.b);
    ASSERT_EQ(out.a, 65535);
}

static void test_all_diff_channels() {
    // All channels distinct: r, g, b map independently
    Pixel in{ 10, 20, 30, 0 };
    Pixel out{0, 0, 0, 0};
    Background back{0};

    int mr = ilinear_g22((int)in.r);
    int mg = ilinear_g22((int)in.g);
    int mb = ilinear_g22((int)in.b);

    Pixel expected;
    expected.r = static_cast<uint16_t>(mr);
    expected.g = static_cast<uint16_t>(mg);
    expected.b = static_cast<uint16_t>(mb);
    expected.a = 65535;

    gpc_Lin(&out, &in, &back);

    ASSERT_EQ(out.r, expected.r);
    ASSERT_EQ(out.g, expected.g);
    ASSERT_EQ(out.b, expected.b);
    ASSERT_EQ(out.a, 65535);
}

static void run_all_tests() {
    test_all_equal_channels();
    test_r_eq_g_not_b();
    test_r_ne_g_b_eq_r();
    test_r_ne_g_b_eq_g();
    test_all_diff_channels();
}

int main(void) {
    run_all_tests();

    std::cout.setf(std::ios::fmtflags(0), std::ios::basefield);
    std::cout << "gpc_Lin test results: run=" << tests_run
              << ", failures=" << tests_failed << std::endl;

    // Return non-zero if any test failed
    return (tests_failed > 0) ? 1 : 0;
}