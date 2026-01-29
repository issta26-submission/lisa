// Test suite for the focal method CheckGray in a LittleCMS-like environment.
// This suite is written in C++11 without using Google Test and uses a lightweight
// in-house test harness (no external test framework) to maximize coverage with
// non-terminating assertions.
//
// What this test targets:
// - The function CheckGray(cmsHTRANSFORM xform, cmsUInt8Number g, double L) which
//   uses cmsDoTransform to produce a Lab value from a gray input and then validates:
//     a* == 0 (within 0.001), b* == 0 (within 0.001), L within 0.01 of expected.
//
// Test strategy:
// - Create a grayscale-to-Lab transform and verify that g=0 maps to L≈0 with a≈0, b≈0.
// - Verify that g=255 maps to L≈100 with a≈0, b≈0.
// - Verify that providing an incorrect L yields a false result to exercise the negative path.
// - Use a lightweight, self-contained test harness to print PASS/FAIL per test and
//   accumulate summary results. Tests are designed to be executable with the
//   project's existing LightCMS-like API (lcms) and do not rely on GTest.
//
// Important notes for understandability (mapped to Step 1-3 guidance in the prompt):
// - Candidate Keywords: CheckGray, cmsDoTransform, Lab (a*, b*, L), gray input (g),
//   cmsHTRANSFORM xform, color transformation pipeline, 8-bit gray (TYPE_GRAY_8),
//   Lab double type (TYPE_Lab_DBL), IsGoodVal semantics, test harness.
// - Dependencies (Step 2): The test relies on the typical LittleCMS-like API
//   (lcms2.h) to build a Gray-to-Lab transform. It assumes availability of
//   cmsCreateGrayProfile, cmsCreateLab4Profile, cmsCreateTransform, cmsDeleteTransform,
//   cmsCloseProfile and the COLORSPACE/TYPES/macros (TYPE_GRAY_8, TYPE_Lab_DBL, etc.).
// - Domain knowledge (Step 3): Uses a non-terminating, boolean-style test result
//   (returning true/false) to maximize coverage without terminating on first failure.
// - Static/class scope notes (Step 3): The test relies on the public API only; static
//   internals of the tested code are not accessed directly. The test uses the public
//   interface and asserts via non-terminating checks (the test harness prints results).

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <lcms2.h>


// Include LittleCMS-like API header. The real project may name this differently.
// If your environment uses a different header or path, adjust accordingly.

// Declare the focal function under test. This exists in the codebase as:
// cmsInt32Number CheckGray(cmsHTRANSFORM xform, cmsUInt8Number g, double L)
extern "C" cmsInt32Number CheckGray(cmsHTRANSFORM xform, cmsUInt8Number g, double L);

// Lightweight test framework (non-terminating assertions)
struct TestResult {
    std::string name;
    bool passed;
};

static std::vector<TestResult> g_testResults;

// Simple IsGoodVal mimic used by CheckGray for per-value tolerances.
// If the real IsGoodVal is more elaborate in the project, provide an alias/proxy here.
extern "C" int IsGoodVal(const char* /*title*/, double in, double out, double maxErr)
{
    // Return non-zero (true) if fabs(in - out) <= maxErr
    const double diff = std::abs(in - out);
    return (diff <= maxErr) ? 1 : 0;
}

// Helper to create a Gray-to-Lab transform for testing.
// This function uses a grayscale input profile and a Lab output profile.
// It assumes the environment provides these profile constructors:
// - cmsCreateGrayProfile
// - cmsCreateLab4Profile
// - cmsCreateTransform
// - cmsDeleteTransform
// - cmsCloseProfile
// The exact API may differ slightly by library version; adapt as needed.
static cmsHTRANSFORM CreateGrayToLabTransform()
{
    // Create a simplistic Gray and Lab profile pair for transform
    // Some environments require a white point; here we pass nullptr as a placeholder.
    cmsHPROFILE hGray = cmsCreateGrayProfile(nullptr);
    cmsHPROFILE hLab  = cmsCreateLab4Profile(nullptr);

    if (!hGray || !hLab) {
        if (hGray) cmsCloseProfile(hGray);
        if (hLab) cmsCloseProfile(hLab);
        return nullptr;
    }

    // Create a transform: input Gray 8-bit to Lab double
    // The exact constants TYPE_GRAY_8 and TYPE_Lab_DBL are commonly available in lcms2.h
    cmsHTRANSFORM xform = cmsCreateTransform(hGray, TYPE_GRAY_8, hLab, TYPE_Lab_DBL, INTENT_RELATIVE_COLORIMETRIC);

    // Cleanup input/output profiles; the transform keeps their resources
    cmsCloseProfile(hGray);
    cmsCloseProfile(hLab);

    return xform;
}

