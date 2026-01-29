// Test suite for FixColorSpaces in cmsvirt.c
// Notes:
// - This test suite uses the public Little CMS API to create a test profile and
//   then calls the focal function FixColorSpaces via a C linkage declaration.
// - The tests exercise different branches of the dwFlags path and the PCS-
//   related branching logic. We rely on IsPCS implemented inside the library to
//   drive the conditional branches.
// - All tests are written in plain C++11 without Google Test. A small lightweight
//   test harness is embedded to collect and print results. No private methods or
//   fields are accessed directly; only public API is used where possible.

#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>



// Ensure we can call the focal function from C++ tests.
extern "C" {
    // The focal function under test (assumed to have C linkage in cmsvirt.c)
    void FixColorSpaces(cmsHPROFILE hProfile,
                        cmsColorSpaceSignature ColorSpace,
                        cmsColorSpaceSignature PCS,
                        cmsUInt32Number dwFlags);
}

// Lightweight test harness
struct TestResult {
    std::string name;
    bool passed;
    std::string details;
};

static void PrintResult(const TestResult& r) {
    if (r.passed) {
        std::cout << "[PASS] " << r.name << "\n";
    } else {
        std::cout << "[FAIL] " << r.name << " - " << r.details << "\n";
    }
}

// Helper to create a fresh test profile
static cmsHPROFILE CreateTestProfile() {
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return NULL;
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(ContextID);
    // We deliberately do not delete the context to avoid extra boilerplate.
    return hProfile;
}

// Helper to fetch device class, color space and PCS for verification
static bool GetProfileDescriptors(cmsHPROFILE hProfile,
                                  cmsProfileClassSignature& outDeviceClass,
                                  cmsColorSpaceSignature& outColorSpace,
                                  cmsColorSpaceSignature& outPCS) {
    if (hProfile == NULL) return false;
    outDeviceClass = cmsGetDeviceClass(hProfile);
    outColorSpace  = cmsGetColorSpace(hProfile);
    outPCS          = cmsGetPCS(hProfile);
    return true;
}

// Test 1: dwFlags includes cmsFLAGS_GUESSDEVICECLASS and both ColorSpace and PCS are PCS.
// Expected: Abstract class, and ColorSpace/PCS are set exactly as provided.
static TestResult Test_FixColorSpaces_BothPCS_AbstractClass() {
    TestResult res;
    res.name = "FixColorSpaces_GuessDevice_BothPCS_AbstractClass";

    cmsHPROFILE hProfile = CreateTestProfile();
    if (!hProfile) {
        res.passed = false;
        res.details = "Failed to create test profile.";
        return res;
    }

    // Choose two color spaces that are expected to be PCS according to common LCMS builds.
    const cmsColorSpaceSignature ColorSpace = cmsSigLabData; // PCS in most implementations
    const cmsColorSpaceSignature PCS        = cmsSigXYZData; // PCS in most implementations

    FixColorSpaces(hProfile, ColorSpace, PCS, cmsFLAGS_GUESSDEVICECLASS);

    cmsProfileClassSignature deviceClass;
    cmsColorSpaceSignature colorSpaceAfter;
    cmsColorSpaceSignature pcsAfter;
    if (!GetProfileDescriptors(hProfile, deviceClass, colorSpaceAfter, pcsAfter)) {
        res.passed = false;
        res.details = "Failed to fetch profile descriptors.";
        cmsCloseProfile(hProfile);
        return res;
    }

    bool ok = (deviceClass == cmsSigAbstractClass) &&
              (colorSpaceAfter == ColorSpace) &&
              (pcsAfter == PCS);

    cmsCloseProfile(hProfile);
    res.passed = ok;
    if (!ok) {
        res.details = "Expected AbstractDeviceClass with provided ColorSpace/PCS, got differences.";
    }
    return res;
}

// Test 2: dwFlags set, ColorSpace is PCS and PCS is non-PCS.
// Expected: Output class; PCS is set to ColorSpace, ColorSpace is set to PCS.
static TestResult Test_FixColorSpaces_Guess_ColorSpacePCS_PCSNotPCS() {
    TestResult res;
    res.name = "FixColorSpaces_Guess_ColorSpacePCS_PCSNotPCS";

    cmsHPROFILE hProfile = CreateTestProfile();
    if (!hProfile) {
        res.passed = false;
        res.details = "Failed to create test profile.";
        return res;
    }

    // ColorSpace is PCS (XYZ), PCS is non-PCS (RGB)
    const cmsColorSpaceSignature ColorSpace = cmsSigXYZData;  // typically a PCS
    const cmsColorSpaceSignature PCS        = cmsSigRGBData;  // likely non-PCS

    FixColorSpaces(hProfile, ColorSpace, PCS, cmsFLAGS_GUESSDEVICECLASS);

    cmsProfileClassSignature deviceClass;
    cmsColorSpaceSignature colorSpaceAfter;
    cmsColorSpaceSignature pcsAfter;
    if (!GetProfileDescriptors(hProfile, deviceClass, colorSpaceAfter, pcsAfter)) {
        res.passed = false;
        res.details = "Failed to fetch profile descriptors.";
        cmsCloseProfile(hProfile);
        return res;
    }

    bool ok = (deviceClass == cmsSigOutputClass) &&
              (colorSpaceAfter == PCS) &&      // ColorSpace tag becomes PCS
              (pcsAfter == ColorSpace);        // PCS tag becomes original ColorSpace

    cmsCloseProfile(hProfile);
    res.passed = ok;
    if (!ok) {
        res.details = "Expected OutputClass with ColorSpace set to PCS and PCS set to ColorSpace.";
    }
    return res;
}

