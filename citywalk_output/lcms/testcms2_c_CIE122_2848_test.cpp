#include <cstring>
#include <iostream>
#include <cmath>
#include <testcms2.h>


// Import project dependencies (as per domain knowledge)

// The focal method is defined in testcms2.c and has C linkage.
// Declare the function for C++ access.
extern "C" cmsFloat32Number CIE122(cmsFloat32Number x, const cmsFloat64Number Params[]);

static int g_total_tests = 0;
static int g_passed_tests = 0;

// Lightweight, non-terminating assertion helpers (no test abortion)
static void log_result(const char* testName, bool passed, const char* details = nullptr) {
    g_total_tests++;
    if (passed) {
        g_passed_tests++;
    } else {
        std::cerr << "Test FAILED: " << testName;
        if (details) std::cerr << " | " << details;
        std::cerr << std::endl;
    }
}

// Helper: approximate equality for floating point results
static bool almost_equal(float a, float b, double tol = 1e-6) {
    return std::fabs(static_cast<double>(a) - static_cast<double>(b)) <= tol;
}

// Test 1
// Description: Outer condition holds (x >= -Params[2]/Params[1]) and inner condition holds (e > 0).
// Expect Val = pow(e, Params[0]).
void test_case_path_outer_and_inner_true() {
    // Parameters chosen to make threshold = -Params[2]/Params[1] = -(-1)/1 = 1
    // Params[0] = 2.0 (power), Params[1] = 1.0, Params[2] = -1.0
    cmsFloat64Number Params[3] = {2.0, 1.0, -1.0};
    cmsFloat32Number x = 2.0f; // x >= 1.0
    cmsFloat32Number result = CIE122(x, Params);

    // e = Params[1]*x + Params[2] = 1*2 + (-1) = 1 > 0
    // Val = pow(1, 2) = 1
    bool ok = almost_equal(static_cast<float>(result), 1.0f);
    log_result("CIE122_PathOuterInnerTrue", ok,
               "Expected result 1.0 when x=2 with Params {2,1,-1}");
}

// Test 2
// Description: Outer condition holds but inner condition fails (e <= 0).
// Expect Val = 0.
void test_case_path_outer_inner_false() {
    cmsFloat64Number Params[3] = {2.0, 1.0, -1.0};
    cmsFloat32Number x = 1.0f; // threshold value; e = 0
    cmsFloat32Number result = CIE122(x, Params);

    // e = Params[1]*x + Params[2] = 1*1 + (-1) = 0 -> inner if (e > 0) is false
    // Val should be 0
    bool ok = almost_equal(static_cast<float>(result), 0.0f);
    log_result("CIE122_PathOuterInnerFalse", ok,
               "Expected result 0.0 when x equals threshold (e == 0)");
}

// Test 3
// Description: Outer condition fails (x < threshold) so Val = 0 unconditionally.
// Expect Val = 0.
void test_case_path_outer_condition_false() {
    cmsFloat64Number Params[3] = {2.0, 1.0, -1.0};
    cmsFloat32Number x = 0.5f; // threshold is 1.0
    cmsFloat32Number result = CIE122(x, Params);

    // Since x < threshold, Val = 0
    bool ok = almost_equal(static_cast<float>(result), 0.0f);
    log_result("CIE122_PathOuterConditionFalse", ok,
               "Expected result 0.0 when x < threshold (outer condition false)");
}

// Test 4
// Description: Use a different Params set where inner computation yields a non-integer result.
// Params[0] = 0.5 (sqrt-like), same threshold, x chosen to produce e = 4 for sqrt(4) = 2.
// Expect Val = sqrt(4) = 2.
void test_case_path_inner_non_integer_exponent() {
    cmsFloat64Number Params[3] = {0.5, 1.0, -1.0}; // Params[0] = 0.5
    cmsFloat32Number x = 5.0f; // e = 1*5 + (-1) = 4
    cmsFloat32Number result = CIE122(x, Params);

    // Val = pow(4, 0.5) = 2
    bool ok = almost_equal(static_cast<float>(result), 2.0f);
    log_result("CIE122_PathInnerNonIntegerExponent", ok,
               "Expected result 2.0 when e=4 and Params[0]=0.5");
}

int main(int argc, char* argv[]) {
    // Run tests
    test_case_path_outer_and_inner_true();
    test_case_path_outer_inner_false();
    test_case_path_outer_condition_false();
    test_case_path_inner_non_integer_exponent();

    // Summary
    std::cout << "CIE122 tests: " << g_passed_tests << "/" << g_total_tests
              << " passed." << std::endl;

    return (g_total_tests == g_passed_tests) ? 0 : 1;
}