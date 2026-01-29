// Test suite for the Gamma function extracted from testcms2.c
// This file provides a small, self-contained test harness (no GTest) to validate
// the behaviour of Gamma(x, Params) where Params[0] is the exponent.
// The Gamma function is assumed to be defined in testcms2.c with C linkage.
// We compile this as part of a C++11 project and link against testcms2.c.
//
// Key Candidate Keywords discovered from the focal method:
// - Gamma: the function under test
// - cmsFloat32Number, cmsFloat64Number: input/output numeric types
// - pow: the mathematical operation used inside Gamma
// - Params[0]: exponent parameter extracted from the Params array
// - x: input base for the exponentiation
//
// Domain knowledge notes:
// - Gamma(x, [p]) returns (cmsFloat32Number) pow(x, p)
// - We test various exponent values (positive, zero, negative, fractional)
// - We test positive, negative, and zero bases, including NaN outcomes for non-integer exponents with negative bases
//
// Implementation details:
// - Non-terminating test assertions: tests print PASS/FAIL but do not abort on failure
// - Tests are executed from main() as gtest is not allowed
// - Static helpers are kept local to avoid exposing test internals

#include <cstring>
#include <cstdio>
#include <limits>
#include <testcms2.h>
#include <cstdlib>
#include <cmath>


// Forward declaration of the focal function with C linkage
#ifdef __cplusplus
extern "C" {
#endif
// The Gamma function signature as provided in testcms2.c
typedef float cmsFloat32Number;
typedef double cmsFloat64Number;

cmsFloat32Number Gamma(cmsFloat32Number x, const cmsFloat64Number Params[]);
#ifdef __cplusplus
}
#endif

// Import dependencies from testcms2.h if available (optional, to ensure types align)
// We attempt to include, but do not rely on them for core Gamma testing.

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple floating-point comparison with tolerance
static bool approx_equal_float(float a, double b, double tol) {
    double da = static_cast<double>(a);
    return std::fabs(da - b) <= tol;
}

// Run a single Gamma test case
static void run_gamma_case(const char* test_name,
                           cmsFloat32Number x,
                           cmsFloat64Number exponent,
                           bool expect_nan,
                           double expected_value) {
    cmsFloat64Number params[1] = { exponent };
    cmsFloat32Number result = Gamma(x, params);

    g_total_tests++;

    // If we expect NaN, verify accordingly
    if (expect_nan) {
        bool is_nan = std::isnan(static_cast<double>(result));
        if (is_nan) {
            std::printf("PASS  : %s\n", test_name);
        } else {
            g_failed_tests++;
            std::printf("FAIL  : %s - expected NaN, got %f\n", test_name, static_cast<double>(result));
        }
        return;
    }

    // Otherwise compare against the expected numeric value with a tolerance
    const double tol = 1e-5;
    if (approx_equal_float(result, expected_value, tol)) {
        std::printf("PASS  : %s\n", test_name);
    } else {
        g_failed_tests++;
        std::printf("FAIL  : %s - expected %.12f, got %.12f\n",
                        test_name, expected_value, static_cast<double>(result));
    }
}

// Entry point for Gamma test suite
static void run_all_gamma_tests() {
    // Test 1: x = 4.0, exponent = 2.0 -> 16.0
    // Rationale: simple positive base, integer exponent
    run_gamma_case("Gamma_PosBase_PosExp_Integer", 4.0f, 2.0, false, 16.0);

    // Test 2: x = 2.0, exponent = 0.0 -> 1.0
    // Rationale: any non-zero base raised to 0 is 1
    run_gamma_case("Gamma_Base_PosExp_Zero", 2.0f, 0.0, false, 1.0);

    // Test 3: x = 2.0, exponent = -1.0 -> 0.5
    // Rationale: negative exponent yields reciprocal
    run_gamma_case("Gamma_PosBase_NegExp_Reciprocal", 2.0f, -1.0, false, 0.5);

    // Test 4: x = 0.0, exponent = 2.0 -> 0.0
    // Rationale: 0 raised to positive exponent is 0
    run_gamma_case("Gamma_ZeroBase_PosExp", 0.0f, 2.0, false, 0.0);

    // Test 5: x = -2.0, exponent = 2.0 -> 4.0
    // Rationale: negative base with even exponent yields positive result
    run_gamma_case("Gamma_NegBase_EvenExp_Positive", -2.0f, 2.0, false, 4.0);

    // Test 6: x = -2.0, exponent = 0.5 -> NaN
    // Rationale: negative base with fractional exponent is undefined in reals
    run_gamma_case("Gamma_NegBase_FracExp_NaN", -2.0f, 0.5, true, std::numeric_limits<double>::quiet_NaN());

    // Test 7: x = 9.0, exponent = 0.5 -> 3.0
    // Rationale: square root (positive base)
    run_gamma_case("Gamma_PosBase_FracExp_Sqrt", 9.0f, 0.5, false, 3.0);

    // Test 8: x = 10.0, exponent = 3.0 -> 1000.0
    // Rationale: simple positive base, cubic
    run_gamma_case("Gamma_PosBase_PosExp_Cubic", 10.0f, 3.0, false, 1000.0);

    // Test 9: x = -3.0, exponent = 3.0 -> -27.0
    // Rationale: negative base with odd exponent yields negative result
    run_gamma_case("Gamma_NegBase_OddExp_Positive", -3.0f, 3.0, false, -27.0);
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    // Start tests
    std::printf("Starting Gamma test suite...\n");

    run_all_gamma_tests();

    // Summary
    std::printf("Gamma tests completed: %d total, %d failed.\n",
                g_total_tests, g_failed_tests);

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}