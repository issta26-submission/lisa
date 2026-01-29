// test_CheckOutGray.cpp
// A small, self-contained test suite for the focal function:
// cmsInt32Number CheckOutGray(cmsHTRANSFORM xform, double L, cmsUInt8Number g)
//
// This test suite uses the project's provided test harness helpers
// (from testcms2.h) to construct a Lab -> Gray transform and validate
// CheckOutGray behaviour without using GTest. It follows the required
// step-by-step guidelines: understands dependencies, generates tests
// for the focal method, and refines them for coverage.

#include <iostream>
#include <testcms2.h>
#include <cstdlib>


// Helper: run a single test and print status
static bool runTest(const char* name, bool condition) {
    if (condition) {
        std::cout << "[PASS] " << name << std::endl;
        return true;
    } else {
        std::cout << "[FAIL] " << name << std::endl;
        return false;
    }
}

int main(int argc, char* argv[]) {
    // Step 2: Unit Test Generation
    // Prepare test profiles used by the test harness. This is a typical setup
    // step in the project's tests and is required to construct transforms.
    // We rely on the provided CreateTestProfiles function to initialize
    // internal test resources. If it fails, bail out with a clear message.
    if (!CreateTestProfiles()) {
        std::cerr << "Failed to initialize test profiles (CreateTestProfiles() returned false)." << std::endl;
        return 1;
    }

    // Create source (Lab) and destination (Gray) profiles used to build a transform.
    // The test harness provides Create_GrayLab() and Create_Gray22() as helpers
    // to generate suitable profiles for Lab -> Gray transformations.
    cmsHPROFILE hLab      = Create_GrayLab(); // Profile used as input: Lab color space
    cmsHPROFILE hGrayOut  = Create_Gray22();  // Profile used as output: Gray space

    if (hLab == nullptr || hGrayOut == nullptr) {
        std::cerr << "Failed to create test profiles for Lab->Gray transform." << std::endl;
        return 1;
    }

    // Create a transform from Lab to Gray. The focal function under test
    // uses a Lab input and outputs an 8-bit Gray value. We assume correct
    // format codes TYPE_Lab_DBL and TYPE_GRAY_8, and a standard relative colorimetric
    // intent. If the library uses different macros, adjust accordingly in your env.
    cmsHTRANSFORM xform = cmsCreateTransform(hLab, TYPE_Lab_DBL, hGrayOut, TYPE_GRAY_8, INTENT_RELATIVE, 0);

    if (xform == nullptr) {
        std::cerr << "Failed to create Lab->Gray transform (cmsCreateTransform returned NULL)." << std::endl;
        cmsCloseProfile(hLab);
        cmsCloseProfile(hGrayOut);
        return 1;
    }

    // Test 1: True case for black (L = 0) should produce gray value g = 0
    // This covers the "true" branch where the computed g_out matches the requested g.
    cmsInt32Number t1 = CheckOutGray(xform, 0.0, 0); // L=0, g=0
    bool pass1 = (t1 == 1);
    runTest("CheckOutGray with L=0, g=0 (expect true)", pass1);

    // Test 2: True case for white (L = 100) should produce gray value g = 255
    // This tests an edge where L is at the upper bound. Depending on the LUT,
    // this should map to the maximum Gray value.
    cmsInt32Number t2 = CheckOutGray(xform, 100.0, 255); // L=100, g=255
    bool pass2 = (t2 == 1);
    runTest("CheckOutGray with L=100, g=255 (expect true)", pass2);

    // Test 3: False case to verify negative branch behavior.
    // If the mapping is monotonic and correct, asking for an incorrect Gray value
    // should cause CheckOutGray to return a non-1 (i.e., fail).
    cmsInt32Number t3 = CheckOutGray(xform, 0.0, 1); // L=0, g=1 (incorrect by design)
    bool pass3 = (t3 != 1);
    runTest("CheckOutGray with L=0, g=1 (expect false)", pass3);

    // Summary
    int total = 3;
    int passed = 0;
    // We rely on the booleans computed above to summarize results.
    if (pass1) ++passed;
    if (pass2) ++passed;
    if (pass3) ++passed;

    std::cout << "Test summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Cleanup
    cmsDeleteTransform(xform);
    cmsCloseProfile(hLab);
    cmsCloseProfile(hGrayOut);

    return (passed == total) ? 0 : 2;
}

// Notes and rationale for the test cases:
// - The tests rely on the project-provided test harness helpers to create Lab and Gray profiles
//   and to build a Lab->Gray transform. We then call the focal function with specific L and g
//   values to validate both successful (true) and unsuccessful (false) outcomes.
// - Test 1 and Test 2 exercise the "true" branch where the computed Gray value matches the requested
//   g (within the library's allowed tolerance). Test 3 exercises the "false" branch by providing an
//   deliberately incorrect target Gray value.
// - All tests use the project's non-terminating assertion style: we print PASS/FAIL per test and
//   continue executing the remaining tests to maximize coverage.
// - Static and private member handling is not required here since the tests interact with public API
//   provided by the library and test harness. If static state exists internally in the library, ensure
//   tests reset or isolate it as needed in your environment.