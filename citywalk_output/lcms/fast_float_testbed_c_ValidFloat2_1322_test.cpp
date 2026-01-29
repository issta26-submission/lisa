// Test suite for ValidFloat2 in fast_float_testbed.c
// This test is written for C++11 compilation, without using Google Test.
// It directly calls the focal function and reports non-terminating results,
// allowing all tests to execute in a single run.
//
// Notes:
// - The focal function uses CMS type aliases (cmsBool, cmsFloat32Number).
// - We include fast_float_internal.h (as provided by the project) to obtain
//   the necessary type definitions. The function ValidFloat2 is assumed to have C linkage.
// - We use simple in-process assertions that print failures but do not abort
//   the test suite, to maximize coverage.
//
// Candidate Keywords (from Step 1): ValidFloat2, fabsf, 0.007 (tolerance),
// cmsBool, cmsFloat32Number, a, b (float inputs), boundary behavior, tolerance tests.

#include <cstdio>
#include <fast_float_internal.h>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// Include the project header that defines cmsBool and cmsFloat32Number.
// If the header uses C linkage for the public API, ensure compatibility here.

// Declare the focal function with C linkage to ensure correct symbol resolution.
// If the project already exposes ValidFloat2 via a header with proper linkage, this
// declaration should still match and link correctly.
extern "C" cmsBool ValidFloat2(cmsFloat32Number a, cmsFloat32Number b);

// Simple non-terminating assertion macros.
// They log failures but do not abort the test suite, enabling full coverage.
static int g_test_total = 0;
static int g_test_failed = 0;

#define ASSERT_TRUE(cond, msg) do { \
    ++g_test_total; \
    if(!(cond)) { \
        ++g_test_failed; \
        printf("Test FAILED at %s:%d: %s\n", __FILE__, __LINE__, (msg)); \
    } \
} while(0)

#define ASSERT_FALSE(cond, msg) do { \
    ++g_test_total; \
    if((cond)) { \
        ++g_test_failed; \
        printf("Test FAILED at %s:%d: %s\n", __FILE__, __LINE__, (msg)); \
    } \
} while(0)

#define ASSERT_EQUAL(expected, actual, msg) do { \
    ++g_test_total; \
    if(!((expected) == (actual))) { \
        ++g_test_failed; \
        printf("Test FAILED at %s:%d: %s | Expected: %g, Actual: %g\n", __FILE__, __LINE__, (msg), static_cast<double>(expected), static_cast<double>(actual)); \
    } \
} while(0)

// Run a comprehensive set of tests for ValidFloat2.
// Coverage goals:
// - True for equal numbers
// - True for numbers within tolerance (< 0.007)
// - False for numbers at or beyond the boundary (>= 0.007 difference)
// - False for clearly different values
// - Handles negative values correctly
static void test_ValidFloat2_basic_cases() {
    // Case 1: Equal values => should be true
    ASSERT_TRUE(ValidFloat2(1.234f, 1.234f), "Identical values must be considered valid.");

    // Case 2: Within tolerance (diff = 0.006999... < 0.007) => should be true
    ASSERT_TRUE(ValidFloat2(0.0f, 0.006999f), "Difference within tolerance must be valid.");

    // Case 3: Boundary condition (diff exactly 0.007) => should be false
    ASSERT_FALSE(ValidFloat2(0.0f, 0.007f), "Difference exactly at tolerance boundary must be invalid.");

    // Case 4: Outside tolerance (diff > 0.007) => should be false
    ASSERT_FALSE(ValidFloat2(0.0f, 0.01f), "Difference outside tolerance must be invalid.");

    // Case 5: Negative values within tolerance (diff = 0.005) => true
    ASSERT_TRUE(ValidFloat2(-1.000f, -1.005f), "Negative values within tolerance must be valid.");

    // Case 6: Negative values outside tolerance (diff > 0.007) => false
    ASSERT_FALSE(ValidFloat2(-10.0f, -9.992f), "Negative values with large diff must be invalid.");

    // Case 7: Near-zero and negative within tolerance
    ASSERT_TRUE(ValidFloat2(0.0f, -0.006999f), "Zero and negative value within tolerance must be valid.");

    // Case 8: Large numbers with small relative diff within tol (relative checks are absolute here)
    ASSERT_TRUE(ValidFloat2(1234.000f, 1234.005f), "Large magnitude values within tolerance must be valid.");

    // Case 9: Large numbers with diff at boundary
    ASSERT_FALSE(ValidFloat2(1234.000f, 1234.007f), "Large magnitude values at boundary must be invalid.");
}

// Additional helper tests to increase coverage of the implementation paths
static void test_ValidFloat2_boundary_and_sanity() {
    // Sanity check: extremely small numbers
    ASSERT_TRUE(ValidFloat2(0.000001f, 0.000001f), "Tiny identical values must be valid.");

    // Sanity check: one value zero, another within tolerance
    ASSERT_TRUE(ValidFloat2(0.0f, 0.006f), "Zero vs within-tolerance value should be valid.");

    // Sanity check: NaN handling (behavior depends on fabsf and comparison)
    // We avoid assuming NaN behavior beyond standard IEEE expectations.
    // If NaN is passed, diff becomes NaN; comparison with < 0.007 yields false.
    ASSERT_FALSE(ValidFloat2(0.0f, NAN), "NaN in one input should yield false under standard comparison.");

    // Sanity check: Inf handling
    ASSERT_FALSE(ValidFloat2(0.0f, INFINITY), "Infinity in one input should yield false under standard comparison.");
}

// Entry point for running tests.
// Per DOMAIN_KNOWLEDGE, call test methods from main and print final summary.
int main() {
    printf("Starting ValidFloat2 test suite (C++11, non-GTest)...\n");

    // Run basic coverage tests
    test_ValidFloat2_basic_cases();

    // Run additional boundary/sanity tests
    test_ValidFloat2_boundary_and_sanity();

    // Summary
    printf("ValidFloat2 test summary: Total = %d, Failures = %d\n", g_test_total, g_test_failed);

    // Exit code: 0 indicates success if no failures occurred.
    // Note: We purposely do not exit early on failures to maximize coverage.
    if (g_test_failed != 0) {
        // Non-zero exit code indicates that at least one assertion failed.
        return 1;
    }
    return 0;
}