// Test 3: dwFlags set, PCS is PCS and ColorSpace is non-PCS.
// Expected: Input class; ColorSpace and PCS preserved per branch.
static TestResult Test_FixColorSpaces_Guess_PCSColorSpaceNotPCS() {
    TestResult res;
    res.name = "FixColorSpaces_Guess_PCS_PCSNotPCS";

    cmsHPROFILE hProfile = CreateTestProfile();
    if (!hProfile) {
        res.passed = false;
        res.details = "Failed to create test profile.";
        return res;
    }

    // ColorSpace is non-PCS (RGB), PCS is PCS (XYZ)
    const cmsColorSpaceSignature ColorSpace = cmsSigRGBData; // typically non-PCS
    const cmsColorSpaceSignature PCS        = cmsSigXYZData; //PCS

    FixColorSpaces(hProfile, ColorSpace, PCS, cmsFLAGS_GUESSDEVICECLASS);

    cmsProfileClassSignature deviceClass;
    cmsColorSpaceSignature colorSpaceAfter;
    cmsColorSpaceSignature pcsAfter;
    if (!GetProfileDescriptors(hProfile, deviceClass, colorSpaceAfter, pcsAfter)) {
        res.passed = false;
        res.details = "Failed to fetch profile descriptors.";
        cmsCloseProfile(hProfile);
        return res;
    }

    bool ok = (deviceClass == cmsSigInputClass) &&
              (colorSpaceAfter == ColorSpace) &&
              (pcsAfter == PCS);

    cmsCloseProfile(hProfile);
    res.passed = ok;
    if (!ok) {
        res.details = "Expected InputClass with provided ColorSpace/PCS.";
    }
    return res;
}

// Test 4: No GUD flag (dwFlags without cmsFLAGS_GUESSDEVICECLASS).
// Expected: Link class; ColorSpace and PCS are set as provided.
static TestResult Test_FixColorSpaces_NoGuessedDeviceClass() {
    TestResult res;
    res.name = "FixColorSpaces_NoGuessedDeviceClass";

    cmsHPROFILE hProfile = CreateTestProfile();
    if (!hProfile) {
        res.passed = false;
        res.details = "Failed to create test profile.";
        return res;
    }

    // Choose two non-PCS values to exercise the fallback path
    const cmsColorSpaceSignature ColorSpace = cmsSigRGBData;
    const cmsColorSpaceSignature PCS        = cmsSigGRAYData; // assume non-PCS

    FixColorSpaces(hProfile, ColorSpace, PCS, 0); // no GUD flag

    cmsProfileClassSignature deviceClass;
    cmsColorSpaceSignature colorSpaceAfter;
    cmsColorSpaceSignature pcsAfter;
    if (!GetProfileDescriptors(hProfile, deviceClass, colorSpaceAfter, pcsAfter)) {
        res.passed = false;
        res.details = "Failed to fetch profile descriptors.";
        cmsCloseProfile(hProfile);
        return res;
    }

    bool ok = (deviceClass == cmsSigLinkClass) &&
              (colorSpaceAfter == ColorSpace) &&
              (pcsAfter == PCS);

    cmsCloseProfile(hProfile);
    res.passed = ok;
    if (!ok) {
        res.details = "Expected LinkClass with provided ColorSpace/PCS when not guessing device class.";
    }
    return res;
}

// Simple harness to run all tests
int main() {
    std::vector<TestResult> results;

    results.push_back(Test_FixColorSpaces_BothPCS_AbstractClass());
    results.push_back(Test_FixColorSpaces_Guess_ColorSpacePCS_PCSNotPCS());
    results.push_back(Test_FixColorSpaces_Guess_PCSColorSpaceNotPCS());
    results.push_back(Test_FixColorSpaces_NoGuessedDeviceClass());

    int failures = 0;
    for (const auto& r : results) {
        PrintResult(r);
        if (!r.passed) ++failures;
    }

    std::cout << "Total tests: " << results.size() << ", Failures: " << failures << "\n";
    return failures;
}