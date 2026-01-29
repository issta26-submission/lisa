/*
Unit test suite for png_fp_add (located in png.c)

Notes:
- This test uses the real libpng types/macros by including <png.h>.
- It is written for C++11 (no GoogleTest). A lightweight, non-terminating test
  harness is implemented using simple EXPECT-like macros that print failures
  but do not abort execution, to maximize code coverage.
- Test cases cover true/false branches and overflow behavior as per the focal method.
- The tests call the function png_fp_add directly (assumed to be provided by the
  project under test along with libpng headers).
*/

#include <cstdint>
#include <climits>
#include <iostream>
#include <pngpriv.h>
#include <png.h>


// Provide a minimal, non-terminating assertion framework
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define EXPECT_EQ(actual, expected, desc)                                         \
    do {                                                                            \
        ++g_tests_run;                                                              \
        if (static_cast<decltype(actual)>(actual) != static_cast<decltype(expected)>(expected)) { \
            std::cerr << "[FAIL] " << desc << " | Expected: " << (expected)         \
                      << ", Got: " << (actual) << std::endl;                     \
        } else {                                                                    \
            ++g_tests_passed;                                                       \
        }                                                                           \
    } while (0)

#define EXPECT_TRUE(cond, desc)                                                     \
    do {                                                                              \
        ++g_tests_run;                                                                \
        if (!(cond)) {                                                                \
            std::cerr << "[FAIL] " << desc << " | Condition failed." << std::endl;  \
        } else {                                                                      \
            ++g_tests_passed;                                                         \
        }                                                                             \
    } while (0)

// Ensure linkage with the focal function. The signature is assumed to match the
// libpng definition: png_int_32 png_fp_add(png_int_32 addend0, png_int_32 addend1, int *error)
extern "C" png_int_32 png_fp_add(png_int_32 addend0, png_int_32 addend1, int *error);

// Test 1: Normal addition within signed 32-bit range; error should remain 0
void test_png_fp_add_normal() {
    png_int_32 error = 0;
    png_int_32 result = png_fp_add(1, 2, &error);

    EXPECT_EQ(error, 0, "Normal add: error flag should remain 0");
    EXPECT_EQ(result, 3, "Normal add: 1 + 2 should equal 3");
}

// Test 2: Overflow when adding two positive values; error should be set and result should be PNG_FP_1/2
void test_png_fp_add_overflow_positive() {
    // Use INT32_MAX + 1 to force overflow
    png_int_32 error = 0;
    png_int_32 result = png_fp_add(INT32_MAX, 1, &error);

    EXPECT_EQ(error, 1, "Overflow positive: error flag should be set to 1");
    EXPECT_EQ(result, PNG_FP_1/2, "Overflow positive: result should be PNG_FP_1/2");
}

// Test 3: Underflow-like case for negative addend; should set error and return PNG_FP_1/2
void test_png_fp_add_overflow_negative() {
    // INT_MIN + (-1) would overflow negative direction; trigger overflow path
    png_int_32 error = 0;
    png_int_32 result = png_fp_add(INT32_MIN, -1, &error);

    EXPECT_EQ(error, 1, "Overflow negative: error flag should be set to 1");
    EXPECT_EQ(result, PNG_FP_1/2, "Overflow negative: result should be PNG_FP_1/2");
}

// Test 4: When addend0 is zero, function should return addend1 and not modify error
void test_png_fp_add_zero_lead() {
    int error_initial = 12345;
    png_int_32 result = png_fp_add(0, 123, &error_initial);

    // Error should remain unchanged since no overflow path is taken
    EXPECT_EQ(error_initial, 12345, "Zero addend: error should remain unchanged");
    EXPECT_EQ(result, 123, "Zero addend: result should equal addend1 (123)");
}

int main() {
    // Run tests
    test_png_fp_add_normal();
    test_png_fp_add_overflow_positive();
    test_png_fp_add_overflow_negative();
    test_png_fp_add_zero_lead();

    // Summary
    std::cout << "Test results: " << g_tests_passed << " / " << g_tests_run
              << " tests passed." << std::endl;

    // Return non-zero if any test failed
    if (g_tests_passed != g_tests_run) {
        return 1;
    }
    return 0;
}