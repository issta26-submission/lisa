/*
Step 1: Program Understanding (cited in comments)
- FOCAL_METHOD: cmsFloat32Number sigmoidal(cmsFloat32Number x, const cmsFloat64Number Params[])
  Val = pow(1.0 - pow(1 - x, 1/Params[0]), 1/Params[0]);
  return (cmsFloat32Number) Val;
- Core dependencies: cmsFloat32Number (float), cmsFloat64Number (double), Params[] (array of doubles).
- Domain considerations:
  - Params[0] must be > 0 to avoid division by zero.
  - For x in [0,1], the expression is well-defined for Params[0] > 0.
  - For x > 1, inner pow uses a negative base with fractional exponent; result may be NaN.
- Test objective: verify correctness for representative positive Params[0] values, including edge cases x=0 and x=1, and behavior for out-of-domain x (NaN propagation).

Candidate Keywords (core dependent components)
- sigmoidal, cmsFloat32Number, cmsFloat64Number, Params, pow, x
- Exponent 1/Params[0], outer pow with same exponent
- Domain: Params[0] > 0, x in [0,1], boundary cases x=0, x=1
- NaN/undefined behavior when x is out of domain (e.g., x > 1)

Step 2: Unit Test Generation
This test suite targets the focal function in testcms2.c by linking against it from C++11, avoiding GTest, and using a lightweight custom harness to maximize coverage and execution.

Notes about dependencies:
- We rely on the provided testcms2.h (as included in the FOCAL_CLASS_DEP) for type aliases (cmsFloat32Number, cmsFloat64Number, etc.) and the sigmoidal function prototype.
- The tests are written in C++11, invoked from main(), and do not depend on private members or private helpers.

Step 3: Test Case Refinement
- Coverage across representative Params[0] values: 1.0, 2.0, 0.5.
- Coverage of boundary inputs: x=0 and x=1 (should yield 0 and 1 respectively for Params>0).
- Coverage of out-of-domain input: x > 1 (expect NaN).
- Non-terminating assertions: implemented as a lightweight test harness that records failures but continues execution.
- All tests are annotated with explanatory comments describing intent.

Code: (CTest harness in C++11 calling the C function sigmoidal)

*/

#include <limits>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cmath>


// Declare the C function to ensure linkage from C++
extern "C" {
    cmsFloat32Number sigmoidal(cmsFloat32Number x, const cmsFloat64Number Params[]);
}

/*
Lightweight test harness
- Does not abort on failure (non-terminating).
- Records the total and failed tests and prints a summary.
- Uses a tolerant floating-point comparison with a specified tolerance.
*/

// Global counters for test results
static int g_totalTests = 0;
static int g_failedTests = 0;

// Utility: approximate equality for floating-point numbers
static bool approxEqualDouble(double a, double b, double tol) {
    // Handle NaN and infinities explicitly
    if (std::isnan(a) && std::isnan(b)) return true;
    if (std::isinf(a) || std::isinf(b)) return (a == b);
    return std::fabs(a - b) <= tol;
}

// Custom expectation: compare actual (cmsFloat32Number) to expected (double)
#define EXPECT_APPROX_EQ(actual, expected, tol, note) do {                          \
    double a = static_cast<double>(actual);                                         \
    double e = (expected);                                                            \
    if (!approxEqualDouble(a, e, (tol))) {                                         \
        std::cerr << "  [FAIL] " << (note) << "\n";                                 \
        std::cerr << "         Expected ~" << (e) << ", got " << (a)             \
                  << " (tol " << (tol) << ")\n";                                   \
        ++g_failedTests;                                                            \
    } else {                                                                        \
        std::cout << "  [PASS] " << (note) << "\n";                                 \
    }                                                                               \
} while(0)

// Test harness runner: registers a test, runs it, reports result
static void RunTest(const std::string& testName, const std::function<void()>& fn) {
    std::cout << "Running test: " << testName << std::endl;
    // Before running the test, record the start
    try {
        fn();
    } catch (...) {
        std::cerr << "  [EXCEPTION] Test threw an uncaught exception: " << testName << "\n";
        ++g_failedTests;
    }
    ++g_totalTests;
}

// Helper: compute the expected value using the same mathematical definition in double precision.
// This ensures independent verification against the implementation under test.
static double ExpectedSigmoidal(double x, double p0) {
    return std::pow(1.0 - std::pow(1.0 - x, 1.0 / p0), 1.0 / p0);
}

