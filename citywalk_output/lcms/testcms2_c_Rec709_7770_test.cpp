// Test suite for the focal function Rec709(double L)
// This suite is designed for C++11 environments without relying on any
// testing framework (e.g., Google Test). It uses a lightweight test harness.
// The Rec709 function is defined in C (testcms2.c). We declare it with C linkage.

#include <iostream>
#include <cmath>
#include <testcms2.h>


// Declare the focal function with C linkage to ensure proper name mangling during linking.
extern "C" double Rec709(double L);

// Utility: compare two doubles with a small tolerance
static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Test 1: True branch - L < 0.018 should return 4.5 * L
// This verifies the lower linear segment of Rec709.
bool test_Rec709_below_threshold_true_branch() {
    double L = 0.01; // strictly below threshold
    double expected = 4.5 * L;
    double actual = Rec709(L);
    return almost_equal(expected, actual);
}

// Test 2: False branch threshold boundary - L == 0.018 should use the nonlinear branch
// This verifies the switch to the nonlinear segment exactly at the threshold.
bool test_Rec709_at_threshold_false_branch() {
    double L = 0.018; // exactly at threshold; should use nonlinear path
    double expected = 1.099 * std::pow(L, 0.45) - 0.099;
    double actual = Rec709(L);
    return almost_equal(expected, actual);
}

// Test 3: Unity-like input - L = 1.0 should yield 1.0 (since pow(1,0.45)=1 and 1.099-0.099=1.0)
// This confirms the upper branch computes correctly for a simple input.
bool test_Rec709_at_unity_input() {
    double L = 1.0;
    double expected = 1.0;
    double actual = Rec709(L);
    return almost_equal(expected, actual);
}

// Test 4: Negative input - L < 0.018 uses the linear branch for negatives as well
// This checks that negative small values produce the expected linear result.
bool test_Rec709_negative_input() {
    double L = -0.5;
    double expected = 4.5 * L; // -2.25
    double actual = Rec709(L);
    return almost_equal(expected, actual);
}

// Test 5: Generic value in nonlinear region - L = 0.5
// Verifies correctness of the nonlinear computation for a mid-range input.
bool test_Rec709_generic_value() {
    double L = 0.5;
    double expected = 1.099 * std::pow(L, 0.45) - 0.099;
    double actual = Rec709(L);
    return almost_equal(expected, actual);
}

// Test 6: Large input in nonlinear region - L = 2.0
// Ensures the nonlinear path handles larger values correctly.
bool test_Rec709_large_value() {
    double L = 2.0;
    double expected = 1.099 * std::pow(L, 0.45) - 0.099;
    double actual = Rec709(L);
    return almost_equal(expected, actual);
}

// Simple test harness
int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool result) {
        ++total;
        if (result) {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    // Explanatory comments accompany each test invocation
    run("Rec709_below_threshold_true_branch", test_Rec709_below_threshold_true_branch());
    run("Rec709_at_threshold_false_branch", test_Rec709_at_threshold_false_branch());
    run("Rec709_at_unity_input", test_Rec709_at_unity_input());
    run("Rec709_negative_input", test_Rec709_negative_input());
    run("Rec709_generic_value", test_Rec709_generic_value());
    run("Rec709_large_value", test_Rec709_large_value());

    // Summary
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}