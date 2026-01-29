// Test suite for CAM02COLOR NonlinearCompression in cmscam02.c
// Notes:
// - This test harness targets the NonlinearCompression function.
// - It relies on the project providing the original types CAM02COLOR and cmsCIECAM02
//   and the function NonlinearCompression(CAM02COLOR clr, cmsCIECAM02* pMod).
// - The tests exercise true/false branches of the RGBp sign predicate by providing
//   positive, negative, and mixed RGBp inputs.
// - Floating-point results are compared using a tolerance to account for numeric precision.
// - The test suite uses a lightweight, non-terminating assertion style (no GTest).
// - Compile with C++11, linking against the project's C sources (e.g., cmscam02.c).

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cmath>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include the library header that defines CAM02COLOR, cmsCIECAM02, and NonlinearCompression.
// The exact header name may vary by project setup; adjust the path if needed.
// This header is expected to declare the types used by the focal method.
extern "C" {
}

// If the project uses a different header name, uncomment and adjust the following line accordingly:
// #include "cmscam02.h"

using std::cerr;
using std::cout;
using std::fabs;
using std::string;

// Tolerance for floating point comparisons
const double EPS = 1e-6;

// Lightweight non-terminating assertion macro
static int g_test_failures = 0;
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { \
    cerr << "EXPECT_TRUE failed: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
    ++g_test_failures; \
} } while(0)

#define EXPECT_NEAR(expected, actual, tol, msg) do { \
    double a = (double)(actual); double e = (double)(expected); \
    if (fabs(a - e) > (tol)) { \
        cerr << "EXPECT_NEAR failed: " << (msg) << " | expected=" << e << ", actual=" << a \
             << ", tol=" << (tol) << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_test_failures; \
    } \
} while(0)

// Helper to run a test function and print its name
#define RUN_TEST(name) do { \
    bool ok = (name)(); \
    if (ok) { \
        cout << "[PASSED] " << #name << "\n"; \
    } else { \
        cout << "[FAILED] " << #name << "\n"; \
    } \
} while(0)

// Compute expected results for NonlinearCompression given input clrIn and mod
static CAM02COLOR compute_expected_NonlinearCompression(const CAM02COLOR& clrIn, const cmsCIECAM02& mod) {
    CAM02COLOR exp = clrIn; // We'll fill RGBpa and A as expected
    for (uint32_t i = 0; i < 3; ++i) {
        double temp;
        if (clrIn.RGBp[i] < 0) {
            temp = pow((-1.0 * mod.FL * clrIn.RGBp[i] / 100.0), 0.42);
            exp.RGBpa[i] = (-1.0 * 400.0 * temp) / (temp + 27.13) + 0.1;
        } else {
            temp = pow((mod.FL * clrIn.RGBp[i] / 100.0), 0.42);
            exp.RGBpa[i] = (400.0 * temp) / (temp + 27.13) + 0.1;
        }
    }
    exp.A = (((2.0 * exp.RGBpa[0]) + exp.RGBpa[1] +
              (exp.RGBpa[2] / 20.0)) - 0.305) * mod.Nbb;
    return exp;
}

// Test 1: All RGBp non-negative -> positive branch in NonlinearCompression
bool test_NonlinearCompression_PositiveBranch() {
    // Prepare input
    CAM02COLOR clr;
    clr.RGBp[0] = 50.0;
    clr.RGBp[1] = 100.0;
    clr.RGBp[2] = 25.0;

    // Output containers
    CAM02COLOR result;

    // Mod parameters
    cmsCIECAM02 mod;
    // Initialize with values that exercise the positive branch
    mod.FL = 1.0;
    mod.Nbb = 1.0;

    // Copy input to preserve for expected computation
    CAM02COLOR input = clr;

    // Call the function under test
    result = NonlinearCompression(clr, &mod);

    // Compute expected values
    CAM02COLOR exp = compute_expected_NonlinearCompression(input, mod);

    // Assertions on RGBpa
    for (int i = 0; i < 3; ++i) {
        string msg = "RGBpa[" + std::to_string(i) + "] mismatch (PositiveBranch)";
        EXPECT_NEAR(exp.RGBpa[i], result.RGBpa[i], EPS, msg);
    }
    // A (luminance-like component)
    {
        string msg = "A mismatch (PositiveBranch)";
        EXPECT_NEAR(exp.A, result.A, EPS, msg);
    }

    // Return true if no failures
    return g_test_failures == 0;
}

// Test 2: All RGBp negative -> negative branch in NonlinearCompression
bool test_NonlinearCompression_NegativeBranch() {
    CAM02COLOR clr;
    clr.RGBp[0] = -40.0;
    clr.RGBp[1] = -80.0;
    clr.RGBp[2] = -10.0;

    cmsCIECAM02 mod;
    mod.FL = 0.75;
    mod.Nbb = 1.25;

    CAM02COLOR input = clr;

    CAM02COLOR result = NonlinearCompression(clr, &mod);

    CAM02COLOR exp = compute_expected_NonlinearCompression(input, mod);

    for (int i = 0; i < 3; ++i) {
        string msg = "RGBpa[" + std::to_string(i) + "] mismatch (NegativeBranch)";
        EXPECT_NEAR(exp.RGBpa[i], result.RGBpa[i], EPS, msg);
    }
    {
        string msg = "A mismatch (NegativeBranch)";
        EXPECT_NEAR(exp.A, result.A, EPS, msg);
    }

    return g_test_failures == 0;
}

// Test 3: Mixed signs in RGBp to cover both branches within single invocation
bool test_NonlinearCompression_MixedBranch() {
    CAM02COLOR clr;
    clr.RGBp[0] = -30.0;
    clr.RGBp[1] = 60.0;
    clr.RGBp[2] = -5.0;

    cmsCIECAM02 mod;
    mod.FL = 1.2;
    mod.Nbb = 0.9;

    CAM02COLOR input = clr;

    CAM02COLOR result = NonlinearCompression(clr, &mod);

    CAM02COLOR exp = compute_expected_NonlinearCompression(input, mod);

    for (int i = 0; i < 3; ++i) {
        string msg = "RGBpa[" + std::to_string(i) + "] mismatch (MixedBranch)";
        EXPECT_NEAR(exp.RGBpa[i], result.RGBpa[i], EPS, msg);
    }
    {
        string msg = "A mismatch (MixedBranch)";
        EXPECT_NEAR(exp.A, result.A, EPS, msg);
    }

    return g_test_failures == 0;
}

// Entry point for test runner
int main() {
    cout << "Running NonlinearCompression tests for CAM02COLOR...\n";

    RUN_TEST(test_NonlinearCompression_PositiveBranch);
    RUN_TEST(test_NonlinearCompression_NegativeBranch);
    RUN_TEST(test_NonlinearCompression_MixedBranch);

    if (g_test_failures == 0) {
        cout << "All tests PASSED.\n";
        return 0;
    } else {
        cout << g_test_failures << " test(s) FAILED.\n";
        return 1;
    }
}