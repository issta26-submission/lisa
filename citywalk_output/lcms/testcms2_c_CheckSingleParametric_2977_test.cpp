// High-quality C++11 unit test suite for the focal method CheckSingleParametric
// This file provides two targeted test cases (success and failure paths) for
// the function cmsBool CheckSingleParametric(const char* Name, dblfnptr fn, cmsInt32Number Type, const cmsFloat64Number Params[])
// The tests are designed to be compiled into a project that already includes Little CMS
// and its test scaffolding. No GTest is used; a lightweight main-based harness is provided.

// Note: We rely on the existing test scaffolding’s IsGoodVal helper through the code under test.
// To keep this test self-contained, we mock IsGoodVal behavior by implementing a C linkage
// function with the same name signature. This function is resolved at link time by the
// CheckSingleParametric implementation. We also provide minimal stubs for DbgThread if required
// by the parametric curve builder (we return NULL context which is accepted by the library in tests).

#include <cstdio>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <lcms2.h>
#include <cmath>


// Bring in Little CMS declarations with C linkage
extern "C" {
}

// Function pointer type expected by CheckSingleParametric
typedef cmsFloat32Number (*dblfnptr)(cmsFloat32Number, const cmsFloat64Number[]);

// Declaration of the focal function from the project under test.
// We declare it with C linkage to match its original signature.
extern "C" cmsBool CheckSingleParametric(const char* Name, dblfnptr fn, cmsInt32Number Type, const cmsFloat64Number Params[]);

// -----------------------------------------------------------------------------
// Test scaffolding: provide IsGoodVal compatibility stub expected by test harness
// The real test framework in the project uses IsGoodVal to compare expected vs actual.
// Here we implement a lightweight version with a tolerance and log any mismatch.
// This symbol must be visible to the linker so that the library's CheckSingleParametric
// can call it during the test run.
extern "C" cmsBool IsGoodVal(const char* title, cmsFloat64Number in, cmsFloat64Number out, cmsFloat64Number max)
{
    cmsFloat64Number diff = std::abs(in - out);
    cmsBool ok = (diff <= max) ? 1 : 0;
    if (!ok) {
        std::fprintf(stderr, "IsGoodVal failed: %s | in=%f, out=%f, diff=%f, tol=%f\n",
                     title, in, out, diff, max);
    }
    return ok;
}

// Optional: provide a DbgThread() context provider if the library requires a cmsContext.
// Some test environments define this in the test harness already; we supply a conservative stub.
extern "C" cmsContext DbgThread(void)
{
    // Returning NULL as a safe default context for tests. If the library requires a valid
    // context, replace with cmsCreateContext(NULL, NULL) or a proper factory as needed.
    return NULL;
}

// -----------------------------------------------------------------------------
// Focal parametric function implementations (matching dblfnptr signature)
// We implement Gamma as a simple power function: y = x^Params[0].
// This aligns with the idea of a parametric gamma curve used by cmsBuildParametricToneCurve(Type, Params).
extern "C" cmsFloat32Number GammaFn(cmsFloat32Number x, const cmsFloat64Number Params[])
{
    // Guard against NaN/negative inputs; for domain [0,1] this is safe.
    double p = Params[0];
    if (x < 0.0) return 0.0f;
    return static_cast<cmsFloat32Number>(std::pow(static_cast<double>(x), p));
}

// A deliberately incorrect function to exercise the false-path in CheckSingleParametric.
// This will cause y_fn (from GammaFn) and y_param2 (fn(x_param)) to diverge.
extern "C" cmsFloat32Number WrongFn(cmsFloat32Number x, const cmsFloat64Number Params[])
{
    // Intentionally wrong transformation to trigger a mismatch
    return static_cast<cmsFloat32Number>(x * 2.0);
}

// -----------------------------------------------------------------------------
// Lightweight test cases (non-terminating assertions style)

// Test 1: Verify that a correct parametric gamma function (GammaFn) with Type 0 and Params[0]=2.2
// yields a TRUE result for CheckSingleParametric. This exercises the "happy path" and the
// inverse consistency check inside the function.
bool Test_Gamma_Parametric_Succeeds()
{
    cmsFloat64Number Params[1] = { 2.2 }; // gamma exponent
    const char* Name = "Gamma";

    // The Type value 0 is chosen to correspond to the gamma parametric type in cmsBuildParametricToneCurve
    // The library will internally construct tc (Type, Params) and tc_1 (-Type, Params)
    cmsInt32Number Type = 0;

    cmsBool result = CheckSingleParametric(Name, GammaFn, Type, Params);

    // Non-terminating assertion style: return a boolean; main will log PASS/FAIL
    return (result != 0);
}

// Test 2: Verify that CheckSingleParametric returns FALSE when the provided function does not
// match the parametric curve behavior (WrongFn). This exercises the "Error" path.
bool Test_Gamma_Parametric_WrongFn_Fails()
{
    cmsFloat64Number Params[1] = { 2.2 }; // gamma exponent (same as in Test 1)
    const char* Name = "Gamma_WrongFn";

    cmsInt32Number Type = 0;

    cmsBool result = CheckSingleParametric(Name, WrongFn, Type, Params);

    // Expect failure (FALSE) since WrongFn does not reconstruct the same forward curve
    return (result == 0);
}

// -----------------------------------------------------------------------------
// Main harness

int main()
{
    int failures = 0;

    std::cout << "Running tests for CheckSingleParametric (focal method)..." << std::endl;

    // Test 1: expected to pass
    if (Test_Gamma_Parametric_Succeeds()) {
        std::cout << "[PASS] Test_Gamma_Parametric_Succeeds: Gamma parametric curve forwards/inverses correctly." << std::endl;
    } else {
        std::cout << "[FAIL] Test_Gamma_Parametric_Succeeds: Gamma parametric curve failed to match expected output." << std::endl;
        ++failures;
    }

    // Test 2: expected to fail (false path)
    if (Test_Gamma_Parametric_WrongFn_Fails()) {
        std::cout << "[PASS] Test_Gamma_Parametric_WrongFn_Fails: Incorrect function correctly triggers failure path." << std::endl;
    } else {
        std::cout << "[FAIL] Test_Gamma_Parametric_WrongFn_Fails: Failure path not triggered as expected." << std::endl;
        ++failures;
    }

    // Summary
    if (failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}