/*
Unit Test Suite for the focal method: sigmoid_factory in cmsgamma.c

Step 1 - Program Understanding (brief)
- Focal method under test: cmsINLINE double sigmoid_factory(double k, double t)
  - Computes a scaled sigmoid-like value using an inner helper sigmoid_base(k, ...)
  - Formula: correction = 0.5 / sigmoid_base(k, 1)
             return correction * sigmoid_base(k, 2.0 * t - 1.0) + 0.5
  - Key dependent components (Candidate Keywords): sigmoid_factory, sigmoid_base, correction, 0.5 constant, linear transform on t

Step 2 - Unit Test Generation
- Target: Provide a comprehensive test suite for sigmoid_factory without relying on GoogleTest.
- Approach: Validate critical invariants that hold regardless of the exact implementation of sigmoid_base:
  - Invariant A: sigmoid_factory(k, 1) should be exactly 1 for all k (assuming sigmoid_base(k,1) != 0).
  - Invariant B: For t in [0,1], sigmoid_factory(k,t) should lie in [0,1].
  - Invariant C: Sigmoid_factory is monotonic non-decreasing in t (if sigmoid_base is monotonic in its second argument).
  - Invariant D: For a range of extreme k values, the function remains finite (not NaN/Inf).
- Test harness uses a minimal C++ test runner (no GTest) and non-terminating assertions to maximize coverage.

Step 3 - Test Case Refinement
- Tests are written to maximize execution paths and ensure executable, self-contained tests in C++11.
- Access to the focal function is via C linkage: extern "C" double sigmoid_factory(double k, double t);
- Static functions and private state inside the C file are not accessed directly; tests rely on public behavior of sigmoid_factory.

Note: This test suite assumes that the project (including cmsgamma.c) is compilable under C++11 and will be linked during the test build. If the compiler/linker flags differ, adjust accordingly.

Now the code:

*/

#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>
#include <sstream>


// Declaration of the function under test with C linkage.
// The actual implementation resides in cmsgamma.c (provided by the project under test).
extern "C" double sigmoid_factory(double k, double t);

// Simple, non-terminating assertion helper.
// Keeps track of total failures and logs diagnostic messages without aborting tests early.
static int g_failures = 0;

static void expect_true(bool condition, const std::string& message) {
    if (!condition) {
        ++g_failures;
        std::cerr << "Expectation failed: " << message << std::endl;
    }
}

// Utility: convert double to string with reasonable precision for messaging
static std::string dtoa(double v, int precision = 12) {
    std::ostringstream oss;
    oss << std::setprecision(precision) << std::fixed << v;
    return oss.str();
}

// Test 1: For t = 1.0, sigmoid_factory(k, 1.0) should equal 1.0 for a variety of k.
// Rationale: Function design mathematically yields 0.5 + (0.5 / sigmoid_base(k,1)) * sigmoid_base(k,1) = 1.0.
static void test_sigmoid_factory_returns_one_at_t1() {
    std::vector<double> ks = { -10.0, -1.0, 0.0, 0.5, 1.0, 5.0, 10.0, 123.456 };
    for (double k : ks) {
        double val = sigmoid_factory(k, 1.0);
        bool ok = std::fabs(val - 1.0) <= 1e-12;
        expect_true(ok, "sigmoid_factory(k=" + dtoa(k) + ", t=1.0) == 1.0; got " + dtoa(val));
    }
}

// Test 2: For t in [0, 1], the output should lie within [0, 1] (within a small epsilon).
static void test_sigmoid_factory_bounds() {
    std::vector<double> ks = { -20.0, -2.5, -0.5, 0.0, 0.5, 2.0, 8.0, 20.0 };
    std::vector<double> ts = { 0.0, 0.25, 0.5, 0.75, 1.0 };

    for (double k : ks) {
        for (double t : ts) {
            double val = sigmoid_factory(k, t);
            bool ok = (val >= -1e-12) && (val <= 1.0 + 1e-12);
            expect_true(ok, "sigmoid_factory(k=" + dtoa(k) + ", t=" + dtoa(t) +
                               ") is outside [0,1]; got " + dtoa(val));
        }
    }
}

// Test 3: Monotonicity in t (non-decreasing with respect to t) for representative k.
// This checks that increasing t does not decrease the output (assuming sigmoid_base is monotonic in its second arg).
static void test_sigmoid_factory_monotonicity() {
    std::vector<double> ks = { -5.0, -1.0, 0.0, 1.0, 3.0, 7.0 };
    const std::vector<double> ts = { 0.0, 0.1, 0.2, 0.3, 0.4,
                                    0.5, 0.6, 0.7, 0.8, 0.9, 1.0 };

    for (double k : ks) {
        double prev = sigmoid_factory(k, ts.front());
        for (size_t i = 1; i < ts.size(); ++i) {
            double cur = sigmoid_factory(k, ts[i]);
            // Non-decreasing condition; allow a tiny tolerance for floating-point quirks.
            bool ok = (cur + 1e-12 >= prev);
            expect_true(ok, "sigmoid_factory monotonicity violated for k=" +
                               dtoa(k) + " at t=" + dtoa(ts[i - 1]) + "->" + dtoa(ts[i]) +
                               " (prev=" + dtoa(prev) + ", cur=" + dtoa(cur) + ")");
            prev = cur;
        }
    }
}

// Test 4: Sanity check that the function remains finite for extreme k values.
// This ensures we don't emit NaN/Inf for large magnitudes.
static void test_sigmoid_factory_extreme_values() {
    std::vector<double> ks = { -1e6, -1e3, -1e2, 1e2, 1e3, 1e6 };
    std::vector<double> ts = { 0.0, 0.5, 1.0 };

    for (double k : ks) {
        for (double t : ts) {
            double val = sigmoid_factory(k, t);
            bool finite = std::isfinite(val);
            expect_true(finite, "sigmoid_factory(k=" + dtoa(k) + ", t=" + dtoa(t) +
                                  ") produced non-finite value: " + dtoa(val));
        }
    }
}

// Main test runner
int main() {
    // Run the test cases
    test_sigmoid_factory_returns_one_at_t1();
    test_sigmoid_factory_bounds();
    test_sigmoid_factory_monotonicity();
    test_sigmoid_factory_extreme_values();

    // Report summary
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED" << std::endl;
        return g_failures;
    }
}