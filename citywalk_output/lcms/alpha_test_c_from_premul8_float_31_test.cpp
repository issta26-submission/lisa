/*
Unit test suite for the focal method: from_premul8_float(uint8_t rgb8, uint8_t a8)

Step 1 (Program Understanding summary):
- Functionality: If alpha (a8) is zero, return rgb8 unchanged. Otherwise compute a premultiplied RGB value by scaling rgb8 by alpha_factor = 255.0 / a8, clamp the result to 255, and round to nearest integer.
- Core behavior (dependent components / Candidate Keywords):
  - rgb8, a8 inputs
  - Early return when a8 == 0
  - alpha_factor = 255.0 / a8
  - rgb = rgb8 * alpha_factor
  - clamp rgb to 255
  - rounding: uint8_t cast of std::round(rgb)
  - Linkage with C function (extern "C")
  - Boundary cases around 0 and 255 outputs

Step 2 (Unit Test Generation guidance):
- We focus on from_premul8_float and its behavior under a8 == 0 (early return) and a8 > 0 (computations).
- Cover boundary values and typical scenarios:
  - a8 == 0 with arbitrary rgb8
  - rgb8 values that yield exact, near-exact, and saturated results
  - various a8 values to exercise scaling and rounding
- We implement a lightweight, non-terminating test harness (no GTest) suitable for C++11.

Step 3 (Test Case Refinement guidance):
- Use explicit expected outcomes for a subset of representative inputs to ensure deterministic expectations.
- Provide explanatory comments for each test to document intent and edge coverage.
- Use a minimal assertion framework with non-terminating behavior (print failures but continue).

Notes:
- The focal function is defined in C (presumably in a C source linked into the test). We declare it with extern "C" for C++ linkage.
- The tests are designed to compile with a C++11 compiler and link against the C implementation without GTest.
- The test harness prints failures and a final summary; the process exit code reflects overall success.
*/

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Linkage to the C implementation
extern "C" uint8_t from_premul8_float(uint8_t rgb8, uint8_t a8);

/* Lightweight test harness:
   - Non-terminating on failure (prints message and continues)
   - Keeps a simple count of total and passed tests
*/
static int g_total = 0;
static int g_passed = 0;

// Utility macro for readable uint8_t printing as unsigned
#define U8(x) static_cast<unsigned>(x)

// Generic expectation comparison
#define EXPECT_EQ(_expected, _actual) do { \
    ++g_total; \
    if (static_cast<unsigned>(_actual) == static_cast<unsigned>(_expected)) { \
        ++g_passed; \
    } else { \
        std::cerr << "FAILED in " << __func__ << ": expected " << U8(_expected) \
                  << ", got " << U8(_actual) << std::endl; \
    } \
} while(0)

#define EXPECT_TRUE(_cond) do { \
    ++g_total; \
    if (_cond) { \
        ++g_passed; \
    } else { \
        std::cerr << "FAILED in " << __func__ << ": condition is false" << std::endl; \
    } \
} while(0)

/*
Candidate Keywords (for reference):
from_premul8_float, rgb8, a8, alpha_factor, premul, clamp, round, 0, 255, saturation
*/

/* Test Case A: a8 == 0 should return rgb8 unchanged (early exit path) */
static void test_case_a8_zero()
{
    // Reason: Ensure early return path is exercised
    uint8_t rgb8 = 123;
    uint8_t a8 = 0;
    uint8_t res = from_premul8_float(rgb8, a8);
    EXPECT_EQ(rgb8, res);
}

/* Test Case B: rgb8 = 16, a8 = 255 should yield exact factor of 1.0 and no change */
static void test_case_no_saturation_exact()
{
    uint8_t rgb8 = 16;
    uint8_t a8 = 255;
    uint8_t res = from_premul8_float(rgb8, a8);
    EXPECT_EQ(16, res);
}

/* Test Case C: rgb8 = 50, a8 = 128 -> expected 100 after rounding (approx 99.609...) */
static void test_case_rounding_near_half_down_up()
{
    uint8_t rgb8 = 50;
    uint8_t a8 = 128;
    uint8_t res = from_premul8_float(rgb8, a8);
    EXPECT_EQ(100, res);
}

/* Test Case D: rgb8 = 128, a8 = 128 -> factor = 255/128, product exactly 255 -> clamp to 255 */
static void test_case_maximum_clamp_at_half_exact()
{
    uint8_t rgb8 = 128;
    uint8_t a8 = 128;
    uint8_t res = from_premul8_float(rgb8, a8);
    EXPECT_EQ(255, res);
}

/* Test Case E: rgb8 = 200, a8 = 100 -> product exceeds 255, expect clamp to 255 */
static void test_case_saturation_due_to_scaling()
{
    uint8_t rgb8 = 200;
    uint8_t a8 = 100;
    uint8_t res = from_premul8_float(rgb8, a8);
    EXPECT_EQ(255, res);
}

/* Test Case F: rgb8 = 1, a8 = 3 -> factor ~85.0, expect 85 after rounding */
static void test_case_rounding_specific()
{
    uint8_t rgb8 = 1;
    uint8_t a8 = 3;
    uint8_t res = from_premul8_float(rgb8, a8);
    EXPECT_EQ(85, res);
}

/* Test Case G: rgb8 = 0, a8 = 255 -> result should be 0 */
static void test_case_zero_rgb_with_full_alpha()
{
    uint8_t rgb8 = 0;
    uint8_t a8 = 255;
    uint8_t res = from_premul8_float(rgb8, a8);
    EXPECT_EQ(0, res);
}

/* Test Case H: rgb8 = 255, a8 = 1 -> extreme alpha -> clamp to 255 */
static void test_case_max_rgb_with_min_alpha()
{
    uint8_t rgb8 = 255;
    uint8_t a8 = 1;
    uint8_t res = from_premul8_float(rgb8, a8);
    EXPECT_EQ(255, res);
}

/* Test Runner and Summary */
int main()
{
    // Run all test cases
    test_case_a8_zero();
    test_case_no_saturation_exact();
    test_case_rounding_near_half_down_up();
    test_case_maximum_clamp_at_half_exact();
    test_case_saturation_due_to_scaling();
    test_case_rounding_specific();
    test_case_zero_rgb_with_full_alpha();
    test_case_max_rgb_with_min_alpha();

    // Summary
    std::cout << "Unit tests finished. Total: " << g_total
              << ", Passed: " << g_passed
              << ", Failed: " << (g_total - g_passed) << std::endl;

    // Return non-zero if any test failed
    return (g_total == g_passed) ? 0 : 1;
}