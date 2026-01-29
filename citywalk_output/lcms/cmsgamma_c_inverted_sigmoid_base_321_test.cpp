// Unit test suite for the focal method: inverted_sigmoid_base
// This test is designed to compile with C++11 and does not rely on GTest.
// It follows the given guidance to cover true/false branches and uses a
// lightweight test harness with non-terminating assertions (no fatal exit).

#include <limits>
#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>
#include <cmath>


// Focal method under test (reproduced for unit testing within this TU).
// In the original project this would come from cmsgamma.c via the cmsINLINE macro.
static inline double inverted_sigmoid_base(double k, double t) {
    // Domain note (for reference):
    // The expression (1.0 / (t + 0.5)) - 1.0 must be > 0 to have a valid log.
    // This implies t in (-0.5, 0.5) for finite real results with typical k != 0.
    // We do not enforce domain checks here; tests are designed for valid inputs
    // and to observe behavior on boundary-like inputs.
    return -std::log((1.0 / (t + 0.5)) - 1.0) / k;
}

// Simple non-terminating assertion helpers
static bool almost_equal(double a, double b, double tol = 1e-9) {
    // Treat NaN as not equal to anything (including itself)
    if (std::isnan(a) || std::isnan(b)) return false;
    return std::fabs(a - b) <= tol;
}

// Test 1: k=1.0, t=0.0 -> expect 0.0
// Verifies the base case where the log argument equals 1 and cancels out.
static bool test_inverted_sigmoid_basic_zero() {
    double k = 1.0;
    double t = 0.0;
    double res = inverted_sigmoid_base(k, t);
    double expected = 0.0;
    bool ok = almost_equal(res, expected, 1e-12);
    if (!ok) {
        std::cout << "[FAIL] test_inverted_sigmoid_basic_zero: got " 
                  << std::setprecision(15) << res << ", expected " 
                  << expected << std::endl;
    }
    return ok;
}

// Test 2: k=2.0, t=0.25
// Verifies typical mid-range inputs produce a finite positive result.
static bool test_inverted_sigmoid_positive_case() {
    double k = 2.0;
    double t = 0.25;
    // Precomputed expected value: -log((1/(t+0.5)) - 1) / k
    // (1/(0.75) - 1) = 1.3333333333333333 - 1 = 0.3333333333333333
    // log(0.3333333333333333) = -1.0986122886681098
    // -log(...) / 2 = 0.5493061443340549
    double res = inverted_sigmoid_base(k, t);
    double expected = 0.5493061443340549;
    bool ok = almost_equal(res, expected, 1e-9);
    if (!ok) {
        std::cout << "[FAIL] test_inverted_sigmoid_positive_case: got " 
                  << std::setprecision(15) << res << ", expected " 
                  << expected << std::endl;
    }
    return ok;
}

// Test 3: k=-3.0, t=0.25
// Verifies negative k yields expected negative value.
static bool test_inverted_sigmoid_negative_k() {
    double k = -3.0;
    double t = 0.25;
    // Expected: 1.0986122886681098 / -3 = -0.3662040962227033
    double res = inverted_sigmoid_base(k, t);
    double expected = -0.3662040962227033;
    bool ok = almost_equal(res, expected, 1e-9);
    if (!ok) {
        std::cout << "[FAIL] test_inverted_sigmoid_negative_k: got " 
                  << std::setprecision(15) << res << ", expected " 
                  << expected << std::endl;
    }
    return ok;
}

// Test 4: t near -0.5 boundary: k=1.0, t=-0.49
// Verifies large magnitude negative output as t approaches -0.5 from above.
static bool test_inverted_sigmoid_near_minus_half() {
    double k = 1.0;
    double t = -0.49; // t+0.5 = 0.01 -> 1/0.01 - 1 = 99; log(99)=4.59511985
    double res = inverted_sigmoid_base(k, t);
    double expected = -std::log(99.0); // ~ -4.59511985013459
    bool ok = almost_equal(res, expected, 1e-9);
    if (!ok) {
        std::cout << "[FAIL] test_inverted_sigmoid_near_minus_half: got " 
                  << std::setprecision(15) << res << ", expected " 
                  << expected << std::endl;
    }
    return ok;
}

