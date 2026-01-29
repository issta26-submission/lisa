/*
Unit test suite for the focal method gpc_gprq located in pngstest.c

Overview:
- The focal method gpc_gprq applies a grayscale-like computation on an input Pixel
  and writes to an output Pixel. It uses a static-like dependency set (ilineara,
  u16d, YfromRGB, sRGB_to_d, etc.) that are defined in the same C translation unit
  in the original project. For testing, we link against the actual pngstest.c so that
  gpc_gprq executes with its intended internal dependencies.

- This test harness uses a minimal, non-terminating assertion style (no aborts on
  failure) and reports a final summary of failures. It avoids GTest and relies only
  on the C++ standard library.

Notes for the test runner:
- The test is written as a standalone C++11 file that declares the required C types
  and the gpc_gprq function with C linkage.
- Pixel is defined to match the expected layout (r, g, b, a as 16-bit unsigned).
- We provide two tests to cover both branches of the conditional predicate inside gpc_gprq:
  1) Equal channels (r == g == b) path
  2) Non-equal channels path (else branch)

Build guidance (external to this file):
- Compile and link this test with the C file containing gpc_gprq (pngstest.c):
  g++ -std=c++11 -I<path-to-png-headers-if-needed> test_gpc_gprq.cpp pngstest.c -o test_gpc_gprq
- Ensure that the PNG test environment provides the required dependencies for gpc_gprq
  (as in the original project). The test focuses on observable properties rather than exact
  numeric values when the internal exact mapping is not directly accessible.

*/

// Include standard headers
#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <ctype.h>


// If compiled as C++, ensure C linkage for the focal function and related types
extern "C" {
    // Pixel and Background definitions matching the usage in gpc_gprq.
    // The actual pngstest.c defines a similar Pixel type; this layout mirrors it.
    typedef struct {
        uint16_t r;
        uint16_t g;
        uint16_t b;
        uint16_t a;
    } Pixel;

    typedef struct {
        // Contents are not used by gpc_gprq directly (back is intentionally unused)
    } Background;

    // Focal function under test (assumed to be defined in pngstest.c)
    void gpc_gprq(Pixel *out, const Pixel *in, const Background *back);
}

// Lightweight non-terminating test harness
static int gFailures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::fprintf(stderr, "TEST FAILED: %s\n", (msg)); \
            ++gFailures; \
        } \
    } while (0)

#define EXPECT_EQ_UINT16(actual, expected, msg) \
    do { \
        if (static_cast<uint16_t>(actual) != static_cast<uint16_t>(expected)) { \
            std::fprintf(stderr, "TEST FAILED: %s (actual=%u, expected=%u)\n", (msg), (uint16_t)(actual), (uint16_t)(expected)); \
            ++gFailures; \
        } \
    } while (0)

// Test 1: Equal input channels should yield grayscale with alpha set to 65535
static void test_gpc_gprq_equal_channels() {
    Pixel in;
    in.r = 12345;
    in.g = 12345;
    in.b = 12345;
    in.a = 40000; // arbitrary alpha within 0..65535

    Pixel out;
    Background back; // unused by gpc_gprq, but provided to match signature

    gpc_gprq(&out, &in, &back);

    // Branch 1: all channels equal -> out should be grayscale (r==g==b)
    EXPECT_TRUE(out.r == out.g && out.g == out.b,
                "When input channels are equal, output must be grayscale (r==g==b).");
    // Branch 1: alpha must be set to 65535
    EXPECT_EQ_UINT16(out.a, 65535, "Output alpha must be 65535 for equal-channel input.");
}

// Test 2: Non-equal input channels should still yield a grayscale output color
// and alpha 65535, exercising the else-branch path.
static void test_gpc_gprq_non_equal_channels() {
    Pixel in;
    in.r = 10;
    in.g = 20;
    in.b = 30;
    in.a = 5000; // arbitrary alpha

    Pixel out;
    Background back; // unused by gpc_gprq

    gpc_gprq(&out, &in, &back);

    // Branch 2: ensure grayscale output (r==g==b)
    EXPECT_TRUE(out.r == out.g && out.g == out.b,
                "When input channels are not equal, output must be grayscale (r==g==b).");
    // Alpha should still be 65535
    EXPECT_EQ_UINT16(out.a, 65535, "Output alpha must be 65535 for non-equal-channel input.");
}

// Main driver: run all tests and report aggregate results
int main(void) {
    test_gpc_gprq_equal_channels();
    test_gpc_gprq_non_equal_channels();

    if (gFailures == 0) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        std::printf("TOTAL FAILURES: %d\n", gFailures);
        return gFailures;
    }
}