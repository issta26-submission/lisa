/*
Unit Test Suite for cmsHPROFILE Create_Gray30(void)

Step 1 - Program Understanding (Candidate Keywords)
- Curve: cmsToneCurve* Curve
- Gamma build: cmsBuildGamma(DbgThread(), 3.0)
- Context: DbgThread()
- Profile creation: cmsCreateGrayProfileTHR(DbgThread(), cmsD50_xyY(), Curve)
- Resource management: cmsFreeToneCurve(Curve)
- Result: cmsHPROFILE hProfile

Notes:
- The focal method creates a gamma curve with gamma = 3.0, builds a Gray ICC profile using D50 xyY, frees the gamma curve, and returns the profile.
- It returns NULL only if Curve creation fails (Curve == NULL).
- We will validate that the returned profile is non-null and has the Gray color space, indicating a proper Gray profile creation path.

Step 2 - Unit Test Generation (based on dependencies)
- Dependencies referenced by the focal method (via testcms2.h and lcms2.h): cmsBuildGamma, DbgThread, cmsCreateGrayProfileTHR, cmsD50_xyY, cmsFreeToneCurve, cmsHPROFILE, cmsCloseProfile (for cleanup), cmsGetColorSpace, cmsSigGrayData.
- Tests target:
  1) Positive path: Create_Gray30 returns a non-null profile.
  2) Positive path validation: The returned profile has the Gray color space signature (cmsSigGrayData).
- We will not mock internal static helpers; we rely on the actual library behavior. The tests will be non-terminating (they log failures and continue), per domain guidance.

Step 3 - Test Case Refinement
- Use a lightweight C++11 test harness (no GTest). Implement a small CHECK-like mechanism that reports failures but continues execution.
- Tests will be standalone functions invoked from main(), as instructed for environments where GTest is not allowed.
- We will ensure proper cleanup by calling cmsCloseProfile on any non-null profile.

Code (test_CreateGray30.cpp):
*/

// Include minimal standard library and LCMS headers
#include <iostream>
#include <lcms2.h>
#include <cstdio>
#include <testcms2.h>


// Declaration of the focal function under test (C linkage)
extern "C" cmsHPROFILE Create_Gray30(void);

/*
Candidate Keywords reflected in tests:
- Curve, cmsBuildGamma, DbgThread, cmsCreateGrayProfileTHR, cmsD50_xyY, cmsFreeToneCurve
*/

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_pass = 0;
static int g_tests_fail = 0;

// Helper macro for non-terminating assertions (logs and continues)
#define TEST_CHECK(cond, msg) \
    do { \
        ++g_tests_run; \
        if (cond) { \
            ++g_tests_pass; \
        } else { \
            ++g_tests_fail; \
            std::cerr << "[FAIL] " << msg << "\n"; \
        } \
    } while (0)

// Test 1: Ensure Create_Gray30 returns a non-null profile (positive path)
void test_CreateGray30_ReturnsNonNull_Profile(void)
{
    cmsHPROFILE hProfile = Create_Gray30();
    TEST_CHECK(hProfile != NULL, "Create_Gray30() should return a non-null profile on normal path.");

    if (hProfile != NULL) {
        cmsCloseProfile(hProfile); // Cleanup to avoid leaks
    }
}

// Test 2: Validate that the produced profile has the Gray color space signature
void test_CreateGray30_Profile_Is_Gray_ColorSpace(void)
{
    cmsHPROFILE hProfile = Create_Gray30();
    TEST_CHECK(hProfile != NULL, "Create_Gray30() returned NULL; cannot verify color space.");

    if (hProfile != NULL) {
        cmsColorSpaceSignature cs = cmsGetColorSpace(hProfile);
        TEST_CHECK(cs == cmsSigGrayData, "Profile color space should be Gray (cmsSigGrayData).");
        cmsCloseProfile(hProfile);
    }
}

// Entry point for tests
int main(int argc, char* argv[])
{
    // Run tests
    test_CreateGray30_ReturnsNonNull_Profile();
    test_CreateGray30_Profile_Is_Gray_ColorSpace();

    // Summary
    std::cout << "\nTest Summary:\n";
    std::cout << "Total tests run: " << g_tests_run << "\n";
    std::cout << "Passed: " << g_tests_pass << "\n";
    std::cout << "Failed: " << g_tests_fail << "\n";

    // Return non-zero if any test failed
    return (g_tests_fail != 0) ? 1 : 0;
}