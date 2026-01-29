// Test suite for the focal method gpc_g16q in a C/C++ mixed project.
// This test is written in C++11 (no GoogleTest) and uses a lightweight
// self-contained harness with a minimal, non-terminating assertion style.
// The test targets the behavior of gpc_g16q which computes the luma of the input
// RGB color and writes it to the output pixel's r,g,b channels (as 16-bit values),
// while copying the alpha channel. The 'back' parameter is unused in the function
// (explicitly cast to void), so we pass a nullptr for simplicity.
//
// Assumptions based on the focal method:
// - YfromRGBint(r,g,b) returns the luma value on a 0..255 scale (common 8-bit Y).
// - u16d(y) converts a 0..255 Y value to a 16-bit value by scaling to 0..65535.
//   A typical mapping is out16 = round(y * 257) so that 255 maps to 65535.
// These helpers are provided by the PNG testing framework in the original project.
// For unit testing purposes we rely on the actual implementation in the linked C source.

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <iomanip>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <ctype.h>


// Forward declaration of the function under test from the C sources.
// We assume a C interface to gpc_g16q with Pixel and Background types.
extern "C" {
    // Lightweight Pixel and Background definitions matching the usage in gpc_g16q.
    // The input pixel components are treated as 8-bit values logically,
    // but stored here in 16-bit fields to align with the production code's
    // 16-bit output channels (r,g,b,a) per the provided focal method.
    typedef struct Pixel {
        uint16_t r;
        uint16_t g;
        uint16_t b;
        uint16_t a;
    } Pixel;

    typedef struct Background {
        // Intentionally empty as the focal method ignores this parameter
        // (except for the explicit (void)back; suppression).
        int dummy;
    } Background;

    // Function under test
    void gpc_g16q(Pixel *out, const Pixel *in, const Background *back);
}

// Lightweight, domain-appropriate, non-terminating assertions
static int tests_run = 0;
static int tests_failed = 0;

#define TESTCASE(name) TestCase name##_case;
#define RUN_TEST(name) do {                                           \
    if (!name##_case()) {                                              \
        std::cerr << "Test failed: " #name "\n";                       \
        ++tests_failed;                                                \
    } else {                                                           \
        std::cout << "Test passed: " #name "\n";                      \
    }                                                                  \
} while(0)

typedef bool (*TestCaseFunc)();

static bool case_black_white_alpha() {
    // Test 1: Black color with alpha 0
    // Expected: r=g=b=0, a preserved
    Pixel in{0, 0, 0, 0};
    Pixel out{0, 0, 0, 0};
    Background back{0};

    gpc_g16q(&out, &in, &back);

    // Reference: Y from RGB 0,0,0 => 0, u16d(0) => 0
    // All channels should be 0; alpha should equal input a (0)
    uint16_t expected = 0;
    bool pass = (out.r == expected) && (out.g == expected) && (out.b == expected) && (out.a == in.a);
    if (!pass) {
        std::cerr << "  [case_black_white_alpha] Expected (0,0,0," << in.a
                  << "), got (" << out.r << "," << out.g << "," << out.b
                  << "," << out.a << ")\n";
    }
    return pass;
}

static bool case_white_fullalpha() {
    // Test 2: White color with full alpha
    Pixel in{255, 255, 255, 255}; // input RGB in 8-bit range, a=255
    Pixel out{0, 0, 0, 0};
    Background back{0};

    gpc_g16q(&out, &in, &back);

    // Reference: Y = 255, out16 = llround(255 * 257) = 65535
    uint16_t expected = static_cast<uint16_t>(llround(255.0 * 257.0));
    bool pass = (out.r == expected) && (out.g == expected) && (out.b == expected) && (out.a == in.a);
    if (!pass) {
        std::cerr << std::setw(18) << "  [case_white_fullalpha] "
                  << "Expected (" << expected << "," << expected << "," << expected
                  << "," << in.a << "), got (" << out.r << "," << out.g << "," << out.b
                  << "," << out.a << ")\n";
    }
    return pass;
}

