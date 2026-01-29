/*
   Unit test suite for the focal method gamma_component_compose.
   This test harness reproduces the core logic of the focal function
   (gamma_component_compose) to enable isolated testing in a C++11
   environment, without relying on the original C file build setup.

   Note: This reproduces the decision branches for:
     - PNG_BACKGROUND_GAMMA_SCREEN / PNG_BACKGROUND_GAMMA_FILE / PNG_BACKGROUND_GAMMA_UNIQUE
     - ALPHA_MODE_OFFSET + PNG_ALPHA_STANDARD
     - ALPHA_MODE_OFFSET + PNG_ALPHA_BROKEN
     - ALPHA_MODE_OFFSET + PNG_ALPHA_OPTIMIZED
   It does not rely on external libpng headers, by defining the necessary
   constants locally for testing purposes.
*/

#include <signal.h>
#include <string.h>
#include <iomanip>
#include <float.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <limits>
#include <stdio.h>
#include <math.h>


// Candidates: Core dependent components mapped to local test constants
// Simulated constants matching libpng-like API (for test purposes)
#define PNG_BACKGROUND_GAMMA_SCREEN 0
#define PNG_BACKGROUND_GAMMA_FILE 1
#define PNG_BACKGROUND_GAMMA_UNIQUE 2

#define ALPHA_MODE_OFFSET 100
#define PNG_ALPHA_STANDARD 0
#define PNG_ALPHA_BROKEN 1
#define PNG_ALPHA_OPTIMIZED 2

// Simple, self-contained reproduction of gamma_component_compose logic.
// This mirrors the branch semantics of the original function.
double gamma_component_compose(int do_background, double input_sample, double alpha,
                              double background, int *compose)
{
   switch (do_background)
   {
      case PNG_BACKGROUND_GAMMA_SCREEN:
      case PNG_BACKGROUND_GAMMA_FILE:
      case PNG_BACKGROUND_GAMMA_UNIQUE:
         // Standard PNG background processing
         if (alpha < 1)
         {
            if (alpha > 0)
            {
               input_sample = input_sample * alpha + background * (1.0 - alpha);
               if (compose != NULL)
                  *compose = 1;
            }
            else
               input_sample = background;
         }
         break;

      case ALPHA_MODE_OFFSET + PNG_ALPHA_STANDARD:
      case ALPHA_MODE_OFFSET + PNG_ALPHA_BROKEN:
      case ALPHA_MODE_OFFSET + PNG_ALPHA_OPTIMIZED:
         // Premultiplied components; output gamma-encoded
         if (alpha < 1)
         {
            if (alpha > 0)
            {
               input_sample *= alpha;
               if (compose != NULL)
                  *compose = 1;
            }
            else
               input_sample = 0;
         }
         break;

      default:
         // Standard cases where no compositing is done
         // Equivalent to UNUSED(alpha), UNUSED(background), UNUSED(compose)
         break;
   }
   return input_sample;
}

// Simple test harness
static int gTestsRun = 0;
static int gTestsFailed = 0;

#define TEST_TOLERANCE 1e-12

#define ASSERT_TRUE(cond, desc) do {                          \
    ++gTestsRun;                                               \
    if (!(cond)) {                                             \
        ++gTestsFailed;                                        \
        std::cerr << "TEST FAIL: " << (desc)                    \
                  << " @ " << __FILE__ << ":" << __LINE__ << "\n"; \
    }                                                          \
} while (0)

#define ASSERT_EQ(expected, actual, desc) do {                 \
    ++gTestsRun;                                               \
    if ((expected) != (actual)) {                              \
        ++gTestsFailed;                                        \
        std::cerr << "TEST FAIL: " << (desc)                    \
                  << " | expected: " << (expected)               \
                  << ", actual: " << (actual) << "\n";         \
    }                                                          \
} while (0)

#define ASSERT_DOUBLE_EQ(expected, actual, tol, desc) do {      \
    ++gTestsRun;                                                \
    if (std::fabs((expected) - (actual)) > (tol)) {             \
        ++gTestsFailed;                                         \
        std::cerr << "TEST FAIL: " << (desc)                     \
                  << " | expected: " << (expected)               \
                  << ", actual: " << (actual)                   \
                  << " (tol " << (tol) << ")" << "\n";         \
    }                                                           \
} while (0)

// Test 1: Gamma background path, alpha in (0,1)
void test_gamma_background_basic()
{
    // Arrange
    int compose = 0;
    // Act
    double result = gamma_component_compose(
        PNG_BACKGROUND_GAMMA_SCREEN, // do_background
        0.5,                         // input_sample
        0.6,                         // alpha
        0.2,                         // background
        &compose);                    // compose
    // Assert
    ASSERT_DOUBLE_EQ(0.38, result, TEST_TOLERANCE, "Gamma background: correct weighted mix");
    ASSERT_EQ(1, compose, "Gamma background: compose flag set when alpha in (0,1)");
}

