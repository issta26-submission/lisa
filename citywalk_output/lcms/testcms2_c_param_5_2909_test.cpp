// Unit test suite for the focal method param_5 in testcms2.c
// This test suite is written for C++11 without GoogleTest.
// It uses a lightweight, non-terminating assertion approach that reports
// PASS/FAIL per test and continues execution to maximize coverage.
//
// Key dependencies and keywords gathered from the focal method:
// - Candidate Keywords: cmsFloat32Number (x), cmsFloat64Number (Params[]), pow, if (x >= Params[4]),
//   e = Params[1]*x + Params[2], if (e > 0), Val, Params[0] (Gamma), Params[5], Params[3] (c), Params[6] (f)
// - Branches to cover: outer condition x >= Params[4] (true/false) and inner condition e > 0 (true/false)

#include <cfloat>
#include <iostream>
#include <cmath>
#include <testcms2.h>


extern "C" {
    // Focal C function to test (assumed to be provided by the project under test)
    cmsFloat32Number param_5(cmsFloat32Number x, const cmsFloat64Number Params[]);
}

// Lightweight test harness (non-terminating)
static int g_failures = 0;

static void test_pass(const char* test_name) {
    std::cout << "[PASS] " << test_name << std::endl;
}

static void test_fail(const char* test_name, const char* message) {
    std::cout << "[FAIL] " << test_name << " - " << message << std::endl;
    ++g_failures;
}

// Floating comparison helper
static void assert_float_eq(const char* test_name, cmsFloat32Number expected, cmsFloat32Number actual, cmsFloat64Number tol = 1e-4) {
    double de = static_cast<double>(expected);
    double da = static_cast<double>(actual);
    if (std::fabs(de - da) <= tol) {
        test_pass(test_name);
    } else {
        char buf[256];
        std::snprintf(buf, sizeof(buf), "expected %.6f, got %.6f (tol %.6f)", de, da, tol);
        test_fail(test_name, buf);
    }
}

// Test 1: Outer branch true (x >= d) and inner branch true (e > 0)
void test_param5_case_outer_true_inner_true() {
    // Params layout: [0]=Gamma, [1]=a, [2]=b, [3]=c, [4]=d, [5]=add, [6]=f
    cmsFloat64Number Params[7] = {3.0, 0.5, 2.0, 4.0, 10.0, 1.5, 0.0};
    cmsFloat32Number x = 12.0f; // x >= d (12 >= 10)
    cmsFloat32Number result = param_5(x, Params);

    // e = a*x + b = 0.5*12 + 2 = 8 > 0
    // Val = e^Gamma + add = 8^3 + 1.5 = 512 + 1.5 = 513.5
    double expected_d = std::pow(0.5*12.0 + 2.0, 3.0) + 1.5;
    cmsFloat32Number expected = static_cast<cmsFloat32Number>(expected_d);

    assert_float_eq("param_5_case_outer_true_inner_true", expected, result);
}

// Test 2: Outer branch true (x >= d) and inner branch false (e <= 0)
void test_param5_case_outer_true_inner_false() {
    // Use a negative slope to force e <= 0 for x >= d
    cmsFloat64Number Params[7] = {2.0, -1.0, 1.0, 4.0, 5.0, 1.5, 0.0}; // d=5, a=-1, b=1
    cmsFloat32Number x = 12.0f; // 12 >= d (5)
    cmsFloat32Number result = param_5(x, Params);

    // e = -1*12 + 1 = -11 <= 0 -> Val = 0
    cmsFloat32Number expected = 0.0f;

    assert_float_eq("param_5_case_outer_true_inner_false", expected, result);
}

// Test 3: Outer branch false (x < d)
void test_param5_case_outer_false() {
    // Params layout: [0]=Gamma, [1]=a, [2]=b, [3]=c, [4]=d, [5]=add, [6]=f
    cmsFloat64Number Params[7] = {3.0, 0.5, 2.0, 4.0, 10.0, 1.5, 0.0}; // d=10
    cmsFloat32Number x = 5.0f; // x < d
    cmsFloat32Number result = param_5(x, Params);

    // Val = x*c + f = 5*4 + 0 = 20
    cmsFloat32Number expected = 20.0f;

    assert_float_eq("param_5_case_outer_false", expected, result);
}

// Test 4: Boundary case where x == d and e > 0 (outer true, inner true at boundary)
void test_param5_case_boundary_outer_true_inner_true() {
    // Params where x == d and e > 0
    cmsFloat64Number Params[7] = {3.0, 0.5, 2.0, 4.0, 10.0, 1.5, 0.0};
    cmsFloat32Number x = 10.0f; // x == d
    cmsFloat32Number result = param_5(x, Params);

    // e = 0.5*10 + 2 = 7 > 0
    // Val = 7^3 + 1.5 = 343 + 1.5 = 344.5
    double expected_d = std::pow(0.5*10.0 + 2.0, 3.0) + 1.5;
    cmsFloat32Number expected = static_cast<cmsFloat32Number>(expected_d);

    assert_float_eq("param_5_case_boundary_outer_true_inner_true", expected, result);
}

// Test 5: Inner branch edge case where e == 0 (still outer true)
void test_param5_case_inner_edge_e_zero() {
    // Choose a and b to make e == 0 for x >= d
    cmsFloat64Number Params[7] = {2.0, -0.5, 6.0, 4.0, 10.0, 2.0, 0.5}; // e = -0.5*x + 6
    cmsFloat32Number x = 12.0f; // e = -6 + 6 = 0
    cmsFloat32Number result = param_5(x, Params);

    // Since e == 0, inner if (e > 0) is false -> Val = 0
    cmsFloat32Number expected = 0.0f;

    assert_float_eq("param_5_case_inner_edge_e_zero", expected, result);
}

int main() {
    // Run all tests
    test_param5_case_outer_true_inner_true();
    test_param5_case_outer_true_inner_false();
    test_param5_case_outer_false();
    test_param5_case_boundary_outer_true_inner_true();
    test_param5_case_inner_edge_e_zero();

    // Summary
    std::cout << "Total failures: " << g_failures << std::endl;
    // Return non-zero if any test failed, but do not terminate tests prematurely
    return (g_failures == 0) ? 0 : 1;
}