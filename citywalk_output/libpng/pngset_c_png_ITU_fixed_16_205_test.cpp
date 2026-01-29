// Unit tests for the focal method: png_ITU_fixed_16
// This test suite exercises the behavior of png_ITU_fixed_16(int *error, png_fixed_point v)
// according to the ITU H273 scaling rules as implemented in the provided source.
// The tests are written in C++11, without any GTest usage, and use a lightweight
// in-process test harness that continues execution after assertion failures.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Include libpng internal types and declarations.
// The FOCAL_CLASS_DEP section indicates dependencies like png_fixed_point and
// png_uint_16 are defined via pngpriv.h (and related headers).

// Function under test (assumed to be linked with the test binary).
// Declared with C linkage to match the library's symbol.
extern "C" png_uint_16 png_ITU_fixed_16(int *error, png_fixed_point v);

// Lightweight test harness counters
static int g_total_tests = 0;
static int g_passed_tests = 0;
static int g_failed_tests = 0;

// Macros to perform non-terminating checks with clear reporting
#define TEST_CHECK_EQ(actual, expected, msg)                                      \
    do {                                                                            \
        ++g_total_tests;                                                            \
        if ((actual) == (expected)) {                                             \
            ++g_passed_tests;                                                      \
        } else {                                                                    \
            ++g_failed_tests;                                                      \
            std::cerr << "FAIL: " << (msg) << " | Expected: " << (expected)        \
                      << ", Actual: " << (actual) << std::endl;                   \
        }                                                                           \
    } while (0)

#define TEST_CHECK_TRUE(cond, msg)                                                   \
    do {                                                                               \
        ++g_total_tests;                                                               \
        if (cond) {                                                                    \
            ++g_passed_tests;                                                         \
        } else {                                                                       \
            ++g_failed_tests;                                                         \
            std::cerr << "FAIL: " << (msg) << std::endl;                             \
        }                                                                              \
    } while (0)

// Test Case 1: Within safe range after halving (0 <= v/2 <= 65535)
static void test_within_safe_range() {
    // Given a value that after halving stays within 0..65535
    int error = 0;
    png_fixed_point v = 100000; // 100000 / 2 => 50000
    png_uint_16 result = png_ITU_fixed_16(&error, v);

    TEST_CHECK_EQ(result, 50000u, "png_ITU_fixed_16 should halve 100000 to 50000");
    TEST_CHECK_EQ(error, 0, "png_ITU_fixed_16 should not set error for within-range input");
}

// Test Case 2: Overflow after halving should trigger error and return 0
static void test_overflow_after_halving() {
    int error = 0;
    png_fixed_point v = 131072; // 131072 / 2 => 65536, which is > 65535
    png_uint_16 result = png_ITU_fixed_16(&error, v);

    TEST_CHECK_EQ(result, 0u, "png_ITU_fixed_16 should return 0 on overflow after halving");
    TEST_CHECK_EQ(error, 1, "png_ITU_fixed_16 should set error on overflow");
}

// Test Case 3: Negative input causes halved value to be negative (trigger error)
static void test_negative_input_triggers_error() {
    int error = 0;
    png_fixed_point v = -2; // -2 / 2 => -1, which is < 0
    png_uint_16 result = png_ITU_fixed_16(&error, v);

    TEST_CHECK_EQ(result, 0u, "png_ITU_fixed_16 should return 0 when halved value is negative");
    TEST_CHECK_EQ(error, 1, "png_ITU_fixed_16 should set error when halved value is negative");
}

// Test Case 4: Negative input that halved to zero should not set error
static void test_negative_input_halved_to_zero_without_error() {
    int error = 0;
    png_fixed_point v = -1; // -1 / 2 => 0 (toward zero)
    png_uint_16 result = png_ITU_fixed_16(&error, v);

    TEST_CHECK_EQ(result, 0u, "png_ITU_fixed_16 should return 0 when halved value is zero");
    TEST_CHECK_EQ(error, 0, "png_ITU_fixed_16 should not set error when halved value is zero");
}

// Test Case 5: Zero input remains zero with no error
static void test_zero_input() {
    int error = 0;
    png_fixed_point v = 0; // 0 / 2 => 0
    png_uint_16 result = png_ITU_fixed_16(&error, v);

    TEST_CHECK_EQ(result, 0u, "png_ITU_fixed_16 should return 0 for input 0");
    TEST_CHECK_EQ(error, 0, "png_ITU_fixed_16 should not set error for input 0");
}

// Test Case 6: Boundary condition just at the limit after halving (131070 -> 65535)
static void test_boundary_after_halving() {
    int error = 0;
    png_fixed_point v = 131070; // 131070 / 2 => 65535
    png_uint_16 result = png_ITU_fixed_16(&error, v);

    TEST_CHECK_EQ(result, 65535u, "png_ITU_fixed_16 should yield 65535 for input 131070");
    TEST_CHECK_EQ(error, 0, "png_ITU_fixed_16 should not set error at boundary");
}

// Helper to summarize test results
static void summarize_results() {
    std::cout << "Test summary: Total=" << g_total_tests
              << ", Passed=" << g_passed_tests
              << ", Failed=" << g_failed_tests << std::endl;
}

// Entry point for tests
int main() {
    // Run all test cases
    test_within_safe_range();
    test_overflow_after_halving();
    test_negative_input_triggers_error();
    test_negative_input_halved_to_zero_without_error();
    test_zero_input();
    test_boundary_after_halving();

    // Print final summary
    summarize_results();

    // Return non-zero if any test failed to aid integration with build systems
    return (g_failed_tests == 0) ? 0 : 1;
}