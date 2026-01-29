// Minimal C++11 test suite for the focal function CheckFloatlinearXFORM from testcms2.c
// This suite uses the existing LittleCMS test helpers (IdentityMatrixProfile, CreateTestProfiles, etc.)
// and asserts behavior without terminating the program on failure (non-terminating style).

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <lcms2.h>


// Bring in the C headers and LittleCMS types. Use extern "C" to avoid name mangling for C APIs.
extern "C" {
}

// Lightweight test-case container
struct TestCase {
    std::string name;
    bool passed;
    std::string message;
};

// Forward declarations of the focal function (from testcms2.c)
extern "C" cmsInt32Number CheckFloatlinearXFORM(cmsHTRANSFORM xform, cmsInt32Number nChan);

// Helper: Run a single identity-gray transform test (nChan = 1)
static bool test_CheckFloatlinearXFORM_IdentityGray() {
    // Setup test profiles: identity-like gray space
    if (CreateTestProfiles() == 0) {
        // Fall back if profile creation failed
        std::cerr << "CreateTestProfiles failed\n";
        return false;
    }

    cmsHPROFILE inProf  = IdentityMatrixProfile(cmsSigGrayData); // 1-channel gray space
    cmsHPROFILE outProf = IdentityMatrixProfile(cmsSigGrayData);

    if (inProf == NULL || outProf == NULL) {
        std::cerr << "IdentityMatrixProfile returned NULL profile(s)\n";
        if (inProf) cmsCloseProfile(inProf);
        if (outProf) cmsCloseProfile(outProf);
        return false;
    }

    // Create a simple 32-bit floating point transform (identity-like)
    cmsHTRANSFORM xform = cmsCreateTransform(inProf, TYPE_FLOAT, outProf, TYPE_FLOAT, INTENT_PERCEPTUAL, 0);
    if (xform == NULL) {
        std::cerr << "cmsCreateTransform failed\n";
        cmsCloseProfile(inProf);
        cmsCloseProfile(outProf);
        return false;
    }

    // Invoke the focal function with a single channel
    cmsInt32Number result = CheckFloatlinearXFORM(xform, 1);

    // Cleanup
    cmsDeleteTransform(xform);
    cmsCloseProfile(inProf);
    cmsCloseProfile(outProf);

    // Expect true (non-zero)
    return (result == 1);
}

// Helper: Run a single identity RGB transform test (nChan = 3)
static bool test_CheckFloatlinearXFORM_IdentityRGB() {
    // Identity in RGB space
    cmsHPROFILE inProf  = IdentityMatrixProfile(cmsSigRGBData);
    cmsHPROFILE outProf = IdentityMatrixProfile(cmsSigRGBData);

    if (inProf == NULL || outProf == NULL) {
        std::cerr << "IdentityMatrixProfile returned NULL profile(s) for RGB\n";
        if (inProf) cmsCloseProfile(inProf);
        if (outProf) cmsCloseProfile(outProf);
        return false;
    }

    cmsHTRANSFORM xform = cmsCreateTransform(inProf, TYPE_FLOAT, outProf, TYPE_FLOAT, INTENT_PERCEPTUAL, 0);
    if (xform == NULL) {
        std::cerr << "cmsCreateTransform RGB failed\n";
        cmsCloseProfile(inProf);
        cmsCloseProfile(outProf);
        return false;
    }

    cmsInt32Number result = CheckFloatlinearXFORM(xform, 3);

    cmsDeleteTransform(xform);
    cmsCloseProfile(inProf);
    cmsCloseProfile(outProf);

    return (result == 1);
}

// Main test runner
int main() {
    std::vector<TestCase> tests;

    // Test 1: Identity transform in Gray space, 1 channel
    {
        TestCase tc;
        tc.name = "CheckFloatlinearXFORM: IdentityGray (1 channel)";
        bool ok = test_CheckFloatlinearXFORM_IdentityGray();
        tc.passed = ok;
        tc.message = ok ? "PASS" : "FAIL";
        tests.push_back(tc);
    }

    // Test 2: Identity transform in RGB space, 3 channels
    {
        TestCase tc;
        tc.name = "CheckFloatlinearXFORM: IdentityRGB (3 channels)";
        bool ok = test_CheckFloatlinearXFORM_IdentityRGB();
        tc.passed = ok;
        tc.message = ok ? "PASS" : "FAIL";
        tests.push_back(tc);
    }

    // Summary and non-terminating assertions (log-only for failures, but continue)
    int passed = 0;
    for (const auto &t : tests) {
        std::cout << t.name << " => " << t.message << "\n";
        if (t.passed) ++passed;
    }
    std::cout << "Summary: " << passed << " / " << tests.size() << " tests passed.\n";

    // Return non-zero if any test failed to signal failure to external harnesses (but not a hard abort)
    return (passed == (int)tests.size()) ? 0 : 1;
}