static bool case_red_only() {
    // Test 3: Pure red with moderate alpha
    Pixel in{255, 0, 0, 100};
    Pixel out{0, 0, 0, 0};
    Background back{0};

    gpc_g16q(&out, &in, &back);

    // Reference for red: Y = 0.299*255 ≈ 76.245; out16 ≈ round(76.245 * 257) ≈ 19595
    uint16_t expected = static_cast<uint16_t>(llround((0.299 * 255.0) * 257.0));
    bool pass = (out.r == expected) && (out.g == expected) && (out.b == expected) && (out.a == in.a);
    if (!pass) {
        std::cerr << std::setw(18) << "  [case_red_only] "
                  << "Expected (" << expected << "," << expected << "," << expected
                  << "," << in.a << "), got (" << out.r << "," << out.g << "," << out.b
                  << "," << out.a << ")\n";
    }
    return pass;
}

static bool case_green_only() {
    // Test 4: Pure green with medium alpha
    Pixel in{0, 255, 0, 200};
    Pixel out{0, 0, 0, 0};
    Background back{0};

    gpc_g16q(&out, &in, &back);

    // Reference for green: Y = 0.587*255 ≈ 149.685; out16 ≈ round(149.685 * 257) ≈ 38469
    uint16_t expected = static_cast<uint16_t>(llround((0.587 * 255.0) * 257.0));
    bool pass = (out.r == expected) && (out.g == expected) && (out.b == expected) && (out.a == in.a);
    if (!pass) {
        std::cerr << std::setw(18) << "  [case_green_only] "
                  << "Expected (" << expected << "," << expected << "," << expected
                  << "," << in.a << "), got (" << out.r << "," << out.g << "," << out.b
                  << "," << out.a << ")\n";
    }
    return pass;
}

static bool case_blue_only() {
    // Test 5: Pure blue with some alpha
    Pixel in{0, 0, 255, 50};
    Pixel out{0, 0, 0, 0};
    Background back{0};

    gpc_g16q(&out, &in, &back);

    // Reference for blue: Y = 0.114*255 ≈ 29.07; out16 ≈ round(29.07 * 257) ≈ 7471
    uint16_t expected = static_cast<uint16_t>(llround((0.114 * 255.0) * 257.0));
    bool pass = (out.r == expected) && (out.g == expected) && (out.b == expected) && (out.a == in.a);
    if (!pass) {
        std::cerr << std::setw(18) << "  [case_blue_only] "
                  << "Expected (" << expected << "," << expected << "," << expected
                  << "," << in.a << "), got (" << out.r << "," << out.g << "," << out.b
                  << "," << out.a << ")\n";
    }
    return pass;
}

// Helper to invoke Y from RGB + u16d-like interpretation for expected value.
// This mirrors the common conversion used by the focal method to derive 16-bit output.
static uint16_t expected_luma16(uint8_t r, uint8_t g, uint8_t b) {
    double y = 0.299 * static_cast<double>(r)
             + 0.587 * static_cast<double>(g)
             + 0.114 * static_cast<double>(b);
    double out16 = y * 257.0;
    return static_cast<uint16_t>(llround(out16));
}

// Entry point of the test suite
int main() {
    // Attach a friendly banner
    std::cout << "Running gpc_g16q unit tests (C++11 harness, no GTest)\n";

    // Expose test case function pointers to a simple runner
    // and ensure we exercise a variety of input colors.
    // Note: The real code expects a C translation unit; this harness
    // assumes the build links gpc_g16q from the provided C source.
    tests_run = 0;
    tests_failed = 0;

    // NOTE: We implement a small wrapper approach by reusing the static test
    // case functions defined above. In a fuller harness, you might register
    // tests dynamically; here we invoke them in a deterministic order.

    // Compile-time linkage sanity: define a macro to aid in consistent reporting
    #undef RUN_TEST
    #define RUN_TEST(name) do { \
        ++tests_run; \
        if ((name##_case())) { \
            std::cout << "Test passed: " #name "\n"; \
        } else { \
            ++tests_failed; \
            std::cerr << "Test failed: " #name "\n"; \
        } \
    } while(0)

    // Bind test cases to the runner
    TESTCASE(black_white_alpha);
    TESTCASE(white_fullalpha);
    TESTCASE(red_only);
    TESTCASE(green_only);
    TESTCASE(blue_only);

    // Execute tests
    RUN_TEST(case_black_white_alpha);
    RUN_TEST(case_white_fullalpha);
    RUN_TEST(case_red_only);
    RUN_TEST(case_green_only);
    RUN_TEST(case_blue_only);

    // Final summary
    std::cout << "Total tests run: " << tests_run << "\n";
    if (tests_failed > 0) {
        std::cerr << "Total failures: " << tests_failed << "\n";
        return 1;
    } else {
        std::cout << "All tests passed.\n";
        return 0;
    }
}