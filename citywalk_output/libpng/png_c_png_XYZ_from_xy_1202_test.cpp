// Test suite for the focal method: png_XYZ_from_xy
// Target file: png.c (function: png_XYZ_from_xy)
// Testing framework: lightweight C++11 test harness (no GTest)
// Notes:
// - We rely on libpng public/internal API headers to access png_XYZ_from_xy, png_XYZ, png_xy.
// - The tests exercise true/false branches of input validation and a successful path.
// - All tests use non-terminating assertions to maximize code coverage.

#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


// Include libpng headers to access required types and the focal function.
// Depending on your build, you may need to adjust the include path.


// Lightweight non-terminating assertion macros
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::fprintf(stderr, "Expectation failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
    } \
} while(0)

#define EXPECT_GE(a, b) do { \
    if(!((a) >= (b))) { \
        std::fprintf(stderr, "Expectation failed: %s >= %s (actual: %lld vs %lld) at %s:%d\n", #a, #b, static_cast<long long>(a), static_cast<long long>(b), __FILE__, __LINE__); \
    } \
} while(0)

#define EXPECT_NEQ(a, b) do { \
    if((a) == (b)) { \
        std::fprintf(stderr, "Expectation failed: %s != %s (value: %lld) at %s:%d\n", #a, #b, static_cast<long long>(a), __FILE__, __LINE__); \
    } \
} while(0)


// Helper to convert a double in [0, ~1.1] to libpng fixed-point
static inline png_fixed_point FP(double v) {
    // libpng uses 16.16 fixed point (PNG_FP_1 == 1<<16)
    // Clamp to some reasonable range to avoid overflow in tests
    if (v < 0.0) v = 0.0;
    // We keep a little headroom for values slightly above 1.0
    double capped = (v > 2.0) ? 2.0 : v;
    return static_cast<png_fixed_point>(capped * static_cast<double>(PNG_FP_1) + 0.5);
}

// Human-friendly sRGB fixed-point chromaticities (approximate)
// red ~ (0.64, 0.33), green ~ (0.30, 0.60), blue ~ (0.15, 0.06), white ~ (0.3127, 0.3290)
static void make_srgb_xy(png_xy &xy) {
    xy.redx   = FP(0.64);
    xy.redy   = FP(0.33);
    xy.greenx = FP(0.30);
    xy.greeny = FP(0.60);
    xy.bluex  = FP(0.15);
    xy.bluey  = FP(0.06);
    xy.whitex  = FP(0.3127);
    xy.whitey  = FP(0.3290);
}

// Test 1: Valid sRGB-like xy should succeed (return 0) and fill XYZ reasonably
static void test_XYZ_from_xy_valid_srgb() {
    png_xy xy;
    make_srgb_xy(xy);

    png_XYZ XYZ;
    std::memset(&XYZ, 0, sizeof(XYZ));

    int res = png_XYZ_from_xy(&XYZ, &xy);
    EXPECT_TRUE(res == 0);

    // Basic sanity checks on produced XYZ values (non-negative and finite)
    // Depending on libpng internal scaling, exact values vary; we just ensure they are non-negative.
    EXPECT_TRUE(XYZ.red_X  >= 0);
    EXPECT_TRUE(XYZ.red_Y  >= 0);
    EXPECT_TRUE(XYZ.red_Z  >= 0);
    EXPECT_TRUE(XYZ.green_X >= 0);
    EXPECT_TRUE(XYZ.green_Y >= 0);
    EXPECT_TRUE(XYZ.green_Z >= 0);
    EXPECT_TRUE(XYZ.blue_X  >= 0);
    EXPECT_TRUE(XYZ.blue_Y  >= 0);
    EXPECT_TRUE(XYZ.blue_Z  >= 0);
}

// Test 2: Negative redx should trigger a failure (first bound check)
static void test_XYZ_from_xy_negative_redx() {
    png_xy xy;
    make_srgb_xy(xy);
    xy.redx = -1; // violate lower bound

    png_XYZ XYZ;
    std::memset(&XYZ, 0, sizeof(XYZ));

    int res = png_XYZ_from_xy(&XYZ, &xy);
    EXPECT_TRUE(res == 1);
}

// Test 3: redy out of allowed range (greater than fpLimit - redx)
static void test_XYZ_from_xy_redy_out_of_range() {
    png_xy xy;
    make_srgb_xy(xy);
    // Force redy to exceed the limit by increasing it slightly
    // fpLimit ~ 1.1 in fixed-point; set redx ~ 0.5 to create room, then push redy beyond limit
    xy.redx = FP(0.50);
    xy.redy = FP(0.70); // fpLimit - redx ≈ 0.60; 0.70 > 0.60 -> fail

    png_XYZ XYZ;
    std::memset(&XYZ, 0, sizeof(XYZ));

    int res = png_XYZ_from_xy(&XYZ, &xy);
    EXPECT_TRUE(res == 1);
}

// Test 4: whitey too small (below 5 fixed-point units)
static void test_XYZ_from_xy_whitey_too_small() {
    png_xy xy;
    make_srgb_xy(xy);
    xy.whitey = 4; // below the minimum threshold (5)

    png_XYZ XYZ;
    std::memset(&XYZ, 0, sizeof(XYZ));

    int res = png_XYZ_from_xy(&XYZ, &xy);
    EXPECT_TRUE(res == 1);
}

// Test 5: whitey too large relative to whitex (exceeds fpLimit - whitex)
static void test_XYZ_from_xy_whitey_too_large() {
    png_xy xy;
    make_srgb_xy(xy);
    // Set whitex small (near 0) to tighten the upper bound
    xy.whitex = FP(0.0);
    // Then set whitey above the limit
    // fpLimit ~ 1.1, so whitey > 1.1 will trigger
    xy.whitey = FP(1.2); // 1.2 > fpLimit when whitex ~ 0

    png_XYZ XYZ;
    std::memset(&XYZ, 0, sizeof(XYZ));

    int res = png_XYZ_from_xy(&XYZ, &xy);
    EXPECT_TRUE(res == 1);
}

// Test 6: Negative redy should fail (second condition)
static void test_XYZ_from_xy_negative_redy() {
    png_xy xy;
    make_srgb_xy(xy);
    xy.redy = -1; // negative

    png_XYZ XYZ;
    std::memset(&XYZ, 0, sizeof(XYZ));

    int res = png_XYZ_from_xy(&XYZ, &xy);
    EXPECT_TRUE(res == 1);
}

// Test harness: run all tests
int main() {
    std::cout << "Running png_XYZ_from_xy unit tests (C++11)..." << std::endl;

    test_XYZ_from_xy_valid_srgb();
    test_XYZ_from_xy_negative_redx();
    test_XYZ_from_xy_redy_out_of_range();
    test_XYZ_from_xy_whitey_too_small();
    test_XYZ_from_xy_whitey_too_large();
    test_XYZ_from_xy_negative_redy();

    std::cout << "Unit tests completed.\n";
    return 0;
}