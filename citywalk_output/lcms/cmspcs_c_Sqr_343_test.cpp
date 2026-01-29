// Test suite for the focal method: Sqr(cmsFloat64Number v)
// Step 1 (Conceptual): Candidate Keywords representing the core dependencies
// - Sqr: the function under test (cmsFloat64Number Sqr(cmsFloat64Number v))
// - cmsFloat64Number: floating-point type used by the function
// - v: input parameter to Sqr
// - operator*: multiplication (v * v) to compute the square
// - return: the squared value
// Step 2 (Unit Test Generation): Coverage of true/false branches is trivial for Sqr,
// but we still exercise a representative set of inputs including edge cases (0, ±1, ±3.5),
// as well as special floating-point values (NaN, Inf) to ensure correct handling.
// Step 3 (Test Case Refinement): Uses a lightweight, non-terminating custom test harness
// suitable for C++11 without external test frameworks.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <limits>


// Include CMS types from the cms library (needed for cmsFloat64Number)

// Declare C function Sqr from the C file CMSpcs.c
extern "C" cmsFloat64Number Sqr(cmsFloat64Number v);

// Simple non-terminating test harness
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Report overall results
static void report_results() {
    std::cout << "Test results: " << tests_passed << " passed, "
              << tests_failed << " failed, "
              << tests_run << " total." << std::endl;
}

// Helper to log a failed test with context
static void log_failure(const char* test_name,
                        cmsFloat64Number input,
                        cmsFloat64Number expected,
                        cmsFloat64Number actual) {
    std::cerr << "[FAILED] " << test_name
              << " | input=" << static_cast<double>(input)
              << " | expected=" << static_cast<double>(expected)
              << " | actual=" << static_cast<double>(actual)
              << std::endl;
}

// Test 1: Basic values including zero, positive and negative integers, and a fractional value
// This exercises the basic numeric path: result = v * v
static void test_Sqr_basic_values() {
    // Candidate test cases for exact equality (choosing values with exact representation)
    struct Case { cmsFloat64Number input; cmsFloat64Number expected; const char* name; };
    Case cases[] = {
        { static_cast<cmsFloat64Number>(0.0), static_cast<cmsFloat64Number>(0.0), "Sqr(0.0)" },
        { static_cast<cmsFloat64Number>(1.0), static_cast<cmsFloat64Number>(1.0), "Sqr(1.0)" },
        { static_cast<cmsFloat64Number>(-1.0), static_cast<cmsFloat64Number>(1.0), "Sqr(-1.0)" },
        { static_cast<cmsFloat64Number>(2.0), static_cast<cmsFloat64Number>(4.0), "Sqr(2.0)" },
        { static_cast<cmsFloat64Number>(-2.0), static_cast<cmsFloat64Number>(4.0), "Sqr(-2.0)" },
        { static_cast<cmsFloat64Number>(3.5), static_cast<cmsFloat64Number>(12.25), "Sqr(3.5)" },
        { static_cast<cmsFloat64Number>(-3.5), static_cast<cmsFloat64Number>(12.25), "Sqr(-3.5)" }
    };

    const int n = sizeof(cases) / sizeof(cases[0]);
    for (int i = 0; i < n; ++i) {
        ++tests_run;
        cmsFloat64Number input = cases[i].input;
        cmsFloat64Number expected = cases[i].expected;
        cmsFloat64Number actual = Sqr(input);
        if (actual == expected) {
            ++tests_passed;
        } else {
            ++tests_failed;
            log_failure(cases[i].name, input, expected, actual);
        }
    }
}

// Test 2: NaN handling
// Square of NaN should be NaN according to IEEE rules; use isnan to verify.
static void test_Sqr_NaN() {
    ++tests_run;
    cmsFloat64Number nan = std::numeric_limits<cmsFloat64Number>::quiet_NaN();
    cmsFloat64Number actual = Sqr(nan);
    if (std::isnan(static_cast<double>(actual))) {
        ++tests_passed;
    } else {
        ++tests_failed;
        log_failure("Sqr(NaN)", nan, std::numeric_limits<cmsFloat64Number>::quiet_NaN(), actual);
    }
}

// Test 3: Infinity handling
// Square of infinity should be infinity; handle both +Inf and -Inf.
static void test_Sqr_Infinity() {
    // +Inf
    ++tests_run;
    cmsFloat64Number pos_inf = std::numeric_limits<cmsFloat64Number>::infinity();
    cmsFloat64Number actual_pos = Sqr(pos_inf);
    if (std::isinf(static_cast<double>(actual_pos)) && (actual_pos > 0)) {
        ++tests_passed;
    } else {
        ++tests_failed;
        log_failure("Sqr(+Inf)", pos_inf, std::numeric_limits<cmsFloat64Number>::infinity(), actual_pos);
    }

    // -Inf
    ++tests_run;
    cmsFloat64Number neg_inf = -std::numeric_limits<cmsFloat64Number>::infinity();
    cmsFloat64Number actual_neg = Sqr(neg_inf);
    if (std::isinf(static_cast<double>(actual_neg)) && (actual_neg > 0)) {
        ++tests_passed;
    } else {
        ++tests_failed;
        log_failure("Sqr(-Inf)", neg_inf, std::numeric_limits<cmsFloat64Number>::infinity(), actual_neg);
    }
}

// Entry point
int main() {
    // Run test cases
    test_Sqr_basic_values();
    test_Sqr_NaN();
    test_Sqr_Infinity();

    // Report results
    report_results();

    // Return non-zero if any test failed (simple convention)
    return (tests_failed == 0) ? 0 : 1;
}