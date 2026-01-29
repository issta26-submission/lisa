// Test suite for the focal method LinearInterpInt in fast_float_cmyk.c
// This test suite is built to run with a C++11 compiler without GTest.
// It uses a tiny, non-terminating assertion framework to maximize code coverage.

#include <fast_float_internal.h>
#include <limits>
#include <iostream>
#include <string>
#include <cmath>


// Include project headers to obtain type definitions (e.g., cmsFloat32Number)
// and the prototype/availability of LinearInterpInt.
// The exact path may differ in your environment; adjust as needed.

extern "C" cmsFloat32Number LinearInterpInt(cmsFloat32Number a, cmsFloat32Number l, cmsFloat32Number h);

// Simple non-terminating test framework
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating assertion for approximate equality (floats)
#define EXPECT_NEAR(expected, actual, tol) do { \
    ++g_tests_run; \
    double _exp = static_cast<double>(expected); \
    double _act = static_cast<double>(actual); \
    double _tol = static_cast<double>(tol); \
    if (std::fabs(_exp - _act) > _tol) { \
        std::cout << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << "  Expected near: " << _exp \
                  << ", Actual: " << _act \
                  << ", Tol: " << _tol << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)

// Non-terminating assertion for boolean conditions
#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cout << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << "  Condition failed: " << (msg) << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)

// POD-style helpers (for readability in tests)
static inline cmsFloat32Number f(float v) { return static_cast<cmsFloat32Number>(v); }

// Step 1 Keywords (Candidate Keywords) derived from the focal method
// - LinearInterpInt: performs linear interpolation between l and h using factor a
// - Formula: (h - l) * a + l
// - cmsFloat32Number: 32-bit float scalar type
// - l, h: lower/upper bounds
// - a: interpolation parameter in [0,1] (not enforced here)
// - Floating-point edge cases: NaN handling is testable via std::isnan
// - Dependency: This function is declared in fast_float_internal.h and/or defined in the focal file
// - Domain: uses standard C++ math operations; no external state is touched

// Test 1: Basic true-paths for the core interpolation formula
// Verify a = 0 -> result is l, a = 1 -> result is h, a = 0.5 -> midpoint
static void test_linearInterpInt_basic() {
    // Expose the standard interpolant using a simple setup
    cmsFloat32Number l = f(2.0f);
    cmsFloat32Number h = f(8.0f);

    cmsFloat32Number a0 = f(0.0f);
    cmsFloat32Number a1 = f(1.0f);
    cmsFloat32Number a05 = f(0.5f);

    cmsFloat32Number r0 = LinearInterpInt(a0, l, h);
    cmsFloat32Number r1 = LinearInterpInt(a1, l, h);
    cmsFloat32Number r05 = LinearInterpInt(a05, l, h);

    // Expected results
    EXPECT_NEAR(l, r0, 1e-6f);       // a = 0 -> l
    EXPECT_NEAR(h, r1, 1e-6f);       // a = 1 -> h
    EXPECT_NEAR(static_cast<double>(5.0f), static_cast<double>(r05), 1e-6f); // a = 0.5 -> (l+h)/2 = 5
}

// Test 2: Out-of-range interpolation factors (a outside [0,1])
// Ensure the function computes using the same linear formula without clamping
static void test_linearInterpInt_out_of_range() {
    cmsFloat32Number l = f(-3.0f);
    cmsFloat32Number h = f(4.0f);

    cmsFloat32Number a_neg = f(-0.5f);
    cmsFloat32Number a_pos = f(2.0f);

    cmsFloat32Number r_neg = LinearInterpInt(a_neg, l, h);
    cmsFloat32Number r_pos = LinearInterpInt(a_pos, l, h);

    // Explicitly compute expected values using the same formula
    cmsFloat32Number expect_neg = (h - l) * a_neg + l;
    cmsFloat32Number expect_pos = (h - l) * a_pos + l;

    EXPECT_NEAR(static_cast<double>(expect_neg),
                static_cast<double>(r_neg), 1e-6f);
    EXPECT_NEAR(static_cast<double>(expect_pos),
                static_cast<double>(r_pos), 1e-6f);
}

// Test 3: NaN propagation behavior
// If a is NaN, the result of the linear expression should also be NaN
static void test_linearInterpInt_nan_input() {
    cmsFloat32Number l = f(1.0f);
    cmsFloat32Number h = f(2.0f);
    cmsFloat32Number nan = std::numeric_limits<cmsFloat32Number>::quiet_NaN();

    cmsFloat32Number r = LinearInterpInt(nan, l, h);
    // Expect NaN propagation
    EXPECT_TRUE(std::isnan(static_cast<double>(r)),
                "LinearInterpInt with NaN a should produce NaN");
}

// Test 4: Precision edge case with known fractional input
// Verify that small fractions produce expected precise results
static void test_linearInterpInt_precision() {
    cmsFloat32Number l = f(0.0f);
    cmsFloat32Number h = f(1.0f);
    cmsFloat32Number a = f(0.3333333f);

    cmsFloat32Number r = LinearInterpInt(a, l, h);

    // Expected close to 0.3333333
    EXPECT_NEAR(static_cast<double>(0.3333333f),
                static_cast<double>(r), 1e-6f);
}

// Main runner: execute all tests and report summary
int main() {
    std::cout << "Starting test suite for LinearInterpInt...\n";

    test_linearInterpInt_basic();
    test_linearInterpInt_out_of_range();
    test_linearInterpInt_nan_input();
    test_linearInterpInt_precision();

    if (g_tests_failed == 0) {
        std::cout << "[PASS] All tests succeeded. Total tests run: " << g_tests_run << std::endl;
        return 0;
    } else {
        std::cout << "[FAIL] Tests failed: " << g_tests_failed
                  << " / " << g_tests_run << ". See above for details." << std::endl;
        return 1;
    }
}