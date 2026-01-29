// Test suite for the focal method: png_pass_rows
// This test suite is written in C++11, does not rely on GTest, and uses a lightweight
// custom test harness that relies only on the C ABI of the focal function.
// The focal function is declared with C linkage to ensure proper symbol resolution.
//
// Assumptions:
// - The focal function png_pass_rows is defined in a C source (pngvalid.c) and is linked
//   with this test binary.
// - The function signature used by the library is:
//     unsigned int png_pass_rows(unsigned int height, int pass);
// - We do not include or rely on libpng internals beyond the function under test.
// - We exercise true/false branches of the inner conditional by varying height and pass.
// - We verify basic invariants (e.g., height = 0 yields 0, monotonic behavior with height,
//   and expected non-zero for valid passes with full height, etc.)
//
// Note: If your build system already provides a main() in the C test source (pngvalid.c),
// you may need to adapt to avoid duplicate mains. Here, we provide a standalone main
// test harness; integrate as appropriate for your build configuration.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Declare the focal function with C linkage to ensure correct linking across C/C++ boundary.
extern "C" unsigned int png_pass_rows(unsigned int height, int pass);

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;

#define TEST_FAIL(msg) do { \
    std::cerr << "TEST FAILURE: " << (msg) << std::endl; \
    ++g_failures; \
} while (0)

#define ASSERT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        TEST_FAIL(std::string("ASSERT_EQ failed: ") + (msg) + \
                  " | expected: " + std::to_string(expected) + \
                  ", actual: " + std::to_string(actual)); \
    } \
} while (0)

#define ASSERT_GE(actual, boundary, msg) do { \
    if ((actual) < (boundary)) { \
        TEST_FAIL(std::string("ASSERT_GE failed: ") + (msg) + \
                  " | actual: " + std::to_string(actual) + \
                  ", lower_bound: " + std::to_string(boundary)); \
    } \
} while (0)

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        TEST_FAIL(std::string("ASSERT_TRUE failed: ") + (msg)); \
    } \
} while (0)


// Test 1: Height = 0 should always yield 0 regardless of pass
void test_png_pass_rows_height_zero_is_zero()
{
    unsigned int res0 = png_pass_rows(0u, 0);
    unsigned int res1 = png_pass_rows(0u, 5);
    unsigned int res2 = png_pass_rows(0u, 7);

    ASSERT_EQ(res0, 0u, "png_pass_rows(0,0) must return 0");
    ASSERT_EQ(res1, 0u, "png_pass_rows(0,5) must return 0");
    ASSERT_EQ(res2, 0u, "png_pass_rows(0,7) must return 0");
}

// Test 2: Monotonicity with height for a fixed pass
// For a fixed pass, increasing height (0..7) should never decrease the result.
void test_png_pass_rows_monotonic_with_height_for_each_pass()
{
    for (int pass = 0; pass <= 6; ++pass) { // Adam7 passes 1..7 map to input pass 0..6
        unsigned int last = png_pass_rows(0u, pass);
        // height from 1 to 7
        for (unsigned int h = 1; h <= 7; ++h) {
            unsigned int cur = png_pass_rows(h, pass);
            // The function uses height masked to 7 and increments rows when y < height.
            // Therefore, non-decreasing with height is expected.
            ASSERT_GE(cur, last, "png_pass_rows must be non-decreasing with height for fixed pass");
            last = cur;
        }
    }
}

// Test 3: For full height (height = 7, i.e., max masked height) the value should be > 0
// for all valid passes, while an invalid/overflowed pass (e.g., 7) should yield 0.
void test_png_pass_rows_full_height_behavior()
{
    // Valid passes (0..6) with height 7 should produce a positive number
    for (int pass = 0; pass <= 6; ++pass) {
        unsigned int res = png_pass_rows(7u, pass);
        ASSERT_TRUE(res > 0, "png_pass_rows(7, pass 0..6) should yield > 0");
    }

    // An invalid pass (7) mapping to comparator value 8 likely doesn't exist in adam7
    // and should yield 0 rows.
    unsigned int res_invalid = png_pass_rows(7u, 7);
    ASSERT_EQ(res_invalid, 0u, "png_pass_rows(7, 7) should yield 0 for invalid pass");
}

// Test 4: Height that triggers tiles (height >= 8).
// Use height = 8 to ensure tiles = height >> 3 == 1 and height masked to 0.
// We expect at least 1 row (tiles contributes) for all valid passes when patterns exist
// in the first tile's rows. For pass==7 (invalid), still 0.
void test_png_pass_rows_height_eight_tiles_behavior()
{
    // height = 8 -> tiles = 1, height masked to 0
    for (int pass = 0; pass <= 6; ++pass) {
        unsigned int res = png_pass_rows(8u, pass);
        // There should be at least one matching row in the first tile for a valid pass.
        ASSERT_GE(res, 1u, "png_pass_rows(8, pass 0..6) should yield >= 1");
    }
    // invalid pass
    unsigned int res_invalid = png_pass_rows(8u, 7);
    // For pass=7, there is no valid Adam7 mapping to 8, so expect 0
    ASSERT_EQ(res_invalid, 0u, "png_pass_rows(8, 7) should yield 0 for invalid pass");
}

// Helper to run all tests
void run_all_tests()
{
    test_png_pass_rows_height_zero_is_zero();
    test_png_pass_rows_monotonic_with_height_for_each_pass();
    test_png_pass_rows_full_height_behavior();
    test_png_pass_rows_height_eight_tiles_behavior();
}

// Entry point
int main()
{
    run_all_tests();

    if (g_failures == 0) {
        std::cout << "ALL PNG_PASS_ROWS TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " PNG_PASS_ROWS TEST(S) FAILED" << std::endl;
        return 1;
    }
}