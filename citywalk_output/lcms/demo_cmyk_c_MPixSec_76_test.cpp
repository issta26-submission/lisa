#include <lcms2_fast_float.h>
#include <limits>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>


// Forward declaration of the focal C function under test.
// We assume cmsFloat64Number is equivalent to double in the provided source.
typedef double cmsFloat64Number;
extern "C" cmsFloat64Number MPixSec(cmsFloat64Number diff);

/*
Unit Test Suite for MPixSec
- Purpose: Validate correctness and behavior of MPixSec(diff) as defined in the focal code.
- Strategy:
  - Use a lightweight, self-contained test harness (no GTest).
  - Implement non-terminating assertions: tests record failures but continue.
  - Test positive diffs to ensure deterministic, monotonic behavior with respect to diff.
  - Avoid dividing by zero by not using diff == 0.
  - Check for determinism with identical inputs.
  - Verify finite results for valid inputs.
- Notes:
  - SIZE_X and SIZE_Y are internal to the focal implementation; tests verify behavioral properties independent of exact pixel counts.
  - This test suite relies on linking with the provided demo_cmyk.c (or equivalent) that contains MPixSec.
*/

// Simple test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion macro: reports failure but does not abort tests
#define TEST_EXPECT(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[TEST FAILED] " << msg \
                  << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")" << std::endl; \
    } \
} while(0)

// Helper: approximate equality for doubles (with relative tolerance)
static bool approx_equal(double a, double b, double rel = 1e-9) {
    double diff = std::fabs(a - b);
    double tol = rel * std::fmax(1.0, std::fabs(a));
    return diff <= tol;
}

/*
Test 1: Monotonicity with respect to diff
- MPixSec(diff) should decrease as diff increases, since
  MPixSec(diff) = (SIZE_X * SIZE_Y) / (1024^2 * (diff / CLOCKS_PER_SEC)).
- This tests that a smaller diff yields a larger megapixel-per-second value.
*/
static void test_monotonicity_with_diff() {
    // Use two distinct positive diffs
    cmsFloat64Number diff_small = (cmsFloat64Number)CLOCKS_PER_SEC * 0.5; // 0.5 seconds
    cmsFloat64Number diff_large = (cmsFloat64Number)CLOCKS_PER_SEC * 2.0;   // 2.0 seconds

    cmsFloat64Number val_small = MPixSec(diff_small);
    cmsFloat64Number val_large = MPixSec(diff_large);

    // Expectation: val_small > val_large
    TEST_EXPECT(val_small > val_large, "MPixSec should decrease as diff increases (monotonic relation).");
    // Additional sanity: ensure both values are finite
    TEST_EXPECT(std::isfinite(val_small) && std::isfinite(val_large),
                "MPixSec results should be finite for positive diffs.");
}

/*
Test 2: Determinism for identical inputs
- Calling MPixSec with the same diff twice should yield exactly the same result.
- This validates there is no internal randomness or state mutation.
*/
static void test_deterministic_on_same_input() {
    cmsFloat64Number diff = (cmsFloat64Number)CLOCKS_PER_SEC * 1.23; // arbitrary non-integer diff
    double val1 = MPixSec(diff);
    double val2 = MPixSec(diff);

    TEST_EXPECT(val1 == val2, "MPixSec should return identical results for identical inputs (deterministic).");
    // Also verify finite values
    TEST_EXPECT(std::isfinite(val1), "MPixSec should return a finite value for a valid input.");
}

/*
Test 3: Behavior with very small and moderate diff
- Compare a very small diff against a larger diff to validate the expected ordering.
*/
static void test_small_vs_moderate_diff_behavior() {
    double val_small = MPixSec(1.0); // diff = 1 clock tick
    double val_moderate = MPixSec((cmsFloat64Number)CLOCKS_PER_SEC); // diff = 1 second

    // Expectation: smaller diff yields larger MPixSec
    TEST_EXPECT(val_small > val_moderate, "Smaller diff should yield larger MPixSec value.");
    TEST_EXPECT(std::isfinite(val_small) && std::isfinite(val_moderate),
                "MPixSec results should be finite for small and moderate diffs.");
}

/*
Test 4: Extreme large diff
- Large diff should produce a significantly smaller MPixSec value.
*/
static void test_extreme_large_diff() {
    cmsFloat64Number diff_normal = (cmsFloat64Number)CLOCKS_PER_SEC;
    cmsFloat64Number diff_extreme = (cmsFloat64Number)CLOCKS_PER_SEC * 1000.0; // very large

    double val_normal = MPixSec(diff_normal);
    double val_extreme = MPixSec(diff_extreme);

    TEST_EXPECT(val_extreme < val_normal, "MPixSec should decrease with very large diff.");
    TEST_EXPECT(std::isfinite(val_extreme) && std::isfinite(val_normal),
                "MPixSec results should be finite for extreme diff values.");
}

/*
Test 5: Non-zero diff yields finite, non-NaN result
- Basic sanity check to ensure we don't produce NaN or Inf.
*/
static void test_non_zero_diff_finite() {
    double val = MPixSec(1.0);
    TEST_EXPECT(!std::isnan(val) && std::isfinite(val),
                "MPixSec should return a finite non-NaN value for diff = 1.0.");
}

int main(void) {
    // Run all tests
    test_monotonicity_with_diff();
    test_deterministic_on_same_input();
    test_small_vs_moderate_diff_behavior();
    test_extreme_large_diff();
    test_non_zero_diff_finite();

    // Summary
    std::cout << "Total tests executed: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests > 0) ? 1 : 0;
}