// High-quality C++ test harness for the focal method gpc_gpre
// This test suite is designed to be compiled with C++11 and linked
// with the C implementation contained in pngstest.c (or the project binary).
// It exercises both branches of the conditional inside gpc_gpre:
// 1) gray input (r == g == b)  -> uses ilineara
// 2) non-gray input            -> uses YfromRGB and sRGB_to_d via u16d
//
// Notes:
// - We declare a Pixel struct locally (with the same layout as expected by the C code)
//   to allow constructing test inputs in C++ code. This mirrors the commonly used
//   layout (r, g, b, a) for 16-bit channels.
// - The tests rely on the library's public interface and on helper functions
//   ilineara, u16d, YfromRGB and the sRGB_to_d array being available via linkage.
// - The tests do not terminate on failure; they print PASS/FAIL and continue to
//   maximize code coverage as requested.

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Forward-declare C linkage for the focal function and related helpers.
// We avoid including any private headers; we rely on the project to provide
// the correct binary-compatible symbols at link time.

extern "C" {

    // Pixel and Background structures assumed to be layout-compatible with the
    // definitions used inside pngstest.c. We mirror the layout here for test input construction.
    typedef struct {
        uint16_t r;
        uint16_t g;
        uint16_t b;
        uint16_t a;
    } Pixel;

    typedef struct {
        // Contents are not used by gpc_gpre; this is a placeholder to satisfy type.
        int dummy;
    } Background;

    // Focal function under test
    void gpc_gpre(Pixel *out, const Pixel *in, const Background *back);

    // Helper functions used by gpc_gpre (signatures inferred from usage in source)
    // The exact semantics are assumed to be provided by the linked C library.
    uint16_t ilineara(uint16_t fixed_srgb, uint16_t alpha);
    uint16_t u16d(double d);

    // sRGB conversion table and internal Y conversion
    extern const double sRGB_to_d[256];
    double YfromRGB(double r, double g, double b);
}

// Simple non-terminating test harness
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_PASS(name) \
    do { printf("PASS: %s\n", (name)); tests_passed++; } while (0)

#define TEST_FAIL(name, msg) \
    do { printf("FAIL: %s - %s\n", (name), (msg)); tests_failed++; } while (0)

#define EXPECT_EQ_U16(actual, expected, test_name, detail) \
    do { \
        tests_run++; \
        if ((actual) == (expected)) { TEST_PASS(test_name); } \
        else { TEST_FAIL(test_name, detail); } \
    } while (0)

#define EXPECT_EQ_STR(actual, expected, test_name, detail) \
    do { \
        tests_run++; \
        if (std::strcmp((actual), (expected)) == 0) { TEST_PASS(test_name); } \
        else { TEST_FAIL(test_name, detail); } \
    } while (0)


// Test 1: Gray path (in.r == in.g == in.b) should set out channels to ilineara(in.g, in.a)
// and set alpha to 257 * in.a
void test_gray_path() {
    // Prepare input: gray pixel with value 123 and alpha 100
    Pixel in;
    in.r = 123;
    in.g = 123;
    in.b = 123;
    in.a = 100;

    Pixel out;
    Background back = { 0 }; // back is unused by gpc_gpre

    gpc_gpre(&out, &in, &back);

    // Expected computations
    uint16_t expected_gray = ilineara(in.g, in.a);
    uint16_t expected_alpha = static_cast<uint16_t>(257u * in.a);

    // Validate that RGB channels are gray and match expected value
    char test_name_rgb[] = "Gray path RGB channels equal to ilineara(in.g, in.a)";
    if (out.r == expected_gray && out.g == expected_gray && out.b == expected_gray) {
        TEST_PASS(test_name_rgb);
        tests_run++;
    } else {
        TEST_FAIL(test_name_rgb, "RGB channels do not match expected gray value");
        // Do not early return; still verify alpha to maximize coverage
    }

    // Validate alpha
    char test_name_a[] = "Gray path alpha == 257 * in.a";
    if (out.a == expected_alpha) {
        TEST_PASS(test_name_a);
    } else {
        TEST_FAIL(test_name_a, "Alpha value incorrect");
    }
}

// Test 2: Non-gray path should compute Y from RGB and convert via u16d
void test_non_gray_path() {
    // Prepare input: non-gray pixel
    Pixel in;
    in.r = 10;
    in.g = 20;
    in.b = 30;
    in.a = 120;

    Pixel out;
    Background back = { 0 };

    gpc_gpre(&out, &in, &back);

    // Compute expected using the available helpers
    double y = YfromRGB(sRGB_to_d[in.r], sRGB_to_d[in.g], sRGB_to_d[in.b]);
    double combined = static_cast<double>(in.a) * 257.0 * y;
    uint16_t expected = u16d(combined);

    // Validate that output channels are equal to expected value and alpha is scaled
    char test_name_rgb[] = "Non-gray path RGB channels equal to u16d(in.a * 257 * YfromRGB(...))";
    if (out.r == expected && out.g == expected && out.b == expected) {
        TEST_PASS(test_name_rgb);
    } else {
        TEST_FAIL(test_name_rgb, "RGB channels do not match expected non-gray value");
    }

    uint16_t expected_alpha = static_cast<uint16_t>(257u * in.a);
    char test_name_a[] = "Non-gray path alpha == 257 * in.a";
    if (out.a == expected_alpha) {
        TEST_PASS(test_name_a);
    } else {
        TEST_FAIL(test_name_a, "Alpha value incorrect in non-gray path");
    }
}

// Optional: ensure back parameter is unused (static analysis could confirm). This test
// simply exercises a non-null back pointer to ensure no crashes occur.
void test_back_parameter_unused() {
    Pixel in;
    in.r = 80;
    in.g = 80;
    in.b = 80;
    in.a = 200;

    Pixel out;
    Background back = { 42 }; // arbitrary non-zero to verify it's ignored

    gpc_gpre(&out, &in, &back);

    // Gray check, since r=g=b
    uint16_t expected_gray = ilineara(in.g, in.a);
    if (out.r == expected_gray && out.g == expected_gray && out.b == expected_gray) {
        TEST_PASS("Back parameter unused (non-null) did not affect gray path");
    } else {
        TEST_FAIL("Back parameter unused", "Unexpected effect from non-null back parameter");
    }
}

// Entry-point test runner
int main(void) {
    printf("Starting gpc_gpre unit tests...\n");

    test_gray_path();
    test_non_gray_path();
    test_back_parameter_unused();

    // Summary
    printf("Tests run: %d, Passed: %d, Failed: %d\n",
           tests_run, tests_passed, tests_failed);

    // Return non-zero if any test failed
    if (tests_failed != 0)
        return 1;
    return 0;
}