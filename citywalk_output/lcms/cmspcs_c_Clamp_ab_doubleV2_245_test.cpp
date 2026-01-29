// Test suite for Clamp_ab_doubleV2 in cmspcs.c
// This test is designed to be compiled with a C++11 compiler without a testing framework (e.g., GTest).
// It uses a lightweight, non-terminating assertion mechanism and prints per-test results.

#include <limits>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cmath>


// Include internal CMS/LittleCMS headers to obtain required types and constants.
// The focal function and its related constants are defined in lcms2_internal.h (as per provided dependencies).

extern "C" cmsFloat64Number Clamp_ab_doubleV2(cmsFloat64Number ab); // Focal method under test (C linkage)

// Global failure counter to allow non-terminating test execution
static int g_failures = 0;

// Utility: exact-ish comparison for doubles with a reasonable tolerance
static bool almost_equal_double(double a, double b, double tol = 1e-9)
{
    return std::abs(a - b) <= tol;
}

// Lightweight EXPECT macro: records a failure but does not abort test execution
#define EXPECT_DOUBLE_EQ(expected, actual) do { \
    if (!almost_equal_double((double)(expected), (double)(actual))) { \
        ++g_failures; \
        std::cerr << "  [EXPECT_FAIL] Expected: " << (double)(expected) \
                  << "  Actual: " << (double)(actual) \
                  << "\n"; \
    } \
} while(0)

// Per-test wrappers to isolate failures and provide clear, labeled output

// Test 1: ab below MIN_ENCODEABLE_ab2 must be clamped to MIN_ENCODEABLE_ab2
static void test_below_min()
{
    // Arrange
    cmsFloat64Number minVal = MIN_ENCODEABLE_ab2;
    cmsFloat64Number input = minVal - 1.0; // clearly below min

    // Act
    cmsFloat64Number result = Clamp_ab_doubleV2(input);

    // Assert
    EXPECT_DOUBLE_EQ(minVal, result);
    // Comment: Confirms the lower bound enforcement of the clamp function.
}

// Test 2: ab exactly at MIN_ENCODEABLE_ab2 should remain unchanged
static void test_at_min()
{
    // Arrange
    cmsFloat64Number minVal = MIN_ENCODEABLE_ab2;

    // Act
    cmsFloat64Number result = Clamp_ab_doubleV2(minVal);

    // Assert
    EXPECT_DOUBLE_EQ(minVal, result);
    // Comment: Ensures boundary value is preserved when already at minimum.
}

// Test 3: ab strictly within the feasible range should be returned as-is
static void test_within_range()
{
    // Arrange
    cmsFloat64Number minVal = MIN_ENCODEABLE_ab2;
    cmsFloat64Number maxVal = MAX_ENCODEABLE_ab2;
    cmsFloat64Number input = (minVal + maxVal) / 2.0;

    // Act
    cmsFloat64Number result = Clamp_ab_doubleV2(input);

    // Assert
    EXPECT_DOUBLE_EQ(input, result);
    // Comment: Verifies mid-range value is not altered by the clamp.
}

// Test 4: ab exactly at MAX_ENCODEABLE_ab2 should remain unchanged
static void test_at_max()
{
    // Arrange
    cmsFloat64Number maxVal = MAX_ENCODEABLE_ab2;

    // Act
    cmsFloat64Number result = Clamp_ab_doubleV2(maxVal);

    // Assert
    EXPECT_DOUBLE_EQ(maxVal, result);
    // Comment: Ensures boundary value is preserved when at maximum.
}

// Test 5: ab above MAX_ENCODEABLE_ab2 must be clamped to MAX_ENCODEABLE_ab2
static void test_above_max()
{
    // Arrange
    cmsFloat64Number maxVal = MAX_ENCODEABLE_ab2;
    cmsFloat64Number input = maxVal + 2.0; // clearly above max

    // Act
    cmsFloat64Number result = Clamp_ab_doubleV2(input);

    // Assert
    EXPECT_DOUBLE_EQ(maxVal, result);
    // Comment: Confirms the upper bound enforcement of the clamp function.
}

// Simple test runner to report per-test status without terminating on failures
static int run_and_report(void (*test)(), const char* name)
{
    int before = g_failures;
    test();
    int delta = g_failures - before;
    if (delta == 0) {
        std::cout << "[PASSED] " << name << "\n";
    } else {
        std::cout << "[FAILED] " << name << " - " << delta << " failure(s)\n";
    }
    return delta;
}

// Main: execute all tests and summarize
int main()
{
    int totalFailures = 0;

    totalFailures += run_and_report(test_below_min, "Clamp_ab_doubleV2: below min");
    totalFailures += run_and_report(test_at_min, "Clamp_ab_doubleV2: at min");
    totalFailures += run_and_report(test_within_range, "Clamp_ab_doubleV2: within range");
    totalFailures += run_and_report(test_at_max, "Clamp_ab_doubleV2: at max");
    totalFailures += run_and_report(test_above_max, "Clamp_ab_doubleV2: above max");

    std::cout << "Total failures: " << totalFailures << "\n";

    // Return non-zero if any test failed to aid integration with build scripts
    return totalFailures;
}