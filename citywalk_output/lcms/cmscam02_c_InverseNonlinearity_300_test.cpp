/*
Unit test suite for the focal method: InverseNonlinearity in cmscam02.c

Step-by-step notes:
- The tests exercise the function CAM02COLOR InverseNonlinearity(CAM02COLOR clr, cmsCIECAM02* pMod)
- Core behavior: For i = 0..2, compute sign c1 from (clr.RGBpa[i] - 0.1) < 0, and set
  clr.RGBp[i] = c1 * (100.0 / pMod->FL) *
              pow(((27.13 * fabs(clr.RGBpa[i] - 0.1)) / (400.0 - fabs(clr.RGBpa[i] - 0.1))),
                  (1.0 / 0.42));
  The function returns the modified clr, leaving clr.RGBpa intact.

Candidate Keywords (Step 1):
- InverseNonlinearity, RGBpa, RGBp, FL
- fabs, pow, 0.1, 27.13, 400.0
- 3-channel loop, sign branch, pMod->FL usage

Dependencies (Step 2):
- We rely on the definitions of CAM02COLOR and cmsCIECAM02 from the CMS headers (lcms2_internal.h).
- The function is non-static and externally callable (extern "C" for linkage safety in C++ tests).

Non-terminating assertions (Step 3):
- We implement a lightweight test harness that reports pass/fail without aborting the process.
- Each test computes expected values using the exact same formula, ensuring robust numerical comparison with a small epsilon.

Notes:
- The test uses direct function prototype for InverseNonlinearity via extern "C" to avoid C++ name mangling.
- We do not mock any private/static internals; we exercise the public-like function signature.
- The tests cover:
  - Branches of (clr.RGBpa[i] - 0.1) < 0 (negative) vs >= 0 (positive)
  - Proper preservation of RGBpa
  - Effect of varying FL on the computed output
*/

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cmath>


// Import CMS internal types (CAM02COLOR, cmsCIECAM02, etc.)

// Prototype for the focal function under test (external linkage)
extern "C" CAM02COLOR InverseNonlinearity(CAM02COLOR clr, cmsCIECAM02* pMod);

// Simple non-terminating assertion helpers
static int g_failures = 0;

static void log_failure(const std::string& test_name, const std::string& message) {
    ++g_failures;
    std::cerr << "FAIL: " << test_name << " - " << message << "\n";
}

static void log_success(const std::string& test_name) {
    std::cout << "PASS: " << test_name << "\n";
}

// Helper to compute the expected value for a given channel
static double expected_inverse_nonlinearity_component(double rgbpa, double FL) {
    const double diffAbs = std::fabs(rgbpa - 0.1);
    const double c1 = ((rgbpa - 0.1) < 0.0) ? -1.0 : 1.0;
    const double inner = (27.13 * diffAbs) / (400.0 - diffAbs);
    const double exponent = 1.0 / 0.42;
    return c1 * (100.0 / FL) * std::pow(inner, exponent);
}

// Test 1: Branch coverage for mixed RGBpa values (some negatives, some non-negatives)
static void test_InverseNonlinearity_BranchSigns_Mixed() {
    const std::string test_name = "InverseNonlinearity_BranchSigns_Mixed";
    cmsCIECAM02 mod;
    mod.FL = 10.0; // Avoid division by zero; choose a representative scaling

    CAM02COLOR clr;
    // RGBpa values chosen to exercise both branches:
    // 0.0  -> diff = -0.1  => c1 = -1
    // 0.1  -> diff = 0.0   => c1 = 1
    // 0.2  -> diff = 0.1   => c1 = 1
    clr.RGBpa[0] = 0.0; clr.RGBpa[1] = 0.1; clr.RGBpa[2] = 0.2;
    // Prepare output containers
    clr.RGBp[0] = clr.RGBp[1] = clr.RGBp[2] = 0.0;

    CAM02COLOR out = InverseNonlinearity(clr, &mod);

    bool pass = true;
    for (int i = 0; i < 3; ++i) {
        double expected = expected_inverse_nonlinearity_component(clr.RGBpa[i], mod.FL);
        double actual   = out.RGBp[i];
        const double eps = 1e-6;
        if (std::fabs(actual - expected) > eps) {
            pass = false;
            log_failure(test_name, "Channel " + std::to_string(i) +
                       " mismatch: expected " + std::to_string(expected) +
                       ", got " + std::to_string(actual));
        }
        // RGBpa should remain unchanged
        if (std::fabs(out.RGBpa[i] - clr.RGBpa[i]) > 1e-12) {
            pass = false;
            log_failure(test_name, "RGBpa changed for channel " + std::to_string(i));
        }
    }

    if (pass) log_success(test_name);
}

