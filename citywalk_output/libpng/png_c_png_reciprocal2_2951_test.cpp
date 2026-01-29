// Lightweight C++11 unit tests for the focal method: png_reciprocal2
// This test suite is designed to be compiled and run without GTest.
// It uses a minimal, non-terminating assertion framework and calls the C function
// png_reciprocal2 from C code (extern "C").
// Explanatory comments accompany each unit test to describe intent and coverage.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Lightweight, non-terminating assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(actual, expected) do { \
    ++g_total_tests; \
    if ((actual) != (expected)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << "  Expected: " << (expected) \
                  << "  Actual: " << (actual) << "\n"; \
        ++g_failed_tests; \
    } \
} while (0)

#define EXPECT_NE(actual, not_expected) do { \
    ++g_total_tests; \
    if ((actual) == (not_expected)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << "  Value unexpectedly equal to: " << (not_expected) << "\n"; \
        ++g_failed_tests; \
    } \
} while (0)

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << "  Condition false: " #cond "\n"; \
        ++g_failed_tests; \
    } \
} while (0)

#define EXPECT_FALSE(cond) do { \
    ++g_total_tests; \
    if (cond) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << "  Condition true: " #cond "\n"; \
        ++g_failed_tests; \
    } \
} while (0)

// Declare the focal C function to be tested, ensuring C linkage in C++ test.
// We assume png_fixed_point is a 32-bit signed integer in the libpng implementation.
extern "C" {
    typedef int32_t png_fixed_point;
    // Function under test: png_reciprocal2(png_fixed_point a, png_fixed_point b)
    png_fixed_point png_reciprocal2(png_fixed_point a, png_fixed_point b);
}

// Test suite for the floating-point arithmetic path (when PNG_FLOATING_ARITHMETIC_SUPPORTED is defined)
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED

// Coverage: true branch that computes an exact reciprocal-like value within 32-bit range.
// We select values so 1e15 / (a*b) yields a precise, in-range integer after flooring.
static void test_png_reciprocal2_floating_true_values()
{
    // Case: a*b = 1e6 -> r = 1e9, within int32 range
    // a = 1000, b = 1000 -> r = floor(1e15/1e6 + 0.5) = 1_000_000_000
    png_fixed_point r1 = png_reciprocal2(1000, 1000);
    EXPECT_EQ(r1, (png_fixed_point)1000000000);

    // Case: a*b = 5e5 -> r = 2e9, within int32 range
    // a = 1000, b = 500 -> r = floor(1e15/5e5 + 0.5) = 2_000_000_000
    png_fixed_point r2 = png_reciprocal2(1000, 500);
    EXPECT_EQ(r2, (png_fixed_point)2000000000);

    // Case: Negative product yields negative reciprocal-like result
    // a = -1000, b = 500 -> a*b = -5e5 -> r ~ -2e9
    png_fixed_point r3 = png_reciprocal2(-1000, 500);
    EXPECT_EQ(r3, (png_fixed_point)(-2000000000));

    // Case: Zero inputs should yield 0 (handled explicitly by function)
    png_fixed_point r_zero = png_reciprocal2(0, 12345);
    EXPECT_EQ(r_zero, (png_fixed_point)0);
}

// Coverage: false branch within the same path where one input is zero to ensure early exit.
// This also ensures no undefined behavior occurs for zero operands.
static void test_png_reciprocal2_floating_zero_inputs()
{
    png_fixed_point r = png_reciprocal2(0, 0);
    EXPECT_EQ(r, (png_fixed_point)0);

    r = png_reciprocal2(0, -12345);
    EXPECT_EQ(r, (png_fixed_point)0);
}

static void test_png_reciprocal2_floating_overflow_guard()
{
    // Case where r would overflow int32 range; expect return 0 (overflow guard)
    // a*b should be small enough to produce r > INT32_MAX, e.g., a*b = 466
    // However, with 1e15 constant, this yields r ~ 2.145e9 for 466000 which is still near range;
    // We'll pick a and b such that r becomes clearly > INT32_MAX, e.g., a*b = 1 gives 1e15.
    // This case is effectively covered by the a=b=1 test, which should return 0.
    png_fixed_point r = png_reciprocal2(1, 1);
    EXPECT_EQ(r, (png_fixed_point)0);
}

#else // PNG_FLOATING_ARITHMETIC_SUPPORTED is not defined
// Test suite for the alternative path where floating arithmetic is not supported.
// This branch uses png_product2(a,b) and png_reciprocal(res). We cannot directly
// verify the exact numeric value without access to png_product2 internals,
// but we can still verify behavior on boundary and non-boundary inputs.

// Coverage: zero inputs should yield 0 due to res == 0
static void test_png_reciprocal2_else_zero_inputs()
{
    // If either argument is zero, product is zero, recproc returns 0
    png_fixed_point r1 = png_reciprocal2(0, 12345);
    EXPECT_EQ(r1, (png_fixed_point)0);

    png_fixed_point r2 = png_reciprocal2(0, 0);
    EXPECT_EQ(r2, (png_fixed_point)0);
}

// Coverage: non-zero product should yield a non-zero reciprocal result.
// We assume that 1000*1000 yields a non-zero reciprocal via the else path.
static void test_png_reciprocal2_else_nonzero_product()
{
    png_fixed_point r = png_reciprocal2(1000, 1000);
    EXPECT_NE(r, (png_fixed_point)0);
}
#endif

// Entry point for evaluating the floating-branch tests
static void run_floating_branch_tests()
{
    test_png_reciprocal2_floating_true_values();
    test_png_reciprocal2_floating_zero_inputs();
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
    test_png_reciprocal2_floating_overflow_guard();
#endif
}

// Entry point for evaluating the non-floating-branch tests
#ifndef PNG_FLOATING_ARITHMETIC_SUPPORTED
static void run_else_branch_tests()
{
    test_png_reciprocal2_else_zero_inputs();
    test_png_reciprocal2_else_nonzero_product();
}
#endif

int main()
{
    // Run the appropriate test suites depending on compile-time macro
#ifdef PNG_FLOATING_ARITHMETIC_SUPPORTED
    run_floating_branch_tests();
#else
    run_else_branch_tests();
#endif

    // Summary
    std::cout << "Total tests run: " << g_total_tests << "\n";
    if (g_failed_tests > 0) {
        std::cerr << "Total failures: " << g_failed_tests << "\n";
        return 1;
    } else {
        std::cout << "All tests passed.\n";
        return 0;
    }
}