// Test 5: t near +0.5 boundary: k=1.0, t=0.49
// Verifies finite large magnitude output as t approaches 0.5 from below.
static bool test_inverted_sigmoid_near_plus_half() {
    double k = 1.0;
    double t = 0.49; // t+0.5 = 0.99 -> 1/0.99 - 1 ≈ 0.010101..., log(...) ≈ -4.59512
    double res = inverted_sigmoid_base(k, t);
    double expected = std::log(1.0 / 0.99 - 1.0) * -1.0; // ≈ 4.59511985
    // To avoid tiny floating differences, compare with a reasonable tolerance
    bool ok = almost_equal(res, expected, 1e-9);
    if (!ok) {
        std::cout << "[FAIL] test_inverted_sigmoid_near_plus_half: got " 
                  << std::setprecision(15) << res << ", expected " 
                  << expected << std::endl;
    }
    return ok;
}

// Test 6: t extremely close to the upper boundary: t = 0.5 - 1e-6
// Verifies large positive value (approx ~13.8155 for k=1).
static bool test_inverted_sigmoid_boundary_close() {
    double k = 1.0;
    double t = 0.5 - 1e-6;
    double res = inverted_sigmoid_base(k, t);
    // Approximate expected using the same math with a small delta
    //  t+0.5 = 0.999999; 1/0.999999 ≈ 1.000001000001; minus 1 ≈ 0.000001000001
    //  log(0.000001000001) ≈ -13.81550956; -log(...) ≈ 13.81550956
    double expected = 13.81550956;
    bool ok = almost_equal(res, expected, 1e-6);
    if (!ok) {
        std::cout << "[FAIL] test_inverted_sigmoid_boundary_close: got " 
                  << std::setprecision(15) << res << ", expected " 
                  << expected << std::endl;
    }
    return ok;
}

// Test 7: k=0.0, t=0.0
// Domain edge: division by zero in the formula; result should be NaN (0/0 scenario yields NaN).
static bool test_inverted_sigmoid_zero_k_nan() {
    double k = 0.0;
    double t = 0.0;
    double res = inverted_sigmoid_base(k, t);
    bool ok = std::isnan(res);
    if (!ok) {
        std::cout << "[FAIL] test_inverted_sigmoid_zero_k_nan: expected NaN, got " 
                  << std::setprecision(15) << res << std::endl;
    }
    return ok;
}

// Runner
int main() {
    // Output headers for clarity
    std::cout << "Running unit tests for inverted_sigmoid_base (cmsgamma.c focal method analog)\n";
    std::cout << "Note: Tests are crafted to cover core domain, edge boundaries, and error handling scenarios.\n\n";

    int total = 0;
    int passed = 0;

    auto run = [&](const std::string& name, bool ok) {
        ++total;
        if (ok) {
            ++passed;
            std::cout << "[OK]    " << name << "\n";
        } else {
            std::cout << "[FAILED] " << name << "\n";
        }
    };

    run("test_inverted_sigmoid_basic_zero", test_inverted_sigmoid_basic_zero());
    run("test_inverted_sigmoid_positive_case", test_inverted_sigmoid_positive_case());
    run("test_inverted_sigmoid_negative_k", test_inverted_sigmoid_negative_k());
    run("test_inverted_sigmoid_near_minus_half", test_inverted_sigmoid_near_minus_half());
    run("test_inverted_sigmoid_near_plus_half", test_inverted_sigmoid_near_plus_half());
    run("test_inverted_sigmoid_boundary_close", test_inverted_sigmoid_boundary_close());
    run("test_inverted_sigmoid_zero_k_nan", test_inverted_sigmoid_zero_k_nan());

    std::cout << "\nTest summary: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}