// Unit test suite for fclamp100 in fast_float_cmyk.c
// This test suite provides a lightweight test harness (no gtest) with
// non-terminating assertions to maximize code execution coverage.

#include <cmath>
#include <cstdio>
#include <fast_float_internal.h>
#include <limits>


// Declaration of the focal function (C linkage) for linking with the C source.
// cmsFloat32Number is typically a float; we mirror the signature as float.
extern "C" float fclamp100(float v);

// Simple lightweight test harness
static int g_total = 0;
static int g_failed = 0;

// Non-terminating assertion: checks a condition and reports if it fails.
// The test continues to execute after a failure.
#define EXPECT_TRUE(cond, msg) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        fprintf(stderr, "EXPECT_TRUE FAILED: %s\n", msg); \
    } \
} while(0)

// Floating-point equality within a tolerance
#define EXPECT_FLOAT_EQ(actual, expected, tol, msg) do { \
    ++g_total; \
    if(!((actual) <= (expected) + (tol) && (actual) >= (expected) - (tol))) { \
        ++g_failed; \
        fprintf(stderr, "EXPECT_FLOAT_EQ FAILED: %s; expected %g, got %g (tol=%g)\n", (msg), (expected), (actual), (tol)); \
    } \
} while(0)

// Run a focused set of tests for fclamp100
static void test_fclamp100_basic_behavior() {
    // Branch coverage: v < 1e-9f or isnan(v) -> 0.0
    // 1) v = 0.0f -> expect 0.0
    {
        float r = fclamp100(0.0f);
        EXPECT_FLOAT_EQ(r, 0.0f, 1e-6f, "fclamp100(0.0) should be 0.0");
    }

    // 2) v = 1e-10f (below threshold) -> 0.0
    {
        float r = fclamp100(1e-10f);
        EXPECT_FLOAT_EQ(r, 0.0f, 1e-6f, "fclamp100(1e-10) should be 0.0");
    }

    // 3) v = 1e-9f (boundary case) -> 1e-9
    {
        float r = fclamp100(1e-9f);
        EXPECT_FLOAT_EQ(r, 1e-9f, 1e-9f, "fclamp100(1e-9) boundary should return 1e-9");
    }

    // 4) v = -5.0f (negative values) -> 0.0 due to threshold
    {
        float r = fclamp100(-5.0f);
        EXPECT_FLOAT_EQ(r, 0.0f, 1e-6f, "fclamp100(-5.0) should be 0.0");
    }

    // Branch coverage: else path with 1e-9 <= v <= 100.0
    // 5) v = 50.0f -> 50.0
    {
        float r = fclamp100(50.0f);
        EXPECT_FLOAT_EQ(r, 50.0f, 1e-6f, "fclamp100(50.0) should be 50.0");
    }

    // 6) v = 100.0f -> 100.0
    {
        float r = fclamp100(100.0f);
        EXPECT_FLOAT_EQ(r, 100.0f, 1e-6f, "fclamp100(100.0) should be 100.0");
    }

    // 7) v = 150.0f (above upper bound) -> 100.0
    {
        float r = fclamp100(150.0f);
        EXPECT_FLOAT_EQ(r, 100.0f, 1e-6f, "fclamp100(150.0) should be 100.0");
    }

    // 8) NaN input -> 0.0
    {
        float nanv = std::numeric_limits<float>::quiet_NaN();
        float r = fclamp100(nanv);
        EXPECT_FLOAT_EQ(r, 0.0f, 1e-6f, "fclamp100(NaN) should be 0.0");
    }

    // 9) Boundary behavior: re-check boundary equals to threshold value
    {
        float r = fclamp100(1e-9f);
        EXPECT_FLOAT_EQ(r, 1e-9f, 1e-9f, "fclamp100(1e-9) boundary should return 1e-9");
    }
}

static void run_all_tests() {
    // This function orchestrates the test cases to drive coverage of fclamp100.
    // The tests aim to cover all branches and edge cases described in the method.
    test_fclamp100_basic_behavior();
}

// Entry point
int main() {
    // Run tests
    run_all_tests();

    // Summary
    printf("fclamp100 test summary: total=%d, failed=%d\n", g_total, g_failed);
    // Non-terminating behavior: return non-zero if any test failed
    return (g_failed != 0) ? 1 : 0;
}