int main(int argc, char* argv[]) {
    // 1) True case: Params[0] = 1.0, x in (0,1) => sigmoidal(x) should equal x exactly (as per identity).
    RunTest("sigmoidal_Param1_X0_25_ReturnsX", [](){
        cmsFloat64Number Params[1] = {1.0};  // 1/Params[0] = 1.0
        cmsFloat32Number x = 0.25f;
        cmsFloat32Number result = sigmoidal(x, Params);
        double expected = 0.25; // by algebra: pow(1 - (1 - x)^1, 1) = x
        EXPECT_APPROX_EQ(result, expected, 1e-6, "sigmoidal(x=0.25, Params[0]=1.0) should equal 0.25");
    });

    // 2) Parameter > 1: Params[0] = 2.0, x = 0.5
    // Expected value computed independently in double precision.
    RunTest("sigmoidal_Param2_X0_5", [](){
        cmsFloat64Number Params[1] = {2.0};
        cmsFloat32Number x = 0.5f;
        cmsFloat32Number result = sigmoidal(x, Params);
        double expected = ExpectedSigmoidal(0.5, 2.0);
        EXPECT_APPROX_EQ(result, expected, 1e-6, "sigmoidal(x=0.5, Params[0]=2.0)");
    });

    // 3) Parameter < 1: Params[0] = 0.5, x = 0.75
    RunTest("sigmoidal_Param05_X0_75", [](){
        cmsFloat64Number Params[1] = {0.5};
        cmsFloat32Number x = 0.75f;
        cmsFloat32Number result = sigmoidal(x, Params);
        double p = 0.5;
        double expected = ExpectedSigmoidal(0.75, p);
        EXPECT_APPROX_EQ(result, expected, 1e-5, "sigmoidal(x=0.75, Params[0]=0.5)");
    });

    // 4) Boundary case: x = 0 => result should be 0 for any Params[0] > 0
    RunTest("sigmoidal_X0_Boundary", [](){
        cmsFloat64Number Params[1] = {3.0};
        cmsFloat32Number x = 0.0f;
        cmsFloat32Number result = sigmoidal(x, Params);
        double expected = 0.0;
        EXPECT_APPROX_EQ(result, expected, 1e-6, "sigmoidal(x=0, Params[0]=3.0) should be 0");
    });

    // 5) Boundary case: x = 1 => result should be 1 for any Params[0] > 0
    RunTest("sigmoidal_X1_Boundary", [](){
        cmsFloat64Number Params[1] = {0.3};
        cmsFloat32Number x = 1.0f;
        cmsFloat32Number result = sigmoidal(x, Params);
        double expected = 1.0;
        EXPECT_APPROX_EQ(result, expected, 1e-6, "sigmoidal(x=1, Params[0]=0.3) should be 1");
    });

    // 6) Out-of-domain input: x > 1, expect NaN propagation or domain error results
    RunTest("sigmoidal_XOutOfDomain_NaN", [](){
        cmsFloat64Number Params[1] = {2.0};
        cmsFloat32Number x = 1.1f; // 1 - x is negative; with fractional exponent, result may be NaN
        cmsFloat32Number result = sigmoidal(x, Params);
        if (std::isnan(static_cast<double>(result))) {
            std::cout << "  [PASS] sigmoidal out-of-domain produced NaN as expected\n";
        } else {
            std::cerr << "  [FAIL] sigmoidal out-of-domain did not produce NaN; value: " << static_cast<double>(result) << "\n";
            ++g_failedTests;
        }
    });

    // 7) Additional coverage: verify that multiple consecutive calls with varying inputs do not crash and produce finite outputs where defined
    RunTest("sigmoidal_MultiCall_VariousInputs", [](){
        cmsFloat64Number Params[1] = {1.5};
        std::vector<double> xs = {0.0, 0.1, 0.3, 0.9, 1.0};
        bool all_ok = true;
        for (double xv : xs) {
            cmsFloat32Number x = static_cast<cmsFloat32Number>(xv);
            cmsFloat32Number r = sigmoidal(x, Params);
            // For x in [0,1], result should be within [0,1] due to the nature of the function
            if (!(static_cast<double>(r) >= 0.0 && static_cast<double>(r) <= 1.0)) {
                all_ok = false;
                std::cerr << "    - Value out of range for x=" << xv << ": result=" << static_cast<double>(r) << "\n";
            }
        }
        if (all_ok) {
            std::cout << "  [PASS] sigmoidal_MultiCall_VariousInputs produced values in [0,1] for x in [0,1]\n";
        } else {
            ++g_failedTests;
        }
    });

    // Summary
    std::cout << "\nTest Summary: " << g_totalTests << " tests run, " << g_failedTests << " failures.\n";

    // Return non-zero if any test failed (conventional for test harnesss)
    return (g_failedTests == 0) ? 0 : 1;
}