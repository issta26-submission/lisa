/*
Unit Test Suite for focal method fsRGB(double l) in file makesRGB.c

Approach:
- Treat fsRGB as a thin wrapper that computes l/max_input and passes it to sRGB_from_linear.
- Use the real sRGB_from_linear and the real max_input symbols from the code under test (linked at test time) to compute expected results.
- Validate that fsRGB(l) == sRGB_from_linear(l / max_input) for a representative set of inputs.
- Add additional coverage-based tests:
  • Deterministic behavior across repeated calls (same input yields same result).
  • Non-decreasing behavior for a monotone input sequence (assuming sRGB_from_linear is monotone, which is typical for color transforms).
  • Edge-case coverage for negative, zero, within [0,1], and >1 ratio through the wrapper.
- Do not rely on Google Test (GTest). Use a lightweight in-process test harness with simple non-terminating assertions (logs) and a final summary.

Notes:
- This test suite uses the actual fsRGB from the code under test and relies on the presence of:
  extern "C" double fsRGB(double l);
  extern "C" double sRGB_from_linear(double x);
  extern "C" double max_input;
- The expected results are computed by directly calling sRGB_from_linear with the transformation l / max_input, mirroring the focal method's behavior.
- If the underlying sRGB_from_linear or max_input implementations differ in the test environment, the expected calculations will adapt accordingly (as they are derived from the same symbols).

The code is C++11-compatible and self-contained as a single file. Compile and link with the project that provides makesRGB.c (and thus fsRGB), ensuring the sRGB_from_linear and max_input symbols are available at link time.

*/

#include <sRGB.h>
#include <../../pngpriv.h>
#include <vector>
#include <iomanip>
#include <string>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <math.h>


// Declarations of the focal method and its dependencies (provided by the code under test).
extern "C" {
    // Focal method under test (wrapper we want to verify)
    double fsRGB(double l);

    // Dependency that fsRGB uses
    double sRGB_from_linear(double x);

    // Global max input used by fsRGB
    extern double max_input;
}

// Simple non-terminating test harness
namespace TestHarness {
    // Tolerance for floating-point comparisons
    constexpr double EPS = 1e-12;

    // Number of failed tests (non-terminating assertion style)
    int g_failures = 0;

    // Pretty-print two doubles
    static std::string to_string_d(double v) {
        std::ostringstream oss;
        oss << std::setprecision(15) << v;
        return oss.str();
    }

    // Approximate equality check for doubles
    bool approx_eq(double a, double b, double tol = EPS) {
        // Handle NaN and Inf gracefully
        if (std::isnan(a) || std::isnan(b)) return false;
        if (!std::isfinite(a) || !std::isfinite(b)) return false;
        return std::abs(a - b) <= tol;
    }

    // Non-terminating assertion: log but do not abort
    void ASSERT_APPROX_EQ(const std::string& name, double actual, double expected, double tol = EPS) {
        if (approx_eq(actual, expected, tol)) {
            std::cout << "[PASS] " << name << " | actual=" << to_string_d(actual)
                      << " expected=" << to_string_d(expected) << "\n";
        } else {
            std::cout << "[FAIL] " << name << " | actual=" << to_string_d(actual)
                      << " expected=" << to_string_d(expected)
                      << " (tol=" << tol << ")\n";
            ++g_failures;
        }
    }

    // Non-terminating assertion for monotonicity
    void ASSERT_NONDECREASING(const std::string& name, const std::vector<double>& values) {
        bool ok = true;
        for (size_t i = 1; i < values.size(); ++i) {
            if (values[i-1] > values[i] + EPS) { ok = false; break; }
        }
        if (ok) {
            std::cout << "[PASS] " << name << " monotonic non-decreasing\n";
        } else {
            std::cout << "[FAIL] " << name << " not monotonic non-decreasing\n";
            ++g_failures;
        }
    }

    // Deterministic test (same input yields same output across repeated calls)
    void ASSERT_DETERMINISTIC(const std::string& name, double l) {
        double first  = fsRGB(l);
        double second = fsRGB(l);
        if (approx_eq(first, second)) {
            std::cout << "[PASS] " << name << " deterministic for l=" << l
                      << " -> " << first << "\n";
        } else {
            std::cout << "[FAIL] " << name << " non-deterministic for l=" << l
                      << " first=" << first << " second=" << second << "\n";
            ++g_failures;
        }
    }

    // Run all tests
    void run_all() {
        std::cout << "Running fsRGB unit tests (C++11 harness)\n";

        // Basic coverage inputs, including negative, zero, within [0,1], and >1 region
        // Note: We rely on the actual sRGB_from_linear and max_input symbols provided by the target code.
        std::vector<double> test_inputs = {
            -1.5,  // negative input
             0.0,  // zero
             0.25, // within [0,1]
             0.5,  // within [0,1]
             1.0,  // boundary
             2.0,  // >1 region (ratio > 0 or >1 depending on max_input)
             3.0   // larger >1 region
        };

        std::cout << "Test: fsRGB(l) should equal sRGB_from_linear(l/max_input) for several l values.\n";

        // Compute and compare expected vs actual for each test input
        for (size_t i = 0; i < test_inputs.size(); ++i) {
            double l = test_inputs[i];
            // Guard against undefined behavior if max_input is not set yet
            // We assume max_input is a valid finite double in the tested environment.
            double ratio = l / max_input;
            double expected = sRGB_from_linear(ratio);
            double actual   = fsRGB(l);
            std::string test_name = "fsRGB_match_l_" + std::to_string(i) +
                                    "_l=" + std::to_string(l);
            ASSERT_APPROX_EQ(test_name, actual, expected, 1e-9);
        }

        // Determinism test: same input should yield same result across calls
        for (double l : test_inputs) {
            std::string name = "fsRGB_deterministic_l_" + std::to_string(l);
            ASSERT_DETERMINISTIC(name, l);
        }

        // Monotonicity test: sample an increasing sequence and ensure non-decreasing outputs
        std::vector<double> monotonic_inputs = { -5.0, -1.0, 0.0, 0.25, 0.75, 1.5, 5.0, 10.0 };
        std::vector<double> monotonic_outputs;
        monotonic_outputs.reserve(monotonic_inputs.size());
        for (double l : monotonic_inputs) {
            monotonic_outputs.push_back(fsRGB(l));
        }
        ASSERT_NONDECREASING("fsRGB_monotonicity", monotonic_outputs);

        // Summary
        if (g_failures == 0) {
            std::cout << "[SUMMARY] All tests PASSED\n";
        } else {
            std::cout << "[SUMMARY] " << g_failures << " test(s) FAILED\n";
        }
    }
} // namespace TestHarness

int main() {
    // Run the test suite
    TestHarness::run_all();
    return 0;
}