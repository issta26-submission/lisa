/*
Unit test suite for the focal method:
  cmsHPROFILE CreateFakeCMYK(cmsFloat64Number InkLimit, cmsBool lUseAboveRGB)

Context and approach:
- The test suite targets two main execution paths:
  1) lUseAboveRGB == true (uses Create_AboveRGB)
  2) lUseAboveRGB == false (uses cmsCreate_sRGBProfile)

- We verify basic metadata of the produced profile to ensure the function creates a valid CMYK output profile:
  - Color space should be CMYK (cmsSigCmykData)
  - Device class should be Output (cmsSigOutputClass)
  - PCS (Profile Connection Space) should be Lab (cmsSigLabData)

- We test several InkLimit values to exercise potential code paths related to ink limit handling:
  - 0.0, 0.5, 1.0

- We perform the tests in a small, self-contained C++11 test harness (no GTest) using the library's public API only.
- Each test has explanatory comments. Non-terminating assertions are used (logged results) so that all tests run even if one fails.

Notes:
- This test suite uses the provided testcms2.h header and the LittleCMS API (lcms2.h) to verify returned profile metadata.
- The tests do not rely on private methods; they exercise publicly observable behavior of CreateFakeCMYK.
- The test main function calls each test and prints a PASS/FAIL result. It does not terminate on first failure to maximize coverage.

Code (test_fake_cmyk_suite.cpp):
*/

#include <lcms2.h>
#include <iostream>
#include <string>
#include <testcms2.h>


// Helper macro for non-terminating assertion with logging
#define LOG_ASSERT(cond, msg) do { \
    if (cond) { \
        std::cout << "[PASS] " << msg << "\n"; \
    } else { \
        std::cout << "[FAIL] " << msg << "\n"; \
        failures++; \
    } \
} while (0)

static int failures = 0;

// Test 1: Validate that CreateFakeCMYK returns a non-null profile for AboveRGB path
//    - lUseAboveRGB = true
//    - InkLimit values tested: 0.5 (typical mid-range)
static void Test_CreateFakeCMYK_AboveRGB_MidInk(void)
{
    // Explanation:
    // Ensure that when AboveRGB pipeline is used, a valid CMYK output profile is produced.
    // Check public metadata to confirm the profile is CMYK, OutputDevice, Lab PCS.
    cmsFloat64Number ink = 0.5;
    cmsBool useAboveRGB = 1; // true

    cmsHPROFILE h = CreateFakeCMYK(ink, useAboveRGB);
    if (!h) {
        std::cout << "[FAIL] Test_CreateFakeCMYK_AboveRGB_MidInk: CreateFakeCMYK returned NULL for InkLimit=0.5 with AboveRGB\n";
        failures++;
        return;
    }

    // Validate essential profile metadata
    cmsColorSpaceSignature cs = cmsGetColorSpace(h);
    cmsDeviceClass dc    = cmsGetDeviceClass(h);
    cmsColorSpaceSignature pcs = cmsGetPCS(h);

    bool ok = (cs == cmsSigCmykData) && (dc == cmsSigOutputClass) && (pcs == cmsSigLabData);
    LOG_ASSERT(ok, "Test_CreateFakeCMYK_AboveRGB_MidInk: Correct color space, device class, and PCS for AboveRGB path");

    cmsCloseProfile(h);
}

// Test 2: Validate that CreateFakeCMYK returns a non-null profile for sRGB path
//    - lUseAboveRGB = false
//    - InkLimit values tested: 0.5
static void Test_CreateFakeCMYK_SRGB_MidInk(void)
{
    // Explanation:
    // Ensure that when sRGB is used (not AboveRGB), a valid CMYK output profile is produced.
    cmsFloat64Number ink = 0.5;
    cmsBool useAboveRGB = 0; // false

    cmsHPROFILE h = CreateFakeCMYK(ink, useAboveRGB);
    if (!h) {
        std::cout << "[FAIL] Test_CreateFakeCMYK_SRGB_MidInk: CreateFakeCMYK returned NULL for InkLimit=0.5 with sRGB path\n";
        failures++;
        return;
    }

    // Validate essential profile metadata
    cmsColorSpaceSignature cs = cmsGetColorSpace(h);
    cmsDeviceClass dc    = cmsGetDeviceClass(h);
    cmsColorSpaceSignature pcs = cmsGetPCS(h);

    bool ok = (cs == cmsSigCmykData) && (dc == cmsSigOutputClass) && (pcs == cmsSigLabData);
    LOG_ASSERT(ok, "Test_CreateFakeCMYK_SRGB_MidInk: Correct color space, device class, and PCS for sRGB path");

    cmsCloseProfile(h);
}