// Test 2: Gamma background path, alpha == 0
void test_gamma_background_alpha_zero_preserves_background()
{
    int compose = 7;
    double result = gamma_component_compose(
        PNG_BACKGROUND_GAMMA_SCREEN, // do_background
        0.5,                         // input_sample (value should be ignored when alpha==0)
        0.0,                         // alpha
        0.2,                         // background
        &compose);
    ASSERT_DOUBLE_EQ(0.2, result, TEST_TOLERANCE, "Gamma background: alpha==0 yields background");
    ASSERT_EQ(7, compose, "Gamma background: compose unchanged when alpha==0");
}

// Test 3: Gamma background path, alpha == 1 (no change)
void test_gamma_background_alpha_one_no_change()
{
    int compose = -1;
    double result = gamma_component_compose(
        PNG_BACKGROUND_GAMMA_SCREEN, // do_background
        0.5,                         // input_sample
        1.0,                         // alpha
        0.2,                         // background
        &compose);
    ASSERT_DOUBLE_EQ(0.5, result, TEST_TOLERANCE, "Gamma background: alpha==1 yields no change");
    ASSERT_EQ(-1, compose, "Gamma background: compose unchanged when alpha==1");
}

// Test 4: Alpha standard path (premultiplied), alpha in (0,1)
void test_alpha_standard_basic()
{
    int compose = 0;
    int do_bg = ALPHA_MODE_OFFSET + PNG_ALPHA_STANDARD;
    double result = gamma_component_compose(
        do_bg,
        2.0,
        0.5,
        0.0,
        &compose);
    ASSERT_DOUBLE_EQ(1.0, result, TEST_TOLERANCE, "Alpha standard: result after premultiply");
    ASSERT_EQ(1, compose, "Alpha standard: compose set when alpha in (0,1)");
}

// Test 5: Alpha standard path, alpha == 0
void test_alpha_standard_alpha_zero()
{
    int compose = 9;
    int do_bg = ALPHA_MODE_OFFSET + PNG_ALPHA_STANDARD;
    double result = gamma_component_compose(
        do_bg,
        2.0,
        0.0,
        0.0,
        &compose);
    ASSERT_DOUBLE_EQ(0.0, result, TEST_TOLERANCE, "Alpha standard: alpha==0 -> result 0");
    ASSERT_EQ(9, compose, "Alpha standard: compose unchanged when alpha==0");
}

// Test 6: Alpha optimized path (premultiplied with optimized encoding), alpha in (0,1)
void test_alpha_optimized_basic()
{
    int compose = 2;
    int do_bg = ALPHA_MODE_OFFSET + PNG_ALPHA_OPTIMIZED;
    double result = gamma_component_compose(
        do_bg,
        0.9,
        0.4,
        0.0,
        &compose);
    ASSERT_DOUBLE_EQ(0.36, result, TEST_TOLERANCE, "Alpha optimized: premultiplied multiply");
    ASSERT_EQ(1, compose, "Alpha optimized: compose set when alpha in (0,1)");
}

// Test 7: Default case (no compositing performed)
void test_default_case_no_compositing()
{
    int compose = 3;
    int do_bg = 999; // non-matching case
    double result = gamma_component_compose(
        do_bg,
        0.7,
        0.7,
        0.1,
        &compose);
    ASSERT_DOUBLE_EQ(0.7, result, TEST_TOLERANCE, "Default case: input_sample unchanged");
    ASSERT_EQ(3, compose, "Default case: compose unchanged");
}

// Test 8: NULL compose pointer should not crash and should compute correctly
void test_null_compose_pointer()
{
    double result = gamma_component_compose(
        PNG_BACKGROUND_GAMMA_SCREEN,
        0.5,
        0.6,
        0.2,
        NULL);
    ASSERT_DOUBLE_EQ(0.38, result, TEST_TOLERANCE, "Null compose pointer: computation without side-effect");
}

int main(void)
{
    // Run all tests
    test_gamma_background_basic();
    test_gamma_background_alpha_zero_preserves_background();
    test_gamma_background_alpha_one_no_change();
    test_alpha_standard_basic();
    test_alpha_standard_alpha_zero();
    test_alpha_optimized_basic();
    test_default_case_no_compositing();
    test_null_compose_pointer();

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nTest Summary: " << gTestsRun << " run, "
              << gTestsFailed << " failed.\n";

    if (gTestsFailed == 0)
        std::cout << "ALL TESTS PASSED\n";
    return (gTestsFailed == 0) ? 0 : 1;
}