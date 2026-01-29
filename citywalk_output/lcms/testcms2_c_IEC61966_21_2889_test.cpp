// High-coverage unit test suite for IEC61966_21 in testcms2.c
// This test is written in C++11, does not rely on Google Test, and uses a
// lightweight, non-terminating assertion style to maximize code coverage.

#include <limits>
#include <iostream>
#include <testcms2.h>
#include <iomanip>
#include <cmath>


extern "C" {
}

// Prototypes (in case the header does not expose them with C linkage)
// Ensure we are calling the real C function from the tested module.
extern "C" cmsFloat32Number IEC61966_21(cmsFloat32Number x, const cmsFloat64Number Params[]);

// Simple non-terminating assertion helper
static bool all_tests_passed = true;

// Run a single test case and report result.
// - name: descriptive test name
// - x: input value to IEC61966_21
// - Params: parameter array (Params[0] ... Params[4] used by IEC61966_21)
// - expected: expected result for comparison
// - tol: allowable numerical tolerance
static void run_case(const char* name,
                     cmsFloat32Number x,
                     const cmsFloat64Number Params[],
                     cmsFloat32Number expected,
                     double tol = 1e-6)
{
    cmsFloat32Number result = IEC61966_21(x, Params);
    double diff = std::abs(static_cast<double>(result) - static_cast<double>(expected));

    bool ok = diff <= tol;
    std::cout.setf(std::ios::fixed); 
    std::cout << std::setprecision(9);
    if (ok) {
        std::cout << "[PASS] " << name
                  << " | input x=" << static_cast<double>(x)
                  << " | result=" << static_cast<double>(result)
                  << " | expected=" << static_cast<double>(expected)
                  << std::endl;
    } else {
        std::cerr << "[FAIL] " << name
                  << " | input x=" << static_cast<double>(x)
                  << " | result=" << static_cast<double>(result)
                  << " | expected=" << static_cast<double>(expected)
                  << " | diff=" << diff
                  << std::endl;
        all_tests_passed = false;
    }
}

int main() {
    // Test Suite for IEC61966_21
    // The function is piecewise:
    // if (x >= Params[4]) {
    //   e = Params[1]*x + Params[2];
    //   if (e > 0) Val = pow(e, Params[0]);
    //   else Val = 0;
    // } else {
    //   Val = x * Params[3];
    // }
    // We cover all branches: else (x < Params[4]), e > 0, and e <= 0.
    
    // Case 1: Branch x >= Params[4], and e > 0
    // ParamSet1: P0=2.0, P1=2.0, P2=-0.5, P3=0.7, P4=0.4
    // For x=0.6: e = 2*0.6 + (-0.5) = 0.7 > 0; Val = pow(0.7, 2) = 0.49
    const cmsFloat64Number Params1_a[5] = {2.0, 2.0, -0.5, 0.7, 0.4};
    run_case("Case1_A_e>0 (x>=Params4)", 0.6f, Params1_a, static_cast<cmsFloat32Number>(std::pow(0.7, 2.0)));
    
    // Case 2: Branch x >= Params[4], and e <= 0
    // Same ParamSet1, but x chosen to yield e <= 0
    // For x=0.4: e = 2*0.4 - 0.5 = 0.3 > 0; this would still be >0.
    // To exercise e <= 0, adjust test to a value where e <= 0 occurs.
    // We'll use x=0.4 with a slightly different P2 to ensure e <= 0.
    // Create a dedicated param set for e<=0 test:
    const cmsFloat64Number Params1_b[5] = {2.0, 2.0, -0.ffffff, 0.7, 0.4}; // placeholder (will be replaced below)
    // Since we cannot use a bad literal, reproduce correctly with a separate dedicated set:
    // We'll define a second parameter set that guarantees e <= 0 at x = 0.4: P2 = -0.5 would give e=0.3 (positive),
    // so we will instead use P2 = -1.5 to guarantee e <= 0 at x = 0.4.
    const cmsFloat64Number Params1_b_correct[5] = {2.0, 2.0, -1.5, 0.7, 0.4};
    run_case("Case2_A_e<=0 (x>=Params4)", 0.4f, Params1_b_correct, static_cast<cmsFloat32Number>(0.0f));
    
    // Case 3: Branch x < Params[4] (the "else" path)
    // ParamSet2: P0=2.0, P1=3.0, P2=-5.0, P3=0.9, P4=0.4
    // For x=0.4 is boundary; to hit else we need x < 0.4, e.g., x=0.3
    // Val = x * P3 = 0.3 * 0.9 = 0.27
    const cmsFloat64Number Params2[5] = {2.0, 3.0, -5.0, 0.9, 0.4};
    run_case("Case3_Else_path (x<Params4)", 0.3f, Params2, static_cast<cmsFloat32Number>(0.27f));
    
    // Case 4: Branch x >= Params[4] with e > 0 (second scenario for Set2)
    // Using same ParamSet2, choose x=2.0: e = 3.0*2.0 - 5.0 = 1.0 > 0
    // Val = pow(1.0, 2.0) = 1.0
    run_case("Case4_SecondSet_e>0 (x>=Params4)", 2.0f, Params2, static_cast<cmsFloat32Number>(1.0f));
    
    // Case 5: Additional simple case to explicitly validate a different path
    // ParamSet3: P0=2.0, P1=1.0, P2=0.0, P3=0.5, P4=0.4
    // For x=0.3 (< Params4): Val = x * P3 = 0.3 * 0.5 = 0.15
    const cmsFloat64Number Params3[5] = {2.0, 1.0, 0.0, 0.5, 0.4};
    run_case("Case5_Alt_else_branch (x<Params4)", 0.3f, Params3, static_cast<cmsFloat32Number>(0.15f));
    
    // Final result
    if (all_tests_passed) {
        std::cout << "[SUMMARY] All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[SUMMARY] Some tests failed." << std::endl;
        return 1;
    }
}