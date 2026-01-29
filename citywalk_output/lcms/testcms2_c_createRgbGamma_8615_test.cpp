// Test suite for cmsHPROFILE createRgbGamma(cmsFloat64Number g)
// This test is written for C++11, does not use GTest, and uses a minimal in-file harness.
// It relies on LittleCMS (lcms2.h) being available during linking.

#include <iostream>
#include <lcms2.h>
#include <testcms2.h>


// Forward declaration of the focal function under test.
// The function is implemented in C, so we declare with extern "C" to avoid name mangling when used from C++.
#ifdef __cplusplus
extern "C" {
#endif
    cmsHPROFILE createRgbGamma(cmsFloat64Number g);
#ifdef __cplusplus
}
#endif

// Simple test harness
static int total_tests = 0;
static int failed_tests = 0;

// Report helper: logs per-test result and updates counters
static void report(bool cond, const char* testName) {
    total_tests++;
    if (!cond) {
        std::cerr << "[FAIL] " << testName << std::endl;
        failed_tests++;
    } else {
        std::cout << "[OK]   " << testName << std::endl;
    }
}

/*
   Test 1: Basic functionality for g = 2.2
   - Expect a non-null profile to be created
   - Expect the resulting profile to be RGB data (inline gamma for R, G, B)
*/
static void test_createRgbGamma_G2_2_Returns_RGB_Profile() {
    cmsFloat64Number g = 2.2;
    cmsHPROFILE p = createRgbGamma(g);
    report(p != NULL, "createRgbGamma(g=2.2) returns non-null profile");
    if (p != NULL) {
        cmsColorSpaceSignature cs = cmsGetColorSpace(p);
        report(cs == cmsSig_RGBData, "Profile color space is RGBData for g=2.2");
        cmsCloseProfile(p);
    }
}

/*
   Test 2: Basic functionality for g = 1.0 (linear gamma)
   - Expect a non-null profile to be created
   - Expect the resulting profile to be RGB data
*/
static void test_createRgbGamma_G1_0_Returns_RGB_Profile() {
    cmsFloat64Number g = 1.0;
    cmsHPROFILE p = createRgbGamma(g);
    report(p != NULL, "createRgbGamma(g=1.0) returns non-null profile");
    if (p != NULL) {
        cmsColorSpaceSignature cs = cmsGetColorSpace(p);
        report(cs == cmsSig_RGBData, "Profile color space is RGBData for g=1.0");
        cmsCloseProfile(p);
    }
}

/*
   Test 3: Edge gamma value close to zero (extreme gamma)
   - Expect a non-null profile to be created
   - Expect the resulting profile to be RGB data
*/
static void test_createRgbGamma_G0_01_Returns_RGB_Profile() {
    cmsFloat64Number g = 0.01;
    cmsHPROFILE p = createRgbGamma(g);
    report(p != NULL, "createRgbGamma(g=0.01) returns non-null profile");
    if (p != NULL) {
        cmsColorSpaceSignature cs = cmsGetColorSpace(p);
        report(cs == cmsSig_RGBData, "Profile color space is RGBData for g=0.01");
        cmsCloseProfile(p);
    }
}

/*
   Test 4: Another elevated gamma value (g = 5.0)
   - Expect a non-null profile to be created
   - Expect the resulting profile to be RGB data
*/
static void test_createRgbGamma_G5_0_Returns_RGB_Profile() {
    cmsFloat64Number g = 5.0;
    cmsHPROFILE p = createRgbGamma(g);
    report(p != NULL, "createRgbGamma(g=5.0) returns non-null profile");
    if (p != NULL) {
        cmsColorSpaceSignature cs = cmsGetColorSpace(p);
        report(cs == cmsSig_RGBData, "Profile color space is RGBData for g=5.0");
        cmsCloseProfile(p);
    }
}

// Entry point: executes all tests and reports summary
int main() {
    // Run tests
    test_createRgbGamma_G2_2_Returns_RGB_Profile();
    test_createRgbGamma_G1_0_Returns_RGB_Profile();
    test_createRgbGamma_G0_01_Returns_RGB_Profile();
    test_createRgbGamma_G5_0_Returns_RGB_Profile();

    std::cout << "Tests run: " << total_tests << ", Failures: " << failed_tests << std::endl;

    return (failed_tests == 0) ? 0 : 1;
}