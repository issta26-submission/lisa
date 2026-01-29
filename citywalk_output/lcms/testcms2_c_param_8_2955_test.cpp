// Test suite for cmsFloat32Number param_8(cmsFloat32Number x, const cmsFloat64Number Params[])
// Step 1 (Conceptual): Candidate Keywords inferred from the focal method
// - param_8: focal function under test
// - pow: used for the base/exponent calculation
// - Params[]: 5-element parameter array with indices [0..4]
// - x: input value (cmsFloat32Number)
// - Result type: cmsFloat32Number (float), conversion from cmsFloat64Number
// - Edge cases: normal computation, NaN/inf propagation, zero base, zero exponent, negative base with integer exponent
// This test suite uses a lightweight harness (no GTest) and prints results to stdout/stderr.

#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <testcms2.h>
#include <cmath>


// Include the project's test header to access cms types and param_8.
// If the header already declares param_8 with C linkage, this forward declaration is safe.
// We force C linkage to ensure compatibility with the C implementation.
extern "C" {

    // Forward declaration to ensure linkage in C++ test binary.
    // If testcms2.h already declares this, this is harmless.
    cmsFloat32Number param_8(cmsFloat32Number x, const cmsFloat64Number Params[]);
}

// Simple, non-terminating test harness (no abort on failure)
static int g_total = 0;
static int g_passed = 0;

// Log result helper
static void log_result(const char* test_name, bool passed) {
    g_total++;
    if (passed) {
        g_passed++;
    } else {
        fprintf(stderr, "Test failed: %s\n", test_name);
    }
}

// Approximate comparison for floating-point values with tolerance
static bool almost_equal(double a, double b, double tol) {
    return std::fabs(a - b) <= tol;
}

// Test 1: Basic positive case
static void test_param_8_basic_positive() {
    // Params: [0]=2.0, [1]=3.0, [2]=1.0, [3]=0.0, [4]=0.5
    // x = 0.0
    // exponent = Params[2]*x + Params[3] = 0.0
    // Val = 2.0 * pow(3.0, 0.0) + 0.5 = 2.0 * 1.0 + 0.5 = 2.5
    cmsFloat64Number P[5] = {2.0, 3.0, 1.0, 0.0, 0.5};
    cmsFloat32Number res = param_8(0.0f, P);
    double expected = 2.5;
    double diff = std::fabs((double)res - expected);
    log_result("param_8_basic_positive", diff <= 1e-5);
}

// Test 2: Non-integer exponent with positive base
static void test_param_8_noninteger_exponent() {
    // Params: [0]=1.5, [1]=2.0, [2]=0.5, [3]=1.0, [4]=-0.25
    // x = 1.0
    // exponent = 0.5*1.0 + 1.0 = 1.5
    // Val = 1.5 * pow(2.0, 1.5) - 0.25
    // pow(2, 1.5) = 2.8284271247461903; Val ≈ 4.242640687+(-0.25) ≈ 3.992640687
    cmsFloat64Number P[5] = {1.5, 2.0, 0.5, 1.0, -0.25};
    cmsFloat32Number res = param_8(1.0f, P);
    double expected = 3.992640687; // precise enough for tolerance
    double diff = std::fabs((double)res - expected);
    log_result("param_8_noninteger_exponent", diff <= 1e-5);
}

// Test 3: Negative base with integer exponent (valid scenario)
static void test_param_8_negative_base_integer_exponent() {
    // Params: [0]=1.0, [1]=-2.0, [2]=2.0, [3]=0.0, [4]=0.0
    // x = 1.0
    // exponent = 2*1.0 + 0 = 2
    // pow(-2, 2) = 4; Val = 1.0 * 4 + 0 = 4.0
    cmsFloat64Number P[5] = {1.0, -2.0, 2.0, 0.0, 0.0};
    cmsFloat32Number res = param_8(1.0f, P);
    double expected = 4.0;
    double diff = std::fabs((double)res - expected);
    log_result("param_8_negative_base_integer_exponent", diff <= 1e-5);
}

// Test 4: NaN propagation when exponent is non-integer with negative base
static void test_param_8_nan_nan_result() {
    // Params: [0]=1.0, [1]=-2.0, [2]=1.0, [3]=0.5, [4]=0.0
    // x = 0.0
    // exponent = 1.0*0.0 + 0.5 = 0.5 (non-integer) => pow(-2, 0.5) = NaN
    cmsFloat64Number P[5] = {1.0, -2.0, 1.0, 0.5, 0.0};
    cmsFloat32Number res = param_8(0.0f, P);
    double resd = (double)res;
    bool is_nan = std::isnan(resd);
    log_result("param_8_nan_propagation", is_nan);
}

// Test 5: Zero base with positive exponent yields Params[4]
static void test_param_8_zero_base_positive_exponent() {
    // Params: [0]=2.0, [1]=0.0, [2]=1.0, [3]=2.0, [4]=7.5
    // x = 0.0
    // exponent = 0.0*0.0 + 2.0 = 2.0
    // pow(0.0, 2.0) = 0.0; Val = 2.0 * 0.0 + 7.5 = 7.5
    cmsFloat64Number P[5] = {2.0, 0.0, 1.0, 2.0, 7.5};
    cmsFloat32Number res = param_8(0.0f, P);
    double expected = 7.5;
    double diff = std::fabs((double)res - expected);
    log_result("param_8_zero_base_positive_exponent", diff <= 1e-5);
}

// Test 6: Overflow behavior (very large exponent leading to Inf)
static void test_param_8_overflow_behavior() {
    // Params: [0]=1.0, [1]=10.0, [2]=3.0, [3]=0.0, [4]=0.0
    // x = 1000.0
    // exponent = 3*1000 = 3000; pow(10,3000) => overflow to Inf
    cmsFloat64Number P[5] = {1.0, 10.0, 3.0, 0.0, 0.0};
    cmsFloat32Number res = param_8(1000.0f, P);
    double resd = (double)res;
    bool is_inf = std::isinf(resd);
    log_result("param_8_overflow_behavior", is_inf);
}

// Test 7: 0^0 edge-case (poisoned by math lib semantics in some implementations)
// We check that the function returns a well-defined value (likely 1.0) for 0^0
static void test_param_8_zero_pow_zero() {
    // Params: [0]=1.0, [1]=0.0, [2]=0.0, [3]=0.0, [4]=0.0
    // x = 0.0
    // exponent = 0*0 + 0 = 0; pow(0, 0) -> defined as 1 in many math libs
    cmsFloat64Number P[5] = {1.0, 0.0, 0.0, 0.0, 0.0};
    cmsFloat32Number res = param_8(0.0f, P);
    double expected = 1.0;
    double diff = std::fabs((double)res - expected);
    log_result("param_8_zero_pow_zero", diff <= 1e-5);
}

int main() {
    // Run all test cases
    test_param_8_basic_positive();
    test_param_8_noninteger_exponent();
    test_param_8_negative_base_integer_exponent();
    test_param_8_nan_nan_result();
    test_param_8_zero_base_positive_exponent();
    test_param_8_overflow_behavior();
    test_param_8_zero_pow_zero();

    // Summary
    printf("param_8 test suite: %d passed, %d total\n", g_passed, g_total);
    // Return non-zero if any test failed
    return (g_passed == g_total) ? 0 : 1;
}