// Test 2: All RGBpa values > 0.1 -> all c1 = 1
static void test_InverseNonlinearity_BranchSigns_AllPositive() {
    const std::string test_name = "InverseNonlinearity_BranchSigns_AllPositive";
    cmsCIECAM02 mod;
    mod.FL = 7.5;

    CAM02COLOR clr;
    clr.RGBpa[0] = 0.2; clr.RGBpa[1] = 0.3; clr.RGBpa[2] = 0.4;
    clr.RGBp[0] = clr.RGBp[1] = clr.RGBp[2] = 0.0;

    CAM02COLOR out = InverseNonlinearity(clr, &mod);

    bool pass = true;
    for (int i = 0; i < 3; ++i) {
        double expected = expected_inverse_nonlinearity_component(clr.RGBpa[i], mod.FL);
        double actual   = out.RGBp[i];
        const double eps = 1e-6;
        if (std::fabs(actual - expected) > eps) {
            pass = false;
            log_failure(test_name, "Channel " + std::to_string(i) +
                       " mismatch: expected " + std::to_string(expected) +
                       ", got " + std::to_string(actual));
        }
        // RGBpa unchanged
        if (std::fabs(out.RGBpa[i] - clr.RGBpa[i]) > 1e-12) {
            pass = false;
            log_failure(test_name, "RGBpa changed for channel " + std::to_string(i));
        }
    }

    if (pass) log_success(test_name);
}

// Test 3: All RGBpa values < 0.1 -> all c1 = -1
static void test_InverseNonlinearity_BranchSigns_AllNegative() {
    const std::string test_name = "InverseNonlinearity_BranchSigns_AllNegative";
    cmsCIECAM02 mod;
    mod.FL = 12.0;

    CAM02COLOR clr;
    clr.RGBpa[0] = 0.0; clr.RGBpa[1] = 0.05; clr.RGBpa[2] = 0.09;
    clr.RGBp[0] = clr.RGBp[1] = clr.RGBp[2] = 0.0;

    CAM02COLOR out = InverseNonlinearity(clr, &mod);

    bool pass = true;
    for (int i = 0; i < 3; ++i) {
        double expected = expected_inverse_nonlinearity_component(clr.RGBpa[i], mod.FL);
        double actual   = out.RGBp[i];
        const double eps = 1e-6;
        if (std::fabs(actual - expected) > eps) {
            pass = false;
            log_failure(test_name, "Channel " + std::to_string(i) +
                       " mismatch: expected " + std::to_string(expected) +
                       ", got " + std::to_string(actual));
        }
        // RGBpa unchanged
        if (std::fabs(out.RGBpa[i] - clr.RGBpa[i]) > 1e-12) {
            pass = false;
            log_failure(test_name, "RGBpa changed for channel " + std::to_string(i));
        }
    }

    if (pass) log_success(test_name);
}

// Test 4: Verify that changing FL scales the output inversely (consistency across FL)
static void test_InverseNonlinearity_FlowScalingConsistency() {
    const std::string test_name = "InverseNonlinearity_FlowScalingConsistency";
    // Two different FL values, same RGBpa values
    const double FL1 = 5.0;
    const double FL2 = 20.0;

    cmsCIECAM02 mod1; mod1.FL = FL1;
    cmsCIECAM02 mod2; mod2.FL = FL2;

    CAM02COLOR clr;
    clr.RGBpa[0] = 0.25; clr.RGBpa[1] = 0.15; clr.RGBpa[2] = 0.35;
    clr.RGBp[0] = clr.RGBp[1] = clr.RGBp[2] = 0.0;

    CAM02COLOR out1 = InverseNonlinearity(clr, &mod1);
    CAM02COLOR out2 = InverseNonlinearity(clr, &mod2);

    bool pass = true;
    for (int i = 0; i < 3; ++i) {
        // Expect outputs to scale roughly with 100/FL
        double expected1 = expected_inverse_nonlinearity_component(clr.RGBpa[i], mod1.FL);
        double expected2 = expected_inverse_nonlinearity_component(clr.RGBpa[i], mod2.FL);
        // Compare each against its own computed expectation
        if (std::fabs(out1.RGBp[i] - expected1) > 1e-6) {
            pass = false;
            log_failure(test_name, "FL1 channel " + std::to_string(i) +
                       " mismatch: expected " + std::to_string(expected1) +
                       ", got " + std::to_string(out1.RGBp[i]));
        }
        if (std::fabs(out2.RGBp[i] - expected2) > 1e-6) {
            pass = false;
            log_failure(test_name, "FL2 channel " + std::to_string(i) +
                       " mismatch: expected " + std::to_string(expected2) +
                       ", got " + std::to_string(out2.RGBp[i]));
        }
        // RGBpa unchanged in both cases
        if (std::fabs(out1.RGBpa[i] - clr.RGBpa[i]) > 1e-12 ||
            std::fabs(out2.RGBpa[i] - clr.RGBpa[i]) > 1e-12) {
            pass = false;
            log_failure(test_name, "RGBpa changed for channel " + std::to_string(i));
        }
    }

    if (pass) log_success(test_name);
}

// Main function to drive all tests
int main() {
    std::cout << "Starting InverseNonlinearity unit tests (CMS CAM02) ...\n";

    test_InverseNonlinearity_BranchSigns_Mixed();
    test_InverseNonlinearity_BranchSigns_AllPositive();
    test_InverseNonlinearity_BranchSigns_AllNegative();
    test_InverseNonlinearity_FlowScalingConsistency();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests PASSED.\n";
    } else {
        std::cerr << "Total failures: " << g_failures << "\n";
    }
    return g_failures == 0 ? 0 : 1;
}