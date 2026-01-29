// Test suite for the focal method CheckJointFloatCurves_sRGB (as declared in testcms2.c)
// This test suite is written in C++11, does not rely on GTest, and uses a lightweight
// in-process test harness with non-terminating assertions.
// The tests exercise the focal method and its core dependencies to ensure correct behavior.

#include <iostream>
#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Step 1 and 2 context (from the provided focal method and class dependencies):
// - Focal method under test: CheckJointFloatCurves_sRGB(void)
// - Core dependent components used by the focal method:
//   - Build_sRGBGamma() -> cmsToneCurve*
//   - cmsReverseToneCurve(cmsToneCurve* ca) -> cmsToneCurve*
//   - CombineGammaFloat(cmsToneCurve*, cmsToneCurve*) -> cmsToneCurve*
//   - cmsIsToneCurveLinear(cmsToneCurve*) -> cmsBool
// - Memory management: cmsFreeToneCurve(cmsToneCurve*)
// These functions are declared in testcms2.h (C API). We include it with C linkage.

extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_PassCount = 0;
static int g_FailCount = 0;

// CHECK macro: logs failures but continues execution to maximize coverage
#define CHECK(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_FailCount; \
    } else { \
        ++g_PassCount; \
    } \
} while (0)

// Explanatory notes about candidate keywords and dependencies (for maintainers):
// Candidate Keywords: Forward, Reverse, Result, Build_sRGBGamma, cmsReverseToneCurve,
// CombineGammaFloat, cmsIsToneCurveLinear, cmsToneCurve, cmsFreeToneCurve, Forward non-linearity,
// gamma combination yielding linear curve, memory management of tone curves.

// Test 1: Directly verify that the focal method CheckJointFloatCurves_sRGB returns a true value.
// Rationale: The focal method constructs a gamma curve, reverses it, combines them, and checks linearity.
// The expected outcome is that the final Result is a linear tone curve.
void test_CheckJointFloatCurves_sRGB_ReturnsTrue(void)
{
    std::cout << "Running test: CheckJointFloatCurves_sRGB_ReturnsTrue" << std::endl;

    cmsInt32Number rc = CheckJointFloatCurves_sRGB();
    // Expect non-zero (true) return value
    CHECK(rc != 0, "CheckJointFloatCurves_sRGB() should return true (non-zero)");
}

// Test 2: Validate the behavior of the individual gamma components used by the focal method.
// Rationale: This test probes the internal steps by independently using the same helpers as the focal method.
// - Build_sRGBGamma should produce a non-linear Forward curve.
// - cmsReverseToneCurve(Forward) should produce a valid Reverse curve.
// - CombineGammaFloat(Forward, Reverse) should yield a linear Result curve.
// - All allocated curves must be freed to avoid leaks.
void test_SRGBGammaDecompositionConsistency(void)
{
    std::cout << "Running test: SRGBGammaDecompositionConsistency" << std::endl;

    cmsToneCurve* Forward = Build_sRGBGamma();
    CHECK(Forward != nullptr, "Build_sRGBGamma() returned null");

    if (Forward != nullptr) {
        cmsBool isLinearForward = cmsIsToneCurveLinear(Forward);
        // Forward curve for sRGB gamma should not be linear (non-linear gamma curve)
        CHECK(isLinearForward == 0, "Forward gamma should be non-linear (not linear)");

        cmsToneCurve* Reverse = cmsReverseToneCurve(Forward);
        CHECK(Reverse != nullptr, "cmsReverseToneCurve(Forward) returned null");

        if (Reverse != nullptr) {
            cmsToneCurve* Result = CombineGammaFloat(Forward, Reverse);
            CHECK(Result != nullptr, "CombineGammaFloat(Forward, Reverse) returned null");

            if (Result != nullptr) {
                cmsBool isLinearResult = cmsIsToneCurveLinear(Result);
                // The composition of forward and reverse gamma should yield a linear curve
                CHECK(isLinearResult != 0, "Combined gamma (Forward * Reverse) should be linear");

                cmsFreeToneCurve(Result);
            }

            cmsFreeToneCurve(Reverse);
        }

        cmsFreeToneCurve(Forward);
    }
}

// Main function to drive tests. It calls all test functions and prints a final summary.
// The tests rely on the internal C API defined in testcms2.h; no private members are accessed.
int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    std::cout << "Starting test suite for CheckJointFloatCurves_sRGB...\n" << std::endl;

    // Execute tests
    test_CheckJointFloatCurves_sRGB_ReturnsTrue();
    test_SRGBGammaDecompositionConsistency();

    // Summary
    int total = g_PassCount + g_FailCount;
    std::cout << "\nTest Summary: " << total << " checks, "
              << g_PassCount << " passed, "
              << g_FailCount << " failed." << std::endl;

    // Return code: non-zero if any test failed
    return (g_FailCount > 0) ? 1 : 0;
}