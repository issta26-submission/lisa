/*
  Test suite for the focal method:
  cmsInt32Number CheckGammaWordTable(cmsFloat64Number g)

  Step 1 (brief keywords that guided test design):
  - pow, 1024/1025 sampling grid, 65535 scaling
  - Values[] array, Tone Curve creation via cmsBuildTabulatedToneCurve16
  - cmsEvalToneCurveFloat, gamma comparison via pow(in, g)
  - MaxErr tracking, optional print of |Err|<MaxErr, and CheckGammaEstimation(Curve, g)
  - Curve cleanup with cmsFreeToneCurve, DbgThread context
  - Boundary coverage via g values around typical gamma (1.0, 2.2, etc.)

  Step 2: We rely on the external C test harness defined in testcms2.h
  and expose the C linkage function:
     cmsInt32Number CheckGammaWordTable(cmsFloat64Number g);

  Step 3: The tests are implemented in C++11, without GoogleTest, and
  call the C function directly from main. Each test prints a short
  explanatory message and is designed to execute under a standard
  build that links testcms2.c/.h alongside this test file.
*/

#include <iostream>
#include <string>
#include <testcms2.h>


// Include the test harness/types from the project (assumed to exist in the environment)

// Declare the focal function with C linkage (as provided by the focal file)
extern "C" cmsInt32Number CheckGammaWordTable(cmsFloat64Number g);

// Simple test harness helpers
struct TestResult {
    std::string name;
    bool passed;
    std::string note;
};

// Wrapper to run a single gamma test and record result
static TestResult RunGammaWordTableTest(double gamma, const std::string& testName, const std::string& note) {
    // The core functionality is exercised here
    cmsInt32Number result = CheckGammaWordTable(static_cast<cmsFloat64Number>(gamma));

    bool ok = (result == 1);

    TestResult tr;
    tr.name = testName;
    tr.passed = ok;
    tr.note = note;
    if (ok) {
        std::cout << "[PASS] " << testName << " (gamma=" << gamma << "): " << note << "\n";
    } else {
        std::cerr << "[FAIL] " << testName << " (gamma=" << gamma << "): " << note << "\n";
    }
    return tr;
}

// Entry points for individual tests with explanatory comments

// Test 1: Identity gamma (g = 1.0) - should yield a straight line behavior
// This covers the "gamma = 1" edge case where pow(in, 1.0) = in.
static TestResult Test_GammaIdentity(void) {
    // Expected: CheckGammaWordTable returns 1 (success)
    return RunGammaWordTableTest(1.0, "Test_GammaIdentity", "Ensures identity gamma is handled correctly; exercises the 1.0 edge case.");
}

// Test 2: Common perceptual gamma (g ≈ 2.2) - typical sRGB-like gamma
// This should exercise the common non-linear gamma path and CheckGammaEstimation.
static TestResult Test_Gamma_2_2(void) {
    return RunGammaWordTableTest(2.2, "Test_Gamma_2_2", "Represents standard gamma usage (approximate sRGB gamma).");
}

// Test 3: Inverse gamma (g ≈ 0.4545) - reciprocal of 2.2
// This tests a gamma value less than 1, ensuring proper handling of inverse relationship.
static TestResult Test_Gamma_0_4545(void) {
    return RunGammaWordTableTest(0.4545, "Test_Gamma_0_4545", "Inverse gamma case; ensures lower-than-one gamma is processed.");
}

// Test 4: Higher gamma value (g = 4.0) - stress test for strong non-linearity
// This helps exercise the upper-range behavior of the algorithm and the
// MaxErr reporting path, while still relying on CheckGammaEstimation.
static TestResult Test_Gamma_4_0(void) {
    return RunGammaWordTableTest(4.0, "Test_Gamma_4_0", "High gamma value to stress non-linearity handling.");
}

// Test 5: Small gamma (g = 0.1) - very gentle curve
// Verifies behavior for near-linear but still non-trivial gamma curve.
static TestResult Test_Gamma_0_1(void) {
    return RunGammaWordTableTest(0.1, "Test_Gamma_0_1", "Small gamma value to verify gentle curvature handling.");
}

// Main entry point: run all tests and report summary
int main(void) {
    std::cout << "Starting tests for: cmsInt32Number CheckGammaWordTable(cmsFloat64Number g)\n";

    TestResult r1 = Test_GammaIdentity();
    TestResult r2 = Test_Gamma_2_2();
    TestResult r3 = Test_Gamma_0_4545();
    TestResult r4 = Test_Gamma_4_0();
    TestResult r5 = Test_Gamma_0_1();

    // Summary
    int failures = 0;
    auto count_failures = [&](const TestResult& tr) {
        if (!tr.passed) ++failures;
    };
    count_failures(r1);
    count_failures(r2);
    count_failures(r3);
    count_failures(r4);
    count_failures(r5);

    std::cout << "\nTest summary:\n";
    std::cout << "Total tests run: 5\n";
    std::cout << "Failures: " << failures << "\n";

    if (failures > 0) {
        std::cerr << "Some gamma word table tests failed. See above for details.\n";
        return 1;
    } else {
        std::cout << "All gamma word table tests passed.\n";
        return 0;
    }
}