// Test harness for cmsFloat64Number computeFL(cmsCIECAM02* pMod)
// This test does not rely on GTest/GMock. It uses a lightweight custom assertion
// framework to maximize code coverage while keeping the test executable self-contained.

#include <cstring>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <cmath>


// Include the internal CMS headers to access the cmsCIECAM02 type and related
// definitions. This mirrors the dependencies declared in the focal class file.
// The test assumes the environment provides cmsFloat64Number and the function
// prototype for computeFL with C linkage.

extern "C" {
    // Prototypes for the focal method under test.
    cmsFloat64Number computeFL(cmsCIECAM02* pMod);
}

// Lightweight test harness (non-terminating assertions)
static int g_totalTests = 0;
static int g_passedTests = 0;

// Custom assertion helper: checks double equality within a tolerance
static void expect_double_close(const char* testName, double got, double expected, double tol = 1e-12) {
    ++g_totalTests;
    if (std::fabs(got - expected) <= tol) {
        std::cout << "[PASS] " << testName << "\n";
        ++g_passedTests;
    } else {
        std::cerr << std::fixed << std::setprecision(12)
                  << "[FAIL] " << testName
                  << ": got " << got << ", expected " << expected
                  << ", diff " << std::fabs(got - expected) << "\n";
    }
}

// Independent, reference computation of the expected FL value for a given LA.
// This mirrors the exact formula used in computeFL to avoid test fragility.
static double compute_expected_FL(double LA) {
    double k = 1.0 / ((5.0 * LA) + 1.0);
    double term1 = 0.2 * std::pow(k, 4.0) * (5.0 * LA);
    double term2 = 0.1 * std::pow((1.0 - std::pow(k, 4.0)), 2.0) *
                   std::pow((5.0 * LA), (1.0 / 3.0));
    return term1 + term2;
}

// Test case: verify computeFL for a specific LA value.
// This encapsulates a single scenario with a descriptive name.
static void test_FL_for_LA(double LA) {
    // Prepare a cmsCIECAM02 instance with LA initialized.
    cmsCIECAM02 mod;
    std::memset(&mod, 0, sizeof(mod)); // Zero-initialize to avoid uninitialized fields
    mod.LA = LA;

    // Call the focal method (C linkage)
    cmsFloat64Number result = computeFL(&mod);

    // Expected value computed independently
    double expected = compute_expected_FL(LA);

    // Use a reasonably tight tolerance to account for floating-point differences
    expect_double_close("computeFL_LA=" + std::to_string(LA), static_cast<double>(result), expected, 1e-9);
}

// Run all test cases to maximize coverage
static void run_all_tests() {
    // True branch coverage is not applicable since computeFL contains no branching,
    // but we ensure values across a range are exercised.

    // Edge case: LA = 0 -> k = 1, FL should be 0
    test_FL_for_LA(0.0);

    // Negative but non-singular LA to exercise non-positive FLOP region
    test_FL_for_LA(-0.05);

    // Moderate positive LA
    test_FL_for_LA(10.0);

    // Larger LA
    test_FL_for_LA(50.0);

    // Very large LA to exercise numerical stability
    test_FL_for_LA(100.0);
}

int main() {
    run_all_tests();

    // Summary
    std::cout << "\nTest Summary: " << g_passedTests << " / " << g_totalTests << " passed.\n";
    return (g_totalTests == g_passedTests) ? 0 : 1;
}