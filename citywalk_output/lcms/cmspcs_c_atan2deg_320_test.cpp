// Comprehensive unit tests for atan2deg in cmspcs.c
// This test suite uses a lightweight in-file testing framework (no Google Test).
// It covers true/false branches, boundary conditions, and various input quadrants.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>


// Assume the focal function has C linkage. If cmsFloat64Number is typedef double,
// the following declaration matches the expected signature in cmspcs.c.
extern "C" double atan2deg(double a, double b);

// Simple lightweight test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

static const double TEST_EPS = 1e-12;
static const double PI = 3.141592653589793238462643383279502884;

// Log a failing test with a descriptive message
static void log_fail(const char* desc, double actual, double expected) {
    std::cerr << "[TEST_FAIL] " << desc
              << " | Actual: " << actual
              << " | Expected: " << expected << std::endl;
}

// Expectation: actual should be approximately equal to expected within tol
#define EXPECT_NEAR(actual, expected, tol, desc) do { \
    ++g_tests_run; \
    double a = static_cast<double>(actual); \
    double e = static_cast<double>(expected); \
    double diff = std::fabs(a - e); \
    if (diff > (tol)) { \
        ++g_tests_failed; \
        log_fail(desc, a, e); \
    } \
} while (0)

// Expectation: condition must be true; otherwise log a failure
#define EXPECT_TRUE(cond, desc) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[TEST_FAIL] " << desc << std::endl; \
    } \
} while (0)

// Helper: compute expected value using standard atan2 and normalization rules.
// This mirrors the logic inside cmsFloat64Number atan2deg from the focal code.
static double expected_atan2deg(double a, double b) {
    if (a == 0.0 && b == 0.0) {
        return 0.0;
    } else {
        double rad = std::atan2(a, b);
        double h = rad * 180.0 / PI;
        // Normalize into [0, 360)
        while (h > 360.0) h -= 360.0;
        while (h < 0.0)   h += 360.0;
        return h;
    }
}

// Test 1: a = 0, b = 0 -> special-case branch should yield 0
static void test_case_zero_zero() {
    // Focal: if (a == 0 && b == 0) h = 0;
    double a = 0.0;
    double b = 0.0;
    double actual = atan2deg(a, b);
    double expected = expected_atan2deg(a, b);
    EXPECT_NEAR(actual, expected, TEST_EPS, "atan2deg(0,0) should be 0 (special-case branch)");
}

// Test 2: a = 0, b > 0 -> result should be 0 degrees
static void test_case_zero_positive() {
    double a = 0.0;
    double b = 5.0;
    double actual = atan2deg(a, b);
    double expected = expected_atan2deg(a, b);
    EXPECT_NEAR(actual, expected, TEST_EPS, "atan2deg(0, positive) should be 0 degrees");
}

// Test 3: a = 0, b < 0 -> result should be 180 degrees
static void test_case_zero_negative() {
    double a = 0.0;
    double b = -5.0;
    double actual = atan2deg(a, b);
    double expected = expected_atan2deg(a, b);
    EXPECT_NEAR(actual, expected, TEST_EPS, "atan2deg(0, negative) should be 180 degrees");
}

// Test 4: a > 0, b = 0 -> 90 degrees
static void test_case_positive_y_axis() {
    double a = 5.0;
    double b = 0.0;
    double actual = atan2deg(a, b);
    double expected = expected_atan2deg(a, b);
    EXPECT_NEAR(actual, expected, TEST_EPS, "atan2deg(positive, 0) should be 90 degrees");
}

// Test 5: a < 0, b = 0 -> 270 degrees
static void test_case_negative_y_axis() {
    double a = -5.0;
    double b = 0.0;
    double actual = atan2deg(a, b);
    double expected = expected_atan2deg(a, b);
    EXPECT_NEAR(actual, expected, TEST_EPS, "atan2deg(negative, 0) should be 270 degrees");
}

// Test 6: Quadrant I: a > 0, b > 0 -> 45 degrees for a=b
static void test_case_quadrant_I() {
    double a = 2.0;
    double b = 2.0;
    double actual = atan2deg(a, b);
    double expected = expected_atan2deg(a, b);
    EXPECT_NEAR(actual, expected, TEST_EPS, "atan2deg(2,2) should be 45 degrees");
}

// Test 7: Quadrant II: a > 0, b < 0 -> 135 degrees
static void test_case_quadrant_II() {
    double a = 2.0;
    double b = -2.0;
    double actual = atan2deg(a, b);
    double expected = expected_atan2deg(a, b);
    EXPECT_NEAR(actual, expected, TEST_EPS, "atan2deg(2,-2) should be 135 degrees");
}

// Test 8: Quadrant III: a < 0, b > 0 -> 315 degrees
static void test_case_quadrant_III() {
    double a = -2.0;
    double b = 2.0;
    double actual = atan2deg(a, b);
    double expected = expected_atan2deg(a, b);
    EXPECT_NEAR(actual, expected, TEST_EPS, "atan2deg(-2,2) should be 315 degrees");
}

// Test 9: General case in Quadrant I with small angle
static void test_case_quadrant_I_small_angle() {
    double a = 1.0;
    double b = 10.0;
    double actual = atan2deg(a, b);
    double expected = expected_atan2deg(a, b);
    EXPECT_NEAR(actual, expected, TEST_EPS, "atan2deg(1,10) should be small positive angle (~5.7106 deg)");
}

// Test 10: General case in Quadrant IV
static void test_case_quadrant_IV() {
    double a = -1.0;
    double b = 10.0;
    double actual = atan2deg(a, b);
    double expected = expected_atan2deg(a, b);
    EXPECT_NEAR(actual, expected, TEST_EPS, "atan2deg(-1,10) should be small negative angle wrapped to ~354.289 deg");
}

// Run all tests
static void run_all_tests() {
    test_case_zero_zero();
    test_case_zero_positive();
    test_case_zero_negative();
    test_case_positive_y_axis();
    test_case_negative_y_axis();
    test_case_quadrant_I();
    test_case_quadrant_II();
    test_case_quadrant_III();
    test_case_quadrant_I_small_angle();
    test_case_quadrant_IV();
}

// Entry point
int main() {
    run_all_tests();

    std::cout << "atan2deg test summary: "
              << "Tests run = " << g_tests_run
              << ", Failures = " << g_tests_failed << std::endl;

    // Return non-zero if any test failed to aid automation
    return g_tests_failed ? 1 : 0;
}