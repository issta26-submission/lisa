// C++11 unit tests for the focal method Build_sRGBGamma (no GTest, lightweight harness)

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <iomanip>
#include <cassert>
#include <cmath>


// Dependency: testcms2.h (provides test harness utilities and cms interface stubs if needed)

extern "C" {
    // Minimal opaque/type declarations to interface with LittleCMS-like API
    typedef void cmsContext;
    typedef void cmsToneCurve;
    typedef double cmsFloat64Number;
    typedef int cmsInt32Number;

    // Focal function under test (provided by the project under test)
    cmsToneCurve* Build_sRGBGamma(void);

    // Core LittleCMS-like API surface used by the test (extern "C" to avoid name mangling)
    cmsToneCurve* cmsBuildParametricToneCurve(cmsContext ContextID, cmsInt32Number Type, const cmsFloat64Number Params[]);
    cmsContext DbgThread(void);
    cmsFloat64Number cmsEvalToneCurveFloat(const cmsToneCurve* ToneCurve, cmsFloat64Number In);
    void cmsFreeToneCurve(cmsToneCurve* Curve);
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

static void log_failure(const std::string& msg)
{
    std::cerr << "Test Failure: " << msg << std::endl;
    ++g_failures;
}

static void check_true(bool cond, const std::string& msg)
{
    if (!cond) {
        log_failure(msg);
    }
}

// Expected sRGB to linear mapping for the specific inverse-like curve built by Build_sRGBGamma
static double expected_srgb_inverse(double s)
{
    // Based on the sRGB inverse mapping (s = sRGB value, maps to linear)
    // If s <= 0.04045 then linear = s / 12.92 else linear = ((s + 0.055) / 1.055) ^ 2.4
    const double d = 0.04045;
    if (s <= d) {
        return s / 12.92;
    } else {
        return std::pow((s + 0.055) / 1.055, 2.4);
    }
}

// Test 1: Build_sRGBGamma should return a non-null tone curve
static void Test_Build_sRGBGamma_NotNull()
{
    cmsToneCurve* curve = Build_sRGBGamma();
    check_true(curve != nullptr, "Build_sRGBGamma() returned a null tone curve");
    if (curve != nullptr) {
        cmsFreeToneCurve(curve);
    }
}

// Test 2: Evaluate the generated curve against the known sRGB inverse mapping for multiple inputs
static void Test_Build_sRGBGamma_EvalMatchesExpected()
{
    cmsToneCurve* curve = Build_sRGBGamma();
    check_true(curve != nullptr, "Build_sRGBGamma() returned a null tone curve for evaluation");

    if (curve == nullptr) {
        return;
    }

    // Test samples in [0,1], including threshold around 0.04045
    const std::vector<double> samples = {
        0.0, 0.01, 0.04, 0.04045, 0.05, 0.2, 0.5, 1.0
    };

    const double tol = 1e-6; // allowable absolute difference
    for (size_t i = 0; i < samples.size(); ++i) {
        double s = samples[i];
        // Clamp to [0,1] to be safe in case of edge noise
        if (s < 0.0) s = 0.0;
        if (s > 1.0) s = 1.0;

        double expected = expected_srgb_inverse(s);
        double actual = cmsEvalToneCurveFloat(curve, s);

        double diff = std::fabs(actual - expected);
        if (diff > tol) {
            log_failure("Mismatch at sample " + std::to_string(s) +
                        ": expected " + std::to_string(expected) +
                        ", got " + std::to_string(actual) +
                        " (diff=" + std::to_string(diff) + ")");
        }
        // Also enforce monotonicity locally
        if (i > 0) {
            double prev_s = samples[i - 1];
            double prev_out = cmsEvalToneCurveFloat(curve, prev_s);
            if (prev_out > actual + 1e-12) {
                log_failure("Curve not monotonic for inputs " +
                            std::to_string(prev_s) + " -> " + std::to_string(s) +
                            " (prev_out=" + std::to_string(prev_out) +
                            ", curr_out=" + std::to_string(actual) + ")");
            }
        }
    }

    cmsFreeToneCurve(curve);
}

// Test 3: Static behavior probing (ensuring static-like expectations are preserved through public API)
static void Test_Build_sRGBGamma_StaticBehavior()
{
    // Accessing static-like integrity through public API:
    // Rebuild twice and ensure non-null results and consistent evaluation.
    cmsToneCurve* curve1 = Build_sRGBGamma();
    cmsToneCurve* curve2 = Build_sRGBGamma();

    check_true(curve1 != nullptr, "First Build_sRGBGamma() call returned null");
    check_true(curve2 != nullptr, "Second Build_sRGBGamma() call returned null");

    if (curve1 && curve2) {
        // Simple consistency check on a couple of sample points
        double a = cmsEvalToneCurveFloat(curve1, 0.15);
        double b = cmsEvalToneCurveFloat(curve2, 0.15);
        const double tol = 1e-6;
        check_true(std::fabs(a - b) <= tol, "Inconsistent evaluation between two subsequent builds at 0.15");

        cmsFreeToneCurve(curve1);
        cmsFreeToneCurve(curve2);
    } else {
        // If either curve is null, the previous asserts would have recorded failures.
        if (curve1) cmsFreeToneCurve(curve1);
        if (curve2) cmsFreeToneCurve(curve2);
    }
}

// Entry point for the test suite
int main(int argc, char* argv[])
{
    // Run tests
    Test_Build_sRGBGamma_NotNull();
    Test_Build_sRGBGamma_EvalMatchesExpected();
    Test_Build_sRGBGamma_StaticBehavior();

    // Summary
    std::cout << "------------------------------" << std::endl;
    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}