// Minimal C++11 unit test harness for the Mean(LPSTAT st) function found in tifdiff.c
// This test harness does not rely on any external testing framework (GTest, etc.)
// It uses a small custom assertion framework that continues execution on failures
// to maximize code coverage as requested in <DOMAIN_KNOWLEDGE>.
//
// Important: We declare a compatible LPSTAT type locally to be able to call Mean.
// We rely on the fact that the Mean implementation accesses st->x and st->n.
// If the real LPSTAT structure in the target project differs in memory layout for x or n,
// this test may need adjustment. The provided test uses a lightweight layout with
// two numeric fields (x and n) as a best-fit approximation for unit testing.

#include <cstdio>
#include <iostream>
#include <string>
#include <utils.h>
#include <cmath>
#include <tiffio.h>


// Candidate Keywords extracted from focal method and dependencies:
// Mean, LPSTAT, st->x, st->n
// x: statistic accumulator, n: count or divisor
// tifdiff.c: the focal file containing Mean
//
// We create a compatible type LPSTAT and forward declare Mean with C linkage to enable testing.

extern "C" {
    // Declaration of Mean with a compatibility LPSTAT type.
    // We declare a local, simple struct to model LPSTAT for testing purposes.
    // The real LPSTAT in tifdiff.c is assumed to have at least double fields x and n.
    struct MySTAT {
        double x;
        double n;
    };
    typedef MySTAT* LPSTAT;

    // The function under test (Mean) is defined in tifdiff.c with C linkage.
    // We declare it here so the test can call it.
    double Mean(LPSTAT st);
}

// Lightweight testing framework
static int g_test_failures = 0;

static void log_failure(const char* test_name, const char* message) {
    std::cerr << "TEST FAILED: " << test_name << " - " << message << "\n";
    ++g_test_failures;
}

static void assert_double_equal(const char* test_name, double actual, double expected, double tol = 1e-9) {
    if (std::fabs(actual - expected) > tol) {
        char buf[256];
        std::snprintf(buf, sizeof(buf), "expected %.12f, got %.12f (tol %.1e)", expected, actual, tol);
        log_failure(test_name, buf);
    }
}

// Test 1: Basic positive values
// Verifies Mean computes x/n for standard integers/floating values.
// Domain considerations: normal operating range; typical case.
static void test_Mean_basic() {
    // Candidate Keywords: x, n, division, basic case
    const char* test_name = "Mean_basic";
    MySTAT s;
    s.x = 6.0;
    s.n = 3.0;
    LPSTAT st = &s;

    double m = Mean(st);

    // Expect 6.0 / 3.0 = 2.0
    assert_double_equal(test_name, m, 2.0, 1e-9);
}

// Test 2: Negative and positive values
// Verifies Mean handles negative numerators correctly.
static void test_Mean_negative_positive() {
    const char* test_name = "Mean_negative_positive";
    MySTAT s;
    s.x = -9.0;
    s.n = 3.0;
    LPSTAT st = &s;

    double m = Mean(st);

    // Expect -9.0 / 3.0 = -3.0
    assert_double_equal(test_name, m, -3.0, 1e-9);
}

// Test 3: Zero denominator behavior
// We test behavior when n is zero. In typical IEEE754 environments, division by zero
// for floating-point numbers yields +/-Inf. We check that the result is Inf and positive.
static void test_Mean_zero_denominator() {
    const char* test_name = "Mean_zero_denominator";
    MySTAT s;
    s.x = 5.0;
    s.n = 0.0;
    LPSTAT st = &s;

    double m = Mean(st);

    // Expect NaN or Inf depending on platform; we'll check for Inf to ensure a non-crash path.
    if (!std::isinf(m)) {
        char msg[128];
        std::snprintf(msg, sizeof(msg), "expected Inf, got %f", m);
        log_failure(test_name, msg);
    } else if (m <= 0.0) {
        // If Inf but negative, flag as error as we expect +Inf for positive x / 0
        log_failure(test_name, "expected positive Inf for positive x / 0");
    }
}

// Test 4: Large values to assess numerical stability
// Verifies Mean yields correct ratio for very large numbers (x and n large but finite).
static void test_Mean_large_values() {
    const char* test_name = "Mean_large_values";
    MySTAT s;
    s.x = 1e300;
    s.n = 1e300;
    LPSTAT st = &s;

    double m = Mean(st);

    // Expect 1.0 (within tolerance)
    assert_double_equal(test_name, m, 1.0, 1e-12);
}

// Test 5: Fractional values to ensure proper double precision handling
static void test_Mean_fractional() {
    const char* test_name = "Mean_fractional";
    MySTAT s;
    s.x = 1.25;
    s.n = 0.5; // 1.25 / 0.5 = 2.5
    LPSTAT st = &s;

    double m = Mean(st);

    assert_double_equal(test_name, m, 2.5, 1e-12);
}

// Driver: Run all tests
int main() {
    // Step-by-step test execution following the requirements.
    test_Mean_basic();
    test_Mean_negative_positive();
    test_Mean_zero_denominator();
    test_Mean_large_values();
    test_Mean_fractional();

    if (g_test_failures == 0) {
        std::cout << "All Mean tests PASSED.\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " Mean test(s) FAILED.\n";
        return 1;
    }
}