// Test 3: Validate edge-case InkLimit = 0.0 (low ink scenario) with AboveRGB path
static void Test_CreateFakeCMYK_AboveRGB_ZeroInk(void)
{
    cmsFloat64Number ink = 0.0;
    cmsBool useAboveRGB = 1;

    cmsHPROFILE h = CreateFakeCMYK(ink, useAboveRGB);
    if (!h) {
        std::cout << "[FAIL] Test_CreateFakeCMYK_AboveRGB_ZeroInk: CreateFakeCMYK returned NULL for InkLimit=0.0 with AboveRGB\n";
        failures++;
        return;
    }

    cmsColorSpaceSignature cs = cmsGetColorSpace(h);
    cmsDeviceClass dc    = cmsGetDeviceClass(h);
    cmsColorSpaceSignature pcs = cmsGetPCS(h);

    bool ok = (cs == cmsSigCmykData) && (dc == cmsSigOutputClass) && (pcs == cmsSigLabData);
    LOG_ASSERT(ok, "Test_CreateFakeCMYK_AboveRGB_ZeroInk: Metadata valid for InkLimit=0.0 AboveRGB");

    cmsCloseProfile(h);
}

// Test 4: Validate edge-case InkLimit = 1.0 (high ink scenario) with sRGB path
static void Test_CreateFakeCMYK_SRGB_OneInk(void)
{
    cmsFloat64Number ink = 1.0;
    cmsBool useAboveRGB = 0;

    cmsHPROFILE h = CreateFakeCMYK(ink, useAboveRGB);
    if (!h) {
        std::cout << "[FAIL] Test_CreateFakeCMYK_SRGB_OneInk: CreateFakeCMYK returned NULL for InkLimit=1.0 with sRGB path\n";
        failures++;
        return;
    }

    cmsColorSpaceSignature cs = cmsGetColorSpace(h);
    cmsDeviceClass dc    = cmsGetDeviceClass(h);
    cmsColorSpaceSignature pcs = cmsGetPCS(h);

    bool ok = (cs == cmsSigCmykData) && (dc == cmsSigOutputClass) && (pcs == cmsSigLabData);
    LOG_ASSERT(ok, "Test_CreateFakeCMYK_SRGB_OneInk: Metadata valid for InkLimit=1.0 in sRGB path");

    cmsCloseProfile(h);
}

// Test 5: Stress test: multiple sequential invocations to ensure stability across paths
static void Test_CreateFakeCMYK_MultipleSequential(void)
{
    // This test ensures that successive calls (both AboveRGB and sRGB paths)
    // do not crash and consistently return valid CMYK profiles.
    bool overallOk = true;

    const struct {
        cmsFloat64Number ink;
        cmsBool useAboveRGB;
    } cases[] = {
        {0.25, 1},
        {0.75, 1},
        {0.25, 0},
        {0.75, 0}
    };

    for (size_t i = 0; i < sizeof(cases)/sizeof(cases[0]); ++i) {
        cmsHPROFILE h = CreateFakeCMYK(cases[i].ink, cases[i].useAboveRGB);
        if (!h) {
            std::cout << "[FAIL] Test_CreateFakeCMYK_MultipleSequential: NULL profile for case " << i
                      << " (Ink=" << cases[i].ink << ", AboveRGB=" << (cases[i].useAboveRGB ? "true" : "false") << ")\n";
            failures++;
            overallOk = false;
            continue;
        }

        cmsColorSpaceSignature cs = cmsGetColorSpace(h);
        cmsDeviceClass dc    = cmsGetDeviceClass(h);
        cmsColorSpaceSignature pcs = cmsGetPCS(h);

        bool ok = (cs == cmsSigCmykData) && (dc == cmsSigOutputClass) && (pcs == cmsSigLabData);
        if (!ok) {
            std::cout << "[FAIL] Test_CreateFakeCMYK_MultipleSequential: Metadata invalid for case " << i
                      << " (Ink=" << cases[i].ink << ", AboveRGB=" << (cases[i].useAboveRGB ? "true" : "false") << ")\n";
            failures++;
            overallOk = false;
        } else {
            std::cout << "[PASS] Test_CreateFakeCMYK_MultipleSequential: Case " << i
                      << " (Ink=" << cases[i].ink << ", AboveRGB=" << (cases[i].useAboveRGB ? "true" : "false") << ")\n";
        }

        cmsCloseProfile(h);
    }

    if (overallOk) {
        // All cases passed
        std::cout << "[PASS] Test_CreateFakeCMYK_MultipleSequential: All cases passed\n";
    }
}

// Main: run all tests and report a summary
int main(int argc, char* argv[])
{
    std::cout << "Starting CreateFakeCMYK unit tests (no GTest, C++11)..." << std::endl;
    failures = 0;

    Test_CreateFakeCMYK_AboveRGB_MidInk();
    Test_CreateFakeCMYK_SRGB_MidInk();
    Test_CreateFakeCMYK_AboveRGB_ZeroInk();
    Test_CreateFakeCMYK_SRGB_OneInk();
    Test_CreateFakeCMYK_MultipleSequential();

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}