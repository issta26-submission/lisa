/*
Test Suite for the focal method: CheckJoint16Curves_sRGB (in testcms2.c)

Overview:
- Step 1: Identify candidate dependencies used by the focal method.
  Key elements (Candidate Keywords): Forward (Build_sRGBGamma), Reverse (cmsReverseToneCurve), Result (CombineGamma16),
  Check (cmsIsToneCurveLinear), Cleanup (cmsFreeToneCurve), and the involved tone curves (Forward, Reverse, Result).

- Step 2: Build a small, executable C++11 test harness (no GTest) that exercises:
  1) The focal function CheckJoint16Curves_sRGB (returns non-zero on success).
  2) Core dependent functions directly to improve coverage:
     - Build_sRGBGamma returns non-null and is non-linear (cmsIsToneCurveLinear == false).
     - cmsReverseToneCurve and CombineGamma16 produce a linear result when chained together.

- Step 3: Test Case Refinement
  - Provide multiple non-terminating assertions within each test.
  - Ensure proper memory management by freeing tone curves in all paths.
  - Compile under C++11, link with the library providing testcms2.h, and wrap the C header for C++ usage.

Notes:
- This file uses a minimal, non-GTest test harness suitable for C++11 builds.
- Test functions are designed to be safe if underlying library calls fail (null checks).
- The tests aim to cover both true and false branches of the tone-curve linearity predicate via independent tests.

Usage:
- Compile this test file as part of your test suite (ensure testcms2.h and the library are in the include/link path).
- The main function runs three tests and prints a concise summary.

*/

// Include necessary standard library headers for I/O and simple test scaffolding
#include <iostream>
#include <testcms2.h>
#include <functional>


// If the CMS library exposes C API via testcms2.h, include it with C linkage
extern "C" {
}

// Test 1: Validate that the focal function CheckJoint16Curves_sRGB returns a non-zero value
// This confirms that the combination of forward & reverse gamma curves yields a "linear-ish" result
// as defined by cmsIsToneCurveLinear in the focal routine.
bool test_CheckJoint16Curves_sRGB_ReturnsTrue() {
    cmsInt32Number rc = CheckJoint16Curves_sRGB();
    bool ok = (rc != 0);
    if (!ok) {
        std::cerr << "FAIL: CheckJoint16Curves_sRGB() expected non-zero (true), got 0." << std::endl;
    } else {
        std::cout << "PASS: CheckJoint16Curves_sRGB_ReturnsTrue" << std::endl;
    }
    return ok;
}

// Test 2: Verify that Build_sRGBGamma() returns a non-null tone curve and that it is non-linear.
// The sRGB gamma curve should be non-linear; cmsIsToneCurveLinear should return false for Forward.
bool test_sRGB_Gamma_Is_Not_Linear() {
    cmsToneCurve* Forward = Build_sRGBGamma();
    if (Forward == nullptr) {
        std::cerr << "FAIL: Build_sRGBGamma() returned null." << std::endl;
        return false;
    }
    cmsBool isLinear = cmsIsToneCurveLinear(Forward);
    bool ok = (isLinear == 0); // false means non-linear
    if (!ok) {
        std::cerr << "FAIL: Build_sRGBGamma() should be non-linear (IsToneCurveLinear should be false)." << std::endl;
    } else {
        std::cout << "PASS: sRGB Forward gamma is non-linear as expected." << std::endl;
    }
    cmsFreeToneCurve(Forward);
    return ok;
}

// Test 3: Verify a manual reverse + 16-bit gamma combination produces a linear tone curve.
// This exercises cmsReverseToneCurve and CombineGamma16 independently of the focal function.
bool test_Reverse_and_Combine_Gamma16_Produces_Linear() {
    bool ok = true;

    cmsToneCurve* Forward = Build_sRGBGamma();
    if (Forward == nullptr) {
        std::cerr << "FAIL: Build_sRGBGamma() returned null in Reverse/Combine test." << std::endl;
        return false;
    }

    cmsToneCurve* Reverse = cmsReverseToneCurve(Forward);
    if (Reverse == nullptr) {
        std::cerr << "FAIL: cmsReverseToneCurve() returned null." << std::endl;
        cmsFreeToneCurve(Forward);
        return false;
    }

    cmsToneCurve* Result = CombineGamma16(Forward, Reverse);
    if (Result == nullptr) {
        std::cerr << "FAIL: CombineGamma16() returned null." << std::endl;
        cmsFreeToneCurve(Forward);
        cmsFreeToneCurve(Reverse);
        return false;
    }

    cmsBool linear = cmsIsToneCurveLinear(Result);
    ok = (linear != 0);
    if (!ok) {
        std::cerr << "FAIL: Combined 16-bit gamma curve is not linear as expected." << std::endl;
    } else {
        std::cout << "PASS: Reverse + CombineGamma16 yields a linear curve." << std::endl;
    }

    // Free all allocated tone curves
    cmsFreeToneCurve(Forward);
    cmsFreeToneCurve(Reverse);
    cmsFreeToneCurve(Result);

    return ok;
}

// Simple test harness to run all tests and report summary
int main(int argc, char* argv[]) {
    // Suppress any potential i/o sync delays for performance (optional)
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    struct TestCase {
        const char* name;
        std::function<bool()> fn;
    };

    TestCase tests[] = {
        {"CheckJoint16Curves_sRGB_ReturnsTrue", test_CheckJoint16Curves_sRGB_ReturnsTrue},
        {"sRGB_Gamma_Is_Not_Linear", test_sRGB_Gamma_Is_Not_Linear},
        {"Reverse_and_Combine_Gamma16_Produces_Linear", test_Reverse_and_Combine_Gamma16_Produces_Linear},
    };

    const int total = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;

    std::cout << "Running " << total << " test(s) for CheckJoint16Curves_sRGB and related helpers...\n";

    for (int i = 0; i < total; ++i) {
        bool result = tests[i].fn();
        if (result) ++passed;
        // The individual tests print their own PASS/FAIL messages.
        // We print a concise summary line per test here as well.
        std::cout << (result ? "[OK] " : "[KO] ") << tests[i].name << std::endl;
    }

    std::cout << "Test Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}