// Test 1: g = 0 should map to Lab.L ≈ 0, a ≈ 0, b ≈ 0
bool Test_CheckGray_GrayZero()
{
    cmsHTRANSFORM xform = CreateGrayToLabTransform();
    if (!xform) {
        std::cerr << "[Test_CheckGray_GrayZero] Failed to create transform." << std::endl;
        return false;
    }

    cmsUInt8Number g = 0;
    double expectedL = 0.0;
    cmsInt32Number ret = CheckGray(xform, g, expectedL);

    cmsDeleteTransform(xform);

    bool ok = (ret == 1);
    if (!ok) {
        std::cerr << "[Test_CheckGray_GrayZero] CheckGray() reported failure for g=0, L≈0." << std::endl;
    }
    else {
        // If CheckGray reported success, ensure a and b checks pass as part of internal logic.
        // The actual a,b checks are performed via IsGoodVal within CheckGray with tolerance 0.001.
        // We rely on the success of CheckGray as the test result.
    }

    return ok;
}

// Test 2: g = 255 should map to Lab.L ≈ 100, a ≈ 0, b ≈ 0
bool Test_CheckGray_GrayMax()
{
    cmsHTRANSFORM xform = CreateGrayToLabTransform();
    if (!xform) {
        std::cerr << "[Test_CheckGray_GrayMax] Failed to create transform." << std::endl;
        return false;
    }

    cmsUInt8Number g = 255;
    double expectedL = 100.0;
    cmsInt32Number ret = CheckGray(xform, g, expectedL);

    cmsDeleteTransform(xform);

    bool ok = (ret == 1);
    if (!ok) {
        std::cerr << "[Test_CheckGray_GrayMax] CheckGray() reported failure for g=255, L≈100." << std::endl;
    }

    return ok;
}

// Test 3: Invalid L value should cause CheckGray to fail (negative path)
bool Test_CheckGray_InvalidL()
{
    cmsHTRANSFORM xform = CreateGrayToLabTransform();
    if (!xform) {
        std::cerr << "[Test_CheckGray_InvalidL] Failed to create transform." << std::endl;
        return false;
    }

    cmsUInt8Number g = 0;
    double invalidL = 50.0; // far from the expected 0 for g=0
    cmsInt32Number ret = CheckGray(xform, g, invalidL);

    cmsDeleteTransform(xform);

    bool ok = (ret == 0);
    if (!ok) {
        std::cerr << "[Test_CheckGray_InvalidL] CheckGray() unexpectedly passed for invalid L." << std::endl;
    }

    return ok;
}

// Utility to run a single test and print outcome
void RunTest(const std::string& testName, bool (*testFn)())
{
    bool result = testFn();
    g_testResults.push_back({testName, result});
    std::cout << (result ? "[PASS] " : "[FAIL] ") << testName << std::endl;
}

// Entry point
int main()
{
    // Step 2 / Step 3: Register and run tests. The tests are designed to exercise
    // the focal method's positive and negative branches with a Gray->Lab transform.

    RunTest("Test_CheckGray_GrayZero", Test_CheckGray_GrayZero);
    RunTest("Test_CheckGray_GrayMax",  Test_CheckGray_GrayMax);
    RunTest("Test_CheckGray_InvalidL", Test_CheckGray_InvalidL);

    // Summary
    int passed = 0;
    for (const auto& r : g_testResults) if (r.passed) ++passed;
    int total = static_cast<int>(g_testResults.size());

    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}

// Explanatory notes for code reviewers:
// - The tests rely on the presence of minimal LittleCMS-like API functions to
//   construct a Gray-to-Lab transform and invoke the focal CheckGray function.
// - Test 1 validates the positive path for the lower bound (g=0, L≈0).
// - Test 2 validates the positive path for the upper bound (g=255, L≈100).
// - Test 3 validates the negative path by providing an incorrect L, ensuring
//   that the function returns 0.
// - All tests are non-terminating (they do not abort on failure; they report
//   PASS/FAIL and continue).