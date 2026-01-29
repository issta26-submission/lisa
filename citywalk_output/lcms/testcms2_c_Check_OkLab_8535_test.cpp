// Test suite for the focal method Check_OkLab and its key dependencies
// This test suite is written for C++11, does not use GTest, and calls the
// focal method from the C API (Little CMS). It also exercises basic
// dependency behavior (profile creation, identity transform) to improve
// coverage of the dependent components.
//
// Key Candidate Keywords mirrored from Step 1 understanding:
// - cmsCreate_OkLabProfile, cmsCreateXYZProfile, cmsCloseProfile
// - cmsCreateTransform, cmsDoTransform, cmsDeleteTransform
// - TYPE_XYZ_DBL, TYPE_OKLAB_DBL, INTENT_RELATIVE_COLORIMETRIC
// - cmsHPROFILE, cmsCIEXYZ, cmsCIELab, cmsHTRANSFORM
//
// Notes:
// - The tests use only the public C API from Little CMS to avoid private/static scope issues.
// - We avoid terminating assertions; instead, we accumulate failures and print a summary.

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <lcms2.h>
#include <cmath>


// Declaration of the focal method (from testcms2.c) to be tested.
// The function is provided in the target project; we declare it here to link against it.
extern "C" int Check_OkLab(void);

// Global test failure counter
static int g_failures = 0;

// Utility: check if a XYZ triple contains finite numbers
static bool IsFiniteXYZ(const cmsCIEXYZ& xyz) {
    // cmsFloat64Number is typically a double in lcms2
    return std::isfinite(xyz.X) && std::isfinite(xyz.Y) && std::isfinite(xyz.Z);
}

// Test 1: Validate that the focal method Check_OkLab returns 1 (success per implementation).
// This directly exercises the function under test and ensures the expected contract.
static void test_Check_OkLab_Returns_1() {
    // Purpose: Ensure the focal method reports success (returns 1)
    int ret = Check_OkLab();
    if (ret != 1) {
        ++g_failures;
        std::cerr << "[FAIL] test_Check_OkLab_Returns_1: Expected 1, got " << ret << std::endl;
    } else {
        std::cout << "[PASS] test_Check_OkLab_Returns_1" << std::endl;
    }
}

// Test 2: Basic dependency exercise: create and destroy OKLab/XYZ profiles and perform
// an identity transform between two XYZ profiles. This ensures core lib dependencies are functional
// and that memory management paths do not crash.
static void test_Dependency_BasicProfileAndIdentityTransform() {
    // Create two profiles
    cmsHPROFILE hIn  = cmsCreateXYZProfile();
    cmsHPROFILE hOut = cmsCreateXYZProfile();

    if (!hIn || !hOut) {
        ++g_failures;
        std::cerr << "[FAIL] test_Dependency_BasicProfileAndIdentityTransform: Failed to create profiles" << std::endl;
        if (hIn) cmsCloseProfile(hIn);
        if (hOut) cmsCloseProfile(hOut);
        return;
    }

    // Prepare an XYZ input
    cmsCIEXYZ in;
    in.X = 0.25; in.Y = 0.50; in.Z = 0.75;

    cmsCIEXYZ out;

    // Create an identity transform: XYZ_DBL -> XYZ_DBL
    cmsHTRANSFORM xform = cmsCreateTransform(hIn,  TYPE_XYZ_DBL, hOut, TYPE_XYZ_DBL, INTENT_RELATIVE_COLORIMETRIC, 0);
    if (!xform) {
        ++g_failures;
        std::cerr << "[FAIL] test_Dependency_BasicProfileAndIdentityTransform: Failed to create transform" << std::endl;
        cmsCloseProfile(hIn);
        cmsCloseProfile(hOut);
        return;
    }

    // Perform the transform
    cmsDoTransform(xform, &in, &out, 1);

    // Sanity check: the output should be finite
    if (!IsFiniteXYZ(out)) {
        ++g_failures;
        std::cerr << "[FAIL] test_Dependency_BasicProfileAndIdentityTransform: Output contains non-finite values" << std::endl;
    } else {
        std::cout << "[PASS] test_Dependency_BasicProfileAndIdentityTransform" << std::endl;
    }

    // Cleanup
    cmsDeleteTransform(xform);
    cmsCloseProfile(hIn);
    cmsCloseProfile(hOut);
}

// Test 3: Basic OKLab profile lifecycle: create and destroy the OKLab profile to ensure
// that the specific profile type used by the focal method is available and safe to construct.
// This indirectly exercises the dependency on the OkLab profile support.
static void test_OkLabProfile_CreateDestroy() {
    cmsHPROFILE hOkLab = cmsCreate_OkLabProfile(NULL);
    if (!hOkLab) {
        ++g_failures;
        std::cerr << "[FAIL] test_OkLabProfile_CreateDestroy: Failed to create OkLab profile" << std::endl;
        return;
    }
    cmsCloseProfile(hOkLab);
    std::cout << "[PASS] test_OkLabProfile_CreateDestroy" << std::endl;
}

// Runner to execute all tests and print a final summary
static void RunAllTests() {
    std::vector<std::string> test_names = {
        "test_Check_OkLab_Returns_1",
        "test_Dependency_BasicProfileAndIdentityTransform",
        "test_OkLabProfile_CreateDestroy"
    };

    // Dispatch by name if needed; for now we call them in order
    test_Check_OkLab_Returns_1();
    test_Dependency_BasicProfileAndIdentityTransform();
    test_OkLabProfile_CreateDestroy();

    // Summary
    std::cout << "=====================" << std::endl;
    if (g_failures == 0) {
        std::cout << "All tests PASSED (" << test_names.size() << " tests)." << std::endl;
    } else {
        std::cout << g_failures << " test(s) FAILED out of " << test_names.size() << "." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Run the test suite
    RunAllTests();
    return g_failures;
}