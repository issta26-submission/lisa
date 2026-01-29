// Test suite for fclamp128 function (cmsFloat32Number fclamp128(cmsFloat32Number v))
// This test uses a lightweight, non-GTest approach with non-terminating EXPECT-style assertions.

#include <iostream>
#include <cmath>
#include <fast_float_internal.h>
#include <limits>



// Declare the focal function with C linkage to avoid name mangling issues during linking.
// The function signature uses cmsFloat32Number (likely typedef float) as in the provided file.
extern "C" {
    cmsFloat32Number fclamp128(cmsFloat32Number v);
}

// Simple, non-terminating test harness
static int g_total = 0;
static int g_failed = 0;

// Non-terminating assertion macro: records failure but does not abort the test run.
#define EXPECT_EQ(a, b) do { \
    ++g_total; \
    if (!((a) == (b))) { \
        ++g_failed; \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " -> expected: " << (b) << " , actual: " << (a) << std::endl; \
    } \
} while (0)

// Test 1: Value well below -128 should clamp to -128
// Coverage: predicate (v < -128) is true -> clamps to -128
void test_fclamp128_below_negative_bound() {
    cmsFloat32Number v = -200.0f;
    cmsFloat32Number r = fclamp128(v);
    EXPECT_EQ(r, -128.0f);
}

// Test 2: Exact lower bound -128 should remain -128
// Coverage: predicate (v < -128) is false and v != NaN, next clamp path
void test_fclamp128_at_negative_bound() {
    cmsFloat32Number v = -128.0f;
    cmsFloat32Number r = fclamp128(v);
    EXPECT_EQ(r, -128.0f);
}

// Test 3: Inside the valid range should be returned unchanged
// Coverage: path where -128 <= v <= 128 and not NaN
void test_fclamp128_inside_range() {
    cmsFloat32Number v1 = -127.0f;
    cmsFloat32Number r1 = fclamp128(v1);
    EXPECT_EQ(r1, -127.0f);

    cmsFloat32Number v2 = 0.0f;
    cmsFloat32Number r2 = fclamp128(v2);
    EXPECT_EQ(r2, 0.0f);

    cmsFloat32Number v3 = 127.0f;
    cmsFloat32Number r3 = fclamp128(v3);
    EXPECT_EQ(r3, 127.0f);
}

// Test 4: Upper bounds and slight overflows:
// - 128.0f should return 128.0f
// - 128.0001f (just above threshold) should clamp to 128.0f
// - Very large positive should clamp to 128.0f
void test_fclamp128_at_positive_bound_and_over() {
    cmsFloat32Number v = 128.0f;
    cmsFloat32Number r = fclamp128(v);
    EXPECT_EQ(r, 128.0f);

    v = 128.0001f;
    r = fclamp128(v);
    EXPECT_EQ(r, 128.0f);

    v = 9999.0f;
    r = fclamp128(v);
    EXPECT_EQ(r, 128.0f);
}

// Test 5: NaN input should be treated as below threshold and clamp to -128
// Coverage: isnan(v) path should cause -128 result
void test_fclamp128_nan_input() {
    cmsFloat32Number nanVal = std::numeric_limits<cmsFloat32Number>::quiet_NaN();
    cmsFloat32Number r = fclamp128(nanVal);
    EXPECT_EQ(r, -128.0f);
}

int main() {
    // Run tests
    test_fclamp128_below_negative_bound();
    test_fclamp128_at_negative_bound();
    test_fclamp128_inside_range();
    test_fclamp128_at_positive_bound_and_over();
    test_fclamp128_nan_input();

    // Summary
    std::cout << "fclamp128 test suite completed. "
              << "Total tests: " << g_total
              << ", Failures: " << g_failed << std::endl;

    // Return non-zero on any failure to aid CI systems
    return (g_failed == 0) ? 0 : 1;
}