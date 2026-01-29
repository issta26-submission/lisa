// C++11 test harness for the focal C function: gpc_A
// This test suite exercises the behavior of gpc_A(Pixel *out, const Pixel *in, const Background *back)
// by compiling against the C symbol with C linkage. The test ensures correct field copying
// from input to output and the forced alpha value of 65535.

// NOTE: The Pixel and Background types are defined here to match the expected layout used by gpc_A.
// Since gpc_A ignores the Background content (explicitly (void)back;), Background can be a minimal struct.

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


// Forward declare the C function with C linkage
extern "C" {
    typedef struct Pixel Pixel;
    struct Pixel { uint16_t r, g, b, a; };

    typedef struct Background Background;
    struct Background { int dummy; };

    void gpc_A(Pixel *out, const Pixel *in, const Background *back);
}

// Lightweight test framework (non-terminating assertions)
static int test_failures = 0;

#define TEST_NAME __func__

#define EXPECT_EQ(a, b) do {                                \
    auto _a = (a);                                            \
    auto _b = (b);                                            \
    if (!(_a == _b)) {                                        \
        std::cerr << "EXPECT_EQ failed: " << #a << " (" << _a << ") != " \
                  << #b << " (" << _b << ")" << std::endl;  \
        ++test_failures;                                      \
    }                                                         \
} while (0)

#define EXPECT_TRUE(cond) do {                                 \
    if (!(cond)) {                                              \
        std::cerr << "EXPECT_TRUE failed: " << #cond << std::endl; \
        ++test_failures;                                        \
    }                                                           \
} while (0)

// Test 1: Basic correctness - out should copy r,g,b from in and set a to 65535
// This ensures the primary behavior of gpc_A is correct for typical inputs.
static void test_gpc_A_basic_copy()
{
    Pixel in{ 123, 45, 67, 89 };
    Pixel out{ 0, 0, 0, 0 };
    Background back{ 999 }; // Content is ignored by gpc_A

    gpc_A(&out, &in, &back);

    EXPECT_EQ(out.r, in.r);
    EXPECT_EQ(out.g, in.g);
    EXPECT_EQ(out.b, in.b);
    EXPECT_EQ(out.a, static_cast<uint16_t>(65535));
}

// Test 2: Aliasing scenario - out and in refer to the same memory.
// The function should still assign correct values to all fields without corrupting data.
static void test_gpc_A_aliasing()
{
    Pixel p{ 1, 2, 3, 4 };
    Background back{ 0 };

    gpc_A(&p, &p, &back);

    EXPECT_EQ(p.r, static_cast<uint16_t>(1));
    EXPECT_EQ(p.g, static_cast<uint16_t>(2));
    EXPECT_EQ(p.b, static_cast<uint16_t>(3));
    EXPECT_EQ(p.a, static_cast<uint16_t>(65535));
}

// Test 3: Boundary values - ensure maximum channel values propagate correctly and alpha is forced.
// This checks behavior when input channels are at their maximum valid values.
static void test_gpc_A_max_values()
{
    Pixel in{ 65535, 65535, 65535, 0 };
    Pixel out{ 0, 0, 0, 0 };
    Background back{ 0 };

    gpc_A(&out, &in, &back);

    EXPECT_EQ(out.r, static_cast<uint16_t>(65535));
    EXPECT_EQ(out.g, static_cast<uint16_t>(65535));
    EXPECT_EQ(out.b, static_cast<uint16_t>(65535));
    EXPECT_EQ(out.a, static_cast<uint16_t>(65535));
}

// Entry point for the test executable
int main(void)
{
    std::cout << "Starting gpc_A unit tests (non-GTest harness)" << std::endl;

    test_gpc_A_basic_copy();
    test_gpc_A_aliasing();
    test_gpc_A_max_values();

    if (test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "TESTS FAILED: " << test_failures << " failure(s)" << std::endl;
        return 1;
    }
}