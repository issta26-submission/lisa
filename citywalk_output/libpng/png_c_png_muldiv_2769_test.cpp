// Small C++11 test suite for the C function png_muldiv used in png.c
// This test avoids GTest and uses a lightweight non-terminating assertion style.
// It exercises several code paths of png_muldiv (divisor == 0, a == 0, times == 0,
// simple positive result, negative inputs, negative divisor, and a large integer case).

#include <cstdint>
#include <pngpriv.h>
#include <cstdio>


// Forward declare the types used by the focal function and its signature.
// We mirror the libpng style to enable linking with the C implementation in png.c.
typedef int32_t png_fixed_point;
typedef png_fixed_point* png_fixed_point_p;
typedef int32_t png_int_32;

// Declare the focal function from png.c (C linkage)
extern "C" int png_muldiv(png_fixed_point_p res, png_fixed_point a,
                          png_int_32 times, png_int_32 divisor);

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ_INT(actual, expected, testName) do {                 \
    ++g_total_tests;                                                     \
    if (static_cast<int64_t>(actual) != static_cast<int64_t>(expected)) { \
        fprintf(stderr, "TEST FAILED [%s]: expected %lld, got %lld (actual types may vary)\n", \
                testName, (long long)(expected), (long long)(actual)); \
        ++g_failed_tests;                                                \
    }                                                                     \
} while(0)

#define EXPECT_TRUE(cond, testName) do {                                \
    ++g_total_tests;                                                     \
    if (!(cond)) {                                                       \
        fprintf(stderr, "TEST FAILED [%s]: condition is false\n", testName); \
        ++g_failed_tests;                                                \
    }                                                                    \
} while(0)

// Test 1: Divisor is zero -> function should return 0 (failure to compute)
// This tests the early exit path when divisor == 0.
static void test_divisor_zero() {
    png_fixed_point res;
    int ret = png_muldiv(&res, 123, 10, 0);
    EXPECT_EQ_INT(ret, 0, "png_muldiv_divisor_zero_returns_0");
}

// Test 2: a is zero -> should set res to 0 and return 1
// This covers the branch a == 0 with divisor != 0.
static void test_a_zero() {
    png_fixed_point res;
    int ret = png_muldiv(&res, 0, 999, 5);
    EXPECT_EQ_INT(ret, 1, "png_muldiv_a_zero_returns_1");
    EXPECT_EQ_INT(res, 0, "png_muldiv_a_zero_res_zero");
}

// Test 3: times is zero -> should set res to 0 and return 1
// This covers the branch times == 0 with divisor != 0.
static void test_times_zero() {
    png_fixed_point res;
    int ret = png_muldiv(&res, 123, 0, 5);
    EXPECT_EQ_INT(ret, 1, "png_muldiv_times_zero_returns_1");
    EXPECT_EQ_INT(res, 0, "png_muldiv_times_zero_res_zero");
}

// Test 4: Simple positive integer result (a * times) / divisor is an integer
// This ensures correct computation and rounding when the result is an exact integer.
static void test_positive_integer_result() {
    png_fixed_point res;
    int ret = png_muldiv(&res, 2, 3, 3); // (2*3)/3 = 2
    EXPECT_EQ_INT(ret, 1, "png_muldiv_positive_integer_result_return_1");
    EXPECT_EQ_INT(res, 2, "png_muldiv_positive_integer_result_value_2");
}

// Test 5: Negative a with positive divisor and integer result
// This checks sign handling when the final result should be negative and integral.
static void test_negative_a_integer_result() {
    png_fixed_point res;
    int ret = png_muldiv(&res, -2, 3, 3); // (-2*3)/3 = -2
    EXPECT_EQ_INT(ret, 1, "png_muldiv_negative_a_integer_result_return_1");
    EXPECT_EQ_INT(res, -2, "png_muldiv_negative_a_integer_result_value_-2");
}

// Test 6: Positive a with negative divisor and integer result
// This checks sign handling when divisor is negative and the result is integral.
static void test_divisor_negative_integer_result() {
    png_fixed_point res;
    int ret = png_muldiv(&res, 2, 3, -3); // (2*3)/(-3) = -2
    EXPECT_EQ_INT(ret, 1, "png_muldiv_divisor_negative_integer_result_return_1");
    EXPECT_EQ_INT(res, -2, "png_muldiv_divisor_negative_integer_result_value_-2");
}

// Test 7: Large integer case within 32-bit bounds
// This ensures the function can handle near-32-bit max products without overflow.
// Use divisor = 1 so the result equals the product.
static void test_large_integer_case() {
    png_fixed_point res;
    // a = 46340, times = 46340, divisor = 1 -> product = 46340*46340 = 2147395600
    int ret = png_muldiv(&res, 46340, 46340, 1);
    EXPECT_EQ_INT(ret, 1, "png_muldiv_large_integer_case_return_1");
    EXPECT_EQ_INT(res, 2147395600, "png_muldiv_large_integer_case_value_2147395600");
}

// Entry point
int main() {
    fprintf(stdout, "Starting png_muldiv test suite...\n");

    test_divisor_zero();
    test_a_zero();
    test_times_zero();
    test_positive_integer_result();
    test_negative_a_integer_result();
    test_divisor_negative_integer_result();
    test_large_integer_case();

    fprintf(stdout, "Tests run: %d, Failures: %d\n", g_total_tests, g_failed_tests);
    if (g_failed_tests > 0) {
        fprintf(stderr, "Some tests failed. Review the log above for details.\n");
        return 1;
    }
    fprintf(stdout, "All tests passed.\n");
    return 0;
}